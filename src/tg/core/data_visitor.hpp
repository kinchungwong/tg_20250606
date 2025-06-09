#pragma once
#include "tg/core/fwd.hpp"

namespace tg::core
{

/**
 * @brief Visitor pattern for Data objects.
 * 
 * @details
 * Allows visiting Data objects embedded into other classes (such as Step
 * and Proc) to perform operations on them, without having to build 
 * parallel hierarchies that replicate the structures of those classes.
 */
class DataVisitor
{
public:
    virtual ~DataVisitor() = default;
    virtual void visit(StepData& data) = 0;

protected:
    DataVisitor() = default;

private:
    DataVisitor(const DataVisitor&) = delete;
    DataVisitor& operator=(const DataVisitor&) = delete;
    DataVisitor(DataVisitor&&) = delete;
    DataVisitor& operator=(DataVisitor&&) = delete;
};

} // namespace tg::core
