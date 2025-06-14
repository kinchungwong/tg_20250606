#include "tg/testcase/blur_step.hpp"

#include <opencv2/imgproc.hpp>
#include "tg/testcase/blur_step.hpp"

namespace tg::testcase
{

Step::Step()
    : m_info{}
{
}

Step::~Step()
{
}

const StepInfo& Step::info() const
{
    return *m_info;
}

StepInfo& Step::info()
{
    return *m_info;
}


} // namespace tg::testcase
