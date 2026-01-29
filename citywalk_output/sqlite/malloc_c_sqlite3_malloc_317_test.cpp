/* File: test_sqlite3_malloc_autoinit.cpp
   Purpose: Validate sqlite3_malloc behavior when AUTOINIIT is enabled (default path).
   Focus: 
   - true/false branches of sqlite3_initialize() outcome
   - memory allocation path when n <= 0 vs n > 0
   - uses real sqlite3Malloc from malloc.c for positive allocations
   Notes:
   - This test compiles in a project where malloc.c (providing sqlite3_malloc and sqlite3_malloc) is part of the build.
   - We stub sqlite3_initialize to control behavior without altering production code.
   - We do not mock sqlite3Malloc (we rely on the real allocator from malloc.c).
*/
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


// Lightweight test harness: non-terminating assertions
static int g_tot_autoinit = 0;
static int g_fail_autoinit = 0;

#define AUT_ASSERT(cond, msg) do { \
    ++g_tot_autoinit; \
    if(!(cond)) { \
        ++g_fail_autoinit; \
        std::cerr << "[AUTOINIT TEST FAIL] " << msg << std::endl; \
    } \
} while(0)

// Prototypes for integration with the production code under test (malloc.c)
extern "C" void *sqlite3_malloc(int n); // focal method under test
// The production code uses sqlite3_initialize() before sqlite3Malloc. We stub this to control outcomes.
extern "C" int sqlite3_initialize(void);

// We provide a test-controlled implementation of sqlite3_initialize.
// The linker will resolve this symbol in the same program where sqlite3_malloc is linked.
static int g_init_result_autoinit = 0;
extern "C" int sqlite3_initialize(void) {
    return g_init_result_autoinit;
}

// Also declare the real allocator symbol to ensure linkage to malloc.c's sqlite3Malloc
extern "C" void *sqlite3Malloc(unsigned long long n);

// Helper: run all tests for AUTOINIT path
static void run_autoinit_tests() {
    std::cout << "Running sqlite3_malloc AUTOINIT path tests...\n";

    // Case 1: Initialization succeeds (sqlite3_initialize returns 0), negative input
    g_init_result_autoinit = 0;
    int r1 = -5;
    // n <= 0 should yield 0 regardless of allocator state
    void *res_neg = sqlite3_malloc(r1);
    AUT_ASSERT(res_neg == 0, "sqlite3_malloc should return 0 for negative input when AUTOINIT succeeds");

    // Case 2: Initialization succeeds (sqlite3_initialize returns 0), zero input
    res_neg = sqlite3_malloc(0);
    AUT_ASSERT(res_neg == 0, "sqlite3_malloc should return 0 for zero input when AUTOINIT succeeds");

    // Case 3: Initialization succeeds, positive input -> should call sqlite3Malloc and return non-null
    g_init_result_autoinit = 0;
    void *res_pos = sqlite3_malloc(10);
    AUT_ASSERT(res_pos != 0, "sqlite3_malloc should return non-null pointer when AUTOINIT succeeds and n>0");

    // Case 4: Initialization fails (sqlite3_initialize returns non-zero) -> should return 0
    g_init_result_autoinit = 1;
    void *res_fail = sqlite3_malloc(10);
    AUT_ASSERT(res_fail == 0, "sqlite3_malloc should return 0 when AUTOINIT fails (sqlite3_initialize non-zero)");
}

int main() {
    run_autoinit_tests();

    // Summary for AUTOINIT tests
    std::cout << "AUTOINIT tests run: " << g_tot_autoinit << ", failures: " << g_fail_autoinit << std::endl;
    // Return non-zero if any test failed
    return g_fail_autoinit != 0;
}
*/

/* File: test_sqlite3_malloc_no_autoinit.cpp
   Purpose: Validate sqlite3_malloc behavior when AUTOINIT is omitted (SQLITE_OMIT_AUTOINIT defined).
   Approach:
   - Compile-time macro SQLITE_OMIT_AUTOINIT is defined (via -DSQLITE_OMIT_AUTOINIT)
   - sqlite3_malloc should bypass initialization and directly use sqlite3Malloc for n > 0
   - No explicit mocking of sqlite3Malloc is performed; rely on malloc.c implementation
*/

// Lightweight test harness: non-terminating assertions
static int g_tot_noauto = 0;
static int g_fail_noauto = 0;

#define NOAUTO_ASSERT(cond, msg) do { \
    ++g_tot_noauto; \
    if(!(cond)) { \
        ++g_fail_noauto; \
        std::cerr << "[NO_AUTOINIT TEST FAIL] " << msg << std::endl; \
    } \
} while(0)

// Prototypes for integration with the production code under test (malloc.c)
extern "C" void *sqlite3_malloc(int n); // focal method under test
// With SQLITE_OMIT_AUTOINIT defined, no sqlite3_initialize call should be made.
extern "C" void *sqlite3Malloc(unsigned long long n);

// Helper to run tests for NO_AUTOINIT path
static void run_no_autoinit_tests() {
    std::cout << "Running sqlite3_malloc NO_AUTOINIT path tests (SQLITE_OMIT_AUTOINIT defined)...\n";

    // Case 1: n <= 0 should yield 0 without invoking initialization
    void *res0 = sqlite3_malloc(-1);
    NOAUTO_ASSERT(res0 == 0, "sqlite3_malloc should return 0 for negative input when AUTOINIT is omitted");

    void *res1 = sqlite3_malloc(0);
    NOAUTO_ASSERT(res1 == 0, "sqlite3_malloc should return 0 for zero input when AUTOINIT is omitted");

    // Case 2: n > 0 should return a non-null pointer from sqlite3Malloc
    void *res2 = sqlite3_malloc(4);
    NOAUTO_ASSERT(res2 != 0, "sqlite3_malloc should return non-null pointer for positive input when AUTOINIT is omitted");
}

int main() {
    run_no_autoinit_tests();

    // Summary for NO_AUTOINIT tests
    std::cout << "NO_AUTOINIT tests run: " << g_tot_noauto << ", failures: " << g_fail_noauto << std::endl;
    return g_fail_noauto != 0;
}