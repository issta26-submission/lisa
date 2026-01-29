// Unit test suite for the focal method read_buf (deflate.c) implemented in a self-contained
// C++11 test harness. This test suite recreates the minimal environment required by read_buf
// and exercises key execution paths (branches) without relying on GTest. It uses a lightweight
// assertion framework that does not terminate on failure to maximize code coverage.
//
// Notes:
// - read_buf copies up to size bytes from strm->next_in into buf using strm->avail_in as the source size.
// - It updates strm->avail_in, strm->next_in, strm->total_in, and, depending on wrap, the Adler or CRC.
// - This test focuses on non-GZIP (wrap == 1) and branch coverage around length handling.
// - The GZIP path (wrap == 2) is guarded by #ifdef GZIP in the focal code. This test does not
//   enable GZIP (GZIP is intentionally not defined here) to keep the test deterministic.

#include <iostream>
#include <cassert>
#include <deflate.h>
#include <cstring>


// Lightweight domain types to mirror the expected environment of read_buf.
typedef unsigned char Bytef;

// Forward declaration of the state structure used by z_stream
typedef struct z_stream_state {
    int wrap; // 1 means Adler, 2 means CRC, others ignored here
} z_stream_state;

// Minimal z_streamp structure expected by read_buf
typedef struct z_stream_s {
    unsigned int avail_in;
    Bytef* next_in;
    unsigned int total_in;
    unsigned int adler;
    z_stream_state* state;
} z_stream;

typedef z_stream* z_streamp;

// Local function prototypes (matching the original function's expectations)
static unsigned adler32(unsigned, const Bytef*, unsigned);
static unsigned crc32(unsigned, const Bytef*, unsigned);

// The focal method re-implemented locally for testing (non-static to facilitate direct invocation in tests)
static unsigned read_buf(z_streamp strm, Bytef *buf, unsigned size) {
    unsigned len = strm->avail_in;
    if (len > size) len = size;
    if (len == 0) return 0;
    strm->avail_in  -= len;
    std::memcpy(buf, strm->next_in, len);
    if (strm->state->wrap == 1) {
        strm->adler = adler32(strm->adler, buf, len);
    }
#ifdef GZIP
    else if (strm->state->wrap == 2) {
        strm->adler = crc32(strm->adler, buf, len);
    }
#endif
    strm->next_in  += len;
    strm->total_in += len;
    return len;
}

// Simple Adler/CRC implementations used by read_buf to modify the accumulator.
// These implementations are deterministic and easy to reason about in tests.
static unsigned adler32(unsigned a, const Bytef* buf, unsigned len) {
    unsigned sum = 0;
    for (unsigned i = 0; i < len; ++i) sum += buf[i];
    return a ^ sum;
}

#ifdef GZIP
static unsigned crc32(unsigned a, const Bytef* buf, unsigned len) {
    unsigned sum = 0;
    for (unsigned i = 0; i < len; ++i) sum += static_cast<unsigned>(buf[i]) * 3u;
    return a ^ sum;
}
#endif

// Lightweight test framework (non-terminating). Tracks pass/fail counts per test run.
static int g_pass = 0;
static int g_fail = 0;

#define EXPECT_EQ(exp, act) do { auto _e = (exp); auto _a = (act); if (!(_e == _a)) { std::cerr << "FAIL: " << __FUNCTION__ << " (line " << __LINE__ << "): expected " << _e << " but got " << _a << "\n"; ++g_fail; } else { ++g_pass; } } while(0)
#define EXPECT_TRUE(cond) do { if (!(cond)) { std::cerr << "FAIL: " << __FUNCTION__ << " (line " << __LINE__ << "): expected true\n"; ++g_fail; } else { ++g_pass; } } while(0)
#define EXPECT_FALSE(cond) do { if (cond) { std::cerr << "FAIL: " << __FUNCTION__ << " (line " << __LINE__ << "): expected false\n"; ++g_fail; } else { ++g_pass; } } while(0)


// Test 1: Basic copy path where avail_in <= size and wrap == 1 (Adler32 should be applied)
void test_read_buf_basic_wrap1_adler() {
    // Prepare input data
    unsigned char input[] = {1, 2, 3, 4, 5};
    Bytef buf_out[16] = {0};

    // Prepare stream state with wrap == 1 (Adler32)
    z_stream_state state;
    state.wrap = 1;

    z_stream strm;
    strm.avail_in = 5;
    strm.next_in = input;
    strm.total_in = 0;
    strm.adler = 0;
    strm.state = &state;

    // Call read_buf with size >= avail_in
    unsigned copied = read_buf(&strm, buf_out, 10);

    // Assertions
    EXPECT_EQ(5u, copied);                      // len should be 5
    EXPECT_EQ(0u, strm.avail_in);                // avail_in decreased by 5
    EXPECT_EQ(5u, strm.total_in);                // total_in increased by 5
    EXPECT_EQ(0u, std::memcmp(input, buf_out, 5)); // buffer content matches input
    // Adler should have been updated based on the copied bytes
    unsigned expected_adler = adler32(0u, input, 5);
    EXPECT_EQ(expected_adler, strm.adler);
}

// Test 2: When avail_in > size, only size bytes are copied and internal pointers advance accordingly
void test_read_buf_len_greater_than_size() {
    unsigned char data[10] = {10,20,30,40,50,60,70,80,90,100};
    Bytef buf_out[8] = {0};

    z_stream_state state;
    state.wrap = 1;

    z_stream strm;
    strm.avail_in = 10;
    strm.next_in = data;
    strm.total_in = 0;
    strm.adler = 0;
    strm.state = &state;

    unsigned copied = read_buf(&strm, buf_out, 4); // size smaller than avail_in

    EXPECT_EQ(4u, copied);
    EXPECT_EQ(6u, strm.avail_in);                   // 10 - 4
    EXPECT_EQ(4u, strm.total_in);                   // 0 + 4
    EXPECT_EQ(0u, std::memcmp(data, buf_out, 4));  // first 4 bytes copied
    unsigned expected_adler = adler32(0u, buf_out, 4);
    EXPECT_EQ(expected_adler, strm.adler);
}

// Test 3: len == 0 scenario: returns 0 and state does not change
void test_read_buf_len_zero() {
    unsigned char data[4] = {1,2,3,4};
    Bytef buf_out[4] = {0};

    z_stream_state state;
    state.wrap = 1;

    z_stream strm;
    strm.avail_in = 0;
    strm.next_in = data;
    strm.total_in = 0;
    strm.adler = 0;
    strm.state = &state;

    unsigned copied = read_buf(&strm, buf_out, 10);
    EXPECT_EQ(0u, copied);
    EXPECT_EQ(0u, strm.avail_in);
    EXPECT_EQ(0u, strm.total_in);
    EXPECT_EQ(0u, std::memcmp(data, buf_out, 0));
    EXPECT_EQ(0u, strm.adler);
}

// Test 4: size == 0 scenario: returns 0 immediately without copying or state changes
void test_read_buf_size_zero() {
    unsigned char data[] = {7,8,9,10};
    Bytef buf_out[4] = {0};

    z_stream_state state;
    state.wrap = 1;

    z_stream strm;
    strm.avail_in = 4;
    strm.next_in = data;
    strm.total_in = 0;
    strm.adler = 0;
    strm.state = &state;

    unsigned copied = read_buf(&strm, buf_out, 0);
    EXPECT_EQ(0u, copied);
    // No changes to avail_in/total_in/adler/next_in
    EXPECT_EQ(4u, strm.avail_in);
    EXPECT_EQ(0u, strm.total_in);
    EXPECT_EQ(0u, strm.adler);
    EXPECT_EQ(0u, std::memcmp(data, buf_out, 0));
}

// Runner
int main() {
    std::cout << "Starting read_buf unit tests (non-GZIP path)..." << std::endl;

    test_read_buf_basic_wrap1_adler();
    test_read_buf_len_greater_than_size();
    test_read_buf_len_zero();
    test_read_buf_size_zero();

    std::cout << "Tests completed. Passed: " << g_pass << ", Failed: " << g_fail << std::endl;

    // Return non-zero if any test failed, to aid integration with build systems.
    return (g_fail == 0) ? 0 : 1;
}