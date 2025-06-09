#include <iostream>
#include <string>
#include <functional>
#include "tg/common/project_macros.hpp"
#include "tg/data/specialized/unique_list.hpp"

struct OStrm
{
    std::ostream* m_pstrm;
    OStrm(std::ostream& ostrm)
        : m_pstrm(&ostrm)
    {}


    template <typename T>
    INLINE_NEVER
    FLATTEN
    OStrm& operator<<(T&& value)
    {
        ((*m_pstrm).operator<<)(std::forward<T>(value));
        return *this;
    }

    INLINE_NEVER
    FLATTEN
    OStrm& operator<<(std::ostream& (*f)(std::ostream&))
    {
        f(*m_pstrm);
        return *this;
    }
};

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
}

INLINE_NEVER
void unique_list_testcase()
{
    OStrm cout(std::cout);
    unique_list_testcase_1(cout);
    unique_list_testcase_2(cout);
}
