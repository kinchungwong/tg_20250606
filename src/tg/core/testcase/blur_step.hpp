#pragma once
#include "tg/core/step.hpp"
#include "tg/core/step_info.hpp"

namespace tg::core::testcase
{

class BlurStep final
    : public Step
{
public:
    BlurStep();
    ~BlurStep() final;

    using Step::info;
    void execute(std::vector<VarData>& data) final;

private:
    static StepInfoPtr stc_make_info();

private:
    double m_sigmax;
    double m_sigmay;
};


} // namespace tg::core::testcase
