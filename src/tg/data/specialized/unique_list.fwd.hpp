#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <optional>
#include <utility>
#include <functional>
#include <type_traits>
#include "tg/data/specialized/detail/unique_list_base.hpp"
#include "tg/data/specialized/detail/unique_list_traits.hpp"

namespace tg::data::specialized
{

template <
    typename T,
    typename Hash = std::hash<T>,
    typename KeyEqual = std::equal_to<T>
>
class UniqueList;

template <typename T, typename Hash, typename KeyEqual>
class UniqueList : private detail::UniqueListBase
{
public:
    static_assert(detail::is_hash<T, Hash>::value,
        "Hash::operator() must return a size_t value when called with a const T&"
    );
    static_assert(detail::is_equal<T, KeyEqual>::value,
        "KeyEqual::operator() must return a bool value when called with two const T&"
    );

public:
    using value_type = T;
    using hasher = Hash;
    using key_equal = KeyEqual;

public:
    explicit UniqueList(const hasher& hash = hasher{}, const key_equal& equal = key_equal{});

    using detail::UniqueListBase::size;
    using detail::UniqueListBase::clear;
    using detail::UniqueListBase::reserve;

    size_t insert(const T& item);
    size_t insert(T&& item);
    size_t insert(std::shared_ptr<T> item);
    std::optional<size_t> find(const T& item) const;
    std::shared_ptr<T> at(size_t index) const;

    /**
     * @brief Returns a shallow copy of the UniqueList.
     * @returns A shared pointer to a new UniqueList instance that contains
     * a copy of the std::shared_ptr<T> pointers, in the same order as the
     * current instance.
     */
    std::shared_ptr<UniqueList> shallow_copy() const;

protected:
    size_t call_hash(const void* pvoid) const final;
    bool call_equal(const void* lhs, const void* rhs) const final;

private:
    using detail::UniqueListBase::detail_npos;

private:
    /** 
     * @brief Deleted copy constructor. Use shallow_copy() instead.
     */
    UniqueList(const UniqueList&) = delete;
    UniqueList(UniqueList&&) = delete;
    UniqueList& operator=(const UniqueList&) = delete;
    UniqueList& operator=(UniqueList&&) = delete;

private:
    hasher m_hash;
    key_equal m_equal;
};
    
} // namespace tg::data::specialized
