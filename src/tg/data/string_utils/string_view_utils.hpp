#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include <stdexcept>
#include <vector>
#include <array>

namespace tg::data::string_view_utils
{

void split_by_charsep(
    std::string_view input,
    std::string_view sep_list,
    bool (*callback)(const std::string_view&, const std::string_view&, void*) = nullptr,
    void* callback_data = nullptr
);

std::string_view trim(
    std::string_view input, 
    std::string_view trim_chars = " \t\n\r\f\v"
);

} // namespace tg::data::string_view_utils
