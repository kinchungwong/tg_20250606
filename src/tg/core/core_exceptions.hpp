#pragma once
#include <stdexcept>

namespace tg::core
{

class StepInfoBadAssignment : std::runtime_error { using std::runtime_error::runtime_error; };
class StepInfoNameConflict : std::runtime_error { using std::runtime_error::runtime_error; };
class StepInfoFrozen : std::runtime_error { using std::runtime_error::runtime_error; };
class StepInfoNotEmpty : std::runtime_error { using std::runtime_error::runtime_error; };
class StepInfoNameNotFound : std::runtime_error { using std::runtime_error::runtime_error; };
class StepInfoBadIndex : std::runtime_error { using std::runtime_error::runtime_error; };

} // namespace tg::core
