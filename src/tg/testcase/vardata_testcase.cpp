#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include "tg/testcase/vardata_testcase.hpp"
#include "tg/data/vardata.hpp"
#include "tg/testcase/ostrm.hpp"

using tg::data::VarData;

void vardata_testcase_1(OStrm& cout)
{
    VarData var1 = std::make_shared<int>(42);
    VarData var2 = std::make_shared<float>(3.14f);
    VarData var3 = std::make_shared<std::string>("Hello, World!");

    cout << "var1: " << var1.as<int>() << "\n";
    cout << "var2: " << var2.as<float>() << "\n";
    cout << "var3: " << var3.as<std::string>() << "\n";
}

void vardata_testcase_2(OStrm& cout)
{
    VarData v(std::make_shared<int>(42));
    try
    {
        float v2 = v.as<float>();
    }
    catch(const std::exception& e)
    {
        cout << "Intentionally triggered " << e.what() << '\n';
    }
}

void vardata_testcase_3(OStrm& cout)
{
    VarData v(std::make_shared<int>(42));
    try
    {
        std::string v2 = v.as<std::string>();
    }
    catch(const std::exception& e)
    {
        cout << "Intentionally triggered " << e.what() << '\n';
    }
}

void vardata_testcase_4(OStrm& cout)
{
    VarData v;
    std::vector<int> data{{1, 2, 3, 4}};
    v = std::make_shared<std::vector<int>>(data);
    cout << "Address of data: " << std::addressof(data) << std::endl;
    cout << "Address of data in VarData: " << std::addressof(v.as<std::vector<int>>()) << std::endl;
    cout << "Reading from std::vector<int> stored in VarData: " << std::endl;
    for (int value : v.as<std::vector<int>>())
    {
        cout << ", " << value << std::endl;
    }
}

void vardata_testcase_5(OStrm& cout)
{
    VarData v;
    std::vector<int> data{{1, 2, 3, 4}};
    v = std::make_shared<std::vector<int>>(data);
    v.as<std::vector<int>>().at(2) = 42; 
    cout << "Reading from std::vector<int> stored in VarData after modifying 3rd element: " << std::endl;
    for (int value : v.as<std::vector<int>>())
    {
        cout << ", " << value << std::endl;
    }
}

void vardata_testcase_6(OStrm& cout)
{
    VarData v;
    v.emplace<std::unordered_map<std::string, int>>();
    auto add_to = [](std::unordered_map<std::string, int>& the_map, const std::string& key, int value) {
        the_map[key] = value;
    };
    add_to(v.as<std::unordered_map<std::string, int>>(), "one", 1);
    add_to(v.as<std::unordered_map<std::string, int>>(), "two", 2);
    add_to(v.as<std::unordered_map<std::string, int>>(), "three", 3);
    cout << "Reading from std::unordered_map<std::string, int> stored in VarData: " << std::endl;
    for (const auto& [key, value] : v.as<std::unordered_map<std::string, int>>())
    {
        cout << key << ": " << value << std::endl;
    }
}

void vardata_testcase()
{
    OStrm cout;
    vardata_testcase_1(cout);
    vardata_testcase_2(cout);
    vardata_testcase_3(cout);
    vardata_testcase_4(cout);
    vardata_testcase_5(cout);
    vardata_testcase_6(cout);
}
