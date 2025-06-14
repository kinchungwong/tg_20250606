#pragma once
#include <functional>
#include "tg/core/fwd.hpp"

namespace tg::core::utility
{

class StepInfoSummary
{
public:
    using TextLineCallback = std::function<void(const std::string&)>;

public:
    explicit StepInfoSummary(const StepInfo& step_info);
    ~StepInfoSummary();

public:
    void generate(TextLineCallback text_line_callback) const;

private:
    static const std::unordered_map<DataUsage, std::string>& stc_usage_to_string();

private:
    const StepInfo* m_step_info;
};


} // namespace tg::core::utility
