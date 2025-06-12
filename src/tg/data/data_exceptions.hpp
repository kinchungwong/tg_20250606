#pragma once

#include <stdexcept>
#include <string_view>
#include <typeindex>

namespace tg::data
{

/**
 * @brief Exception thrown when a type mismatch occurs in VarData operations.
 * 
 * @details This exception is used to indicate that an operation on a VarData
 * failed due to a type mismatch, such as trying to access data of a different
 * type than what is stored in the VarData.
 */
class VarDataTypeMismatchException : public std::bad_cast
{
public:
    /**
     * @brief Constructs a VarDataTypeMismatchException with a custom message.
     * 
     * @param message The custom error message (optional).
     * @param expected_type The expected type (optional).
     * @param actual_type The actual type (optional).
     */
    explicit VarDataTypeMismatchException(
        std::string_view message_or_empty, 
        const std::type_index* p_expected_type = nullptr,
        const std::type_index* p_actual_type = nullptr);

    virtual ~VarDataTypeMismatchException() noexcept final;

    virtual const char* what() const noexcept final;

private:
    char m_message[1024u];
};

} // namespace tg::data
