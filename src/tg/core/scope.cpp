#include "tg/core/scope.hpp"
#include "tg/core/step.hpp"
#include "tg/core/step_data.hpp"
#include "tg/core/proc.hpp"
#include "tg/core/data_callback.hpp"

namespace tg::core
{

std::string Scope::name() const
{
    return m_name;
}

void Scope::visit(Proc& proc)
{
    std::vector<StepPtr> steps;
    proc.get_steps(steps);
    for (const auto& step : steps)
    {
        this->visit(*step);
    }
}

void Scope::visit(Step& step)
{
    //! @todo step.set_scope_name(this->m_name);
    DataCallback callback{[this, &step](StepData& data) -> void {
        this->visit(step, data);
    }};
    step.visit_data(callback);
}

void Scope::visit(Step& step, StepData& data)
{
    data.set_scope_name(this->m_name);
}

} // namespace tg::core
