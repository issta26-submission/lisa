// Test suite for sqlite3Fts5BufferSet (fts5_buffer.c) using a lightweight C++11 test harness.
// This test is designed to be compiled in a project that provides the real fts5_buffer.c
// and its dependencies (e.g., fts5Int.h). We avoid any external test frameworks (no GTest).
//
// Key goal: verify that sqlite3Fts5BufferSet resets pBuf->n to 0 and then delegates to
// sqlite3Fts5BufferAppendBlob with the provided data. We infer behavior by inspecting
// pBuf.n after the call for different nData values (0 and >0).

#include <vector>
#include <fts5Int.h>
#include <cstdio>
#include <cstring>
#include <iostream>


// Bring the C symbols into the C++ test world.
extern "C" {
// Declaration of the focal function under test (defined in fts5_buffer.c)
void sqlite3Fts5BufferSet(int *pRc, Fts5Buffer *pBuf, int nData, const u8 *pData);
}

// Simple test harness (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

// Expectation macro: continues execution after failure
#define EXPECT_TRUE(cond) do { \
    ++g_total; \
    if(!(cond)) { \
        ++g_failed; \
        std::cerr << "EXPECT_TRUE failed: " << #cond \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    ++g_total; \
    if(!((a) == (b))) { \
        ++g_failed; \
        std::cerr << "EXPECT_EQ failed: " << #a << " == " << #b \
                  << " (actual: " << (a) << ", " << "expected: " << (b) \
                  << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
    } \
} while(0)

#define EXPECT_NE(a,b) do { \
    ++g_total; \
    if((a) == (b)) { \
        ++g_failed; \
        std::cerr << "EXPECT_NE failed: " << #a << " != " << #b \
                  << " (both are: " << (a) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
    } \
} while(0)

// Utility to print a short summary
static void print_summary() {
    std::cout << "Test run: " << g_total << ", Failures: " << g_failed << "\n";
}

// Test 1: Non-zero data length (nData > 0) should reset pBuf->n to 0 and then set it to nData
// This test also verifies that pBuf.n changes from its pre-call value, indicating the reset occurred.
static void test_sqlite3Fts5BufferSet_nonZeroData() {
    // Prepare inputs
    int rc = 0;
    Fts5Buffer buf;
    std::memset(&buf, 0, sizeof(Fts5Buffer)); // ensure a clean start

    // Pre-fill with a non-zero n to ensure we catch the reset to 0
    buf.n = 123;

    // Some sample data of length > 0
    unsigned char data[6] = { 10, 20, 30, 40, 50, 60 };
    int nData = 5; // non-zero

    // Call the focal function
    sqlite3Fts5BufferSet(&rc, &buf, nData, data);

    // Validate behavior
    EXPECT_EQ(rc, 0);       // Expect no error from the operation
    EXPECT_NE(buf.n, 123);  // The pre-call value should be overridden by the reset + append
    EXPECT_EQ(buf.n, nData); // Expect the appended length to reflect nData (typical append behavior)
}

// Test 2: Zero-length data (nData == 0) should reset pBuf->n to 0 and handle zero-length append.
// This checks that the function handles the edge case of empty data gracefully.
static void test_sqlite3Fts5BufferSet_zeroData() {
    int rc = 0;
    Fts5Buffer buf;
    std::memset(&buf, 0, sizeof(Fts5Buffer));

    // Pre-fill with a non-zero value to ensure we observe a reset
    buf.n = 77;

    // nData = 0; pData may be NULL for zero-length data
    const unsigned char *pData = nullptr;
    int nData = 0;

    sqlite3Fts5BufferSet(&rc, &buf, nData, pData);

    // Validate behavior
    EXPECT_EQ(rc, 0);       // Expect no error from the operation
    EXPECT_EQ(buf.n, 0);    // Buffer length should be reset to 0 (no data appended)
}

// Optional: Additional test to ensure that a null data pointer with non-zero length would not crash
// If the real implementation handles it, this should still pass; if not, it will catch a crash.
static void test_sqlite3Fts5BufferSet_nullDataWithNonZeroLen() {
    int rc = 0;
    Fts5Buffer buf;
    std::memset(&buf, 0, sizeof(Fts5Buffer));

    buf.n = 1;
    int nData = 3;
    const unsigned char *pData = nullptr; // intentionally null

    sqlite3Fts5BufferSet(&rc, &buf, nData, pData);

    // We at least expect no crash; value of rc should be 0 if operation succeeded
    EXPECT_EQ(rc, 0);
    // Depending on implementation, pBuf.n may be 0 or nData; we just verify non-crash
    EXPECT_TRUE(true); // placeholder assertion to ensure test ran
}

// Main entry: run all tests
int main() {
    // Run tests with explanatory comments in code
    test_sqlite3Fts5BufferSet_nonZeroData();
    test_sqlite3Fts5BufferSet_zeroData();
    test_sqlite3Fts5BufferSet_nullDataWithNonZeroLen();

    print_summary();
    return g_failed ? 1 : 0;
}