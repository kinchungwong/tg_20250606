#include "tg/core/data_checker.hpp"
#include "tg/core/step.hpp"
#include "tg/core/step_info.hpp"
#include "tg/core/scope.hpp"
#include "tg/core/scope_info.hpp"
#include "tg/core/data_info_tuple.hpp" // as returned from StepInfo.
// #include "tg/core/dag/scoped_data_info_tuple.hpp" // for use within DataList.
#include "tg/data/specialized/equiv_set.hpp"

namespace tg::core
{

using ScopedDataInfoTuple = DataChecker::ScopedDataInfoTuple;

DataChecker::DataChecker()
    : m_scopes()
{
}

DataChecker::~DataChecker() = default;

void DataChecker::add_scope(ScopePtr scope)
{
    if (!scope)
    {
        throw std::invalid_argument("Cannot add a null scope to DataChecker.");
    }
    m_scopes.push_back(std::move(scope));
}

#if 0 // PENDING DESIGN
// void add_connection(const ConnectedDataInfo&) = delete;
#endif // PENDING DESIGN

#if 0 // PENDING DESIGN
// void DataChecker::connect_data(std::string_view lhs_name, std::string_view rhs_name)
// {}
#endif // PENDING DESIGN

void DataChecker::run_checks()
{
    rescan();
    /**
     * @todo Implement the actual checks.
     * @details
     * 1. Checks rules on the parsed data definitions, individually.
     * 2. Validate any additional data unification rules, if specified.
     * 3. Apply the unificiation (both default and user-defined), capture result.
     * 4. Checks rules after unification.
     */
}

void DataChecker::rescan()
{
    const size_t scope_count = m_scopes.size();
    for (size_t scope_index = 0; scope_index < scope_count; ++scope_index)
    {
        const auto& scope = m_scopes.at(scope_index);
        ensure_not_null(scope);
        const auto& scopename = scope->scopename();
        const size_t step_count = scope->step_count();
        for (size_t step_index = 0; step_index < step_count; ++step_index)
        {
            const auto& step = scope->step_at(step_index);
            ensure_not_null(scope, step);
            const auto& step_info = step->info();
            const auto& step_shortname = step_info.shortname();
            const size_t data_count = step_info.data_count();
            for (size_t data_local_index = 0; data_local_index < data_count; ++data_local_index)
            {
                const auto& data_info = step_info.get_data_info(data_local_index);
                // assigned_equiv_label: Prior to unification, all items are 
                // assigned their current index as their equivalence label,
                // essentially making each item into a singleton.
                const size_t assigned_equiv_label = m_parsed_data.size();
                ScopedDataInfoTuple scoped_data_info;
                scoped_data_info.m_wp_scope = scope;
                scoped_data_info.m_wp_step = step;
                scoped_data_info.m_scope_index = scope_index;
                scoped_data_info.m_step_index = step_index;
                scoped_data_info.m_step_data_index = data_local_index;
                scoped_data_info.m_data_equiv_label = assigned_equiv_label;
                scoped_data_info.m_data_shortname = data_info.m_shortname;
                scoped_data_info.m_qualified_name = (
                    scopename + "." + step_shortname + "." + data_info.m_shortname
                );
                scoped_data_info.m_usage = data_info.m_usage;
                scoped_data_info.m_type = data_info.m_type;
                m_parsed_data.push_back(scoped_data_info);
            }
        }
    }
}

void DataChecker::ensure_not_null(ScopePtr scope)
{
    if (!scope)
    {
        throw std::invalid_argument("DataChecker::ensure_not_null(): Encountered a null scope.");
    }
}

void DataChecker::ensure_not_null(ScopePtr scope, StepPtr step)
{
    ensure_not_null(scope);
    if (!step)
    {
        auto scope_name = scope->scopename();
        if (scope_name.empty())
        {
            scope_name = "(noname)";
        }
        auto msg = std::string(
            "DataChecker::ensure_not_null(): "
            "Encountered a null step in scope: "
        );
        msg += scope_name;
        throw std::invalid_argument(msg);
    }
}

ParsedDataNames DataChecker::parse_data_names(std::string_view lhs_name, std::string_view rhs_name)
{
    std::string_view lhs_parts[4u];
    std::string_view rhs_parts[4u];
    split_dots(lhs_name, lhs_parts);
    split_dots(rhs_name, rhs_parts);
    validate_format(lhs_name, lhs_parts);
    validate_format(rhs_name, rhs_parts);
    ParsedDataNames parsed;
    parsed.left_scope = lhs_parts[0];
    parsed.left_step = lhs_parts[1];
    parsed.left_data = lhs_parts[2];
    parsed.right_scope = rhs_parts[0];
    parsed.right_step = rhs_parts[1];
    parsed.right_data = rhs_parts[2];
    return parsed;
}

void DataChecker::validate_format(std::string_view name, const std::string_view (&result)[4u])
{
    // Expects three non-empty parts. There should not be a fourth part.
    if (result[0].empty() || result[1].empty() || result[2].empty() || !result[3].empty())
    {
        throw std::invalid_argument("Invalid data name format: " + std::string(name));
    }
}

void DataChecker::split_dots(std::string_view input, std::string_view (&result)[4u])
{
    /**
     * @note This function is written in a more generic way than is required
     * by DataChecker, so that it may be extracted into a reusable utility function.
     */
    const size_t npos = std::string_view::npos;
    const size_t len = input.length();
    // Initialize to empty strings with valid lifetime,
    // basically a zero-length substring at the end of the input.
    result[0] = input.substr(len, 0u);
    result[1] = input.substr(len, 0u);
    result[2] = input.substr(len, 0u);
    result[3] = input.substr(len, 0u);
    // First portion
    size_t dot_first = input.find_first_of('.');
    if (dot_first == npos)
    {
        result[0] = input;
        return;
    }
    else
    {
        result[0] = input.substr(0, dot_first);
    }
    // Second portion
    size_t dot_second = input.find_first_of('.', dot_first + 1);
    if (dot_second == npos)
    {
        result[1] = input.substr(dot_first + 1);
        return;
    }
    else
    {
        result[1] = input.substr(dot_first + 1, dot_second - dot_first - 1);
    }
    // Third portion
    size_t dot_third = input.find_first_of('.', dot_second + 1);
    if (dot_third == npos)
    {
        result[2] = input.substr(dot_second + 1);
        return;
    }
    else
    {
        result[2] = input.substr(dot_second + 1, dot_third - dot_second - 1);
    }
    // Fourth portion, which captures the rest of the string.
    // (It will not be further parsed even if the rest of the string contains a dot.)
    result[3] = input.substr(dot_third + 1);
}

} // namespace tg::core
