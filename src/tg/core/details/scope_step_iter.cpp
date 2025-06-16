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
     * @brief Two iterators are equal when they reference the same ScopeInfo and
     * their positions are both past end or hold the same index. Expired
     * iterators compare equal to each other and to any iterator whose position
     * is past the current step count.
     */

    auto sp_this = m_wp_scopeinfo.lock();
    auto sp_other = other.m_wp_scopeinfo.lock();

    if (sp_this && sp_other)
    {
        if (sp_this.get() != sp_other.get())
        {
            return false;
        }
        size_t count = sp_this->step_count();
        bool this_end = m_pos >= count;
        bool other_end = other.m_pos >= count;
        if (this_end && other_end)
        {
            return true;
        }
        return m_pos == other.m_pos;
    }
    else if (!sp_this && !sp_other)
    {
        return true;
    }
    else
    {
        const ScopeInfo& ref = sp_this ? *sp_this : *sp_other;
        size_t count = ref.step_count();
        size_t pos = sp_this ? m_pos : other.m_pos;
        return pos >= count;
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
