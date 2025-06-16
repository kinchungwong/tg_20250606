#include "tg/core/scope_info.hpp"
#include "tg/core/scope.hpp"
#include "tg/core/step.hpp"
#include "tg/core/step_info.hpp"

namespace tg::core
{

ScopeInfo::ScopeInfo(std::string_view scopename, size_t owner_token)
    : m_scopename{scopename}
    , m_wp_steps{}
    , m_owner_token{owner_token}
    , m_frozen{false}
{
    if (owner_token == 0u)
    {
        throw std::invalid_argument("ScopeInfo::ScopeInfo(): owner token cannot be set to zero.");
    }
}

ScopeInfo::~ScopeInfo()
{
}

void ScopeInfo::assert_owner_token(size_t owner_token) const
{
    if (owner_token != m_owner_token)
    {
        throw std::logic_error("ScopeInfo::assert_owner_token(): wrong owner token.");
    }
}

size_t ScopeInfo::step_count() const
{
    return m_wp_steps.size();
}

void ScopeInfo::add_step(size_t owner_token, StepPtr step)
{
    if (m_frozen)
    {
        throw std::logic_error("ScopeInfo::add_step(): ScopeInfo is frozen.");
    }
    if (!step)
    {
        throw std::invalid_argument("ScopeInfo::add_step(): step cannot be null.");
    }
    this->assert_owner_token(owner_token);
    /**
     * @note The owning Scope is responsible for preventing duplicate Step
     * additions; therefore no checks needed here. 
     * 
     * @note Furthermore, the Step items in ScopeInfo will be in the same order
     * as in the owning Scope.
     */
    m_wp_steps.emplace_back(step);
}

StepPtr ScopeInfo::step_at(size_t index) const
{
    if (index >= m_wp_steps.size())
    {
        return StepPtr{};
    }
    return m_wp_steps.at(index).lock();
}

const std::string& ScopeInfo::scopename() const
{
    return m_scopename;
}

bool ScopeInfo::is_frozen() const
{
    return m_frozen;
}

void ScopeInfo::freeze()
{
    m_frozen = true;
}

} // namespace tg::core
