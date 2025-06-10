#pragma once
#include "tg/core/fwd.hpp"

namespace tg::core
{

class Graph
{
public:
    void add_step(ScopePtr scope, StepPtr step);
};

} // namespace tg::core
