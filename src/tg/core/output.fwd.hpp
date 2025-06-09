#pragma once
#include "tg/core/fwd.hpp"
#include "tg/core/step_data.hpp"

namespace tg::core
{

/**
 * @brief Typed output data object for a Step.
 * @note Dereference operators should only be used after the emplace() method.
 */
template <typename T>
class Output : public StepData
{
public:
    Output();
    
    /**
     * @brief Constructs the output data in-place with the given arguments.
     * 
     * @tparam Args Types of the arguments to be forwarded to the constructor of T.
     * @param args Arguments to be forwarded to the constructor of T.
     * @return Reference to the constructed object.
     */
    template <typename... Args>
    T& emplace(Args&&... args);

    /**
     * @brief Dereference operator to access the output data.
     * @note This operator should only be used after emplace() has been called.
     */
    T& operator*();

    /**
     * @brief Arrow operator to access members of the output data.
     * @note This operator should only be used after emplace() has been called.
     */
    T* operator->();
};

} // namespace tg::core
