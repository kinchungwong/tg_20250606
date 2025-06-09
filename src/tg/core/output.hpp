#include <stdexcept>
#include "tg/core/output.fwd.hpp"

namespace tg::core
{

template <typename T>
Output<T>::Output()
    : StepData(std::type_index(typeid(T)))
{
}

template <typename T>
template <typename... Args>
T& Output<T>::emplace(Args&&... args)
{
    auto p = std::make_shared<T>(std::forward<Args>(args)...);
    this->assign(p);
    return *static_cast<T*>(this->read());
}

template <typename T>
T& Output<T>::operator*()
{
    return *static_cast<T*>(this->read());
}

template <typename T>
T* Output<T>::operator->()
{
    return static_cast<T*>(this->read());
}

} // namespace tg::core
