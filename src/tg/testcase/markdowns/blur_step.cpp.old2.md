#include <opencv2/imgproc.hpp>
#include "tg/testcase/blur_step.hpp"

namespace tg::testcase
{

Step::Step()
    : m_init_info{std::make_shared<StepInitInfo>()}
    , m_sealed_info{}
    , m_exec_context{}
{
}

Step::~Step()
{
}

BlurStep::BlurStep(double sigmax, double sigmay)
    : Step{}
    , m_sigmax(sigmax)
    , m_sigmay(sigmay)
    , m_info_input{std::make_shared<StepDataInfo>()}
    , m_info_output{std::make_shared<StepDataInfo>()}
{
    std::string desc = "BlurStep";
    m_info_input->m_desc = "Input image";
    m_info_input->m_usage = DataUsage::Read;
    m_info_input->m_type = std::type_index{typeid(cv::Mat)};
    m_info_output->m_desc = "Output image";
    m_info_output->m_usage = DataUsage::Write;
    m_info_output->m_type = std::type_index{typeid(cv::Mat)};
    auto init_info = this->get_init_info();
    init_info->set_desc(desc);
    init_info->add(m_info_input);
    init_info->add(m_info_output);
}

void BlurStep::execute()
{
    auto context = this->get_exec_context();
    VarData var_input = context->get(m_info_input);
    const cv::Mat& input = var_input.as<cv::Mat>();
    cv::Mat output;
    this->execute_core(input, output);
    context->set(m_info_output, VarData{std::make_shared<cv::Mat>(output)});
}

void BlurStep::execute_core(const cv::Mat& input, cv::Mat& output)
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

ConnCompStep::ConnCompStep()
    : Step{}
    , m_info{ConnCompStep::make_info()}
    , m_info_fore{std::make_shared<StepDataInfo>()}
    , m_info_labels{std::make_shared<StepDataInfo>()}
    , m_info_stats{std::make_shared<StepDataInfo>()}
    , m_info_centroids{std::make_shared<StepDataInfo>()}
{
    std::string desc = "ConnCompStep";
    m_info_fore->m_desc = "Foreground mask";
    m_info_fore->m_usage = DataUsage::Read;
    m_info_fore->m_type = std::type_index{typeid(cv::Mat)};
    m_info_labels->m_desc = "Labels of connected components";
    m_info_labels->m_usage = DataUsage::Write;
    m_info_labels->m_type = std::type_index{typeid(cv::Mat)};
    m_info_stats->m_desc = "Statistics of connected components";
    m_info_stats->m_usage = DataUsage::Write;
    m_info_stats->m_type = std::type_index{typeid(cv::Mat)};
    m_info_centroids->m_desc = "Centroids of connected components";
    m_info_centroids->m_usage = DataUsage::Write;
    m_info_centroids->m_type = std::type_index{typeid(cv::Mat)};
    
    auto init_info = this->get_init_info();
    init_info->set_desc(desc);
    init_info->add(m_info_fore);
    init_info->add(m_info_labels);
    init_info->add(m_info_stats);
    init_info->add(m_info_centroids);
}

ConnCompStep::~ConnCompStep()
{
}

void ConnCompStep::execute()
{
    auto context = this->get_exec_context();
    cv::Mat input = context->get(m_info_fore).as<cv::Mat>();
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    int connectivity = 8;
    int label_type = CV_32S;
    cv::connectedComponentsWithStats(
        input, labels, stats, centroids, connectivity, label_type
    );
    context->set(m_info_labels, VarData{std::make_shared<cv::Mat>(labels)});
    context->set(m_info_stats, VarData{std::make_shared<cv::Mat>(stats)});
    context->set(m_info_centroids, VarData{std::make_shared<cv::Mat>(centroids)});
}

const std::vector<StaticStepDataInfo> ConnCompStep::make_info()
{
    return std::vector<StaticStepDataInfo>{{
        StaticStepDataInfo::make_read<cv::Mat>("input"),
        StaticStepDataInfo::make_write<cv::Mat>("labels"),
        StaticStepDataInfo::make_write<cv::Mat>("stats"),
        StaticStepDataInfo::make_write<cv::Mat>("centroids"),
    }};
}


} // namespace tg::testcase
