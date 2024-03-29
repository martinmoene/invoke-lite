// Copyright 2021-2021 by Martin Moene
//
// https://github.com/martinmoene/invoke-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef TEST_INVOKE_LITE_H_INCLUDED
#define TEST_INVOKE_LITE_H_INCLUDED

#include invoke_INVOKE_HEADER

// Provide macros for compilation with std::invoke(), std::apply():

#ifndef invoke_COMPILER_MSVC_VER
# if defined(_MSC_VER ) && !defined(__clang__)
#  define invoke_COMPILER_MSVC_VER      (_MSC_VER )
# else
#  define invoke_COMPILER_MSVC_VER      0
# endif
#endif

// half-open range [lo..hi):
#ifndef  invoke_BETWEEN
# define invoke_BETWEEN( v, lo, hi ) ( (lo) <= (v) && (v) < (hi) )
#endif

#ifndef  invoke_noexcept_op
# define invoke_noexcept_op(expr) noexcept(expr)
#endif

// Compiler warning suppression for usage of lest:

#ifdef __clang__
# pragma clang diagnostic ignored "-Wstring-conversion"
# pragma clang diagnostic ignored "-Wunused-parameter"
# pragma clang diagnostic ignored "-Wunused-template"
# pragma clang diagnostic ignored "-Wunused-function"
# pragma clang diagnostic ignored "-Wunused-member-function"
#elif defined __GNUC__
# pragma GCC   diagnostic ignored "-Wunused-parameter"
# pragma GCC   diagnostic ignored "-Wunused-function"
#endif

// Limit C++ Core Guidelines checking to GSL Lite:

#if _MSC_VER >= 1910
# include <CppCoreCheck/Warnings.h>
# pragma warning(disable: ALL_CPPCORECHECK_WARNINGS)
#endif

#include <iostream>

namespace nonstd {

// use oparator<< instead of to_string() overload;
// see  http://stackoverflow.com/a/10651752/437272

// inline std::ostream & operator<<( std::ostream & os, xxx const & )
// {
// }

} // namespace nonstd

namespace lest {

// using ::nonstd::operator<<;
// using ::nonstd::operator==;

} // namespace lest

namespace std {

// using ::nonstd::operator<<;
// using ::nonstd::string::operator<<;

}

#if 1
# define   lest_FEATURE_AUTO_REGISTER 1
# include "lest.hpp"
#else
# include "lest_cpp03.hpp"  // Fails with `EXPECT( nonstd::invoke( add, 1, 2 ) == 3 );`
#endif

extern lest::tests & specification();

#define CASE( name ) lest_CASE( specification(), name )

#endif // TEST_INVOKE_LITE_H_INCLUDED

// end of file
