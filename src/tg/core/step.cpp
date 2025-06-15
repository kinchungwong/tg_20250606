#include "tg/core/fwd.hpp"
#include "tg/core/step.hpp"
#include "tg/core/step_info.hpp"

namespace tg::core
{

Step::Step(StepInfoPtr step_info)
    : m_step_info(std::move(step_info))
    , m_init_fault(false)
    , m_exec_fault(false)
{
}

Step::~Step()
{
}

StepInfo& Step::info()
{
    this->trap_on_fault();
    if (!m_step_info)
    {
        m_step_info = this->create_step_info();
        if (!m_step_info)
        {
            this->m_init_fault = true;
            throw std::runtime_error(
                "Step::info(): creation function "
                "Derived(Step)::create_step_info() returned null."
            );
        }
    }
    return *m_step_info;
}

void Step::pre_execute_validation(const std::vector<VarData>& data)
{
    this->trap_on_fault();
    const auto& step_info = this->info();
    if (data.size() != step_info.data_count())
    {
        this->m_exec_fault = true;
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
                this->m_exec_fault = true;
                throw std::runtime_error(
                    "Step::pre_execute_validation: " + step_info.shortname() + 
                    ": Data " + name + " at index " + std::to_string(k) + " is required but not provided."
                );
            }
            if (dak.type() != expect_type)
            {
                this->m_exec_fault = true;
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
    this->trap_on_fault();
    const auto& step_info = this->info();
    if (data.size() != step_info.data_count())
    {
        this->m_exec_fault = true;
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
                this->m_exec_fault = true;
                throw std::runtime_error(
                    "Step::post_execute_validation: " + step_info.shortname() + 
                    ": Data " + name + " at index " + std::to_string(k) + 
                    " is required but not provided."
                );
            }
            if (dak.type() != expect_type)
            {
                this->m_exec_fault = true;
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

StepInfoPtr Step::create_step_info()
{
    return std::make_shared<StepInfo>();
}

void Step::trap_on_fault() const
{
    if (m_init_fault || m_exec_fault)
    {
        throw std::runtime_error(
            "Step::info(): StepInfo not initialized due to previous fault."
        );
    }
}

} // namespace tg::core
