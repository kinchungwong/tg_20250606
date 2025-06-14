#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include "tg/data/intmath/bitperm/hypercube_perm.hpp"
#include "tg/data/intmath/bitperm/bit_rotate.hpp"
#include "tg/data/intmath/bitperm/carryless.hpp"

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
     * @brief Bijective obfuscation of the pointer, serving the needs of both
     * the hash functor and the equality functor.
     * 
     * @details
     * Unlike a hash function, the bijective obfuscation has to satisfy:
     * ```(lhs == rhs)``` if and only if ```(SHUFFLE(lhs) == SHUFFLE(lhs))```
     * 
     * Because bijective-ness requires at least the same number of bits in
     * the output, ```std::uintptr_t``` is used for the output type.
     * 
     * Bijective-ness implies invertibility. Theoretically a lookup table
     * would suffice; practically, it is easier to invert by executing the
     * bitwise steps in the procedure in reverse order.
     * 
     * @warning ((Known Implementation Defects)) Refer to comments in code.
     */
    static std::uintptr_t stc_shuffle(const void* ptr) noexcept
    {
        /**
         * @todo We don't have a good choice of shuffle function yet.
         * Here we just do the bare minimum to make the pointer value
         * not looking like an ordinary pointer value.
         * 
         * @note We require std::uintptr_t to be same as uint64_t because we
         * simply don't know how to implement if this is not the case.
         */
        using namespace tg::data::intmath::bitperm;
        static_assert(std::is_same_v<std::uintptr_t, uint64_t>, "Unable to shuffle bits if uintptr_t is not uint64_t");
        /**
         * @brief Some random stuff that can be used to obfuscate the pointer.
         * 
         * @warning Because the address can vary between different builds and 
         * runs of the program, the resulting obfuscation function can cause
         * unreliable test results.
         */
        uint64_t some_random_stuff = static_cast<uint64_t>(
            reinterpret_cast<std::uintptr_t>(
                &stc_shuffle
            )
        );
        uint64_t input = static_cast<uint64_t>(
            reinterpret_cast<std::uintptr_t>(ptr)
        );
        uint64_t shuffled = input;
        constexpr auto reverse = [](uint64_t value) {
            return hc_u64_mirror0(hc_u64_mirror1(hc_u64_mirror2(
                hc_u64_mirror3(hc_u64_mirror4(hc_u64_mirror5(
                    value
                )))
            )));
        };
        /**
         * @brief Obfuscation steps.
         *
         * @details
         * Just some random bitwise operations to obfuscate the pointer.
         * 
         * @warning The current code below is probably NOT bijective,
         * in particular because XOR can cause catastrophic cancellations
         * (zeroing out bits) when both arguments are derived from the input.
         * 
         * @note This defect has been observed in practice, mainly because when
         * the input is a pointer, a bunch of objects of the same type and allocated
         * in quick succession will often have addresses that differ only by small
         * amounts (the aligned object size), and in unfortunate cases where it's
         * a power of two, these addresses may only differ by a single bit.
         * Constructing a not-so-bijective hash function that differentiates
         * between such addresses is not easy.
         * 
         * @note A full analysis of (XOR, ROL, ROR) always require carryless algebra;
         * no such analysis is performed here.
         * 
         * @todo Implement true bijective obfuscation. Typically, the key material
         * should be used to construct a sequence of single-input single-output bijective
         * bitwise operations, each of which is bijective, and only uses the immediate
         * result of the previous operation. (This is sometimes called a "block", and
         * may consist of a sequence of bitwise operations.) Other than for the 
         * construction of the schedule, the key material shall not be used as an input
         * to any of the bitwise operations.
         */
        some_random_stuff = clmul_u64_portable_unrolled(rotate_right_u64(some_random_stuff, 12), rotate_right_u64(input, 15));
        shuffled ^= some_random_stuff;
        some_random_stuff = clmul_u64_portable_unrolled(rotate_right_u64(some_random_stuff, 16), rotate_right_u64(input, 7));
        shuffled ^= reverse(some_random_stuff);
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
