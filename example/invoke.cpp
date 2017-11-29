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

// invoke-lite/example/invoke.cpp

#include <invoke.hpp>
#include <iostream>

struct Foo
{
    int num_;

    Foo(int num)
    : num_(num) {}

    static
    void print_stc(         )       { std::cout << "static void Foo::print_chr()\n"; }
    void print_num(         ) const { std::cout << "void Foo::print_num() const: " << num_ << "\n"; }
    void print_add( int arg ) const { std::cout << "void Foo::print_add(int arg) const: " << num_+arg << '\n'; }

//    void print_add_constref( int         arg ) const { std::cout << "void Foo::print_add(int arg) const: " << num_+arg << '\n'; }
    void print_add_constref( int const & arg ) const { std::cout << "void Foo::print_add(int const & arg) const: " << num_+arg << '\n'; }
    void print_add_nonconst( int         arg )       { std::cout << "void Foo::print_add_nonconst(int arg): " << num_+arg << '\n'; }

};

struct Bar
{
    void change_arg( int & arg )
    {
        std::cout << "void Bar::change_arg(int & arg): " << arg;
        arg = -99;
        std::cout << ", changed arg: " << arg;
    }
};


#if invoke_CPP11_OR_GREATER

void print_exc() noexcept( false )
{
    std::cout << "print_exc() noexcept(false)\n";
}
#endif

void print_noarg()
{
    std::cout << "print_noarg()\n";
}

long print_arg(int i)
{
    std::cout << "print_arg(arg): " << i << '\n'; return i;
}

struct printer
{
    int operator()(int i) const
    {
        std::cout << "printer::op()(arg): " << i << '\n'; return 0;
    }
};

int main()
{
    // invoke a free function:
#if invoke_CPP11_OR_GREATER
    std::cout << "nonstd::invoke( print_exc ): "; nonstd::invoke( print_exc );
#endif
    std::cout << "nonstd::invoke( print_noarg ): "; nonstd::invoke( print_noarg   );
    std::cout << "nonstd::invoke( print_arg, -9 ): " << nonstd::invoke( print_arg, -9 ) << "\n";

    // invoke a member function:
          Foo ncf( 3333 );
    const Foo foo( 1234 );
    const int num = 100;
    std::cout << "nonstd::invoke( &Foo::print_stc ): "; nonstd::invoke( &Foo::print_stc );
    std::cout << "nonstd::invoke( &Foo::print_num, foo ): "; nonstd::invoke( &Foo::print_num, foo );
    std::cout << "nonstd::invoke( &Foo::print_add, foo, 10  ): "; nonstd::invoke( &Foo::print_add, foo, 10 );
    std::cout << "nonstd::invoke( &Foo::print_add, foo, num ): "; nonstd::invoke( &Foo::print_add, foo, num );
#if invoke_CPP11_OR_GREATER
    std::cout << "nonstd::invoke( &Foo::print_add_constref, foo, 10 ): "; nonstd::invoke( &Foo::print_add_constref, foo, 10 );
    std::cout << "nonstd::invoke( &Foo::print_add_constref, foo, num ): "; nonstd::invoke( &Foo::print_add_constref, foo, num );
#endif
    std::cout << "nonstd::invoke( &Foo::print_add_nonconst, ncf, 1 ): "; nonstd::invoke( &Foo::print_add_nonconst, ncf, 1 );
    std::cout << "nonstd::invoke( &Foo::print_add_nonconst, ncf, num ): "; nonstd::invoke( &Foo::print_add_nonconst, ncf, num );

    // invoke a member function, changing its reference argument:
    Bar bar; int arg = 7;
    std::cout << "nonstd::invoke( &Bar::change_arg, bar, arg ): "; nonstd::invoke( &Bar::change_arg, bar, arg ); std::cout << "; original arg: " << arg << "\n";

    // invoke (access) a data member:
    std::cout << "Access member data via     const object ref: nonstd::invoke(&Foo::num_,  foo): " << nonstd::invoke(&Foo::num_,  foo) << "\n";
    std::cout << "Access member data via     const object ptr: nonstd::invoke(&Foo::num_, &foo): " << nonstd::invoke(&Foo::num_, &foo) << "\n";
    std::cout << "Access member data via non-const object ref: nonstd::invoke(&Foo::num_,  ncf): " << nonstd::invoke(&Foo::num_,  ncf) << "\n";
    std::cout << "Access member data via non-const object ptr: nonstd::invoke(&Foo::num_, &ncf): " << nonstd::invoke(&Foo::num_, &ncf) << "\n";

    // change an invoked (accessed) data member:
    std::cout << "Change member data via non-const object ref: nonstd::invoke(&Foo::num_,  ncf) = 77: " << (nonstd::invoke(&Foo::num_,  ncf) = 77) << "\n";
    std::cout << "Change member data via non-const object ptr: nonstd::invoke(&Foo::num_, &ncf) = 88: " << (nonstd::invoke(&Foo::num_, &ncf) = 88) << "\n";

    // invoke a function object:
    nonstd::invoke( printer(), 99 );

#if invoke_CPP11_OR_GREATER
    // invoke a lambda:
    nonstd::invoke( [](){ std::cout << "Lambda, print_arg( 42 ): "; print_arg( 42 ); } );
#endif
}

// MSVC 2010 (VC10) and newer
// cl -EHsc -I../include/nonstd/ invoke.cpp && invoke.exe

// GNUC, std=c++98, c++11, c++17
// g++ -std=c++98 -I../include/nonstd/ -o invoke.exe invoke.cpp && invoke.exe
