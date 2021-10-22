//
// Copyright (c) 2017-2021 Martin Moene
//
// https://github.com/martinmoene/invoke-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef NONSTD_INVOKE_LITE_HPP
#define NONSTD_INVOKE_LITE_HPP

#define invoke_lite_MAJOR  0
#define invoke_lite_MINOR  0
#define invoke_lite_PATCH  0

#define invoke_lite_VERSION  invoke_STRINGIFY(invoke_lite_MAJOR) "." invoke_STRINGIFY(invoke_lite_MINOR) "." invoke_STRINGIFY(invoke_lite_PATCH)

#define invoke_STRINGIFY(  x )  invoke_STRINGIFY_( x )
#define invoke_STRINGIFY_( x )  #x

// invoke-lite configuration:

#define invoke_INVOKE_DEFAULT  0
#define invoke_INVOKE_NONSTD   1
#define invoke_INVOKE_STD      2

// tweak header support:

#ifdef __has_include
# if __has_include(<nonstd/invoke.tweak.hpp>)
#  include <nonstd/invoke.tweak.hpp>
# endif
#define invoke_HAVE_TWEAK_HEADER  1
#else
#define invoke_HAVE_TWEAK_HEADER  0
//# pragma message("invoke.hpp: Note: Tweak header not supported.")
#endif

// invoke selection and configuration:

#ifndef  invoke_CONFIG_SELECT_INVOKE
# define invoke_CONFIG_SELECT_INVOKE  invoke_INVOKE_DEFAULT
#endif

#ifndef  invoke_USE_ALTERNATE_IMPL
# define invoke_USE_ALTERNATE_IMPL  0
#endif

// Control presence of exception handling (try and auto discover):

#ifndef invoke_CONFIG_NO_EXCEPTIONS
# if defined(_MSC_VER)
#  include <cstddef>    // for _HAS_EXCEPTIONS
# endif
# if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || (_HAS_EXCEPTIONS)
#  define invoke_CONFIG_NO_EXCEPTIONS  0
# else
#  define invoke_CONFIG_NO_EXCEPTIONS  1
# endif
#endif

// C++ language version detection (C++20 is speculative):
// Note: VC14.0/1900 (VS2015) lacks too much from C++14.

#ifndef   invoke_CPLUSPLUS
# if defined(_MSVC_LANG ) && !defined(__clang__)
#  define invoke_CPLUSPLUS  (_MSC_VER == 1900 ? 201103L : _MSVC_LANG )
# else
#  define invoke_CPLUSPLUS  __cplusplus
# endif
#endif

#define invoke_CPP98_OR_GREATER  ( invoke_CPLUSPLUS >= 199711L )
#define invoke_CPP11_OR_GREATER  ( invoke_CPLUSPLUS >= 201103L )
#define invoke_CPP14_OR_GREATER  ( invoke_CPLUSPLUS >= 201402L )
#define invoke_CPP17_OR_GREATER  ( invoke_CPLUSPLUS >= 201703L )
#define invoke_CPP20_OR_GREATER  ( invoke_CPLUSPLUS >= 202000L )

// Use C++17 std::invoke if available and requested:

#if invoke_CPP17_OR_GREATER
# define  invoke_HAVE_STD_INVOKE  1
#else
# define  invoke_HAVE_STD_INVOKE  0
#endif

#define invoke_USES_STD_INVOKE  ( (invoke_CONFIG_SELECT_INVOKE == invoke_INVOKE_STD) || ((invoke_CONFIG_SELECT_INVOKE == invoke_INVOKE_DEFAULT) && invoke_HAVE_STD_INVOKE) )

//
// Use standard C++17 version:
//

#if invoke_USES_STD_INVOKE

#pragma message ("*** Using std::invoke")

#include <functional>

namespace nonstd {
    using std::invoke;
}

#elif invoke_CPP11_OR_GREATER
#pragma message ("*** Using nonstd::invoke - C++11")

//
// C++11, code taken from http://en.cppreference.com/w/cpp/types/result_of:
//

#include <type_traits>
#include <utility>

#if invoke_USE_ALTERNATE_IMPL   // see issue #1
# include <functional>
#endif

// Compiler versions:
//
// MSVC++  6.0  _MSC_VER == 1200  invoke_COMPILER_MSVC_VERSION ==  60  (Visual Studio 6.0)
// MSVC++  7.0  _MSC_VER == 1300  invoke_COMPILER_MSVC_VERSION ==  70  (Visual Studio .NET 2002)
// MSVC++  7.1  _MSC_VER == 1310  invoke_COMPILER_MSVC_VERSION ==  71  (Visual Studio .NET 2003)
// MSVC++  8.0  _MSC_VER == 1400  invoke_COMPILER_MSVC_VERSION ==  80  (Visual Studio 2005)
// MSVC++  9.0  _MSC_VER == 1500  invoke_COMPILER_MSVC_VERSION ==  90  (Visual Studio 2008)
// MSVC++ 10.0  _MSC_VER == 1600  invoke_COMPILER_MSVC_VERSION == 100  (Visual Studio 2010)
// MSVC++ 11.0  _MSC_VER == 1700  invoke_COMPILER_MSVC_VERSION == 110  (Visual Studio 2012)
// MSVC++ 12.0  _MSC_VER == 1800  invoke_COMPILER_MSVC_VERSION == 120  (Visual Studio 2013)
// MSVC++ 14.0  _MSC_VER == 1900  invoke_COMPILER_MSVC_VERSION == 140  (Visual Studio 2015)
// MSVC++ 14.1  _MSC_VER >= 1910  invoke_COMPILER_MSVC_VERSION == 141  (Visual Studio 2017)
// MSVC++ 14.2  _MSC_VER >= 1920  invoke_COMPILER_MSVC_VERSION == 142  (Visual Studio 2019)

#if defined(_MSC_VER ) && !defined(__clang__)
# define invoke_COMPILER_MSVC_VER      (_MSC_VER )
# define invoke_COMPILER_MSVC_VERSION  (_MSC_VER / 10 - 10 * ( 5 + (_MSC_VER < 1900 ) ) )
#else
# define invoke_COMPILER_MSVC_VER      0
# define invoke_COMPILER_MSVC_VERSION  0
#endif

#define invoke_COMPILER_VERSION( major, minor, patch )  ( 10 * ( 10 * (major) + (minor) ) + (patch) )

#if defined(__clang__)
# define invoke_COMPILER_CLANG_VERSION  invoke_COMPILER_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)
#else
# define invoke_COMPILER_CLANG_VERSION  0
#endif

#if defined(__GNUC__) && !defined(__clang__)
# define invoke_COMPILER_GNUC_VERSION  invoke_COMPILER_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#else
# define invoke_COMPILER_GNUC_VERSION  0
#endif

// half-open range [lo..hi):
//#define invoke_BETWEEN( v, lo, hi ) ( (lo) <= (v) && (v) < (hi) )

// Presence of language and library features:

#define invoke_HAVE( feature )  ( invoke_HAVE_##feature )

#ifdef _HAS_CPP0X
# define invoke_HAS_CPP0X  _HAS_CPP0X
#else
# define invoke_HAS_CPP0X  0
#endif

#define invoke_CPP11_90   (invoke_CPP11_OR_GREATER || invoke_COMPILER_MSVC_VER >= 1500)
#define invoke_CPP11_100  (invoke_CPP11_OR_GREATER || invoke_COMPILER_MSVC_VER >= 1600)
#define invoke_CPP11_120  (invoke_CPP11_OR_GREATER || invoke_COMPILER_MSVC_VER >= 1800)
#define invoke_CPP11_140  (invoke_CPP11_OR_GREATER || invoke_COMPILER_MSVC_VER >= 1900)

#define invoke_CPP11_000  (invoke_CPP11_OR_GREATER)
#define invoke_CPP14_000  (invoke_CPP14_OR_GREATER)
#define invoke_CPP17_000  (invoke_CPP17_OR_GREATER)

// Presence of C++11 language features:

#define invoke_HAVE_NOEXCEPT                invoke_CPP11_140
#define invoke_HAVE_TYPE_TRAITS             invoke_CPP11_90

// Presence of C++14 language features:

// Presence of C++17 language features:

#define invoke_HAVE_NODISCARD               invoke_CPP17_000

// Presence of C++ language features:

#if invoke_HAVE_NOEXCEPT && !invoke_CONFIG_NO_EXCEPTIONS
# define invoke_noexcept noexcept
# define invoke_noexcept_op(expr) noexcept(expr)
#else
# define invoke_noexcept /*noexcept*/
# define invoke_noexcept_op(expr) /*noexcept(expr)*/
#endif

#if invoke_HAVE_NODISCARD
# define invoke_nodiscard [[nodiscard]]
#else
# define invoke_nodiscard /*[[nodiscard]]*/
#endif

// additional includes:

#if invoke_CONFIG_NO_EXCEPTIONS
# include <cassert>
#else
# include <typeinfo>
#endif

#if invoke_HAVE_TYPE_TRAITS
# include <type_traits>
#elif invoke_HAVE_TR1_TYPE_TRAITS
# include <tr1/type_traits>
#endif

namespace nonstd {
namespace detail {

#if invoke_USE_ALTERNATE_IMPL   // see issue #1

#pragma message ("*** Using alternate version")

// C++11 implementation contributed by Peter Featherstone, @pfeatherstone

template< typename F, typename ... Args >
auto INVOKE( F&& fn, Args&& ... args )
-> typename std::enable_if<
    std::is_member_pointer<typename std::decay<F>::type>::value,
    decltype(std::mem_fn(fn)( std::forward<Args>(args)...) )>::type
{
    return std::mem_fn(fn)( std::forward<Args>(args)...);
}

template< typename F, typename ... Args >
auto INVOKE( F&& fn, Args&& ... args )
-> typename std::enable_if<
    ! std::is_member_pointer<typename std::decay<F>::type>::value,
    decltype(std::forward<F>(fn)( std::forward<Args>(args)...) )>::type
{
    return std::forward<F>(fn)(std::forward<Args>(args)...);
}

#else // invoke_USE_ALTERNATE_IMPL

template< typename T > struct is_reference_wrapper : std::false_type {};
template< typename U > struct is_reference_wrapper<std::reference_wrapper<U>> : std::true_type {};

template <typename Base, typename T, typename Derived, typename... Args>
auto INVOKE( T Base::*pmf, Derived && ref, Args &&... args )
    -> typename std::enable_if<
        std::is_function<T>::value
        && std::is_base_of<Base, typename std::decay<Derived>::type>::value
        , decltype((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...))>::type
{
    return (std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...);
}

template< typename Base, typename T, typename RefWrap, typename... Args >
auto INVOKE( T Base::*pmf, RefWrap && ref, Args &&... args )
    -> typename std::enable_if<
        std::is_function<T>::value
        && is_reference_wrapper<typename std::decay<RefWrap>::type>::value
        , decltype((ref.get().*pmf)(std::forward<Args>(args)...))>::type
{
    return (ref.get().*pmf)(std::forward<Args>(args)...);
}

template< typename Base, typename T, typename Ptr, typename... Args>
auto INVOKE( T Base::*pmf, Ptr && ptr, Args &&... args )
    -> typename std::enable_if<
        std::is_function<T>::value
        && !is_reference_wrapper<typename std::decay<Ptr>::type>::value
        && !std::is_base_of<Base, typename std::decay<Ptr>::type>::value
        , decltype(((*std::forward<Ptr>(ptr)).*pmf)(std::forward<Args>(args)...))>::type
{
    return ((*std::forward<Ptr>(ptr)).*pmf)( std::forward<Args>( args )...);
}

template< typename Base, typename T, typename Derived>
auto INVOKE( T Base::*pmd, Derived && ref )
    -> typename std::enable_if<
        !std::is_function<T>::value
        && std::is_base_of<Base, typename std::decay<Derived>::type>::value
        , decltype(std::forward<Derived>(ref).*pmd)>::type
{
    return std::forward<Derived>(ref).*pmd;
}

template< typename Base, typename T, typename RefWrap>
auto INVOKE( T Base::*pmd, RefWrap && ref )
    -> typename std::enable_if<
        !std::is_function<T>::value
        && is_reference_wrapper<typename std::decay<RefWrap>::type>::value
        , decltype(ref.get().*pmd)>::type
{
    return std::forward<RefWrap>(ref.get()).*pmd;
}

template< typename Base, typename T, typename Ptr>
auto INVOKE( T Base::*pmd, Ptr && ptr )
    -> typename std::enable_if<
        !std::is_function<T>::value
        && !is_reference_wrapper<typename std::decay<Ptr>::type>::value
        && !std::is_base_of<Base, typename std::decay<Ptr>::type>::value
        , decltype((*std::forward<Ptr>(ptr)).*pmd)>::type
{
    return (*std::forward<Ptr>(ptr)).*pmd;
}

template< typename F, typename... Args>
auto INVOKE( F && f, Args &&... args )
 -> typename std::enable_if<
    !std::is_member_pointer<typename std::decay<F>::type>::value
    , decltype(std::forward<F>(f)(std::forward<Args>(args)...))>::type
{
    return std::forward<F>( f )( std::forward<Args>( args )...);
}

#endif // invoke_USE_ALTERNATE_IMPL

} // namespace detail

// conforming C++14 implementation (is also a valid C++11 implementation):

namespace detail {

template< typename AlwaysVoid, typename, typename...>
struct invoke_result {};

template< typename F, typename... Args >
struct invoke_result< decltype( void(detail::INVOKE(std::declval<F>(), std::declval<Args>()...)) ), F, Args...>
{
    using type = decltype( detail::INVOKE(std::declval<F>(), std::declval<Args>()...) );
};

//template< typename Fn, typename... ArgTypes >
//struct is_nothrow_invocable : std::false_type{};;

} // namespace detail

template< typename > struct result_of;

template< typename F, typename... ArgTypes >
struct result_of< F(ArgTypes...) > : detail::invoke_result< void, F, ArgTypes...> {};

template< typename F, typename... ArgTypes >
struct invoke_result : detail::invoke_result< void, F, ArgTypes...> {};

template< typename F, typename... Args >
typename invoke_result< F, Args...>::type
invoke( F && f, Args &&... args )
// noexcept( detail::is_nothrow_invocable<F, Args...>::value )
{
    return detail::INVOKE( std::forward<F>( f ), std::forward<Args>( args )...);
}

} // namespace nonstd

#else // not C++17, not C++11 - invoke()

#pragma message ("*** Using nonstd::invoke - C++98")

//
// C++98, code based on proposal n1454.
//
// N1454. A uniform method for computing function object return types (revision 1),
// by Douglas Gregor, http://wg21.link/n1454

namespace nonstd {
namespace std98 {

// type traits:

template< typename T, T v >
struct integral_constant
{
   typedef integral_constant< T, v > type;
   typedef T value_type;
   static const T value = v;
};

typedef integral_constant< bool, true  > true_type;
typedef integral_constant< bool, false > false_type;

//template< typename T > struct remove_reference     { typedef T type; };
//template< typename T > struct remove_reference<T&> { typedef T type; };
////template< typename T > struct remove_reference<T&&> {typedef T type;};
//
//template< typename T > struct add_lvalue_reference     { typedef T& type; };
//template< typename T > struct add_lvalue_reference<T&> { typedef T& type; };

template< typename T > struct remove_const          { typedef T type; };
template< typename T > struct remove_const<const T> { typedef T type; };

template< typename T > struct remove_volatile             { typedef T type; };
template< typename T > struct remove_volatile<volatile T> { typedef T type; };

template< typename T > struct remove_cv { typedef typename remove_volatile<typename remove_const<T>::type>::type type; };

template< typename T > struct is_const          : false_type {};
template< typename T > struct is_const<const T> : true_type {};

template< typename T > struct is_pointer_helper     : false_type {};
template< typename T > struct is_pointer_helper<T*> : true_type {};
template< typename T > struct is_pointer : is_pointer_helper<typename remove_cv<T>::type> {};

template< typename T >              struct is_member_pointer_helper : false_type {};
template< typename T , typename U > struct is_member_pointer_helper<T U::*> : true_type {};
template< typename T >              struct is_member_pointer : is_member_pointer_helper<typename remove_cv<T>::type> {};

template< bool B, typename T = void > struct enable_if {};
template<         typename T        > struct enable_if< true, T > { typedef T type; };

// result type deduction:

namespace detail {

template< typename F >
struct forwardN;

} // namespace detail

template< typename F >
struct result_of;

template< typename F >
struct result_of<F()> { typedef typename detail::forwardN<F>::template result<F>::type type; };

template< typename F, typename T1 >
struct result_of<F(T1)> { typedef typename detail::forwardN<F>::template result<T1>::type type; };

template< typename F, typename T1, typename T2 >
struct result_of<F(T1,T2)> { typedef typename detail::forwardN<F>::template result<T1,T2>::type type; };

namespace detail {

template< typename F >
struct forwardN
{
    F f;

    template< typename T> struct result { typedef T type; };
    template< typename R> struct result<R (*)() > { typedef R type; };
//    template< typename R, typename C > struct result< R (C::*)() > { typedef R type; };

    template< typename T, typename T1 > struct result<       T(T1) > { typedef typename result_of<       F( T1 ) >::type type; };
    template< typename T, typename T1 > struct result< const T(T1) > { typedef typename result_of< const F( T1 ) >::type type; };

    template< typename T, typename T1, typename T2 > struct result<       T(T1, T2) > { typedef typename result_of<       F( T1, T2 ) >::type type; };
    template< typename T, typename T1, typename T2 > struct result< const T(T1, T2) > { typedef typename result_of< const F( T1, T2 ) >::type type; };

    template< typename T1> typename result<       forwardN(T1) >::type operator()( T1 & t1 )       { return f( t1 ); }
    template< typename T1> typename result< const forwardN(T1) >::type operator()( T1 & t1 ) const { return f( t1 ); }

    template< typename T1, typename T2> typename result<       forwardN(T1, T2) >::type operator()( T1 & t1, T2 & t2 )       { return f( t1, t2 ); }
    template< typename T1, typename T2> typename result< const forwardN(T1, T2) >::type operator()( T1 & t1, T2 & t2 ) const { return f( t1, t2 ); }
};

} // namespace detail

// limited set of invoke():
// - 0..2 arguments
// - no mix of const and non-const reference arguments
// - ...

template< typename F>
typename result_of<F()>::type
invoke( F f ) { return f(); }

template< typename F, typename T1 >
typename enable_if< !is_member_pointer<F>::value, typename result_of<F(T1)>::type >::type
invoke( F f, T1 const & t1 ) { return f( t1 ); }

template< typename F, typename T1, typename T2, int >
typename enable_if< !is_member_pointer<F>::value, typename result_of<F(T1,T2)>::type >::type
invoke( F f, T1 const & t1, T2 const & t2 ) { return f( t1, t2 ); }

template< typename Base, typename T, typename Obj>
typename enable_if< !is_pointer<Obj>::value && !is_const<Obj>::value, T& >::type
invoke( T Base::* pmd, Obj & ref ) { return ref.*pmd; }

template< typename Base, typename T, typename Obj>
typename enable_if< !is_pointer<Obj>::value && is_const<Obj>::value, T >::type
invoke( T const Base::* pmd, Obj & ref ) { return ref.*pmd; }

template< typename Base, typename T, typename Obj>
typename enable_if< !is_const<Obj>::value, T& >::type
invoke( T Base::* pmd, Obj * ptr ) { return (*ptr).*pmd; }

template< typename Base, typename T, typename Obj>
typename enable_if< is_const<Obj>::value, T >::type
invoke( T const Base::*pmd, Obj * ptr ) { return (*ptr).*pmd; }

template< typename R, typename Base, typename Obj> R invoke( R (Base::*pmf)() const, Obj const & ref )  { return (ref.*pmf)(); }
template< typename R, typename Base, typename Obj> R invoke( R (Base::*pmf)()      , Obj       & ref )  { return (ref.*pmf)(); }

template< typename R, typename Base, typename Obj> R invoke( R (Base::*pmf)() const, Obj       * ptr )  { return ((*ptr).*pmf)(); }
template< typename R, typename Base, typename Obj> R invoke( R (Base::*pmf)()      , Obj       * ptr )  { return ((*ptr).*pmf)(); }

template< typename R, typename Base, typename Obj, typename T1 > R invoke( R (Base::*pmf)(T1) const, Obj const & ref, T1 const & t1 ) { return (ref.*pmf)( t1 ); }
template< typename R, typename Base, typename Obj, typename T1 > R invoke( R (Base::*pmf)(T1)      , Obj       & ref, T1 const & t1 ) { return (ref.*pmf)( t1 ); }
template< typename R, typename Base, typename Obj, typename A1
                                                 , typename T1 > R invoke( R (Base::*pmf)(A1)      , Obj       & ref, T1       & t1 ) { return (ref.*pmf)( t1 ); }

template< typename R, typename Base, typename Obj, typename T1 > R invoke( R (Base::*pmf)(T1) const, Obj const * ptr, T1 const & t1 ) { return ((*ptr).*pmf)( t1 ); }
template< typename R, typename Base, typename Obj, typename T1 > R invoke( R (Base::*pmf)(T1)      , Obj       * ptr, T1 const & t1 ) { return ((*ptr).*pmf)( t1 ); }
template< typename R, typename Base, typename Obj, typename A1
                                                 , typename T1 > R invoke( R (Base::*pmf)(A1)      , Obj       * ptr, T1       & t1 ) { return ((*ptr).*pmf)( t1 ); }

template< typename R, typename Base, typename Obj, typename T1, typename T2 > R invoke( R (Base::*pmf)(T1,T2) const, Obj const & ref, T1 const & t1 , T2 const & t2 ) { return (ref.*pmf)( t1, t2 ); }
template< typename R, typename Base, typename Obj, typename T1, typename T2 > R invoke( R (Base::*pmf)(T1,T2)      , Obj       & ref, T1 const & t1 , T2 const & t2 ) { return (ref.*pmf)( t1, t2 ); }
template< typename R, typename Base, typename Obj, typename A1, typename A2
                                                 , typename T1, typename T2 > R invoke( R (Base::*pmf)(A1,A2)      , Obj       & ref, T2       & t1 , T2       & t2 ) { return (ref.*pmf)( t1, t2 ); }

template< typename R, typename Base, typename Obj, typename T1, typename T2 > R invoke( R (Base::*pmf)(T1,T2) const, Obj const * ptr, T1 const & t1 , T2 const & t2 ) { return ((*ptr).*pmf)( t1, t2 ); }
template< typename R, typename Base, typename Obj, typename T1, typename T2 > R invoke( R (Base::*pmf)(T1,T2)      , Obj       * ptr, T1 const & t1 , T2 const & t2 ) { return ((*ptr).*pmf)( t1, t2 ); }
template< typename R, typename Base, typename Obj, typename A1, typename A2
                                                 , typename T1, typename T2 > R invoke( R (Base::*pmf)(A1,A2)      , Obj       * ptr, T2       & t1 , T2       & t2 ) { return ((*ptr).*pmf)( t1, t2 ); }

}} // namespace nonstd::std98

// Bring C++98 invoke() into scope nonstd:

namespace nonstd {

using std98::invoke;

}


#endif // invoke_CPP17_OR_GREATER, invoke_CPP11_OR_GREATER - invoke()

//
// nonstd::apply():
//

// C++11 implementation contributed by Peter Featherstone, @pfeatherstone.

#if invoke_USES_STD_INVOKE

#pragma message ("*** Using std::apply.")

#include <tuple>

using std::apply;

namespace nonstd {
    using std::apply;
}

#elif invoke_CPP11_OR_GREATER

#pragma message ("*** Using nonstd::invoke - C++11")

#include <array>

namespace nonstd { namespace apply_lite {

#if invoke_CPLUSPLUS < 201402L

#pragma message ("*** Defining index_sequence etc.")

template< std::size_t... Ints >
struct index_sequence
{
    using type       = index_sequence;
    using value_type = std::size_t;

    static constexpr std::size_t size() noexcept { return sizeof...(Ints); }
};

template< class Sequence1, class Sequence2 >
struct merge_and_renumber;

template< std::size_t... I1, std::size_t... I2 >
struct merge_and_renumber< index_sequence<I1...>, index_sequence<I2...>>
    : index_sequence<I1..., (sizeof...(I1) + I2)...> {};

template<std::size_t N>
struct make_index_sequence
    : merge_and_renumber<
        typename make_index_sequence < N / 2 >::type,
        typename make_index_sequence < N - N / 2 >::type > {};

template<> struct make_index_sequence<0> : index_sequence<> {};
template<> struct make_index_sequence<1> : index_sequence<0> {};

template<typename... Ts>
using index_sequence_for = make_index_sequence<sizeof...(Ts)>;

#else // invoke_CPLUSPLUS < 201402L

using std::index_sequence;
using std::make_index_sequence;
using std::index_sequence_for;

#endif // invoke_CPLUSPLUS < 201402L

#pragma message ("*** Using nonstd::apply.")

namespace detail {

template< typename F, typename Tuple, std::size_t... I >
auto apply_impl( F&& fn, Tuple && tpl, index_sequence<I...> )
    -> decltype( invoke( std::forward<F>(fn), std::get<I>(std::forward<Tuple>(tpl) )...) )
{
    return invoke( std::forward<F>(fn), std::get<I>( std::forward<Tuple>(tpl) )...);
}

} // namespace detail

template< typename F, typename Tuple >
auto apply( F&& fn, Tuple && tpl )
    -> decltype(
        detail::apply_impl(
            std::forward<F>( fn  )
            , std::forward<Tuple>( tpl )
            , make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type>::value>{}
        )
    )
{
    return detail::apply_impl(
        std::forward<F>(fn)
        , std::forward<Tuple>(tpl)
        , make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type >::value>{}
    );
}

}} // namespace nonstd::apply_lite

// Bring apply() into scope nonstd:

namespace nonstd {

using apply_lite::apply;

}

#else // invoke_CPP11_OR_GREATER - apply()

#pragma message ("*** nonstd::apply(): No implementation for C++98.")

#endif // invoke_USES_STD_INVOKE - apply()

#endif // NONSTD_INVOKE_LITE_HPP
