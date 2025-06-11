#include "tg/data/data_exceptions.hpp"

namespace tg::data
{

VarDataTypeMismatchException::~VarDataTypeMismatchException() noexcept
{
}

const char* VarDataTypeMismatchException::what() const noexcept
{
    return m_message;
}

VarDataTypeMismatchException::VarDataTypeMismatchException(
    std::optional<std::string_view> message, 
    std::optional<std::type_index> expected_type,
    std::optional<std::type_index> actual_type)
    : std::bad_cast{}
    , m_message{}
{
    std::string_view message_2 = message.value_or("VarData type mismatch");
    std::string_view expected_2 = (
        expected_type.has_value() ? expected_type->name() : "(omitted)"
    );
    std::string_view actual_2 = (
        actual_type.has_value() ? actual_type->name() : "(omitted)"
    );
    snprintf(
        m_message, sizeof(m_message),
        "\"%s\": expected type \"%s\", actual type \"%s\"",
        message_2.data(), expected_2.data(), actual_2.data()
    );
}

} // namespace tg::data
