@echo off & setlocal enableextensions enabledelayedexpansion
::
:: tc.bat - compile & run tests (clang).
::

set      unit=invoke
set unit_file=invoke

:: if no std is given, use c++11

set std=c++11
if NOT "%1" == "" set std=%1 & shift

set UCAP=%unit%
call :toupper UCAP

set unit_select=%unit%_%UCAP%_NONSTD
::set unit_select=%unit%_CONFIG_SELECT_%UCAP%_NONSTD
if NOT "%1" == "" set unit_select=%1 & shift

set args=%1 %2 %3 %4 %5 %6 %7 %8 %9

set  clang=clang

call :CompilerVersion version
echo %clang% %version%: %std% %unit_select% %args%

set unit_config=^
    -Dinvoke_INVOKE_HEADER=\"nonstd/invoke.hpp\" ^
    -Dinvoke_TEST_NODISCARD=0 ^
    -Dinvoke_CONFIG_SELECT_INVOKE=%unit_select%

rem -flto / -fwhole-program
set  optflags=-O2
set warnflags=-Wall -Wextra -Wpedantic -Weverything -Wshadow -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-missing-noreturn -Wno-documentation-unknown-command -Wno-documentation-deprecated-sync -Wno-documentation -Wno-weak-vtables -Wno-missing-prototypes -Wno-missing-variable-declarations -Wno-exit-time-destructors -Wno-global-constructors

"%clang%" -m32 -std=%std% %optflags% %warnflags% %unit_config% -fms-compatibility-version=19.00 -isystem "%VCInstallDir%include" -isystem "%WindowsSdkDir_71A%include" -isystem lest -I../include -I. -o %unit_file%-main.t.exe %unit_file%-main.t.cpp %unit_file%.t.cpp && %unit_file%-main.t.exe
endlocal & goto :EOF

:: subroutines:

:CompilerVersion  version
echo off & setlocal enableextensions
set tmpprogram=_getcompilerversion.tmp
set tmpsource=%tmpprogram%.c

echo #include ^<stdio.h^>     > %tmpsource%
echo int main(){printf("%%d.%%d.%%d\n",__clang_major__,__clang_minor__,__clang_patchlevel__);} >> %tmpsource%

"%clang%" -m32 -o %tmpprogram% %tmpsource% >nul
for /f %%x in ('%tmpprogram%') do set version=%%x
del %tmpprogram%.* >nul
endlocal & set %1=%version%& goto :EOF

:: toupper; makes use of the fact that string
:: replacement (via SET) is not case sensitive
:toupper
for %%L IN (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) DO SET %1=!%1:%%L=%%L!
goto :EOF
