#pragma once
#include "tg/core/fwd.hpp"

namespace tg::core::details
{

class ScopeStepIter
{
public:
    static constexpr size_t npos = ~static_cast<size_t>(0u);

public:
    ScopeStepIter();
    explicit ScopeStepIter(ScopeInfoPtr scopeinfo, size_t pos = 0u);
    ~ScopeStepIter();
    ScopeStepIter(const ScopeStepIter&);
    ScopeStepIter(ScopeStepIter&&);
    ScopeStepIter& operator=(const ScopeStepIter&);
    ScopeStepIter& operator=(ScopeStepIter&&);

public:
    /**
     * @brief Returns an iterator to the beginning of the Steps in the ScopeInfo.
     * @returns A ScopeStepIter pointing to the first Step in the ScopeInfo.
     */
    ScopeStepIter begin() const;

    /**
     * @brief Returns an iterator to the end of the Steps in the ScopeInfo.
     * @returns A ScopeStepIter pointing past the last Step in the ScopeInfo.
     */
    ScopeStepIter end() const;

    /**
     * @brief Dereference operator to get the Step at the current position.
     * @returns A strong pointer to the Step at the current position,
     * or an empty pointer if the position is out of bounds or if the
     * ScopeInfo or Step is no longer valid.
     */
    StepPtr operator*() const;

    /**
     * @brief Pre-increment operator to move to the next Step.
     * @returns A reference to the current ScopeStepIter after incrementing.
     */
    ScopeStepIter& operator++();

    /**
     * @brief Post-increment operator to move to the next Step.
     * @returns A copy of the current ScopeStepIter before incrementing.
     */
    ScopeStepIter operator++(int);

    /**
     * @brief Equality operator to compare two ScopeStepIter objects.
     * @returns True if both are valid and equal, or if both are invalid (past end).
     */
    bool operator==(const ScopeStepIter& other) const;

    /**
     * @brief Inequality operator to compare two ScopeStepIter objects.
     */
    bool operator!=(const ScopeStepIter& other) const;

    /**
     * @brief Checks if the iterator is past the end of the Steps in the ScopeInfo.
     * @returns True if the current position is past the last Step in the ScopeInfo,
     * or if the ScopeInfo is no longer valid.
     */
    [[nodiscard]]
    bool is_past_end() const;

private:
    bool detail_is_past_end(const ScopeInfo& ref_info) const;

private:
    ScopeInfoWPtr m_wp_scopeinfo;
    size_t m_pos;
};

} // namespace tg::core::details
