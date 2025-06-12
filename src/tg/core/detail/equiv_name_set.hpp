#pragma once
#include "tg/core/fwd.hpp"
#include "tg/data/specialized/unique_list.fwd.hpp"
#include "tg/data/specialized/equiv_set.hpp"

namespace tg::core::detail
{

/**
 * @brief A equivalence-set data structure for linking names.
 * 
 * @details This is a utility class for the task graph system that allows
 * treating items with different names as equivalent. It is used to connect
 * StepData instances belonging to different Step instances, so that a
 * dependency relation can be established between the step that produces it
 * and steps that use it.
 */
class EquivNameSet
{
public:
    using NameList = tg::data::specialized::UniqueList<std::string>;
    using EquivSet = tg::data::specialized::EquivSet;

public:
    /**
     * @brief Constructs an empty EquivNameSet.
     */
    EquivNameSet();

    /**
     * @brief Destructor.
     */
    ~EquivNameSet();

    /**
     * @brief Copy constructor.
     */
    EquivNameSet(const EquivNameSet& other);

    /**
     * @brief Move constructor.
     * @details Move is implemented by transferring ownership of the
     * two internal helper classes.
     */
    EquivNameSet(EquivNameSet&& other);

public:
    /**
     * @brief Inserts a name into the set.
     * 
     * @param name The name to insert.
     * @return The index of the inserted name.
     */
    size_t insert(const std::string& name);

    /**
     * @brief Links two names together in the equivalence set.
     * 
     * @param lhs The first name.
     * @param rhs The second name.
     */
    void link(const std::string& lhs, const std::string& rhs);

    /**
     * @brief Checks if a name exists in the set.
     * 
     * @param name The name to check.
     * @return True if the name exists, false otherwise.
     */
    bool contains(const std::string& name) const;

    /**
     * @brief Optimizes the equivalence set for faster lookups.
     * @details This method performs path-compression on the equivalence set.
     */
    void optimize();

    /**
     * @brief Given a name, returns all names belonging to the same equivalence
     * set.
     * 
     * @param name The name to find the representative for.
     * @param members Output vector to be filled with members names, or empty
     * if the name is not found in any sets.
     * 
     * @note The ordering of members is not guaranteed stable if EquivNameSet
     * is modified.
     */
    void get_members(const std::string& name, std::vector<std::string>& members) const;

private:
    /**
     * @brief Deleted move constructor and assignment operators.
     * @details By design, all destructive operations on an already-constructed
     * instance have to be forbidden, since they cannot be safely supported
     * due to limitations of the underlying data structures.
     */
    EquivNameSet& operator=(const EquivNameSet&) = delete;
    EquivNameSet& operator=(EquivNameSet&&) = delete;

private:
    std::shared_ptr<NameList> m_names;
    std::shared_ptr<EquivSet> m_equiv_set;
};

} // namespace tg::core::detail
