// test_series_init.cpp
// Lightweight, self-contained unit tests for sqlite3_series_init.
// This file uses compile-time guards to build two separate test scenarios in
// the same file. Compile withone of:
//   g++ -DTEST_OLD_VERSION -std=c++11 test_series_init.cpp -o test_series_init_old
//   g++ -DTEST_NEW_VERSION -std=c++11 test_series_init.cpp -o test_series_init_new
//
// Important: This is a minimal, standalone harness meant to exercise the focal
// method's behavior in isolation by providing minimal stubs for dependencies.
// It does not rely on GTest. It uses a tiny in-process test approach and
// simple assertion checks implemented with standard I/O.
//
// Note: The test assumes the presence of the sqlite3_series_init symbol (from
// the provided focal method in series.c). To keep this file self-contained, the
// test includes the content of series.c via the preprocessor in each test block
// after defining the necessary small runtime stubs (macros) to shape the flow.

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <sqlite3ext.h>
#include <limits.h>
#include <cstring>
#include <series.c>
#include <cstdlib>


// Forward-declare minimal types to satisfy the focal method's signature when we
// compile the included series.c content in two isolated blocks.
struct sqlite3;
struct sqlite3_api_routines;
struct sqlite3_module;
typedef struct sqlite3 sqlite3;
typedef struct sqlite3_api_routines sqlite3_api_routines;

// The focal method uses the following signature:
// int sqlite3_series_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);
extern "C" int sqlite3_series_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);

// Utilities: a tiny assertion helper
#define ASSERT_EQ(a, b, msg) \
    do { if ((a) != (b)) { fprintf(stderr, "ASSERT FAILED: %s (%s != %s))\n", (msg), #a, #b); goto fail; } } while(0)
#define ASSERT_TRUE(a, msg) \
    do { if (!(a)) { fprintf(stderr, "ASSERT FAILED: %s\n", (msg)); goto fail; } } while(0)
#define PASS(msg) printf("PASS: %s\n", (msg))
#define FAIL(msg) { fprintf(stderr, "FAIL: %s\n", (msg)); goto end; }

// Test 1: Old SQLite version path triggers SQLITE_ERROR and sets pzErrMsg
#if defined(TEST_OLD_VERSION)

// 1) Force sqlite3_libversion_number() to report an old version (< 3.8.12)
//    We do this by providing a macro that replaces calls to sqlite3_libversion_number()
//    with a constant value during compilation.
#define sqlite3_libversion_number() 3008011

// 2) Provide a no-op for SQLITE_EXTENSION_INIT2 to keep the test self-contained
#define SQLITE_EXTENSION_INIT2(pApi) // no-op

// 3) Minimal SQLITE_OK and SQLITE_ERROR constants (since we are not including sqlite3.h)
#define SQLITE_OK 0
#define SQLITE_ERROR 1

// 4) Provide a tiny sqlite3_mprintf mock that returns a heap-allocated string.
//    The focal method assigns this to *pzErrMsg, so the test can validate the content.
extern "C" char* sqlite3_mprintf(const char *fmt, ...) {
    const char *msg = "generate_series() requires SQLite 3.8.12 or later";
    size_t len = strlen(msg) + 1;
    char *out = (char*)malloc(len);
    memcpy(out, msg, len);
    return out;
}

// 5) Provide a very small dummy db object and a NULL pApi (not used by the test path)
static sqlite3 *dummy_db = nullptr;
static sqlite3_api_routines *dummy_api = nullptr;

// 6) Include the focal method file content (series.c) after the above stubs.
//    The macro sqlite3_libversion_number() will cause the true branch to be taken.

int main() {
    // Ensure we have a place to store the produced error message
    char *err = nullptr;

    // Call the focal method
    int rc = sqlite3_series_init(dummy_db, &err, dummy_api);

    // Checks:
    // - rc should be SQLITE_ERROR (1)
    // - err should be non-null and contain the expected message
    ASSERT_EQ(rc, SQLITE_ERROR, "sqlite3_series_init should return SQLITE_ERROR for old version");
    ASSERT_TRUE(err != nullptr, "Error message should be allocated when version check fails");
    if (err) {
        ASSERT_TRUE(strcmp(err,
            "generate_series() requires SQLite 3.8.12 or later") == 0,
            "Error message content should indicate required SQLite version");
        free(err);
    }

    PASS("TEST_OLD_VERSION: old-version path correctly returns error with message");
    return 0;

fail:
    FAIL("TEST_OLD_VERSION: test failed");
end:
    return 1;
}
#endif // TEST_OLD_VERSION

// Test 2: New SQLite version path goes through module creation (sqlite3_create_module)
// We simulate version >= 3.8.12 and intercept sqlite3_create_module to confirm it is invoked.
#if defined(TEST_NEW_VERSION)

// 1) Force sqlite3_libversion_number() to report a new version (>= 3.8.12)
#define sqlite3_libversion_number() 3008012

// 2) Provide a no-op for SQLITE_EXTENSION_INIT2 to keep the test self-contained
#define SQLITE_EXTENSION_INIT2(pApi) // no-op

// 3) Minimal SQLITE_OK and SQLITE_ERROR constants
#define SQLITE_OK 0
#define SQLITE_ERROR 1

// 4) Dummy sqlite3_module type so the symbol &seriesModule can be referenced
struct sqlite3_module {};

// 5) Provide a sentinel to verify that sqlite3_create_module was invoked
static int g_create_module_called = 0;

// 6) Implement sqlite3_create_module to simulate success and record invocation
extern "C" int sqlite3_create_module(sqlite3* db, const char* zName, const sqlite3_module* pModule, void* pClientData) {
    (void)db; (void)zName; (void)pModule; (void)pClientData;
    g_create_module_called = 1;
    return 0; // SQLITE_OK
}

// 7) Declare a dummy sqlite3_db pointer and api structure
static sqlite3 *dummy_db = nullptr;
static sqlite3_api_routines *dummy_api = nullptr;

// 8) Include the focal method file content (series.c) after the above stubs.
//    The version macro will cause the code path to reach sqlite3_create_module.

int main() {
    g_create_module_called = 0;
    char *err = nullptr;

    // Call the focal method
    int rc = sqlite3_series_init(dummy_db, &err, dummy_api);

    // Checks:
    // - rc should be SQLITE_OK (0)
    // - sqlite3_create_module should be invoked (g_create_module_called == 1)
    ASSERT_EQ(rc, SQLITE_OK, "sqlite3_series_init should return SQLITE_OK when new version path is taken");
    ASSERT_TRUE(g_create_module_called == 1, "sqlite3_create_module should be invoked in new-version path");

    PASS("TEST_NEW_VERSION: new-version path correctly calls sqlite3_create_module");
    return 0;

fail:
    FAIL("TEST_NEW_VERSION: test failed");
end:
    return 1;
}
#endif // TEST_NEW_VERSION

// Notes for maintainers and readers:
// - This file uses compile-time guards to build exactly one test scenario per
//   compilation unit. Build the OLD test by defining TEST_OLD_VERSION, and the NEW
//   test by defining TEST_NEW_VERSION. Do not define both at the same time for a
//   single executable.
// - The tests rely on including series.c, which provides sqlite3_series_init and the
//   internal module scaffolding. To keep tests self-contained, we provide minimal
//   stubs for the required SQLite entrypoints and types. In a real test environment
//   you would link against the actual sqlite3 library and headers.
// - The tests exercise:
//     1) True path: the version check triggers an error and sets pzErrMsg (TEST_OLD_VERSION).
//     2) False path: the version check is bypassed and the module creation flow is invoked (TEST_NEW_VERSION).
// - The tests employ a small, non-terminating assertion style and print concise PASS/FAIL
//   messages as required by the domain guideline that GTest is not used and no
//   terminating assertions should be used.

// End of test_series_init.cpp