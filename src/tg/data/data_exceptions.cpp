#include <algorithm>
#include <typeinfo> // to ensure std::type_index::name() return non-empty string
#include "tg/data/data_exceptions.hpp"

namespace /*(unnamed)*/ 
{
    int string_view_length(std::string_view sv, int max_len)
    {
        return static_cast<int>(std::min<size_t>(
            std::max<int>(0, max_len), 
            sv.size()
        ));
    }
}

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
    std::string_view message_or_empty, 
    const std::type_index* p_expected_type,
    const std::type_index* p_actual_type)
    : std::bad_cast{}
    , m_message{}
{
    std::string_view sv_message = "VarData type mismatch";
    std::string_view sv_expected = "(omitted)";
    std::string_view sv_actual = "(omitted)";
    if (message_or_empty.size())
    {
        sv_message = message_or_empty;
    }
    if (p_expected_type)
    {
        sv_expected = p_expected_type->name();
    }
    if (p_actual_type)
    {
        sv_actual = p_actual_type->name();
    }
    /**
     * @note All three fields can possibly be long, in particular mangled type
     * names of standard library types can be long due to mangled type
     * parameters for templates.
     */
    int n_message = string_view_length(sv_message, 256);
    int n_expected = string_view_length(sv_expected, 256);
    int n_actual = string_view_length(sv_actual, 256);
    *std::begin(m_message) = '\0';
    snprintf(
        m_message, sizeof(m_message),
        (
            "\"%.*s\""
            ": expected type "
            "\"%.*s\""
            ", actual type "
            "\"%.*s\""
        ),
        n_message, sv_message.data(),
        n_expected, sv_expected.data(),
        n_actual, sv_actual.data()
    );
    *std::rbegin(m_message) = '\0'; // Ensure null-termination
}

} // namespace tg::data
