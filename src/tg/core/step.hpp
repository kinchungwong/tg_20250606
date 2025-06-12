#pragma once
#include "tg/core/fwd.hpp"

namespace tg::core
{

class Step
{
public:
    virtual ~Step() = default;
    virtual std::string name() const = 0;
    virtual void visit_data(DataVisitor& visitor) = 0;
    virtual void execute() = 0;

protected:
    Step() = default;

private:
    Step(const Step&) = delete;
    Step(Step&&) = delete;
    Step& operator=(const Step&) = delete;
    Step& operator=(Step&&) = delete;
};

} // namespace tg::core
