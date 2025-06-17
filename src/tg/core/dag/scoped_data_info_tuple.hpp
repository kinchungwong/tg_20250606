#pragma once
#include <cstdint>
#include <string>
#include <typeindex>
#include "tg/core/fwd.hpp"

namespace tg::core::dag
{

struct ScopedDataInfoTuple
{
public:
    /**
     * @brief The scope containing the step, which in turn contain the data usage.
     */
    ScopeInfoWPtr m_wp_scopeinfo;

    /**
     * @brief The step which specify the data usage.
     */
    StepWPtr m_wp_step;

    /**
     * @brief The assigned index of the Scope when it is inserted into a DataList.
     */
    size_t m_scope_index;

    /**
     * @brief The assigned index of the Step when it is inserted into a DataList.
     * @note Upon inserting a Step into DataList, it is assigned an index that
     * can be used independently of m_scope_index.
     */
    size_t m_step_index;

    /**
     * @brief The index of the data definition as it appears in the Step.
     */
    size_t m_step_data_index;

    /**
     * @brief The equivalence-resolved label of the data. 
     * @note DataList may re-run EquivSet multiple times, and each run may cause
     * this value to change. The assigned value is not guaranteed to be stable.
     */
    size_t m_data_equiv_label;

    /**
     * @brief The current shortname given to the data by the Step.
     * @note Because the data shortname is mutable on the Step object, it is
     * refreshed every time DataList runs its fetching and analysis.
     */
    std::string m_data_shortname;

    /**
     * @brief The qualified name, constructed from "scope.step.data".
     */
    std::string m_qualified_name;

    /**
     * @brief Usage of this data as it appears on the Step.
     */
    DataUsage m_usage;

    /**
     * @brief Type of the data.
     */
    std::type_index m_type;

public:
    ScopedDataInfoTuple() = default;
    ~ScopedDataInfoTuple() = default;
    ScopedDataInfoTuple(const ScopedDataInfoTuple&) = default;
    ScopedDataInfoTuple(ScopedDataInfoTuple&&) = default;
    ScopedDataInfoTuple& operator=(const ScopedDataInfoTuple&) = default;
    ScopedDataInfoTuple& operator=(ScopedDataInfoTuple&&) = default;
};

} // namespace tg::core
