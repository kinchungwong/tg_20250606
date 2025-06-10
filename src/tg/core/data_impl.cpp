#include "tg/core/data_impl.hpp"

namespace tg::core
{

DataImpl::DataImpl(std::type_index type, std::vector<KeyUsagePair> keyusage)
    : m_mutex{}
    , m_type(type)
    , m_keyusage(std::move(keyusage))
    , m_void{}
    /* , m_counter(std::make_shared<DataUsageCounter>()) */
{
}

DataImpl::~DataImpl()
{
}

std::type_index DataImpl::get_type() const
{
    LockType lock(m_mutex);
    return m_type;
}

const void* DataImpl::peek() const
{
    LockType lock(m_mutex);
    return m_void.get();
}

void DataImpl::set(size_t usage_index, size_t access_key, std::shared_ptr<void> value)
{
    LockType lock(m_mutex);
    if (usage_index >= m_keyusage.size())
    {
        throw std::out_of_range("DataImpl: bad usage index.");
    }
    KeyUsagePair kup = m_keyusage.at(usage_index);
    if (kup.first != access_key)
    {
        throw std::runtime_error("DataImpl: bad access key.");
    }
    if (kup.second != StepDataUsage::Output)
    {
        throw std::runtime_error("DataImpl: cannot set value for non-output usage.");
    }
    m_void = std::move(value);
}

std::shared_ptr<void> DataImpl::get(size_t usage_index, size_t access_key)
{
    LockType lock(m_mutex);
    if (usage_index >= m_keyusage.size())
    {
        throw std::out_of_range("DataImpl: bad usage index.");
    }
    KeyUsagePair kup = m_keyusage.at(usage_index);
    if (kup.first != access_key)
    {
        throw std::runtime_error("DataImpl: bad access key.");
    }
    if (kup.second != StepDataUsage::Input)
    {
        throw std::runtime_error("DataImpl: cannot get value for non-input usage.");
    }
    return m_void;
}

std::shared_ptr<void> DataImpl::consume(size_t usage_index, size_t access_key)
{
    throw NotImplemented("DataImpl::consume is not implemented yet.");
}

} // namespace tg::core
