#ifndef TEST_CASE_H
#define TEST_CASE_H

#include "godot_cpp/classes/resource.hpp"

#include "testing/test_macros.h"

namespace godot::Testing
{
    class TestSuite;

    class TestCase : public Resource
    {
        GDCLASS(TestCase, Resource)

    public:
        explicit TestCase(TestSuite* parent_test_suite = nullptr);

        TestSuite* get_parent_test_suite() const;
        void set_parent_test_suite(TestSuite* parent_test_suite);

        void run();

        Callable get_runner();

        bool get_suppress_details() const;
        void set_suppress_details(bool suppress_details);

        std::int64_t get_passed_checks() const;
        std::int64_t get_failed_checks() const;
        std::int64_t get_errored_checks() const;

    protected:
        static void _bind_methods();

        virtual void test_body() = 0;

        TestSuite* m_parent_test_suite = nullptr;

        bool m_suppress_details = false;

        std::int64_t m_passed_checks = 0;
        std::int64_t m_failed_checks = 0;
        std::int64_t m_errored_checks = 0;

        Callable m_runner = Callable(this, "run");

        const String LINE = String("-").repeat(80);
    };
}

#endif
