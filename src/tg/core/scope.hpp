#pragma once
#include "tg/core/fwd.hpp"

namespace tg::core
{

/**
 * @brief Represents a scope, or namespace, for a collection of steps and
 * data.
 * 
 * @details Scope allows its member steps and data to be referenced
 * by a shorter name, rather than a fully qualified name.
 * 
 * When it becomes necessary to connect steps and data across different
 * scopes, their fully qualified names can be created by applying
 * the scope name as a prefix.
 * 
 * @todo Work in progress: not implemented yet.
 * @todo Proc, Scope and Graph are work in progress, class design and 
 * code logic may be incorrect.
 */
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
