#include <cmath>
#include <opencv2/imgproc.hpp>
#include "tg/testcase/blur_step.hpp"

namespace tg::testcase
{

StepData::StepData(Step& task, StepDataUsage usage, std::type_index type)
    : m_key()
    , m_usage(usage)
    , m_type(type)
    , m_void()
{
}

BlurStep::BlurStep()
    : m_input()
    , m_output()
    , m_sigmax(0.0)
    , m_sigmay(0.0)
{
}

void BlurStep::visit_data(DataVisitor& visitor)
{
    visitor.visit(m_input);
    visitor.visit(m_output);
}

void BlurStep::execute()
{
    const cv::Mat& exec_input = *m_input;
    auto sz = exec_input.size();
    int cvt = exec_input.type();
    cv::Mat& exec_output = m_output.emplace(sz, cvt);
    /**
     * @note Workaround for OpenCV bug require sigmax and sigmay to be strictly positive.
     */
    const double sigmin = 1.0e-3;
    double sigmax = std::max(sigmin, m_sigmax);
    double sigmay = std::max(sigmin, m_sigmay);
    /**
     * @note Kernel length required to be an odd integer for symmetry.
     * @note For sigma < 0.5, kernel length of 1 is okay since there is practically no blurring.
     */
    auto ksize = cv::Size
    {
        cv::saturate_cast<int>(sigmax * 2.0) * 2 + 1,
        cv::saturate_cast<int>(sigmay * 2.0) * 2 + 1
    };
    const int border = cv::BORDER_DEFAULT;
    const auto algo = cv::ALGO_HINT_DEFAULT;
    cv::GaussianBlur(exec_input, exec_output, ksize, sigmax, sigmay, border, algo);
}

void StepExecutor::execute(GlobalData& global_data, DataMap& data_map, Step& step)
{
}

StepExecutor::StepExecutorDataVisitor::StepExecutorDataVisitor(GlobalData& global_data, DataMap& data_map, Step& step)
    : DataVisitor()
    , m_global_data(global_data)
    , m_data_map(data_map)
    , m_step(step)
    , m_post_execute(false)
{
}

void StepExecutor::StepExecutorDataVisitor::visit(StepData& data)
{
    const auto usage = data.usage();
    const auto& key = data.key();
    auto& gdata = m_global_data.get(m_data_map.get(key));
    if (!m_post_execute)
    {
        data.clear();
        if (usage == StepDataUsage::Read)
        {
            gdata.copy_to(data);
            data.sync();
        }
        else if (usage == StepDataUsage::Consume)
        {
            gdata.move_to(data);
            data.sync();
        }
    }
    else 
    {
        //! assume(m_post_execute == true)
        if (usage == StepDataUsage::Create)
        {
            data.sync();
            gdata.move_from(data);
        }
        data.clear();
    }
}

} // namespace tg::testcase
