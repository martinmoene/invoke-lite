@setlocal
@set std=%1
@if "%std%"=="" set std=c++98
g++ -std=%std% -I../include/ -o invoke.exe invoke.cpp && invoke.exe

::g++ -std=%std% -O2 -Wall -Wextra -Wno-unused-parameter -I../include/ optional-bare.t.cpp -o optional-bare.t.exe optional.t.cpp && optional-bare.t.exe
@endlocal

