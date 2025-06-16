#pragma once
#include <atomic>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <typeindex>
#include <typeinfo>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>
#include "tg/data/vardata.hpp"
#include "tg/data/specialized/opaque_ptr_key.hpp"
#include "tg/core/core_enums.hpp"
#include "tg/core/core_exceptions.hpp"

namespace tg::core
{

using VarData = tg::data::VarData;
using OpaquePtrKey = tg::data::specialized::OpaquePtrKey;

class Step;
using StepPtr = std::shared_ptr<Step>;
using StepWPtr = std::weak_ptr<Step>;

class StepInfo;
using StepInfoPtr = std::shared_ptr<StepInfo>;

struct DataInfoTuple;

class Scope;
class ScopeIter;
using ScopePtr = std::shared_ptr<Scope>;

class ScopeInfo;
using ScopeInfoPtr = std::shared_ptr<ScopeInfo>;
using ScopeInfoWPtr = std::weak_ptr<ScopeInfo>;

} // namespace tg::core
