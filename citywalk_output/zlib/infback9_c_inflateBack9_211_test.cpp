// Test suite for inflateBack9 (C library) using C++11
// Note: This test uses the library's own headers and types. It avoids Google Test
// and instead uses a lightweight, non-terminating assertion style via
// manual result reporting. The tests are designed to exercise key behavior
// of inflateBack9, including error paths and a minimal successful path.

#include <cstring>
#include <inflate9.h>
#include <infback9.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <inftree9.h>
#include <zutil.h>
#include <inffix9.h>
#include <cstdlib>


// Include the library headers (assumed to be in the include path)
extern "C" {
}

// The testing namespace / scope
namespace InflateBack9Test {

    // Lightweight, non-terminating test assertion macro
    // Prints a message on failure but does not abort the program.
    #define TEST_ASSERT(cond, msg)                                   \
        do {                                                           \
            if (!(cond)) {                                             \
                std::cerr << "TEST FAILURE: " << (msg) << std::endl;  \
                ++g_fail_count;                                        \
            }                                                          \
        } while (0)

    static int g_fail_count = 0;
}

// Forward declarations of test callbacks that match the library's expected signatures.
// These prototypes rely on the H files (zutil.h, infback9.h, etc.) defining
// in_func and out_func appropriately. We provide simple implementations below.
using namespace InflateBack9Test;

// A tiny in_desc/in buffer used for test 3 (minimal stored-block input):
struct InBuffer {
    const unsigned char* data;
    unsigned int size;
    unsigned int pos;
};

// In callback: provide input data on first call; the rest of the buffer is ignored
unsigned in_back9(void* in_desc, z_const unsigned char** next) {
    // We supply a single contiguous block of input data for the test:
    // 0x01 header (lastblock=1, type=stored)
    // 0x04 0x00 0xFB 0xFF length (0x0004) and its complement
    // 'ABCD' as the stored data to copy
    static InBuffer* ib = reinterpret_cast<InBuffer*>(in_desc);
    if (!ib) {
        *next = Z_NULL;
        return 0;
    }
    // Provide the entire buffer in one go on the first call
    if (ib->pos == 0) {
        *next = ib->data;
        unsigned int remaining = ib->size - ib->pos;
        // Mark as consumed for this test; subsequent calls (if any) won't supply more data
        ib->pos = ib->size;
        return remaining;
    }
    // No more data
    *next = Z_NULL;
    return 0;
}

// Out callback: collect emitted bytes into a vector for verification
static std::vector<unsigned char> g_out_buffer;
int out_back9(void* out_desc, const unsigned char* data, unsigned int len) {
    (void)out_desc; // unused in this test
    if (data && len > 0) {
        g_out_buffer.insert(g_out_buffer.end(), data, data + len);
    }
    return 0; // success
}

// Helper to run individual tests and report summary
static void runAllTests();

// Test 1: Null strm should return Z_STREAM_ERROR
static void test_inflateBack9_null_strm() {
    std::cout << "Running Test 1: inflateBack9 with null z_stream -> expect Z_STREAM_ERROR" << std::endl;

    // Callback placeholders (not used since strm is null)
    unsigned dummy_in(void*, z_const unsigned char**){ return 0; }
    int dummy_out(void*, const unsigned char*, unsigned int){ return 0; }

    int ret = inflateBack9(NULL, (in_func)dummy_in, NULL, (out_func)dummy_out, NULL);
    TEST_ASSERT(ret == Z_STREAM_ERROR, "inflateBack9(null) should return Z_STREAM_ERROR");
}

// Test 2: Non-null strm but null state should return Z_STREAM_ERROR
static void test_inflateBack9_null_state() {
    std::cout << "Running Test 2: inflateBack9 with non-null strm but null state -> expect Z_STREAM_ERROR" << std::endl;

    z_stream z;
    std::memset(&z, 0, sizeof(z)); // ensure clean
    z.state = Z_NULL;              // explicitly no state

    int ret = inflateBack9(&z, (in_func)nullptr, nullptr, (out_func)nullptr, nullptr);
    TEST_ASSERT(ret == Z_STREAM_ERROR, "inflateBack9(null state) should return Z_STREAM_ERROR");
}

// Test 3: Minimal stored-block path (TYPE=stored, lastblock=1) with exact data
// Expected to decompress "ABCD" into the sliding window and return Z_STREAM_END
static void test_inflateBack9_minimal_stored_block() {
    std::cout << "Running Test 3: inflateBack9 minimal stored-block path -> expect Z_STREAM_END and 'ABCD' output" << std::endl;

    // Build test input data for a single stored block containing "ABCD"
    // Header: lastblock=1, type=stored => 0b00000001 (low 3 bits)
    // First byte: 0x01 (binary 00000001)
    // 32-bit stored block length: 4 (0x0004) with complement 0xFFFB
    // Data: 'A','B','C','D'
    unsigned char input_bytes[9] = {
        0x01,       // block header: lastblock=1, type=stored
        0x04, 0x00, // length = 4 (little endian)
        0xFB, 0xFF, // complement of 4 (4 ^ 0xFFFF = 0xFFFB)
        'A', 'B', 'C', 'D' // 4 data bytes to copy
    };

    // Prepare input descriptor
    InBuffer ib;
    ib.data = input_bytes;
    ib.size = sizeof(input_bytes);
    ib.pos  = 0;

    // Prepare z_stream and inflate_state
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    // Allocate a sliding window
    unsigned char* window = (unsigned char*)std::malloc(WSIZE);
    std::memset(window, 0, WSIZE);

    // Allocate and initialize inflate_state
    struct inflate_state* state = (struct inflate_state*)std::malloc(sizeof(struct inflate_state));
    std::memset(state, 0, sizeof(struct inflate_state));
    state->window = window;

    // Attach to strm
    strm.state = (void*)state;
    // Input source
    strm.next_in = input_bytes;
    strm.avail_in = sizeof(input_bytes);

    // Prepare input/output callback pointers
    g_out_buffer.clear();

    // Call inflateBack9 with our callbacks
    int ret = inflateBack9(&strm, in_back9, &ib, out_back9, NULL);

    // Assertions
    TEST_ASSERT(ret == Z_STREAM_END, "inflateBack9 should return Z_STREAM_END on successful end of stream");

    // The output buffer should contain the 4 copied bytes "ABCD" at the start
    // Since the window starts at index 0, the first left-overs should be that data.
    TEST_ASSERT(g_out_buffer.size() == 4, "Output should contain 4 bytes (ABCD)");
    if (g_out_buffer.size() >= 4) {
        TEST_ASSERT(g_out_buffer[0] == 'A' && g_out_buffer[1] == 'B' &&
                    g_out_buffer[2] == 'C' && g_out_buffer[3] == 'D',
                    "Output bytes should be 'A','B','C','D'");
    }

    // Cleanup
    std::free(state);
    std::free(window);
}

int main() {
    // Run tests in sequence
    test_inflateBack9_null_strm();
    test_inflateBack9_null_state();
    test_inflateBack9_minimal_stored_block();

    // Summary
    if (InflateBack9Test::g_fail_count == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << InflateBack9Test::g_fail_count << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}

// Note about coverage and domain knowledge:
// - This suite focuses on: null-pointer handling (TYPE/Stored path not executed),
//   and a concrete, minimal stored-block path exercising the store-block path.
// - It ensures the function returns Z_STREAM_ERROR for bad inputs and Z_STREAM_END
//   for a complete, valid end-of-stream scenario.
// - Static helpers are not accessed directly; the test uses public library API.
// - All tests run from main as required by environments where GTest/GMock is not allowed.