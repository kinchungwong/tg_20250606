#include <iostream>
#include <string>
#include <functional>
#include "tg/core/fwd.hpp"
#include "tg/core/step.hpp"
#include "tg/core/step_info.hpp"
#include "tg/core/scope.hpp"
#include "tg/core/scope_info.hpp"
#include "tg/common/project_macros.hpp"
#include "tg/testcase/ostrm.hpp"

using namespace tg::core;

class DummyStep : public Step
{
public:
    DummyStep(std::string_view name) : Step{} {
        this->info().set_shortname(name);
    }
    void execute(std::vector<VarData>& data) {}
};

class WeakExpiryChecker
{
public:
    WeakExpiryChecker() = default;
    ~WeakExpiryChecker() = default;
    template <typename T>
    void add(std::shared_ptr<T> wp)
    {
        m_wps.push_back(wp);
    }
    void assert_all_expired(OStrm& cout) const
    {
        for (const auto& wp : m_wps)
        {
            auto sp = wp.lock();
            if (sp)
            {
                throw std::runtime_error("WeakExpiryChecker: at least one weak pointer has not expired.");
            }
        }
        cout << "WeakExpiryChecker success: all weak pointers are expired as expected." << std::endl;
    }
private:
    std::vector<std::weak_ptr<void>> m_wps;
};

void scope_step_iter_testcase_1(OStrm cout, WeakExpiryChecker& wp_checker)
{
    cout << "====== BEGIN scope_step_iter_testcase_1 ======" << std::endl;
    auto a = std::make_shared<DummyStep>("a");
    wp_checker.add(a);
    auto b = std::make_shared<DummyStep>("b");
    wp_checker.add(b);
    auto c = std::make_shared<DummyStep>("c");
    wp_checker.add(c);
    auto d = std::make_shared<DummyStep>("d");
    wp_checker.add(d);
    auto e = std::make_shared<DummyStep>("e");
    wp_checker.add(e);
    auto f = std::make_shared<DummyStep>("f");
    wp_checker.add(f);
    auto scope_zero = std::make_shared<Scope>("scope_zero");
    wp_checker.add(scope_zero);
    auto scope_one = std::make_shared<Scope>("scope_one");
    wp_checker.add(scope_one);
    auto scope_two = std::make_shared<Scope>("scope_two");
    wp_checker.add(scope_two);
    auto scope_three = std::make_shared<Scope>("scope_three");
    wp_checker.add(scope_three);
    scope_one->add(a);
    scope_two->add(b);
    scope_two->add(c);
    scope_three->add(d);
    scope_three->add(e);
    scope_three->add(f);
    auto begin_zero = scope_zero->begin();
    auto end_zero = scope_zero->end();
    auto begin_one = scope_one->begin();
    auto end_one = scope_one->end();
    auto begin_two = scope_two->begin();
    auto end_two = scope_two->end();
    auto begin_three = scope_three->begin();
    auto end_three = scope_three->end();
    auto iters = {
        begin_zero, end_zero, begin_one, end_one, begin_two, end_two, begin_three, end_three
    };
    for (const auto& lhs : iters)
    {
        for (const auto& rhs : iters)
        {
            bool result = (lhs == rhs);
            cout << (result ? "T" : "F") << " ";
        }
        cout << std::endl;
    }
    //
    size_t count_zero = 0u;
    for (auto iter = begin_zero; iter != end_zero; ++iter)
    {
        ++count_zero;
    }
    cout << "count_zero = " << count_zero << std::endl;
    //
    size_t count_one = 0u;
    for (auto iter = begin_one; iter != end_one; ++iter)
    {
        ++count_one;
    }
    cout << "count_one = " << count_one << std::endl;
    //
    size_t count_two = 0u;
    for (auto iter = begin_two; iter != end_two; ++iter)
    {
        ++count_two;
    }
    cout << "count_two = " << count_two << std::endl;
    //
    size_t count_three = 0u;
    for (auto iter = begin_three; iter != end_three; ++iter)
    {
        ++count_three;
    }
    cout << "count_three = " << count_three << std::endl;
    //
    cout << "====== END scope_step_iter_testcase_1 ======" << std::endl;
}

void scope_step_iter_testcase_2(OStrm cout)
{
    cout << "====== BEGIN scope_step_iter_testcase_2 ======" << std::endl;
    auto scope_one = std::make_shared<Scope>("scope_one");
    auto a = std::make_shared<DummyStep>("a");
    scope_one->add(a);
    auto begin_iter = scope_one->begin();
    auto end_iter = scope_one->end();
    auto begin_iter_to_increment = begin_iter;
    try
    {
        ++begin_iter_to_increment;
    }
    catch (std::exception& ex)
    {
        cout << "Incrementing begin-iter (step_count == 1) Failed: expects no exception, but actually throwed." << std::endl;
        cout << ex.what() << std::endl;
    }
    if (begin_iter_to_increment == begin_iter)
    {
        cout << "Incrementing begin-iter (step_count == 1) Failed: after incrementing, should not compare equal with the original begin iter." << std::endl;
    }
    if (begin_iter_to_increment != end_iter)
    {
        cout << "Incrementing begin-iter (step_count == 1) Failed: after incrementing, should compare equal with the end iter." << std::endl;
    }
    auto end_iter_to_increment = end_iter;
    try
    {
        ++end_iter_to_increment;
        // If this line is reached, it's failure.
        cout << "Incrementing end-iter Failed: Expects exception to be throw, but actually not." << std::endl;
    }
    catch (std::exception& ex)
    {
        cout << "Incrementing end-iter works as expected: exception is thrown and caught. For reference, here is the message:" << std::endl;
        cout << "\t" << ex.what() << std::endl;
    }
    // After catching the exception , the "invalidly increment" end-iter should remain unchanged, 
    // i.e. it should still compare equal to the original end-iter, and should still act as a sentinel.
    if (end_iter_to_increment != end_iter)
    {
        cout << "Incrementing end-iter Failed: after the invalid increment attempt threw and caught, expects the end-iter to remain unchanged." << std::endl;
    }
}

void scope_step_iter_testcase()
{
    OStrm cout;
    cout << "===== BEGIN scope_step_iter_testcase ======" << std::endl;
    WeakExpiryChecker wp_checker_1;
    scope_step_iter_testcase_1(cout, wp_checker_1);
    wp_checker_1.assert_all_expired(cout);
    //
    scope_step_iter_testcase_2(cout);
    cout << "===== END scope_step_iter_testcase ======" << std::endl;
}
