// High-quality C++11 test suite for sqlite3Fts5BufferAppendBlob
// Focus: validate behavior of sqlite3Fts5BufferAppendBlob in fts5_buffer.c
// This test suite does not use GTest; it uses a small, self-contained test harness.

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fts5Int.h>


// Import C headers with C linkage to ensure proper symbol resolution
extern "C" {
}

// Declaration of the focal function (should be available via linking with the C file)
extern "C" {
int sqlite3Fts5BufferAppendBlob(int *pRc, Fts5Buffer *pBuf, u32 nData, const u8 *pData);
}

// Simple lightweight test framework (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_OK(cond, msg) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        ++g_failed_tests; \
        fprintf(stderr, "Test FAILED: %s\n", msg); \
    } \
} while(0)

#define TEST_MEM_EQ(src, dst, n, msg) do { \
    ++g_total_tests; \
    if(std::memcmp((src), (dst), (n)) != 0) { \
        ++g_failed_tests; \
        fprintf(stderr, "Test FAILED: %s\n", msg); \
    } \
} while(0)

// Utility: initialize a buffer with given capacity
static void init_buffer(Fts5Buffer *buf, int capacity) {
    buf->p = (u8*)std::malloc(capacity);
    buf->n = 0;
    // Some implementations use 'm' as capacity
    buf->m = capacity;
}

// Utility: free buffer memory
static void free_buffer(Fts5Buffer *buf) {
    if(buf && buf->p) {
        std::free(buf->p);
        buf->p = nullptr;
        buf->n = 0;
        buf->m = 0;
    }
}

// Test 1: nData == 0 should perform no operation (false branch of the outer if)
static void test_append_blob_noop_when_nData_zero() {
    // Prepare buffer with ample capacity
    Fts5Buffer buf;
    init_buffer(&buf, 16); // capacity 16, n initially 0

    int rc = 0;
    const u8 data[] = { 0x01, 0x02, 0x03, 0x04 }; // not used since nData == 0
    sqlite3Fts5BufferAppendBlob(&rc, &buf, 0, data);

    // Expect no change to buffer state
    TEST_OK(rc == 0, "rc should remain 0 when nData == 0");
    TEST_OK(buf.n == 0, "buffer length should remain 0 when nData == 0");
    // Nothing to compare in memory since nData == 0
    free_buffer(&buf);
}

// Test 2: nData > 0 and there is enough capacity (growth not needed)
static void test_append_blob_append_without_growth() {
    // Prepare buffer with enough capacity to hold nData without growth
    Fts5Buffer buf;
    init_buffer(&buf, 16); // capacity 16, n = 0

    const u8 src[] = { 0x10, 0x20, 0x30, 0x40 }; // 4 bytes to append
    int rc = 0;

    sqlite3Fts5BufferAppendBlob(&rc, &buf, sizeof(src), src);

    // Validate that data is copied and length updated
    TEST_OK(rc == 0, "rc should be 0 when append succeeds without growth");
    TEST_OK(buf.n == (int)sizeof(src), "buffer length should equal appended data length");

    // Validate content matches source
    TEST_MEM_EQ(src, buf.p, sizeof(src), "buffer content should match appended data (test 2)");
    free_buffer(&buf);
}

// Test 3: nData > 0 and growth is required (buffer reallocates)
static void test_append_blob_append_with_growth() {
    // Prepare buffer with small capacity to force growth
    Fts5Buffer buf;
    init_buffer(&buf, 4); // capacity 4, n = 0

    const u8 src[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11 }; // 7 bytes
    int rc = 0;

    sqlite3Fts5BufferAppendBlob(&rc, &buf, sizeof(src), src);

    // Validate that data is copied and length updated
    TEST_OK(rc == 0, "rc should be 0 when append triggers growth and succeeds");
    TEST_OK(buf.n == (int)sizeof(src), "buffer length should equal appended data length after growth");

    // Validate content matches source (even if memory relocated)
    TEST_MEM_EQ(src, buf.p, sizeof(src), "buffer content should match appended data after growth (test 3)");
    free_buffer(&buf);
}

int main() {
    // Run tests
    test_append_blob_noop_when_nData_zero();
    test_append_blob_append_without_growth();
    test_append_blob_append_with_growth();

    // Summary
    if(g_failed_tests) {
        fprintf(stderr, "\nTotal tests: %d, Failures: %d\n", g_total_tests, g_failed_tests);
        return 1;
    } else {
        fprintf(stdout, "\nAll tests passed. Total tests: %d\n", g_total_tests);
        return 0;
    }
}