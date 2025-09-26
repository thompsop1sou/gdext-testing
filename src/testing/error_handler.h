#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>

#include <vector>
#include <tuple>
#include <functional>

namespace godot
{
    class ErrorHandler
    {
    public:
        template <typename... Args>
        static void printerr(const Variant &p_arg1, const Args &...p_args)
        {
            m_printerr_message_indexes.push_back(m_messages.size());
            m_messages.push_back(UtilityFunctions::str(p_arg1, p_args...));

            if (!suppress_printerr)
                UtilityFunctions::printerr(p_arg1, p_args...);
        }

        template <typename... Args>
        static void push_error(const Variant &p_arg1, const Args &...p_args)
        {
            m_push_error_message_indexes.push_back(m_messages.size());
            m_messages.push_back(UtilityFunctions::str(p_arg1, p_args...));

            if (!suppress_push_error)
                UtilityFunctions::push_error(p_arg1, p_args...);
        }

        template <typename... Args>
        static void push_warning(const Variant &p_arg1, const Args &...p_args)
        {
            m_push_warning_message_indexes.push_back(m_messages.size());
            m_messages.push_back(UtilityFunctions::str(p_arg1, p_args...));

            if (!suppress_push_warning)
                UtilityFunctions::push_warning(p_arg1, p_args...);
        }

        static int get_total_count()
        {
            return m_messages.size();
        }

        static int get_printerr_count()
        {
            return m_printerr_message_indexes.size();
        }

        static int get_push_error_count()
        {
            return m_push_error_message_indexes.size();
        }

        static int get_push_warning_count()
        {
            return m_push_warning_message_indexes.size();
        }

        static void set_suppress_all(std::tuple<bool, bool, bool> suppress)
        {
            suppress_printerr = std::get<0>(suppress);
            suppress_push_error = std::get<1>(suppress);
            suppress_push_warning = std::get<2>(suppress);
        }

        static std::tuple<bool, bool, bool> get_suppress_all()
        {
            return {suppress_printerr, suppress_push_error, suppress_push_warning};
        }

        static PackedStringArray get_last_n_errors(std::int64_t n)
        {
            return get_last_n_messages_helper(n, m_messages.size(),
                [&](std::int64_t i) -> String {return m_messages[i];});
        }

        static PackedStringArray get_last_n_printerrs(std::int64_t n)
        {
            return get_last_n_messages_helper(n, m_printerr_message_indexes.size(),
                [&](std::int64_t i) -> String {return m_messages[m_printerr_message_indexes[i]];});
        }

        static PackedStringArray get_last_n_push_errors(std::int64_t n)
        {
            return get_last_n_messages_helper(n, m_push_error_message_indexes.size(),
                [&](std::int64_t i) -> String {return m_messages[m_push_error_message_indexes[i]];});
        }

        static PackedStringArray get_last_n_push_warnings(std::int64_t n)
        {
            return get_last_n_messages_helper(n, m_push_warning_message_indexes.size(),
                [&](std::int64_t i) -> String {return m_messages[m_push_warning_message_indexes[i]];});
        }

        static inline bool suppress_printerr = false;
        static inline bool suppress_push_error = false;
        static inline bool suppress_push_warning = false;

    private:
        static PackedStringArray get_last_n_messages_helper(std::int64_t n, std::int64_t message_count, std::function<String (std::int64_t i)> get_message)
        {
            PackedStringArray last_n_messages;
            std::int64_t messages_start_index;
            std::int64_t last_n_messages_size;

            if (message_count < n)
            {
                last_n_messages_size = message_count;
                messages_start_index = 0;
            }
            else
            {
                last_n_messages_size = n;
                messages_start_index = message_count - n;
            }

            last_n_messages.resize(last_n_messages_size);
            for (std::int64_t i = 0; i < last_n_messages_size; ++i)
                last_n_messages[i] = get_message(i + messages_start_index);

            return last_n_messages;
        }

        static inline std::vector<String> m_messages = {};
        static inline std::vector<std::int64_t> m_printerr_message_indexes = {};
        static inline std::vector<std::int64_t> m_push_error_message_indexes = {};
        static inline std::vector<std::int64_t> m_push_warning_message_indexes = {};
    };
}

#define SUPPRESS_ALL_ERRORS(EXPR) \
    { \
        auto suppress_all = ErrorHandler::get_suppress_all(); \
        ErrorHandler::set_suppress_all({true, true, true}); \
        EXPR; \
        ErrorHandler::set_suppress_all(suppress_all); \
    }

#define SUPPRESS_PRINTERR(EXPR) \
    { \
        bool previous_suppress_printerr = ErrorHandler::suppress_printerr; \
        ErrorHandler::suppress_printerr = true; \
        EXPR; \
        ErrorHandler::suppress_printerr = previous_suppress_printerr; \
    }

#define SUPPRESS_PUSH_ERROR(EXPR) \
    { \
        bool previous_suppress_push_error = ErrorHandler::suppress_push_error; \
        ErrorHandler::suppress_push_error = true; \
        EXPR; \
        ErrorHandler::suppress_push_error = previous_suppress_push_error; \
    }

#define SUPPRESS_PUSH_WARNING(EXPR) \
    { \
        bool previous_suppress_push_warning = ErrorHandler::suppress_push_warning; \
        ErrorHandler::suppress_push_warning = true; \
        EXPR; \
        ErrorHandler::suppress_push_warning = previous_suppress_push_warning \
    }

#define COUNT_ALL_ERRORS(EXPR, count) \
    { \
        std::int64_t previous_count = ErrorHandler::get_total_count(); \
        EXPR; \
        count = ErrorHandler::get_total_count() - previous_count; \
    }

#define COUNT_PRINTERR(EXPR, count) \
    { \
        std::int64_t previous_count = ErrorHandler::get_printerr_count(); \
        EXPR; \
        count = ErrorHandler::get_printerr_count() - previous_count; \
    }

#define COUNT_PUSH_ERROR(EXPR, count) \
    { \
        std::int64_t previous_count = ErrorHandler::get_push_error_count(); \
        EXPR; \
        count = ErrorHandler::get_push_error_count() - previous_count; \
    }

#define COUNT_PUSH_WARNING(EXPR, count) \
    { \
        std::int64_t previous_count = ErrorHandler::get_push_warning_count(); \
        EXPR; \
        count = ErrorHandler::get_push_warning_count() - previous_count; \
    }

#define SUPPRESS_AND_COUNT_ALL_ERRORS(EXPR, count) COUNT_ALL_ERRORS(SUPPRESS_ALL_ERRORS(EXPR), count)

#define SUPPRESS_AND_COUNT_PRINTERR(EXPR, count) COUNT_PRINTERR(SUPPRESS_PRINTERR(EXPR), count)

#define SUPPRESS_AND_COUNT_PUSH_ERROR(EXPR, count) COUNT_PUSH_ERROR(SUPPRESS_PUSH_ERROR(EXPR), count)

#define SUPPRESS_AND_COUNT_PUSH_WARNING(EXPR, count) COUNT_PUSH_WARNING(SUPPRESS_PUSH_WARNING(EXPR), count)

#endif
