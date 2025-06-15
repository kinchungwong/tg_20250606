#include "tg/core/scope.hpp"
#include "tg/core/scope_info.hpp"
#include "tg/core/step.hpp"
#include "tg/core/step_info.hpp"

namespace tg::core
{

Scope::Scope(std::string_view scopename)
    : m_scopename{scopename}
    , m_sp_scopeinfo{/*(defer initialized in ctor)*/}
    , m_steps{}
    , m_owner_token{}
    , m_frozen{false}
{
    if (m_scopename.empty())
    {
        throw std::invalid_argument("Scope::Scope(): scopename cannot be empty.");
    }
    /**
     * @todo Enforce other naming rules on m_scopename. In particular,
     * it cannot contain the scope separator.
     */
    /**
     * @todo Set the owner token to something non-zero; do not use something
     * obvious (e.g. a constant), and do not leak information in doing so.
     */
    m_owner_token = 1u;
    /**
     * @note Use the owner token to assert ownership over the ScopeInfo.
     */
    m_sp_scopeinfo = std::make_shared<ScopeInfo>(m_scopename, m_owner_token);
}

Scope::~Scope()
{
}

const ScopeInfo& Scope::info() const
{
    return *m_sp_scopeinfo;
}

const std::string& Scope::scopename() const
{
    return m_scopename;
}

void Scope::add(StepPtr step)
{
    if (m_frozen)
    {
        throw std::runtime_error("Scope::add(): cannot add Step to a frozen Scope.");
    }
    if (!step)
    {
        throw std::invalid_argument("Scope::add(): step cannot be null.");
    }
    if (contains(step))
    {
        throw std::runtime_error("Scope::add(): cannot add same Step to Scope twice.");
    }
    m_steps.push_back(step);
    m_sp_scopeinfo->add_step(m_owner_token, step);
    step->info().on_added_to_scope(m_owner_token, m_sp_scopeinfo);
}

bool Scope::contains(StepPtr step) const
{
    for (const auto& s : m_steps)
    {
        if (s.get() == step.get())
        {
            return true;
        }
    }
    return false;
}

std::vector<StepPtr> Scope::get_steps() const
{
    return m_steps;
}

void Scope::freeze()
{
    if (m_frozen)
    {
        return; // Already frozen
    }
    m_frozen = true;
    m_sp_scopeinfo->freeze();
    for (auto& step : m_steps)
    {
        step->info().freeze();
    }
}

bool Scope::is_frozen() const
{
    return m_frozen;
}

} // namespace tg::core
