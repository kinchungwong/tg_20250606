#pragma once
#include "tg/core/fwd.hpp"
#include "tg/core/dag/scoped_data_info_tuple.hpp" // for use within DataList.
#include "tg/data/specialized/equiv_set.hpp"

namespace tg::core::dag
{
    // Forward declarations.
    class ScopedDataInfoTuple;
    class DataList;
}

namespace tg::core::dag
{

/**
  * @brief A list of data items gathered from a collection of steps,
  * to allow the binding of outputs and inputs between them.
  * 
  * @details DataList accepts additions of Scopes and Steps,
  * and upon adding the data defined in each Step will be indexed.
  * 
  * Then, several data binding directives can be applied to connect
  * the inputs and outputs. Firstly, for Steps passed in via a
  * Scope, it will be checked for several lists regarding data:
  * 
  * - Data binding by name: when multiple Steps belonging to the
  * same Scope give the same name to their data, and if that name
  * also appears on the Scope's "data binding by name" list, they
  * are bound together.
  * 
  * - Explicit data binding: Additional bindings can be added
  * to the DataList by the user. The user can refer to each Step's
  * data usage by one of the several schemes.
  * 
  * - External inputs / outputs to a Scope: a Scope can specify that
  * certain inputs and outputs are to be bound to producers or
  * consumers from outside of the Scope.
  * 
  * Internally, DataList uses ```tg::data::specialized::EquivSet```
  * to resolve the reference equivalence of data, and then perform
  * some validations or reporting task in order to give timely
  * advice as to the overall validity of data definitions.
  * 
  * DataList is designed so that it can be useful even if the Scopes,
  * Steps and data items are not frozen. This relies heavily on the
  * append-only design of Scopes and Steps, without which such
  * incrementally performed analyses would not be possible.
  */
class DataList
{
public:
    using EquivSet = tg::data::specialized::EquivSet;

public:
    DataList();
    ~DataList();

    /**
     * @brief Add a Scope and all its Steps to the list.
     */
    void add_scope(ScopePtr scope);

    /**
     * @brief Add a single Step to the list. The Step must belong to a Scope
     *        that has been added previously.
     */
    void add_step(StepPtr step);

    /**
     * @brief Returns the number of scopes stored.
     */
    size_t scope_count() const;

    /**
     * @brief Returns the number of steps stored.
     */
    size_t step_count() const;

    /**
     * @brief Returns the number of data items collected.
     */
    size_t data_count() const;

    /**
     * @brief Access collected scopes.
     */
    const std::vector<ScopePtr>& scopes() const;

    /**
     * @brief Access collected steps.
     */
    const std::vector<StepPtr>& steps() const;

    /**
     * @brief Access collected scoped data info tuples.
     */
    const std::vector<ScopedDataInfoTuple>& datainfos() const;

private:
    size_t find_scope_index(ScopeInfoPtr scopeinfo) const;

    std::vector<ScopePtr> m_scopes;
    std::vector<StepPtr> m_steps;
    std::vector<ScopedDataInfoTuple> m_datainfo;
#if 0 // PENDING IMPLEMENTATION DESIGN
    // std::unordered_map<size_t, size_t> m_stepid_to_scopeid;
    // std::unordered_map<std::pair<size_t, size_t>, size_t> m_stepdataid_to_my_datainfoid;
#endif // PENDING IMPLEMENTATION DESIGN

    /**
     * @brief The data equivalence resolution.
     * @note Each re-run of analysis creates a new EquivSet instance.
     */
    std::shared_ptr<EquivSet> m_equivset;
};

} // namespace tg::core::dag
