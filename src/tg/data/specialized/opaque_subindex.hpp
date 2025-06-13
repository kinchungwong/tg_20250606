#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <type_traits>

// Forward declarations
namespace tg::data::specialized { class OpaqueSubindex; }
namespace std { template <> struct hash<tg::data::specialized::OpaqueSubindex>; }

namespace tg::data::specialized
{

/**
 * @brief A property key class that uniquely identifies a subindex associated
 * with an owner object.
 * 
 * @tparam T The type of the opaque owner object. This class does not require
 * the complete type of T. However, T cannot be a pointer or reference type.
 *
 * @details This class is an opaque identifier that associates with a specific
 * owner object and a subindex. It is designed to be opaque in the sense that
 * the owner's address cannot be recovered from its hash.
 * 
 * The class supports standard equality comparison and hashing, making it 
 * suitable for use as a key in standard unordered containers.
 * 
 * This class is trivially copyable, movable, assignable, and destructible.
 * 
 * @note It is meaningless to create an OpaqueSubindex from an expiring value.
 * Currently, this class does not prevent such usage, but users should take
 * care of this potential bug.
 * 
 * @note Do not create OpaqueSubindex from the address of owner objects that
 * are value-stored in std::vector, since their addresses are invalidated
 * when the vector is resized or reallocated. Instead, store such owner objects
 * as smart pointers inside a std::vector.
 * 
 * @note While the class is robust against null pointers, users should be aware
 * of the risk of errors from such usage.
 * 
 * @note This class will never access the owner object, and will never
 * dereference the owner pointer.
 * 
 * @note This class can be vulnerable to an unintended collision attack if the
 * original owner object was freed and then another object of the same type was
 * allocated at the same address.
 */
class OpaqueSubindex
{
public:
    constexpr OpaqueSubindex() noexcept
        : OpaqueSubindex{0u, 0u, 0u} {}

#if 0
    // /**
    //  * @brief (Deleted) Prevents OpaqueSubindex creation from an expiring value.
    //  * @fixme
    //  */
    // template <typename T>
    // static OpaqueSubindex create(T&& owner_ref, uint64_t subindex) noexcept = delete;
#endif

    /**
     * @brief Creates an OpaqueSubindex from the address of an owner object and a subindex.
     */
    template <typename T>
    static auto create(const T& owner_ref, uint64_t subindex) noexcept 
        -> std::enable_if_t<
            (!std::is_pointer_v<T> && !std::is_reference_v<T> && !std::is_array_v<T>),
            OpaqueSubindex
        >
    {
        return OpaqueSubindex{
            detail_typehash<T>(),
            detail_ptrhash(std::addressof(owner_ref)),
            subindex
        };
    }

    /**
     * @brief Creates an OpaqueSubindex from the address of an owner object and a subindex.
     */
    template <typename T>
    static OpaqueSubindex create(const T* owner_p, uint64_t subindex) noexcept
    {
        return OpaqueSubindex{
            detail_typehash<T>(),
            detail_ptrhash(owner_p),
            subindex
        };
    }

    /**
     * @brief Creates an OpaqueSubindex from a pointer to an owner object and a subindex.
     * @param owner_up A pointer to the owner object. Can be null.
     * @param index The subindex associated with the owner object.
     * @return An OpaqueSubindex representing the owner and subindex.
     * @exception None. This method does not throw, even if the pointer is null.
     */
    template <typename T>
    static OpaqueSubindex create(const std::unique_ptr<T>& owner_up, uint64_t index) noexcept
    {
        return OpaqueSubindex{
            detail_typehash<T>(),
            detail_ptrhash(owner_up.get()),
            index
        };
    }

    /**
     * @brief Creates an OpaqueSubindex from a shared pointer to an owner object and a subindex.
     * @param owner_sp A shared pointer to the owner object.
     * @param index The subindex associated with the owner object.
     * @return An OpaqueSubindex representing the owner and subindex.
     * @exception None. This method does not throw, even if the shared pointer is empty.
     */
    template <typename T>
    static OpaqueSubindex create(const std::shared_ptr<T>& owner_sp, uint64_t index) noexcept
    {
        return OpaqueSubindex{
            detail_typehash<T>(),
            detail_ptrhash(owner_sp.get()),
            index
        };
    }

    /**
     * @brief Creates an OpaqueSubindex from a weak pointer to an owner object and a subindex.
     * @param owner_wp A weak pointer to the owner object.
     * @param index The subindex associated with the owner object.
     * @return An OpaqueSubindex representing the owner and subindex. If the weak pointer is
     * expired, it will behave as if the pointer is null, and the hash of that null pointer
     * will be used.
     * @exception None. This method does not throw, even if the weak pointer is empty or expired.
     */
    template <typename T>
    static OpaqueSubindex create(const std::weak_ptr<T>& owner_wp, uint64_t index) noexcept
    {
        return OpaqueSubindex{
            detail_typehash<T>(),
            detail_ptrhash(owner_wp.lock().get()),
            index
        };
    }
    constexpr OpaqueSubindex(const OpaqueSubindex&) noexcept = default;
    constexpr OpaqueSubindex(OpaqueSubindex&&) noexcept = default;
    constexpr OpaqueSubindex& operator=(const OpaqueSubindex&) noexcept = default;
    constexpr OpaqueSubindex& operator=(OpaqueSubindex&&) noexcept = default;
    ~OpaqueSubindex() noexcept = default;
    constexpr bool operator==(const OpaqueSubindex& other) const noexcept
    {
        return m_type_hash == other.m_type_hash &&
               m_ptr_hash == other.m_ptr_hash &&
               m_subindex == other.m_subindex;
    }
    constexpr bool operator!=(const OpaqueSubindex& other) const noexcept 
    {
        return !(*this == other);
    }
    constexpr size_t get_hash() const noexcept
    {
        /**
         * @todo Consider using a more robust hash combine function.
         */
        return m_type_hash ^ m_ptr_hash ^ m_subindex;
    }
private:
    constexpr OpaqueSubindex(size_t type_hash, size_t ptr_hash, size_t subindex) noexcept
        : m_type_hash{type_hash}
        , m_ptr_hash{ptr_hash}
        , m_subindex{subindex}
    {}
    static size_t detail_ptrhash(const void* ptr) noexcept
    {
        return std::hash<const void*>{}(ptr);
    }
    template <typename T>
    static size_t detail_typehash() noexcept
    {
        return typeid(T).hash_code();
    }
private:
    size_t m_type_hash;
    size_t m_ptr_hash;
    size_t m_subindex;
};

} // namespace tg::data::specialized
   
/**
 * @brief Specialization of std::hash for tg::data::specialized::OpaqueSubindex.
 */
template <>
struct std::hash<tg::data::specialized::OpaqueSubindex>
{
    constexpr std::size_t operator()(const tg::data::specialized::OpaqueSubindex& id) const noexcept
    {
        return id.get_hash();
    }
};
