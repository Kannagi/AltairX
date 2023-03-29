#ifndef ALTAIR_COMPILER_STACK_ALLOCATOR_HPP_INCLUDED
#define ALTAIR_COMPILER_STACK_ALLOCATOR_HPP_INCLUDED

#include <vector>

namespace ar
{

class stack_allocator
{
public:
    struct stack_entry
    {

    };

public:
    stack_allocator();
    ~stack_allocator();
    stack_allocator(const stack_allocator&) = delete;
    stack_allocator& operator=(const stack_allocator&) = delete;
    stack_allocator(stack_allocator&&) noexcept = delete;
    stack_allocator& operator=(stack_allocator&&) noexcept = delete;

private:
    std::vector<stack_entry> m_entries{};
};

}

#endif
