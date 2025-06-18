#include <stdexcept>
#include "tg/data/string_utils/string_utils.hpp"
#include "tg/data/string_utils/string_view_utils.hpp"
#include "tg/common/project_macros.hpp"

namespace tg::data::string_utils::details
{
struct SplitByCharsepCallback
{
    std::vector<std::string>* result_vec_ptr;
    bool trimmed;
    bool allow_empty;
    size_t max_return;
    std::string_view trim_chars;

    bool operator()(std::string_view part_nonsep)
    {
        if (!result_vec_ptr)
        {
            return false;
        }
        std::vector<std::string>& ref_result = *result_vec_ptr;
        const size_t prev_sz = ref_result.size();
        if (prev_sz >= max_return)
        {
            return false;
        }
        std::string_view candidate = part_nonsep;
        if (trimmed && !trim_chars.empty())
        {
            candidate = string_view_utils::trim(candidate, trim_chars);
        }
        if (allow_empty || !candidate.empty())
        {
            ref_result.emplace_back(candidate);
        }
        bool should_continue = ref_result.size() < max_return;
        return should_continue;
    }

    static bool static_callback_func(
        size_t /*start_nonsep*/,
        const std::string_view& part_nonsep,
        const std::string_view& /*part_sep*/,
        void* user_data
    )
    {
        if (!user_data)
        {
            return false;
        }
        auto& self = *static_cast<SplitByCharsepCallback*>(user_data);
        return self(part_nonsep);
    }
}; // struct SplitByCharsepCallback

} // namespace tg::data::string_utils::details

namespace tg::data::string_utils
{

std::string trim(std::string_view input, std::string_view trim_chars)
{
    return std::string(string_view_utils::trim(input, trim_chars));
}

std::vector<std::string> split_by_charsep(
    std::string_view input, std::string_view sep_list,
    bool trimmed, bool allow_empty, size_t max_return,
    std::string_view trim_chars
)
{
    if (input.empty() || sep_list.empty() || max_return == 0)
    {
        return {};
    }
    std::vector<std::string> result;
    using CB = details::SplitByCharsepCallback;
    constexpr auto stc_fn = CB::static_callback_func;
    CB callback_data;
    callback_data.result_vec_ptr = &result;
    callback_data.trimmed = trimmed;
    callback_data.allow_empty = allow_empty;
    callback_data.max_return = max_return;
    callback_data.trim_chars = trim_chars;
    string_view_utils::split_by_charsep(input, sep_list, stc_fn, &callback_data);
    return result;
}

} // namespace tg::data::string_utils
