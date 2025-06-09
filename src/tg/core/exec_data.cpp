#include "tg/core/exec_data.hpp"
#include "tg/core/data_impl.hpp"

namespace tg::core
{

ExecData::ExecData(
    std::vector<DataImplPtr> data,
    std::unordered_map<AccessKey, KeyInfo> keys
)
    : m_data(std::move(data))
    , m_keys(std::move(keys))
{
    if (m_data.empty())
    {
        throw std::runtime_error("ExecData: No data provided.");
    }
    if (m_keys.empty())
    {
        throw std::runtime_error("ExecData: No keys provided.");
    }
}

ExecData::~ExecData()
{
}

std::type_index ExecData::get_type(size_t access_key)
{
    const auto& info = detail_get_info(access_key);
    return m_data.at(info.data_index)->get_type();
}

void ExecData::set(size_t access_key, std::shared_ptr<void> value)
{
    const auto& info = detail_get_info(access_key);
    m_data.at(info.data_index)->set(info.usage_index, access_key, std::move(value));
}

std::shared_ptr<void> ExecData::get(size_t access_key)
{
    const auto& info = detail_get_info(access_key);
    return m_data.at(info.data_index)->get(info.usage_index, access_key);
}

std::shared_ptr<void> ExecData::consume(size_t access_key)
{
    const auto& info = detail_get_info(access_key);
    return m_data.at(info.data_index)->consume(info.usage_index, access_key);
}

const KeyInfo& ExecData::detail_get_info(size_t access_key) const
{
    auto it = m_keys.find(access_key);
    if (it == m_keys.end())
    {
        throw std::out_of_range("ExecData: Access key not found.");
    }
    return it->second;
}

} // namespace tg::core
