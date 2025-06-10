#include "tg/core/data_callback.hpp"

namespace tg::core
{

DataCallback::DataCallback(std::function<void(StepData&)> callback)
    : m_callback(std::move(callback))
{
    if (!m_callback)
    {
        throw std::runtime_error("DataCallback: callback cannot be null.");
    }
}

DataCallback::~DataCallback()
{
}

void DataCallback::visit(StepData& data)
{
    this->m_callback(data);
}

} // namespace tg::core
