#pragma once
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <string>
#include <mutex>
#include <utility>
#include <stdexcept>
#include <vector>
#include <unordered_map>
#include <atomic>

#include "tg/core/enums.hpp"
#include "tg/core/exceptions.hpp"

namespace tg::core
{

class Step;
using StepPtr = std::shared_ptr<Step>;

class Proc;
using ProcPtr = std::shared_ptr<Proc>;

class Scope;
using ScopePtr = std::shared_ptr<Scope>;

class Graph;
using GraphPtr = std::shared_ptr<Graph>;

class ExecData;
using ExecDataPtr = std::shared_ptr<ExecData>;

class StepData;
template <typename T> class Input;
template <typename T> class Output;
class DataVisitor;

class DataImpl;
using AccessKey = size_t;
using KeyUsagePair = std::pair<AccessKey, StepDataUsage>;

struct KeyInfo
{
    size_t data_index;
    size_t usage_index;
};

} // namespace tg::core
