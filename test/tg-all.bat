:: g++ - Compile all permutations of C++ std and invoke_view-s:

@echo off
for %%v in ( invoke_CONFIG_SELECT_INVOKE_STD invoke_CONFIG_SELECT_INVOKE_NONSTD ) do (
    for %%s in ( c++11 c++14 c++17 c++2a ) do (
        call tg.bat %%s %%v
    )
)
