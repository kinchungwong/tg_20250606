#include "tg/data/string_utils/string_view_utils.hpp"
#include "tg/common/project_macros.hpp"

namespace tg::data::string_view_utils
{


INLINE_NEVER
void split_by_charsep(
    std::string_view input,
    std::string_view sep_list,
    bool (*callback)(size_t, const std::string_view&, const std::string_view&, void*),
    void* callback_data
)
{
    if (input.empty() || sep_list.empty() || !callback || !callback_data)
    {
        return;
    }
    /**
     * @details
     * The algorithm works by:
     * (1) scanning the input string, and 
     * (2) by alternating between two types of searches:
     * (2.1) the next longest non-separator substring, and then,
     * (2.2) the next longest separator substring that comes next.
     * (3) Each pair of result is sent to the callback.
     * 
     * The callback is expected to return `true` to continue processing,
     * or `false` to stop processing.
     * 
     * The substrings that are sent to the callback may be empty;
     * the callback is expected to interpret them accordingly.
     */
    constexpr size_t npos = std::string_view::npos;
    const size_t len = input.length();
    size_t next = 0u;
    while (next < len)
    {
        /**
         * @details
         * The result of the two types of searches are represented by:
         * (1) ```(start_nonsep, end_nonsep)``` (excluding end_nonsep) ...
         * ... for the non-separator substring, and
         * (2) ```(end_nonsep, end_sep)``` (excluding end_sep) ...
         * ... for the separator substring.
         */
        const size_t start_nonsep = next;
        const size_t end_nonsep = input.find_first_of(sep_list, start_nonsep);
        const size_t start_sep = end_nonsep;
        const size_t end_sep = (start_sep == npos) ? npos : input.find_first_not_of(sep_list, start_sep);
        next = end_sep;
        if (end_nonsep == npos)
        {
            /**
             * @note The string_view arguments passed to the callback can be
             * function-local temporaries, but they cannot be inline temporaries.
             */
            std::string_view part_nonsep = input.substr(start_nonsep);
            std::string_view part_sep;
            callback(start_nonsep, part_nonsep, part_sep, callback_data);
            return;
        }
        else if (end_sep == npos)
        {
            // The input string ends with one or more separator.
            std::string_view part_nonsep = input.substr(start_nonsep, end_nonsep - start_nonsep);
            std::string_view part_sep = input.substr(start_sep);
            callback(start_nonsep, part_nonsep, part_sep, callback_data);
            return;
        }
        else
        {
            // Both non-separator and separator substrings are found,
            // and then there are more in the input.
            std::string_view part_nonsep = input.substr(start_nonsep, end_nonsep - start_nonsep);
            std::string_view part_sep = input.substr(start_sep, end_sep - start_sep);
            bool should_continue = callback(start_nonsep, part_nonsep, part_sep, callback_data);
            if (!should_continue)
            {
                return;
            }
        }
    }
}

std::string_view default_trim_chars()
{
    // The default characters are: " \t\n\r\f\v"
    // These are the common whitespace characters.
    static constexpr const char default_chars[] = " \t\n\r\f\v";
    return std::string_view{default_chars};
}

INLINE_NEVER
std::string_view trim(std::string_view input, std::string_view trim_chars)
{
    size_t start = input.find_first_not_of(trim_chars);
    if (start == std::string_view::npos)
    {
        return {};
    }
    size_t len = input.find_last_not_of(trim_chars) - start + 1u;
    return input.substr(start, len);
}

} // namespace tg::data::string_view_utils
