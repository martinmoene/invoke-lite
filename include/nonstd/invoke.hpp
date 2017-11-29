//
// Copyright (c) 2017 Martin Moene
//
// https://github.com/martinmoene/invoke-lite
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef NONSTD_INVOKE_LITE_HPP
#define NONSTD_INVOKE_LITE_HPP

#define invoke_lite_VERSION  "0.0.0"

#define invoke_CPP11_OR_GREATER  ( __cplusplus >= 201103L )
#define invoke_CPP14_OR_GREATER  ( __cplusplus >= 201402L )
#define invoke_CPP17_OR_GREATER  ( __cplusplus >= 201703L )

#if invoke_CPP17_OR_GREATER

//
// C++17, use standard version:
//

#include <functional>

namespace nonstd {
    using std::invoke;
}

#elif invoke_CPP11_OR_GREATER

//
// C++11, code taken from http://en.cppreference.com/w/cpp/types/result_of:
//

#include <type_traits>
#include <utility>

namespace nonstd {
namespace detail {

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
typename invoke_result< F, Args...>::type invoke( F && f, Args &&... args )
// noexcept( detail::is_nothrow_invocable<F, Args...>::value )
{
    return detail::INVOKE( std::forward<F>( f ), std::forward<Args>( args )...);
}

} // namespace nonstd

#else // not C++17, not C++11

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

} // namespace std98

using namespace std98;

} // namespace nonstd

#endif // invoke_CPP17_OR_GREATER, invoke_CPP11_OR_GREATER

#endif // NONSTD_INVOKE_LITE_HPP
