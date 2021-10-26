// Copyright 2021-2021 by Martin Moene
//
// https://github.com/martinmoene/invoke-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "invoke-main.t.hpp"
#include <array>    // required with invoke_INVOKE_STD

namespace {

constexpr
int f42()
{
    return 42;
}

constexpr
int f42_nef() invoke_noexcept_op( false )
{
    return 42;
}

constexpr
int arg( int a  )
{
    return a;
}

constexpr
int add( int a, int b )
{
    return a + b;
}

struct holder
{
    int v;

    constexpr
    holder( int v_)
        : v(v_)
    {}

    constexpr
    int val() const
    {
        return v;
    }

    constexpr
    int add( int a ) const
    {
        return v + a;
    }

    constexpr
    int add_constref( int const & a ) const
    {
        return v + a;
    }

    constexpr
    int add_nonconst( int a )
    {
        return v + a;
    }

    void change_arg( int & a ) const
    {
        a = v;
    }
};

struct functor
{
    constexpr
    int operator()() const
    {
        return 42;
    }

    constexpr
    int operator()(int i) const
    {
        return i;
    }
};

//
// invoke():
//

CASE("invoke: a free function, no arguments")
{
    EXPECT( nonstd::invoke( f42 ) == 42 );
}

CASE("invoke: a free function, no arguments, noexcept(false) (C++11)")
{
    EXPECT( nonstd::invoke( f42_nef ) == 42 );
}

CASE("invoke: a free function, 1 argument")
{
    EXPECT( nonstd::invoke( arg, 7 ) == 7 );
}

CASE("invoke: a free function, 2 arguments")
{
    EXPECT( nonstd::invoke( add, 1, 2 ) == 3 );
}

CASE("invoke: a member function, no arguments")
{
    const holder v(42);

    EXPECT( nonstd::invoke( &holder::val, v ) == 42 );
}

CASE("invoke: a member function, 1 argument")
{
    const holder v(1);

    EXPECT( nonstd::invoke( &holder::add, v, 2 ) == 3 );
}

CASE("invoke: a member function, 1 const ref argument")
{
    const holder v(1);

    EXPECT( nonstd::invoke( &holder::add_constref, v, 2 ) == 3 );
}

CASE("invoke: a non-const member function, 1 argument")
{
    /*const*/ holder v(1);

    EXPECT( nonstd::invoke( &holder::add_nonconst, v, 2 ) == 3 );
}

CASE("invoke: a member function, changing its reference argument")
{
    int arg = 7;
    holder v(42);

    nonstd::invoke( &holder::change_arg, v, arg );

    EXPECT( arg == 42 );
}

CASE("invoke: (access) a data member - via const object ref")
{
    const holder h(42);

    EXPECT( nonstd::invoke( &holder::v, h ) == 42 );
}

CASE("invoke: (access) a data member - via const object ptr")
{
    const holder h(42);

    EXPECT( nonstd::invoke( &holder::v, &h ) == 42 );
}

CASE("invoke: (access) a data member - via non-const object ref")
{
    holder h(42);

    EXPECT( nonstd::invoke( &holder::v, h ) == 42 );
}

CASE("invoke: (access) a data member - via non-const object ptr")
{
    holder h(42);

    EXPECT( nonstd::invoke( &holder::v, &h ) == 42 );
}

CASE("invoke: change an invoked (accessed) data member - via non-const object ref")
{
    holder h(42);

    nonstd::invoke( &holder::v, h ) = 7;

    EXPECT( h.v == 7 );
}

CASE("invoke: change an invoked (accessed) data member - via non-const object ptr")
{
    holder h(42);

    nonstd::invoke( &holder::v,  &h ) = 7;

    EXPECT( h.v == 7 );
}

CASE("invoke: a function object, no arguments")
{
    EXPECT( nonstd::invoke( functor() ) == 42 );
}

CASE("invoke: a function object, 1 argument")
{
    EXPECT( nonstd::invoke( functor(), 42 ) == 42 );
}

CASE("invoke: a lambda, no arguments")
{
    EXPECT( nonstd::invoke( [](){ return 42; } ) == 42 );
}

CASE("invoke: a lambda, 1 argument")
{
    EXPECT( nonstd::invoke( [](int a){ return a; }, 42 ) == 42 );
}

// invoke - constexpr:

CASE("invoke: a free function, no arguments - constexpr")
{
#if invoke_CPP11_OR_GREATER && !(invoke_USES_STD_INVOKE && invoke_BETWEEN(invoke_COMPILER_MSVC_VER, 1910, 1920))
    constexpr auto constexpr_result = nonstd::invoke( f42 );

    EXPECT( constexpr_result == 42 );
#else
    EXPECT( !!"invoke(): constexpr is not available (no C++11, or VC141/1910/VS2017 with std::invoke())" );
#endif
}

CASE("invoke: a free function, no arguments, noexcept(false) (C++11) - constexpr")
{
#if invoke_CPP11_OR_GREATER && !(invoke_USES_STD_INVOKE && invoke_BETWEEN(invoke_COMPILER_MSVC_VER, 1910, 1920))
    constexpr auto constexpr_result = nonstd::invoke( f42_nef );

    EXPECT( constexpr_result == 42 );
#else
    EXPECT( !!"invoke(): constexpr is not available (no C++11, or VC141/1910/VS2017 with std::invoke())" );
#endif
}

CASE("invoke: a free function, 1 argument - constexpr")
{
#if invoke_CPP11_OR_GREATER && !(invoke_USES_STD_INVOKE && invoke_BETWEEN(invoke_COMPILER_MSVC_VER, 1910, 1920))
    constexpr auto constexpr_result = nonstd::invoke( arg, 7 );

    EXPECT( constexpr_result == 7 );
#else
    EXPECT( !!"invoke(): constexpr is not available (no C++11, or VC141/1910/VS2017 with std::invoke())" );
#endif
}

CASE("invoke: a free function, 2 arguments - constexpr")
{
#if invoke_CPP11_OR_GREATER && !(invoke_USES_STD_INVOKE && invoke_BETWEEN(invoke_COMPILER_MSVC_VER, 1910, 1920))
    constexpr auto constexpr_result = nonstd::invoke( add, 1, 2 );

    EXPECT( constexpr_result == 3 );
#else
    EXPECT( !!"invoke(): constexpr is not available (no C++11, or VC141/1910/VS2017 with std::invoke())" );
#endif
}

CASE("invoke: a member function, no arguments - constexpr")
{
#if invoke_CPP11_OR_GREATER && !(invoke_USES_STD_INVOKE && invoke_BETWEEN(invoke_COMPILER_MSVC_VER, 1910, 1920))
    constexpr holder v(42);
    constexpr auto constexpr_result = nonstd::invoke( &holder::val, v );

    EXPECT( constexpr_result == 42 );
#else
    EXPECT( !!"invoke(): constexpr is not available (no C++11, or VC141/1910/VS2017 with std::invoke())" );
#endif
}

CASE("invoke: a member function, 1 argument - constexpr")
{
#if invoke_CPP11_OR_GREATER && !(invoke_USES_STD_INVOKE && invoke_BETWEEN(invoke_COMPILER_MSVC_VER, 1910, 1920))
    constexpr holder v(1);
    constexpr auto constexpr_result = nonstd::invoke( &holder::add, v, 2 );

    EXPECT( constexpr_result == 3 );
#else
    EXPECT( !!"invoke(): constexpr is not available (no C++11, or VC141/1910/VS2017 with std::invoke())" );
#endif
}

CASE("invoke: a member function, 1 const ref argument - constexpr")
{
#if invoke_CPP11_OR_GREATER && !(invoke_USES_STD_INVOKE && invoke_BETWEEN(invoke_COMPILER_MSVC_VER, 1910, 1920))
    constexpr holder v(1);
    constexpr auto constexpr_result = nonstd::invoke( &holder::add_constref, v, 2 );

    EXPECT( constexpr_result == 3 );
#else
    EXPECT( !!"invoke(): constexpr is not available (no C++11, or VC141/1910/VS2017 with std::invoke())" );
#endif
}

// CASE("invoke: a non-const member function, 1 argument - constexpr")
// {
// #if invoke_CPP11_OR_GREATER
//     constexpr holder v(1);
//     constexpr auto constexpr_result = nonstd::invoke( &holder::add_nonconst, v, 2 );
//
//     EXPECT( constexpr_result == 3 );
// #else
//     EXPECT( !!"invoke(): constexpr is not available (no C++11)" );
// #endif
// }

// CASE("invoke: a member function, changing its reference argument - constexpr")
// {
// #if invoke_CPP11_OR_GREATER
//     int arg = 7;
//     constexpr holder v(42);
//
//     nonstd::invoke( &holder::change_arg, v, arg );
//
//     EXPECT( arg == 42 );
// #else
//     EXPECT( !!"invoke(): constexpr is not available (no C++11)" );
// #endif
// }

CASE("invoke: (access) a data member - via const object ref - constexpr")
{
#if invoke_CPP11_OR_GREATER && !(invoke_USES_STD_INVOKE && invoke_BETWEEN(invoke_COMPILER_MSVC_VER, 1910, 1920))
    constexpr holder h(42);
    constexpr auto constexpr_result = nonstd::invoke( &holder::v, h );

    EXPECT( constexpr_result == 42 );
#else
    EXPECT( !!"invoke(): constexpr is not available (no C++11, or VC141/1910/VS2017 with std::invoke())" );
#endif
}

CASE("invoke: (access) a data member - via const object ptr - constexpr")
{
#if invoke_CPP11_OR_GREATER && !(invoke_USES_STD_INVOKE && invoke_BETWEEN(invoke_COMPILER_MSVC_VER, 1910, 1920))
    constexpr holder h(42);
    constexpr auto constexpr_result = nonstd::invoke( &holder::v, &h );

    EXPECT( constexpr_result == 42 );
#else
    EXPECT( !!"invoke(): constexpr is not available (no C++11, or VC141/1910/VS2017 with std::invoke())" );
#endif
}

// CASE("invoke: (access) a data member - via non-const object ref - constexpr")
// {
// #if invoke_CPP11_OR_GREATER
//     holder h(42);
//     constexpr auto constexpr_result = nonstd::invoke( &holder::v, h );

//     EXPECT( constexpr_result == 42 );
// #else
//     EXPECT( !!"invoke(): constexpr is not available (no C++11)" );
// #endif
// }

// CASE("invoke: (access) a data member - via non-const object ptr - constexpr")
// {
// #if invoke_CPP11_OR_GREATER
//     holder h(42);
//     constexpr auto constexpr_result = nonstd::invoke( &holder::v, &h );

//     EXPECT( constexpr_result == 42 );
// #else
//     EXPECT( !!"invoke(): constexpr is not available (no C++11)" );
// #endif
// }

// CASE("invoke: change an invoked (accessed) data member - via non-const object ref - constexpr")
// {
// #if invoke_CPP11_OR_GREATER
//     holder h(42);

//     nonstd::invoke( &holder::v, h ) = 7;

//     EXPECT( h.v == 7 );
// #else
//     EXPECT( !!"invoke(): constexpr is not available (no C++11)" );
// #endif
// }

// CASE("invoke: change an invoked (accessed) data member - via non-const object ptr - constexpr")
// {
// #if invoke_CPP11_OR_GREATER
//     holder h(42);

//     nonstd::invoke( &holder::v,  &h ) = 7;

//     EXPECT( h.v == 7 );
// #else
//     EXPECT( !!"invoke(): constexpr is not available (no C++11)" );
// #endif
// }

CASE("invoke: a function object, no arguments - constexpr")
{
#if invoke_CPP11_OR_GREATER && !(invoke_USES_STD_INVOKE && invoke_BETWEEN(invoke_COMPILER_MSVC_VER, 1910, 1920))
    constexpr auto constexpr_result = nonstd::invoke( functor() );

    EXPECT( constexpr_result == 42 );
#else
    EXPECT( !!"invoke(): constexpr is not available (no C++11, or VC141/1910/VS2017 with std::invoke())" );
#endif
}

CASE("invoke: a function object, 1 argument - constexpr")
{
#if invoke_CPP11_OR_GREATER && !(invoke_USES_STD_INVOKE && invoke_BETWEEN(invoke_COMPILER_MSVC_VER, 1910, 1920))
    constexpr auto constexpr_result = nonstd::invoke( functor(), 42 );

    EXPECT( constexpr_result == 42 );
#else
    EXPECT( !!"invoke(): constexpr is not available (no C++11, or VC141/1910/VS2017 with std::invoke())" );
#endif
}

CASE("invoke: a lambda, no arguments - constexpr (C++17)")
{
#if invoke_CPP17_OR_GREATER && !(invoke_USES_STD_INVOKE && invoke_BETWEEN(invoke_COMPILER_MSVC_VER, 1910, 1920))
    constexpr auto constexpr_result = nonstd::invoke( [](){ return 42; } );

    EXPECT( constexpr_result == 42 );
#else
    EXPECT( !!"invoke(): constexpr closure is not available (no C++17, or VC141/1910/VS2017 with std::invoke())" );
#endif
}

CASE("invoke: a lambda, 1 argument - constexpr (C++17)")
{
#if invoke_CPP17_OR_GREATER && !(invoke_USES_STD_INVOKE && invoke_BETWEEN(invoke_COMPILER_MSVC_VER, 1910, 1920))
    constexpr auto constexpr_result = nonstd::invoke( [](int a){ return a; }, 42 );

    EXPECT( constexpr_result == 42 );
#else
    EXPECT( !!"invoke(): constexpr closure is not available (no C++17, or VC141/1910/VS2017 with std::invoke())" );
#endif
}

//
// apply():
//

CASE("apply: a function object, std::pair of 2 arguments (C++11)")
{
#if invoke_CPP11_OR_GREATER
    EXPECT( nonstd::apply( add, std::pair<int, int>(1, 2) ) == 3 );
#else
    EXPECT( !!"apply() is not available (no C++11)" );
#endif
}

CASE("apply: a function object, std::tuple of 2 arguments (C++11)")
{
#if invoke_CPP11_OR_GREATER
    EXPECT( nonstd::apply( add, std::tuple<int, int>(1, 2) ) == 3 );
#else
    EXPECT( !!"apply() is not available (no C++11)" );
#endif
}

CASE("apply: a function object, std::array of 2 arguments (C++11)")
{
#if invoke_CPP11_OR_GREATER
    EXPECT( nonstd::apply( add, std::array<int,2>({ 1, 2}) ) == 3 );
#else
    EXPECT( !!"apply() is not available (no C++11)" );
#endif
}

CASE("apply: a lambda, std::pair of 2 arguments (C++11)")
{
#if invoke_CPP11_OR_GREATER
    EXPECT( nonstd::apply( [](int a, int b){ return a + b; }, std::pair<int, int>(1, 2)) == 3 );
#else
    EXPECT( !!"apply() is not available (no C++11)" );
#endif
}

CASE("apply: a lambda, std::tuple of 2 arguments (C++11)")
{
#if invoke_CPP11_OR_GREATER
    EXPECT( nonstd::apply( add, std::tuple<int, int>(1, 2) ) == 3 );
#else
    EXPECT( !!"apply() is not available (no C++11)" );
#endif
}

CASE("apply: a lambda, std::array of 2 arguments (C++11)")
{
#if invoke_CPP11_OR_GREATER
    EXPECT( nonstd::apply( [](int a, int b){ return a + b; }, std::array<int,2>({ 1, 2})) == 3 );
#else
    EXPECT( !!"apply() is not available (no C++11)" );
#endif
}

// apply - constexpr:

CASE("apply: a function object, std::pair of 2 arguments (C++11) - constexpr")
{
#if invoke_CPP11_OR_GREATER
    constexpr auto constexpr_result = nonstd::apply( add, std::pair<int, int>(1, 2) );
    EXPECT( constexpr_result == 3 );
#else
    EXPECT( !!"apply() is not available (no C++11)" );
#endif
}

CASE("apply: a function object, std::tuple of 2 arguments (C++11) - constexpr")
{
#if invoke_CPP11_OR_GREATER
    constexpr auto constexpr_result = nonstd::apply( add, std::tuple<int, int>(1, 2) );
    EXPECT( constexpr_result == 3 );
#else
    EXPECT( !!"apply() is not available (no C++11)" );
#endif
}

CASE("apply: a function object, std::array of 2 arguments (C++11) - constexpr")
{
#if invoke_CPP11_OR_GREATER
    constexpr auto constexpr_result = nonstd::apply( add, std::array<int,2>({ 1, 2}) );
    EXPECT( constexpr_result == 3 );
#else
    EXPECT( !!"apply() is not available (no C++11)" );
#endif
}

CASE("apply: a lambda, std::pair of 2 arguments (C++11) - constexpr")
{
#if invoke_CPP17_OR_GREATER
    constexpr auto constexpr_result = nonstd::apply( [](int a, int b){ return a + b; }, std::pair<int, int>(1, 2) );
    EXPECT( constexpr_result == 3 );
#else
    EXPECT( !!"apply(): constexpr closure is not available (no C++17)" );
#endif
}

CASE("apply: a lambda, std::tuple of 2 arguments (C++11) - constexpr")
{
#if invoke_CPP11_OR_GREATER
    constexpr auto constexpr_result = nonstd::apply( add, std::tuple<int, int>(1, 2) );
    EXPECT( constexpr_result == 3 );
#else
    EXPECT( !!"apply() is not available (no C++11)" );
#endif
}

CASE("apply: a lambda, std::array of 2 arguments (C++11) - constexpr")
{
#if invoke_CPP17_OR_GREATER
    constexpr auto constexpr_result = nonstd::apply( [](int a, int b){ return a + b; }, std::array<int,2>({ 1, 2}));
    EXPECT( constexpr_result == 3 );
#else
    EXPECT( !!"apply(): constexpr closure is not available (no C++17)" );
#endif
}

//
// Other:
//

CASE("invoke: App" "[.invoke][.app]")
{
    std::cout << "App: ...\n";
}

CASE( "tweak header: Reads tweak header if supported " "[tweak]" )
{
#if invoke_HAVE_TWEAK_HEADER
    EXPECT( invoke_TWEAK_VALUE == 42 );
#else
    EXPECT( !!"Tweak header is not available (invoke_HAVE_TWEAK_HEADER: 0)." );
#endif
}

} // anonymous namespace
