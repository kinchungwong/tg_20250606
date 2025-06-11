#include <algorithm>
#include <stdexcept>
#include "tg/data/specialized/equiv_set.hpp"

namespace tg::data::specialized
{

bool EquivSet::contains(size_t item) const
{
    if (item >= m_parents.size())
    {
        return false;
    }
    return m_parents.at(item) != detail_npos;
}

void EquivSet::insert(size_t item)
{
    if (item >= max_item)
    {
        throw std::out_of_range("EquivSet::insert: item index exceeded maximum limit");
    }
    if (item >= m_parents.size())
    {
        m_parents.resize(item + 1u, detail_npos);
    }
    // assert(item < m_parents.size())
    size_t& ref_item = m_parents.at(item);
    if (ref_item == detail_npos)
    {
        // initialize item as set of one (singleton)
        ref_item = item;
    }
    // otherwise, item is already initialized
}

void EquivSet::link(size_t lhs, size_t rhs)
{
    this->insert(lhs);
    if (lhs == rhs)
    {
        // no-op
        return; 
    }
    this->insert(rhs);
    size_t root_lhs = this->detail_follow(lhs);
    size_t root_rhs = this->detail_follow(rhs);
    if (root_lhs < root_rhs)
    {
        // relink rhs to lhs
        this->detail_relink(rhs, root_lhs);
    }
    else if (root_rhs < root_lhs)
    {
        // relink lhs to rhs
        this->detail_relink(lhs, root_rhs);
    }
}

size_t EquivSet::detail_follow(size_t start) const
{
    if (!contains(start))
    {
        return detail_npos; // not initialized
    }
    size_t current = start;
    while (true)
    {
        size_t next = m_parents.at(current);
        if (current == next)
        {
            // found
            break;
        }
        if (!contains(next))
        {
            throw std::out_of_range((
                "EquivSet::detail_follow: "
                "inconsistent state, "
                "valid current points to invalid parent."
            ));
        }
        // follow
        current = next;
    }
    return current;
}

void EquivSet::detail_relink(size_t start, size_t root)
{
    if (!contains(start))
    {
        throw std::out_of_range("EquivSet::detail_relink: start index out of range");
    }
    if (!contains(root))
    {
        throw std::out_of_range("EquivSet::detail_relink: root index out of range");
    }
    size_t current = start;
    while (current != root)
    {
        size_t& next_ref = m_parents.at(current);
        size_t next_val = next_ref;
        if (!contains(next_val))
        {
            throw std::out_of_range((
                "EquivSet::detail_relink: "
                "inconsistent state, "
                "valid current points to invalid parent."
            ));
        }
        next_ref = root; // relink
        current = next_val;
    }
}

void EquivSet::optimize()
{
    const size_t sz = m_parents.size();
    for (size_t current = 0; current < sz; ++current)
    {
        if (!contains(current))
        {
            continue; // skip uninitialized items
        }
        // relink to root
        size_t root = this->detail_follow(current);
        this->detail_relink(current, root);
    }
}

void EquivSet::export_sorted(
    std::vector<size_t>& roots,
    std::vector<std::pair<size_t, size_t>>& members)
{
    this->optimize(); // ensure all items are optimized before exporting
    roots.clear();
    members.clear();
    const size_t sz = m_parents.size();
    // This may over-allocate due to non-initialized members that will
    // not be exported; this is considered okay.
    members.reserve(sz);
    for (size_t current = 0; current < sz; ++current)
    {
        size_t root_val = m_parents.at(current);
        if (root_val == detail_npos)
        {
            continue; // skip over items not initialized
        }
        if (root_val == current)
        {
            roots.push_back(current);
        }
        members.emplace_back(root_val, current);
    }
    // roots already sorted.
    // members to be sorted lexicographically.
    std::sort(members.begin(), members.end());
}

void EquivSet::export_one(size_t item, std::vector<size_t>& members) const
{
    members.clear();
    if (item >= max_item)
    {
        throw std::out_of_range("EquivSet::export_one: item index exceeded maximum limit");
    }
    size_t item_root = this->detail_follow(item);
    if (item_root == detail_npos)
    {
        // item not in any set
        return;
    }
    const size_t sz = m_parents.size();
    for (size_t other = 0; other < sz; ++other)
    {
        size_t other_root = this->detail_follow(other);
        if (other_root == item_root)
        {
            members.push_back(other);
        }
    }
}

} // namespace tg::data::specialized
