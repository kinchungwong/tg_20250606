// standard includes
#include <iostream>
#include <string>
#include <functional>
// code under test
#include "tg/core/fwd.hpp"
#include "tg/core/step.hpp"
#include "tg/core/step_info.hpp"
#include "tg/core/scope.hpp"
#include "tg/core/scope_info.hpp"
#include "tg/core/data_info_tuple.hpp"
#include "tg/core/dag/data_list.hpp"
#include "tg/core/dag/scoped_data_info_tuple.hpp"
// utilities for test running
#include "tg/common/project_macros.hpp"
#include "tg/testcase/ostrm.hpp"

using namespace tg::core;
using namespace tg::core::dag;

void data_list_testcase_0(OStrm& cout)
{
    cout << "\n====== BEGIN data_list_testcase_0 ======\n";
    DataList datalist;
    cout << "DataList constructor success\n";
    cout << "\n====== END data_list_testcase_0 ======\n";
    cout << std::flush;
}


void data_list_testcase_1(OStrm& cout)
{
    cout << "\n====== BEGIN data_list_testcase_1 ======\n";
    cout << "\nWARNING: THIS TESTCASE IS NOT IMPLEMENTED YET!\n";
    cout << "\n====== END data_list_testcase_1 ======\n";
    cout << std::flush;
}

void data_list_testcase()
{
    OStrm cout;
    cout << std::flush;
    cout << "\n\n\n";
    data_list_testcase_0(cout);
    data_list_testcase_1(cout);
    cout << "\n\n\n";
    cout << std::flush;
}
