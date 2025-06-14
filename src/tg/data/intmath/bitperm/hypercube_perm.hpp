/**
 * @file hypercube_perm.hpp
 * @brief Header file for hypercube bit permutation functions 
 * for standard interger types.
 */
#pragma once
#include <cstdint>
#include "tg/common/project_macros.hpp"

namespace tg::data::intmath::bitperm
{

/**
 * @brief ```mirror0```: swaps the bits of a 64-bit unsigned integer
 * by mirroring on the basis of (the 0-th bit of (the bit index of each bit)).
 *
 * @details 
 * In hypercube bit permutation, the bit index functional is:
 * ```(bin.abcdex) -> (bin.abcdeX)``` where ```X := not(x)```.
 */
INLINE_ALWAYS
constexpr
inline uint64_t hc_u64_mirror0(uint64_t input)
{
    // 01010101... pattern
    constexpr uint64_t mask = UINT64_C(0x5555555555555555);
    return ((input & mask) << 1) | ((input >> 1) & mask);
}

/**
 * @brief ```mirror1```: swaps the bits of a 64-bit unsigned integer
 * by mirroring on the basis of (the 1-th bit of (the bit index of each bit)).
 *
 * @details 
 * In hypercube bit permutation, the bit index functional is:
 * ```(bin.abcdxe) -> (bin.abcdXe)``` where ```X := not(x)```.
 */
INLINE_ALWAYS
constexpr
inline uint64_t hc_u64_mirror1(uint64_t input)
{
    // 00110011... pattern
    constexpr uint64_t mask = UINT64_C(0x3333333333333333);
    return ((input & mask) << 2) | ((input >> 2) & mask);
}
/**
 * @brief ```mirror2```: swaps the bits of a 64-bit unsigned integer
 * by mirroring on the basis of (the 2-th bit of (the bit index of each bit)).
 *
 * @details 
 * In hypercube bit permutation, the bit index functional is:
 * ```(bin.abcxde) -> (bin.abcXde)``` where ```X := not(x)```.
 */
INLINE_ALWAYS
constexpr
inline uint64_t hc_u64_mirror2(uint64_t input)
{
    // 00001111... pattern
    constexpr uint64_t mask = UINT64_C(0x0F0F0F0F0F0F0F0F);
    return ((input & mask) << 4) | ((input >> 4) & mask);
}

/**
 * @brief ```mirror3```: swaps the bits of a 64-bit unsigned integer
 * by mirroring on the basis of (the 3-th bit of (the bit index of each bit)).
 *
 * @details 
 * In hypercube bit permutation, the bit index functional is:
 * ```(bin.abxcde) -> (bin.abXcde)``` where ```X := not(x)```.
 */
INLINE_ALWAYS
constexpr
inline uint64_t hc_u64_mirror3(uint64_t input)
{
    // 0000000011111111... pattern
    constexpr uint64_t mask = UINT64_C(0x00FF00FF00FF00FF);
    return ((input & mask) << 8) | ((input >> 8) & mask);
}

/**
 * @brief ```mirror4```: swaps the bits of a 64-bit unsigned integer
 * by mirroring on the basis of (the 4-th bit of (the bit index of each bit)).
 *
 * @details 
 * In hypercube bit permutation, the bit index functional is:
 * ```(bin.aXbcde) -> (bin.aXbcde)``` where ```X := not(x)```.
 */
INLINE_ALWAYS
constexpr
inline uint64_t hc_u64_mirror4(uint64_t input)
{
    // set0(63..48)++set1(47..32)++set0(31..16)++set1(15..0)
    constexpr uint64_t mask = UINT64_C(0x0000FFFF0000FFFF);
    return ((input & mask) << 16) | ((input >> 16) & mask);
}

/**
 * @brief ```mirror5```: swaps the bits of a 64-bit unsigned integer
 * by mirroring on the basis of (the 5-th bit of (the bit index of each bit)).
 *
 * @details 
 * In hypercube bit permutation, the bit index functional is:
 * ```(bin.Xabcde) -> (bin.Xabcde)``` where ```X := not(x)```.
 */
INLINE_ALWAYS
constexpr
inline uint64_t hc_u64_mirror5(uint64_t input)
{
    // set0(63..32)++set1(31..0)
    constexpr uint64_t mask = UINT64_C(0x00000000FFFFFFFF);
    return ((input & mask) << 32) | ((input >> 32) & mask);
}


#if 0 // NOT IMPLEMENTED YET
// /**
//  * @brief ```gather0```: extracts bits on the basis of the 0-th bit
//  * of the bit index of each bit.
//  * 
//  * @param input The 64-bit unsigned integer to be bit-sorted.
//  * @returns The concatenation of the sorted low and high bits.
//  * 
//  * @details
//  * The lower 32 bits of the output contains the gathered and packed bits
//  * from the input where their source bit indices are of the pattern 
//  * ```(bin.xxxxx0)```.
//  * 
//  * The upper 32 bits of the output contains the gathered and packed bits
//  * from the input where their source bit indices are of the pattern
//  * ```(bin.xxxxx1)```.
//  * 
//  * In hypercube bit permutation, the operation equations is:
//  * ```(bin.abcdex) -> (bin.xabcde)```
//  */
// INLINE_ALWAYS
// inline uint64_t hc_u64_gather0(uint64_t input)
// {}

// /**
//  * @brief ```gather1```: extracts bits on the basis of the 1-th bit
//  * of the bit index of each bit.
//  * 
//  * @param input The 64-bit unsigned integer to be bit-sorted.
//  * @returns The concatenation of the sorted low and high bits.
//  * 
//  * @details
//  * The lower 32 bits of the output contains the gathered and packed bits
//  * from the input where their source bit indices are of the pattern 
//  * ```(bin.xxxx0x)```.
//  * 
//  * The upper 32 bits of the output contains the gathered and packed bits
//  * from the input where their source bit indices are of the pattern
//  * ```(bin.xxxx1x)```.
//  * 
//  * In hypercube bit permutation, the operation equations is:
//  * ```(bin.abcdxe) -> (bin.xabcde)```
//  */
// INLINE_ALWAYS
// inline uint64_t hc_u64_gather1(uint64_t input)
// {}

// /**
//  * @brief ```gather2```: extracts bits on the basis of the 2-th bit
//  * of the bit index of each bit.
//  * 
//  * @param input The 64-bit unsigned integer to be bit-sorted.
//  * @returns The concatenation of the sorted low and high bits.
//  * 
//  * @details
//  * The lower 32 bits of the output contains the gathered and packed bits
//  * from the input where their source bit indices are of the pattern 
//  * ```(bin.xxx0xx)```.
//  * 
//  * The upper 32 bits of the output contains the gathered and packed bits
//  * from the input where their source bit indices are of the pattern
//  * ```(bin.xxx1xx)```.
//  * 
//  * In hypercube bit permutation, the operation equations is:
//  * ```(bin.abcxde) -> (bin.xabcde)```
//  */
// INLINE_ALWAYS
// inline uint64_t hc_u64_gather2(uint64_t input)
// {}
#endif // NOT IMPLEMENTED YET

} // namespace tg::data::intmath::bitperm
