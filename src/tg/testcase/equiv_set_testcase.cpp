#include <iostream>
#include <string>
#include <functional>
#include "tg/common/project_macros.hpp"
#include "tg/testcase/ostrm.hpp"
#include "tg/data/specialized/equiv_set.hpp"

void equiv_set_testcase_1(OStrm& cout)
{
    cout << "running equiv_set_testcase_1()" << std::endl;
    tg::data::specialized::EquivSet es;
    es.insert(0u);
    es.insert(3u);
    es.insert(7u);
    es.link(7u, 5u);
    std::vector<size_t> roots;
    std::vector<std::pair<size_t, size_t>> members;
    es.export_sorted(roots, members);
    for (size_t root : roots)
    {
        cout << "root: " << root << std::endl;
    }
    for (const auto& r_and_m : members)
    {
        const size_t root = r_and_m.first;
        const size_t member = r_and_m.second;
        cout << "root: " << root << ", member: " << member << std::endl;
    }
}

void equiv_set_testcase_2(OStrm& cout)
{
    cout << "running equiv_set_testcase_2()" << std::endl;
    tg::data::specialized::EquivSet es;
    es.insert(0u);
    es.insert(3u);
    es.insert(7u);
    es.link(7u, 5u);
    std::vector<size_t> members;
    for (size_t i = 0; i < 10u; ++i)
    {
        es.export_one(i, members);
        cout << "Dumping the equiv. set containing " << i << ": ";
        if (members.empty())
        {
            cout << "none" << std::endl;
            continue;
        }
        for (size_t member : members)
        {
            cout << member << ", ";
        }
        cout << std::endl;
    }
}

void equiv_set_testcase()
{
    OStrm cout;
    equiv_set_testcase_1(cout);
    equiv_set_testcase_2(cout);
}
