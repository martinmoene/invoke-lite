// Copyright 2021-2021 by Martin Moene
//
// https://github.com/martinmoene/invoke-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "invoke-main.t.hpp"

// C++ language version (represent 98 as 3):

#define invoke_CPLUSPLUS_V  ( invoke_CPLUSPLUS / 100 - (invoke_CPLUSPLUS > 200000 ? 2000 : 1994) )

// Compiler versions:

// MSVC++  6.0  _MSC_VER == 1200  invoke_COMPILER_MSVC_VERSION ==  60  (Visual Studio 6.0)
// MSVC++  7.0  _MSC_VER == 1300  invoke_COMPILER_MSVC_VERSION ==  70  (Visual Studio .NET 2002)
// MSVC++  7.1  _MSC_VER == 1310  invoke_COMPILER_MSVC_VERSION ==  71  (Visual Studio .NET 2003)
// MSVC++  8.0  _MSC_VER == 1400  invoke_COMPILER_MSVC_VERSION ==  80  (Visual Studio 2005)
// MSVC++  9.0  _MSC_VER == 1500  invoke_COMPILER_MSVC_VERSION ==  90  (Visual Studio 2008)
// MSVC++ 10.0  _MSC_VER == 1600  invoke_COMPILER_MSVC_VERSION == 100  (Visual Studio 2010)
// MSVC++ 11.0  _MSC_VER == 1700  invoke_COMPILER_MSVC_VERSION == 110  (Visual Studio 2012)
// MSVC++ 12.0  _MSC_VER == 1800  invoke_COMPILER_MSVC_VERSION == 120  (Visual Studio 2013)
// MSVC++ 14.0  _MSC_VER == 1900  invoke_COMPILER_MSVC_VERSION == 140  (Visual Studio 2015)
// MSVC++ 14.1  _MSC_VER >= 1910  invoke_COMPILER_MSVC_VERSION == 141  (Visual Studio 2017)
// MSVC++ 14.2  _MSC_VER >= 1920  invoke_COMPILER_MSVC_VERSION == 142  (Visual Studio 2019)

#if 0

#if defined(_MSC_VER ) && !defined(__clang__)
# define invoke_COMPILER_MSVC_VER           (_MSC_VER )
# define invoke_COMPILER_MSVC_VERSION       (_MSC_VER / 10 - 10 * ( 5 + (_MSC_VER < 1900 ) ) )
# define invoke_COMPILER_MSVC_VERSION_FULL  (_MSC_VER - 100 * ( 5 + (_MSC_VER < 1900 ) ) )
#else
# define invoke_COMPILER_MSVC_VER           0
# define invoke_COMPILER_MSVC_VERSION       0
# define invoke_COMPILER_MSVC_VERSION_FULL  0
#endif

#define invoke_COMPILER_VERSION( major, minor, patch ) ( 10 * ( 10 * (major) + (minor) ) + (patch) )

#if defined( __apple_build_version__ )
# define invoke_COMPILER_APPLECLANG_VERSION invoke_COMPILER_VERSION( __clang_major__, __clang_minor__, __clang_patchlevel__ )
# define invoke_COMPILER_CLANG_VERSION 0
#elif defined( __clang__ )
# define invoke_COMPILER_APPLECLANG_VERSION 0
# define invoke_COMPILER_CLANG_VERSION invoke_COMPILER_VERSION( __clang_major__, __clang_minor__, __clang_patchlevel__ )
#else
# define invoke_COMPILER_APPLECLANG_VERSION 0
# define invoke_COMPILER_CLANG_VERSION 0
#endif

#if defined(__GNUC__) && !defined(__clang__)
# define invoke_COMPILER_GNUC_VERSION invoke_COMPILER_VERSION( __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__ )
#else
# define invoke_COMPILER_GNUC_VERSION 0
#endif

#endif

#define invoke_PRESENT( x ) \
    std::cout << #x << ": " << x << "\n"

#define invoke_ABSENT( x ) \
    std::cout << #x << ": (undefined)\n"

lest::tests & specification()
{
    static lest::tests tests;
    return tests;
}

CASE( "invoke-lite version" "[.invoke][.version]" )
{
    invoke_PRESENT( invoke_lite_MAJOR   );
    invoke_PRESENT( invoke_lite_MINOR   );
    invoke_PRESENT( invoke_lite_PATCH   );
    invoke_PRESENT( invoke_lite_VERSION );

    // invoke_PRESENT( invoke_CPP98_FALLBACK );
}

CASE( "invoke-lite configuration" "[.invoke][.config]" )
{
#if invoke_USES_STD_INVOKE
    std::cout << "(Configuration not available: using std::string_view)\n";
#else
    invoke_PRESENT( invoke_CPLUSPLUS );
    invoke_PRESENT( invoke_CPLUSPLUS_V );
#endif
}

CASE( "__cplusplus" "[.stdc++]" )
{
    invoke_PRESENT( __cplusplus );

#if _MSVC_LANG
    invoke_PRESENT( _MSVC_LANG );
#else
    invoke_ABSENT(  _MSVC_LANG );
#endif
}

CASE( "Compiler version" "[.compiler]" )
{
#if invoke_USES_STD_INVOKE
    std::cout << "(Compiler version not available: using std::string_view)\n";
#else
    // invoke_PRESENT( invoke_COMPILER_APPLECLANG_VERSION );
    invoke_PRESENT( invoke_COMPILER_CLANG_VERSION );
    invoke_PRESENT( invoke_COMPILER_GNUC_VERSION );
    invoke_PRESENT( invoke_COMPILER_MSVC_VERSION );
    // invoke_PRESENT( invoke_COMPILER_MSVC_VERSION_FULL );
#endif
}

CASE( "presence of C++ language features" "[.stdlanguage]" )
{
#if invoke_USES_STD_INVOKE
    std::cout << "(C++ language features not available: using std::string_view)\n";
#else
    invoke_PRESENT( invoke_HAVE_CONSTEXPR_11 );
    // invoke_PRESENT( invoke_HAVE_DEFAULT_FN_TPL_ARGS );
    // invoke_PRESENT( invoke_HAVE_EXPLICIT_CONVERSION );
    invoke_PRESENT( invoke_HAVE_NODISCARD );
    invoke_PRESENT( invoke_HAVE_NOEXCEPT );
#endif
}

CASE( "presence of C++ library features" "[.stdlibrary]" )
{
#if defined(_HAS_CPP0X) && _HAS_CPP0X
    invoke_PRESENT( _HAS_CPP0X );
#else
    invoke_ABSENT(  _HAS_CPP0X );
#endif
}

int main( int argc, char * argv[] )
{
    return lest::run( specification(), argc, argv );
}

#if 0
g++            -I../include -o string-main.t.exe string-main.t.cpp && string-main.t.exe --pass
g++ -std=c++98 -I../include -o string-main.t.exe string-main.t.cpp && string-main.t.exe --pass
g++ -std=c++03 -I../include -o string-main.t.exe string-main.t.cpp && string-main.t.exe --pass
g++ -std=c++0x -I../include -o string-main.t.exe string-main.t.cpp && string-main.t.exe --pass
g++ -std=c++11 -I../include -o string-main.t.exe string-main.t.cpp && string-main.t.exe --pass
g++ -std=c++14 -I../include -o string-main.t.exe string-main.t.cpp && string-main.t.exe --pass
g++ -std=c++17 -I../include -o string-main.t.exe string-main.t.cpp && string-main.t.exe --pass

cl -EHsc -I../include string-main.t.cpp && string-main.t.exe --pass
#endif

// end of file
