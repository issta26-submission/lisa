// Test suite for the focal method _tr_stored_block in trees.c
// This test is designed for C++11 compilation without a mocking framework.
// It relies on the actual implementation in trees.c linked alongside this test.
// The tests exercise key behaviors: header/block encoding, stored_len handling, and debug statistics when ZLIB_DEBUG is defined.

// Notes based on provided guidance (Domain Knowledge):
// - We verify that put_short writes two bytes (little-endian) for stored_len and ~stored_len.
// - We verify that when stored_len > 0, the input buffer is copied into pending_buf after the header.
// - We verify the case stored_len == 0 still emits the header (4 bytes) but no payload.
// - If ZLIB_DEBUG is defined, we verify compressed_len and bits_sent updates.
// - We do not rely on private static helpers; we exercise via the public _tr_stored_block entry point.

#include <cstring>
#include <cstdio>
#include <deflate.h>
#include <cstdint>
#include <iostream>
#include <vector>


// Include the project header that defines deflate_state and related types.

// Declare the focal function from the C file (no name mangling for C linkage)
extern "C" void _tr_stored_block(deflate_state *s, unsigned char *buf, ulg stored_len, int last);

// Simple non-terminating assertion framework
static int g_failures = 0;
#define EXPECT_EQ(a, b) \
    do { \
        auto _a = (a); \
        auto _b = (b); \
        if (!(_a == _b)) { \
            ++g_failures; \
            std::cerr << "EXPECT_EQ failed: " #a " (" << _a << ") != " #b " (" << _b << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
        } \
    } while (0)

#define EXPECT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            ++g_failures; \
            std::cerr << "EXPECT_TRUE failed: " #cond " is false at " << __FILE__ << ":" << __LINE__ << "\n"; \
        } \
    } while (0)

#define TEST_PASSED(msg) \
    do { \
        std::cout << "[TEST PASSED] " << msg << "\n"; \
    } while (0)


// Test 1: stored_len = 0, last = 0
// - Expect header bytes corresponding to stored_len = 0 and ~0 (0xFFFF) in little-endian order.
// - Expect 4 bytes written by the two put_short calls.
// - Expect no payload copied (stored_len == 0). pending should be 4.
void test_tr_stored_block_zero_length() {
    // Prepare state
    deflate_state s;
    std::memset(&s, 0, sizeof(deflate_state));
    // Allocate a reasonably large output buffer
    unsigned char *pending_buf = new unsigned char[64];
    s.pending_buf = pending_buf;
    s.pending = 0;

    unsigned char payload[8] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 };

    // Call the focal function
    _tr_stored_block(&s, payload, 0, 0);

    // Validate expected behavior
    // After two put_short calls: 4 bytes of header
    EXPECT_EQ(s.pending, 4);

    // Header: first short is stored_len (0x0000), next short is ~stored_len (0xFFFF)
    EXPECT_EQ(s.pending_buf[0], 0x00);
    EXPECT_EQ(s.pending_buf[1], 0x00);
    EXPECT_EQ(s.pending_buf[2], 0xFF);
    EXPECT_EQ(s.pending_buf[3], 0xFF);

    // No payload should be copied
    EXPECT_EQ(s.pending_buf[4], 0); // unchanged from initialization

    delete[] pending_buf;
    if (g_failures == 0) TEST_PASSED("test_tr_stored_block_zero_length");
}


// Test 2: stored_len > 0, verify header encoding, payload copy, and last flag influence
void test_tr_stored_block_with_payload() {
    deflate_state s;
    std::memset(&s, 0, sizeof(deflate_state));

    // Allocate buffers
    unsigned char *pending_buf = new unsigned char[128];
    s.pending_buf = pending_buf;
    s.pending = 0;

    // Input payload to be copied
    unsigned char input[5] = { 1, 2, 3, 4, 5 };

    // Call the focal function with stored_len = 5
    _tr_stored_block(&s, input, 5, 1); // last = 1 (for last block)

    // Validate header
    // First short: 5 -> 0x05 0x00 (little-endian)
    EXPECT_EQ(s.pending_buf[0], 0x05);
    EXPECT_EQ(s.pending_buf[1], 0x00);

    // Second short: ~5 -> 0xFFFA in 16-bit, little-endian -> 0xFA, 0xFF
    EXPECT_EQ(s.pending_buf[2], 0xFA);
    EXPECT_EQ(s.pending_buf[3], 0xFF);

    // Payload should start at offset 4
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(s.pending_buf[4 + i], input[i]);
    }

    // Total pending should be 4 (header) + 5 (payload) = 9
    EXPECT_EQ(s.pending, 9);

    delete[] pending_buf;
    if (g_failures == 0) TEST_PASSED("test_tr_stored_block_with_payload");
}


// Test 3: Debug path (ZLIB_DEBUG defined) validates compressed_len and bits_sent updates
void test_tr_stored_block_debug_metrics() {
    // Only perform if the project builds with debug instrumentation
    deflate_state s;
    std::memset(&s, 0, sizeof(deflate_state));

    // Allocate buffers
    unsigned char *pending_buf = new unsigned char[128];
    s.pending_buf = pending_buf;
    s.pending = 0;

    // Initialize debug-related fields
    s.compressed_len = 0;
    s.bits_sent = 0;

    unsigned char input[3] = { 9, 9, 9 };

    _tr_stored_block(&s, input, 3, 0);

    // Header + payload size
    EXPECT_EQ(s.pending, 4 + 3);

    // Debug metrics (only if compiled with ZLIB_DEBUG)
#ifdef ZLIB_DEBUG
    // compressed_len: initial 0 -> (0 + 3 + 7) & ~7 = 8, then + (stored_len + 4) << 3
    // => 8 + (3+4)*8 = 8 + 56 = 64
    EXPECT_EQ(s.compressed_len, 64);

    // bits_sent: 0 -> 2*16 = 32, plus stored_len<<3 = 24
    EXPECT_EQ(s.bits_sent, 56);
#else
    // If not compiled with debug, we skip metric checks
    EXPECT_TRUE(true);
#endif

    // Payload content copy check
    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(s.pending_buf[4 + i], input[i]);
    }

    delete[] pending_buf;
    if (g_failures == 0) TEST_PASSED("test_tr_stored_block_debug_metrics");
}


// Main driver to run all tests
int main() {
    test_tr_stored_block_zero_length();
    test_tr_stored_block_with_payload();
    test_tr_stored_block_debug_metrics();

    if (g_failures == 0) {
        std::cout << "[ALL TESTS PASSED] All focal method tests succeeded.\n";
        return 0;
    } else {
        std::cerr << "[TESTS FAILED] Total failures: " << g_failures << "\n";
        return 1;
    }
}