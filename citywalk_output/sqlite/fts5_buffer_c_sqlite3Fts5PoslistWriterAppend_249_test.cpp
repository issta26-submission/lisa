// Minimal C++11 test harness for sqlite3Fts5PoslistWriterAppend
// This test does not use any external testing framework (no GTest).
// It provides lightweight test cases with explanatory comments and
// non-terminating assertions (tests print results and continue).

#include <iostream>
#include <cstdint>
#include <fts5Int.h>


// Domain-specific types (simplified for unit testing)
using i64 = int64_t;

// Mocked minimal structures to exercise the focal function
struct Fts5Buffer {
    int dummy; // unused, just to have a complete type
};

struct Fts5PoslistWriter {
    i64 iPrev;
};

// Lightweight test harness state
static bool g_growShouldFail = false;
static int  g_grow_rc = 0;
static const int SQLITE_OK = 0;

// Mocked dependency: fts5BufferGrow used by focal function
// If g_growShouldFail is true, simulate an error by returning non-zero
extern "C" int fts5BufferGrow(int *pRc, Fts5Buffer *pBuf, int nByte) {
    (void)pBuf; (void)nByte; // unused in this mock
    if (g_growShouldFail) {
        *pRc = g_grow_rc;
        return 1;  // non-zero => error path in focal function
    }
    *pRc = 0;
    return 0;      // success
}

// Mocked dependency: sqlite3Fts5PoslistSafeAppend used by focal function
// It should update the iPrev value to reflect the appended position.
extern "C" void sqlite3Fts5PoslistSafeAppend(Fts5Buffer *pBuf, i64 *piPrev, i64 iPos) {
    (void)pBuf; // unused in this mock
    if (piPrev) {
        *piPrev = iPos;
    }
}

// Focal method under test (replica of the original logic in fts5_buffer.c)
extern "C" int sqlite3Fts5PoslistWriterAppend(
    Fts5Buffer *pBuf,
    Fts5PoslistWriter *pWriter,
    i64 iPos
) {
    int rc = 0;   /* Initialized only to suppress erroneous warning from Clang */
    if( fts5BufferGrow(&rc, pBuf, 5+5+5) ) return rc;
    sqlite3Fts5PoslistSafeAppend(pBuf, &pWriter->iPrev, iPos);
    return SQLITE_OK;
}

// Simple assertion helpers (non-terminating)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define ASSERT_EQ(actual, expected, msg) do {                          \
    ++g_tests_run;                                                       \
    if ((actual) != (expected)) {                                      \
        std::cerr << "[FAIL] " << msg << " | Expected: " << (expected)  \
                  << "  Got: " << (actual) << std::endl;              \
        ++g_tests_failed;                                              \
    } else {                                                             \
        std::cout << "[PASS] " << msg << std::endl;                    \
    }                                                                    \
} while(0)

#define ASSERT_TRUE(cond, msg) do {                                      \
    ++g_tests_run;                                                       \
    if (!(cond)) {                                                       \
        std::cerr << "[FAIL] " << msg << std::endl;                    \
        ++g_tests_failed;                                              \
    } else {                                                             \
        std::cout << "[PASS] " << msg << std::endl;                    \
    }                                                                    \
} while(0)

// Test 1: Growth fails -> apk should return non-zero rc and should not call SafeAppend (iPrev unchanged)
void test_grow_failure_does_not_append() {
    // Explanation:
    // - Configure grow to fail
    g_growShouldFail = true;
    g_grow_rc = 123; // arbitrary non-zero error code

    Fts5Buffer buf;
    Fts5PoslistWriter w;
    w.iPrev = -1; // initial value should be preserved if append is skipped

    int rc = sqlite3Fts5PoslistWriterAppend(&buf, &w, 42);

    // Expect rc to be exactly the simulated error code
    ASSERT_EQ(rc, g_grow_rc, "Growth failure returns rc from fts5BufferGrow");
    // Expect iPrev to remain unchanged because SafeAppend should not be invoked
    ASSERT_EQ(w.iPrev, -1, "iPrev unchanged when grow fails");
    g_growShouldFail = false; // reset for subsequent tests
}

// Test 2: Growth succeeds -> SafeAppend should run and update iPrev to iPos
void test_growth_succeeds_updates_iPrev() {
    // Explanation:
    // - Simulate successful grow
    g_growShouldFail = false;
    // rc will be set to 0 in the mock (success)
    Fts5Buffer buf;
    Fts5PoslistWriter w;
    w.iPrev = 0; // initial value

    i64 testPos = 555;
    int rc = sqlite3Fts5PoslistWriterAppend(&buf, &w, testPos);

    // Expect success and iPrev updated to testPos
    ASSERT_EQ(rc, SQLITE_OK, "Successful append returns SQLITE_OK");
    ASSERT_EQ(w.iPrev, testPos, "iPrev updated to appended position");
}

// Test 3: Multiple appends with different iPos values to ensure correct behavior over time
void test_multiple_appends_update_correctly() {
    // Explanation:
    // - Grow succeeds
    g_growShouldFail = false;

    Fts5Buffer buf;
    Fts5PoslistWriter w;
    w.iPrev = -999; // arbitrary start

    i64 positions[] = { 1, -123, 999999, 0 };
    for (size_t i = 0; i < sizeof(positions)/sizeof(positions[0]); ++i) {
        int rc = sqlite3Fts5PoslistWriterAppend(&buf, &w, positions[i]);
        ASSERT_EQ(rc, SQLITE_OK, "Append should succeed");
        ASSERT_EQ(w.iPrev, positions[i], "iPrev matches the latest appended position");
    }
}

// Test 4: Ensure true negative and positive values are handled (sanity check)
void test_extreme_values() {
    g_growShouldFail = false;
    Fts5Buffer buf;
    Fts5PoslistWriter w;
    w.iPrev = 0;

    i64 testPos = -2147483648LL; // min 32-bit int (extreme negative)
    int rc = sqlite3Fts5PoslistWriterAppend(&buf, &w, testPos);
    ASSERT_EQ(rc, SQLITE_OK, "Append extreme negative value succeeds");
    ASSERT_EQ(w.iPrev, testPos, "iPrev matches extreme negative value");

    testPos = 2147483647LL; // max 32-bit int (extreme positive)
    rc = sqlite3Fts5PoslistWriterAppend(&buf, &w, testPos);
    ASSERT_EQ(rc, SQLITE_OK, "Append extreme positive value succeeds");
    ASSERT_EQ(w.iPrev, testPos, "iPrev matches extreme positive value");
}

// Entry point: run all tests and summarize
int main() {
    std::cout << "Running sqlite3Fts5PoslistWriterAppend unit tests (C++11, no GTest)" << std::endl;

    test_grow_failure_does_not_append();
    test_growth_succeeds_updates_iPrev();
    test_multiple_appends_update_correctly();
    test_extreme_values();

    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << std::endl;

    // Return non-zero if any test failed
    return g_tests_failed ? 1 : 0;
}