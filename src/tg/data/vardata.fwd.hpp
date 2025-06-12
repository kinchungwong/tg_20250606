#pragma once
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <string_view>

namespace tg::data
{

/**
 * @brief Open variant type via type-erasure.
 * 
 * @details
 * VarData is an open variant type that allows storing any data type
 * compatible with ```std::shared_ptr```.
 * 
 * Type-erasure is enabled using ```std::shared_ptr<void>```. Runtime
 * type checks are performed by capturing and comparing the 
 * ```std::type_index``` at each time-of-use.
 * 
 * Accessing the stored data (writing or reading) requires complete type
 * information of that data.
 * 
 * Classes that merely manages instances of VarData, such as standard
 * library containers, do not require the complete type information of
 * its data.
 * 
 * @note The stored data is always treated as non-const-qualified, even
 * on const-qualified VarData instances and methods.
 */
class VarData
{
public:
    VarData();
    VarData(const VarData& other);
    VarData(VarData&& other);
    VarData& operator=(const VarData& other);
    VarData& operator=(VarData&& other);
    ~VarData();

public:
    template <typename T>
    VarData(std::shared_ptr<T> value);

    template <typename T>
    VarData& operator=(std::shared_ptr<T> value);

public:
    void clear();
    std::type_index type() const;
    void* raw() const;
    bool has_value() const;
    bool operator!() const;
    bool operator==(const VarData& other) const;
    bool operator!=(const VarData& other) const;

    template <typename T, typename... Args>
    T& emplace(Args&&... args);

    template <typename T>
    std::shared_ptr<T> detach_as();

    template <typename T>
    T& as() const;

    template <typename T>
    T* ptr() const;

private:
    template <typename T>
    void type_guard(std::string_view message) const;

    void type_guard_impl(std::string_view message, std::type_index expect) const;
    void null_guard(std::string_view message) const;

private:
    std::type_index m_type;
    std::shared_ptr<void> m_ptr;    
};

} // namespace tg::data
