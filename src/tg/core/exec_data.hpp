#pragma once
#include "tg/core/fwd.hpp"

namespace tg::core
{

/**
 * @brief Global data during task graph execution.
 * 
 * @details
 * During execution, the definitions of global data are frozen, but
 * the smart pointers will be populated, used, and erased when steps
 * are executed.
 * 
 * To reduce mutex contention, the main data structure is made constant
 * (and can only be populated by factory pattern), and will be accessed
 * without mutex during execution time. Whereas access control and mutex
 * protection is delegated to the individual DataImpl objects.
 * 
 * StepData accesses ExecData via AccessKey, which are randomly generated
 * and unique. An AccessKey is unique for each StepData. Internally, an
 * AccessKey maps to the index of the DataImpl, and also the permitted
 * usage of that data by the Step (as configured in StepData).
 */
class ExecData
{
public:
    using MutexType = std::mutex;
    using LockType = std::unique_lock<MutexType>;
    using DataImplPtr = std::unique_ptr<DataImpl>;


    ExecData(std::vector<DataImplPtr> data, std::unordered_map<AccessKey, KeyInfo> keys);
    ~ExecData();

    std::type_index get_type(size_t access_key);
    void set(size_t access_key, std::shared_ptr<void> value);
    std::shared_ptr<void> get(size_t access_key);
    std::shared_ptr<void> consume(size_t access_key);

private:
    /**
     * @brief Lookup the access key.
     * @return KeyInfo containing the data index and usage index. 
     * @exception std::out_of_range If the access key is not found.
     * @note If this method returns, the access key and the returned KeyInfo
     * are guaranteed to be valid.
     */
    const KeyInfo& detail_get_info(size_t access_key) const;

private:
    ExecData(const ExecData&) = delete;
    ExecData(ExecData&&) = delete;
    ExecData& operator=(const ExecData&) = delete;
    ExecData& operator=(ExecData&&) = delete;

private:
    const std::vector<DataImplPtr> m_data;
    const std::unordered_map<AccessKey, KeyInfo> m_keys;
};

} // namespace tg::core
