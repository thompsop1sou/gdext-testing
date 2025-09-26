#ifndef EXAMPLE_TEST_SUITE_H
#define EXAMPLE_TEST_SUITE_H

#include "testing/test_suite.h"

namespace godot
{
    class ExampleTestSuite : public Testing::TestSuite
    {
        GDCLASS(ExampleTestSuite, Testing::TestSuite)

    public:
        ExampleTestSuite();

        DECLARE_TEST_CASE(ExampleTestCase1, example_test_case_1);
        DECLARE_TEST_CASE(ExampleTestCase2, example_test_case_2);

    protected:
        static void _bind_methods();
    };
}

#endif
