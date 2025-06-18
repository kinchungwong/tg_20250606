#pragma once
#include "tg/core/fwd.hpp"
#include "tg/core/data_info_tuple.hpp"
#include "tg/core/details/scoped_data_info_tuple.hpp"

namespace tg::core
{

struct ParsedDataNames
{
    std::string left_scope;
    std::string left_step;
    std::string left_data;
    std::string right_scope;
    std::string right_step;
    std::string right_data;
};

/**
 * @brief (Not implemented yet) Describes a rule to unify (connect) data definitions.
 * @note A rule is not limited to a single pair of data definitions.
 * @todo Design and implement the rule schema.
 */
struct ConnectedDataInfo{};

/**
 * 
 */
class DataChecker
{
public:
    using ScopedDataInfoTuple = tg::core::details::ScopedDataInfoTuple;

public:
    DataChecker();
    ~DataChecker();

public:
    /**
      * @brief Adds a scope to the data checker.
      */
    void add_scope(ScopePtr scope);


    /**
     * @brief (Not implemented yet) Adds a rule to unify (connect) data definitions.
     */
    void add_connection(const ConnectedDataInfo&) = delete;

#if 0 // PENDING DESIGN
    // /**
    //  * @brief Adds a data connection to unify two data definitions belonging
    //  * to different steps.
    //  * 
    //  * @param lhs_name The first data name. The format is 
    //  * ```scope_name.step_name.data_name```. Of these, ```scope_name```
    //  * and ```step_name``` accepts (```*```) as a wildcard.
    //  *
    //  * @param rhs_name The second data name. The format is same as for
    //  * ```lhs_name```.
    //  * A special case is when ```lhs_name``` has the wildcard
    //  * ```*.*.data_name```, ```rhs_name``` can be omitted. This causes
    //  * all data definitions with the name ```data_name``` across all
    //  * scopes and steps to be unified.
    //  */
    // void connect_data(std::string_view lhs_name, std::string_view rhs_name);
#endif // PENDING DESIGN

    /**
     * @brief Runs all data integrity checks across all added scopes.
     */
    void run_checks();

private:
    void rescan();
    static void ensure_not_null(ScopePtr scope);
    static void ensure_not_null(ScopePtr scope, StepPtr step);
    static ParsedDataNames parse_data_names(std::string_view lhs_name, std::string_view rhs_name);
    static void split_dots(std::string_view input, std::string_view (&result)[4u]);
    static void validate_format(std::string_view name, const std::string_view (&result)[4u]);

private:
    DataChecker(const DataChecker&) = delete;
    DataChecker(DataChecker&&) = delete;
    DataChecker& operator=(const DataChecker&) = delete;
    DataChecker& operator=(DataChecker&&) = delete;

private:
    std::vector<ScopePtr> m_scopes;
    std::vector<ScopedDataInfoTuple> m_parsed_data;
};

} // namespace tg::core
