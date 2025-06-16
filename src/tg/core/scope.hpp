#pragma once
#include "tg/core/fwd.hpp"

namespace tg::core
{

class ScopeStepIter
{
public:
    static constexpr size_t npos = ~static_cast<size_t>(0u);
    ScopeStepIter()
        : m_wp_scopeinfo()
        , m_pos(npos)
    {}
    explicit ScopeStepIter(ScopeInfoPtr scopeinfo, size_t pos = 0u)
        : m_wp_scopeinfo(scopeinfo)
        , m_pos(pos)
    {}
    ~ScopeStepIter() = default;
    ScopeStepIter(const ScopeStepIter&) = default;
    ScopeStepIter(ScopeStepIter&&) = default;
    ScopeStepIter& operator=(const ScopeStepIter&) = default;
    ScopeStepIter& operator=(ScopeStepIter&&) = default;
public:
    ScopeStepIter& operator++()
    {
        ++m_pos;
        return *this;
    }
    ScopeStepIter operator++(int)
    {
        ScopeIter tmp(*this);
        ++(*this);
        return tmp;
    }
    bool operator==(const ScopeStepIter& other) const
    {
        if (this == &other)
        {
            return true;
        }
        auto sp_this = m_wp_scopeinfo.lock();
        auto sp_other = other.m_wp_scopeinfo.lock();
        bool valid_this = (bool)sp_this;
        bool valid_other = (bool)sp_other;
        size_t this_size = valid_this ? sp_this->step_count() : 0u;
        size_t other_size = valid_other ? sp_other->step_count() : 0u;
        bool this_past_end = (m_pos >= this_size);
        bool other_past_end = (other.m_pos >= other_size);
        if (this_past_end && other_past_end)
        {
            // All past-end iterators (including containers expired or empty)
            // are considered equal, because they act as sentinels to stop a
            // for-loop.
            // This is the case even if the underlying ScopeInfo addresses
            // are different.
            return true;
        }
        if (this_past_end || other_past_end)
        {
            return false;
        }
        if (!valid_this || !valid_other)
        {
            return false;
        }
        if (sp_this.get() != sp_other.get())
        {
            return false;
        }
        return (m_pos == other.m_pos);
    }

private:
    ScopeInfoWPtr m_wp_scopeinfo;
    size_t m_pos;
};

/**
 * @brief Scope acts as both a namespace and a collection of Steps.
 * 
 * @details
 * Scope is a container for Steps, allowing them to be grouped together
 * within a namespace. Steps within a Scope can refer to each other,
 * and allows for certain operations such as renaming Steps without
 * collision, binding the data source and users between Steps,
 * and more.
 * 
 * @note
 * Scope owns strong refs to Steps and the StepInfo.
 * Steps and ScopeInfo each have weak refs to the other.
 */
class Scope
{
public:
    /**
     * @brief Initializes a Scope with a given name.
     * @param scopename The name of the Scope, which must be unique.
     * @note The current implementation does not enforce uniqueness.
     * @note The current implementation does not support changing
     * the scopename.
     */
    explicit Scope(std::string_view scopename);

    /**
     * @brief Destructor.
     */
    ~Scope();

    /**
     * @brief Returns a read-only reference to the ScopeInfo.
     * 
     * @details Scope owns a strong ref to ScopeInfo; the lifetime of
     * the ScopeInfo is tied to the Scope.
     */
    const ScopeInfo& info() const;

    /**
     * @brief Retrieves the short name of the Scope.
     */
    const std::string& scopename() const;

    /**
     * @brief Adds a Step to the Scope.
     * 
     * @details Scope will own a strong ref to the Step, while the Step
     * and the ScopeInfo will each own a weak ref to the other.
     */
    void add(StepPtr step);

    /**
     * @brief Checks whether the Step is already added to the Scope.
     */
    bool contains(StepPtr step) const;

    /**
     * @brief Gets all Steps belonging to the Scope.
     */
    std::vector<StepPtr> get_steps() const;

    /**
     * @brief Freezes the NameScope as well as all Step objects added to it.
     */
    void freeze();

    /**
     * @brief Checks if the NameScope is frozen.
     */
    [[nodiscard]]
    bool is_frozen() const;

private:
    Scope(const Scope&) = delete;
    Scope(Scope&&) = delete;
    Scope& operator=(const Scope&) = delete;
    Scope& operator=(Scope&&) = delete;

private:
    const std::string m_scopename;
    ScopeInfoPtr m_sp_scopeinfo;
    std::vector<StepPtr> m_steps;
    size_t m_owner_token;
    bool m_frozen;
};

} // namespace tg::core
