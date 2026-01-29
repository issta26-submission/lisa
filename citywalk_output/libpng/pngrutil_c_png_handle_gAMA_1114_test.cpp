// Lightweight C++11 unit test suite for a self-contained, mocked version
// of the focal function logic (png_handle_gAMA) to achieve high coverage
// of its control flow without relying on GoogleTest.
// Note: This is a self-contained reproduction suitable for environments
// where the actual libpng internals are not available. It focuses on the
// logical branches described in the focal method and uses a minimal mock
// of the needed state and helpers.

#include <cstdint>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain-specific: provide a small, self-contained mock of libpng-like types
// and helpers used by the focal method. The goal is to test logic branches.

#define PNG_UINT_31_MAX 2147483647u
#define PNG_READ_GAMMA_SUPPORTED 1
#define PNG_UNUSED(x) (void)(x)

typedef uint8_t png_byte;
typedef uint32_t png_uint_32;
typedef int32_t png_fixed_point;
typedef void* png_inforp;
typedef struct png_struct_mock* png_structrp;

// Global flag to simulate gamma handling when the macro is "enabled"
static bool g_gamma_runtime_enabled = true;

// Simple assertion framework (non-terminating). Keeps count of failures.
static int g_test_failures = 0;
#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        ++g_test_failures; \
        std::cerr << "[ASSERT FAIL] " << (msg) << " (in " << __FUNCTION__ << ")\n"; \
    } \
} while(0)
#define ASSERT_EQ(expected, actual, msg) do { \
    if(!((expected) == (actual))) { \
        ++g_test_failures; \
        std::cerr << "[ASSERT EQ FAIL] " << (msg) << " Expected: " << (expected) \
                  << " Actual: " << (actual) << " (in " << __FUNCTION__ << ")\n"; \
    } \
} while(0)

// Mocked internal state and helpers to simulate the focal method's environment

struct png_read_state {
    png_byte buf[4];       // bytes read by png_crc_read
    int finish_return;     // value returned by png_crc_finish(png_ptr, 0)
};

// Minimal png_struct mock to hold gamma state and read state
struct png_struct_mock {
    int chunk_gamma;                 // corresponds to png_ptr->chunk_gamma
    png_fixed_point gAMA_fixed;       // internal fixed gamma storage (simulated)
    png_read_state read_state;         // simulates CRC read state
};

// Forward declarations of helpers mirroring the focal function's usage

static void png_debug(int /*level*/, const char* /*msg*/) {
    // no-op in test environment
    (void)level;
    (void)msg;
}

static void png_chunk_benign_error(png_structrp /*png_ptr*/, const char* /*msg*/) {
    // In tests we do not treat benign errors as fatal; can be extended if needed
    (void)/*silence*/0;
    (void)/*msg*/0;
}

static void png_set_gAMA_fixed(png_structrp png_ptr, png_inforp /*info_ptr*/, png_fixed_point gamma) {
    // Record the fixed gamma into the mock structure
    if (png_ptr) {
        png_ptr->gAMA_fixed = static_cast<png_uint_32>(gamma);
    }
}

static uint32_t png_get_uint_32(const png_byte buf[4]) {
    // Big-endian interpretation as in libpng
    return ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
           ((uint32_t)buf[2] << 8)  | ((uint32_t)buf[3]);
}

// Mocked CRC read/finish to control test scenarios

static void png_crc_read(png_structrp png_ptr, png_byte* buf, png_uint_32 length) {
    (void)length; // in this test, length is always 4 for the gAMA chunk data
    if (png_ptr && buf) {
        // Copy the 4-byte value prepared in the test state
        std::memcpy(buf, png_ptr->read_state.buf, 4);
    }
}

static int png_crc_finish(png_structrp png_ptr, int /*skip*/) {
    // Return the preconfigured finish value
    if (png_ptr) {
        return png_ptr->read_state.finish_return;
    }
    return 1;
}

// The focal method under test (mocked/versioned for the test harness)
static int fake_png_handle_gAMA(png_structrp png_ptr, png_inforp /*info_ptr*/, png_uint_32 length) {
    png_uint_32 ugamma;
    png_byte buf[4];
    png_debug(1, "in png_handle_gAMA");
    png_crc_read(png_ptr, buf, 4);
    if (png_crc_finish(png_ptr, 0) != 0)
        return -1; // handled_error

    ugamma = png_get_uint_32(buf);
    if (ugamma > PNG_UINT_31_MAX) {
        png_chunk_benign_error(png_ptr, "invalid");
        return -1; // handled_error
    }

    png_set_gAMA_fixed(png_ptr, nullptr, (png_fixed_point)/*SAFE*/ugamma);

#if defined(PNG_READ_GAMMA_SUPPORTED)
    if (png_ptr->chunk_gamma == 0) {
        png_ptr->chunk_gamma = (png_fixed_point)/*SAFE*/ugamma;
    }
#endif

    PNG_UNUSED(length);
    return 1; // handled_ok
}

// Helper to initialize a new mock png_ptr with given read bytes and finish flag
static png_structrp make_png_ptr_mock(const png_byte ugamma_bytes[4], int finish_ret, int initial_chunk_gamma = 0) {
    static png_struct_mock gpu; // static to simplify lifetime management in tests
    // Initialize fresh per-call state
    gpu.chunk_gamma = initial_chunk_gamma;
    gpu.gAMA_fixed = 0;
    std::memset(gpu.read_state.buf, 0, 4);
    std::memcpy(gpu.read_state.buf, ugamma_bytes, 4);
    gpu.read_state.finish_return = finish_ret;
    return &gpu;
}

// Utility to build a 4-byte big-endian representation of a 32-bit value
static void to_be_bytes(png_byte out[4], uint32_t value) {
    out[0] = static_cast<png_byte>((value >> 24) & 0xFF);
    out[1] = static_cast<png_byte>((value >> 16) & 0xFF);
    out[2] = static_cast<png_byte>((value >> 8) & 0xFF);
    out[3] = static_cast<png_byte>(value & 0xFF);
}

// =======================================
// Test cases
// =======================================

// Test 1: CRC finish reports non-zero (simulated error path)
static void test_png_handle_gAMA_crc_finish_error() {
    // ugamma value (arbitrary) but not used because finish_nonzero triggers error
    png_byte ugamma_bytes[4];
    to_be_bytes(ugamma_bytes, 0x01020304);

    png_ptr = make_png_ptr_mock(ugamma_bytes, /*finish_ret=*/1, 0);
    // Call the focal function (mocked)
    int result = fake_png_handle_gAMA(png_ptr, nullptr, 4);

    // Expect handled_error
    ASSERT_EQ(-1, result, "CRC finish non-zero should yield handled_error");
}

// Test 2: ugamma exceeds PNG_UINT_31_MAX triggers benign_error path
static void test_png_handle_gAMA_ugamma_over_max() {
    png_byte ugamma_bytes[4];
    // ugamma = PNG_UINT_31_MAX + 1
    to_be_bytes(ugamma_bytes, PNG_UINT_31_MAX + 1u);

    png_ptr = make_png_ptr_mock(ugamma_bytes, /*finish_ret=*/0, 0);
    int result = fake_png_handle_gAMA(png_ptr, nullptr, 4);

    // Expect handled_error due to invalid gamma value
    ASSERT_EQ(-1, result, "ugamma > PNG_UINT_31_MAX should yield handled_error");
}

// Test 3: Normal case with gamma less than max; verify gAMA_fixed and chunk_gamma update
static void test_png_handle_gAMA_basic_sets_gamma_and_chunk() {
    png_byte ugamma_bytes[4];
    const uint32_t ugamma = 123456u;
    to_be_bytes(ugamma_bytes, ugamma);

    png_ptr = make_png_ptr_mock(ugamma_bytes, /*finish_ret=*/0, /*initial_chunk_gamma=*/0);

    int result = fake_png_handle_gAMA(png_ptr, nullptr, 4);

    ASSERT_EQ(1, result, "Valid ugamma should yield handled_ok");
    ASSERT_EQ(ugamma, static_cast<uint32_t>(png_ptr->gAMA_fixed), "gAMA_fixed should be set to ugamma");
    ASSERT_TRUE(png_ptr->chunk_gamma == static_cast<png_fixed_point>(ugamma),
                "chunk_gamma should be updated to ugamma when initially 0");
}

// Test 4: gamma already set (non-zero); ensure it is not overridden when a new ugamma is provided
static void test_png_handle_gAMA_gamma_not_overwritten_if_already_set() {
    png_byte ugamma_bytes[4];
    const uint32_t ugamma = 70000u;
    to_be_bytes(ugamma_bytes, ugamma);

    // Initialize with existing chunk_gamma non-zero (e.g., 5000)
    png_ptr = make_png_ptr_mock(ugamma_bytes, /*finish_ret=*/0, /*initial_chunk_gamma=*/5000);

    int result = fake_png_handle_gAMA(png_ptr, nullptr, 4);

    ASSERT_EQ(1, result, "Valid ugamma should yield handled_ok even if gamma preset");
    // gAMA_fixed should be ugamma
    ASSERT_EQ(ugamma, static_cast<uint32_t>(png_ptr->gAMA_fixed), "gAMA_fixed should be ugamma");
    // chunk_gamma should remain as initially set (not overwritten)
    ASSERT_EQ(5000, static_cast<uint32_t>(png_ptr->chunk_gamma),
              "chunk_gamma should not be overwritten when already set");
}

// =======================================
// Module main to run tests
// =======================================

int main() {
    // Note: In a real project, we would separate interface vs. implementation
    // and use a proper test framework. Here we orchestrate a simple test run.

    // Run tests with explanatory comments above each test
    std::cout << "Running unit tests for fake_png_handle_gAMA (self-contained mock)\n";

    // Test 1: CRC finish non-zero -> error path
    test_png_handle_gAMA_crc_finish_error();

    // Test 2: ugamma > PNG_UINT_31_MAX -> benign_error path
    test_png_handle_gAMA_ugamma_over_max();

    // Test 3: Normal path -> gAMA_fixed and chunk_gamma updated
    test_png_handle_gAMA_basic_sets_gamma_and_chunk();

    // Test 4: chunk_gamma pre-set -> not overwritten; gAMA_fixed updated
    test_png_handle_gAMA_gamma_not_overwritten_if_already_set();

    // Summary
    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << g_test_failures << " TEST(S) FAILED\n";
        return 1;
    }
}

// Note: The code above provides a compact, self-contained unit test harness for the
// focal logic paths described in png_handle_gAMA. It intentionally does not rely on
// Google Test, and uses a minimal mock of the required state to exercise true/false
// branches of predicates, as requested by the task. In a real project, one would
// prefer to integrate with the project's existing test framework and link against
// the actual libpng implementation.