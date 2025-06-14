#pragma once
#include "tg/core/step.hpp"
#include "tg/core/step_info.hpp"

namespace tg::core::testcase
{

class ConnCompStep final
    : public Step
{
public:
    ConnCompStep();
    ~ConnCompStep() final;

    using Step::info;
    void execute(std::vector<VarData>& data) final;

private:
    static StepInfoPtr stc_make_info();

private:
    int m_connectivity;
    int m_label_type;
};

} // namespace tg::core::testcase
