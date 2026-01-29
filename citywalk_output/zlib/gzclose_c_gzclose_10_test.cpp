// Lightweight C++11 test harness for the focal function: gzclose
// This test suite is crafted without Google Test, using simple
// runtime checks and output to stdout/stderr.
// It focuses on providing coverage for the core dependent paths of gzclose
// as defined in the provided focal method, including both branches of the
// conditional and both branches of the mode-checking ternary operator.

#include <iostream>
#include <gzguts.h>


// Minimal reimplementation of the types and constants required by gzclose.
// This is a self-contained harness to validate the logic of gzclose without
// needing the full zlib build setup.

// Typedefs and constants (mirroring the expectations of the focal code)
typedef void* gzFile;
typedef struct gz_state {
    int mode; // GZ_READ or GZ_WRITE
} gz_state;
typedef gz_state* gz_statep;

#define GZ_READ 0
#define GZ_WRITE 1

// The real zlib uses Z_STREAM_ERROR (-2) for null/invalid stream parameters.
// We replicate the essential constant here for assertion purposes.
#define Z_STREAM_ERROR -2

// The export macro used in the focal code (no special behavior needed here)
#define ZEXPORT

// Forward declarations of the internal helpers that gzclose would call.
// In the real project these would be provided by the library.
// Here we provide simple test doubles to observe behavior.
extern "C" int gzclose_r(gzFile file);
extern "C" int gzclose_w(gzFile file);

// Internal flags to verify which branch is taken by the focal method.
static bool g_r_called = false;
static bool g_w_called = false;
static void* g_last_r_file = nullptr;
static void* g_last_w_file = nullptr;

// Mock implementations to control behavior of the internal helpers.
extern "C" int gzclose_r(gzFile file) {
    g_r_called = true;
    g_last_r_file = file;
    return 123; // Arbitrary sentinel value for test validation
}
extern "C" int gzclose_w(gzFile file) {
    g_w_called = true;
    g_last_w_file = file;
    return 456; // Arbitrary sentinel value for test validation
}

// Focal method under test (as provided in the prompt, adapted to this test harness)
int ZEXPORT gzclose(gzFile file) {
#ifndef NO_GZCOMPRESS
    gz_statep state;
    if (file == NULL)
        return Z_STREAM_ERROR;
    state = (gz_statep)file;
    return state->mode == GZ_READ ? gzclose_r(file) : gzclose_w(file);
#else
    return gzclose_r(file);
#endif
}

// Lightweight assertion helper: non-terminating, prints outcome and continues.
static void expect_int_eq(int actual, int expected, const char* message) {
    if (actual != expected) {
        std::cerr << "FAIL: " << message
                  << " | Expected: " << expected
                  << "  Actual: " << actual << std::endl;
    } else {
        std::cout << "PASS: " << message << std::endl;
    }
}

// Test 1: NULL file should return Z_STREAM_ERROR (true branch of the NULL check)
static void test_gzclose_null_file() {
    // Comment: Ensures gzclose handles NULL input correctly by returning Z_STREAM_ERROR.
    int result = gzclose(nullptr);
    expect_int_eq(result, Z_STREAM_ERROR, "gzclose(NULL) returns Z_STREAM_ERROR");
}

// Test 2: Mode = GZ_READ should call gzclose_r and return its value (true branch of the ternary)
static void test_gzclose_read_mode_calls_r() {
    // Reset trackers
    g_r_called = false;
    g_w_called = false;
    g_last_r_file = nullptr;
    g_last_w_file = nullptr;

    gz_state s;
    s.mode = GZ_READ;
    void* file_ptr = &s;

    int res = gzclose(file_ptr);

    // Assertions
    expect_int_eq(g_r_called ? 1 : 0, 1, "gzclose_r should be called when mode is GZ_READ");
    expect_int_eq(g_w_called ? 1 : 0, 0, "gzclose_w should not be called when mode is GZ_READ");
    expect_int_eq(res, 123, "gzclose_r return value is propagated");
    // Ensure the correct file is passed through
    expect_int_eq(g_last_r_file == file_ptr ? 1 : 0, 1, "gzclose_r should receive the original file pointer");
}

// Test 3: Mode = GZ_WRITE should call gzclose_w and return its value (false branch of the ternary)
static void test_gzclose_write_mode_calls_w() {
    // Reset trackers
    g_r_called = false;
    g_w_called = false;
    g_last_r_file = nullptr;
    g_last_w_file = nullptr;

    gz_state s;
    s.mode = GZ_WRITE;
    void* file_ptr = &s;

    int res = gzclose(file_ptr);

    // Assertions
    expect_int_eq(g_w_called ? 1 : 0, 1, "gzclose_w should be called when mode is GZ_WRITE");
    expect_int_eq(g_r_called ? 1 : 0, 0, "gzclose_r should not be called when mode is GZ_WRITE");
    expect_int_eq(res, 456, "gzclose_w return value is propagated");
    // Ensure the correct file is passed through
    expect_int_eq(g_last_w_file == file_ptr ? 1 : 0, 1, "gzclose_w should receive the original file pointer");
}

// Run all tests
static void run_tests() {
    std::cout << "Starting gzclose unit tests (no GTest):" << std::endl;
    test_gzclose_null_file();
    test_gzclose_read_mode_calls_r();
    test_gzclose_write_mode_calls_w();
    std::cout << "gzclose unit tests completed." << std::endl;
}

int main() {
    run_tests();
    return 0;
}