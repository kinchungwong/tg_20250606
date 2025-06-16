#include <cassert>
#include "tg/core/scope.hpp"
#include "tg/core/step.hpp"
#include "tg/core/step_info.hpp"

using namespace tg::core;

struct DummyStep : Step
{
    DummyStep() : Step(std::make_shared<StepInfo>("Dummy")) {}
    void execute(std::vector<VarData>&) override {}
};

int main()
{
    // Create scope and add one step
    Scope scope("S");
    auto s1 = std::make_shared<DummyStep>();
    scope.add(s1);

    // Capture iterators before modifying the scope
    auto it_begin_before = scope.begin();
    auto it_end_before = scope.end();

    // Grow the scope
    auto s2 = std::make_shared<DummyStep>();
    scope.add(s2);

    auto it_end_after = scope.end();

    // Old end iterator should compare equal to new end iterator
    assert(it_end_before == it_end_after);

    // Increment iterator captured before growth
    auto iter = it_begin_before;
    ++iter;                   // now at index 1
    assert(iter != it_end_after);
    ++iter;                   // now past end
    assert(iter == it_end_after);

    // Expired iterators compare equal
    ScopeStepIter expired_begin;
    ScopeStepIter expired_end;
    {
        Scope tmp("Tmp");
        tmp.add(std::make_shared<DummyStep>());
        expired_begin = tmp.begin();
        expired_end = tmp.end();
    } // tmp destroyed
    assert(expired_begin == expired_end);

    return 0;
}
