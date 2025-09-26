#ifndef TESTING_MACROS_H
#define TESTING_MACROS_H

#include <algorithm>

#include "testing/error_handler.h"

// Macros defining commonly used strings

#define ERRORED_START "[color=#FF80FF][b]"
#define ERRORED_END "[/b][/color]"
#define PASSED_START "[color=#80FF80][b]"
#define PASSED_END "[/b][/color]"
#define FAILED_START "[color=#FF8080][b]"
#define FAILED_END "[/b][/color]"
#define MIXED_START "[color=#FFFF80][b]"
#define MIXED_END "[/b][/color]"
#define TEST_NAME_START "[color=#8080FF][b]"
#define TEST_NAME_END "[/b][/color]"

// Macros for setting up test cases within a test suite

#define DECLARE_TEST_CASE(TestCaseName, test_case_name) \
    public: \
        class TestCaseName : public godot::Testing::TestCase \
        { \
            GDCLASS(TestCaseName, godot::Testing::TestCase) \
        public: \
            using godot::Testing::TestCase::TestCase; \
        protected: \
            virtual void test_body() override; \
            static void _bind_methods() {} \
        }; \
        Ref<TestCaseName> get_ ## test_case_name() const; \
        void set_ ## test_case_name(const Ref<TestCaseName>& test_case_name); \
    private: \
        Ref<TestCaseName> m_ ## test_case_name; \
    public:

#define REGISTER_TEST_CASE(TestCaseName, test_case_name) \
    m_all_test_cases.push_back({#test_case_name, m_ ## test_case_name});

#define DEFINE_TEST_CASE(TestSuiteName, TestCaseName, test_case_name) \
    Ref<TestSuiteName::TestCaseName> TestSuiteName::get_ ## test_case_name() const \
    { \
        return m_ ## test_case_name; \
    } \
    void TestSuiteName::set_ ## test_case_name(const Ref<TestCaseName>& test_case_name) \
    { \
        m_ ## test_case_name = test_case_name; \
        if (m_ ## test_case_name.is_valid()) \
            m_ ## test_case_name->set_parent_test_suite(this); \
        auto iter = std::find_if(m_all_test_cases.begin(), m_all_test_cases.end(), \
            [](const std::pair<String, Ref<godot::Testing::TestCase>>& test_case_pair) {return test_case_pair.first == #test_case_name;}); \
        if (iter != m_all_test_cases.end()) \
            iter->second = m_ ## test_case_name; \
    } \
    void TestSuiteName::TestCaseName::test_body()

#define BIND_TEST_CASE(TestSuiteName, TestCaseName, test_case_name) \
    GDREGISTER_INTERNAL_CLASS(TestSuiteName::TestCaseName); \
    ClassDB::bind_method(D_METHOD(vformat("get_%s", #test_case_name)), &TestSuiteName::get_ ## test_case_name); \
    ClassDB::bind_method(D_METHOD(vformat("set_%s", #test_case_name), #test_case_name), &TestSuiteName::set_ ## test_case_name); \
    ADD_PROPERTY( \
        PropertyInfo( \
            Variant::OBJECT, \
            #test_case_name, \
            PROPERTY_HINT_RESOURCE_TYPE, \
            #TestCaseName, \
            PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_EDITOR_INSTANTIATE_OBJECT \
        ), \
        vformat("set_%s", #test_case_name), \
        vformat("get_%s", #test_case_name) \
    );

// Macros defining common return values for checks

#define CHECK_PASSED 1
#define CHECK_FAILED 0
#define CHECK_ERRORED -1

// Macros for manually passing a check

#define _PRINT_PASSED(check_string) \
    UtilityFunctions::print_rich(vformat("    %s[PASSED]%s %s", PASSED_START, PASSED_END, check_string)); \

#define _PRINT_FAILED(check_string) \
    UtilityFunctions::print_rich(vformat("    %s[FAILED]%s %s", FAILED_START, FAILED_END, check_string)); \

#define _PRINT_ERRORED(check_string) \
    UtilityFunctions::print_rich(vformat("    %s[ERRORED]%s %s", ERRORED_START, ERRORED_END, check_string)); \

#define _CHECK_PASS_FAIL_CUSTOM(COUNT, MESSAGE, PASS_OR_FAIL, passed_or_failed_checks, check_result) \
    [&]() -> int \
    { \
        _PRINT_ ## PASS_OR_FAIL ## ED(vformat("CHECK_%s_CUSTOM(COUNT, MESSAGE)", #PASS_OR_FAIL)); \
        if (!m_suppress_details) \
        { \
            UtilityFunctions::print_rich(vformat("        COUNT: %s", COUNT)); \
            UtilityFunctions::print_rich(vformat("        MESSAGE: %s", MESSAGE)); \
            UtilityFunctions::print_rich(""); \
        } \
        passed_or_failed_checks += COUNT; \
        return check_result; \
    }()

#define CHECK_PASS_CUSTOM(COUNT, MESSAGE) _CHECK_PASS_FAIL_CUSTOM(COUNT, MESSAGE, PASS, m_passed_checks, CHECK_PASSED)

#define CHECK_FAIL_CUSTOM(COUNT, MESSAGE) _CHECK_PASS_FAIL_CUSTOM(COUNT, MESSAGE, FAIL, m_failed_checks, CHECK_FAILED)

#define _CHECK_PASS_FAIL(PASS_OR_FAIL, passed_or_failed_checks, check_result) \
    [&]() -> int \
    { \
        UtilityFunctions::print_rich(vformat("    %s[%sED]%s CHECK_%s()", PASS_OR_FAIL ## ED_START, #PASS_OR_FAIL, PASS_OR_FAIL ## ED_END, #PASS_OR_FAIL)); \
        if (!m_suppress_details) \
            UtilityFunctions::print_rich(""); \
        passed_or_failed_checks++; \
        return check_result; \
    }()

#define CHECK_PASS() _CHECK_PASS_FAIL(PASS, m_passed_checks, CHECK_PASSED)

#define CHECK_FAIL() _CHECK_PASS_FAIL(FAIL, m_failed_checks, CHECK_FAILED)

// Macros for checking for the existence (or non-existence) of errors

#define _PRINT_ERRORED_DETAILS(error_count, get_last_n_function) \
    [&]() \
    { \
        PackedStringArray error_messages = ErrorHandler::get_last_n_function(error_count); \
        if (error_messages.size() == 1) \
        { \
            UtilityFunctions::print_rich(vformat("        error: %s", error_messages[0])); \
        } \
        else if (error_messages.size() > 1) \
        { \
            UtilityFunctions::print_rich("        errors:"); \
            for (std::int64_t i = 0; i < error_messages.size(); ++i) \
                UtilityFunctions::print_rich(vformat("            %s", error_messages[i])); \
        } \
    }()

#define _CHECK_ERRORS(EXPR, count_comparison_operator, count_comparison_value, check_string, ERROR_MACRO_SUFFIX, get_last_n_function) \
    [&]() -> int \
    { \
        std::int64_t count = 0; \
        SUPPRESS_AND_COUNT_ ## ERROR_MACRO_SUFFIX(EXPR, count); \
        int check_result; \
        if (count count_comparison_operator count_comparison_value) \
        { \
            _PRINT_PASSED(check_string); \
            if (!m_suppress_details) \
            { \
                UtilityFunctions::print_rich(vformat("        EXPR code: %s", #EXPR)); \
                _PRINT_ERRORED_DETAILS(count, get_last_n_function); \
            } \
            m_passed_checks++; \
            check_result = CHECK_PASSED; \
        } \
        else \
        { \
            _PRINT_FAILED(check_string); \
            if (!m_suppress_details) \
            { \
                UtilityFunctions::print_rich(vformat("        EXPR code: %s", #EXPR)); \
                _PRINT_ERRORED_DETAILS(count, get_last_n_function); \
            } \
            m_failed_checks++; \
            check_result = CHECK_FAILED; \
        } \
        if (!m_suppress_details) \
            UtilityFunctions::print_rich(""); \
        return check_result; \
    }()

#define CHECK_N_ERRORS(EXPR, N) _CHECK_ERRORS(EXPR, ==, N, "CHECK_N_ERRORS(EXPR, N)", ALL_ERRORS, get_last_n_errors)

#define CHECK_N_PRINTERRS(EXPR, N) _CHECK_ERRORS(EXPR, ==, N, "CHECK_N_PRINTERRS(EXPR, N)", PRINTERR, get_last_n_printerrs)

#define CHECK_N_PUSH_ERRORS(EXPR, N) _CHECK_ERRORS(EXPR, ==, N, "CHECK_N_PUSH_ERRORS(EXPR, N)", PUSH_ERROR, get_last_n_push_errors)

#define CHECK_N_PUSH_WARNINGS(EXPR, N) _CHECK_ERRORS(EXPR, ==, N, "CHECK_N_PUSH_WARNINGS(EXPR, N)", PUSH_WARNING, get_last_n_push_warnings)

#define CHECK_SOME_ERRORS(EXPR) _CHECK_ERRORS(EXPR, >, 0, "CHECK_SOME_ERRORS(EXPR)", ALL_ERRORS, get_last_n_errors)

#define CHECK_SOME_PRINTERRS(EXPR) _CHECK_ERRORS(EXPR, >, 0, "CHECK_SOME_PRINTERRS(EXPR)", PRINTERR, get_last_n_printerrs)

#define CHECK_SOME_PUSH_ERRORS(EXPR) _CHECK_ERRORS(EXPR, >, 0, "CHECK_SOME_PUSH_ERRORS(EXPR)", PUSH_ERROR, get_last_n_push_errors)

#define CHECK_SOME_PUSH_WARNINGS(EXPR) _CHECK_ERRORS(EXPR, >, 0, "CHECK_SOME_PUSH_WARNINGS(EXPR)", PUSH_WARNING, get_last_n_push_warnings)

#define CHECK_NO_ERRORS(EXPR) _CHECK_ERRORS(EXPR, <=, 0, "CHECK_NO_ERRORS(EXPR)", ALL_ERRORS, get_last_n_errors)

#define CHECK_NO_PRINTERRS(EXPR) _CHECK_ERRORS(EXPR, <=, 0, "CHECK_NO_PRINTERRS(EXPR)", PRINTERR, get_last_n_printerrs)

#define CHECK_NO_PUSH_ERRORS(EXPR) _CHECK_ERRORS(EXPR, <=, 0, "CHECK_NO_PUSH_ERRORS(EXPR)", PUSH_ERROR, get_last_n_push_errors)

#define CHECK_NO_PUSH_WARNINGS(EXPR) _CHECK_ERRORS(EXPR, <=, 0, "CHECK_NO_PUSH_WARNINGS(EXPR)", PUSH_WARNING, get_last_n_push_warnings)

// Macros for checking if an expression is true or false

#define _PRINT_CODE_AND_VALUE(EXPR, expr_string, expr_value) \
    [&]() \
    { \
        UtilityFunctions::print_rich(vformat("        %s code: %s", expr_string, #EXPR)); \
        UtilityFunctions::print_rich(vformat("        %s value: %s", expr_string, godot::Testing::to_string(expr_value))); \
    }()

#define _CHECK_BOOL(EXPR, not_operator, TRUE_OR_FALSE) \
    [&]() -> int \
    { \
        String check_string = vformat("CHECK_%s(EXPR)", #TRUE_OR_FALSE); \
        auto suppress_all = ErrorHandler::get_suppress_all(); \
        ErrorHandler::set_suppress_all({true, true, true}); \
        std::int64_t previous_error_count = ErrorHandler::get_total_count(); \
        auto expr_value = EXPR; \
        std::int64_t error_count = ErrorHandler::get_total_count() - previous_error_count; \
        ErrorHandler::set_suppress_all(suppress_all); \
        int check_result; \
        if (error_count > 0) \
        { \
            _PRINT_ERRORED(check_string); \
            if (!m_suppress_details) \
            { \
                _PRINT_CODE_AND_VALUE(EXPR, "EXPR", expr_value); \
                _PRINT_ERRORED_DETAILS(error_count, get_last_n_errors); \
            } \
            m_errored_checks++; \
            check_result = CHECK_ERRORED; \
        } \
        else if (not_operator(expr_value)) \
        { \
            _PRINT_PASSED(check_string); \
            if (!m_suppress_details) \
                _PRINT_CODE_AND_VALUE(EXPR, "EXPR", expr_value); \
            m_passed_checks++; \
            check_result = CHECK_PASSED; \
        } \
        else \
        { \
            _PRINT_FAILED(check_string); \
            if (!m_suppress_details) \
                _PRINT_CODE_AND_VALUE(EXPR, "EXPR", expr_value); \
            m_failed_checks++; \
            check_result = CHECK_FAILED; \
        } \
        if (!m_suppress_details) \
            UtilityFunctions::print_rich(""); \
        return check_result; \
    }()

#define CHECK_TRUE(a) _CHECK_BOOL(a, , TRUE)

#define CHECK_FALSE(a) _CHECK_BOOL(a, !, FALSE)

// Macros for comparing two values, A and B, using an operator, ==, !=, <, <=, >, or >=

#define _EQUAL(value_a, value_b) value_a == value_b
#define _NOT_EQUAL(value_a, value_b) value_a != value_b
#define _LESS(value_a, value_b) value_a < value_b
#define _LESS_OR_EQUAL(value_a, value_b) value_a <= value_b
#define _GREATER(value_a, value_b) value_a > value_b
#define _GREATER_OR_EQUAL(value_a, value_b) value_a >= value_b
#define _APPROX_EQUAL(value_a, value_b) godot::Testing::approx_equal(value_a, value_b)

#define _CHECK_COMPARE(EXPR_A, EXPR_B, COMPARE) \
    [&]() -> int \
    { \
        String check_string = vformat("CHECK_%s(EXPR_A, EXPR_B)", #COMPARE); \
        auto suppress_all = ErrorHandler::get_suppress_all(); \
        ErrorHandler::set_suppress_all({true, true, true}); \
        std::int64_t previous_error_count = ErrorHandler::get_total_count(); \
        auto expr_a_value = EXPR_A; \
        auto expr_b_value = EXPR_B; \
        std::int64_t error_count = ErrorHandler::get_total_count() - previous_error_count; \
        ErrorHandler::set_suppress_all(suppress_all); \
        int check_result; \
        if (error_count > 0) \
        { \
            _PRINT_ERRORED(check_string); \
            if (!m_suppress_details) \
            { \
                _PRINT_CODE_AND_VALUE(EXPR_A, "EXPR_A", expr_a_value); \
                _PRINT_CODE_AND_VALUE(EXPR_B, "EXPR_B", expr_b_value); \
                _PRINT_ERRORED_DETAILS(error_count, get_last_n_errors); \
            } \
            m_errored_checks++; \
            check_result = CHECK_ERRORED; \
        } \
        else if (_ ## COMPARE(expr_a_value, expr_b_value)) \
        { \
            _PRINT_PASSED(check_string); \
            if (!m_suppress_details) \
            { \
                _PRINT_CODE_AND_VALUE(EXPR_A, "EXPR_A", expr_a_value); \
                _PRINT_CODE_AND_VALUE(EXPR_B, "EXPR_B", expr_b_value); \
            } \
            m_passed_checks++; \
            check_result = CHECK_PASSED; \
        } \
        else \
        { \
            _PRINT_FAILED(check_string); \
            if (!m_suppress_details) \
            { \
                _PRINT_CODE_AND_VALUE(EXPR_A, "EXPR_A", expr_a_value); \
                _PRINT_CODE_AND_VALUE(EXPR_B, "EXPR_B", expr_b_value); \
            } \
            m_failed_checks++; \
            check_result = CHECK_ERRORED; \
        } \
        if (!m_suppress_details) \
            UtilityFunctions::print_rich(""); \
        return check_result; \
    }()

#define CHECK_EQUAL(EXPR_A, EXPR_B) _CHECK_COMPARE(EXPR_A, EXPR_B, EQUAL)

#define CHECK_NOT_EQUAL(EXPR_A, EXPR_B) _CHECK_COMPARE(EXPR_A, EXPR_B, NOT_EQUAL)

#define CHECK_LESS(EXPR_A, EXPR_B) _CHECK_COMPARE(EXPR_A, EXPR_B, LESS)

#define CHECK_LESS_OR_EQUAL(EXPR_A, EXPR_B) _CHECK_COMPARE(EXPR_A, EXPR_B, LESS_OR_EQUAL)

#define CHECK_GREATER(EXPR_A, EXPR_B) _CHECK_COMPARE(EXPR_A, EXPR_B, GREATER)

#define CHECK_GREATER_OR_EQUAL(EXPR_A, EXPR_B) _CHECK_COMPARE(EXPR_A, EXPR_B, GREATER_OR_EQUAL)

#define CHECK_APPROX_EQUAL(EXPR_A, EXPR_B) _CHECK_COMPARE(EXPR_A, EXPR_B, APPROX_EQUAL)

#endif
