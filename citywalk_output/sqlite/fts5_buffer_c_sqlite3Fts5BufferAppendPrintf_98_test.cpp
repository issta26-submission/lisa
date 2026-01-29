// FTS5BufferPrintf unit-test suite (C++11, no GTest)
// This test suite targets the focal method:
//   void sqlite3Fts5BufferAppendPrintf(int *pRc, Fts5Buffer *pBuf, char *zFmt, ...)
// It uses lightweight, in-repo style stubs for external dependencies to enable
// pure unit testing without a full SQLite dependency set.
//
// The test harness is intentionally minimal and non-terminating: it reports per-test
// outcomes and continues execution to maximize coverage.

#include <fts5Int.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <string>
#include <iostream>
#include <cstdlib>


// Domain knowledge and test strategy notes (embedded as comments for maintainability):
// - We exercise three code paths:
//   1) *pRc != SQLITE_OK -> function should do nothing (no VMPRINTF, no append).
//   2) *pRc == SQLITE_OK and sqlite3_vmprintf returns non-NULL -> append string and free temp.
//   3) *pRc == SQLITE_OK and sqlite3_vmprintf returns NULL -> set *pRc = SQLITE_NOMEM.
// - We rely on tiny, in-test stubs for sqlite3_vmprintf, sqlite3_free, and sqlite3Fts5BufferAppendString
//   to observe internal behavior without requiring a full SQLite/fts5 environment.
// - All tests are standalone C++11 executables intended to be linked with the real
//   fts5_buffer.c (which provides sqlite3Fts5BufferAppendPrintf), using C linkage for stubs.

extern "C" {

// Forward declaration for opaque C type used by the focal function.
// We treat Fts5Buffer as an opaque struct to keep the test independent of its internals.
typedef struct Fts5Buffer Fts5Buffer;

// Prototyping the focal function under test (with C linkage).
void sqlite3Fts5BufferAppendPrintf(int *pRc, Fts5Buffer *pBuf, char *zFmt, ...);

// Global test-state shared by stubs to observe behavior
static std::string g_lastAppended;  // last string passed to sqlite3Fts5BufferAppendString
static int g_vmprintf_calls = 0;    // number of times sqlite3_vmprintf was invoked
static int g_freed_count = 0;       // number of times sqlite3_free was invoked
static bool g_force_null_vmprintf = false; // force vmprintf to return NULL to simulate OOM

// Minimal, test-focused stubs to satisfy the focal method's dependencies.
// These stubs are only valid for the duration of the test binary and do not attempt
// to emulate full SQLite behavior; they merely observe interactions.

void sqlite3_free(void* p) {
    if (p) {
        free(p);
        ++g_freed_count;
    }
}

char* sqlite3_vmprintf(const char* zFormat, va_list ap) {
    ++g_vmprintf_calls;
    if (g_force_null_vmprintf) {
        return nullptr; // Simulate memory allocation failure
    }
    va_list ap_copy;
    va_copy(ap_copy, ap);
    int n = vsnprintf(nullptr, 0, zFormat, ap_copy);
    va_end(ap_copy);
    if (n < 0) return nullptr;
    char* zOut = (char*)malloc(n + 1);
    if (!zOut) return nullptr;
    vsnprintf(zOut, n + 1, zFormat, ap);
    return zOut;
}

// Stub for sqlite3Fts5BufferAppendString that captures the string without
// depending on the internal layout of Fts5Buffer.
void sqlite3Fts5BufferAppendString(int* pRc, Fts5Buffer* /*pBuf*/, const char* zStr) {
    if (pRc && *pRc == 0) {
        g_lastAppended = zStr ? std::string(zStr) : std::string();
    }
}

} // extern "C"

// Small, non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define SQLITE_OK 0
#define SQLITE_NOMEM 7

// Helper to run a single test and print result
#define RUN_TEST(test_func) do { \
    std::cout << "RUN: " #test_func " ... "; \
    test_func(); \
} while(0)

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if (cond) { \
        std::cout << "[PASS] " << msg << std::endl; \
    } else { \
        ++g_failed_tests; \
        std::cout << "[FAIL] " << msg << std::endl; \
    } \
} while(0)

// Helper to reset test state between tests
static void reset_test_state() {
    g_lastAppended.clear();
    g_vmprintf_calls = 0;
    g_freed_count = 0;
    g_force_null_vmprintf = false;
}

// Test 1: When pRc == SQLITE_OK and vmprintf returns a non-null string, the string is appended
static void test_sqlite3Fts5BufferAppendPrintf_success() {
    reset_test_state();
    int rc = SQLITE_OK;
    Fts5Buffer *pBuf = nullptr; // not used by our stubs
    char fmt[] = "val=%d";
    sqlite3Fts5BufferAppendPrintf(&rc, pBuf, fmt, 7);

    // Validate expected branches and side-effects
    EXPECT_TRUE(rc == SQLITE_OK, "rc remains SQLITE_OK after successful append");
    EXPECT_TRUE(g_vmprintf_calls == 1, "vmprintf invoked exactly once");
    EXPECT_TRUE(g_lastAppended == "val=7", "sqlite3Fts5BufferAppendString received the correct string");
    EXPECT_TRUE(g_freed_count == 1, "vmprintf temp string freed exactly once");
}

// Test 2: When pRc == SQLITE_OK but vmprintf returns NULL, rc is set to SQLITE_NOMEM
static void test_sqlite3Fts5BufferAppendPrintf_oom() {
    reset_test_state();
    int rc = SQLITE_OK;
    g_force_null_vmprintf = true; // simulate OOM in vmprintf
    Fts5Buffer *pBuf = nullptr;
    char fmt[] = "ignored %d";
    sqlite3Fts5BufferAppendPrintf(&rc, pBuf, fmt, 123);

    // Validate OOM path
    EXPECT_TRUE(rc == SQLITE_NOMEM, "rc set to SQLITE_NOMEM on vmprintf NULL");
    EXPECT_TRUE(g_vmprintf_calls == 1, "vmprintf invoked once (even on failure)");
    EXPECT_TRUE(g_lastAppended.empty(), "No append occurred when vmprintf failed");
    // No extra free should be performed for the NULL case
    g_force_null_vmprintf = false;
    reset_test_state();
}

// Test 3: When pRc != SQLITE_OK, the function should do nothing (no vmprintf or append)
static void test_sqlite3Fts5BufferAppendPrintf_short_circuit() {
    reset_test_state();
    int rc = 1; // non-OK, simulate early exit
    Fts5Buffer *pBuf = nullptr;
    char fmt[] = "should %s";
    // This call should not touch vmprintf or lastAppended
    sqlite3Fts5BufferAppendPrintf(&rc, pBuf, fmt, "be ignored");

    EXPECT_TRUE(rc != SQLITE_OK, "rc remains non-zero; function short-circuits");
    EXPECT_TRUE(g_vmprintf_calls == 0, "vmprintf not invoked when rc != SQLITE_OK");
    EXPECT_TRUE(g_lastAppended.empty(), "No append occurred when rc != SQLITE_OK");
}

// Main: run all tests
int main() {
    std::cout << "Starting sqlite3Fts5BufferAppendPrintf unit tests (C++11, no GTest)" << std::endl;

    RUN_TEST(test_sqlite3Fts5BufferAppendPrintf_success);
    RUN_TEST(test_sqlite3Fts5BufferAppendPrintf_oom);
    RUN_TEST(test_sqlite3Fts5BufferAppendPrintf_short_circuit);

    std::cout << "Test summary: " << g_total_tests << " executed, "
              << g_failed_tests << " failed." << std::endl;

    // Return non-zero if any test failed
    return g_failed_tests > 0 ? 1 : 0;
}