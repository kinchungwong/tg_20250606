#pragma once
#include "tg/core/fwd.hpp"

namespace tg::core::detail
{

/**
 * @brief Utility class for generating names for steps and data.
 * 
 * @note Names are only unique within a single instance of this class.
 */
class UniqueNamer
{
public:
    /**
     * @brief Constructs a UniqueNamer with an initial prefix.
     * 
     * @param prefix The prefix to use for generated names. Must be non-empty.
     * Generally speaking this prefix should contain at least one "reserved
     * characters" such as the pound sign (#) for easier identification of
     * the generated names.
     * 
     * @param start_id The starting ID for the names. Must be non-negative.
     */
    explicit UniqueNamer(std::string prefix, int start_id = 1000);
    ~UniqueNamer();

public:
    std::string generate();

private:
    UniqueNamer(const UniqueNamer&) = delete;
    UniqueNamer(UniqueNamer&&) = delete;
    UniqueNamer& operator=(const UniqueNamer&) = delete;
    UniqueNamer& operator=(UniqueNamer&&) = delete;

private:
    std::string m_prefix;
    int m_next_id; 
};

} // namespace tg::core::detail
