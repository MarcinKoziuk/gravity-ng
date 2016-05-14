/*
 * game/logging.hpp
 *
 * Defines the base macros for logging.
 * TODO: Replace it with liblogog?
 *
 * Copyright (c) 2014, 2015
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_LIB_LOGGING_HPP
#define GRAVITY_LIB_LOGGING_HPP

#include <cstddef>
#include <iostream>
#include <sstream>

namespace Gravity {
namespace Log {

enum Severity {
    trace,
    debug,
    info,
    notice,
    warning,
    error,
    critical,
    fatal
};

/*
 * Prevents unused variable warning, while keeping both enum and string
 * definitions together.
 */
#ifdef GRAVITY_LOGGING_IMPL
static const char* SEVERITY_TEXT_REPR[] = {
    "TRACE",
    "DEBUG",
    "INFO",
    "NOTICE",
    "WARNING",
    "ERROR",
    "CRITICAL",
    "FATAL"
};
#endif

class SimpleLogStream {
private:
    Severity severity;
    std::string file;
    std::string func;
    int line;
    std::stringstream ss;

public:
    SimpleLogStream(Severity severity, const char* file, const char* func, int line);

    ~SimpleLogStream();

    bool IsWarningOrHigher() const;

    std::ostream& PhysicalStream() const;

    template<class T>
    SimpleLogStream& operator<<(const T &x)
    {
        ss << x;
        return *this;
    }
};

} // namespace Log
} // namespace Gravity

/*
 * Logs a message.
 * Usage: LOG(<severity>) << "<text to output>";
 */
#define LOG(severity) Gravity::Log::SimpleLogStream(Gravity::Log::severity, __FILE__, __func__, __LINE__)

#endif /* GRAVITY_LIB_LOGGING_HPP */
