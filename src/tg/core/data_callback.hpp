#pragma once
#include "tg/core/fwd.hpp"
#include "tg/core/data_visitor.hpp"

namespace tg::core
{

class DataCallback
    : public DataVisitor
{
public:
    explicit DataCallback(std::function<void(StepData&)> callback);
    ~DataCallback();
    void visit(StepData& data) final;

private:
    DataCallback(const DataCallback&) = delete;
    DataCallback& operator=(const DataCallback&) = delete;
    DataCallback(DataCallback&&) = delete;
    DataCallback& operator=(DataCallback&&) = delete;

private:
    std::function<void(StepData&)> m_callback;
};

} // namespace tg::core
