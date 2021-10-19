@setlocal
@set std=%1
@if "%std%"=="" set std=c++latest
cl -nologo -std:%std% -EHsc -Dinvoke_CONFIG_SELECT_INVOKE=invoke_invoke_NONSTD -Dinvoke_USE_ALTERNATE_IMPL=1 -I../include apply.cpp && apply.exe
@endlocal
