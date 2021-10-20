// invoke-lite/example/00-basic.cpp

#include "nonstd/invoke.hpp"
#include <utility>

int add(int first, int second)
{
    return first + second;
}

int main()
{
    return
        + nonstd::invoke(add, 1, 2)
        + nonstd::apply(add, std::pair<int, int>(1, 2));
}

// MSVC 2015 (VC140/1900) and newer
// cl -nologo -EHsc -Dinvoke_CONFIG_SELECT_INVOKE=invoke_invoke_NONSTD -I../include 00-basic.cpp && 00-basic.exe

// GNUC, std=c++98, c++11, c++17
// g++ -std=c++11 -Dinvoke_CONFIG_SELECT_INVOKE=invoke_invoke_NONSTD -I../include -o 00-basic.exe 00-basic.cpp && 00-basic.exe
