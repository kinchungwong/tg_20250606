#include "tg/core/usage_grid.hpp"
#include "tg/data/specialized/unique_list.hpp"
#include "tg/core/step.hpp"
#include "tg/core/step_data.hpp"
#include "tg/core/data_callback.hpp"

namespace tg::core
{

UsageGrid::UsageGrid(std::vector<StepPtr> steps, bool generate_names)
    : m_orig_steps{std::move(steps)}
    , m_steps{}
    , m_data_names{}
    , m_usages{}
    , m_namer{"#"}
    , m_generate_names{generate_names}
{
    this->validate_steps();
    this->insert_steps();
    this->fill_or_validate_data_names();
    this->insert_data_names();
    this->init_grid();
    this->fill_usage();
}

UsageGrid::~UsageGrid()
{
}

void UsageGrid::validate_steps() const
{
    for (const StepPtr& step : m_orig_steps)
    {
        if (!step)
        {
            throw std::invalid_argument("UsageGrid::validate_steps: Step pointer cannot be null.");
        }
    }
}

void UsageGrid::insert_steps()
{
    for (const auto& step : m_orig_steps)
    {
        m_steps.insert(step);
    }
}

void UsageGrid::fill_or_validate_data_names()
{
    if (m_generate_names)
    {
        this->fill_data_names();
    }
    else
    {
        this->validate_data_names();
    }
}

void UsageGrid::fill_data_names()
{
    const size_t step_count = m_steps.size();
    for (size_t step_index = 0u; step_index < step_count; ++step_index)
    {
        const StepPtr& step = *m_steps.at(step_index);
        DataCallback callback{[this](StepData& data) {
            auto data_name = data.name();
            if (data_name.empty())
            {
                data_name = m_namer.generate();
                data.set_name(data_name);
            }
        }};
        step->visit_data(callback);
    }
}

void UsageGrid::validate_data_names() const
{
    const size_t step_count = m_steps.size();
    for (size_t step_index = 0u; step_index < step_count; ++step_index)
    {
        const StepPtr& step = *m_steps.at(step_index);
        DataCallback callback{[this](StepData& data) {
            if (data.name().empty())
            {
                throw std::invalid_argument("UsageGrid::validate_data_names: StepData name cannot be empty.");
            }
        }};
        step->visit_data(callback);
    }
}

void UsageGrid::insert_data_names()
{
    m_data_names.clear();
    const size_t step_count = m_steps.size();
    for (size_t step_index = 0u; step_index < step_count; ++step_index)
    {
        const StepPtr& step = *m_steps.at(step_index);
        DataCallback callback{[this](StepData& data) {
            m_data_names.insert(data.name());
        }};
        step->visit_data(callback);
    }
}

void UsageGrid::init_grid()
{
    const size_t step_count = m_steps.size();
    const size_t data_count = m_data_names.size();
    m_usages.resize(step_count * data_count, StepDataUsage::None);
}

void UsageGrid::fill_usage()
{
    const size_t step_count = m_steps.size();
    const size_t data_count = m_data_names.size();
    std::vector<std::pair<std::string, StepDataUsage>> temp_pairs;
    for (size_t step_index = 0u; step_index < step_count; ++step_index)
    {
        const auto step = m_steps.at(step_index);
        temp_pairs.clear();
        DataCallback callback{[this, &temp_pairs](StepData& data) {
            temp_pairs.emplace_back(data.name(), data.usage());
        }};
        (**step).visit_data(callback);
        for (const auto& [data_name, usage] : temp_pairs)
        {
            auto data_index_opt = m_data_names.find(data_name);
            if (!data_index_opt.has_value())
            {
                throw std::runtime_error("UsageGrid::fill_usage: Internal logic error, data name not found.");
            }
            this->at(step_index, data_index_opt.value()) = usage;
        }
    }
}

StepDataUsage& UsageGrid::at(size_t step_index, size_t data_index)
{
    const size_t step_count = m_steps.size();
    const size_t data_count = m_data_names.size();
    if (step_index >= step_count || data_index >= data_count)
    {
        throw std::out_of_range("UsageGrid::at: Index out of range.");
    }
    return m_usages.at(step_index * data_count + data_index);
}

const StepDataUsage& UsageGrid::at(size_t step_index, size_t data_index) const
{
    const size_t step_count = m_steps.size();
    const size_t data_count = m_data_names.size();
    if (step_index >= step_count || data_index >= data_count)
    {
        throw std::out_of_range("UsageGrid::at: Index out of range.");
    }
    return m_usages.at(step_index * data_count + data_index);
}

std::string UsageGrid::get_diagnostics() const
{
    std::unordered_map<StepDataUsage, std::string> m;
    m.emplace(StepDataUsage::None, "-");
    m.emplace(StepDataUsage::Input, "I");
    m.emplace(StepDataUsage::Output, "O");
    std::string s;
    const size_t step_count = m_steps.size();
    const size_t data_count = m_data_names.size();
    for (size_t step_index = 0u; step_index < step_count; ++step_index)
    {
        auto p_step = m_steps.at(step_index);
        auto step_name = (*p_step)->name();
        s += "Step [" + std::to_string(step_index) + "] : " + step_name + "\n";
    }
    for (size_t data_index = 0u; data_index < data_count; ++data_index)
    {
        auto p_data_name = m_data_names.at(data_index);
        s += "Data [" + std::to_string(data_index) + "] : " + (*p_data_name) + "\n";
    }
    for (size_t step_index = 0u; step_index < step_count; ++step_index)
    {
        for (size_t data_index = 0u; data_index < data_count; ++data_index)
        {
            StepDataUsage usage = this->at(step_index, data_index);
            s += m.at(usage);
        }
        s += "\n";
    }
    return s;
}

} // namespace tg::core
