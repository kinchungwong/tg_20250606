#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <vector>

namespace tg::data::specialized
{

class ParamBag
{
public:
    using PtrTypePair = std::pair<std::shared_ptr<void>, std::type_index>;

public:
    ParamBag();
    ~ParamBag();

    void set(std::string_view name, std::string value);
    void set(std::string_view name, int value);
    void set(std::string_view name, double value);
    void set(std::string_view name, std::vector<std::string> values);
    void set(std::string_view name, std::vector<int> values);
    void set(std::string_view name, std::vector<double> values);

    std::type_index get_type(std::string_view name) const;
    bool is_vector(std::string_view name) const;
    std::type_index get_vector_element_type(std::string_view name) const;

    /**
     * @brief Copies a value from the bag.
     * @tparam T The actual type of the value. Exact match required.
     * @note This method does not implement any conversions.
     * @note T is required to be a complete type and supports copy construction.
     */
    template <typename T>
    T get_as(std::string_view name) const;

private:
    std::unordered_map<std::string, PtrTypePair> m_data;
};

} // namespace tg::data::specialized
