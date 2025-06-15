#include <cstdio> // for sprintf
#include "tg/core/fwd.hpp"
#include "tg/core/step_info.hpp"
#include "tg/core/scope.hpp"
#include "tg/core/scope_info.hpp"

namespace tg::core
{

StepInfo::StepInfo()
    : m_step_shortname{}
    , m_step_type{std::type_index(typeid(void))}
    , m_wp_scopeinfo{}
    , m_datainfos{}
    , m_names_frozen{false}
    , m_scope_frozen{false}
    , m_datainfos_frozen{false}
{}

StepInfo::StepInfo(std::string_view shortname)
    : StepInfo()
{
    this->set_shortname(shortname);
}

StepInfo::StepInfo(std::string_view shortname, std::type_index type)
    : StepInfo()
{
    this->set_shortname(shortname);
    this->detail_set_step_type(type);
}

StepInfo::StepInfo(const StepInfo& other)
    : m_step_shortname{other.m_step_shortname}
    , m_step_type{other.m_step_type}
    , m_wp_scopeinfo{other.m_wp_scopeinfo}
    , m_datainfos{other.m_datainfos}
    , m_names_frozen{false}
    , m_scope_frozen{false}
    , m_datainfos_frozen{false}
{
}

StepInfo::StepInfo(StepInfo&& other)
    : m_step_shortname{std::move(other.m_step_shortname)}
    , m_step_type{std::move(other.m_step_type)}
    , m_wp_scopeinfo{std::move(other.m_wp_scopeinfo)}
    , m_datainfos{std::move(other.m_datainfos)}
    , m_names_frozen{false}
    , m_scope_frozen{false}
    , m_datainfos_frozen{false}
{
    other.m_step_shortname.clear();
    other.m_step_type = std::type_index(typeid(void));
    other.m_wp_scopeinfo.reset();
    other.m_datainfos.clear();
    other.m_names_frozen = true;
    other.m_scope_frozen = true;
    other.m_datainfos_frozen = true;
}

StepInfo& StepInfo::operator=(const StepInfo& other)
{
    if (this != &other)
    {
        if (m_names_frozen || m_scope_frozen || m_datainfos_frozen)
        {
            throw StepInfoFrozen("");
        }
        if (!m_datainfos.empty())
        {
            throw StepInfoNotEmpty("");
        }
        // Note: we already verified that both frozen flags are false,
        // means that this instance allows modifications.
        this->m_step_shortname = other.m_step_shortname;
        this->m_step_type = other.m_step_type;
        this->m_wp_scopeinfo = other.m_wp_scopeinfo;
        this->m_datainfos = other.m_datainfos;
    }
    return *this;
}

StepInfo& StepInfo::operator=(StepInfo&& other)
{
    if (this != &other)
    {
        if (m_names_frozen || m_scope_frozen || m_datainfos_frozen)
        {
            throw StepInfoFrozen("");
        }
        if (!m_datainfos.empty())
        {
            throw StepInfoNotEmpty("");
        }
        // Note: we already verified that both frozen flags are false,
        // means that this instance allows modifications.
        this->m_step_shortname = std::move(other.m_step_shortname);
        this->m_step_type = other.m_step_type;
        this->m_wp_scopeinfo = std::move(other.m_wp_scopeinfo);
        this->m_datainfos = std::move(other.m_datainfos);
        // Clear the expiring instance, and prevent modifications.
        other.m_step_shortname.clear();
        other.m_step_type = std::type_index(typeid(void));
        other.m_wp_scopeinfo.reset();
        other.m_datainfos.clear();
        other.m_names_frozen = true;
        other.m_scope_frozen = true;
        other.m_datainfos_frozen = true;
    }
    return *this;
}

void StepInfo::names_freeze()
{
    m_names_frozen = true;
}

bool StepInfo::names_is_frozen() const
{
    return m_names_frozen;
}

void StepInfo::datainfos_freeze()
{
    m_datainfos_frozen = true;
}

bool StepInfo::datainfos_is_frozen() const
{
    return m_datainfos_frozen;
}

void StepInfo::scope_freeze()
{
    m_scope_frozen = true;
}

bool StepInfo::scope_is_frozen() const
{
    return m_scope_frozen;
}

void StepInfo::freeze()
{
    m_names_frozen = true;
    m_scope_frozen = true;
    m_datainfos_frozen = true;
}

bool StepInfo::is_all_frozen() const
{
    return (
        m_names_frozen &&
        m_scope_frozen &&
        m_datainfos_frozen
    );
}

const std::string& StepInfo::shortname() const
{
    return m_step_shortname;
}

std::type_index StepInfo::step_type() const
{
    return m_step_type;
}

ScopeInfoPtr StepInfo::scopeinfo() const
{
    return m_wp_scopeinfo.lock();
}

std::string StepInfo::scopename() const
{
    auto scopeinfo = this->scopeinfo();
    if (scopeinfo)
    {
        return scopeinfo->scopename();
    }
    return {};
}

void StepInfo::on_added_to_scope(size_t scope_owner_token, ScopeInfoPtr scopeinfo)
{
    if (scope_owner_token == 0u)
    {
        throw std::invalid_argument("StepInfo::on_added_to_scope(): scope owner token cannot be zero.");
    }
    if (!scopeinfo)
    {
        throw std::invalid_argument("StepInfo::on_added_to_scope(): scopeinfo cannot be null.");
    }
    if (m_scope_frozen)
    {
        throw StepInfoFrozen("StepInfo::on_added_to_scope(): scope is frozen.");
    }
    scopeinfo->assert_owner_token(scope_owner_token);
    m_wp_scopeinfo = scopeinfo; // weak ref
}

void StepInfo::set_shortname(std::string_view shortname)
{
    if (m_names_frozen)
    {
        throw StepInfoFrozen("");
    }
    m_step_shortname = shortname;
}

void StepInfo::detail_set_step_type(std::type_index type)
{
    if (m_step_type != type && m_step_type != std::type_index{typeid(void)})
    {
        throw StepInfoBadAssignment("");
    }
    m_step_type = type;
}

size_t StepInfo::data_count() const
{
    return m_datainfos.size();
}

DataInfoTuple StepInfo::get_data_info(std::string_view shortname) const
{
    auto idx_opt = this->find_data(shortname);
    if (!idx_opt.has_value())
    {
        throw StepInfoNameNotFound("");
    }
    return m_datainfos.at(idx_opt.value());
}

DataInfoTuple StepInfo::get_data_info(size_t local_index) const
{
    size_t sz = m_datainfos.size();
    if (local_index >= sz)
    {
        throw StepInfoBadIndex("");
    }
    return m_datainfos.at(local_index);
}

void StepInfo::get_data_infos(std::vector<DataInfoTuple>& datainfos) const
{
    datainfos = m_datainfos;
}

std::optional<size_t> StepInfo::find_data(std::string_view shortname) const
{
    for (const auto& info : m_datainfos)
    {
        if (info.m_shortname == shortname)
        {
            return info.m_local_index;
        }
    }
    return std::nullopt;
}

void StepInfo::rename_data(std::string_view old_shortname, std::string_view new_shortname)
{
    if (m_names_frozen)
    {
        throw StepInfoFrozen("");
    }
    if (old_shortname == new_shortname)
    {
        return;
    }
    auto old_idx_opt = this->find_data(old_shortname);
    if (!old_idx_opt.has_value())
    {
        //! @todo reimplement
        char msg[1024];
        snprintf(msg, sizeof(msg),
            "StepInfo::rename_data(): old name not found. Old name: %.*s, New name: %.*s",
            static_cast<int>(std::min<size_t>(256u, old_shortname.size())),
            old_shortname.data(),
            static_cast<int>(std::min<size_t>(256u, new_shortname.size())),
            new_shortname.data()
        );
        throw StepInfoNameNotFound(msg);
    }
    auto new_idx_opt = this->find_data(new_shortname);
    if (new_idx_opt.has_value())
    {
        //! @todo reimplement
        char msg[1024];
        snprintf(msg, sizeof(msg),
            "StepInfo::rename_data(): new name already exists. Old name: %.*s, New name: %.*s",
            static_cast<int>(std::min<size_t>(256u, old_shortname.size())),
            old_shortname.data(),
            static_cast<int>(std::min<size_t>(256u, new_shortname.size())),
            new_shortname.data()
        );
        throw StepInfoNameConflict(msg);
    }
    size_t old_index = old_idx_opt.value();
    auto& info = m_datainfos.at(old_index);
    info.m_shortname = new_shortname;
}

void StepInfo::mark_data_as_consume(std::string_view shortname)
{
    if (m_datainfos_frozen)
    {
        throw StepInfoFrozen("");
    }
    auto idx_opt = this->find_data(shortname);
    if (!idx_opt.has_value())
    {
        throw StepInfoNameNotFound("");
    }
    DataUsage& ref_usage = m_datainfos.at(idx_opt.value()).m_usage;
    if (ref_usage == DataUsage::Consume)
    {
        // no change needed
        return; 
    }
    if (ref_usage != DataUsage::Read)
    {
        // The only elevation allowed is from Read to Consume.
        throw StepInfoBadAssignment("");
    }
    ref_usage = DataUsage::Consume;
}

size_t StepInfo::detail_add_data(std::string&& shortname, DataUsage usage, std::type_index data_type)
{
    if (m_datainfos_frozen)
    {
        throw StepInfoFrozen("");
    }
    auto idx_opt = this->find_data(shortname);
    if (idx_opt.has_value())
    {
        throw StepInfoNameConflict("");
    }
    size_t local_index = m_datainfos.size();
    m_datainfos.emplace_back(local_index, std::move(shortname), usage, data_type);
    return local_index;
}

} // namespace tg::core
