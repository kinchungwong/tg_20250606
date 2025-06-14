/**
 * @file carryless.hpp
 * @brief Header file for basic carryless functions for standard interger
 * types.
 */
#pragma once
#include <cstdint>

namespace tg::data::intmath::bitperm
{

/**
 * @brief Carryless multiplication, a portable but slow implementation.
 */
uint64_t clmul_u64_portable_slow(uint64_t a, uint64_t b);

/**
 * @brief Carryless multiplication, a portable and unrolled implementation.
 */
uint64_t clmul_u64_portable_unrolled(uint64_t a, uint64_t b);

} // namespace tg::data::intmath::bitperm
