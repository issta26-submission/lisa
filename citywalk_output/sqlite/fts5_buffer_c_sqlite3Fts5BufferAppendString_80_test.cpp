// C++11 test harness for sqlite3Fts5BufferAppendString
// This test provides a minimal environment to verify the behavior of
// sqlite3Fts5BufferAppendString by mocking its dependent blob append
// function (sqlite3Fts5BufferAppendBlob) to observe changes in Fts5Buffer::n.
// It relies on the real fts5_buffer.c being compiled alongside this test
// so that sqlite3Fts5BufferAppendString is linked from the actual source.

#include <cstdio>
#include <cstring>
#include <fts5Int.h>


// Include the project header that defines Fts5Buffer and related types.
// This header is assumed to be available in the project include paths.

// The real implementation of sqlite3Fts5BufferAppendString is in fts5_buffer.c.
// It calls sqlite3Fts5BufferAppendBlob. We provide a test-specific override of
// sqlite3Fts5BufferAppendBlob here to observe the effect on Fts5Buffer::n.
// The override must match the project types (u32, u8, Fts5Buffer) as defined
// by fts5Int.h to ensure correct linkage.

extern "C" {

// Mock implementation: simulate appending by increasing pBuf->n by the number
// of data bytes supposedly appended. We intentionally avoid touching any other
// internal buffer state to keep the test robust against internal layout changes.
void sqlite3Fts5BufferAppendBlob(int *pRc, Fts5Buffer *pBuf, u32 nData, const u8 *pData) {
    if (pRc) *pRc = 0;
    // Increment buffer length by nData to simulate a successful append.
    // We assume Fts5Buffer has at least an 'n' member (as used by the focal method).
    pBuf->n += (int)nData;
}
} // extern "C"

// Lightweight test harness (non-GTest, no termination on first failure)

static int g_failures = 0;

// Simple assertion helpers that do not terminate test execution.
#define EXPECT_TRUE(cond) \
    do { \
        if(!(cond)) { \
            printf("TEST FAILURE (%s:%d): expected true, got false: %s\n", __FILE__, __LINE__, #cond); \
            ++g_failures; \
        } \
    } while(0)

#define EXPECT_EQ(a, b) \
    do { \
        if( (a) != (b) ) { \
            printf("TEST FAILURE (%s:%d): expected %d, got %d\n", __FILE__, __LINE__, (int)(a), (int)(b)); \
            ++g_failures; \
        } \
    } while(0)


// Test 1: Append a non-empty string to an empty buffer
// Expectation: final n equals strlen(zStr)
void test_append_string_basic() {
    int rc = 0;
    Fts5Buffer buf;
    buf.n = 0;
    const char *zStr = "hello"; // length 5

    sqlite3Fts5BufferAppendString(&rc, &buf, zStr);

    EXPECT_TRUE(rc == 0);
    // Our mock AppendBlob increments by nData = strlen(zStr) + 1 for the blob,
    // then the focal function decrements by 1. Net effect: +strlen(zStr) = 5
    EXPECT_EQ(buf.n, 5);
}

// Test 2: Append an empty string to an empty buffer
// Expectation: final n equals 0
void test_append_string_empty() {
    int rc = 0;
    Fts5Buffer buf;
    buf.n = 0;
    const char *zStr = ""; // length 0

    sqlite3Fts5BufferAppendString(&rc, &buf, zStr);

    EXPECT_TRUE(rc == 0);
    // Net effect: +0
    EXPECT_EQ(buf.n, 0);
}

// Test 3: Append to a buffer with pre-existing content
// Expectation: final n equals initial_n + strlen(zStr)
void test_append_string_with_initial_content() {
    int rc = 0;
    Fts5Buffer buf;
    buf.n = 7;            // initial length
    const char *zStr = "abcd"; // length 4

    sqlite3Fts5BufferAppendString(&rc, &buf, zStr);

    EXPECT_TRUE(rc == 0);
    EXPECT_EQ(buf.n, 11); // 7 + 4
}

// Test 4: Append a longer string to verify scaling
void test_append_string_long() {
    int rc = 0;
    Fts5Buffer buf;
    buf.n = 2;
    const char *zStr = "The quick brown fox"; // length 19

    sqlite3Fts5BufferAppendString(&rc, &buf, zStr);

    EXPECT_TRUE(rc == 0);
    EXPECT_EQ(buf.n, 21); // 2 + 19
}

// Run all tests
void run_all_tests() {
    printf("Starting sqlite3Fts5BufferAppendString tests...\n");
    test_append_string_basic();
    test_append_string_empty();
    test_append_string_with_initial_content();
    test_append_string_long();
    if (g_failures == 0) {
        printf("ALL TESTS PASSED\n");
    } else {
        printf("TOTAL FAILURES: %d\n", g_failures);
    }
}

int main() {
    run_all_tests();
    return g_failures;
}