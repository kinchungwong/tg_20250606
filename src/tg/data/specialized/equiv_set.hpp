#pragma once
#include <cstdint>
#include <climits>
#include <vector>

namespace tg::data::specialized
{

/**
 * @brief An equivalence set implementation.
 *
 * @details This class implements a disjoint-set data structure using a 
 * basic algorithm (parent pointer tree). This implementation is suitable
 * for small to medium-sized sets, intended for use in the task graph
 * framework.
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
     * @brief Insert a new item into the equivalence set.
     * @note If the item is already present, this method does nothing.
     * @note Items not explicitly inserted or linked are considered
     * uninitialized (not part of any set).
     */
    void insert(size_t item);

    /**
     * @brief Link two items together in the equivalence set.
     * @note The two items will be automatically inserted by this method.
     */
    void link(size_t lhs, size_t rhs);

    /**
     * @brief Optimize all items so that they point directly to their root.
     */
    void optimize();

    /**
     * @brief Exports all equivalence sets as a list of (root, member) pairs.
     * @param roots Vector of root elements.
     * @param members Vector of (root, member) pairs, lexicographically sorted.
     * @note Uninitialized items (items never inserted or linked) are not
     * included in the output.
     * @note This method calls optimize() automatically.
     */
    void export_sorted(
        std::vector<size_t>& roots,
        std::vector<std::pair<size_t, size_t>>& members);

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
