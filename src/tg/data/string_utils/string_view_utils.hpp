#pragma once
#include <string_view>

namespace tg::data::string_view_utils
{

/**
 * @brief Scans a string_view against a set of separator characters, calling
 * a static callback function at each algorithm step.
 * 
 * @details
 * The callback accepts the following arguments:
 * 1. The current position of the algorithm step.
 * 2. The maximum length non-separator string, from the current position.
 * 3. The maximum length separator string, which follows immediately after
 *    ... the non-separator string.
 * 4. A pointer to user-defined data, which allows the static callback function
 *    ... to access additional context or state.
 * 
 * @param input The input string_view to be split. Required; cannot be empty.
 * @param sep_list A string_view containing the characters to be used as separators.
 *     Required; cannot be empty.
 * @param callback Pointer to a static callback function. Required; cannot be null.
 * @param callback_data Pointer to user-defined data that will be passed to the
 *     callback. Required; cannot be null, even if the callback does not need it.
 */
void split_by_charsep(
    std::string_view input,
    std::string_view sep_list,
    bool (*callback)(size_t, const std::string_view&, const std::string_view&, void*),
    void* callback_data
);

/**
 * @brief Returns a default set of whitespace characters used in trimming.
 * 
 * @details
 * The default characters are: " \t\n\r\f\v".
 * 
 * The returned string_view is backed by a string literal with static
 * storage duration.
 * 
 * @returns A string_view containing the default whitespace characters.
 */
std::string_view default_trim_chars();

/**
 * @brief Returns a trimmed string_view as a substring of the input.
 * @param input The input string_view to be trimmed.
 * @param trim_chars The list of trimmable characters.
 * @returns A string_view that is trimmed of leading and trailing characters,
 * or an empty string_view if the input is empty or contains only trimmable
 * characters.
 */
std::string_view trim(
    std::string_view input, 
    std::string_view trim_chars = default_trim_chars()
);

} // namespace tg::data::string_view_utils
