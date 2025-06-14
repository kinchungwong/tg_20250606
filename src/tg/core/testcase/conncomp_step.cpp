#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "tg/core/testcase/conncomp_step.hpp"

namespace tg::core::testcase
{

ConnCompStep::ConnCompStep()
    : Step{stc_make_info()}
    , m_connectivity{8}
    , m_label_type{CV_32S}
{}

ConnCompStep::~ConnCompStep()
{
}

StepInfoPtr ConnCompStep::stc_make_info()
{
    StepInfoPtr info = std::make_shared<StepInfo>("ConnCompStep");
    info->set_step_type<ConnCompStep>();
    info->add_data<cv::Mat>("input", DataUsage::Read);
    info->add_data<cv::Mat>("labels", DataUsage::Write);
    info->add_data<cv::Mat>("stats", DataUsage::Write);
    info->add_data<cv::Mat>("centroids", DataUsage::Write);
    info->datainfos_freeze();
    return info;
}

void ConnCompStep::execute(std::vector<VarData>& data)
{
    this->pre_execute_validation(data);
    cv::Mat input = data.at(0).as<cv::Mat>();
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    cv::connectedComponentsWithStats(
        input, labels, stats, centroids, m_connectivity, m_label_type
    );
    data.at(1).emplace<cv::Mat>(labels);
    data.at(2).emplace<cv::Mat>(stats);
    data.at(3).emplace<cv::Mat>(centroids);
    this->post_execute_validation(data);
}

} // namespace tg::core::testcase
