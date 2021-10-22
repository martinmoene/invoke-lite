// invoke-lite/example/apply.cpp

#include "nonstd/invoke.hpp"

#include <iostream>
#include <utility>

#if invoke_CPP11_OR_GREATER

#include <tuple>

int add(int first, int second)
{
    return first + second;
}

template<typename T>
T add_generic(T first, T second)
{
    return first + second;
}

auto add_lambda = [](float first, float second)
{
    return first + second;
};

#endif // invoke_CPP11_OR_GREATER

int main()
{
#if invoke_CPP11_OR_GREATER

    // OK
    std::cout <<
        "\nnonstd::apply(add, std::pair <int, int>(1, 2)) : " <<
           nonstd::apply(add, std::pair <int, int>(1, 2))     <<
        "\nnonstd::apply(add, std::tuple<int, int>(1, 2)) : " <<
           nonstd::apply(add, std::tuple<int, int>(1, 2))     <<
        "\nnonstd::apply(add, std::array<int, 2>({ 1, 2})): " <<
           nonstd::apply(add, std::array<int, 2>({ 1, 2}))    <<
         '\n';

    // Error: can't deduce the function type
    // std::cout << std::apply(add_generic, std::make_pair(2.0f, 3.0f)) << '\n';

    // OK
    std::cout <<
        "\nnonstd::apply(add_lambda, std::pair <float, float>(2.0f, 3.0f)) : " <<
           nonstd::apply(add_lambda, std::pair <float, float>(2.0f, 3.0f))     <<
        "\nnonstd::apply(add_lambda, std::tuple<float, float>(2.0f, 3.0f)) : " <<
           nonstd::apply(add_lambda, std::tuple<float, float>(2.0f, 3.0f))     <<
        "\nnonstd::apply(add_lambda, std::array<float,   2>({ 2.0f, 3.0f})): " <<
           nonstd::apply(add_lambda, std::array<float,   2>({ 2.0f, 3.0f}))    <<
         '\n';

#else
    std::cout << "This example requires C++11\n";
#endif // invoke_CPP17_OR_GREATER
 }

// MSVC 20xx (VCxx) and newer
// cl -EHsc -Dinvoke_CONFIG_SELECT_INVOKE=invoke_INVOKE_NONSTD -I../include apply.cpp && apply.exe

// GNUC, std=c++98, c++11, c++17
// g++ -std=c++11 -Dinvoke_CONFIG_SELECT_INVOKE=invoke_INVOKE_NONSTD -I../include -o apply.exe apply.cpp && apply.exe
