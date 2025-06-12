#include "tg/core/detail/equiv_name_set.hpp"

namespace tg::core::detail
{


EquivNameSet::EquivNameSet()
    : m_names{std::make_shared<NameList>()}
    , m_equiv_set{std::make_shared<EquivSet>()}
{
}

EquivNameSet::~EquivNameSet()
{
}

EquivNameSet::EquivNameSet(const EquivNameSet& other)
    : m_names{other.m_names->shallow_copy()}
    , m_equiv_set{std::make_shared<EquivSet>(*other.m_equiv_set)}
{
}

EquivNameSet::EquivNameSet(EquivNameSet&& other)
    : m_names{std::move(other.m_names)}
    , m_equiv_set{std::move(other.m_equiv_set)}
{
    // Reset the moved-from instance to a valid empty state
    other.m_names = std::make_shared<NameList>();
    other.m_equiv_set = std::make_shared<EquivSet>();
}

size_t EquivNameSet::insert(const std::string& name)
{
    if (name.empty())
    {
        throw std::invalid_argument("EquivNameSet::insert: Name cannot be empty.");
    }
    return m_names->insert(name);
}

void EquivNameSet::link(const std::string& lhs, const std::string& rhs)
{
    if (lhs.empty() || rhs.empty())
    {
        throw std::invalid_argument("EquivNameSet::link: Names cannot be empty.");
    }
    auto lhs_idx_opt = m_names->find(lhs);
    auto rhs_idx_opt = m_names->find(rhs);
    if (!lhs_idx_opt.has_value() || !rhs_idx_opt.has_value())
    {
        throw std::invalid_argument("EquivNameSet::link: Both names must exist in the set.");
    }
    size_t lhs_idx = lhs_idx_opt.value();
    size_t rhs_idx = rhs_idx_opt.value();
    m_equiv_set->link(lhs_idx, rhs_idx);
}

bool EquivNameSet::contains(const std::string& name) const
{
    return m_names->find(name).has_value();
}

void EquivNameSet::optimize()
{
    m_equiv_set->optimize();
}

void EquivNameSet::get_members(const std::string& name, std::vector<std::string>& members) const
{
    members.clear();
    if (name.empty())
    {
        throw std::invalid_argument("EquivNameSet::get_members: Name cannot be empty.");
    }
    auto idx_opt = m_names->find(name);
    if (!idx_opt.has_value())
    {
        // Name not found, return empty members
        return;
    }
    size_t idx = idx_opt.value();
    std::vector<size_t> equiv_members;
    m_equiv_set->export_one(idx, equiv_members);
    for (size_t member_idx : equiv_members)
    {
        std::shared_ptr<std::string> p_member_name = m_names->at(member_idx);
        members.push_back(*p_member_name);
    }
}

} // namespace tg::core::detail
