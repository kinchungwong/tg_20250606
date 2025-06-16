#include "tg/core/details/scope_step_iter.hpp"
#include "tg/core/scope_info.hpp"

namespace tg::core::details
{

ScopeStepIter::ScopeStepIter()
    : m_wp_scopeinfo()
    , m_pos(npos)
{}

ScopeStepIter::ScopeStepIter(ScopeInfoPtr scopeinfo, size_t pos)
    : m_wp_scopeinfo(scopeinfo)
    , m_pos(pos)
{}

ScopeStepIter::~ScopeStepIter()
{}

ScopeStepIter::ScopeStepIter(const ScopeStepIter&) = default;
ScopeStepIter::ScopeStepIter(ScopeStepIter&&) = default;
ScopeStepIter& ScopeStepIter::operator=(const ScopeStepIter&) = default;
ScopeStepIter& ScopeStepIter::operator=(ScopeStepIter&&) = default;

ScopeStepIter ScopeStepIter::begin() const
{
    return ScopeStepIter(m_wp_scopeinfo.lock(), 0u);
}

ScopeStepIter ScopeStepIter::end() const
{
    auto sp_scopeinfo = m_wp_scopeinfo.lock();
    if (!sp_scopeinfo)
    {
        return ScopeStepIter();
    }
    return ScopeStepIter(sp_scopeinfo, sp_scopeinfo->step_count());
}

StepPtr ScopeStepIter::operator*() const
{
    auto sp_scopeinfo = m_wp_scopeinfo.lock();
    if (!sp_scopeinfo)
    {
        return nullptr;
    }
    return sp_scopeinfo->step_at(m_pos);
}

ScopeStepIter& ScopeStepIter::operator++()
{
    ++m_pos;
    return *this;
}

ScopeStepIter ScopeStepIter::operator++(int)
{
    ScopeStepIter tmp(*this);
    ++(*this);
    return tmp;
}

bool ScopeStepIter::operator==(const ScopeStepIter& other) const
{
    if (this == &other)
    {
        return true;
    }
    /**
     * @details Requirements:
     * (1) If both iterators point to some valid ScopeInfo, and...
     * (1.1) If the ScopeInfo pointers are equal (SAME INSTANCE, ...), and...
     * (1.1.1) If the positions are equal, return true; no need to check bounds (IMPLIED (EITHER (BOTH GO, SAME POS) OR (BOTH STOP))). However...
     * (1.1.2) If the positions are not equal, compare their positions to the ScopeInfo's step count. Then...
     * (1.1.2.1) If both are out-of-bounds, return true (BOTH STOP). Otherwise...
     * (1.1.2.2) Return true if their positions are equal (BOTH GO, SAME POS), false otherwise. (BOTH GO, DIFFERENT POS).
     * (1.2) If their ScopeInfo pointers are not equal, return false (DIFFERENT INSTANCES).
     * (2) If both iterators point to a null or expired ScopeInfo, return true (BOTH STOP).
     * (3) If one iterator points to a valid ScopeInfo and the other to a null or expired ScopeInfo, ...
     * (3...) (cont'd) On the iterator containing the valid ScopeInfo, compare its position to its step count. Then...
     * (3...) (cont'd) If the position is out-of-bounds, return true (BOTH STOP). False otherwise.
     */
    throw std::runtime_error("ScopeStepIter::operator==(): Not implemented yet");
#if 0 // DISABLED - POSSIBLY INCORRECT IMPLEMENTATION
    // /**
    //  * @note Logic table.
    //  * 
    //  * Shorthands.
    //  * L for LHS (left-hand-side, or first argument to equality operator).
    //  * R for RHS.
    //  * Z for zero. (pointer null or expired)
    //  * T for terminal. (pos >= size) (both are unsigned integers.)
    //  * V for valid. (pointer valid, pos < size)
    //  * PPE for all equal (both pointer and position).
    //  * 
    //  * Let:
    //  * (1) LZ = LHS null/expire, (2) RZ = RHS null/expire,
    //  * (3) LT = LHS past-end,    (4) RT = RHS past-end,
    //  * (5) LV = LHS valid,       (6) RV = RHS valid.
    //  * (7) PPE = (LHS valid) AND (RHS valid) AND (LHS.pos == RHS.pos)
    //  * Given:
    //  * (LV == true) <===> (LZ == false AND LQ == false)
    //  * (RV == true) <===> ((RX OR RE) == false)
    //  * (EqualityOperatorResult == true) <===> ((5 AND 6) OR ((1 OR 3) AND (2 or 4)))
    //  */
    // auto sp_this = m_wp_scopeinfo.lock();
    // auto sp_other = other.m_wp_scopeinfo.lock();
    // if (sp_this == sp_other && m_pos == other.m_pos)
    // {
    //     /**
    //      * @note Both iterators point to the same ScopeInfo and position.
    //      * Pas-end check is not needed here, because when all attributes 
    //      * are equal, their past-end checks result will also be equal.
    //      */
    //     return true;
    // }
    // /**
    //  * @note Past-end check with the already-locked ScopeInfo ptrs.
    //  */
    // bool this_past_end = this->detail_is_past_end(*sp_this);
    // bool other_past_end = other.detail_is_past_end(*sp_other);
    // if (this_past_end && other_past_end)
    // {
    //     // All past-end iterators (including containers expired or empty)
    //     // are considered equal, because they act as sentinels to stop a
    //     // for-loop.
    //     // This is the case even if the underlying ScopeInfo addresses
    //     // are different.
    //     return true;
    // }
    // return (sp_this == sp_other && m_pos == other.m_pos);
#endif // DISABLED - POSSIBLY INCORRECT IMPLEMENTATION
}

bool ScopeStepIter::operator!=(const ScopeStepIter& other) const
{
    return !(*this == other);
}

bool ScopeStepIter::is_past_end() const
{
    ScopeInfoPtr sp_info = m_wp_scopeinfo.lock();
    if (!sp_info)
    {
        return true;
    }
    return this->detail_is_past_end(*sp_info);
}

bool ScopeStepIter::detail_is_past_end(const ScopeInfo& ref_info) const
{
    return m_pos >= ref_info.step_count();
}

} // namespace tg::core::details
