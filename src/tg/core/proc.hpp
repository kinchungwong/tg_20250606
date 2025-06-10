#pragma once
#include "tg/core/fwd.hpp"

namespace tg::core
{

/**
 * @brief Represents a collection of related steps.
 * 
 * @details
 * A Proc automatically forms a Scope for its steps and their StepData,
 * so that within this scope, these steps can produce and use data
 * using a simplified data naming scheme.
 * 
 * When a step is added to a Proc, any StepData whose name matches
 * the name of another StepData in another Step within the same Proc
 * will be considered to be the same data item.
 * 
 * All StepData within a Proc will have an associated qualified name,
 * formed from the concatenation of the Scope name and its own name.
 */
class Proc
{
public:
    Proc();
    ~Proc();
    void add_step(StepPtr step);
    void get_steps(std::vector<StepPtr>& steps) const;

private:
    ScopePtr m_scope;
    std::vector<StepPtr> m_steps;
};

} // namespace tg::core
