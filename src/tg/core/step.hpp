#pragma once
#include "tg/core/fwd.hpp"

namespace tg::core
{

class Step
{
public:
    /**
     * @brief Retrieves the StepInfo associated with this Step.
     * 
     * @note If the StepInfo is not initialized, it will be created using
     * the provided CreateStepInfoFunc during construction.
     */
    StepInfo& info();

public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~Step();

    /**
     * @brief Function to be called by the Executor to execute the Step.
     * 
     * @param data The array of VarData, used for both inputs and outputs.
     * 
     * @note 
     * The ```data``` array size is same as ```StepInfo::data_count()```,
     * and the items are stored according to the order of their definition
     * in the StepInfo.
     */
    virtual void execute(std::vector<VarData>& data) = 0;

protected:
    /**
     * @brief Initialize Step as a base class.
     *
     * @param step_info The StepInfo object to be bound to this Step.
     * This can be null; see note.
     *
     * @note Derived classes should do exactly one of:
     * (1) passing in a valid ```StepInfoPtr``` at Step construction, _OR_
     * (2) override the ```create_step_info()``` method to return a valid
     *     ```StepInfoPtr``` from there.
     */
    explicit Step(StepInfoPtr step_info = nullptr);

protected:
    /**
     * @brief Validates the data before executing the Step.
     * @note This validation is redundant because the Executor already
     * guarantees the data meets the requirements. However, it is
     * provided here for convenience during development and debugging.
     */
    void pre_execute_validation(const std::vector<VarData>& data);

    /**
     * @brief Validates the data after executing the Step.
     * @note This validation is redundant because the Executor already
     * guarantees the data meets the requirements. However, it is
     * provided here for convenience during development and debugging.
     */
    void post_execute_validation(const std::vector<VarData>& data);

    /**
     * @brief A virtual method to create a StepInfo instance.
     * 
     * @details
     * This method is only called if the ```info()``` method is called for the
     * first time when ```m_step_info``` has not yet been set.
     * 
     * If the Step constructor was called with a non-null StepInfoPtr, this
     * method will never be called.
     * 
     * The base implementation of this method returns a new StepInfo instance
     * without any step or data definitions.
     */
    virtual StepInfoPtr create_step_info();

    /**
     * @brief Raises exception or trap if the Step is in a fault state.
     */
    void trap_on_fault() const;

private:
    Step(const Step&) = delete;
    Step(Step&&) = delete;
    Step& operator=(const Step&) = delete;
    Step& operator=(Step&&) = delete;

private:
    StepInfoPtr m_step_info;
    bool m_init_fault;
    bool m_exec_fault;
};

} // namespace tg::core
