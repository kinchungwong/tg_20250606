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
// demo steps
#include "tg/core/testcase/blur_step.hpp"
#include "tg/core/testcase/conncomp_step.hpp"
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
    cout << "DataList collects scopes, steps and their data for binding.\n";
    auto scope = std::make_shared<Scope>("DemoScope");
    auto blur = std::make_shared<tg::core::testcase::BlurStep>();
    auto conn = std::make_shared<tg::core::testcase::ConnCompStep>();
    scope->add(blur);
    scope->add(conn);

    DataList datalist;
    datalist.add_scope(scope);

    cout << "Data items collected: " << datalist.data_count() << "\n";
    for (size_t i = 0; i < datalist.data_count(); ++i)
    {
        const auto& info = datalist.datainfos().at(i);
        cout << "(" << i << ") " << info.m_qualified_name
             << " equiv:" << info.m_data_equiv_label << "\n";
    }

    size_t label_blur_input = 0;
    size_t label_conn_input = 0;
    bool found_blur = false;
    bool found_conn = false;
    for (const auto& info : datalist.datainfos())
    {
        if (info.m_data_shortname == "input" && info.m_step_index == 0)
        {
            label_blur_input = info.m_data_equiv_label;
            found_blur = true;
        }
        if (info.m_data_shortname == "input" && info.m_step_index == 1)
        {
            label_conn_input = info.m_data_equiv_label;
            found_conn = true;
        }
    }
    if (found_blur && found_conn && label_blur_input == label_conn_input)
    {
        cout << "Equivalence label match success\n";
    }
    else
    {
        cout << "Equivalence label mismatch\n";
    }
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
