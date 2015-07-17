/*
 * logging.hpp
 *
 * Defines the base macros for logging.
 * TODO: Eventually replace it with a non-trivial logger
 *
 * Copyright (c) 2014
 * Marcin Koziuk <marcin.koziuk@gmail.com>
 */

#ifndef GRAVITY_LOGGING_HPP
#define GRAVITY_LOGGING_HPP

//#include <boost/log/trivial.hpp>
#include <iostream>

/*
 * Logs a message.
 * Usage: LOG(<severity>) << "<text to output>";
 */
#define LOG(severity) std::cout << #severity << ": "

#endif /* GRAVITY_LOGGING_HPP */
