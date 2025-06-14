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
#include "tg/data/specialized/opaque_subindex.hpp"

namespace tg::testcase
{

using VarData = tg::data::VarData;
using OpaqueSubindex = tg::data::specialized::OpaqueSubindex;

enum class DataUsage : int
{
    None = 0,
    Read = 1,
    Write = 2,
    Expire = 3
};


struct StaticStepDataInfo
{
    const OpaqueSubindex m_index;
    const DataUsage m_usage;
    const std::type_index m_type;
    const std::string m_shortname;

    template <typename T>
    static StaticStepDataInfo make_read(OpaqueSubindex index, std::string_view shortname) {
        return StaticStepDataInfo{
            index,
            DataUsage::Read,
            std::type_index{typeid(T)},
            std::string(shortname)
        };
    }

    template <typename T>
    static StaticStepDataInfo make_write(OpaqueSubindex index, std::string_view shortname) {
        return StaticStepDataInfo{
            index,
            DataUsage::Write,
            std::type_index{typeid(T)},
            std::string(shortname)
        };
    }

    template <typename T>
    static StaticStepDataInfo make_expire(OpaqueSubindex index, std::string_view shortname) {
        return StaticStepDataInfo{
            index,
            DataUsage::Expire,
            std::type_index{typeid(T)},
            std::string(shortname)
        };
    }

private:
    StaticStepDataInfo(OpaqueSubindex index, DataUsage usage, std::type_index type, std::string shortname)
        : m_index(index)
        , m_usage(usage)
        , m_type(type)
        , m_shortname(std::move(shortname))
    {}
};

class Step;

struct StepDataInfo
{
    // Description (for display only)
    std::string m_desc;

    // How this data is used in the step
    DataUsage m_usage;

    // For C++ runtime type checking
    std::type_index m_type;

    StepDataInfo()
        : m_desc{}
        , m_usage{DataUsage::None}
        , m_type{std::type_index{typeid(void)}} {}
};

using StepDataInfoPtr = std::shared_ptr<StepDataInfo>;

class StepInitInfo
{
public:
    StepInitInfo();
    ~StepInitInfo();

public:
    void set_desc(const std::string& desc)
    {
        m_desc = desc;
    }

    void add(StepDataInfoPtr data_info)
    {
        m_data_info.push_back(data_info);
    }

    StepDataInfoPtr data_info_at(size_t index) const
    {
        if (index >= m_data_info.size())
        {
            throw std::out_of_range("Index out of range in StepInitInfo::data_info_at");
        }
        return m_data_info.at(index);
    }

private:
    std::string m_desc;
    std::vector<StepDataInfoPtr> m_data_info;
};

class StepSealedInfo{};
class StepExecContext
{
public:
    VarData get(StepDataInfoPtr info);
    void set(StepDataInfoPtr info, const VarData& value);
};

using StepInitInfoPtr = std::shared_ptr<StepInitInfo>;
using StepSealedInfoPtr = std::shared_ptr<StepSealedInfo>;
using StepExecContextPtr = std::shared_ptr<StepExecContext>;
using StepExecContextWeak = std::weak_ptr<StepExecContext>;

class Step
{
public:
    virtual ~Step();

protected:
    Step();

public:
    StepInitInfoPtr get_init_info() const
    {
        return m_init_info;
    }

    StepSealedInfoPtr get_sealed_info() const
    {
        if (!m_sealed_info)
        {
            throw std::runtime_error("Step::get_sealed_info: not assigned yet.");
        }
        return m_sealed_info;
    }

    void set_sealed_info(StepSealedInfoPtr sealed_info)
    {
        if (!sealed_info)
        {
            throw std::invalid_argument("Step::set_sealed_info: argument cannot be null.");
        }
        if (m_sealed_info)
        {
            throw std::runtime_error("Step::set_sealed_info: already assigned and sealed.");
        }
        m_sealed_info = std::move(sealed_info);
    }
    
    StepExecContextPtr get_exec_context() const
    {
        auto context = m_exec_context.lock();
        if (!context)
        {
            throw std::runtime_error("Step::get_exec_context: either not set or expired.");
        }
        return context;
    }

    void set_exec_context(StepExecContextPtr context)
    {
        m_exec_context = std::move(context);
    }

    void remove_exec_context()
    {
        m_exec_context.reset();
    }

protected:
    StepInitInfoPtr m_init_info;
    StepSealedInfoPtr m_sealed_info;
    StepExecContextWeak m_exec_context;
};

class BlurStep
    : public Step
{
public:
    explicit BlurStep(double sigmax = 1.0, double sigmay = 1.0);
    ~BlurStep();

public:
    using Step::get_init_info;
    using Step::get_sealed_info;
    using Step::set_sealed_info;
    using Step::get_exec_context;
    using Step::set_exec_context;
    using Step::remove_exec_context;
    
public:
    void execute();

private:
    void execute_core(const cv::Mat& input, cv::Mat& output);

private:
    double m_sigmax;
    double m_sigmay;
    StepDataInfoPtr m_info_input;
    StepDataInfoPtr m_info_output;
};

class ConnCompStep
    : public Step
{
public:
    explicit ConnCompStep();
    ~ConnCompStep();

public:
    void execute();

private:
    static const std::vector<StaticStepDataInfo> make_info();

private:
    const std::vector<StaticStepDataInfo> m_info;

    StepDataInfoPtr m_info_fore;
    StepDataInfoPtr m_info_labels;
    StepDataInfoPtr m_info_stats;
    StepDataInfoPtr m_info_centroids;
};

} // namespace tg::testcase
