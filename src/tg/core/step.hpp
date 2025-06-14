#pragma once
#include "tg/core/fwd.hpp"

namespace tg::core
{

class Step
{
public:
    using CreateStepInfoFunc = std::function<StepInfoPtr()>;

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
     * @brief Initialize Step as a base class with an initial StepInfo.
     * @param step_info The StepInfo to be associated with this Step.
     * @see Step(CreateStepInfoFunc)
     */
    explicit Step(StepInfoPtr step_info);

    /**
     * @brief Initialize Step as a base class with deferred initialization
     * of StepInfo via a function.
     * @param create_step_info_func A function that creates and returns a
     * StepInfoPtr. This function will be called on the first call to
     * ```info()```.
     * @see Step(StepInfoPtr)
     */
    explicit Step(CreateStepInfoFunc create_step_info_func);

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

private:
    Step(const Step&) = delete;
    Step(Step&&) = delete;
    Step& operator=(const Step&) = delete;
    Step& operator=(Step&&) = delete;

private:
    StepInfoPtr m_step_info;
    CreateStepInfoFunc m_create_step_info_func;
};

} // namespace tg::core
