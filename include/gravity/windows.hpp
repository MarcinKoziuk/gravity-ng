/*
* windows.hpp
*
* Include Windows.h in a sane way
*
* Copyright (c) 2014-2016
* Marcin Koziuk <marcin.koziuk@gmail.com>
*/

#ifndef GRAVITY_WINDOWS_HPP
#define GRAVITY_WINDOWS_HPP

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN 1
	#define VC_EXTRALEAN 1
	#define NOMINMAX 1
	#define STRICT 1
	#include <Windows.h>
	#undef RGB
#else
	#error "Not Windows. Why include windows.hpp?"
#endif

#endif /* GRAVITY_WINDOWS_HPP */
