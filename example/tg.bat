@setlocal
@set std=%1
@if "%std%"=="" set std=c++11
g++ -std=%std% -Dinvoke_CONFIG_SELECT_INVOKE=invoke_invoke_NONSTD -Dinvoke_USE_ALTERNATE_IMPL=1 -isystem lest -I../include -o invoke.exe invoke.cpp && invoke.exe
@endlocal
