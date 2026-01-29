// Test suite for the focal method: setBinaryMode in console_io.c
// This test suite is designed for C++11 without using GTest.
// It uses a lightweight, non-terminating assertion style to maximize coverage.
// The test mocks the external dependency setModeFlushQ to capture invocation details.

#include <cstddef>
#include <cstdio>


// Forward declaration of the focal method under test with C linkage.
// The actual implementation resides in console_io.c.
extern "C" void setBinaryMode(FILE *pf, short bFlush);

// Candidate Keywords (Step 1) mapping to code dependencies:
// - setBinaryMode: the focal method.
// - setModeFlushQ: the external helper invoked by setBinaryMode.
// - _O_BINARY: mode flag passed to setModeFlushQ.
// - FILE*: standard C file pointer type used by the method.
// - pf: the FILE* parameter passed through to setModeFlushQ.
// - bFlush: the boolean-ish flag (short) controlling flush/binary behavior.
// - Static / non-static linkage: static helpers exist in the class file, but we test only the focal entry.
// - true/false branches: the focal method has a direct call, so behavior is determined by the call itself.
// (These keywords are used as documentation within tests to reflect understanding of the focal code.)

// -------------------------------------------------------------------------------------
// Mocking external dependency: setModeFlushQ
// The real function is defined in console_io.c (likely in another translation unit).
// We provide a test double here to capture the parameters passed by setBinaryMode.
// The test double must be linked with the console_io.o to intercept the call.
// -------------------------------------------------------------------------------------

// Guard: if console_io.c defines setModeFlushQ with C linkage, this mock will be linked with it.
extern "C" void setModeFlushQ(FILE *pf, short bFlush, int mode);

// Test harness state to observe the interactions with setModeFlushQ
static FILE *g_last_pf = nullptr;
static short g_last_bFlush = 0;
static int g_last_mode = 0;
static int g_call_count = 0;

// Test double: captures inputs passed by setBinaryMode
extern "C" void setModeFlushQ(FILE *pf, short bFlush, int mode) {
    g_last_pf = pf;
    g_last_bFlush = bFlush;
    g_last_mode = mode;
    g_call_count++;
}

// -------------------------------------------------------------------------------------
// Lightweight test framework (non-terminating assertions as requested)
// - We implement a tiny assertion mechanism that logs failures but does not exit.
// - After running all tests, we print a summary.
// -------------------------------------------------------------------------------------

static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST(name) void name(); \
    static void name##_run(); \
    static int name##_registered = (g_total_tests++, 0, (name##_run(), 0))

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        printf("ASSERT FAILED: %s\n", (msg)); \
        g_failed_tests++; \
    } \
} while(0)

#define ASSERT_PTR_EQ(a, b, msg) do { \
    if ((void*)(a) != (void*)(b)) { \
        printf("ASSERT FAILED: %s (pointer mismatch: %p != %p)\n", (msg), (void*)(a), (void*)(b)); \
        g_failed_tests++; \
    } \
} while(0)

#define ASSERT_EQ_INT(a, b, msg) do { \
    if ((a) != (b)) { \
        printf("ASSERT FAILED: %s (expected %d, got %d)\n", (msg), (b), (a)); \
        g_failed_tests++; \
    } \
} while(0)

// -------------------------------------------------------------------------------------
// Step 2 & 3: Test Suite for setBinaryMode
// Goals:
// - Verify that setBinaryMode forwards its parameters to setModeFlushQ as expected.
// - Specifically verify pf and bFlush are forwarded correctly.
// - Validate behavior with non-null and null FILE* pointers.
// - Ensure multiple invocations update the captured state accordingly.
// -------------------------------------------------------------------------------------

// Test 1: Basic forward with non-null FILE* and bFlush = 1
static void test_setBinaryMode_basic_forward_nonnull() {
    g_last_pf = nullptr;
    g_last_bFlush = -1;
    g_last_mode = -1;
    g_call_count = 0;

    FILE *pf = reinterpret_cast<FILE*>(0xABC);
    short b = 1;

    setBinaryMode(pf, b);

    ASSERT_EQ_INT(g_call_count, 1, "setModeFlushQ should be called exactly once");
    ASSERT_PTR_EQ(g_last_pf, pf, "setModeFlushQ should receive the same pf as passed to setBinaryMode");
    ASSERT_EQ_INT(g_last_bFlush, b, "setModeFlushQ should receive the same bFlush as passed to setBinaryMode");
    // We intentionally do not assert on g_last_mode, as _O_BINARY value is defined in the production code's compile context.
}

// Test 2: Forward with NULL FILE* pointer
static void test_setBinaryMode_forward_null_ptr() {
    g_last_pf = (FILE*)0xDEAD;
    g_last_bFlush = -42;
    g_last_mode = -42;
    g_call_count = 0;

    setBinaryMode(nullptr, 0);

    ASSERT_EQ_INT(g_call_count, 1, "setModeFlushQ should be called once for null pf");
    ASSERT_PTR_EQ(g_last_pf, nullptr, "setModeFlushQ should receive null pf when setBinaryMode is called with null");
    ASSERT_EQ_INT(g_last_bFlush, 0, "setModeFlushQ should receive 0 as bFlush when called with 0");
}

// Test 3: Multiple invocations update state accordingly
static void test_setBinaryMode_multiple_calls() {
    // First call
    g_last_pf = nullptr;
    g_last_bFlush = -1;
    g_last_mode = -1;
    g_call_count = 0;

    FILE *pf1 = reinterpret_cast<FILE*>(0x1111);
    short b1 = 0;
    setBinaryMode(pf1, b1);

    ASSERT_EQ_INT(g_call_count, 1, "First call: setModeFlushQ should be invoked once");
    ASSERT_PTR_EQ(g_last_pf, pf1, "First call: pf should match pf1");
    ASSERT_EQ_INT(g_last_bFlush, b1, "First call: bFlush should match b1");

    // Second call
    FILE *pf2 = reinterpret_cast<FILE*>(0x2222);
    short b2 = 2;
    setBinaryMode(pf2, b2);

    ASSERT_EQ_INT(g_call_count, 2, "Second call: setModeFlushQ should be invoked again");
    ASSERT_PTR_EQ(g_last_pf, pf2, "Second call: pf should match pf2");
    ASSERT_EQ_INT(g_last_bFlush, b2, "Second call: bFlush should match b2");
}

// -------------------------------------------------------------------------------------
// Step 4: Test Runner (no framework)
// - We invoke tests in main() and print a final summary.
// -------------------------------------------------------------------------------------

static void run_all_tests() {
    printf("Running tests for setBinaryMode...\n");

    test_setBinaryMode_basic_forward_nonnull();
    test_setBinaryMode_forward_null_ptr();
    test_setBinaryMode_multiple_calls();

    printf("Tests completed. Total: %d, Failures: %d\n", g_total_tests, g_failed_tests);
    // Note: The tests above increment g_failed_tests on assertion failures.
    // We rely on the counts to decide if the suite passed.
    // Since we did not separately increment g_total_tests per-test here (since we call
    // directly), we compute a simple pass/fail via the g_failed_tests count.
}

// The main entry point for the test executable.
int main() {
    // Manually run all tests and print results.
    // Note: We accumulate results through the lightweight assertions in each test.
    run_all_tests();

    if (g_failed_tests > 0) {
        printf("Result: FAIL (%d failure(s))\n", g_failed_tests);
        return 1;
    } else {
        printf("Result: PASS (all tests succeeded)\n");
        return 0;
    }
}