#pragma once

#include <string>
#include <vector>

namespace tg::data::string_utils
{

std::string trim(
    std::string_view input,
    std::string_view trim_chars
);

std::vector<std::string> split_by_charsep(
    std::string_view input,
    std::string_view sep_list,
    bool trimmed = true,
    bool allow_empty = false,
    size_t max_return = ~static_cast<size_t>(0),
    std::string_view trim_chars = " \t\n\r\f\v"
);

} // namespace tg::data::string_utils
