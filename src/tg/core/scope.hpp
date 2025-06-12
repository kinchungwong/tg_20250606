#pragma once
#include "tg/core/fwd.hpp"

namespace tg::core
{

struct ScopeDataInfo
{
    /**
     * @brief Opaque pointer to the data.
     * 
     * @details This pointer is used to reference the data in a way that
     * does not expose its type or implementation details.
     */
    const char* m_opaque_ptr;

    /**
     * @brief Simple name given to the data.
     * @note Simple name (unqualified names) are not unique across scopes.
     * Some data may not have been given a name.
     */
    std::string m_data_name;
};

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
    void visit(Step& step, StepData& data);

private:
    std::string m_name;
};

} // namespace tg::core
