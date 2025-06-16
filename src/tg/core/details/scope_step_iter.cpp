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
    if (m_pos == npos)
    {
        throw std::overflow_error("ScopeStepIter::operator++() "
            "bad attempt to increment an end-iterator.");
    }
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
     * @note
     * Equality operator logic.
     * 
     * The primary concern is that loop sentinels must work reliably.
     * The secondary concern is that, for usability, this iterator needs to work gracefully
     * when the underlying container (which is append-only) is grown in size, and when
     * the underlying container (as a weak ref) expires.
     * 
     * The equality operator for ScopeStepIter is defined as follows:
     * (1) Iterators from the same container compare equal when their positions match or when both are past the container’s current size.
     * (2) Iterators from different containers are never equal unless both are expired.
     * (3) When one iterator is expired and the other is valid, they compare equal only if the valid iterator’s position is past its current size.
     */
    auto sp_this = m_wp_scopeinfo.lock();
    auto sp_other = other.m_wp_scopeinfo.lock();
    if (!sp_this && !sp_other)
    {
        return true; // Both iterators are empty or expired.
    }
    else if (sp_this && sp_other)
    {
        if (sp_this != sp_other)
        {
            // Different ScopeInfo instances, cannot be equal.
            return false;
        }
        if (m_pos == other.m_pos)
        {
            // Both iterators point to the same ScopeInfo and position.
            // Either they are both valid, or they're both past-the-end.
            return true; 
        }
        // Same instance, but we still need to perform bounds check.
        size_t both_size = sp_this->step_count();
        if (m_pos >= both_size && other.m_pos >= both_size)
        {
            // Both iterators are past-the-end.
            return true;
        }
        // If they are not past-the-end, they are equal if and only if their positions match.
        return m_pos == other.m_pos;
    }
    else if (sp_this)
    {
        // The other iterator is expired or empty.
        // The iterators are considered equal if and only if this one is past the end,
        // which signifies that both are loop sentinels.
        return this->detail_is_past_end(*sp_this);
    }
    else
    {
        // This iterator is expired or empty, while the other is valid.
        // The iterators are considered equal if and only if the other one is past the end.
        return other.detail_is_past_end(*sp_other);
    }
#if defined(__cpp_lib_unreachable)
#  if __cpp_lib_unreachable >= 202202L
    std::unreachable(); 
#  endif
#endif
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
