#include "tg/core/fwd.hpp"
#include "tg/core/step.hpp"
#include "tg/core/step_info.hpp"

namespace tg::core
{

Step::Step(StepInfoPtr step_info)
    : m_step_info(std::move(step_info))
    , m_create_step_info_func(nullptr)
{
    if (!m_step_info)
    {
        throw std::runtime_error("StepInfo cannot be null.");
    }
}

Step::Step(CreateStepInfoFunc create_step_info_func)
    : m_step_info(nullptr)
    , m_create_step_info_func(std::move(create_step_info_func))
{
    if (!m_create_step_info_func)
    {
        throw std::runtime_error("CreateStepInfoFunc cannot be null.");
    }
}

Step::~Step()
{
}

StepInfo& Step::info()
{
    if (!m_step_info)
    {
        if (!m_create_step_info_func)
        {
            throw std::runtime_error("StepInfo is not initialized.");
        }
        m_step_info = m_create_step_info_func();
        if (!m_step_info)
        {
            throw std::runtime_error("StepInfo creation function returned null.");
        }
        m_create_step_info_func = nullptr; // Clear the function to prevent re-creation.
    }
    return *m_step_info;
}

void Step::pre_execute_validation(const std::vector<VarData>& data)
{
    const auto& step_info = this->info();
    if (data.size() != step_info.data_count())
    {
        throw std::runtime_error(
            "Step::pre_execute_validation: " + step_info.shortname() + 
            ": Data size does not match StepInfo data count.");
    }
    for (size_t k = 0u; k < data.size(); ++k)
    {
        const auto& data_info = step_info.get_data_info(k);
        const auto usage = data_info.m_usage;
        const auto& name = data_info.m_shortname;
        const auto expect_type = data_info.m_type;
        if (usage == DataUsage::Read || usage == DataUsage::Consume)
        {
            const auto& dak = data.at(k);
            if (!dak.has_value())
            {
                throw std::runtime_error(
                    "Step::pre_execute_validation: " + step_info.shortname() + 
                    ": Data " + name + " at index " + std::to_string(k) + " is required but not provided."
                );
            }
            if (dak.type() != expect_type)
            {
                throw std::runtime_error(
                    "Step::pre_execute_validation: " + step_info.shortname() + 
                    ": Data " + name + " at index " + std::to_string(k) + 
                    " has type mismatch. Expected: " + 
                    std::string(expect_type.name()) + ", Actual: " + 
                    std::string(dak.type().name())
                );
            }
        }
    }
}

void Step::post_execute_validation(const std::vector<VarData>& data)
{
    const auto& step_info = this->info();
    if (data.size() != step_info.data_count())
    {
        throw std::runtime_error(
            "Step::post_execute_validation: " + step_info.shortname() + 
            ": Data size does not match StepInfo data count.");
    }
    for (size_t k = 0u; k < data.size(); ++k)
    {
        const auto& data_info = step_info.get_data_info(k);
        const auto usage = data_info.m_usage;
        const auto& name = data_info.m_shortname;
        const auto expect_type = data_info.m_type;
        if (usage == DataUsage::Write)
        {
            const auto& dak = data.at(k);
            if (!dak.has_value())
            {
                throw std::runtime_error(
                    "Step::post_execute_validation: " + step_info.shortname() + 
                    ": Data " + name + " at index " + std::to_string(k) + 
                    " is required but not provided."
                );
            }
            if (dak.type() != expect_type)
            {
                throw std::runtime_error(
                    "Step::post_execute_validation: " + step_info.shortname() + 
                    ": Data " + name + " at index " + std::to_string(k) + 
                    " has type mismatch. Expected: " + 
                    std::string(expect_type.name()) + ", Actual: " + 
                    std::string(dak.type().name())
                );
            }
        }
    }    
}

} // namespace tg::core
