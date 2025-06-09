#pragma once
#include <functional>
#include <type_traits>

namespace tg::data::specialized::detail
{

template <
    typename ValueType, 
    typename Hash,
    bool = std::is_assignable_v<
            size_t&, 
            decltype(std::declval<const Hash&>()(std::declval<const ValueType&>()))
    >
>
struct is_hash;

template <typename ValueType, typename Hash>
struct is_hash<ValueType, Hash, false> : std::false_type {};

template <typename ValueType, typename Hash>
struct is_hash<ValueType, Hash, true> : std::true_type {};

template <
    typename ValueType,
    typename Equal,
    bool = std::is_assignable_v<
        bool&,
        decltype(std::declval<const Equal&>()(std::declval<const ValueType&>(), std::declval<const ValueType&>()))
    >
>
struct is_equal;

template <typename ValueType, typename Equal>
struct is_equal<ValueType, Equal, false> : std::false_type {};

template <typename ValueType, typename Equal>
struct is_equal<ValueType, Equal, true> : std::true_type {};
    
} // namespace tg::data::specialized::detail
