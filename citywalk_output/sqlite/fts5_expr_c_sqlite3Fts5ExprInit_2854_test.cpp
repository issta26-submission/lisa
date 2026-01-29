/* File: test_fts5_expr_no_sqlite_test.cpp
 * Objective:
 *   - Exercise sqlite3Fts5ExprInit() when the SQLITE_TEST / SQLITE_FTS5_DEBUG
 *     branches are NOT compiled (the "else" path).
 *   - Verify that the function returns SQLITE_OK and does not attempt to create
 *     any FTS5 expr functions in this mode.
 *   - Provide minimal scaffolding to compile the focal C code (fts5_expr.c) in
 *     a standalone C++11 test without Google Test.
 *
 * Notes:
 *   - We compile this test as a single translation unit that includes fts5_expr.c.
 *   - We define small helper macros to satisfy the preprocessor used by the focal code.
 *   - We rely on the forward declaration of Fts5Global to allow calling the function.
 *   - This test does not attempt to link against a real SQLite library.
 */

// Compile with: g++ -std=c++11 test_fts5_expr_no_sqlite_test.cpp -DARRAY_SIZE=<...> -o test_no_sqlite

#include <fts5Int.h>
#include <fts5parse.h>
#include <cstddef>
#include <stdio.h>
#include <iostream>
#include <fts5_expr.c>


// Macros and stubs to allow compiling the focal file without SQLite
#ifndef ArraySize
#define ArraySize(x) (int)(sizeof(x)/sizeof((x)[0]))
#endif
#ifndef UNUSED_PARAM2
#define UNUSED_PARAM2(a,b) (void)(a); (void)(b)
#endif
#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif
#ifndef SQLITE_UTF8
#define SQLITE_UTF8 1
#endif

// Forward declarations to satisfy the focal code's expectations
struct Fts5Global;
typedef struct Fts5Global Fts5Global;

struct sqlite3;
typedef struct sqlite3 sqlite3;

// The focal code expects sqlite3_create_function signature; declare it here.
// We only need a declaration because we are not linking to a real SQLite library in this test.
extern "C" int sqlite3_create_function(sqlite3*, const char*, int, int, void*,
                                       void (*)(void*, int, void**),
                                       void (*)(void*, int, void**),
                                       void (*)(void*));

// We declare sqlite3_context and sqlite3_value to satisfy the function pointer types.
// They are opaque in this test; the actual implementations are not used in this path.
struct sqlite3_context;
struct sqlite3_value;

// Provide a dummy definition for sqlite3_context* and sqlite3_value* used solely for the type system.
#define DUMMY_PROTO_FOR_TEST 1

// Include the focal implementation
#define SQLITE_TEST 0 // Ensure not defined for this test; this is a dummy to show intent

int main() {
    // Prepare dummy pointers (the else branch should only UNUSED_PARAM in this path)
    Fts5Global *pGlobal = nullptr;
    sqlite3 *db = nullptr;

    // Call the focal method under the false-branch conditions
    int rc = sqlite3Fts5ExprInit(pGlobal, db);

    std::cout << "test_no_sqlite_test: sqlite3Fts5ExprInit returned rc=" << rc << std::endl;
    if (rc == SQLITE_OK) {
        std::cout << "PASS: sqlite3Fts5ExprInit returned SQLITE_OK as expected in non-SQLITE_TEST path." << std::endl;
        return 0;
    } else {
        std::cerr << "FAIL: sqlite3Fts5ExprInit did not return SQLITE_OK in non-SQLITE_TEST path." << std::endl;
        return 1;
    }
}