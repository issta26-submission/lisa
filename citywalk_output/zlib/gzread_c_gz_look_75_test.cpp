// Test suite for the focal function gz_look located in gzread.c
// This test is written in C++11, but includes the C source file to test the actual
// function in its own translation unit. It uses a minimal, non-terminating assertion
// style suitable when GTest is not allowed.

#include <cstdio>
#include <cstring>
#include <cstdint>
#include <gzread.c>
#include <gzguts.h>
#include <cstdlib>


// Bring in the C code under test with C linkage
extern "C" {
}

// Lightweight test harness (non-terminating, accumulates failures)
static int g_failures = 0;

#define TEST_LOG(fmt, ...) do { \
    printf("[TEST] " fmt "\n", ##__VA_ARGS__); \
} while (0)

#define ASSERT_TRUE(cond) do { \
    if (!(cond)) { \
        g_failures++; \
        TEST_LOG("Assertion failed: %s at %s:%d", #cond, __FILE__, __LINE__); \
    } \
} while (0)

#define ASSERT_EQ_INT(a, b) do { \
    if ((int)(a) != (int)(b)) { \
        g_failures++; \
        TEST_LOG("Assertion failed: %s == %s (got %d != %d) at %s:%d", #a, #b, (int)(a), (int)(b), __FILE__, __LINE__); \
    } \
} while (0)

#define ASSERT_PTR_EQ(a, b) do { \
    if ((void*)(a) != (void*)(b)) { \
        g_failures++; \
        TEST_LOG("Assertion failed: pointer %s == %s (got %p != %p) at %s:%d", #a, #b, (void*)(a), (void*)(b), __FILE__, __LINE__); \
    } \
} while (0)

#define ASSERT_MEM_EQ(buf1, buf2, len) do { \
    if (std::memcmp((buf1), (buf2), (len)) != 0) { \
        g_failures++; \
        TEST_LOG("Assertion failed: memory regions differ for %s vs %s (len=%zu) at %s:%d", #buf1, #buf2, (size_t)(len), __FILE__, __LINE__); \
    } \
} while (0)


// Helper: safe cleanup for per-test allocations if used
static void safe_free(void* ptr) {
    if (ptr) { free(ptr); ptr = nullptr; }
}

// Individual test: verify COPY-path behavior when input is copied to output
// Preconditions:
// - state.size != 0 to skip initialization path
// - state.direct == 1 to ensure COPY path is taken
// - strm.avail_in > 0 and not a gzip header
// - state.out allocated sufficiently large
// Expected:
// - returns 0
// - state.how == COPY
// - state.direct == 1
// - state.x.have equals the number of bytes copied
// - content of state.out equals the input bytes
static void test_gzlook_copy_path() {
    TEST_LOG("Test: gz_look COPY path copies input to output when direct=1");

    gz_state st;
    std::memset(&st, 0, sizeof(st));

    // Setup: bypass initialization
    st.size = 1;
    st.want = 4;

    // Allocate output buffer (should be larger than input)
    st.out = (unsigned char*)malloc(8);
    ASSERT_TRUE(st.out != nullptr);

    // Prepare a small input of 4 bytes
    unsigned char input[4] = { 'A', 'B', 'C', 'D' };
    // Set up the internal stream to point to our input
    st.strm.zalloc = Z_NULL;
    st.strm.zfree  = Z_NULL;
    st.strm.opaque = Z_NULL;
    st.strm.avail_in = 4;
    st.strm.next_in = input;

    // Start in COPY path
    st.direct = 1;
    st.how    = 0;

    // Initialize a placeholder for the "x" structure fields
    st.x.have = 0;
    st.x.next = nullptr;

    int ret = gz_look(&st);

    ASSERT_EQ_INT(ret, 0);
    ASSERT_TRUE(st.how == COPY);
    ASSERT_TRUE(st.direct == 1);
    ASSERT_TRUE(st.strm.avail_in == 0);
    ASSERT_TRUE(st.x.have == 4);
    ASSERT_PTR_EQ(st.x.next, st.out);
    ASSERT_MEM_EQ(st.out, input, 4);

    safe_free(st.out);
    st.out = nullptr;
}

// Individual test: verify gzip header path when a gzip header is detected
// Preconditions:
// - state.size != 0 to skip initialization
// - strm.avail_in > 1 and next_in[0]==0x1F and next_in[1]==0x8B
// - state.direct == 1
// Expected:
// - returns 0
// - state.how == GZIP
// - state.direct == 0
static void test_gzlook_gzip_header_path() {
    TEST_LOG("Test: gz_look detects gzip header and switches to GZIP mode");

    gz_state st;
    std::memset(&st, 0, sizeof(st));

    st.size = 1;
    st.want = 8;

    // Avoid actual allocation path
    unsigned char dummy_out[16] = {0};
    st.out = dummy_out;

    // Build a gzip header in input
    unsigned char header[2] = { 0x1F, 0x8B };
    st.strm.zalloc = Z_NULL;
    st.strm.zfree  = Z_NULL;
    st.strm.opaque = Z_NULL;
    st.strm.avail_in = 2;
    st.strm.next_in = header;

    st.direct = 1;
    st.x.have = 0;
    st.x.next = nullptr;

    int ret = gz_look(&st);

    ASSERT_EQ_INT(ret, 0);
    ASSERT_TRUE(st.how == GZIP);
    ASSERT_TRUE(st.direct == 0);
}

// Individual test: verify path when direct flag is 0 (trailing garbage or end-of-stream)
// Preconditions:
// - state.direct == 0
// - state.size != 0 to skip initialization
// - there is input present but not a gzip header
// Expected:
// - returns 0
// - state.eof == 1
// - state.x.have == 0
static void test_gzlook_direct_zero_path() {
    TEST_LOG("Test: gz_look handles direct==0 by signaling EOF and ignoring trailing data");

    gz_state st;
    std::memset(&st, 0, sizeof(st));

    st.size = 1;
    st.want = 4;

    unsigned char input[8] = { 0x00, 0x01, 0x02, 0x03 };
    st.strm.zalloc = Z_NULL;
    st.strm.zfree  = Z_NULL;
    st.strm.opaque = Z_NULL;
    st.strm.avail_in = 4;
    st.strm.next_in = input;

    st.direct = 0;
    st.eof = 0;
    st.x.have = 123; // some non-zero to ensure not overwritten unexpectedly
    st.x.next = nullptr;

    int ret = gz_look(&st);

    ASSERT_EQ_INT(ret, 0);
    ASSERT_TRUE(st.eof == 1);
    ASSERT_TRUE(st.x.have == 0);
    // In this path, avail_in should be reset to 0
    ASSERT_TRUE(st.strm.avail_in == 0);
}


// Main: run all tests and report summary
int main() {
    TEST_LOG("Starting gz_look unit tests (non-GTest, single translation unit test harness)");

    test_gzlook_copy_path();
    test_gzlook_gzip_header_path();
    test_gzlook_direct_zero_path();

    if (g_failures == 0) {
        TEST_LOG("ALL TESTS PASSED");
    } else {
        TEST_LOG("TESTS FAILED: %d failure(s)", g_failures);
    }

    return g_failures;
}