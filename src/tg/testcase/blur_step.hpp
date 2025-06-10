#pragma once
#include <opencv2/core/mat.hpp>
#include "tg/core/fwd.hpp"
#include "tg/core/step.hpp"

namespace tg::testcase
{
class BlurStep
    : public tg::core::Step
{
public:
    BlurStep();
    ~BlurStep();

    tg::core::Input<cv::Mat>& input() { return *m_input; }
    tg::core::Output<cv::Mat>& output() { return *m_output; }

    void execute() final;

    void visit_data(tg::core::DataVisitor& visitor) final;

private:
    std::unique_ptr<tg::core::Input<cv::Mat>> m_input;
    std::unique_ptr<tg::core::Output<cv::Mat>> m_output;
    double m_sigmax;
    double m_sigmay;
};    

} // namespace tg::testcase
