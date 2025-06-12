#include "tg/core/detail/unique_namer.hpp"

namespace tg::core::detail
{

UniqueNamer::UniqueNamer(std::string prefix, int start_id)
    : m_prefix(prefix), m_next_id(start_id)
{
    if (m_prefix.empty())
    {
        throw std::invalid_argument("Prefix cannot be empty.");
    }
    if (m_next_id < 0)
    {
        throw std::invalid_argument("Starting ID must be non-negative.");
    }
}

UniqueNamer::~UniqueNamer()
{
}

std::string UniqueNamer::generate()
{
    int id = m_next_id;
    ++m_next_id;
    return m_prefix + std::to_string(id);
}

} // namespace tg::core::detail
