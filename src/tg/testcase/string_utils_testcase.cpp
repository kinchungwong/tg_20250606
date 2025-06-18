#include <cstdint>
#include <iostream>
#include <string>
#include "tg/data/string_utils/string_utils.hpp"
#include "tg/data/string_utils/string_view_utils.hpp"
#include "tg/common/project_macros.hpp"
#include "tg/testcase/ostrm.hpp"

namespace SU = tg::data::string_utils;
namespace SVU = tg::data::string_view_utils;

// ====== About the current state of the test cases ======
// Currently, the test cases are over-exhaustive, meaning that they cover all cases
// but in an inefficient manner.
// There is also no assertions, since hard-coding the correct responses for so
// many cases is not practical.
// Currently, the only meaningful outcome is that the code doesn't crash.
// In the near future, the test cases will be refactored and enhanced.
// ====== 

void string_utils_testcase_1(OStrm& cout)
{
    cout << "\n\n====== BEGIN string_utils_testcase_1 ======\n\n";

    static constexpr bool trimmed_list[] = { false, true };
    static constexpr bool allow_empty_list[] = { false, true };
    static constexpr size_t max_return_list[] = { 0u, 1u, 2u, 3u, 1000u };
    for (bool trimmed : trimmed_list)
    {
        for (bool allow_empty : allow_empty_list)
        {
            for (size_t max_return : max_return_list)
            {
                cout << "Parameters:\n"
                    << "\ttrimmed: " << (trimmed ? "true" : "false") << "\n"
                    << "\tallow_empty: " << (allow_empty ? "true" : "false") << "\n"
                    << "\tmax_return: " << max_return << "\n\n";
                char buf[9u];
                for (unsigned test_id = 0u; test_id < 256u; ++test_id)
                {
                    for (unsigned bit_id = 0u; bit_id < 8u; ++bit_id)
                    {
                        buf[bit_id] = (test_id & (1u << bit_id)) ? '.' : 'a';
                    }
                    buf[8u] = '\0';
                    cout << "Test string: (" << buf << ")\n";
                    std::string input = buf;
                    auto result = SU::split_by_charsep(input, ".", trimmed, allow_empty, max_return);
                    cout << "Result: [ ";
                    for (size_t i = 0; i < result.size(); ++i)
                    {
                        cout << "(" << result[i] << "), ";
                    }
                    cout << "]\n";
                    if (result.size() > max_return)
                    {
                        std::string failure_msg = (
                            "Failure: Result size " + 
                            std::to_string(result.size()) + 
                            " exceeds max_return (" +
                            std::to_string(max_return) + 
                            ")."
                        );
                        cout << failure_msg << "\n";
                        throw std::runtime_error(failure_msg);
                    }
                }
                cout << "\n";
            }
        }
    }
    cout << "\n\n====== END string_utils_testcase_1 ======\n\n";
}

void string_utils_testcase_2(OStrm& cout)
{
    cout << "\n\n====== BEGIN string_utils_testcase_2 ======\n\n";

    const std::vector<std::string> input_parts{{
        "", " ", "  ",
        "a", " a", "a ", " a ",
        "  a", "a  ", "  a  ",
        "a b", " a b", "a b ", " a b ",
        "a  b", "  a  b  ",
    }};
    static constexpr bool trimmed_list[] = { false, true };
    static constexpr bool allow_empty_list[] = { false, true };
    static constexpr const size_t max_return_list[] = {
        0u, 1u, 2u, 3u
    };
    for (bool trimmed : trimmed_list)
    {
        for (bool allow_empty : allow_empty_list)
        {
            for (size_t max_return : max_return_list)
            {
                size_t min_input_parts = std::max<size_t>(1u, max_return) - 1u;
                size_t max_input_parts = max_return + 1u;
                for (size_t num_input_parts = min_input_parts; num_input_parts <= max_input_parts; ++num_input_parts)
                {
                    /**
                     * @brief Construct the input from parts.
                     * @todo
                     */
                    /**
                     * @brief Call the function
                     * @todo
                     */
                    /**
                     * @brief Check the result
                     * @todo
                     */
                }
            }
        }
    }
    cout << "\n\n====== END string_utils_testcase_2 ======\n\n";
}

void string_utils_testcase()
{
    OStrm cout;
    cout << "\n\n\n====== BEGIN string_utils_testcase ======\n\n\n";
    string_utils_testcase_1(cout);
    string_utils_testcase_2(cout);
    // string_utils_testcase_2(cout, ...)
    cout << "\n\n\n====== END string_utils_testcase ======\n\n\n";
}
