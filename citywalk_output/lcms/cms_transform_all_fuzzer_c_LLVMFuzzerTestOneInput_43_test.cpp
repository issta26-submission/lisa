#include <cstring>
#include <vector>
#include <cstddef>
#include <iostream>
#include <stdint.h>
#include <lcms2.h>
#include <cstdint>


// This test file includes a self-contained reproduction of the focal function
// LLVMFuzzerTestOneInput and its dependency hook run_test.
// It is designed to be compiled as a single translation unit (C++11) without GTest.

// Global state to capture calls to run_test for verification in tests.
static bool g_last_run_called = false;
static const uint8_t* g_last_run_data_ptr = nullptr;
static size_t g_last_run_size = 0;
static uint32_t g_last_run_intent = 0;
static uint32_t g_last_run_input_format = 0;
static uint32_t g_last_run_output_format = 0;
static uint32_t g_last_run_flags = 0;

// Simple non-terminating test assertion mechanism.
// It records failures but does not abort tests, allowing multiple checks per test.
static int g_test_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "Test Failure: " << (msg) << std::endl; \
        ++g_test_failures; \
    } \
} while (0)

static void reset_last_run_state() {
    g_last_run_called = false;
    g_last_run_data_ptr = nullptr;
    g_last_run_size = 0;
    g_last_run_intent = 0;
    g_last_run_input_format = 0;
    g_last_run_output_format = 0;
    g_last_run_flags = 0;
}

// The dependency function that the focal method forwards to.
// It records all relevant parameters for test verification.
void run_test(const uint8_t *data,
              size_t size,
              uint32_t intent_id,
              uint32_t input_format,
              uint32_t output_format,
              uint32_t flags) {
    (void)data; // data is used by the focal method; for tests, we capture the address only.
    g_last_run_called = true;
    g_last_run_data_ptr = data;
    g_last_run_size = size;
    g_last_run_intent = intent_id;
    g_last_run_input_format = input_format;
    g_last_run_output_format = output_format;
    g_last_run_flags = flags;
}

// The focal method under test (copied here to create a self-contained testable unit).
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < 16) {
        return 0;
    }
    // Generate a random set of args for cmsCreateTransform
    uint32_t input_format  = *((const uint32_t *)data);
    uint32_t output_format = *((const uint32_t *)data+1);
    uint32_t flags         = *((const uint32_t *)data+2);
    uint32_t intent        = *((const uint32_t *)data+3) % 16;
    data += 16;
    size -= 16;
    run_test(data, size, intent, input_format, output_format, flags);
    return 0;
}

// Helper to push a 32-bit value into a byte buffer in host endianness.
static void push32(std::vector<uint8_t>& buf, uint32_t v) {
    const uint8_t* p = reinterpret_cast<const uint8_t*>(&v);
    buf.insert(buf.end(), p, p + 4);
}

// Test 1: When size < 16, the function should return immediately and not call run_test.
void test_size_less_than_16() {
    reset_last_run_state();

    // Prepare a buffer with size 15 (must trigger early return)
    std::vector<uint8_t> buf(15, 0);
    int ret = LLVMFuzzerTestOneInput(buf.data(), buf.size());

    EXPECT_TRUE(ret == 0, "Return value should be 0 for small input.");
    EXPECT_TRUE(!g_last_run_called, "run_test should NOT be called when size < 16.");
}

// Test 2: Ensure that with size >= 16, the function decodes header values correctly
// and passes the correct arguments to run_test, including the data pointer after the header.
void test_header_and_rest_pass_through() {
    reset_last_run_state();

    std::vector<uint8_t> buf;
    // Header (16 bytes): input_format, output_format, flags, intent_seed
    push32(buf, 0x11223344); // input_format
    push32(buf, 0x55667788); // output_format
    push32(buf, 0xAABBCCDD); // flags
    push32(buf, 0x0000000F); // intent_seed -> 15 after mod 16

    // Rest of the data after header (arbitrary content)
    push32(buf, 0xDEADBEEF);
    push32(buf, 0xCAFEBABE);

    int ret = LLVMFuzzerTestOneInput(buf.data(), buf.size());

    EXPECT_TRUE(ret == 0, "Return value should be 0 for normal input.");
    EXPECT_TRUE(g_last_run_called, "run_test should be called when size >= 16.");

    // Validate forwarded header values and computed intent
    EXPECT_TRUE(g_last_run_intent == 15, "Intent should be header[3] % 16 (15).");
    EXPECT_TRUE(g_last_run_input_format == 0x11223344, "Input format should be forwarded correctly.");
    EXPECT_TRUE(g_last_run_output_format == 0x55667788, "Output format should be forwarded correctly.");
    EXPECT_TRUE(g_last_run_flags == 0xAABBCCDD, "Flags should be forwarded correctly.");

    // Validate that data pointer passed to run_test equals buffer region after the 16-byte header
    EXPECT_TRUE(g_last_run_data_ptr == buf.data() + 16, "Data pointer to run_test should point after 16-byte header.");
    EXPECT_TRUE(g_last_run_size == buf.size() - 16, "Size passed to run_test should be the remaining data after header.");
}

// Test 3: Ensure behavior exactly at size == 16.
// The data pointer passed to run_test should be buffer + 16 and remaining size should be 0.
void test_size_equal_16_pass_through() {
    reset_last_run_state();

    std::vector<uint8_t> buf;
    // Header only (16 bytes)
    push32(buf, 0x01234567); // input_format
    push32(buf, 0x89ABCDEF); // output_format
    push32(buf, 0x10111213); // flags
    push32(buf, 0x00000003); // intent_seed -> 3

    int ret = LLVMFuzzerTestOneInput(buf.data(), buf.size());

    EXPECT_TRUE(ret == 0, "Return value should be 0 for exact header size.");
    EXPECT_TRUE(g_last_run_called, "run_test should be called when size >= 16 (even if rest is zero).");
    EXPECT_TRUE(g_last_run_size == 0, "Remaining size should be 0 when header is the entire input.");
    EXPECT_TRUE(g_last_run_data_ptr == buf.data() + 16, "Data pointer should point to the end of buffer (after header).");
    EXPECT_TRUE(g_last_run_intent == 3, "Intent should be header[3] % 16 (3).");
    EXPECT_TRUE(g_last_run_input_format == 0x01234567, "Input format should be forwarded correctly.");
    EXPECT_TRUE(g_last_run_output_format == 0x89ABCDEF, "Output format should be forwarded correctly.");
    EXPECT_TRUE(g_last_run_flags == 0x10111213, "Flags should be forwarded correctly.");
}

// Simple test runner to execute all tests and print a summary.
// This avoids any external test framework per the constraints.
int main() {
    std::cout << "Starting unit tests for LLVMFuzzerTestOneInput (self-contained)..." << std::endl;

    test_size_less_than_16();
    test_header_and_rest_pass_through();
    test_size_equal_16_pass_through();

    if (g_test_failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << g_test_failures << " test(s) FAILED." << std::endl;
        return 1;
    }
}