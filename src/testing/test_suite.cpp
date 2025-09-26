#include "testing/test_suite.h"

#include <godot_cpp/variant/utility_functions.hpp>

namespace godot::Testing
{
    void TestSuite::run_all_test_cases()
    {
        UtilityFunctions::print_rich(LINE);
        UtilityFunctions::print_rich(vformat("\n%s%s%s running...\n", TEST_NAME_START, this->get_class(), TEST_NAME_END));
        UtilityFunctions::print_rich(LINE);

        m_passed_test_cases = 0;
        m_mixed_test_cases = 0;
        m_failed_test_cases = 0;
        m_errored_test_cases = 0;

        for (const auto& [_, test_case] : m_all_test_cases)
        {
            if (!test_case.is_valid())
                continue;

            bool previous_suppress_details = test_case->get_suppress_details();
            test_case->set_suppress_details(m_suppress_details);
            test_case->run();
            test_case->set_suppress_details(previous_suppress_details);

            if (test_case->get_failed_checks() == 0)
                m_passed_test_cases++;
            else if (test_case->get_passed_checks() == 0)
                m_failed_test_cases++;
            else
                m_mixed_test_cases++;

            if (test_case->get_errored_checks() > 0)
                m_errored_test_cases++;
        }

        String test_suite_results;

        if (m_failed_test_cases == 0 && m_mixed_test_cases == 0)
            test_suite_results = vformat("%s[PASSED]%s", PASSED_START, PASSED_END);
        else if (m_passed_test_cases == 0 && m_mixed_test_cases == 0)
            test_suite_results = vformat("%s[FAILED]%s", FAILED_START, FAILED_END);
        else
            test_suite_results = vformat("%s[MIXED]%s", MIXED_START, MIXED_END);

        if (m_errored_test_cases > 0)
            test_suite_results += vformat(" %swith errors%s", ERRORED_START, ERRORED_END);

        UtilityFunctions::print_rich(LINE);
        UtilityFunctions::print_rich(vformat("\n%s%s%s results: %s", TEST_NAME_START, this->get_class(), TEST_NAME_END, test_suite_results));
        UtilityFunctions::print_rich(vformat("    test cases passed: %s%s%s", PASSED_START, m_passed_test_cases, PASSED_END));
        UtilityFunctions::print_rich(vformat("    test cases mixed: %s%s%s", MIXED_START, m_mixed_test_cases, MIXED_END));
        UtilityFunctions::print_rich(vformat("    test cases failed: %s%s%s", FAILED_START, m_failed_test_cases, FAILED_END));
        UtilityFunctions::print_rich(vformat("    test cases with unexpected errors: %s%s%s\n", ERRORED_START, m_errored_test_cases, ERRORED_END));
        UtilityFunctions::print_rich(LINE);
    }

    Callable TestSuite::get_all_test_cases_runner()
    {
        return m_all_test_cases_runner;
    }

    bool TestSuite::get_suppress_details() const
    {
        return m_suppress_details;
    }

    void TestSuite::set_suppress_details(bool suppress_details)
    {
        m_suppress_details = suppress_details;
    }

    std::int64_t TestSuite::get_passed_test_cases() const
    {
        return m_passed_test_cases;
    }

    std::int64_t TestSuite::get_mixed_test_cases() const
    {
        return m_mixed_test_cases;
    }

    std::int64_t TestSuite::get_failed_test_cases() const
    {
        return m_failed_test_cases;
    }

    std::int64_t TestSuite::get_errored_test_cases() const
    {
        return m_errored_test_cases;
    }

    void TestSuite::_bind_methods()
    {
        ClassDB::bind_method(D_METHOD("run_all_test_cases"), &TestSuite::run_all_test_cases);
        ClassDB::bind_method(D_METHOD("get_all_test_cases_runner"), &TestSuite::get_all_test_cases_runner);
        ADD_PROPERTY(PropertyInfo(Variant::CALLABLE, "all_test_cases_runner", PROPERTY_HINT_TOOL_BUTTON, "Run all test cases,Play", PROPERTY_USAGE_EDITOR), "", "get_all_test_cases_runner");

        ClassDB::bind_method(D_METHOD("get_suppress_details"), &TestSuite::get_suppress_details);
        ClassDB::bind_method(D_METHOD("set_suppress_details", "suppress_details"), &TestSuite::set_suppress_details);
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "suppress_details"), "set_suppress_details", "get_suppress_details");

        ClassDB::bind_method(D_METHOD("get_passed_test_cases"), &TestSuite::get_passed_test_cases);
        ADD_PROPERTY(PropertyInfo(Variant::INT, "passed_test_cases", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY | PROPERTY_USAGE_EDITOR), "", "get_passed_test_cases");

        ClassDB::bind_method(D_METHOD("get_mixed_test_cases"), &TestSuite::get_mixed_test_cases);
        ADD_PROPERTY(PropertyInfo(Variant::INT, "mixed_test_cases", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY | PROPERTY_USAGE_EDITOR), "", "get_mixed_test_cases");

        ClassDB::bind_method(D_METHOD("get_failed_test_cases"), &TestSuite::get_failed_test_cases);
        ADD_PROPERTY(PropertyInfo(Variant::INT, "failed_test_cases", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY | PROPERTY_USAGE_EDITOR), "", "get_failed_test_cases");

        ClassDB::bind_method(D_METHOD("get_errored_test_cases"), &TestSuite::get_errored_test_cases);
        ADD_PROPERTY(PropertyInfo(Variant::INT, "errored_test_cases", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY | PROPERTY_USAGE_EDITOR), "", "get_errored_test_cases");
    }
}