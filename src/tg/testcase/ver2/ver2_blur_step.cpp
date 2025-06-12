#include <cmath>
#include <opencv2/imgproc.hpp>
#include "tg/testcase/ver2/ver2_blur_step.hpp"
#include "tg/core/data_visitor.hpp"
#include "tg/core/input.hpp"
#include "tg/core/output.hpp"

using namespace tg::core;

namespace tg::testcase::ver2
{

using StepInfoPtr = StepInfo::StepInfoPtr;

Ver2BlurStep::Ver2BlurStep(double sigmax, double sigmay)
    : m_info(std::make_shared<StepInfo>())
    , m_sigmax(sigmax)
    , m_sigmay(sigmay)
{
    m_info->set_desc("Blur step using Gaussian blur.");
    m_info->add_data_info(StepDataInfo{
        "Input image", 
        DataUsage::Read, 
        std::type_index{typeid(cv::Mat)}, 
        DataKey{}
    });
    m_info->add_data_info(StepDataInfo{
        "Output image", 
        DataUsage::Write, 
        std::type_index{typeid(cv::Mat)}, 
        DataKey{}
    });
    m_info->freeze();
}

StepInfoPtr Ver2BlurStep::info() const
{
    return m_info;
}

void Ver2BlurStep::execute(StepExecContext& context)
{
    m_var_input = context.get(m_info->data_info_at(0).m_dak);
    const cv::Mat& input = m_var_input.as<cv::Mat>();
    cv::Mat output;
    this->execute_core(input, output);
    m_var_output.emplace<cv::Mat>(output);
    context.set(m_info->data_info_at(1).m_dak, m_var_output);
    m_var_input.clear();
    m_var_output.clear();
}

void Ver2BlurStep::execute_core(const cv::Mat& input, cv::Mat& output)
{
    auto sz = input.size();
    int cvt = input.type();
    output.create(sz, cvt);
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
    cv::GaussianBlur(input, output, ksize, sigmax, sigmay, border, algo);    
}

} // namespace tg::testcase::ver2
