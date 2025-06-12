#pragma once
#include <cstdint>
#include <mutex>
#include <memory>
#include <string>
#include <vector>
#include <typeindex>
#include <opencv2/core/mat.hpp>
// #include "tg/core/fwd.hpp"
// #include "tg/core/step.hpp"
#include "tg/data/vardata.hpp"

namespace tg::testcase::ver2
{

namespace
{
    using VarData = tg::data::VarData;
    using MutexType = std::mutex;
    using LockType = std::unique_lock<MutexType>;
}

enum class DataUsage : int
{
    None = 0,
    Read = 1,
    Write = 2,
    Consume = 3
};

struct DataKey
{
    // Step local data index
    int m_step_did;

    // Scope local data index
    int m_scope_did;

    // Global data index
    int m_global_did;

    // Randomly assigned access key to verify access
    int m_access_key;
};

struct StepDataInfo
{
    // Description (for display only)
    std::string m_desc;

    // Locally unique name (for connecting data between steps within same scope)
    std::string m_luname;

    // Globally unique name (for connecting data between steps in different scopes)
    std::string m_guname;

    // How this data is used in the step
    DataUsage m_usage;

    // For C++ runtime type checking
    std::type_index m_type;

    // Data access key, for verifying access in global context
    DataKey m_dak;
};

class StepInfo
{
public:
    using StepInfoPtr = std::shared_ptr<StepInfo>;
    StepInfo()
        : m_desc{}
        , m_dinfo{}
        , m_frozen{false}
    {}
    ~StepInfo(){}
    const std::string& desc() const { return m_desc; }
    const std::vector<StepDataInfo>& data_info() const { return m_dinfo; }
    const StepDataInfo& data_info_at(size_t index) const
    {
        return m_dinfo.at(index);
    }
    StepDataInfo& data_info_at(size_t index)
    {
        return m_dinfo.at(index);
    }
    bool frozen() const
    {
        return m_frozen;
    }
    void freeze()
    {
        m_frozen = true;
    }
    void set_desc(const std::string& desc)
    {
        if (m_frozen)
        {
            throw std::runtime_error("Cannot modify step description after freezing.");
        }
        m_desc = desc;
    }
    void add_data_info(const StepDataInfo& dinfo)
    {
        if (m_frozen)
        {
            throw std::runtime_error("Cannot modify step data info after freezing.");
        }
        m_dinfo.push_back(dinfo);
    }
private:
    std::string m_desc;
    std::vector<StepDataInfo> m_dinfo;
    bool m_frozen;
};

class StepExecContext
{
public:
    VarData get(DataKey key) const;
    void set(DataKey key, VarData value);
};

class Ver2Step
{

};

class Ver2BlurStep
    : public Ver2Step
{
public:
    using StepInfoPtr = StepInfo::StepInfoPtr;

public:
    Ver2BlurStep(double sigmax = 1.0, double sigmay = 1.0);
    StepInfoPtr info() const;
    void execute(StepExecContext& context);

private:
    void execute_core(const cv::Mat& input, cv::Mat& output);

private:
    StepInfoPtr m_info;
    double m_sigmax;
    double m_sigmay;
    VarData m_var_input;
    VarData m_var_output;
};

} // namespace tg::testcase::ver2
