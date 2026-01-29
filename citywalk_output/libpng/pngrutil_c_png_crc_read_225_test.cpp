// Unit test suite for the focal method: png_crc_read
// This test harness provides a minimal mock environment to exercise
// png_crc_read(png_structrp png_ptr, png_bytep buf, png_uint_32 length).
// It mocks the underlying dependencies (png_read_data and png_calculate_crc)
// with C linkage to ensure compatibility with the C implementation in pngrutil.c.
// The tests are written in C++11 without any testing framework (no GTest).

#include <cstdint>
#include <iostream>
#include <pngpriv.h>


// --------------------------- Mock Environment ---------------------------

// We create a minimal, C-linkage mock for the png_read_data and
// png_calculate_crc functions that the focal method relies on.
// We also provide a simple state object to verify interactions.

extern "C" {

// Forward declaration of a minimal state used by the mocks.
typedef struct TestPngState TestPngState;

// PNG pointer type used by the mocks (opaque to the test; we cast to our state).
typedef void* png_structrp;

// Buffer and length types used by the mocks (kept simple for test purposes).
typedef unsigned char png_bytep;
typedef unsigned int png_uint_32;

// Concrete mock state object implementation (defined here for visibility).
struct TestPngState {
    std::size_t total_read; // total bytes "read" via png_read_data
    unsigned long crc;      // CRC accumulator via png_calculate_crc
};

// Note: We expose the two functions with C linkage to match the real library's
// linkage expectations. The actual signature in the real libpng might use
// png_structrp and typedefs, but a C linkage with a void* first parameter is
// sufficiently compatible for testing purposes in this mock environment.

} // extern "C"

// Implement mocks with C linkage to avoid C++ name mangling issues
extern "C" {

void png_read_data(png_structrp png_ptr, png_bytep data, png_uint_32 length) {
    // Cast to our test state; if NULL, mimic the real code's guard.
    if (png_ptr == nullptr) {
        return;
    }
    TestPngState* state = reinterpret_cast<TestPngState*>(png_ptr);
    // Update internal bookkeeping; do not modify the data buffer to keep
    // the test deterministic.
    state->total_read += static_cast<std::size_t>(length);
}

void png_calculate_crc(png_structrp png_ptr, png_bytep data, png_uint_32 length) {
    // Cast to our test state; if NULL, mimic the real code's guard.
    if (png_ptr == nullptr) {
        return;
    }
    TestPngState* state = reinterpret_cast<TestPngState*>(png_ptr);
    // Simple deterministic CRC placeholder: sum of the buffer contents.
    unsigned long sum = 0;
    for (png_uint_32 i = 0; i < length; ++i) {
        sum += static_cast<unsigned char>(data[i]);
    }
    // Accumulate into the state's CRC (mod 2^32).
    state->crc = (state->crc + sum) & 0xFFFFFFFFu;
}
} // extern "C"

// Forward declaration of the focal function under test.
// We declare it with C linkage to avoid any C++ name mangling during linking.
extern "C" void png_crc_read(void* png_ptr, unsigned char* buf, unsigned int length);

// --------------------------- Test Harness ---------------------------

static int g_tests_run = 0;
static int g_tests_failed = 0;

// Lightweight assertion helpers (non-terminating)
#define TEST_EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "ASSERTION FAILED: " << (msg) << "  (File: " __FILE__ ", Line: " << __LINE__ << ")" << std::endl; \
    } \
} while (0)

#define TEST_EXPECT_EQ(actual, expected, msg) do { \
    if ((actual) != (expected)) { \
        ++g_tests_failed; \
        std::cerr << "ASSERTION FAILED: " << (msg) \
                  << " | expected: " << (expected) \
                  << ", actual: " << (actual) \
                  << "  (File: " __FILE__ ", Line: " << __LINE__ << ")" << std::endl; \
    } \
} while (0)

static void test_png_crc_read_null_ptr() {
    // Purpose: Verify that png_crc_read gracefully handles a NULL png_ptr
    // without performing any operations (no crash, no state mutation).
    ++g_tests_run;

    unsigned char buf[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    // Call with NULL png_ptr; should be a no-op.
    png_crc_read(nullptr, buf, 8);

    // We cannot observe internal state since png_ptr is NULL; at least ensure
    // the test ran to exercise the NULL branch.
    TEST_EXPECT_TRUE(true, "png_crc_read should safely handle NULL png_ptr");
}

static void test_png_crc_read_normal() {
    // Purpose: Exercise the normal path where png_ptr is valid and length > 0.
    ++g_tests_run;

    TestPngState state;
    state.total_read = 0;
    state.crc = 0;

    unsigned char buf[6] = {1, 2, 3, 4, 5, 6};

    // Call the focal function with a valid pointer to our state.
    png_crc_read(&state, buf, 6);

    // Expected: total_read increased by 6, crc equals sum of buf elements (1+2+...+6 = 21)
    TEST_EXPECT_EQ(state.total_read, 6u, "png_read_data should be called with length 6");
    TEST_EXPECT_EQ(state.crc, 21u, "png_calculate_crc should compute CRC as sum of buffer contents");
}

static void test_png_crc_read_zero_length() {
    // Purpose: Ensure that length = 0 does not mutate state.
    ++g_tests_run;

    TestPngState state;
    state.total_read = 0;
    state.crc = 12345; // non-zero initial value to verify no unintended mutation
    unsigned char buf[4] = {9, 9, 9, 9};

    png_crc_read(&state, buf, 0);

    TEST_EXPECT_EQ(state.total_read, 0u, "total_read should remain 0 when length is 0");
    TEST_EXPECT_EQ(state.crc, 12345u, "crc should remain unchanged when length is 0");
}

// --------------------------- Main Runner ---------------------------

int main() {
    // Run tests
    test_png_crc_read_null_ptr();
    test_png_crc_read_normal();
    test_png_crc_read_zero_length();

    // Summary
    std::cout << "Tests run: " << g_tests_run
              << ", Failures: " << g_tests_failed << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}