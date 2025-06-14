#pragma once
#include "tg/core/fwd.hpp"
#include "tg/core/data_info_tuple.hpp"

namespace tg::core
{

/**
 * @brief StepInfo contains information about the Step. 
 * 
 * @details StepInfo is used in two ways: as a builder, and as an frozen
 * (immutable) data object.
 * 
 * As a builder, it allows incremental addition of data information,
 * and allows certain properties (e.g. names) to be changed from their
 * default values.
 * 
 * Once it becomes part of Step, it is frozen for task graph initialization
 * and execution.
 */
class StepInfo
{
public:
    StepInfo();
    explicit StepInfo(std::string_view shortname);
    StepInfo(std::string_view shortname, std::type_index type);

    /**
     * @brief Copy constructor.
     * @note The new copy is initially not frozen.
     */
    StepInfo(const StepInfo& other);

    /**
     * @brief Move constructor.
     * @note The new copy is initially not frozen.
     * @note After moving, the other instance will be both empty and frozen,
     * in order to prevent reuse of an expired instance.
     */
    StepInfo(StepInfo&& other);

    /**
     * @brief Copy assignment.
     * @exception StepInfoFrozen if the StepInfo is frozen.
     * @exception StepInfoNotEmpty if the current StepInfo instance already 
     *            has data definitions. This is to prevent invalidation of
     *            existing data definitions.
     */
    StepInfo& operator=(const StepInfo& other);

    /**
     * @brief Move assignment.
     * @exception StepInfoFrozen if the StepInfo is frozen.
     * @exception StepInfoNotEmpty if the current StepInfo instance already 
     *            has data definitions. This is to prevent invalidation of
     *            existing data definitions.
     */
    StepInfo& operator=(StepInfo&& other);

public:
    /**
     * @brief Prevents further changes to the names of the step and its data.
     */
    void names_freeze();
    bool names_is_frozen() const;

    /**
     * @brief Prevents further changes or additions to the data information.
     */
    void datainfos_freeze();
    bool datainfos_is_frozen() const;

    /**
     * @returns True if all properties are frozen, i.e. the StepInfo instance
     * becomes immutable, and does not require locking for thread-safe access.
     */
    bool is_all_frozen() const;

public: // Definitions of Step information.

    /**
     * @brief Retrieves the short name of the Step.
     */
    const std::string& shortname() const;

    /**
     * @brief Sets the short name of the Step.
     * @exception StepInfoFrozen if the StepInfo is frozen.
     */
    void set_shortname(std::string_view shortname);

    /**
     * @brief Retrieves the type of the Step.
     */
    std::type_index step_type() const;

    /**
     * @brief Sets the type of the Step.
     * @tparam StepType The type of the implementation, which must derive from
     * Step. Do not specify Step itself as the type.
     * @note This property cannot be changed once set.
     * @exception StepInfoFrozen if the StepInfo is frozen.
     */
    template <typename StepType>
    auto set_step_type() -> std::enable_if_t<
        (
            std::is_base_of_v<Step, StepType> &&
            !std::is_same_v<Step, StepType>
        ), void>
    {
        detail_set_step_type(typeid(StepType));
    }

public: // Definitions of data information.

    /**
     * @brief Retrieves the number of data items defined in the Step.
     */
    size_t data_count() const;

    /**
     * @brief Retrieves the data information for a specific data item by its short name.
     * @exception StepInfoNameNotFound if the short name is not found.
     */
    DataInfoTuple get_data_info(std::string_view shortname) const;

    /**
     * @brief Retrieves the data information for a specific data item by its local index.
     * @exception StepInfoBadIndex if the local index is out of bounds.
     */
    DataInfoTuple get_data_info(size_t local_index) const;
    
    /**
     * @brief Retrieves all data information.
     */
    void get_data_infos(std::vector<DataInfoTuple>& datainfos) const;

    /**
     * @brief Adds a new data definition to the Step.
     * @tparam T The type of the data item.
     * @param shortname The short name of the data item.
     * @param usage The usage of the data item, which can be Read, Write, or Consume.
     * @exception StepInfoFrozen if the StepInfo is frozen.
     * @exception StepInfoNameConflict if there is a data definition with
     *            the same short name.
     */
    template <typename T>
    void add_data(std::string_view shortname, DataUsage usage)
    {
        static_assert(!std::is_void_v<T>, 
            "Void cannot be aded as a data type."
        );
        static_assert(!std::is_base_of_v<Step, T>,
            "Step cannot be added as a data type."
        );
        static_assert(!std::is_reference_v<T>, 
            "Cannot add references such as T& or T&& as a data type."
        );
        static_assert(!std::is_pointer_v<T>, 
            "Cannot add pointer such as T* as a data type."
        );
        static_assert(!std::is_array_v<T>, 
            "Cannot add arrays such as T[] as a data type."
        );
        // Checking for m_datainfo_frozen is handled by detail_add_data().
        this->detail_add_data(std::string(shortname), usage, std::type_index(typeid(T)));
    }

    /**
     * @brief Finds the index of a data item by its short name.
     * @returns The index of the data item, or std::nullopt if not found.
     */
    std::optional<size_t> find_data(std::string_view shortname) const;

    /**
     * @brief Renames a data item.
     * @param old_shortname The current short name of the data item.
     * @param new_shortname The new short name for the data item.
     * @exception StepInfoFrozen if the StepInfo is frozen.
     * @exception StepInfoNameConflict if the new short name is already used.
     */
    void rename_data(std::string_view old_shortname, std::string_view new_shortname);

    /**
     * @brief For the specified data, elevates its usage from Read to Consume.
     *
     * @details The three implications of elevating from Read to Consume are:
     * 1. The Step is given exclusive ownership of the data at the start of execution.
     * 2. The Step will be rescheduled so that it can only begin execution after all
     *    other Steps that read the data have finished. This means the Step will 
     *    become the last one to execute among the Steps that depends on the data.
     * 3. The data will not be available as a global output.
     */
    void mark_data_as_consume(std::string_view shortname);

private:
    void detail_set_step_type(std::type_index type);
    size_t detail_add_data(std::string&& shortname, DataUsage usage, std::type_index data_type);

private:
    std::string m_step_shortname;
    std::type_index m_step_type;
    std::vector<DataInfoTuple> m_datainfos;
    std::atomic<bool> m_names_frozen;
    std::atomic<bool> m_datainfos_frozen;
};

// class Step
// {
// public:
//     using CreateStepInfoFunc = std::function<StepInfo()>;

// public:
//     virtual ~Step();

//     /**
//      * @brief Function to be called by the Executor to execute the Step.
//      * 
//      * @param data The array of VarData, used for both inputs and outputs.
//      * 
//      * @note 
//      * The ```data``` array size is same as ```StepInfo::data_count()```,
//      * and the items are stored according to the order of their definition
//      * in the StepInfo.
//      */
//     virtual void execute(std::vector<VarData>& data) = 0;

// public:
//     StepInfo& info();

// protected:
//     /**
//      * @brief Constructs a Step with an initial StepInfo.
//      */
//     explicit Step(StepInfo step_info);

//     /**
//      * @brief Constructs a Step with a deferred-initialized StepInfo.
//      */
//     explicit Step(CreateStepInfoFunc create_step_info_func);

// private:
//     Step(const Step&) = delete;
//     Step(Step&&) = delete;
//     Step& operator=(const Step&) = delete;
//     Step& operator=(Step&&) = delete;

// private:
//     StepInfo m_step_info;
//     CreateStepInfoFunc m_create_step_info_func;
// };

} // namespace tg::core
