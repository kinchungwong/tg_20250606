#include <cmath>
#include <opencv2/imgproc.hpp>
#include "tg/testcase/blur_step.hpp"
#include "tg/core/data_visitor.hpp"
#include "tg/core/input.hpp"
#include "tg/core/output.hpp"

using namespace tg::core;

namespace tg::testcase
{

BlurStep::BlurStep()
    : m_input(std::make_unique<Input<cv::Mat>>())
    , m_output(std::make_unique<Output<cv::Mat>>())
    , m_sigmax(0.0)
    , m_sigmay(0.0)
{
}

BlurStep::~BlurStep()
{
}

void BlurStep::visit_data(DataVisitor& visitor)
{
    visitor.visit(*m_input);
    visitor.visit(*m_output);
}

void BlurStep::execute()
{
    const cv::Mat& exec_input = **m_input;
    auto sz = exec_input.size();
    int cvt = exec_input.type();
    cv::Mat& exec_output = m_output->emplace(sz, cvt);
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

} // namespace tg::testcase
