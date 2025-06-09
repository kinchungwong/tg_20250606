#pragma once
#include <stdexcept>

namespace tg::core
{

class NotImplemented : public std::runtime_error
{
public:
    NotImplemented(const std::string& message)
        : std::runtime_error(message)
    {
    }
};

} // namespace tg::core
