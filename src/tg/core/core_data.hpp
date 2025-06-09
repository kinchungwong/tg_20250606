#pragma once
#include "tg/core/fwd.hpp"
#include "tg/core/enums.hpp"

namespace tg::core
{

// /**
//  * @brief CoreData manages a piece of data that may be created, shared,
//  * and discarded during multi-threaded execution of a task graph.
//  * 
//  * @details CoreData contains the essential fields common to both
//  * StepData (a composite used by Step) and DataImpl (a composite used 
//  * by ExecData). It supports run-time variants with mutex protection.
//  */
// class CoreData
// {
// public:
//     using MutexType = std::mutex;
//     using LockType = std::unique_lock<MutexType>;

// protected:
//     explicit CoreData(std::type_index type);
//     ~CoreData();
//     void core_assign(std::shared_ptr<void> value);
//     void core_copy_to(CoreData& other) const;
//     void core_clear();
//     const void* core_get() const;
//     void* core_get();

// private:
//     CoreData(const CoreData&) = delete;
//     CoreData(CoreData&&) = delete;
//     CoreData& operator=(const CoreData&) = delete;
//     CoreData& operator=(CoreData&&) = delete;

// private:
//     mutable MutexType m_mutex;
//     std::type_index m_type;
//     std::shared_ptr<void> m_void;
// };

// class StepData
// {

// public:
//     StepData(std::type_index type, StepDataUsage usage);

//     /**
//      * @brief Returns the type-erased pointer, to be cast to the appropriate
//      * type indicated by the type_index.
//      * @return Pointer to the data. If this function returns, the pointer is
//      * guaranteed to be non-null.
//      * @exception std::runtime_error If the data is null.
//      */
//     void* read() const;

//     /**
//      * @brief Assigns a new value to the StepData.
//      * @param value A shared pointer to the new value. The type of the value
//      * must match the type_index of this StepData.
//      */
//     void assign(std::shared_ptr<void> value);

//     std::string name() const;
//     void set_name(const std::string& name);
//     size_t key() const;
//     void set_key(size_t key);
//     void freeze_metadata();

//     /**
//      * @brief Special function to allow 
//      */
//     void visit();

// private:
//     mutable MutexType m_mutex;
//     std::type_index m_type;
//     std::shared_ptr<void> m_void;
//     std::string m_name;
//     size_t m_access_key;
//     bool m_metadata_frozen;
//     StepDataUsage m_usage;
// };

} // namespace tg::core
