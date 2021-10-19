@setlocal
@set std=%1
@if "%std%"=="" set std=c++17
g++ -std=%std% -Dinvoke_CONFIG_SELECT_INVOKE=invoke_invoke_NONSTD -Dinvoke_USE_ALTERNATE_IMPL=1 -isystem lest -I../include -o apply.exe apply.cpp && apply.exe
@endlocal
