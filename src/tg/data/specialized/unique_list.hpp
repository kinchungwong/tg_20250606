#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <optional>
#include <utility>
#include <functional>
#include <type_traits>
#include "tg/data/specialized/detail/unique_list_base.hpp"
#include "tg/data/specialized/unique_list.fwd.hpp"
#include "tg/common/project_macros.hpp"

namespace tg::data::specialized
{

template <typename T, typename Hash, typename KeyEqual>
UniqueList<T, Hash, KeyEqual>::UniqueList(const hasher& hash, const key_equal& equal)
    : detail::UniqueListBase()
    , m_hash(hash)
    , m_equal(equal)
{}

template <typename T, typename Hash, typename KeyEqual>
INLINE_NEVER
size_t UniqueList<T, Hash, KeyEqual>::insert(const T& item)
{
    size_t index = this->detail_find(&item);
    if (index != detail_npos)
    {
        return index; // Item already exists, return its index
    }
    // Create a new shared_ptr for the item
    auto item_ptr = std::make_shared<T>(item);
    index = this->detail_add(item_ptr);
    return index;
}

template <typename T, typename Hash, typename KeyEqual>
INLINE_NEVER
size_t UniqueList<T, Hash, KeyEqual>::insert(T&& item)
{
    size_t index = this->detail_find(&item);
    if (index != detail_npos)
    {
        return index; // Item already exists, return its index
    }
    // Create a new shared_ptr for the item
    auto item_ptr = std::make_shared<T>(std::move(item));
    index = this->detail_add(item_ptr);
    return index;
}

template <typename T, typename Hash, typename KeyEqual>
INLINE_NEVER
size_t UniqueList<T, Hash, KeyEqual>::insert(std::shared_ptr<T> item)
{
    const auto* p = item.get();
    if (!p)
    {
        throw std::invalid_argument("Cannot upsert a null shared_ptr");
    }
    size_t index = this->detail_find(p);
    if (index != detail_npos)
    {
        return index; // Item already exists, return its index
    }
    index = this->detail_add(std::move(item));
    return index;
}

template <typename T, typename Hash, typename KeyEqual>
INLINE_NEVER
std::optional<size_t> UniqueList<T, Hash, KeyEqual>::find(const T& item) const
{
    const void* p = &item;
    size_t index = this->detail_find(p);
    if (index != detail_npos)
    {
        return index; // Item found, return its index
    }
    return std::nullopt; // Item not found
}

template <typename T, typename Hash, typename KeyEqual>
FLATTEN
size_t UniqueList<T, Hash, KeyEqual>::call_hash(const void* pvoid) const
{
    if (!pvoid)
    {
        // Handle null pointers gracefully
        return 0;
    }
    const auto& ref = *static_cast<const T*>(pvoid);
    return m_hash(ref);
}

template <typename T, typename Hash, typename KeyEqual>
FLATTEN
bool UniqueList<T, Hash, KeyEqual>::call_equal(const void* lhs, const void* rhs) const
{
    if (!lhs || !rhs)
    {
        // Handle null pointers gracefully
        return lhs == rhs;
    }
    const auto& lhs_ref = *static_cast<const T*>(lhs);
    const auto& rhs_ref = *static_cast<const T*>(rhs);
    return m_equal(lhs_ref, rhs_ref);
}
    
} // namespace tg::data::specialized
