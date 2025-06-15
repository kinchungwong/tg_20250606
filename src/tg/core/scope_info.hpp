#pragma once
#include "tg/core/fwd.hpp"

namespace tg::core
{

/**
 * @brief An information-holding class created for a Scope.
 * 
 * @details This class is split off from the Scope class to allow
 * weak references between ScopeInfo and StepInfo.
 * 
 * This design choice is required for Step rename operations,
 * which need to check if the new name conflicts with any other
 * Step instances ever added to the same Scope.
 */
class ScopeInfo
{
public:
    ScopeInfo(std::string_view scopename, size_t owner_token);
    ~ScopeInfo();

public:
    /**
     * @brief Raise exception if the owner token does not match.
     * @exception std::exception
     */
    void assert_owner_token(size_t owner_token) const;

    /**
     * @brief Stores a weak pointer to the Step in the ScopeInfo.
     * 
     * @param owner_token The token of the owner Scope, used for validation.
     * @param step The Step to add, as a strong pointer.
     * 
     * @note This method converts the Step to a weak pointer for storage.
     */
    void add_step(size_t owner_token, StepPtr step);

    /**
     * @brief Retrieves the short name of the Scope.
     */
    const std::string& scopename() const;

    /**
     * @brief Checks if the ScopeInfo is frozen.
     */
    [[nodiscard]]
    bool is_frozen() const;

    /**
     * @brief Freezes the ScopeInfo, preventing further modifications.
     * 
     * @note Once frozen, no new Steps can be added to this ScopeInfo.
     */
    void freeze();

private:
    ScopeInfo(const ScopeInfo&) = delete;
    ScopeInfo(ScopeInfo&&) = delete;
    ScopeInfo& operator=(const ScopeInfo&) = delete;
    ScopeInfo& operator=(ScopeInfo&&) = delete;

private:
    std::string m_scopename;
    std::vector<StepWPtr> m_wp_steps;
    size_t m_owner_token;
    bool m_frozen;
};

} // namespace tg::core
