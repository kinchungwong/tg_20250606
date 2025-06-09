#include <stdexcept>
#include "tg/core/step_data.hpp"

namespace tg::core
{

StepData::StepData(std::type_index type, StepDataUsage usage)
    : m_mutex{}
    , m_type(type)
    , m_void{}
    , m_name{}
    , m_access_key(0)
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

size_t StepData::key() const
{
    LockType lock(m_mutex);
    return m_access_key;
}

void StepData::set_key(size_t key)
{
    LockType lock(m_mutex);
    if (m_metadata_frozen)
    {
        throw std::runtime_error("StepData: Cannot set key when metadata is frozen.");
    }
    m_access_key = key;
}

void StepData::freeze_metadata()
{
    LockType lock(m_mutex);
    m_metadata_frozen = true;
}

} // namespace tg::core
