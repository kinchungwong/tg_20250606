#include "tg/data/vardata.hpp"

namespace tg::data
{

VarData::VarData()
    : m_type(typeid(void))
    , m_ptr(nullptr)
{}

VarData::VarData(const VarData& other)
    : m_type(other.m_type)
    , m_ptr(other.m_ptr)
{}

VarData::VarData(VarData&& other)
    : m_type(std::move(other.m_type))
    , m_ptr(std::move(other.m_ptr))
{
    other.clear();
}

VarData& VarData::operator=(const VarData& other)
{
    if (this != &other)
    {
        m_type = other.m_type;
        m_ptr = other.m_ptr;
    }
    return *this;
}

VarData& VarData::operator=(VarData&& other)
{
    if (this != &other)
    {
        m_type = std::move(other.m_type);
        m_ptr = std::move(other.m_ptr);
        other.clear();
    }
    return *this;
}

VarData::~VarData()
{
}

void VarData::clear()
{
    m_type = typeid(void);
    m_ptr.reset();
}

std::type_index VarData::type() const
{
    return m_type;
}

void* VarData::raw() const
{
    return m_ptr.get();
}

bool VarData::has_value() const
{
    return static_cast<bool>(m_ptr.get());
}

bool VarData::operator!() const
{
    return !m_ptr;
}

bool VarData::operator==(const VarData& other) const
{
    return m_type == other.m_type && m_ptr == other.m_ptr;
}

bool VarData::operator!=(const VarData& other) const
{
    return !(*this == other);
}

void VarData::type_guard_impl(std::string_view message, std::type_index expect) const
{
    auto actual = m_type;
    if (actual != expect)
    {
        throw VarDataTypeMismatchException(message, &expect, &actual);
    }
}

void VarData::null_guard(std::string_view message) const
{
    if (!m_ptr)
    {
        throw std::runtime_error(message.data());
    }
}

} // namespace tg::data
