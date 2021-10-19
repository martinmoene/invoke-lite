// invode-lite/example/apply.cpp

#include "nonstd/invoke.hpp"

#include <iostream>
#include <utility>

#if invoke_CPP14_OR_GREATER

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

auto add_lambda = [](auto first, auto second)
{ 
    return first + second;
};
 
#if invoke_CPP17_OR_GREATER

template<typename... Ts>
std::ostream& operator<<(std::ostream& os, std::tuple<Ts...> const& theTuple)
{
    std::apply
    (
        [&os](Ts const&... tupleArgs)
        {
            os << '[';
            std::size_t n{0};
            ((os << tupleArgs << (++n != sizeof...(Ts) ? ", " : "")), ...);
            os << ']';
        }, theTuple
    );
    return os;
}

#endif // invoke_CPP17_OR_GREATER
#endif // invoke_CPP14_OR_GREATER

int main()
{
#if invoke_CPP17_OR_GREATER

    // OK
    std::cout << nonstd::apply(add, std::pair(1, 2)) << '\n';
 
    // Error: can't deduce the function type
    // std::cout << std::apply(add_generic, std::make_pair(2.0f, 3.0f)) << '\n'; 
 
    // OK
    std::cout << nonstd::apply(add_lambda, std::pair(2.0f, 3.0f)) << '\n'; 
 
    // advanced example
    std::tuple myTuple(25, "Hello", 9.31f, 'c');
    std::cout << myTuple << '\n';

#endif // invoke_CPP17_OR_GREATER
 }

// MSVC 20xx (VCxx) and newer
// cl -EHsc -Dinvoke_CONFIG_SELECT_INVOKE=invoke_invoke_NONSTD -I../include apply.cpp && apply.exe

// GNUC, std=c++98, c++11, c++17
// g++ -std=c++11 -Dinvoke_CONFIG_SELECT_INVOKE=invoke_invoke_NONSTD -I../include -o apply.exe apply.cpp && apply.exe
