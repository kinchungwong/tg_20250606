#pragma once
#include "tg/core/fwd.hpp"
#include "tg/data/specialized/unique_list.fwd.hpp"
#include "tg/data/specialized/equiv_set.hpp"
#include "tg/core/detail/unique_namer.hpp"

namespace tg::core
{

/**
 * @brief A 2D grid of steps and their data usage.
 * 
 * @details This is a diagnostic structure for the task graph system.
 * 
 * The 2D grid represents each step as a row, and each piece of data as a
 * column. The cells of the grid contain the step's usage of the data.
 * 
 * This implementation does not allow incremental updates. All steps and
 * data must be populated at construction. Furthermore, any steps that
 * wish to share data must be linked together before populating the grid,
 * either by name or by instance.
 */
class UsageGrid
{
public:
    using UniqueNamer = tg::core::detail::UniqueNamer;
    using StepList = tg::data::specialized::UniqueList<StepPtr>;
    using DataNameList = tg::data::specialized::UniqueList<std::string>;

public:
    explicit UsageGrid(std::vector<StepPtr> steps, bool generate_names = true);
    ~UsageGrid();

public:
    /**
     * @brief (Debug use only) Dumps the grid as a multi-line string.
     */
    std::string get_diagnostics() const;

private:
    /**
     * @brief Validate caller-provided steps individually, but do not
     * validate data members.
     */
    void validate_steps() const;

    /**
     * @bried Inserts de-duplicated steps into the internal StepList.
     */
    void insert_steps();

    /**
     * @brief Fills or validates data names based on option specified in
     * constructor.
     */
    void fill_or_validate_data_names();

    /**
     * @brief Fills any unassigned data names with generated names.
     * @note After this call, all StepData objects in all steps
     * will have non-empty names.
     */
    void fill_data_names();

    /**
     * @brief Validates that all StepData objects in all steps
     * have non-empty names.
     * @throws std::invalid_argument if any StepData name is empty.
     * @note If this method returns successfully, all StepData
     * objects in all steps will have non-empty names.
     */
    void validate_data_names() const;

    /**
     * @brief Inserts all unique data names into the internal DataNameList.
     */
    void insert_data_names();

    /**
     * @brief Initializes a 2D grid of StepDataUsage large enough for
     * all steps and data names.
     * @note This grid will have many unused cells; their values 
     * will be initialized to StepDataUsage::None.
     */
    void init_grid();

    /**
     * @brief Visits all steps and their StepData to set the usage 
     * information in the grid.
     */
    void fill_usage();

    /**
     * @brief Mutable reference to grid element.
     */
    StepDataUsage& at(size_t step_index, size_t data_index);

    /**
     * @brief Read-only access to grid element.
     */
    const StepDataUsage& at(size_t step_index, size_t data_index) const;

private:
    std::vector<StepPtr> m_orig_steps;
    StepList m_steps;
    DataNameList m_data_names;
    std::vector<StepDataUsage> m_usages;
    UniqueNamer m_namer;
    bool m_generate_names;
};

} // namespace tg::core
