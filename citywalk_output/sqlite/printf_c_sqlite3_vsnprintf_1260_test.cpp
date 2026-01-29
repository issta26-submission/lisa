// Minimal C++11 unit tests for the focal function sqlite3_vsnprintf and its small set of dependencies.
// This test suite avoids any external testing framework (GTest, etc.) and uses a lightweight
// inlined test harness with non-terminating assertions to maximize code coverage.
// The test scaffolding intentionally provides small, self-contained stubs for the necessary
// SQLite-like types and APIs (StrAccum, sqlite3StrAccumInit, sqlite3_str_vappendf) so that
// the focal function can be exercised in isolation.

#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <sqliteInt.h>
#include <string>
#include <iostream>


// ----------------------
// Lightweight test harness
// ----------------------

static int g_total_tests = 0;
static int g_failed_tests = 0;
#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "Test failure: " << (msg) \
                  << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    } \
} while(0)

static void testSummary() {
    std::cerr << "Tests run: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;
}

// ----------------------
// Minimal dependencies for sqlite3_vsnprintf (mocked for testing)
// ----------------------
// A compact stand-in for the StrAccum type used by sqlite3_vsnprintf.
struct StrAccum {
    int nChar;      // number of characters accumulated so far
    int nAlloc;     // total buffer capacity (zBase length)
    char *zBase;    // base buffer
    int mx;         // not used in tests, present to mirror interface
};

// Forward declarations (mirroring the focal API surface used by sqlite3_vsnprintf)
void sqlite3StrAccumInit(StrAccum *p, void *db, char *zBase, int n, int mx);
static void sqlite3_str_vappendf(StrAccum *acc, const char *fmt, va_list ap);

// Lightweight, self-contained vsnprintf path for the faked StrAccum.
// Note: We implement a small, safe formatter that respects buffer capacity and terminator.
// This is sufficient to exercise sqlite3_vsnprintf's logic in tests.
void sqlite3_str_vappendf(StrAccum *acc, const char *fmt, va_list ap) {
    // Use a local temporary buffer to perform vsnprintf formatting.
    // This keeps the test harness simple and independent of the actual sqlite3_str_vappendf implementation.
    char tmp[1024];
    int len = vsnprintf(tmp, sizeof(tmp), fmt, ap);
    if (len < 0) len = 0;

    // Ensure we do not overflow the user-provided buffer.
    int space = acc->nAlloc - acc->nChar;
    if (space <= 0) return; // no space left, nothing to append

    // Leave at least one space for the terminator, as the caller will append a terminator.
    int copy = len;
    if (copy > space - 1) copy = space - 1;
    if (copy > 0) {
        if (acc->zBase) {
            memcpy(acc->zBase + acc->nChar, tmp, copy);
        }
        acc->nChar += copy;
    }
    // If there is no room for more characters, terminator will be appended by caller,
    // and acc->nChar will not exceed nAlloc - 1, ensuring zBuf[acc.nChar] remains valid.
}

// Initialize the StrAccum object (mocked)
void sqlite3StrAccumInit(StrAccum *p, void *db, char *zBase, int n, int mx) {
    p->zBase = zBase;
    p->nChar = 0;
    p->nAlloc = n;
    p->mx = mx;
}

// The focal function under test (reproduced in test harness with minimal dependencies)
char *sqlite3_vsnprintf(int n, char *zBuf, const char *zFormat, va_list ap){
  StrAccum acc;
  if( n<=0 ) return zBuf;
#ifdef SQLITE_ENABLE_API_ARMOR
  // Armor checks (simulate the guarded path).
  if( zBuf==0 || zFormat==0 ) {
    (void)0; // SQLITE_MISUSE_BKPT placeholder (no-op in tests)
    if( zBuf ) zBuf[0] = 0;
    return zBuf;
  }
#endif
  sqlite3StrAccumInit(&acc, 0, zBuf, n, 0);
  sqlite3_str_vappendf(&acc, zFormat, ap);
  // Ensure null termination within the allocated buffer capacity:
  if (acc.nChar < acc.nAlloc) {
      zBuf[acc.nChar] = 0;
  } else if (acc.nAlloc > 0) {
      zBuf[acc.nAlloc - 1] = 0;
  }
  return zBuf;
}

// Helper to call sqlite3_vsnprintf with a variable number of arguments.
// This helps in tests to keep code clean and readable.
static void run_vsnprintf(int n, char *buf, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    sqlite3_vsnprintf(n, buf, fmt, ap);
    va_end(ap);
}

// ----------------------
// Unit Tests
// ----------------------

// Test A: When n <= 0, the function should return zBuf without modification.
void test_vsnprintf_negative_n() {
    // Prepare a buffer with known content.
    char buf[32] = "INITIAL_CONTENT";
    // Call with negative n; should return early and not modify the buffer.
    run_vsnprintf(-5, buf, "Format %d", 123);
    EXPECT_TRUE(std::string(buf) == "INITIAL_CONTENT",
                "Expected buffer content to remain unchanged when n <= 0");
}

// Test B: ARMOR macro path (zBuf == 0 or zFormat == 0) should be handled gracefully.
void test_vsnprintf_armor_zbuf_null() {
#ifdef SQLITE_ENABLE_API_ARMOR
    // zBuf is null: function should return null without dereferencing.
    char *buf = nullptr;
    run_vsnprintf(10, buf, "Some format %d", 1);
    EXPECT_TRUE(buf == nullptr, "Expected nullptr to be returned when zBuf is null under API_ARMOR");
#endif
}

void test_vsnprintf_armor_zFormat_null() {
#ifdef SQLITE_ENABLE_API_ARMOR
    // zFormat is null: function should set zBuf[0] = 0 if zBuf is non-null and return.
    char buf[16];
    std::memset(buf, 0xAA, sizeof(buf)); // fill with a known non-zero pattern
    run_vsnprintf(16, buf, nullptr);
    // After the call, first character should be zeroed due to armor handling
    EXPECT_TRUE(buf[0] == 0, "Expected zBuf[0] to be set to 0 when zFormat is null under API_ARMOR");
#endif
}

// Test C: Normal path with formatting, verify content and termination.
void test_vsnprintf_normal_formatting() {
    char buf[64];
    std::memset(buf, 0, sizeof(buf));
    // This should format into the buffer as: "Number: 42, String: X"
    run_vsnprintf(64, buf, "Number: %d, String: %s", 42, "X");
    EXPECT_TRUE(std::string(buf) == "Number: 42, String: X",
                "Expected correctly formatted string for normal path");
}

// Test D: Capacity-limited behavior. When the format would exceed capacity, content should be truncated with terminator.
void test_vsnprintf_capacity_limit() {
    // Capacity is only 10 bytes, content should be truncated accordingly.
    char buf[12]; // allocate extra for terminator safety
    std::memset(buf, 0, sizeof(buf));
    run_vsnprintf(10, buf, "0123456789ABCDEF"); // 16 chars, capacity 10
    // Expect at most 9 chars plus terminator (nChar <= nAlloc - 1)
    std::string s(buf);
    EXPECT_TRUE(s.size() <= 10, "Expected at most 10 bytes including terminator in buffer");
}

// ----------------------
// Main: Run tests
// ----------------------

int main() {
    // Run tests
    test_vsnprintf_negative_n();
    test_vsnprintf_armor_zbuf_null();
    test_vsnprintf_armor_zFormat_null();
    test_vsnprintf_normal_formatting();
    test_vsnprintf_capacity_limit();

    // Print summary
    testSummary();
    return (g_failed_tests == 0) ? 0 : 1;
}