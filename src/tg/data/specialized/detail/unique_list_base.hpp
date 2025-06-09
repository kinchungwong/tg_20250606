#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <optional>
#include <utility>
#include <functional>
#include <type_traits>

namespace tg::data::specialized::detail
{

class UniqueListBase
{
protected:
    UniqueListBase();
    explicit UniqueListBase(const UniqueListBase& other);
    explicit UniqueListBase(UniqueListBase&& other);
    UniqueListBase& operator=(const UniqueListBase& other);
    UniqueListBase& operator=(UniqueListBase&& other);

public:
    virtual ~UniqueListBase();
    void clear();
    size_t size() const;
    void reserve(size_t capacity, float max_load_factor = 0.75f);

protected:
    static constexpr size_t detail_npos = ~static_cast<size_t>(0);
    size_t detail_find(const void* item) const;
    size_t detail_add(std::shared_ptr<void> item);

protected:
    virtual size_t call_hash(const void* pvoid) const = 0;
    virtual bool call_equal(const void* lhs, const void* rhs) const = 0;

protected:
    struct HashCaller
    {
        HashCaller(UniqueListBase& ref_base);
        size_t operator()(const void* pvoid) const;
        UniqueListBase* m_pbase;
    };
    struct EqualCaller
    {
        EqualCaller(UniqueListBase& ref_base);
        bool operator()(const void* lhs, const void* rhs) const;
        UniqueListBase* m_pbase;
    };
 
protected:
    std::vector<std::shared_ptr<void>> m_items;
    std::unordered_map<const void*, size_t, HashCaller, EqualCaller> m_lookup;
};
    
} // namespace tg::data::specialized::detail
