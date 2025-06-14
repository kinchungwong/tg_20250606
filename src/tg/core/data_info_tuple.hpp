#pragma once
#include <cstdint>
#include <string>
#include <typeindex>
#include "tg/core/core_enums.hpp"

namespace tg::core
{

struct DataInfoTuple
{
public:
    size_t m_local_index;
    std::string m_shortname;
    DataUsage m_usage;
    std::type_index m_type;

public:
    DataInfoTuple(size_t local_index, std::string shortname, DataUsage usage, std::type_index type);
    ~DataInfoTuple();
    DataInfoTuple(const DataInfoTuple&);
    DataInfoTuple(DataInfoTuple&&);
    DataInfoTuple& operator=(const DataInfoTuple&);
    DataInfoTuple& operator=(DataInfoTuple&&);
};

} // namespace tg::core
