/*
 * game/logging.cpp
 *
 * Copyright (c) 2014, 2015
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#include <cstddef>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ctime>

#define GRAVITY_LOGGING_IMPL
#include <gravity/game/logging.hpp>

namespace Gravity {
namespace Log {


static std::string FormatFileString(std::string file)
{
    std::string source_path = GRAVITY_COMPILETIME_SOURCE_PATH;

#ifdef _WIN32
    // temp fix for cmake's source dir representation
    std::replace(source_path.begin(), source_path.end(), '/', '\\');
#endif

    std::size_t index = file.find(source_path);
    if (index != std::string::npos) {
        return file.substr(index + source_path.length() + 1, file.length());
    } else {
        return file;
    }
}

static std::string GetDateString()
{
    std::time_t rawtime;
    std::tm* timeinfo;
    char buffer[80];

    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);

    std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(buffer);
}

SimpleLogStream::SimpleLogStream(Severity severity, const char* file, const char* func, int line)
    : severity(severity)
    , file(file)
    , func(func)
    , line(line)
{
    ss << GetDateString() << ' ' << SEVERITY_TEXT_REPR[severity] << " [" << FormatFileString(file) << ":" << line << "]: ";
}

SimpleLogStream::~SimpleLogStream()
{
    PhysicalStream() << ss.str() << "\n" << std::flush;
}

bool SimpleLogStream::IsWarningOrHigher() const
{
    return severity >= warning;
}

std::ostream& SimpleLogStream::PhysicalStream() const
{
    return IsWarningOrHigher() ? std::cerr : std::cout;
}

} // namespace Log
} // namespace Gravity

