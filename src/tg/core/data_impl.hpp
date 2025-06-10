#pragma once
#include "tg/core/fwd.hpp"
#include "tg/core/enums.hpp"

namespace tg::core
{

#if 0
class DataUsageCounter
{
    std::atomic<int> m_create_enter;
    std::atomic<int> m_create_leave;
    std::atomic<int> m_create_success;
    std::atomic<int> m_get_enter;
    std::atomic<int> m_get_leave;
    std::atomic<int> m_get_total;
    std::atomic<int> m_consume_enter;
    std::atomic<int> m_consume_leave;
    std::atomic<int> m_consume_total;
};
#endif

/**
 * @brief Manages a single data item inside ExecData.
 * 
 * @details DataImpl is a composite used by ExecData to manage a piece of data
 * that may be created, shared, and discarded during multi-threaded execution
 * of a task graph. It is not exclusively tied to any Step, but it does verify
 * that any Step that wants to access it has the permission to do so.
 * 
 * During initialization, ExecData will create DataImpl with type_index
 * (gathered from Step.visit_data()) and a list of unique access keys to be used
 * by each Step and StepData. Each access key has an associated permission
 * (set, get, consume).
 * 
 * @see StepData
 */
class DataImpl
{
public:
    using MutexType = std::mutex;
    using LockType = std::unique_lock<MutexType>;

    /**
     * @brief Constructs a DataImpl with the specified type and the list of
     * access keys and their usage permissions.
     *
     * @note The order of the key-usage pairs in the vector is significant.
     * To speed up access, ExecData requires an additional lookup parameter,
     * the usage index, which is basically the index of the key-usage in
     * that vector.
     */
    DataImpl(std::type_index type, std::vector<KeyUsagePair> keyusage);

    /**
     * @brief Destructor.
     */
    ~DataImpl();

    /**
     * @brief Gets the type of the data.
     */
    std::type_index get_type() const;

    /**
     * @brief Gets the type-erased pointer to the data without an AccessKey.
     * @return Pointer to the data, or nullptr if the data is not set.
     * @note For diagnostic use only.
     */
    const void* peek() const;

    /**
     * @brief Sets the data. Requires AccessKey with StepDataUsage::Output.
     */
    void set(size_t usage_index, size_t access_key, std::shared_ptr<void> value);

    /**
     * @brief Gets the data. Requires AccessKey with StepDataUsage::Input.
     */
    std::shared_ptr<void> get(size_t usage_index, size_t access_key);

    /**
     * @brief (Not implemented; do not use.) Consumes the data. Requires
     * AccessKey with StepDataUsage::Consume.
     */
    std::shared_ptr<void> consume(size_t usage_index, size_t access_key);

private:
    DataImpl(const DataImpl&) = delete;
    DataImpl(DataImpl&&) = delete;
    DataImpl& operator=(const DataImpl&) = delete;
    DataImpl& operator=(DataImpl&&) = delete;

private:
    mutable MutexType m_mutex;
    const std::type_index m_type;
    const std::vector<KeyUsagePair> m_keyusage;
    std::shared_ptr<void> m_void;

#if 0    
    std::shared_ptr<DataUsageCounter> m_counter;
#endif
};

} // namespace tg::core
