#pragma once
#include <unordered_set>
#include "tg/core/fwd.hpp"
#include "tg/data/specialized/unique_list.fwd.hpp"

namespace tg::data::specialized { class EquivSet; }

namespace tg::core
{

/**
 * @brief Info struct for data usage by various steps.
 * 
 * @details Data is matched by name. Steps are given generated
 * named by SmallGraph. Names of steps that use a piece of data
 * for input or output are stored in the respective vectors.
 * 
 * When different instances of StepData (originally having
 * different names) are linked together, their records are
 * merged, and the names are stored in the m_names vector.
 * 
 * Although it is invalid to have multiple outputters for the
 * same data, it is captured anyway, to faciliate diagnostics
 * and logging.
 */
struct DataUsageInfo
{
    // std::unordered_set<StepWPtr> m_steps;
    // std::unordered_set<std::string> m_names;
    // std::unordered_set<std::type_index> m_types;
    // std::unordered_set<std::string> m_outputters;
    // std::unordered_set<std::string> m_inputters;
#if 0 // consumers not implemented yet.
    // std::unordered_set<std::string> m_consumers;
#endif
};

class SmallGraph
{
public:
    using NameList = tg::data::specialized::UniqueList<std::string>;
    using EquivSet = tg::data::specialized::EquivSet;
    using DuiMap = std::unordered_map<std::string, DataUsageInfo>;

public:
    SmallGraph();
    void add(StepPtr step);

private:
    std::pair<std::string, size_t> assign_name();

private:
    std::shared_ptr<NameList> m_names;
    std::shared_ptr<EquivSet> m_equiv_set;
    int m_next_assigned_name_id;
    DuiMap m_dui;
};

} // namespace tg::core
