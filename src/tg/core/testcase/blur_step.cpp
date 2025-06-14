#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "tg/core/testcase/blur_step.hpp"

namespace tg::core::testcase
{

BlurStep::BlurStep()
    : Step{stc_make_info()}
{}

BlurStep::~BlurStep()
{
}

StepInfoPtr BlurStep::stc_make_info()
{
    StepInfoPtr info = std::make_shared<StepInfo>("BlurStep");
    info->set_step_type<BlurStep>();
    info->add_data<cv::Mat>("input", DataUsage::Read);
    info->add_data<cv::Mat>("output", DataUsage::Write);
    info->datainfos_freeze();
    return info;
}

void BlurStep::execute(std::vector<VarData>& data)
{
    this->pre_execute_validation(data);
    const cv::Mat& input = data.at(0).as<cv::Mat>();

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
    cv::Mat output;
    cv::GaussianBlur(input, output, ksize, sigmax, sigmay, border, algo);    
    data.at(1).emplace<cv::Mat>(output);
    this->post_execute_validation(data);
}

} // namespace tg::core::testcase
