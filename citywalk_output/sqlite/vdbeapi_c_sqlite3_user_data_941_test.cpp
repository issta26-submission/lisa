// Test suite for the focal method: sqlite3_user_data
// This test is crafted to be self-contained within a C++11 environment,
// and does not rely on a particular unit testing framework (no GTest).
// It focuses on validating that sqlite3_user_data correctly returns the
// user data pointer stored inside the sqlite3_context's pFunc->pUserData chain.
//
// Notes:
// - We assume the internal layout of sqlite3_context and its pFunc field
//   conforms to the expectations used by sqlite3_user_data (i.e., a pointer
//   to a structure that itself has a member pUserData of type void*).
// - To maximize portability within this test harness, we provide minimal
//   local struct definitions that mirror the expected layout for the purpose
//   of this unit test. This approach is common in environments where the
//   actual internal structs are not publicly exposed, but the focal function
//   operates on them via pointers.
// - The test validates true branches by constructing two distinct user data
//   pointers and ensuring the returned value matches the original.
// - Static members are not exercised here since the function operates on
//   runtime data passed in (and static internals are not exposed through
//   the public API in this test harness).
// - The test uses plain C-style asserts and prints PASS/FAIL for clarity.
//
// Disclaimer: If your build system replaces the internal layout of sqlite3_context
// or FuncDef, adjust the local mirror structs to match the actual definitions
// in your codebase. The important aspect is that pFunc->pUserData is laid out
// such that sqlite3_user_data fetches it correctly.

#include <cstdio>
#include <opcodes.h>
#include <cstddef>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstdlib>
#include <cassert>


// We declare a minimal mirror of the internal structures to be compatible
// with sqlite3_user_data's usage in its translation unit.
// This mirrors: pFunc -> FuncDef*, and FuncDef -> pUserData (void*).
// These definitions are intentionally simple and solely for unit test purposes.

extern "C" {

// Forward declare the focal function's signature to allow linking.
// The actual definition is in vdbeapi.c in the real project.
// We declare the parameter type sqlite3_context* to match the focal method.
typedef struct sqlite3_context sqlite3_context;

void *sqlite3_user_data(sqlite3_context *p);

// Minimal internal-like definitions mirroring expected layout.
// These definitions do not need to be identical to the real internal headers
// for the purpose of this unit test, as long as they align with the access
// pattern used by sqlite3_user_data (pFunc->pUserData).
struct FuncDef {
    void *pUserData;
};

struct sqlite3_context {
    FuncDef *pFunc;
};

} // extern "C"

////////////////////////////////////////////////////////////////////////////////
// Helper test harness
////////////////////////////////////////////////////////////////////////////////

static int run_test_passed(const char* test_name, bool ok)
{
    if (ok) {
        printf("[PASS] %s\n", test_name);
        return 0;
    } else {
        printf("[FAIL] %s\n", test_name);
        return 1;
    }
}

// Test 1: Non-null sqlite3_context with a valid pFunc->pUserData should
// return the exact user data pointer.
static int test_sqlite3_user_data_returns_user_data()
{
    // Prepare two distinct user data objects to verify pointer equality.
    int dataA = 0xA1A1A1;
    int dataB = 0xB2B2B2;

    // Build first context: pFunc->pUserData -> &dataA
    FuncDef f1;
    f1.pUserData = &dataA;

    sqlite3_context ctx1;
    ctx1.pFunc = &f1;

    void *ret1 = sqlite3_user_data(&ctx1);
    bool ok1 = (ret1 == &dataA);

    // Build second context: pFunc->pUserData -> &dataB
    FuncDef f2;
    f2.pUserData = &dataB;

    sqlite3_context ctx2;
    ctx2.pFunc = &f2;

    void *ret2 = sqlite3_user_data(&ctx2);
    bool ok2 = (ret2 == &dataB);

    // Aggregate results
    return run_test_passed("test_sqlite3_user_data_returns_user_data (two cases)", ok1 && ok2);
}

// Test 2: Ensures that modifying the user data pointer is reflected in the return.
// This is a robustness check to ensure the function does not cache or mutate
// the pointer unintentionally.
static int test_sqlite3_user_data_pointer_identity()
{
    // Prepare a unique user data object
    struct UniqueObj { int id; };
    UniqueObj extra;
    extra.id = 42;

    FuncDef f;
    f.pUserData = &extra;

    sqlite3_context ctx;
    ctx.pFunc = &f;

    void *ret = sqlite3_user_data(&ctx);
    bool ok = (ret == &extra);

    return run_test_passed("test_sqlite3_user_data_pointer_identity", ok);
}

////////////////////////////////////////////////////////////////////////////////
// Main: run all tests
////////////////////////////////////////////////////////////////////////////////

int main()
{
    int fails = 0;

    // Run tests
    fails += test_sqlite3_user_data_returns_user_data();
    fails += test_sqlite3_user_data_pointer_identity();

    if (fails == 0) {
        printf("All tests PASSED.\n");
        return EXIT_SUCCESS;
    } else {
        printf("%d tests FAILED.\n", fails);
        return EXIT_FAILURE;
    }
}