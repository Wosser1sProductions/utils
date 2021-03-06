#ifndef UTILS_SQLITE_HPP
#define UTILS_SQLITE_HPP

#include "utils_compiler.hpp"

// Ignore warnings
HEDLEY_DIAGNOSTIC_PUSH
#if HEDLEY_MSVC_VERSION_CHECK(15,0,0)
    // TODO Ignore MSVC warnings
    //#pragma warning(disable:xxxx)
#else
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    #pragma GCC diagnostic ignored "-Wimplicit-fallthrough="
#endif

#include "external/sqlite_orm/sqlite_orm.hpp"

HEDLEY_DIAGNOSTIC_POP

namespace utils {
    /*
     *  Wrap sqlite_orm as ::sqlite
     *  Reference: https://github.com/fnc12/sqlite_orm (2020-08-18)
     *
     *  !! WARNING sqlite_orm has a bug where the connection does not close when using transaction
     *  Add `this->connection->release();` before returning in function at line 8290.
     */
    namespace sqlite = ::sqlite_orm;
}

#endif // UTILS_SQLITE_HPP
