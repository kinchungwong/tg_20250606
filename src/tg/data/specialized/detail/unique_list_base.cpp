#include "tg/common/project_macros.hpp"
#include "tg/data/specialized/detail/unique_list_base.hpp"

namespace tg::data::specialized::detail
{
UniqueListBase::UniqueListBase()
    : m_items{}
    , m_lookup{
        decltype(m_lookup)::size_type{0},
        HashCaller{*this},
        EqualCaller{*this}
    }
{}

UniqueListBase::UniqueListBase(const UniqueListBase& other)
    : UniqueListBase{}
{
    m_items.reserve(other.m_items.size());
    m_items.insert(
        m_items.end(),
        other.m_items.begin(),
        other.m_items.end()
    );
    m_lookup.max_load_factor(other.m_lookup.max_load_factor());
    m_lookup.reserve(other.m_lookup.size());
    m_lookup.insert(
        other.m_lookup.begin(),
        other.m_lookup.end()
    );
}

UniqueListBase::UniqueListBase(UniqueListBase&& other)
    : UniqueListBase{}
{
    m_items = std::move(other.m_items);
    m_lookup = std::move(other.m_lookup);
}

UniqueListBase& UniqueListBase::operator=(const UniqueListBase& other)
{
    if (this != &other)
    {
        m_items = other.m_items;
        m_lookup = other.m_lookup;
    }
    return *this;
}

UniqueListBase& UniqueListBase::operator=(UniqueListBase&& other)
{
    if (this != &other)
    {
        m_items = std::move(other.m_items);
        m_lookup = std::move(other.m_lookup);
    }
    return *this;
}

UniqueListBase::~UniqueListBase()
{
}

INLINE_NEVER
FLATTEN
void UniqueListBase::clear()
{
    m_items.clear();
    m_lookup.clear();
}

INLINE_NEVER
FLATTEN
size_t UniqueListBase::size() const
{
    return m_items.size();
}

INLINE_NEVER
FLATTEN
void UniqueListBase::reserve(size_t capacity, float max_load_factor)
{
    if (capacity > m_lookup.max_size()) {
        m_items.reserve(capacity);
    }
    m_lookup.max_load_factor(max_load_factor);
    m_lookup.reserve(capacity);
}

INLINE_NEVER
FLATTEN
size_t UniqueListBase::detail_find(const void* item) const
{
    auto it = m_lookup.find(item);
    if (it != m_lookup.end())
    {
        return it->second; // Return the index of the item
    }
    return UniqueListBase::detail_npos; // Item not found
}

INLINE_NEVER
FLATTEN
size_t UniqueListBase::detail_add(std::shared_ptr<void> item)
{
    const void* p = item.get();
    size_t index = m_items.size();
    m_items.push_back(std::move(item));
    m_lookup.emplace(p, index);
    return index;
}

FLATTEN
UniqueListBase::HashCaller::HashCaller(UniqueListBase& ref_base)
    : m_pbase(&ref_base)
{}

FLATTEN
size_t UniqueListBase::HashCaller::operator()(const void* pvoid) const
{
    return m_pbase->call_hash(pvoid);
}

FLATTEN
UniqueListBase::EqualCaller::EqualCaller(UniqueListBase& ref_base)
    : m_pbase(&ref_base)
{}

FLATTEN
bool UniqueListBase::EqualCaller::operator()(const void* lhs, const void* rhs) const
{
    return m_pbase->call_equal(lhs, rhs);
}

} // namespace tg::data::specialized::detail
