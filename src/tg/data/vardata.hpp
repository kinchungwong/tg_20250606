#pragma once
#include "tg/data/vardata.fwd.hpp"
#include "tg/data/data_exceptions.hpp"

namespace tg::data
{

template <typename T>
VarData::VarData(std::shared_ptr<T> value)
    : VarData{}
{
    this->operator=(value);
}

template <typename T>
VarData& VarData::operator=(std::shared_ptr<T> value)
{
    if (!value)
    {
        this->clear();
    }
    else
    {
        m_type = typeid(T);
        m_ptr = std::static_pointer_cast<void>(value);
    }
    return *this;
}

template <typename T, typename... Args>
T& VarData::emplace(Args&&... args)
{
    this->operator=(
        std::make_shared<T>(
            std::forward<Args>(args)...
        )
    );
    return this->as<T>();
}

template <typename T>
std::shared_ptr<T> VarData::detach_as()
{
    static constexpr const char* message = "VarData::detach_as<T>()";
    this->type_guard<T>(message);
    this->null_guard(message);
    auto ptr = std::move(m_ptr);
    this->clear();
    return std::static_pointer_cast<T>(ptr);
}

template <typename T>
T& VarData::as() const
{
    static constexpr const char* message = "VarData::as<T>()";
    this->type_guard<T>(message);
    this->null_guard(message);
    return *std::static_pointer_cast<T>(m_ptr);
}

template <typename T>
T* VarData::ptr() const
{
    static constexpr const char* message = "VarData::ptr<T>()";
    this->type_guard<T>(message);
    this->null_guard(message);
    return std::static_pointer_cast<T>(m_ptr).get();
}

template <typename T>
void VarData::type_guard(std::string_view message) const
{
    auto expect = std::type_index(typeid(T));
    this->type_guard_impl(message, expect);
}

} // namespace tg::data
