#include "tg/core/proc.hpp"
#include "tg/core/scope.hpp"
#include "tg/core/step.hpp"
#include "tg/core/step_data.hpp"
#include "tg/core/data_callback.hpp"

namespace tg::core
{

Proc::Proc()
    : m_scope(std::make_shared<Scope>())
    , m_steps{}
{
}

Proc::~Proc()
{
}

void Proc::add_step(StepPtr step)
{
    if (!step)
    {
        throw std::runtime_error("Proc::add_step: step is null.");
    }
    std::string scope_name = m_scope->name();
    DataCallback callback{[this, scope_name](StepData& data) -> void {
        data.set_scope_name(scope_name);
        this->m_scope->visit(data);
    }};
    step->visit_data(callback);
    m_steps.push_back(std::move(step));
}

void Proc::get_steps(std::vector<StepPtr>& steps) const
{
    steps.assign(m_steps.begin(), m_steps.end());
}

} // namespace tg::core

// void Scope::visit(Step& step)
// {
// }

// void Scope::visit(StepData& data)
// {
//     data.set_scope_name(this->m_name);
// }
