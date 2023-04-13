#include "mid_transforms.hpp"

#include <iostream>
#include <algorithm>
#include <numeric>

#include <llvm/IR/Constants.h>
#include <llvm/ADT/PostOrderIterator.h>

#include "intrinsic.hpp"
#include "utilities.hpp"

namespace ar::mid_transforms
{

// `is_signed` is for the `size` of the imm
static llvm::Value* insert_constant_int(llvm::Module& module, llvm::ConstantInt* value, std::size_t size, bool is_signed, llvm::Instruction* position)
{
    const auto compose_int = [&](std::uint64_t val)
    {
        auto low_word{llvm::ConstantInt::get(llvm::Type::getInt64Ty(module.getContext()), val & 0xFFFFull)};

        llvm::Instruction* last{insert_moveu_intrinsic(module, low_word, position)};

        for(std::uint64_t i{1}; i < 4; ++i)
        {
            const std::uint64_t bit_shift{16 * i};

            if(const std::uint64_t current{(val & (0xFFFFull << bit_shift)) >> bit_shift}; current != 0)
            {
                auto word {llvm::ConstantInt::get(llvm::Type::getInt64Ty(module.getContext()), current)};
                auto shift{llvm::ConstantInt::get(llvm::Type::getInt64Ty(module.getContext()), i)};

                last = insert_smove_intrinsic(module, last, word, shift, position);
            }
        }

        return last;
    };

    constexpr std::uint64_t moveu_max{(1ull << 18) - 1};
    constexpr std::int64_t  moven_min{-(1ll << 18)};
    // op codes are 32-bit so it will always fit
    const std::uint64_t inst_max{(1ull << (size - static_cast<std::int64_t>(is_signed))) - 1};
    const std::int64_t  inst_min{is_signed ? -static_cast<std::int64_t>(inst_max) - 1 : 0ll};

    if(value->isNegative())
    {
        const std::int64_t val{value->getSExtValue()};

        if(val >= inst_min && size > 0) //fit in size: do nothing
        {
            return value;
        }
        else if(val >= moven_min) // fit in moves
        {
            return insert_moven_intrinsic(module, value, position);
        }
        else
        {
            return compose_int(static_cast<std::uint64_t>(val));
        }
    }
    else
    {
        const std::uint64_t val{value->getZExtValue()};

        if(val <= inst_max && size > 0) //fit in size: do nothing
        {
            return value;
        }
        else if(val <= moveu_max) // fit in moveu
        {
            return insert_moveu_intrinsic(module, value, position);
        }
        else
        {
            return compose_int(val);
        }
    }
}

void insert_move_for_constant(register_allocator& allocator)
{
    auto& module{allocator.module()};

    for(llvm::BasicBlock& block : allocator.function())
    {
        for(llvm::Instruction& instruction : block)
        {
            //filter intrinsics to prevent altair.move(iX X) from generating infinite loops

            if(auto phi{llvm::dyn_cast<llvm::PHINode>(&instruction)}; phi)
            {
                // Insert move in predecessor if the phi value when comming from this predecessor is a contant

                for(llvm::BasicBlock* predecessor : phi->blocks())
                {
                    if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(phi->getIncomingValueForBlock(predecessor))}; value)
                    {
                        phi->setIncomingValueForBlock(predecessor, insert_constant_int(module, value, 0, false, predecessor->getTerminator()));
                    }

                    //add float, bool and pointer support
                }
            }
            else if(llvm::CallInst* call{llvm::dyn_cast<llvm::CallInst>(&instruction)}; call)
            {
                switch(get_intrinsic_id(*call))
                {
                case intrinsic_id::unknown:
                    for(std::uint32_t i{}; i < call->arg_size(); ++i)
                    {
                        if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(call->getArgOperand(i))}; value)
                        {
                            call->setArgOperand(i, insert_constant_int(module, value, 0, false, call));
                        }
                    }
                    break;

                case intrinsic_id::load:
                    if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(call->getArgOperand(1))}; value)
                    {
                        call->setArgOperand(1, insert_constant_int(module, value, 10, true, call));
                    }
                    break;

                case intrinsic_id::store:
                    if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(call->getArgOperand(1))}; value)
                    {
                        call->setArgOperand(1, insert_constant_int(module, value, 10, true, call));
                    }
                    if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(call->getArgOperand(2))}; value)
                    {
                        call->setArgOperand(2, insert_constant_int(module, value, 0, false, call));
                    }
                    break;

                default:
                    break;
                }
            }
            else if(llvm::BinaryOperator* binary{llvm::dyn_cast<llvm::BinaryOperator>(&instruction)}; binary)
            {
                if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(binary->getOperand(0))}; value)
                {
                    binary->setOperand(0, insert_constant_int(module, value, 0, false, binary));
                }

                if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(binary->getOperand(1))}; value)
                {
                    switch(binary->getOpcode())
                    {
                    case llvm::BinaryOperator::BinaryOps::SDiv: [[fallthrough]];
                    case llvm::BinaryOperator::BinaryOps::SRem:
                        binary->setOperand(1, insert_constant_int(module, value, 9, true, binary));
                        break;
                    default:
                        binary->setOperand(1, insert_constant_int(module, value, 9, false, binary));
                        break;
                    }
                }
            }
            else if(llvm::StoreInst* store{llvm::dyn_cast<llvm::StoreInst>(&instruction)}; store)
            {
                if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(store->getOperand(0))}; value)
                {
                    store->setOperand(0, insert_constant_int(module, value, 0, false, store));
                }
            }
            else if(llvm::ReturnInst* ret{llvm::dyn_cast<llvm::ReturnInst>(&instruction)}; ret)
            {
                if(ret->getReturnValue())
                {
                    if(llvm::ConstantInt* value{llvm::dyn_cast<llvm::ConstantInt>(ret->getReturnValue())}; value)
                    {
                        ret->setOperand(0, insert_constant_int(module, value, 0, false, ret));
                    }
                }
            }
       }
    }

    allocator.perform_analysis();
}

void insert_move_for_global_load(register_allocator& allocator)
{
    auto& module{allocator.module()};
    auto& function{allocator.function()};

    std::unordered_map<llvm::Value*, llvm::SmallVector<llvm::Instruction*>> constant_users;

    for(llvm::GlobalVariable& global : module.globals())
    {
        for(llvm::User* user : global.users())
        {
            if(allocator.index_of(user) != register_allocator::invalid_index)
            {
                auto it{constant_users.find(&global)};
                if(it == std::end(constant_users))
                {
                    it = constant_users.emplace(&global, llvm::SmallVector<llvm::Instruction*>{}).first;
                }

                it->second.emplace_back(llvm::dyn_cast<llvm::Instruction>(user));
            }
        }
    }

    // For now the algorithm just extract constant use out of loops
    // IMPROVEMENT: composing a constant address take two ALU cycle which could be done on channel two
    // or just pipelined enough to have no impact on performances. This will lesser register pressure.
    for(auto& constant_user : constant_users)
    {
        std::size_t first_user_index{std::numeric_limits<std::size_t>::max()};

        for(auto user : constant_user.second)
        {
            const auto user_index{allocator.index_of(user)};
            assert(user_index != register_allocator::invalid_index && "Broken logic, all users must already have been checked as members of this function.");
            first_user_index = std::min(first_user_index, user_index);
        }

        auto& first_user {allocator.values()[first_user_index]};
        auto& first_block{allocator.blocks()[first_user.block]};
        auto& first_scc  {allocator.sccs()[first_block.scc]};

        const auto insert_address = [&module, &constant_user](llvm::Instruction* position)
        {
            llvm::Instruction* low{insert_moveu_intrinsic(module, constant_user.first, position)};
            auto shift{llvm::ConstantInt::get(llvm::Type::getInt64Ty(module.getContext()), 1)};

            return insert_smove_intrinsic(module, low, constant_user.first, shift, position);
        };

        llvm::Value* constant{};

        if(first_scc.loop) // first use is in a loop, extract it out of it
        {
            auto& loop{allocator.info_of(first_scc.loop)};

            if(loop.predecessors.size() == 1)
            {
                llvm::BasicBlock* pred{loop.predecessors.front()};
                constant = insert_address(pred->getTerminator());
            }
            else
            {
                // IMPROVEMENT: for now constant is placed in first block to ensure that multiple entries loop
                // will always have the right value.
                // Assume entry block is never in a loop
                constant = insert_address(function.getEntryBlock().getTerminator());
            }
        }
        else // insert constant before first user
        {
            constant = insert_address(llvm::cast<llvm::Instruction>(first_user.value));
        }

        for(llvm::Instruction* user : constant_user.second)
        {
            user->replaceUsesOfWith(constant_user.first, constant);
        }
    }

    allocator.perform_analysis();
}

static std::size_t latency_of(llvm::Instruction* inst)
{
    if(llvm::isa<llvm::TruncInst>(inst) || llvm::isa<llvm::ZExtInst>(inst))
    {
        return 1;
    }

    return 3;
}
/*
    const auto print_dag = [&]()
    {
        const auto n{range_size};

        std::vector<std::size_t> roots;
        for(std::size_t i = 0; i < n; i++) {
            if(vertices[i].in_degree == 0) {
                roots.push_back(i);
            }
        }

        // iterate over the graph
        for(int i = 0; i < roots.size(); i++) {
            std::size_t root = roots[i];
            std::cout << "Starting from root " << from_dag(root) << ":" << std::endl;
            std::vector<bool> visited(n, false);
            std::vector<std::size_t> queue;
            queue.push_back(root);
            visited[root] = true;
            while(!queue.empty()) {
                std::size_t curr_vertex = queue.front();
                queue.erase(queue.begin());
                std::cout << "Visiting vertex " << from_dag(curr_vertex) << std::endl;
                for(std::size_t j = 0; j < dag[curr_vertex].size(); j++) {
                    std::size_t adj_vertex = dag[curr_vertex][j];
                    if(!visited[adj_vertex]) {
                        visited[adj_vertex] = true;
                        queue.push_back(adj_vertex);
                    }
                }
            }
        }
    };

    const auto print_dag2 = [&]()
    {
        using namespace std;

        int n = dag.size();
        vector<int> in_degrees(n, 0);
        vector<vector<int>> levels;
        queue<int> q;

        // calculate in-degrees of each vertex and add root nodes to queue
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < dag[i].size(); j++) {
                int adj_vertex = dag[i][j];
                in_degrees[adj_vertex]++;
            }
        }
        for(int i = 0; i < n; i++) {
            if(in_degrees[i] == 0) {
                q.push(i);
            }
        }

        // perform breadth-first search and print each level of the graph
        while(!q.empty()) {
            int size = q.size();
            vector<int> level;
            for(int i = 0; i < size; i++) {
                int curr_vertex = q.front();
                q.pop();
                level.push_back(curr_vertex);
                for(int j = 0; j < dag[curr_vertex].size(); j++) {
                    int adj_vertex = dag[curr_vertex][j];
                    if(--in_degrees[adj_vertex] == 0) {
                        q.push(adj_vertex);
                    }
                }
            }
            levels.push_back(level);
        }

        // print the graph in the intuitive layout
        for(int i = 0; i < levels.size(); i++) {
            cout << "Level " << i << ":";
            for(int j = 0; j < levels[i].size(); j++) {
                int vertex = levels[i][j];
                cout << " " << from_dag(vertex);
            }
            cout << endl;
        }
    };*/

static std::vector<std::size_t> optimize_range(register_allocator& allocator, const register_allocator::block_info& block, std::size_t begin, std::size_t end)
{
    /*
    look for instruction that depends on "external values"
        external values : a value that is defined out of the range
        example : func args, phi nodes, values defined before a side effect etc
        these instructions are DAG head nodes
    */

    const auto range_size{end - begin};

    struct vertex_info
    {
        std::size_t latency{3}; // latency of this instruction, decremented when assigning an instruction to output
        std::size_t in_degree{};
        bool visited{};
    };

    const auto to_dag = [begin](std::size_t index)
    {
        return index - begin;
    };

    const auto from_dag = [begin](std::size_t index)
    {
        return begin + index;
    };

    std::vector<vertex_info> vertices{};
    vertices.resize(range_size);
    std::vector<llvm::SmallVector<std::size_t, 8>> dag{};
    dag.resize(range_size);

    const auto push_dag = [&](std::size_t from, std::size_t to)
    {
        dag[to_dag(from)].emplace_back(to_dag(to));
        vertices[to_dag(to)].in_degree += 1;
    };

    // construct DAG
    // In the DAG, a dependency "FROM x TO y" means that y depends on x
    // Example:
    // %23 = shl nuw nsw i32 %22, 12
    // %24 = or i32 %23, %16
    // In this example, a dependency "FROM %23 to %24" will be defined
    for(std::size_t value_index{begin}; value_index < end; ++value_index)
    {
        const auto& value_info{allocator.values()[value_index]};
        auto* inst{llvm::cast<llvm::Instruction>(value_info.value)};
        vertices[to_dag(value_index)].latency = latency_of(inst);

        for(const llvm::Use& op : inst->operands())
        {
            const auto op_index{allocator.index_of(op.get())};
            if(op_index == register_allocator::invalid_index) // constants...
            {
                // IMPROVEMENT: still store useful informations if any ?
                continue;
            }

            // an operand is a phi node value:
            // we add a dependency of any phi node incomming values than are 
            // part of current leaf range
            // Example:
            // %8 = phi i64[%53, %6], [0, %3]
            // %9 = getelementptr inbounds i8, ptr %0, i64 %8
            // %53 = add nuw i64 %8, 4
            // will add a dependency from %9 to %53 to ensure that the addition is not performed 
            // before the getelementptr
            if(llvm::isa<llvm::PHINode>(op))
            {
                auto& op_info{allocator.values()[op_index]};
                if(op_info.group != value_info.group)
                {
                    auto& group{allocator.groups()[op_info.group]};
                    if(group.type == register_allocator::group_type::phi)
                    {
                        for(llvm::Value* member : group.members)
                        {
                            const auto member_index{allocator.index_of(member)};
                            if(member_index == register_allocator::invalid_index)
                            {
                                continue;
                            }

                            if(value_index <= member_index && member_index < end)
                            {
                                // IMPROVEMENT: note that latency should not apply to this dependency ?
                                push_dag(value_index, member_index);
                            }
                        }
                    }
                }
            }

            // TODO: Add guarantees for cmp and block terminators

            // Operand is in range
            // We add a dependency from the operand to the current instruction
            if(begin <= op_index && op_index < end)
            {
                push_dag(op_index, value_index);
            }
        }
    }

    // new_order[i] = allocator.values() index
    std::vector<std::size_t> new_order{};
    new_order.reserve(range_size);
    // this is the queue with all the candite that can be place to the list
    std::vector<std::size_t> queue{};
    queue.reserve(range_size);

    // Initial roots
    for(std::size_t i{}; i < range_size; i++) 
    {
        if(vertices[i].in_degree == 0) 
        {
            // root have default to 0 latency
            // IMPROVEMENT: make more advanced analysis to enable more accurate latency for root vertices
            vertices[i].latency = 0;
            queue.emplace_back(i);
        }
    }

    const auto find_best_candidate = [&queue, &vertices]()
    {
        const auto it = std::min_element(std::begin(queue), std::end(queue), [&vertices](std::size_t vertex, std::size_t smallest)
        {
            return vertices[vertex].latency < vertices[smallest].latency;
        });

        // more advanced checks ?
        const auto output{*it};
        queue.erase(it);

        return output;
    };

    while(!std::empty(queue))
    {
        // choose best candidate in queue
        const auto best_candidate{find_best_candidate()};
        new_order.emplace_back(begin + best_candidate);
        vertices[best_candidate].visited = true;

        for(auto dependee : dag[best_candidate])
        {
            vertices[dependee].in_degree -= 1;
            if(vertices[dependee].in_degree == 0)
            {
                queue.push_back(dependee);
            }
        }

        for(auto& vertex : vertices)
        {
            if(!vertex.visited && vertex.latency > 0)
            {
                vertex.latency -= 1;
            }
        }
    }

    return new_order;
}

void optimize_pipeline(register_allocator& allocator)
{
    std::vector<std::size_t> new_order{};

    for(auto& block : allocator.blocks())
    {
        new_order.clear();
        new_order.reserve(block.end - block.begin_no_phi);

        std::size_t leaf_begin{block.begin_no_phi};
        for(std::size_t i{block.begin_no_phi}; i < block.end; ++i)
        {
            if(!allocator.values()[i].leaf)
            {
                if(i - leaf_begin > 1)
                {
                    auto range_new_order{optimize_range(allocator, block, leaf_begin, i)};
                    new_order.insert(std::end(new_order), std::begin(range_new_order), std::end(range_new_order));
                }

                new_order.emplace_back(i);

                leaf_begin = i + 1;
            }
        }

        if(block.end - 1 - leaf_begin > 1)
        {
            // exclude terminator
            auto range_new_order{optimize_range(allocator, block, leaf_begin, block.end - 1)};
            new_order.insert(std::end(new_order), std::begin(range_new_order), std::end(range_new_order));
        }
        else
        {
            new_order.emplace_back(block.end - 2);
        }

        const auto old_index_of = [&new_order, begin = block.begin_no_phi](std::size_t new_index)
        {
            const auto it{std::find(std::begin(new_order), std::end(new_order), new_index)};
            assert(it != std::end(new_order));
        
            return begin + std::distance(std::begin(new_order), it);
        };

        llvm::Instruction* position{block.block->getTerminator()};
        const auto index_offset{block.end - 1};
        for(std::size_t i{1}; i < std::size(new_order); ++i)
        {
            const auto old_index{new_order[index_offset - block.begin_no_phi - i]};
            auto current{llvm::cast<llvm::Instruction>(allocator.values()[old_index].value)};
            current->moveBefore(position);
            position = current;
        }
    }

    allocator.perform_analysis();
}

void optimize_pipeline2(register_allocator& allocator)
{
    // Reorder instruction based on their latency to maximize instructions throughput
    // Only leaf instructions can be moved anywhere between the previous and the following non leaf instruction
    // Assign instruction to be executed on secondary instruction

    std::vector<std::pair<std::size_t, std::size_t>> leaf_ranges{};
    std::vector<std::size_t> new_indices{};

    for(auto& block : allocator.blocks())
    {
        const auto block_begin{block.begin_no_phi};
        const auto block_end{block.end};

        leaf_ranges.clear();
        leaf_ranges.reserve(block_end - block_begin);

        std::size_t leaf_begin{block_begin};
        for(std::size_t i{block_begin}; i < block_end; ++i)
        {
            if(!allocator.values()[i].leaf)
            {
                if(leaf_begin != i)
                {
                    leaf_ranges.emplace_back(leaf_begin, i);
                }

                leaf_begin = i + 1;
            }
        }

        if(leaf_begin != block_end)
        {
            leaf_ranges.emplace_back(leaf_begin, block_end);
        }

        // generate initial indices map (identity)
        new_indices.clear();
        new_indices.resize(block_end - block_begin);
        std::iota(std::begin(new_indices), std::end(new_indices), block_begin);

        // old_index -> new_index
        const auto index_of = [&new_indices, offset = block_begin](std::size_t old_index)
        {
            return new_indices[old_index - offset];
        };

        const auto old_index_of = [&new_indices, offset = block_begin](std::size_t new_index)
        {
            const auto it{std::find(std::begin(new_indices), std::end(new_indices), new_index)};
            assert(it != std::end(new_indices));

            return offset + std::distance(std::begin(new_indices), it);
        };

        for(const auto& range : leaf_ranges)
        {
            static constexpr std::size_t max_passes{16};
            for(std::size_t pass{}; pass < max_passes; ++pass)
            {
                for(std::size_t i{range.first}; i < range.second; ++i)
                {
                    const auto value_index{index_of(i)};
                    if(value_index == block_begin)
                    {
                        continue; // first instruction can not be moved before
                    }

                    auto& value{allocator.values()[value_index]};
                    if (llvm::isa<llvm::PHINode>(value.value))
                    {
                        continue; // phi nodes can not be moved
                    }

                    std::size_t closest_user_distance{std::numeric_limits<std::size_t>::max()};
                    for(std::size_t new_index{value_index + 1}; new_index < range.second; ++new_index)
                    {
                        const auto user_index{old_index_of(new_index)};
                        if(value.usage[user_index])
                        {
                            closest_user_distance = std::min(closest_user_distance, new_index - value_index);
                        }
                    }

                    if(closest_user_distance == std::numeric_limits<std::size_t>::max())
                    {
                        continue; // not used (forward) in this block, don't move ignore
                    }

                    auto instruction{llvm::cast<llvm::Instruction>(value.value)};
                    // IMPROVEMENT: assume latency of 3 cycles for all instructions, change it to the real per instruction latency
                    std::size_t latency{4};

                    if(closest_user_distance < latency) // try to improve distance with closest user
                    {
                        std::size_t closest_dependency_distance{latency};

                        // check if we can move it before (i.e. does not depends of something defined just before)
                        // broken for groups (phi node for sure)
                        // because phi nodes "return" is a distinc variable but the same register at the end which cases wrong optimization
                        // propagate all group usage/user ?
                        for(const llvm::Use& operand : instruction->operands())
                        {
                            // check for main value
                            const auto operand_old_index{allocator.index_of(operand.get())};
                            if(operand_old_index == register_allocator::invalid_index || operand_old_index < range.first || operand_old_index >= range.second)
                            {
                                continue;
                            }

                            const auto operand_index{index_of(operand_old_index)};
                            if(range.first <= operand_index && operand_index < value_index)
                            {
                                closest_dependency_distance = std::min(closest_dependency_distance, value_index - operand_index);
                            }
                        }

                        if(value.group != register_allocator::no_group)
                        {
                            const auto& group{allocator.groups()[value.group]};

                            for(auto member : group.members)
                            {
                                if(!llvm::isa<llvm::PHINode>(member))
                                {
                                    continue; // ignore non phi node as they will never be placed before definition
                                }

                                const auto phi_index{allocator.index_of(member)};
                                const auto phi_info{allocator.info_of(member)};
                                if(phi_info.block != allocator.index_of(block))
                                {
                                    continue; // not a "loop" phi node
                                }

                                std::size_t furthest_phi_user_index{0};
                                for(std::size_t j{block_begin}; j < value_index; ++j)
                                {
                                    const auto user_index{index_of(j)};
                                    if(phi_info.usage[user_index])
                                    {
                                        furthest_phi_user_index = std::max(furthest_phi_user_index, user_index);
                                    }
                                }
                            
                                closest_dependency_distance = std::min(closest_dependency_distance, value_index - furthest_phi_user_index);
                            }
                        }

                        if(closest_dependency_distance > value_index - block_begin)
                        {
                            closest_dependency_distance = value_index - block_begin; // can not go further than the block beginning
                        }

                        if(closest_dependency_distance > 1)
                        {
                            // move value `closest_dependency_distance` further from its closest user
                            closest_dependency_distance -= 1; // don't move it to actual position of closest dependency
                            const auto new_position{std::max(block_begin, value_index - closest_dependency_distance)};

                            for(auto& index : new_indices)
                            {
                                if(index >= new_position && index < value_index)
                                {
                                    ++index;
                                }
                            }

                            new_indices[i - block_begin] = new_position;
                        }
                    }
                }
            }
        }

    }

    allocator.perform_analysis();
}

}
