#pragma once
#include "tg/core/fwd.hpp"

namespace tg::core
{

class StepExecutor
{
public:
    StepExecutor(Step& step, ExecData& exec_data);
    ~StepExecutor();
    void execute();

private:
    void detail_pre_execute();
    void detail_post_execute();
    void detail_pre_execute_data_callback(StepData& data);
    void detail_post_execute_data_callback(StepData& data);

private:
    StepExecutor(const StepExecutor&) = delete;
    StepExecutor(StepExecutor&&) = delete;
    StepExecutor& operator=(const StepExecutor&) = delete;
    StepExecutor& operator=(StepExecutor&&) = delete;

private:
    Step* m_step;
    ExecData* m_exec_data;
};

} // namespace tg::core
