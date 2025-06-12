#include <iostream>
#include <string>
#include <functional>
#include "tg/common/project_macros.hpp"
#include "tg/testcase/ostrm.hpp"
#include "tg/data/specialized/unique_list.hpp"

using tg::data::specialized::UniqueList;

INLINE_NEVER
void unique_list_testcase_1(OStrm& cout)
{
    cout << "evaluating: UniqueList<std::string>" << std::endl;
    tg::data::specialized::UniqueList<std::string> names;
    size_t idx_0 = names.insert("Alice");
    size_t idx_1 = names.insert("Bob");
    size_t idx_2 = names.insert("Charlie");
    size_t idx_3 = names.insert("Alice"); // Duplicate, should not increase size
    cout << "Size after inserts: " << names.size() << std::endl;
    cout << "Index of 'Alice': " << idx_0 << std::endl;
    cout << "Index of 'Bob': " << idx_1 << std::endl;
    cout << "Index of 'Charlie': " << idx_2 << std::endl;
    cout << "Index of second 'Alice': " << idx_3 << std::endl;
    const size_t sz = names.size();
    for (size_t k = 0u; k < sz; ++k)
    {
        const auto& item = names.at(k);
        cout << "Item at " << k << ": " << *item << ", address: " << item.get() << std::endl;
    }
}

struct Name
{
    std::string first_name;
    std::string last_name;
};

struct NameHash
{
    INLINE_ALWAYS
    size_t operator()(const Name& name) const
    {
        const auto h = std::hash<std::string>{};
        return h(name.first_name) & h(name.last_name);
    }
};

struct NameEqual
{
    INLINE_ALWAYS
    bool operator()(const Name& lhs, const Name& rhs) const
    {
        return lhs.first_name == rhs.first_name &&
            lhs.last_name == rhs.last_name;
    }
};

INLINE_NEVER
void unique_list_testcase_2(OStrm& cout)
{
    cout << "evaluating: UniqueList<Name, NameHash, NameEqual>" << std::endl;
    tg::data::specialized::UniqueList<Name, NameHash, NameEqual> names;
    size_t idx_0 = names.insert({"Alice", "Wonderland"});
    size_t idx_1 = names.insert({"Bob", "Builder"});
    size_t idx_2 = names.insert({"Charlie", "Brown"});
    size_t idx_3 = names.insert({"Alice", "Wonderland"}); // Duplicate, should not increase size
    cout << "Size after inserts: " << names.size() << std::endl;
    cout << "Index of 'Alice': " << idx_0 << std::endl;
    cout << "Index of 'Bob': " << idx_1 << std::endl;
    cout << "Index of 'Charlie': " << idx_2 << std::endl;
    cout << "Index of second 'Alice': " << idx_3 << std::endl;
    const size_t sz = names.size();
    for (size_t k = 0u; k < sz; ++k)
    {
        const auto& item = names.at(k);
        cout << "Item at " << k << ": " << item->first_name << " " << item->last_name << ", address: " << item.get() << std::endl;
    }
}

INLINE_NEVER
void unique_list_testcase_3(OStrm& cout)
{
    // To check that all original data shared_ptrs are still valid after
    // shallow_copy, we construct the data first, then insert them
    // (by shared_ptr, not by value) into the UniqueList, and then shallow_copy.
    cout << "evaluating: UniqueList<std::string>::shallow_copy" << std::endl;
    using StrPtr = std::shared_ptr<std::string>;
    using NameListPtr = std::shared_ptr<UniqueList<std::string>>;
    StrPtr alice = std::make_shared<std::string>("Alice");
    StrPtr bob = std::make_shared<std::string>("Bob");
    StrPtr charlie = std::make_shared<std::string>("Charlie");

    NameListPtr names{
        std::make_shared<UniqueList<std::string>>()
    };
    for (size_t repeat_index = 0u; repeat_index < 3u; ++repeat_index)
    {
        // Insert the same names multiple times to check if shallow_copy works correctly
        names->insert(alice);
        names->insert(bob);
        names->insert(charlie);
    }
    cout << "Size after inserts: " << names->size() << std::endl;
    NameListPtr names_copy = names->shallow_copy();
    cout << "Size of shallow copy: " << names_copy->size() << std::endl;
    cout << "Original ptr of Alice: " << alice.get() << std::endl;
    cout << "Original ptr of Bob: " << bob.get() << std::endl;
    cout << "Original ptr of Charlie: " << charlie.get() << std::endl;
    cout << "Shallow copy ptr item[0]: " << names_copy->at(0).get() << std::endl;
    cout << "Shallow copy ptr item[1]: " << names_copy->at(1).get() << std::endl;
    cout << "Shallow copy ptr item[2]: " << names_copy->at(2).get() << std::endl;
    auto index_alice = names_copy->find("Alice");
    std::string str_index_alice = index_alice.has_value() ? std::to_string(index_alice.value()) : "not found";
    cout << "Index of 'Alice' after shallow copy: "
         << str_index_alice << std::endl;
    auto index_bob = names_copy->find("Bob");
    std::string str_index_bob = index_bob.has_value() ? std::to_string(index_bob.value()) : "not found";
    cout << "Index of 'Bob' after shallow copy: "
         << str_index_bob << std::endl;
    auto index_charlie = names_copy->find("Charlie");
    std::string str_index_charlie = index_charlie.has_value() ? std::to_string(index_charlie.value()) : "not found";
    cout << "Index of 'Charlie' after shallow copy: "
         << str_index_charlie << std::endl;
}

INLINE_NEVER
void unique_list_testcase()
{
    OStrm cout;
    unique_list_testcase_1(cout);
    unique_list_testcase_2(cout);
    unique_list_testcase_3(cout);
}
