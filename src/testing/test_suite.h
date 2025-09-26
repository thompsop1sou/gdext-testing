#ifndef TEST_SUITE_H
#define TEST_SUITE_H

#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/classes/node.hpp>

#include <vector>
#include <utility>

#include "testing/test_case.h"
#include "testing/test_macros.h"

namespace godot::Testing
{
    class TestSuite : public Node
    {
        GDCLASS(TestSuite, Node)

    public:
        void run_all_test_cases();

        Callable get_all_test_cases_runner();

        bool get_suppress_details() const;
        void set_suppress_details(bool suppress_details);

        std::int64_t get_passed_test_cases() const;
        std::int64_t get_mixed_test_cases() const;
        std::int64_t get_failed_test_cases() const;
        std::int64_t get_errored_test_cases() const;

    protected:
        static void _bind_methods();

        std::vector<std::pair<String, Ref<TestCase>>> m_all_test_cases;

    private:
        Callable m_all_test_cases_runner = Callable(this, "run_all_test_cases");

        bool m_suppress_details = true;

        std::int64_t m_passed_test_cases = 0;
        std::int64_t m_mixed_test_cases = 0;
        std::int64_t m_failed_test_cases = 0;
        std::int64_t m_errored_test_cases = 0;

        const String LINE = String("=").repeat(80);
    };

    template <class T>
    String to_string(const T& t)
    {
        return UtilityFunctions::str(t);
    }

    template <class T>
    bool approx_equal(T a, T b)
    {
        return a.is_equal_approx(b);
    }
    template <>
    inline bool approx_equal(double a, double b)
    {
        return Math::is_equal_approx(a, b);
    }
    template <>
    inline bool approx_equal(float a, float b)
    {
        return Math::is_equal_approx(a, b);
    }
}

#endif
