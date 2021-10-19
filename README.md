# invoke lite: C++17's invoke for C++98 and later and apply for C++14 and later

A work in its infancy. Suggested and contributed to by Peter Featherstone.

[![Language](https://img.shields.io/badge/C%2B%2B-11/14/17/20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![License](https://img.shields.io/badge/license-BSL-blue.svg)](https://opensource.org/licenses/BSL-1.0) [![Build Status](https://github.com/martinmoene/any-lite/actions/workflows/ci.yml/badge.svg)](https://github.com/martinmoene/any-lite/actions/workflows/ci.yml) [![Build Status](https://travis-ci.org/martinmoene/invoke-lite.svg?branch=master)](https://travis-ci.org/martinmoene/invoke-lite) [![Build status](https://ci.appveyor.com/api/projects/status/nrnbfhvvp39ex075?svg=true)](https://ci.appveyor.com/project/martinmoene/invoke-lite) [![Version](https://badge.fury.io/gh/martinmoene%2Finvoke-lite.svg)](https://github.com/martinmoene/invoke-lite/releases) [![download](https://img.shields.io/badge/latest-download-blue.svg)](https://raw.githubusercontent.com/martinmoene/invoke-lite/master/include/nonstd/invoke.hpp) [![Conan](https://img.shields.io/badge/on-conan-blue.svg)]() [![Try it online](https://img.shields.io/badge/on-wandbox-blue.svg)](https://wandbox.org/permlink/DiMxDuWYOiUMKsdj) [![Try it on godbolt online](https://img.shields.io/badge/on-godbolt-blue.svg)](https://godbolt.org/z/7dEz5r)

**Contents**  

- [Example usage](#example-usage)
- [In a nutshell](#in-a-nutshell)
- [License](#license)
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Synopsis](#synopsis)
- [Other implementations of invoke](#other-implementations-of-invoke)
- [Notes and references](#notes-and-references)
- [Appendix](#appendix)

<!-- - [Reported to work with](#reported-to-work-with)
- [Building the tests](#building-the-tests) -->

## Example usage

```Cpp
TBD
```

### Compile and run

```Text
TBD
```

## In a nutshell

**invoke lite** is a single-file header-only library to provide [C++17's invoke](https://en.cppreference.com/w/cpp/thread/invoke) for use with C++98 and later. If available, the standard library is used, unless [configured](#configuration) otherwise.

Currently `nonstd::invoke` ... .

Currently `nonstd::apply` ... .

**Features and properties of invoke lite** are ease of installation (single header), freedom of dependencies other than the standard library.

**Limitations of invoke lite** are ... \[*to be summed up*\].

## License

*invoke lite* is distributed under the [Boost Software License](https://github.com/martinmoene/invoke-lite/blob/master/LICENSE.txt).

## Dependencies

*invoke lite* has no other dependencies than the [C++ standard library](http://en.cppreference.com/w/cpp/header).

## Installation

*invoke lite* is a single-file header-only library. Put `invoke.hpp` in the [include](include) folder directly into the project source tree or somewhere reachable from your project.

## Synopsis

- [Documentation of `invoke`](#documentation-of-invoke)
- [*invoke lite* implementation status](#invoke-lite-implementation-status)
- [Configuration](#configuration)

### Documentation of invoke

\[*Envisioned*\] Depending on the compiler and C++ standard used, *invoke lite* behaves less or more like the standard's version. To get an idea of the capabilities of *invoke lite* with your configuration, look at the output of the [tests](test/invoke.t.cpp), issuing `invoke-main.t --pass @`.

For the standard's documentation, see [`invoke`](https://en.cppreference.com/w/cpp/utility/functional/invoke), which is part of the [C++ utility function objects library](https://en.cppreference.com/w/cpp/utility/functional) and [`apply`](https://en.cppreference.com/w/cpp/utility/apply), which is part of the [C++ utility library](https://en.cppreference.com/w/cpp/utility).

### *invoke lite* implementation status

| Kind               | Type or function             | Notes |
|--------------------|------------------------------|-------|
| &nbsp;             | apply()  | &nbsp; |
| &nbsp;             | invoke() | &nbsp; |
| &nbsp;             | &nbsp; | &nbsp; |

### Configuration

#### Tweak header

If the compiler supports [`__has_include()`](https://en.cppreference.com/w/cpp/preprocessor/include), *invoke lite* supports the [tweak header](https://vector-of-bool.github.io/2020/10/04/lib-configuration.html) mechanism. Provide your *tweak header* as `nonstd/invoke.tweak.hpp` in a folder in the include-search-path. In the tweak header, provide definitions as documented below, like `#define invoke_CPLUSPLUS 201103L`.

### Select `std::invoke` or `nonstd::invoke`

\[*To be implemented*\]

At default, *invoke lite* uses `std::invoke` if it is available and lets you use it via namespace `nonstd`. You can however override this default and explicitly request to use `std::invoke` or invoke lite's `nonstd::invoke` as `nonstd::invoke` via the following macros.

-D<b>invoke\_CONFIG\_SELECT\_INVOKE</b>=invoke\_SELECT\_INVOKE\_NONSTD  
Define this to `invoke__CONFIG_SELECT_INVOKE_STD` to select `std::invoke` as `nonstd::invoke`. Define this to `invoke_SELECT_INVOKE_NONSTD` to select `nonstd::invoke` as `nonstd::invoke`. Default is undefined, which has the same effect as defining to `invoke_SELECT_INVOKE_NONSTD` currently (this may change to `invoke_SELECT_INVOKE_DEFAULT`).

#### Standard selection macro

\-D<b>invoke\_CPLUSPLUS</b>=199711L  
Define this macro to override the auto-detection of the supported C++ standard, if your compiler does not set the `__cplusplus` macro correctly.

#### Disable exceptions

-D<b>invoke_CONFIG_NO_EXCEPTIONS</b>=0
Define this to 1 if you want to compile without exceptions. If not defined, the header tries and detect if exceptions have been disabled (e.g. via `-fno-exceptions`). Default is undefined.

## Other implementations of invoke

- TBD.

## Notes and references

- TBD

## Appendix

<a id="a1"></a>
### A.1 Compile-time information

The version of *invoke lite* is available via tag `[.version]`. The following tags are available for information on the compiler and on the C++ standard library used: `[.compiler]`, `[.stdc++]`, `[.stdlanguage]` and `[.stdlibrary]`.

<a id="a2"></a>
### A.2 Invoke lite test specification

<details>
<summary>click to expand</summary>
<p>

```Text
invoke: ...
tweak header: Reads tweak header if supported [tweak]
```

</p>
</details>
