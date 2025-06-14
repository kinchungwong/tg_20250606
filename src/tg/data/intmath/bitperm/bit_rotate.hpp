/**
 * @file bit_rotate.hpp
 * @brief Header file for basic bit rotation functions
 * for standard interger types.
 */
#pragma once
#include <cstdint>
#include "tg/common/project_macros.hpp"

namespace tg::data::intmath::bitperm
{

/**
 * @brief Rotates the bits in the 64-bit input, by interpreting
 * the shift amount as number of bits toward the left (in the 
 * direction of higher bits).
 *
 * @param input The 64-bit unsigned integer to rotate.
 * @param shift The number of bit positions to rotate.
 */
INLINE_ALWAYS
constexpr 
inline uint64_t rotate_left_u64(uint64_t input, int32_t shift)
{
    uint32_t u_left = static_cast<uint32_t>(shift) & 63u;
    uint32_t u_right = (64u - u_left) & 63u;
    return (input << u_left) | (input >> u_right);
}

/**
 * @brief Rotates the bits in the 64-bit input, by interpreting
 * the shift amount as number of bits toward the right (in the 
 * direction of lower bits).
 *
 * @param input The 64-bit unsigned integer to rotate.
 * @param shift The number of bit positions to rotate.
 */
INLINE_ALWAYS
constexpr
inline uint64_t rotate_right_u64(uint64_t input, int32_t shift)
{
    uint32_t u_right = static_cast<uint32_t>(shift) & 63u;
    uint32_t u_left = (64u - u_right) & 63u;
    return (input << u_left) | (input >> u_right);
}

} // namespace tg::data::intmath::bitperm
