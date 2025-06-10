#pragma once
#include "tg/core/fwd.hpp"

namespace tg::core
{

/**
 * @brief Represents a directed graph of steps.
 * 
 * @details The Graph class represents a self-contained system of steps,
 * data, and scopes. All data dependency and barrier relationships are
 * known. Any inherent parallelism is readily exploited when executed
 * in a thread pool with multiple workers.
 *
 * @todo Work in progress: not implemented yet.
 * @todo Proc, Scope and Graph are work in progress, class design and 
 * code logic may be incorrect.
 * @todo Thread pool, task queue are not implemented yet.
 * @todo The parsing of graph information, and the initialization of
 * ExecData, has not been implemented yet.
 */
class Graph
{
public:
    void add_step(ScopePtr scope, StepPtr step);
};

} // namespace tg::core
