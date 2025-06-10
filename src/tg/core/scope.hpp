#pragma once
#include "tg/core/fwd.hpp"

namespace tg::core
{

class Scope
{
public:
    std::string name() const;
    void visit(Proc& proc);
    void visit(Step& step);
    void visit(StepData& data);

private:
    std::string m_name;
};

} // namespace tg::core
