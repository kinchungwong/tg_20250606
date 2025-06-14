#include "tg/core/data_info_tuple.hpp"

namespace tg::core
{

DataInfoTuple::DataInfoTuple(size_t local_index, std::string shortname, DataUsage usage, std::type_index type)
    : m_local_index(local_index)
    , m_shortname(std::move(shortname))
    , m_usage(usage)
    , m_type(type)
{}

DataInfoTuple::~DataInfoTuple() = default;
DataInfoTuple::DataInfoTuple(const DataInfoTuple&) = default;
DataInfoTuple::DataInfoTuple(DataInfoTuple&&) = default;
DataInfoTuple& DataInfoTuple::operator=(const DataInfoTuple&) = default;
DataInfoTuple& DataInfoTuple::operator=(DataInfoTuple&&) = default;

} // namespace tg::core
