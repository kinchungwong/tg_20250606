#pragma once
#include <cstdint>
#include <mutex>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <opencv2/core.hpp>
#include "tg/data/vardata.hpp"
// #include "tg/data/specialized/opaque_subindex.hpp"
#include "tg/data/specialized/opaque_ptr_key.hpp"

namespace tg::testcase
{

using VarData = tg::data::VarData;
// using OpaqueSubindex = tg::data::specialized::OpaqueSubindex;
using OpaquePtrKey = tg::data::specialized::OpaquePtrKey;

class Step;
class StepInfo;

enum class DataUsage
{};

struct StepDataInfo
{
    OpaquePtrKey m_opaque_step;
    OpaquePtrKey m_opaque_stepinfo;
    std::type_index m_data_type;
    DataUsage m_usage;
    size_t m_step_local_index;
    std::string m_shortname;
    StepDataInfo(
        const Step& step, const StepInfo& step_info, std::type_index data_type,
        DataUsage usage, size_t step_local_index, std::string shortname)
        : m_opaque_step{OpaquePtrKey::create(step)}
        , m_opaque_stepinfo{OpaquePtrKey::create(step_info)}
        , m_data_type{data_type}
        , m_usage{usage}
        , m_step_local_index{step_local_index}
        , m_shortname{std::move(shortname)}
    {}
    ~StepDataInfo() = default;
    StepDataInfo(const StepDataInfo&) = default;
    StepDataInfo(StepDataInfo&&) = default;
    StepDataInfo& operator=(const StepDataInfo&) = default;
    StepDataInfo& operator=(StepDataInfo&&) = default;
};

class StepInfo final
{
public:
    using StepDataInfoPtr = std::shared_ptr<StepDataInfo>;

public:
    explicit StepInfo(Step& step, std::string_view shortname, std::string_view desc = "");
    ~StepInfo();

public:
    void add_datainfo(std::string_view shortname, DataUsage usage, std::type_index data_type)
    {
        if (m_datainfos_sealed)
        {
            throw std::runtime_error("");
        }
        size_t step_local_index = m_datainfos.size();
        auto info = std::make_shared<StepDataInfo>(
            m_step,
            *this,
            data_type,
            usage,
            step_local_index,
            std::string{shortname}
        );
        m_datainfos.emplace_back(std::move(info));
    }
    void seal_datainfos()
    {
        m_datainfos_sealed = true;
    }

private:
    StepInfo(const StepInfo&) = delete;
    StepInfo(StepInfo&&) = delete;
    StepInfo& operator=(const StepInfo&) = delete;
    StepInfo& operator=(StepInfo&&) = delete;

private:
    Step& m_step;
    const std::string m_shortname;  // Short name of the step
    std::string m_desc;       // Description of the step
    std::vector<StepDataInfoPtr> m_datainfos;
    bool m_datainfos_sealed;
};

class Step
{
public:
    virtual ~Step();
    virtual void execute() = 0;

public:
    const StepInfo& info() const;
    StepInfo& info();

protected:
    Step();

private:
    Step(const Step&) = delete;
    Step(Step&&) = delete;
    Step& operator=(const Step&) = delete;
    Step& operator=(Step&&) = delete;

protected:
    std::shared_ptr<StepInfo> m_info;
};

class BlurStep
    : public Step
{
public:
    explicit BlurStep(double sigmax = 1.0, double sigmay = 1.0);
    ~BlurStep();

public:
    // using Step::get_init_info;
    // using Step::get_sealed_info;
    // using Step::set_sealed_info;
    // using Step::get_exec_context;
    // using Step::set_exec_context;
    // using Step::remove_exec_context;
    
public:
    void execute();

private:
    void execute_core(const cv::Mat& input, cv::Mat& output);

private:
    double m_sigmax;
    double m_sigmay;
    // StepDataInfoPtr m_info_input;
    // StepDataInfoPtr m_info_output;
};

class ConnCompStep
    : public Step
{
public:
    explicit ConnCompStep();
    ~ConnCompStep();

public:
    void execute();

// private:
//     static const std::vector<StaticStepDataInfo> make_info();

// private:
//     const std::vector<StaticStepDataInfo> m_info;

//     StepDataInfoPtr m_info_fore;
//     StepDataInfoPtr m_info_labels;
//     StepDataInfoPtr m_info_stats;
//     StepDataInfoPtr m_info_centroids;
};

} // namespace tg::testcase
