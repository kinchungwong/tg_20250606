#include <iostream>
#include <string>
#include <functional>
#include "tg/common/project_macros.hpp"
#include "tg/testcase/ostrm.hpp"

#if 0 // Not merged yet.

#include "tg/core/detail/equiv_name_set.hpp"

void equiv_name_set_testcase_101(OStrm& cout)
{
    cout << "running equiv_name_set_testcase_101()" << std::endl;
    tg::core::detail::EquivNameSet ens;
    ens.insert("a");
    ens.insert("b");
    ens.insert("c");
    ens.insert("d");
    ens.insert("e");
    ens.insert("f");
    ens.link("c", "d");
    ens.link("e", "f");
    ens.link("a", "c");
    ens.link("a", "f");
    std::vector<std::string> members;
    ens.get_members("f", members);
    cout << "Dumping the equiv. set containing 'f': ";
    if (members.empty())
    {
        cout << "none" << std::endl;
        return;
    }
    for (const auto& member : members)
    {
        cout << member << ", ";
    }
    cout << std::endl;
}

void equiv_name_set_testcase()
{
    OStrm cout;
    equiv_name_set_testcase_101(cout);
}

#endif // Not merged yet.
