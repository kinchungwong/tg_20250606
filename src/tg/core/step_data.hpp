#pragma once
#include "tg/core/fwd.hpp"
#include "tg/core/enums.hpp"
#include "tg/core/exec_data.hpp"

namespace tg::core
{

class StepData
{
public:
    using MutexType = std::mutex;
    using LockType = std::unique_lock<MutexType>;

public:
    StepData(std::type_index type, StepDataUsage usage);

    /**
     * @brief Returns the type-erased pointer, to be cast to the appropriate
     * type indicated by the type_index.
     * @return Pointer to the data. If this function returns, the pointer is
     * guaranteed to be non-null.
     * @exception std::runtime_error If the data is null.
     */
    void* read() const;

    /**
     * @brief Assigns a new value to the StepData.
     * @param value A shared pointer to the new value. The type of the value
     * must match the type_index of this StepData.
     */
    void assign(std::shared_ptr<void> value);

    std::string name() const;
    void set_name(const std::string& name);
    std::string scope_name() const;
    void set_scope_name(const std::string& scope_name);
    AccessKey key() const;
    void set_key(AccessKey key);
    StepDataUsage usage() const;
    void freeze_metadata();

    /**
     * @brief Uses the AccessKey to sync input from ExecData.
     * @see StepExecutor
     */
    void sync_from_exec_data(ExecData& exec_data);

    /**
     * @brief Uses the AccessKey to sync output to ExecData.
     * @see StepExecutor
     */
    void sync_to_exec_data(ExecData& exec_data);

private:
    [[noreturn]]
    void raise_type_mismatch(std::type_index data_impl_type) const;

    [[noreturn]]
    void raise_output_not_set() const;

private:
    mutable MutexType m_mutex;
    std::type_index m_type;
    std::shared_ptr<void> m_void;
    std::string m_name;
    std::string m_scope_name;
    AccessKey m_key;
    bool m_metadata_frozen;
    StepDataUsage m_usage;
};

} // namespace tg::core
