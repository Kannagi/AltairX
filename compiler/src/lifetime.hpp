#ifndef ALTAIR_COMPILER_LIFETIME_HPP_INCLUDED
#define ALTAIR_COMPILER_LIFETIME_HPP_INCLUDED

#include <cstdint>
#include <vector>
#include <algorithm>
#include <ostream>
#include <iomanip>

namespace ar
{

struct lifetime_range
{
    std::size_t begin{};
    std::size_t end{};

    std::size_t size() const noexcept
    {
        return end - begin;
    }

    bool overlap(const lifetime_range& other) const noexcept
    {
        return (begin < other.begin && end > other.begin) || (other.begin < begin && other.end > begin);
    }
};

struct lifetime : private std::vector<lifetime_range>
{
    using parent_type = std::vector<lifetime_range>;

public:
    using value_type             = parent_type::value_type;
    using allocator_type         = parent_type::allocator_type;
    using size_type              = parent_type::size_type;
    using difference_type        = parent_type::difference_type;
    using reference              = parent_type::reference;
    using const_reference        = parent_type::const_reference;
    using pointer                = parent_type::pointer;
    using const_pointer          = parent_type::const_pointer;
    using iterator               = parent_type::iterator;
    using const_iterator         = parent_type::const_iterator;
    using reverse_iterator       = parent_type::reverse_iterator;
    using const_reverse_iterator = parent_type::const_reverse_iterator;

public:
    lifetime() = default;
    ~lifetime() = default;
    lifetime(const lifetime&) = default;
    lifetime(lifetime&&) = default;
    lifetime& operator=(const lifetime&) = default;
    lifetime& operator=(lifetime&&) = default;

    using parent_type::data;
    using parent_type::empty;
    using parent_type::size;

    using parent_type::begin;
    using parent_type::end;
    using parent_type::cbegin;
    using parent_type::cend;
    using parent_type::rbegin;
    using parent_type::rend;
    using parent_type::crbegin;
    using parent_type::crend;

    using parent_type::clear;
    using parent_type::erase;
    using parent_type::reserve;
    using parent_type::capacity;

    using parent_type::front;
    using parent_type::back;
    using parent_type::operator[];

    lifetime_range& add_range(const lifetime_range& range);
    void remove_range(const lifetime_range& range);

    lifetime_range& add_range(std::size_t begin, std::size_t end)
    {
        return add_range(lifetime_range{begin, end});
    }

    void remove_range(std::size_t begin, std::size_t end)
    {
        return remove_range(lifetime_range{begin, end});
    }

    lifetime_range range() const noexcept
    {
        return lifetime_range{front().begin, back().end};
    }

    std::size_t total_lifetime() const noexcept;

    std::size_t total_span() const noexcept
    {
        return back().end - front().begin;
    }

    bool overlap(const lifetime& other) const noexcept;
    bool overlap(const lifetime_range& range) const noexcept;

    void coalesce(const lifetime& other);

    lifetime holes();

private:
    iterator coalesce(iterator it);
    void coalesce_all();
};

inline std::ostream& operator<<(std::ostream& os, const lifetime& lt)
{
    for(auto&& range : lt)
    {
        os << "[" << range.begin << "; " << range.end << "]";
    }

    return os;
}

}

#endif
