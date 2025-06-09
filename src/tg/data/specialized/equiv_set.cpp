#include <algorithm>
#include <stdexcept>
#include "tg/data/specialized/equiv_set.hpp"

namespace tg::data::specialized
{

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
    const size_t sz = m_parents.size();
    if (start >= sz)
    {
        throw std::out_of_range("EquivSet::detail_follow: start index out of range");
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
        if (next >= sz)
        {
            // Value in m_parents either out of range or not initialized (detail_npos).
            throw std::out_of_range("EquivSet::detail_follow: parent index out of range");
        }
        // follow
        current = next;
    }
    return current;
}

void EquivSet::detail_relink(size_t start, size_t root)
{
    const size_t sz = m_parents.size();
    if (start >= sz)
    {
        throw std::out_of_range("EquivSet::detail_relink: start index out of range");
    }
    if (root >= sz)
    {
        throw std::out_of_range("EquivSet::detail_relink: root index out of range");
    }
    size_t current = start;
    while (current != root)
    {
        size_t& next_ref = m_parents.at(current);
        size_t next_val = next_ref;
        if (next_val >= sz)
        {
            throw std::out_of_range("EquivSet::detail_relink: parent index out of range");
        }
        next_ref = root; // relink
        current = next_val;
    }
}

void EquivSet::optimize()
{
    const size_t sz = m_parents.size();
    for (size_t i = 0; i < sz; ++i)
    {
        if (m_parents.at(i) == detail_npos)
        {
            continue; // not initialized
        }
        // relink to root
        size_t root = this->detail_follow(i);
        this->detail_relink(i, root);
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
    for (size_t item = 0; item < sz; ++item)
    {
        size_t root_val = m_parents.at(item);
        if (root_val == detail_npos)
        {
            continue; // skip over items not initialized
        }
        if (root_val == item)
        {
            roots.push_back(item);
        }
        members.emplace_back(root_val, item);
    }
    // roots already sorted.
    // members to be sorted lexicographically.
    std::sort(members.begin(), members.end());
}

} // namespace tg::data::specialized
