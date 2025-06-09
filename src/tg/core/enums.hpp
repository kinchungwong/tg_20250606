#pragma once
#include <cstdint>

namespace tg::core
{

enum class StepDataUsage : uint32_t
{
    None = 0u,
    Input = 1u,
    Output = 2u,
};

} // namespace tg::core
