/**
 * @file carryless.cpp
 * @brief Source file for basic carryless functions for standard interger
 * types.
 */
#include <cstdlib>
#include <cstdint>
#include "tg/data/intmath/bitperm/carryless.hpp"
#include "tg/common/project_macros.hpp"

namespace tg::data::intmath::bitperm
{

INLINE_NEVER
uint64_t clmul_u64_portable_slow(uint64_t a, uint64_t b)
{
    constexpr uint64_t one = 1u;
    uint64_t result = 0u;
    for (int i = 0; i < 64; ++i)
    {
        if (b & (one << i))
        {
            result ^= (a << i);
        }
    }
    return result;
}

INLINE_NEVER
HOTPATH
uint64_t clmul_u64_portable_unrolled(uint64_t a, uint64_t b)
{
    uint64_t result = 0u;
    for (int i = 0; i < 64; i += 8)
    {
        uint64_t a0 = (b & (1u << 0)) ? (a << 0) : 0u;
        uint64_t a1 = (b & (1u << 1)) ? (a << 1) : 0u;
        uint64_t a2 = (b & (1u << 2)) ? (a << 2) : 0u;
        uint64_t a3 = (b & (1u << 3)) ? (a << 3) : 0u;
        uint64_t a4 = (b & (1u << 4)) ? (a << 4) : 0u;
        uint64_t a5 = (b & (1u << 5)) ? (a << 5) : 0u;
        uint64_t a6 = (b & (1u << 6)) ? (a << 6) : 0u;
        uint64_t a7 = (b & (1u << 7)) ? (a << 7) : 0u;
        result ^= (a0 ^ a1 ^ a2) ^ (a3 ^ a4 ^ a5) ^ a6 ^ a7;
        a <<= 8;
        b >>= 8;
    }
    return result;
}

} // namespace tg::data::intmath::bitperm
