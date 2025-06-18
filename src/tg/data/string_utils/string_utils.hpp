#pragma once

#include <string>
#include <string_view>
#include <vector>
#include "tg/data/string_utils/string_view_utils.hpp"

namespace tg::data::string_utils
{

/**
 * @brief Trim the input by removing leading and trailing characters
 * specified in `trim_chars`.
 * 
 * @remarks
 * Internally, this function uses `string_view_utils::trim`.
 */
std::string trim(
    std::string_view input,
    std::string_view trim_chars
);

/**
 * @brief Split the input string by a set of separator characters,
 * returning a vector of strings.
 * 
 * @remarks
 * Internally, this function uses `string_view_utils::split_by_charsep`
 * to perform the splitting operation.
 */
std::vector<std::string> split_by_charsep(
    std::string_view input,
    std::string_view sep_list,
    bool trimmed = true,
    bool allow_empty = false,
    size_t max_return = ~static_cast<size_t>(0),
    std::string_view trim_chars = tg::data::string_view_utils::default_trim_chars()
);

} // namespace tg::data::string_utils
