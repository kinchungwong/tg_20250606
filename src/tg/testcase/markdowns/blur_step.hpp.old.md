#pragma once
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <mutex>
#include <vector>
#include <string>
#include <unordered_map>
#include <opencv2/core.hpp>

namespace tg::testcase
{

class Step;
class DataVisitor;
class StepData;
class DataImpl;
template <typename T> class TypedData;
template <typename T> class Input;
template <typename T> class Output;

/**
 * @brief Abstract base class for a processing step in a pipeline.
 */
class Step
{
public:
    virtual ~Step() = default;
    virtual void visit_data(DataVisitor& visitor) = 0;
    virtual void execute() = 0;
};

/**
 * @brief Bit flags to specify the nature of the data.
 * 
 * @note This is different from StepDataUsage, which specifies how the data is used by a Step.
 */
enum class DataFlags
{
};

/**
 * @brief Enum to specify how the data is used by a Step.
 * 
 * @note This is different from DataFlags, which specifies the nature of the data itself.
 * 
 * @details
 * - @var StepDataUsage::None: Not specified; for catching uninitialized values (logic errors).
 * - @var StepDataUsage::Create: Data is created by the Step.
 * - @var StepDataUsage::Read: Data is read by the Step, implies shared read and disallowed write.
 * - @var StepDataUsage::Consume: Data is detached from the global and consumed by the Step, gives exclusive and destructive use.
 */
enum class StepDataUsage
{
    None,
    Create,
    Read,
    Consume
};

/**
 * @brief Implementation details for Data class.
 * 
 * @details
 * This class manages the underlying data, including lifetime, type safety,
 * thread safety, access control (e.g. by which Step, and whether it is an
 * input or output), dependency (completion events), and other metadata.
 *
 * Executor has direct access to this class, while Step implementations
 * can use Input<T> and Output<T> to interact with Data objects.
 * 
 * A data object goes through these phases: design phase, produce phase
 * (as a step output), read phase (as a shared step input), and optionally
 * a consume phase (as a "consumed", i.e. exclusive and destructive step input).
 */
class DataImpl
{
public:
    friend class StepData;
    void copy_to(StepData& data);
    void move_to(StepData& data);
    void copy_from(StepData& data);
    void move_from(StepData& data);

private:
    // mutable std::mutex m_mutex;
    // std::shared_ptr<void> m_ptr;
    // std::type_index m_ti;
    // DataFlags m_flags;
    // TaskId m_producer_tid;
    // std::vector<TaskId> m_reader_tids;
    // TaskId m_consumer_tid;
};

class StepDataKey
{};

class StepData
{
public:
    StepData(Step& task, StepDataUsage usage, std::type_index type);
    virtual ~StepData() = default;
    const StepDataKey& key() const { return m_key; }
    StepDataUsage usage() const { return m_usage; }
    std::type_index expect_type() const { return m_type; }
    virtual void sync() {}
    virtual void clear()
    {
        m_void.reset();
    }

protected:
    friend class DataImpl;
    StepDataKey m_key;
    StepDataUsage m_usage;
    std::type_index m_type;
    std::shared_ptr<void> m_void;
};

template <typename T>
class TypedData : public StepData
{
public:
    TypedData(Step& task, StepDataUsage usage)
        : StepData(task, usage, std::type_index{typeid(T)})
        , m_pvalue()
    {
    }
    virtual void sync() override
    {
        if (m_void && !m_pvalue)
        {
            m_pvalue = std::static_pointer_cast<T>(m_void);
        }
        else if (!m_void && m_pvalue)
        {
            m_void = std::static_pointer_cast<void>(m_pvalue);
        }
    }
    virtual void clear() override
    {
        m_void.reset();
        m_pvalue.reset();
    }
protected:
    std::shared_ptr<T> m_pvalue;
};

/**
 * @brief Typed input data object for a Step.
 */
template <typename T>
class Input : public TypedData<T>
{
public:
    Input();
    const T& operator*() const;
    const T* operator->() const;
};

/**
 * @brief Typed output data object for a Step.
 * 
 * @note The lifetime of the output data starts with the emplace() method.
 */
template <typename T>
class Output : public TypedData<T>  
{
public:
    Output();
    template <typename... Args>
    T& emplace(Args&&... args);
    T& operator*();
    T* operator->();
};

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
};

/**
 * @brief A practical test case to validate the soundness of the framework
 * design.
 */
class BlurStep : public Step
{
public:
    BlurStep();

public:
    Input<cv::Mat>& input() { return m_input; }
    Output<cv::Mat>& output() { return m_output; }
    void visit_data(DataVisitor& visitor);

public:
    void execute();

private:
    Input<cv::Mat> m_input;
    Output<cv::Mat> m_output;
    double m_sigmax;
    double m_sigmay;
};

class GlobalDataKey
{
};

/**
 * @todo
 */
class GlobalData
{
public:
    DataImpl& get(const GlobalDataKey& key);

private:
    std::unordered_map<GlobalDataKey, int> m_map;
    std::vector<std::unique_ptr<DataImpl>> m_data;
};

/**
 * @todo
 */
class DataMap
{
public:
    const GlobalDataKey& get(const StepDataKey& key) const;
private:
    std::unordered_map<StepDataKey, GlobalDataKey> m_map;
};

/**
 * @brief (A mock class) Hosts the code that executes a Step.
 */
class StepExecutor
{
public:
    class StepExecutorDataVisitor final
        : public DataVisitor
    {
    public:
        StepExecutorDataVisitor(GlobalData& global_data, DataMap& data_map, Step& step);
        void visit(StepData& data) final;

    private:
        StepExecutorDataVisitor(const StepExecutorDataVisitor&) = delete;
        StepExecutorDataVisitor(StepExecutorDataVisitor&&) = delete;
        StepExecutorDataVisitor& operator=(const StepExecutorDataVisitor&) = delete;
        StepExecutorDataVisitor& operator=(StepExecutorDataVisitor&&) = delete;

    private:
        GlobalData& m_global_data;
        DataMap& m_data_map;
        Step& m_step;
        bool m_post_execute;
    };

public:
    void execute(GlobalData& global_data, DataMap& data_map, Step& step);
};

} // namespace tg::testcase
