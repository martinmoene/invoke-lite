# Copyright 2021-2021 by Martin Moene
#
# https://github.com/martinmoene/invoke-lite
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

if( NOT DEFINED CMAKE_MINIMUM_REQUIRED_VERSION )
    cmake_minimum_required( VERSION 3.5 FATAL_ERROR )
endif()

project( test LANGUAGES CXX )

set( unit_name "invoke" )
set( PACKAGE   ${unit_name}-lite )
set( PROGRAM   ${unit_name}-lite )
set( SOURCES   ${unit_name}-main.t.cpp ${unit_name}.t.cpp )
set( HEADER    "nonstd/invoke.hpp" )
set( TWEAKD    "." )

message( STATUS "Subproject '${PROJECT_NAME}', programs '${PROGRAM}-*'")

# option text to Boolean:

macro( opt option_text var )
    if( ${option_text} )
        set( ${var} 1 )
    else()
        set( ${var} 0 )
    endif()
endmacro()

set( DEFCMN  -Dinvoke_INVOKE_HEADER=\"${HEADER}\" )

set( OPTIONS     "" )

set( HAS_STD_FLAGS  FALSE )
set( HAS_CPP11_FLAG FALSE )
set( HAS_CPP14_FLAG FALSE )
set( HAS_CPP17_FLAG FALSE )
set( HAS_CPP20_FLAG FALSE )
set( HAS_CPPLATEST_FLAG FALSE )

if( MSVC )
    message( STATUS "Matched: MSVC")

    set( HAS_STD_FLAGS TRUE )

    set( OPTIONS     -W3 -EHsc )
    set( DEFINITIONS -D_SCL_SECURE_NO_WARNINGS ${DEFCMN} )

    if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.00 )
        set( HAS_CPP14_FLAG TRUE )
        set( HAS_CPPLATEST_FLAG TRUE )
    endif()
    if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.11 )
        set( HAS_CPP17_FLAG TRUE )
    endif()

elseif( CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang" )
    message( STATUS "CompilerId: '${CMAKE_CXX_COMPILER_ID}'")

    set( HAS_STD_FLAGS  TRUE )

    set( OPTIONS     -Wall -Wextra -Wconversion -Wsign-conversion -Wshadow -Wno-missing-braces -Wno-unnamed-type-template-args -fno-elide-constructors )
    set( DEFINITIONS ${DEFCMN} )

    # GNU: available -std flags depends on version
    if( CMAKE_CXX_COMPILER_ID MATCHES "GNU" )
        message( STATUS "Matched: GNU")

        if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8.0 )
            set( HAS_CPP11_FLAG TRUE )
        endif()
        if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9.2 )
            set( HAS_CPP14_FLAG TRUE )
        endif()
        if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.1.0 )
            set( HAS_CPP17_FLAG TRUE )
        endif()

    # AppleClang: available -std flags depends on version
    elseif( CMAKE_CXX_COMPILER_ID MATCHES "AppleClang" )
        message( STATUS "Matched: AppleClang")

        if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0.0 )
            set( HAS_CPP11_FLAG TRUE )
        endif()
        if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.1.0 )
            set( HAS_CPP14_FLAG TRUE )
        endif()
        if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.2.0 )
            set( HAS_CPP17_FLAG TRUE )
        endif()

    # Clang: available -std flags depends on version
    elseif( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
        message( STATUS "Matched: Clang")

        if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.3.0 )
            set( HAS_CPP11_FLAG TRUE )
        endif()
        if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.4.0 )
            set( HAS_CPP14_FLAG TRUE )
        endif()
        if( NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0.0 )
            set( HAS_CPP17_FLAG TRUE )
        endif()
    endif()

elseif( CMAKE_CXX_COMPILER_ID MATCHES "Intel" )
    # as is
    message( STATUS "Matched: Intel")
else()
    # as is
    message( STATUS "Matched: nothing")
endif()

# enable MS C++ Core Guidelines checker if MSVC:

function( enable_msvs_guideline_checker target )
    if( MSVC )
        set_target_properties( ${target} PROPERTIES
        VS_GLOBAL_EnableCppCoreCheck true
        VS_GLOBAL_CodeAnalysisRuleSet CppCoreCheckRules.ruleset
        VS_GLOBAL_RunCodeAnalysis true )
    endif()
endfunction()

# make target, compile for given standard if specified:

function( make_target target std )
    message( STATUS "Make target: '${std}'" )

    add_executable            ( ${target} ${SOURCES} )
    target_include_directories( ${target} SYSTEM  PRIVATE lest )
    target_include_directories( ${target} PRIVATE ${TWEAKD} )
    target_link_libraries     ( ${target} PRIVATE ${PACKAGE} )
    target_compile_options    ( ${target} PRIVATE ${OPTIONS} )
    target_compile_definitions( ${target} PRIVATE ${DEFINITIONS} )

    if ( UNIX )
        target_link_libraries ( ${target} PRIVATE pthread )
    endif()

    if( std )
        if( MSVC )
            target_compile_options( ${target} PRIVATE -std:${std} )
        else()
            # Necessary for clang 3.x:
            target_compile_options( ${target} PRIVATE -std=${std} )
            # Ok for clang 4 and later:
            # set( CMAKE_CXX_STANDARD ${std} )
            # set( CMAKE_CXX_STANDARD_REQUIRED ON )
            # set( CMAKE_CXX_EXTENSIONS OFF )
        endif()
    endif()
endfunction()

# add generic executable, unless -std flags can be specified:

if( NOT HAS_STD_FLAGS )
    make_target( ${PROGRAM}.t "" )
else()
    if( HAS_CPP11_FLAG )
        make_target( ${PROGRAM}-cpp11.t c++11 )
    endif()

    if( HAS_CPP14_FLAG )
        make_target( ${PROGRAM}-cpp14.t c++14 )
    endif()

    if( HAS_CPP17_FLAG )
        set( std17 c++17 )
        if( CMAKE_CXX_COMPILER_ID MATCHES "AppleClang" )
            set( std17 c++1z )
        endif()
        make_target( ${PROGRAM}-cpp17.t ${std17} )
        enable_msvs_guideline_checker( ${PROGRAM}-cpp17.t )
    endif()

    if( HAS_CPPLATEST_FLAG )
        make_target( ${PROGRAM}-cpplatest.t c++latest )
    endif()
endif()

# with C++17, honour explicit request for std::invoke or nonstd::invoke:

if( HAS_CPP17_FLAG )
    set( WHICH invoke_INVOKE_DEFAULT )

    if( INVOKE_LITE_OPT_SELECT_STD )
        set( WHICH invoke_INVOKE_STD )
        message( STATUS "Using std::invoke(), std::apply(): ${WHICH}")
    elseif( INVOKE_LITE_OPT_SELECT_NONSTD )
        set( WHICH invoke_INVOKE_NONSTD )
        message( STATUS "Using nonstd::invoke(), nonstd::apply(): ${WHICH}")
    endif()

    target_compile_definitions( ${PROGRAM}-cpp17.t PRIVATE -Dinvoke_CONFIG_SELECT_INVOKE=${WHICH} )

    if( HAS_CPPLATEST_FLAG )
        target_compile_definitions( ${PROGRAM}-cpplatest.t PRIVATE -Dinvoke_CONFIG_SELECT_INVOKE=${WHICH} )
    endif()
endif()

# configure unit tests via CTest:

enable_testing()

if( HAS_STD_FLAGS )
    if( HAS_CPP11_FLAG )
        add_test( NAME test-cpp11     COMMAND ${PROGRAM}-cpp11.t )
    endif()
    if( HAS_CPP14_FLAG )
        add_test( NAME test-cpp14     COMMAND ${PROGRAM}-cpp14.t )
    endif()
    if( HAS_CPP17_FLAG )
        add_test( NAME test-cpp17     COMMAND ${PROGRAM}-cpp17.t )
    endif()
    if( HAS_CPPLATEST_FLAG )
        add_test( NAME test-cpplatest COMMAND ${PROGRAM}-cpplatest.t )
    endif()
else()
    add_test(     NAME test           COMMAND ${PROGRAM}.t --pass )
    add_test(     NAME list_version   COMMAND ${PROGRAM}.t --version )
    add_test(     NAME list_tags      COMMAND ${PROGRAM}.t --list-tags )
    add_test(     NAME list_tests     COMMAND ${PROGRAM}.t --list-tests )
endif()

# end of file
