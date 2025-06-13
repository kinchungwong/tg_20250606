#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <type_traits>

// Forward declarations
namespace tg::data::specialized { class OpaquePtrKey; }
namespace std { template <> struct hash<tg::data::specialized::OpaquePtrKey>; }

namespace tg::data::specialized
{

/**
 * @brief Allows the use of any class object's address as a hashable
 * key in standard unordered containers such as ```unordered_map```.
 * 
 * @details The address is obfuscated to prevent unintended access
 * or mis-implied ownership.
 * 
 * @note This class resolves a design defect in OpaqueSubIndex,
 * where due to the non-bijective nature of its hash function
 * and the omission of the full address, it is possible that
 * keys created from two different objects can compare equal.
 * While hash collisions are inevitable and therefore acceptable,
 * the spurious result of the equality comparator cannot be accepted.
 */
class OpaquePtrKey
{
public:
    /**
     * @brief Default constructor that creates an empty OpaquePtrKey.
     */
    OpaquePtrKey() noexcept
        : m_type{std::type_index(typeid(void))}
        , m_shuffled{0u}
    {}
    OpaquePtrKey(const OpaquePtrKey&) noexcept = default;
    OpaquePtrKey(OpaquePtrKey&&) noexcept = default;
    OpaquePtrKey& operator=(const OpaquePtrKey&) noexcept = default;
    OpaquePtrKey& operator=(OpaquePtrKey&&) noexcept = default;
    ~OpaquePtrKey() noexcept = default;

private:
    /**
     * @brief Private constructor. Use create() methods to create an OpaquePtrKey.
     * @param type The type index of the object.
     * @param opaque_ptr The pointer to the object, which will be obfuscated.
     */
    OpaquePtrKey(std::type_index type, const void* opaque_ptr) noexcept
        : m_type{type}
        , m_shuffled{stc_shuffle(opaque_ptr)}
    {}

public:
    /**
      * @brief Creates an OpaquePtrKey from a reference to a class object.
      * @tparam T The type of the class object. Must be a class type.
      * @param value_ref A reference to the class object.
      * @return An OpaquePtrKey representing the class object.
      */
    template <typename T>
    static auto create(const T& value_ref) noexcept 
        -> std::enable_if_t<std::is_class_v<T>, OpaquePtrKey>
    {
        return OpaquePtrKey{std::type_index(typeid(T)), std::addressof(value_ref)};
    }

    //! @todo Implement deleted methods to prevent creation from expiring values unless they are smart pointers.

    /**
     * @brief Creates an OpaquePtrKey from a ```std::unique_ptr```.
     * @returns OpaquePtrKey for the pointer, or an empty OpaquePtrKey if the
     * pointer is null.
     */
    template <typename T>
    static OpaquePtrKey create(const std::unique_ptr<T>& value_up) noexcept
    {
        if (!value_up)
        {
            return OpaquePtrKey{};
        }
        return OpaquePtrKey{std::type_index(typeid(T)), value_up.get()};
    }

    /**
     * @brief Creates an OpaquePtrKey from a ```std::shared_ptr```.
     * @returns OpaquePtrKey for the pointer, or an empty OpaquePtrKey if the
     * pointer is null.
     */
    template <typename T>
    static OpaquePtrKey create(const std::shared_ptr<T>& value_sp) noexcept
    {
        if (!value_sp)
        {
            return OpaquePtrKey{};
        }
        return OpaquePtrKey{std::type_index(typeid(T)), value_sp.get()};
    }

    /**
     * @brief Creates an OpaquePtrKey from a ```std::weak_ptr```.
     * @returns OpaquePtrKey for the pointer, or an empty OpaquePtrKey if the
     * weak pointer is null or expired.
     */
    template <typename T>
    static OpaquePtrKey create(const std::weak_ptr<T>& value_wp) noexcept
    {
        if (auto sp = value_wp.lock())
        {
            return OpaquePtrKey{std::type_index(typeid(T)), sp.get()};
        }
        return OpaquePtrKey{};
    }

public:
    bool operator==(const OpaquePtrKey& other) const noexcept
    {
        return m_type == other.m_type &&
               m_shuffled == other.m_shuffled;
    }
    bool operator!=(const OpaquePtrKey& other) const noexcept 
    {
        return !(*this == other);
    }
    size_t get_hash() const noexcept
    {
        /**
         * @todo Consider using a more robust hash combine function.
         */
        return std::hash<std::type_index>{}(m_type) ^ std::hash<std::uintptr_t>{}(m_shuffled);
    }

private:
    /**
     * @brief Obfuscate the pointer without causing false aliasing.
     * 
     * @note The function is required to be bijective, i.e. 
     * ```(P1 == P2) if and only if (SHUFFLE(P1) == SHUFFLE(P2))```
     * 
     * @note To maintain at least the same number of bits, std::uintptr_t is used.
     * 
     * @todo Implement
     */
    static std::uintptr_t stc_shuffle(const void* ptr) noexcept
    {
        std::uintptr_t shuffled = reinterpret_cast<std::uintptr_t>(ptr);
        return shuffled;
    }

private:
    std::type_index m_type;
    std::uintptr_t m_shuffled;
};

} // namespace tg::data::specialized

/**
 * @brief Specialization for ```std::hash<OpaquePtrKey>```
 */
template <>
struct std::hash<tg::data::specialized::OpaquePtrKey>
{
    std::size_t operator()(const tg::data::specialized::OpaquePtrKey& id) const noexcept
    {
        return id.get_hash();
    }
};
