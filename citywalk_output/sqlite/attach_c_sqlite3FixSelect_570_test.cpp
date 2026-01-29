// Test suite for sqlite3FixSelect (forwarding wrapper) using a minimal
// mock of sqlite3WalkSelect to verify delegation behavior.
// This test is designed to be compiled together with the attach.c source
// that contains sqlite3FixSelect. It uses the real SQLite internal types
// (DbFixer, Walker, Select) from sqliteInt.h.
// No GoogleTest is used; tests are run from main() with non-terminating
// assertions to maximize coverage.

#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Bring in SQLite internal type declarations used by sqlite3FixSelect.
// The include path should point to the project's SQLite internal headers.
extern "C" {
}

// Forward declaration of the function under test (as provided by attach.c)
extern "C" int sqlite3FixSelect(DbFixer *pFix, Select *pSelect);

// Mock of sqlite3WalkSelect to observe forwarding behavior.
// We implement this in the test translation unit so the linker can resolve
// sqlite3FixSelect's call to this mock during testing.
static void* g_lastWalkerPtr = nullptr;
static void* g_lastSelectPtr = nullptr;
static int g_fakeWalkReturn = 0;

// Expose a C-compatible mock function for sqlite3WalkSelect.
// It records the received arguments and returns a configurable value.
extern "C" int sqlite3WalkSelect(Walker* w, Select* s) {
    g_lastWalkerPtr = (void*)w;
    g_lastSelectPtr = (void*)s;
    return g_fakeWalkReturn;
}

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST FAIL: " << (msg) << " [" << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
        ++g_failures; \
    } \
} while(0)


// Test 1: Verify that sqlite3FixSelect forwards the address of pFix->w
// and the pSelect argument to sqlite3WalkSelect, and that the return value
// from sqlite3WalkSelect is propagated back to the caller.
static void test_sqlite3FixSelect_forwards_arguments() {
    // Reset mock state
    g_lastWalkerPtr = nullptr;
    g_lastSelectPtr = nullptr;
    g_fakeWalkReturn = 0; // simulate a successful forward

    // Prepare dummy input objects (structures defined in sqliteInt.h)
    DbFixer fix;
    Select  sel;

    // Zero the input structures to ensure clean state
    std::memset(&fix, 0, sizeof(fix));
    std::memset(&sel, 0, sizeof(sel));

    // Call the function under test
    int rc = sqlite3FixSelect(&fix, &sel);

    // Validate that sqlite3WalkSelect received the exact address of fix.w and the Select object
    EXPECT_TRUE(g_lastWalkerPtr == (void*)&fix.w,
                "sqlite3FixSelect should pass address of pFix->w to sqlite3WalkSelect");
    EXPECT_TRUE(g_lastSelectPtr == (void*)&sel,
                "sqlite3FixSelect should pass the original pSelect to sqlite3WalkSelect");

    // Validate that the return value is propagated
    EXPECT_TRUE(rc == g_fakeWalkReturn,
                "sqlite3FixSelect should propagate the return value from sqlite3WalkSelect");
}

// Test 2: Verify that sqlite3FixSelect propagates non-zero return values exactly.
// This confirms that the wrapper does not swallow or alter the return value.
static void test_sqlite3FixSelect_propagates_return_value() {
    // Use a distinct pair of objects to ensure no accidental reuse from Test 1
    DbFixer fix2;
    Select  sel2;

    std::memset(&fix2, 0, sizeof(fix2));
    std::memset(&sel2, 0, sizeof(sel2));

    // Configure the mock to return a non-zero value
    g_fakeWalkReturn = -12345;

    int rc = sqlite3FixSelect(&fix2, &sel2);

    // Check that forwarding still occurred correctly
    EXPECT_TRUE(g_lastWalkerPtr == (void*)&fix2.w,
                "sqlite3FixSelect should forward address of pFix->w on second test");
    EXPECT_TRUE(g_lastSelectPtr == (void*)&sel2,
                "sqlite3FixSelect should forward pSelect on second test");

    // Check returned value consistency
    EXPECT_TRUE(rc == g_fakeWalkReturn,
                "sqlite3FixSelect should propagate non-zero return value from sqlite3WalkSelect");
}


// Entrypoint for the test suite
int main() {
    std::cout << "Starting sqlite3FixSelect unit tests (no GTest)..." << std::endl;

    test_sqlite3FixSelect_forwards_arguments();
    test_sqlite3FixSelect_propagates_return_value();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}