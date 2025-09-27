#include "example_test_suite.h"

namespace godot
{
    ExampleTestSuite::ExampleTestSuite()
    {
        REGISTER_TEST_CASE(ExampleTestCase1, example_test_case_1);
        REGISTER_TEST_CASE(ExampleTestCase2, example_test_case_2);
    }

    void ExampleTestSuite::_bind_methods()
    {
        BIND_TEST_CASE(ExampleTestSuite, ExampleTestCase1, example_test_case_1);
        BIND_TEST_CASE(ExampleTestSuite, ExampleTestCase2, example_test_case_2);
    }

    DEFINE_TEST_CASE(ExampleTestSuite, ExampleTestCase1, example_test_case_1)
    {
        CHECK_EQUAL(1, 1);
        CHECK_EQUAL(1, 2);
        
        CHECK_NOT_EQUAL(1, 1);
        CHECK_NOT_EQUAL(1, 2);

        CHECK_EQUAL(0.3, 0.1 + 0.2);
        CHECK_APPROX_EQUAL(0.3, 0.1 + 0.2);
    }

    DEFINE_TEST_CASE(ExampleTestSuite, ExampleTestCase2, example_test_case_2)
    {
        auto true_with_error = []() -> bool
        {
            ErrorHandler::push_error("test error");
            return true;
        };

        CHECK_TRUE(true_with_error());
        CHECK_FALSE(true_with_error());

        CHECK_NO_ERRORS(true_with_error());
        CHECK_SOME_ERRORS(true_with_error());
    }
}
