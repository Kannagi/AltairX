#include "lifetime.hpp"

#include <cassert>

namespace ar
{

lifetime_range& lifetime::add_range(const lifetime_range& range)
{
    const auto pred = [](const lifetime_range& current, const lifetime_range& range)
    {
        return current.begin < range.begin;
    };

    return *coalesce(parent_type::insert(std::lower_bound(begin(), end(), range, pred), range));
}

void lifetime::remove_range(const lifetime_range& range)
{
    if(range.begin == range.end)
    {
        return;
    }

    const auto first_pred = [](const lifetime_range& current, const lifetime_range& range)
    {
        return current.end < range.begin;
    };

    const auto last_pred = [](const lifetime_range& current, const lifetime_range& range)
    {
        return current.begin < range.end;
    };

    auto first{std::lower_bound(begin(), end(), range, first_pred)};
    if(first == end())
    {
        return;
    }

    auto last{std::lower_bound(first, end(), range, last_pred) - 1};

    if(first == last) // range is within an existing range
    {
        if(first->begin == range.begin && first->end == range.end) // Remove the first range if it is entirely covered by range
        {
            erase(first);
        }
        else if(first->begin == range.begin) // Truncate the beginning
        {
            first->begin = range.end;
        }
        else if(first->end == range.end) // Truncate the end
        {
            first->end = range.begin;
        }
        else // Split it
        {
            const auto old_end{first->end};

            first->end = range.begin;
            insert(first + 1, lifetime_range{range.end, old_end});
        }
    }
    else // At least one entire range will be removed
    {
        first->end  = range.begin;
        last->begin = range.end;

        if(first->begin != first->end) // Skip the first range if it is not empty
        {
            ++first;
        }

        if(last->begin >= last->end) //The range cover entirely the last range, this may happens when last == end() - 1
        {
            ++last;
        }

        erase(first, last);
    }
}

std::size_t lifetime::total_lifetime() const noexcept
{
    std::size_t output{};

    for(auto&& range : *this)
    {
        output += range.size();
    }

    return output;
}

bool lifetime::overlap(const lifetime& other) const noexcept
{
    for(auto&& this_range : *this)
    {
        for(auto&& other_range : other)
        {
            if(this_range.overlap(other_range))
            {
                return true;
            }
        }
    }

    return false;
}

bool lifetime::overlap(const lifetime_range& range) const noexcept
{
    for(auto&& this_range : *this)
    {
        if(this_range.overlap(range))
        {
            return true;
        }
    }

    return false;
}

void lifetime::coalesce(const lifetime& other)
{
    if(std::empty(other))
    {
        return;
    }

    const auto pred = [](const lifetime_range& current, const lifetime_range& range)
    {
        return current.begin < range.begin;
    };

    auto last{begin()};
    for(auto&& range : other)
    {
        last = parent_type::insert(std::lower_bound(last, end(), range, pred), range);
    }

    coalesce_all();
}

void lifetime::remove(const lifetime& other)
{
    for(auto&& range : other)
    {
        remove_range(range);
    }
}

lifetime lifetime::holes()
{
    lifetime output{};

    if(size() > 1) //Empty or single-range lifetimes have no hole
    {
        auto it  {begin()};
        auto next{it + 1};

        while(next != end())
        {
            output.emplace_back(lifetime_range{it->end, next->begin});

            ++it;
            ++next;
        }
    }

    return output;
}

lifetime::iterator lifetime::coalesce(iterator it)
{
    if(it != begin())
    {
        const auto prev{it - 1};

        assert(prev->begin <= it->begin);
        if(prev->end >= it->begin)
        {
            it->begin = std::min(it->begin, prev->begin);
            it->end = std::max(it->end, prev->end);
            it = erase(prev);
        }
    }

    auto next{it + 1};
    while(next != end() && next->begin <= it->end)
    {
        next->begin = it->begin;
        next->end   = std::max(it->end, next->end);

        it = erase(it);
        next = it + 1;
    }

    return it;
}

void lifetime::coalesce_all()
{
    auto it  {begin()};
    auto next{it + 1};

    while(next != end())
    {
        assert(it->begin <= next->begin);
        if(next->begin <= it->end)
        {
            next->begin = it->begin;
            next->end   = std::max(it->end, next->end);

            it = erase(it);
            next = it + 1;
        }
        else
        {
            ++it;
            ++next;
        }
    }
}

}
