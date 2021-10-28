// ./benchmark/benchmark-invoke.cpp
//
// Using benchmark facility of Catch2 test framework.
// - Project: https://github.com/catchorg/Catch2
// - Single include file: https://github.com/catchorg/Catch2/releases/download/v2.13.7/catch.hpp
//

#define CATCH_CONFIG_ENABLE_BENCHMARKING
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "nonstd/invoke.hpp"

//------------------------------------------------------------------------

#if 0

// Example from Catch2 documentation:

std::uint64_t Fibonacci(std::uint64_t number)
{
    return number < 2 ? 1 : Fibonacci(number - 1) + Fibonacci(number - 2);
}

TEST_CASE("Fibonacci")
{
    CHECK(Fibonacci(0) == 1);
    CHECK(Fibonacci(5) == 8);

    BENCHMARK("Fibonacci 20")
	{
        return Fibonacci(20);
    };

    BENCHMARK("Fibonacci 25")
	{
        return Fibonacci(25);
    };
}

#endif

//------------------------------------------------------------------------

#ifndef  invoke_noexcept_op
# define invoke_noexcept_op(expr) noexcept(expr)
#endif

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

} // anonymous namespace


TEST_CASE("DataBuffer")
{
	BENCHMARK("invoke( f42 )"    ) { return nonstd::invoke( f42 ); };
	BENCHMARK("invoke( arg, 7 )" ) { return nonstd::invoke( arg, 7 ); };
	BENCHMARK("invoke( add, 1, 2 )" ) { return nonstd::invoke( add, 1, 2 ); };
	BENCHMARK("invoke( &holder::val, v )" ) { const holder v(42); return nonstd::invoke( &holder::val, v ); };
	BENCHMARK("invoke( &holder::add, v, 2 )" ) { const holder v(1); return nonstd::invoke( &holder::add, v, 2 ); };
	// BENCHMARK("xxxx" ) { return xxxxx; };
	// BENCHMARK("xxxx" ) { return xxxxx; };
	// BENCHMARK("xxxx" ) { return xxxxx; };
	// BENCHMARK("xxxx" ) { return xxxxx; };

	// BENCHMARK("xxxx" ) { return xxxxx; };
}

// g++ -std=c++17 -Wall -Wextra     -O2 -Dinvoke_CONFIG_SELECT_INVOKE=invoke_INVOKE_NONSTD -o benchmark-invoke -I../include -Icatch benchmark-invoke.cpp && benchmark-invoke --benchmark-no-analysis
// g++ -std=c++17 -Wall -Wextra -g3 -Og -Dinvoke_CONFIG_SELECT_INVOKE=invoke_INVOKE_NONSTD -o benchmark-invoke -I../include -Icatch benchmark-invoke.cpp && benchmark-invoke --benchmark-no-analysis
// g++ -std=c++17 -Wall -Wextra -g3 -O0 -Dinvoke_CONFIG_SELECT_INVOKE=invoke_INVOKE_NONSTD -o benchmark-invoke -I../include -Icatch benchmark-invoke.cpp && benchmark-invoke --benchmark-no-analysis

// cl -nologo -std:c++latest -EHsc -W4 -O2 -Dinvoke_CONFIG_SELECT_INVOKE=invoke_INVOKE_NONSTD -Febenchmark-invoke-nonstd-lite.exe -I../include -Icatch benchmark-invoke.cpp & benchmark-invoke.exe --benchmark-no-analysis

// Example output:
