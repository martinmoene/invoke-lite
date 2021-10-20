#include "invoke-main.t.hpp"

namespace {



int f42()
{
    return 42;
}

int f42_nef() invoke_noexcept_op( false )
{
    return 42;
}

int arg( int a  )
{
    return a;
}

int add( int a, int b )
{
    return a + b;
}

struct holder
{
    int v;

    holder( int v_)
        : v(v_)
    {}

    int val() const
    {
        return v;
    }

    int add( int a ) const
    {
        return v + a;
    }

    int add_constref( int const & a ) const
    {
        return v + a;
    }

    int add_nonconst( int a )
    {
        return v + a;
    }
};

struct functor
{
    int operator()() const
    {
        return 42;
    }

    int operator()(int i) const
    {
        return i;
    }
};

//
// invoke():
//

CASE("invoke: a free function, no arguments")
{
    EXPECT( nonstd::invoke( f42 ) == 42 );
}

CASE("invoke: a free function, no arguments, noexcept(false) (C++11)")
{
    EXPECT( nonstd::invoke( f42_nef ) == 42 );
}

CASE("invoke: a free function, 1 argument")
{
    EXPECT( nonstd::invoke( arg, 7 ) == 7 );
}

CASE("invoke: a free function, 2 arguments")
{
    EXPECT( nonstd::invoke( add, 1, 2 ) == 3 );
}

CASE("invoke: a member function, no arguments")
{
    const holder v(42);

    EXPECT( nonstd::invoke( &holder::val, v ) == 42 );
}

CASE("invoke: a member function, 1 argument")
{
    const holder v(1);

    EXPECT( nonstd::invoke( &holder::add, v, 2 ) == 3 );
}

CASE("invoke: a member function, 1 const ref argument")
{
    const holder v(1);

    EXPECT( nonstd::invoke( &holder::add_constref, v, 2 ) == 3 );
}

CASE("invoke: a non-const member function, 1 argument")
{
    /*const*/ holder v(1);

    EXPECT( nonstd::invoke( &holder::add_nonconst, v, 2 ) == 3 );
}

CASE("invoke: a member function, changing its reference argument")
{
    // invoke a member function, changing its reference argument:
    // Bar bar; int arg = 7;
    // std::cout << "nonstd::invoke( &Bar::change_arg, bar, arg ): "; nonstd::invoke( &Bar::change_arg, bar, arg ); std::cout << "; original arg: " << arg << "\n";
}

CASE("invoke: (access) a data member")
{
    // invoke (access) a data member:
    // std::cout << "Access member data via     const object ref: nonstd::invoke(&Foo::num_,  foo): " << nonstd::invoke(&Foo::num_,  foo) << "\n";
    // std::cout << "Access member data via     const object ptr: nonstd::invoke(&Foo::num_, &foo): " << nonstd::invoke(&Foo::num_, &foo) << "\n";
    // std::cout << "Access member data via non-const object ref: nonstd::invoke(&Foo::num_,  ncf): " << nonstd::invoke(&Foo::num_,  ncf) << "\n";
    // std::cout << "Access member data via non-const object ptr: nonstd::invoke(&Foo::num_, &ncf): " << nonstd::invoke(&Foo::num_, &ncf) << "\n";
}

CASE("invoke: change an invoked (accessed) data member:")
{
    // change an invoked (accessed) data member:
    // std::cout << "Change member data via non-const object ref: nonstd::invoke(&Foo::num_,  ncf) = 77: " << (nonstd::invoke(&Foo::num_,  ncf) = 77) << "\n";
    // std::cout << "Change member data via non-const object ptr: nonstd::invoke(&Foo::num_, &ncf) = 88: " << (nonstd::invoke(&Foo::num_, &ncf) = 88) << "\n";
}

CASE("invoke: a function object, no arguments")
{
    EXPECT( nonstd::invoke( functor() ) == 42 );
}

CASE("invoke: a function object, 1 argument")
{
    EXPECT( nonstd::invoke( functor(), 42 ) == 42 );
}

CASE("invoke: a lambda, no arguments")
{
    EXPECT( nonstd::invoke( [](){ return 42; } ) == 42 );
}

CASE("invoke: a lambda, 1 argument")
{
    EXPECT( nonstd::invoke( [](int a){ return a; }, 42 ) == 42 );
}

//
// apply():
//

//
// Other:
//

CASE("invoke: App" "[.invoke][.app]")
{
    std::cout << "App: ...\n";
}

CASE( "tweak header: Reads tweak header if supported " "[tweak]" )
{
#if invoke_HAVE_TWEAK_HEADER
    EXPECT( invoke_TWEAK_VALUE == 42 );
#else
    EXPECT( !!"Tweak header is not available (invoke_HAVE_TWEAK_HEADER: 0)." );
#endif
}

} // anonymous namespace
