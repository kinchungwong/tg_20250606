#pragma once
#include "tg/core/fwd.hpp"
#include "tg/core/step_data.hpp"

namespace tg::core
{

/**
 * @brief Typed input data object for a Step.
 */
template <typename T>
class Input : public StepData
{
public:
    Input();
    const T& operator*() const;
    const T* operator->() const;
};

} // namespace tg::core
