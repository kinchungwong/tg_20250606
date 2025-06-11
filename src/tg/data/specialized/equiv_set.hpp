#pragma once
#include <cstddef>
#include <cstdint>
#include <climits>
#include <vector>

namespace tg::data::specialized
{

/**
 * @brief Basic implementation of an equivalence-set (disjoint-set) 
 * data structure.
 *
 * @details This class implements a disjoint-set data structure using a
 * simple algorithm (parent pointer tree).
 * 
 * Items can be inserted and linked together, forming equivalence sets.
 * 
 * Items (size_t values) which are never inserted or linked are non-members,
 * that is, they are not considered part of any equivalence set.
 * 
 * For algorithm usability reasons, skipped-over values and out-of-range
 * values are both treated as non-members.
 * 
 * This implementation is kept simple for small to medium-sized sets,
 * intended for use in the task graph framework. Advanced disjoint-set
 * algorithm options (such as union by rank or path compression) are not
 * implemented.
 * 
 * @note Not thread-safe.
 */
class EquivSet
{
public:
    EquivSet() = default;
    ~EquivSet() = default;
    EquivSet(const EquivSet&) = default;
    EquivSet(EquivSet&&) = default;
    EquivSet& operator=(const EquivSet&) = default;
    EquivSet& operator=(EquivSet&&) = default;

public:
    /**
     * @brief Check if the equivalence set contains the given item.
     */
    bool contains(size_t item) const;

    /**
     * @brief Insert an item into the equivalence set if not already so.
     */
    void insert(size_t item);

    /**
     * @brief Link two items together in the equivalence set.
     * @note The two items will be automatically inserted by this method.
     *
     * @details Internally, this method performs path-compression on one
     * of the items, using a two-pass traversal.
     */
    void link(size_t lhs, size_t rhs);

    /**
     * @brief Optimize all items so that they point directly to their root.
     *
     * @details Internally, path-compression is performed on all items.
     */
    void optimize();

    /**
     * @brief Exports all equivalence sets as a list of (root, member) pairs.
     * @param roots Vector of root elements.
     * @param members Vector of (root, member) pairs, lexicographically sorted.
     *
     * @note optimize() is called automatically.
     */
    void export_sorted(
        std::vector<size_t>& roots,
        std::vector<std::pair<size_t, size_t>>& members);

    /**
     * @brief Exports a single equivalence set which contains the
     * given item.
     *
     * @param item An item belonging to the equivalence set to be exported.
     *
     * @param members Output vector to be filled with members of the
     * equivalence set containing the given item, or empty if the given item
     * is a non-member.
     *
     * @note This method does not call optimize(). However, it will run
     * faster if the set is already optimized.
     */
    void export_one(size_t item, std::vector<size_t>& members) const;

private:
    /**
     * @brief Follow the parent pointers to find the root of the set.
     */
    size_t detail_follow(size_t start) const;

    /**
     * @brief Update "start" and all parents to relink to "root".
     * @note If the root of "start" is not "root", it is unconditionally
     * relinked to "root".
     */
    void detail_relink(size_t start, size_t root);

private:
    static constexpr size_t detail_npos = ~static_cast<size_t>(0);
    static constexpr size_t max_item = static_cast<size_t>(INT_MAX - 1);

    /**
     * @brief Parent pointer tree.
     * @details An element's parent may be: itself, another element, or detail_npos
     * if the item is not explicitly inserted via a call to insert(item).
     * @ref https://en.wikipedia.org/wiki/Disjoint-set_data_structure#Representation
     */
    std::vector<size_t> m_parents;
};

} // namespace tg::data::specialized
