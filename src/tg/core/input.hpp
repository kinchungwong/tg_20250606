#include <stdexcept>
#include "tg/core/input.fwd.hpp"

namespace tg::core
{

template <typename T>
Input<T>::Input()
    : StepData(std::type_index(typeid(T)), StepDataUsage::Input)
{
}

template <typename T>
const T& Input<T>::operator*() const
{
    return *static_cast<const T*>(this->read());
}

template <typename T>
const T* Input<T>::operator->() const
{
    return static_cast<const T*>(this->read());
}

} // namespace tg::core
