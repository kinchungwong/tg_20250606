#include "tg/core/step_executor.hpp"
#include "tg/core/step.hpp"
#include "tg/core/step_data.hpp"
#include "tg/core/data_callback.hpp"
#include "tg/core/exec_data.hpp"

namespace tg::core
{

StepExecutor::StepExecutor(Step& step, ExecData& exec_data)
    : m_step(&step), m_exec_data(&exec_data)
{
}

StepExecutor::~StepExecutor()
{
}

void StepExecutor::execute()
{
    /**
     * @todo Analyze what if exception is thrown. Ensure proper unwind,
     * algorithm doesn't go into infinite loop. (Don't just catch;
     * some objects require a notification to unblock.)
     */
    this->detail_pre_execute();
    m_step->execute();
    this->detail_post_execute();
}

void StepExecutor::detail_pre_execute()
{
    DataCallback callback{[this](StepData& data) -> void {
        this->detail_pre_execute_data_callback(data);
    }};
    m_step->visit_data(callback);
}

void StepExecutor::detail_post_execute()
{
    DataCallback callback{[this](StepData& data) -> void {
        this->detail_post_execute_data_callback(data);
    }};
    m_step->visit_data(callback);
}

void StepExecutor::detail_pre_execute_data_callback(StepData& data)
{
    auto usage = data.usage();
    if (usage == StepDataUsage::Input)
    {
        // Sync inputs from ExecData to StepData.
        data.sync_from_exec_data(*(this->m_exec_data));
    }
}

void StepExecutor::detail_post_execute_data_callback(StepData& data)
{
    auto usage = data.usage();
    if (usage == StepDataUsage::Output)
    {
        // Sync outputs from StepData to ExecData.
        data.sync_to_exec_data(*(this->m_exec_data));
    }
}

} // namespace tg::core
