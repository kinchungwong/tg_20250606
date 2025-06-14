#include <mutex>
#include "tg/core/utility/step_info_summary.hpp"
#include "tg/core/step_info.hpp"

namespace tg::core::utility
{

StepInfoSummary::StepInfoSummary(const StepInfo& step_info)
    : m_step_info(&step_info)
{}

StepInfoSummary::~StepInfoSummary()
{}

void StepInfoSummary::generate(TextLineCallback text_line_callback) const
{
    if (!text_line_callback)
    {
        return;
    }
    const auto& uts_map = StepInfoSummary::stc_usage_to_string();
    text_line_callback("(Step name) " + m_step_info->shortname());
    text_line_callback("(Step type) " + std::string(m_step_info->step_type().name()));
    size_t data_count = m_step_info->data_count();
    text_line_callback("(Data count) " + std::to_string(data_count));
    for (size_t k = 0u; k < data_count; ++k)
    {
        const DataInfoTuple& dit = m_step_info->get_data_info(k);
        text_line_callback(
            "(Data " + std::to_string(k) + ")" + 
            " (Name) " + dit.m_shortname + 
            " (Usage) " + uts_map.at(dit.m_usage) +
            " (Type) " + dit.m_type.name()
        );
    }
}

const std::unordered_map<DataUsage, std::string>& StepInfoSummary::stc_usage_to_string()
{
    static std::once_flag stc_map_flag;
    static std::unordered_map<DataUsage, std::string> stc_map;
    std::call_once(stc_map_flag, []() -> void {
        stc_map.emplace(DataUsage::Read, "Read");
        stc_map.emplace(DataUsage::Write, "Write");
        stc_map.emplace(DataUsage::Consume, "Consume");
    });
    return stc_map;
}

} // namespace tg::core::utility
