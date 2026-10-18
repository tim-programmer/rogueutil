#include <spdlog/spdlog.h>
#include <cpptrace/cpptrace.hpp>

template <typename... Args>
[[noreturn]] static inline void log_throw(fmt::format_string<Args...> fmt, Args &&...args)
{
    auto formatted_msg = fmt::format(fmt, std::forward<Args>(args)...);
    spdlog::critical(formatted_msg);
    cpptrace::generate_trace().print();
    throw std::runtime_error(formatted_msg);
}
