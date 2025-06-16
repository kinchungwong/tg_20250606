#include <iostream>
#include <string>
#include <functional>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "tg/core/fwd.hpp"
#include "tg/core/step.hpp"
#include "tg/core/step_info.hpp"
#include "tg/core/scope.hpp"
#include "tg/core/scope_info.hpp"
#include "tg/common/project_macros.hpp"
#include "tg/core/testcase/blur_step.hpp"
#include "tg/core/testcase/conncomp_step.hpp"
#include "tg/core/utility/step_info_summary.hpp"
#include "tg/testcase/ostrm.hpp"

using namespace tg::core;
using namespace tg::core::utility;
using namespace tg::core::testcase;

namespace //(unnamed)
{

INLINE_NEVER
void print_step_summary(Step& step, OStrm& cout)
{
    auto print_to_cout = [&cout](const std::string& line) {
        cout << line << std::endl;
    };
    StepInfoSummary summary(step.info());
    summary.generate(print_to_cout);
}

INLINE_NEVER
std::vector<VarData> create_vardata_vector_for_step(Step& step)
{
    size_t data_count = step.info().data_count();
    std::vector<VarData> data;
    for (size_t k = 0u; k < data_count; ++k)
    {
        data.emplace_back(VarData{});
    }
    return data;
};

} // namespace(unnamed)

INLINE_NEVER
void tg_testcase_1(OStrm cout)
{
    cout << "running tg_testcase_1..." << std::endl;
    BlurStep blur_step;
    ConnCompStep conncomp_step;
    print_step_summary(blur_step, cout);
    print_step_summary(conncomp_step, cout);
    cout << "tg_testcase_1 success." << std::endl;
}

INLINE_NEVER
void tg_testcase_2(OStrm cout)
{
    cout << "running tg_testcase_2..." << std::endl;
    BlurStep blur_step;
    ConnCompStep conncomp_step;
    std::vector<VarData> blur_data = create_vardata_vector_for_step(blur_step);
    blur_data.at(0).emplace<cv::Mat>(cv::Mat::ones(5, 5, CV_8UC1) * 255); // Input image
    blur_step.execute(blur_data);
    std::vector<VarData> conncomp_data = create_vardata_vector_for_step(conncomp_step);
    conncomp_data.at(0).emplace<cv::Mat>(cv::Mat::ones(5, 5, CV_8UC1) * 255); // Input image
    conncomp_step.execute(conncomp_data);
    cout << "tg_testcase_2 success." << std::endl;
}

INLINE_NEVER
void tg_testcase_3(OStrm cout)
{
    cout << "running tg_testcase_3..." << std::endl;
    Scope scope("TestScope");
    auto blur_step = std::make_shared<BlurStep>();
    auto conncomp_step = std::make_shared<ConnCompStep>();
    scope.add(blur_step);
    scope.add(conncomp_step);
    scope.freeze();
    cout << "Scope name: " << scope.scopename() << std::endl;
    size_t next_step_index = 0u;
    for (const auto& step : scope)
    {
        size_t step_index = next_step_index++;
        cout << "Step " << step_index << " in Scope '" << scope.scopename() << "':" << std::endl; 
        print_step_summary(*step, cout);
    }
    cout << "tg_testcase_3 success." << std::endl;
}

INLINE_NEVER
void tg_testcase()
{
    OStrm cout;
    tg_testcase_1(cout);
    tg_testcase_2(cout);
    tg_testcase_3(cout);
}
