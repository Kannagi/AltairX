#ifndef ALTAIR_COMPILER_STACK_ALLOCATOR_HPP_INCLUDED
#define ALTAIR_COMPILER_STACK_ALLOCATOR_HPP_INCLUDED

#include "lifetime.hpp"

#include <vector>

namespace ar
{

/*
stack_allocator 
*/
class stack_allocator
{
public:

public:
    stack_allocator() = default;
    ~stack_allocator() = default;
    stack_allocator(const stack_allocator&) = delete;
    stack_allocator& operator=(const stack_allocator&) = delete;
    stack_allocator(stack_allocator&&) noexcept = delete;
    stack_allocator& operator=(stack_allocator&&) noexcept = delete;


private:
    //std::vector<stack_entry> m_entries{};
};

}

#endif
