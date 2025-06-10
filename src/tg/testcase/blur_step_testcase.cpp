#include <iostream>
#include "tg/testcase/blur_step_testcase.hpp"
#include "tg/testcase/blur_step.hpp"
#include "tg/core/step_executor.hpp"
#include "tg/core/exec_data.hpp"
#include "tg/core/data_impl.hpp"
#include "tg/core/input.hpp"
#include "tg/core/output.hpp"
#include "tg/testcase/ostrm.hpp"

void blur_step_testcase_1(OStrm& cout)
{
    cout << "Running BlurStep Testcase 1..." << std::endl;

    using namespace tg::testcase;
    using namespace tg::core;

    // Create a BlurStep instance
    auto blur_step = std::make_shared<BlurStep>();
    auto& blur_step_input_ref = blur_step->input();
    blur_step_input_ref.set_key(0);
    auto& blur_step_output_ref = blur_step->output();
    blur_step_output_ref.set_key(3);

    std::vector<DataImplPtr> data;
    auto data_0 = std::make_shared<DataImpl>(std::type_index(typeid(cv::Mat)), 
        std::vector<KeyUsagePair>{
            {0, StepDataUsage::Input},
            {1, StepDataUsage::Output}
        });
    data_0->set(1, 1, std::make_shared<cv::Mat>(cv::Mat::ones(100, 100, CV_8UC3) * 255)); // White image
    auto data_1 = std::make_shared<DataImpl>(std::type_index(typeid(cv::Mat)), 
        std::vector<KeyUsagePair>{
            {2, StepDataUsage::Input},
            {3, StepDataUsage::Output}
        });
    data.push_back(data_0);
    data.push_back(data_1);
    std::unordered_map<AccessKey, KeyInfo> keys;
    keys.emplace(0, KeyInfo{0, 0}); // Input key
    keys.emplace(1, KeyInfo{0, 1}); // Output key
    keys.emplace(2, KeyInfo{1, 0}); // Another Input key
    keys.emplace(3, KeyInfo{1, 1}); // Another Output key

    auto exec_data = std::make_shared<ExecData>(data, keys);

    StepExecutor step_executor(*blur_step, *exec_data);

    step_executor.execute();

    cout << "Address of data_0: " << data_0->peek() << std::endl;
    cout << "Address of data_1: " << data_1->peek() << std::endl;
    cout << "BlurStep testcase 1 executed successfully." << std::endl;
}

void blur_step_testcase()
{
    OStrm cout;
    blur_step_testcase_1(cout);
}
