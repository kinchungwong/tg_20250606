#pragma once
#include "tg/core/fwd.hpp"
#include "tg/core/details/scope_step_iter.hpp"

namespace tg::core
{

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
    using ScopeStepIter = tg::core::details::ScopeStepIter;

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
     * @brief Creates a begin-iterator for the steps.
     */
    ScopeStepIter begin() const;

    /**
     * @brief Creates an end-iterator for the steps.
     */
    ScopeStepIter end() const;

    /**
     * @brief Returns the number of Steps in the Scope.
     */
    size_t step_count() const;

    /**
     * @brief Read the Step at the specified index as a strong ref,
     * or return an empty pointer if the index is out of bounds.
     */
    StepPtr step_at(size_t index) const;

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
