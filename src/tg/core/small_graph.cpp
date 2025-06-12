#include "tg/core/small_graph.hpp"
#include "tg/core/step.hpp"
#include "tg/core/step_data.hpp"
#include "tg/core/data_callback.hpp"
#include "tg/data/specialized/unique_list.hpp"
#include "tg/data/specialized/equiv_set.hpp"

namespace tg::core
{

SmallGraph::SmallGraph()
    : m_names(std::make_shared<NameList>())
    , m_equiv_set(std::make_shared<EquivSet>())
    , m_next_assigned_name_id(1000)
{
}

void SmallGraph::add(StepPtr step)
{
    if (!step)
    {
        throw std::invalid_argument("Step pointer cannot be null");
    }
#if 0 // Currently, steps have no names.
    // if (step->name().empty())
    // {
    //     auto [name, idx] = assign_name();
    //     step->set_name(name);
    // }
#endif
    DataCallback callback{[this, step](StepData& data) {
        std::pair<std::string, size_t> assigned;
        std::string data_name = data.name();
        if (data_name.empty())
        {
            assigned = this->assign_name();
            data.set_name(assigned.first);
            m_equiv_set->insert(assigned.second);
        }
        else
        {
            // If name is already set, it is assumed to be referring 
            // to the same data.
            // (Adding to EquivSet twice is harmless.)
            assigned.first = data_name;
            assigned.second = m_names->insert(data_name);
            m_equiv_set->insert(assigned.second);
        }
    }};
    step->visit_data(callback);
}

std::pair<std::string, size_t> SmallGraph::assign_name()
{
    int id = m_next_assigned_name_id;
    ++m_next_assigned_name_id;
    std::string name = "#" + std::to_string(id);
    if (m_names->find(name).has_value())
    {
        throw std::runtime_error("SmallGraph::assign_name: invalid use of reserved prefix '#', unable to assign name '" + name + "'");
    }
    size_t actual_idx = m_names->insert(name);
    return {name, actual_idx};
}

} // namespace tg::core
