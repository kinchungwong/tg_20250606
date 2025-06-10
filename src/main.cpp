#include <iostream>
#include <string>
#include "tg/testcase/blur_step_testcase.hpp"
#include "tg/testcase/unique_list_testcase.hpp"
#include "tg/testcase/equiv_set_testcase.hpp"

int main(int argc, char** argv)
{
    try
    {
        // Put any code that needs to test-build here.
        // Remove when done.
        // unique_list_testcase();
        // equiv_set_testcase();
        blur_step_testcase();
        std::cout << "No exception thrown, exiting normally." << std::endl;
        return 0;
    }
    catch (std::exception& ex)
    {
        std::cout << "Exception thrown." << std::endl;
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}
