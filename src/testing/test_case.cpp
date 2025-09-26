#include "test_case.h"

#include <godot_cpp/variant/utility_functions.hpp>

#include "testing/test_suite.h"

namespace godot::Testing
{
    TestCase::TestCase(TestSuite* parent_test_suite) : m_parent_test_suite(parent_test_suite)
    {}

    TestSuite* TestCase::get_parent_test_suite() const
    {
        return m_parent_test_suite;
    }

    void TestCase::set_parent_test_suite(TestSuite* parent_test_suite)
    {
        m_parent_test_suite = parent_test_suite;
    }

    void TestCase::run()
    {
        UtilityFunctions::print_rich(LINE);
        UtilityFunctions::print_rich(vformat("\n%s%s%s running...\n", TEST_NAME_START, this->get_class(), TEST_NAME_END));

        m_passed_checks = 0;
        m_failed_checks = 0;
        m_errored_checks = 0;

        test_body();

        if (m_suppress_details)
            UtilityFunctions::print_rich("");

        String test_case_results;
        if (m_failed_checks == 0)
            test_case_results = vformat("%s[PASSED]%s", PASSED_START, PASSED_END);
        else if (m_passed_checks == 0)
            test_case_results = vformat("%s[FAILED]%s", FAILED_START, FAILED_END);
        else
            test_case_results = vformat("%s[MIXED]%s", MIXED_START, MIXED_END);
        if (m_errored_checks > 0)
            test_case_results += vformat(" %swith errors%s", ERRORED_START, ERRORED_END);

        UtilityFunctions::print_rich(vformat("%s%s%s results: %s", TEST_NAME_START, this->get_class(), TEST_NAME_END, test_case_results));
        UtilityFunctions::print_rich(vformat("    checks passed: %s%s%s", PASSED_START, m_passed_checks, PASSED_END));
        UtilityFunctions::print_rich(vformat("    checks failed: %s%s%s", FAILED_START, m_failed_checks, FAILED_END));
        UtilityFunctions::print_rich(vformat("    checks with unexpected errors: %s%s%s\n", ERRORED_START, m_errored_checks, ERRORED_END));
        UtilityFunctions::print_rich(LINE);
    }

    Callable TestCase::get_runner()
    {
        return m_runner;
    }

    bool TestCase::get_suppress_details() const
    {
        return m_suppress_details;
    }

    void TestCase::set_suppress_details(bool suppress_details)
    {
        m_suppress_details = suppress_details;
    }

    std::int64_t TestCase::get_passed_checks() const
    {
        return m_passed_checks;
    }

    std::int64_t TestCase::get_failed_checks() const
    {
        return m_failed_checks;
    }

    std::int64_t TestCase::get_errored_checks() const
    {
        return m_errored_checks;
    }

    void TestCase::_bind_methods()
    {
        ClassDB::bind_method(D_METHOD("get_parent_test_suite"), &TestCase::get_parent_test_suite);

        ClassDB::bind_method(D_METHOD("run"), &TestCase::run);
        ClassDB::bind_method(D_METHOD("get_runner"), &TestCase::get_runner);
        ADD_PROPERTY(PropertyInfo(Variant::CALLABLE, "runner", PROPERTY_HINT_TOOL_BUTTON, "Run test case,Play", PROPERTY_USAGE_EDITOR), "", "get_runner");

        ClassDB::bind_method(D_METHOD("get_suppress_details"), &TestCase::get_suppress_details);
        ClassDB::bind_method(D_METHOD("set_suppress_details", "suppress_details"), &TestCase::set_suppress_details);
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "suppress_details"), "set_suppress_details", "get_suppress_details");

        ClassDB::bind_method(D_METHOD("get_passed_checks"), &TestCase::get_passed_checks);
        ADD_PROPERTY(PropertyInfo(Variant::INT, "passed_checks", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY | PROPERTY_USAGE_EDITOR), "", "get_passed_checks");

        ClassDB::bind_method(D_METHOD("get_failed_checks"), &TestCase::get_failed_checks);
        ADD_PROPERTY(PropertyInfo(Variant::INT, "failed_checks", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY | PROPERTY_USAGE_EDITOR), "", "get_failed_checks");

        ClassDB::bind_method(D_METHOD("get_errored_checks"), &TestCase::get_errored_checks);
        ADD_PROPERTY(PropertyInfo(Variant::INT, "errored_checks", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY | PROPERTY_USAGE_EDITOR), "", "get_errored_checks");
    }
}