#include <stdexcept>
#include "tg/core/step_data.hpp"

namespace tg::core
{

StepData::StepData(std::type_index type, StepDataUsage usage)
    : m_mutex{}
    , m_type(type)
    , m_void{}
    , m_name{}
    , m_scope_name{}
    , m_key(0)
    , m_metadata_frozen(false)
    , m_usage(usage)
{
}

void* StepData::read() const
{
    LockType lock(m_mutex);
    void* p = m_void.get();
    if (!p)
    {
        char msg[512u];
        *std::begin(msg) = '\0';
        snprintf(msg, sizeof(msg), "StepData[\"%s\"]: pointer is null for type %s.", m_name.c_str(), m_type.name());
        throw std::runtime_error(msg);
    }
    return p;
}

void StepData::assign(std::shared_ptr<void> value)
{
    LockType lock(m_mutex);
    m_void = std::move(value);
}

std::string StepData::name() const
{
    LockType lock(m_mutex);
    return m_name;
}

void StepData::set_name(const std::string& name)
{
    LockType lock(m_mutex);
    if (m_metadata_frozen)
    {
        throw std::runtime_error("StepData: Cannot set name when metadata is frozen.");
    }
    m_name = name;
}

std::string StepData::scope_name() const
{
    LockType lock(m_mutex);
    return m_scope_name;
}

void StepData::set_scope_name(const std::string& scope_name)
{
    LockType lock(m_mutex);
    if (m_metadata_frozen)
    {
        throw std::runtime_error("StepData: Cannot set scope name when metadata is frozen.");
    }
    m_scope_name = scope_name;
}

AccessKey StepData::key() const
{
    LockType lock(m_mutex);
    return m_key;
}

void StepData::set_key(AccessKey key)
{
    LockType lock(m_mutex);
    if (m_metadata_frozen)
    {
        throw std::runtime_error("StepData: Cannot set key when metadata is frozen.");
    }
    m_key = key;
}

StepDataUsage StepData::usage() const
{
    LockType lock(m_mutex);
    return m_usage;
}

void StepData::freeze_metadata()
{
    LockType lock(m_mutex);
    m_metadata_frozen = true;
}

void StepData::sync_from_exec_data(ExecData& exec_data)
{
    LockType lock(m_mutex);
    if (m_usage != StepDataUsage::Input)
    {
        return;
    }
    auto data_impl_type = exec_data.get_type(m_key);
    if (data_impl_type != m_type)
    {
        raise_type_mismatch(data_impl_type);
    }
    m_void = exec_data.get(m_key);
}

void StepData::sync_to_exec_data(ExecData& exec_data)
{
    LockType lock(m_mutex);
    if (m_usage != StepDataUsage::Output)
    {
        return;
    }
    auto data_impl_type = exec_data.get_type(m_key);
    if (data_impl_type != m_type)
    {
        raise_type_mismatch(data_impl_type);
    }
    if (!m_void)
    {
        raise_output_not_set();
    }
    exec_data.set(m_key, std::move(m_void));
    m_void.reset();
}

void StepData::raise_type_mismatch(std::type_index data_impl_type) const
{
    char msg[512u];
    *std::begin(msg) = '\0';
    snprintf(msg, sizeof(msg),
        "StepData[\"%s\"]: type mismatch. Expected %s (in StepData), got %s (from ExecData).",
        m_name.c_str(), m_type.name(), data_impl_type.name());
    throw std::runtime_error(msg);
}

void StepData::raise_output_not_set() const
{
    char msg[512u];
    *std::begin(msg) = '\0';
    snprintf(msg, sizeof(msg),
        "StepData[\"%s\"]: output not set. Type %s. Cannot sync to ExecData without a value.",
        m_name.c_str(), m_type.name());
    throw std::runtime_error(msg);
}

} // namespace tg::core
