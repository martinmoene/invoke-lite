#include "invoke-main.t.hpp"

namespace {

CASE("invoke: ...")
{
    EXPECT( true );
}

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
