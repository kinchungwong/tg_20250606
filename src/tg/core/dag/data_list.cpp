#include "tg/core/dag/data_list.hpp"
#include "tg/core/step.hpp"
#include "tg/core/step_info.hpp"
#include "tg/core/scope.hpp"
#include "tg/core/scope_info.hpp"
#include "tg/core/data_info_tuple.hpp" // as returned from StepInfo

namespace tg::core::dag
{

DataList::DataList()
    : m_equivset(std::make_shared<EquivSet>())
{}

DataList::~DataList() = default;

size_t DataList::find_scope_index(ScopeInfoPtr scopeinfo) const
{
    if (!scopeinfo)
    {
        return static_cast<size_t>(-1);
    }
    for (size_t i = 0; i < m_scopes.size(); ++i)
    {
        if (&m_scopes.at(i)->info() == scopeinfo.get())
        {
            return i;
        }
    }
    return static_cast<size_t>(-1);
}

void DataList::add_scope(ScopePtr scope)
{
    if (!scope)
    {
        throw std::invalid_argument("DataList::add_scope(): null scope");
    }
    m_scopes.push_back(scope);
    auto steps = scope->get_steps();
    for (const auto& step : steps)
    {
        this->add_step(step);
    }
}

void DataList::add_step(StepPtr step)
{
    if (!step)
    {
        throw std::invalid_argument("DataList::add_step(): null step");
    }
    StepInfo& info = step->info();
    ScopeInfoPtr scopeinfo = info.scopeinfo();
    size_t scope_index = this->find_scope_index(scopeinfo);
    size_t step_index = m_steps.size();
    m_steps.push_back(step);

    std::vector<DataInfoTuple> infos;
    info.get_data_infos(infos);
    for (const auto& dit : infos)
    {
        size_t global_index = m_datainfo.size();
        m_equivset->insert(global_index);
        for (size_t i = 0; i < m_datainfo.size(); ++i)
        {
            if (m_datainfo[i].m_scope_index == scope_index &&
                m_datainfo[i].m_data_shortname == dit.m_shortname)
            {
                m_equivset->link(global_index, i);
            }
        }

        std::vector<size_t> members;
        m_equivset->export_one(global_index, members);
        size_t label = global_index;
        for (size_t m : members)
        {
            label = std::min(label, m);
        }
        for (size_t m : members)
        {
            if (m < m_datainfo.size())
            {
                m_datainfo[m].m_data_equiv_label = label;
            }
        }

        ScopedDataInfoTuple sdit{};
        sdit.m_wp_scopeinfo = scopeinfo;
        sdit.m_wp_step = step;
        sdit.m_scope_index = scope_index;
        sdit.m_step_index = step_index;
        sdit.m_step_data_index = dit.m_local_index;
        sdit.m_data_equiv_label = label;
        sdit.m_data_shortname = dit.m_shortname;
        sdit.m_usage = dit.m_usage;
        sdit.m_type = dit.m_type;
        sdit.m_qualified_name =
            info.scopename() + "." + info.shortname() + "." + dit.m_shortname;

        m_datainfo.push_back(std::move(sdit));
    }
}

size_t DataList::scope_count() const { return m_scopes.size(); }
size_t DataList::step_count() const { return m_steps.size(); }
size_t DataList::data_count() const { return m_datainfo.size(); }
const std::vector<ScopePtr>& DataList::scopes() const { return m_scopes; }
const std::vector<StepPtr>& DataList::steps() const { return m_steps; }
const std::vector<ScopedDataInfoTuple>& DataList::datainfos() const { return m_datainfo; }

} // namespace tg::core::dag
