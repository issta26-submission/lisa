/*
Unit test suite for the focal method:
int ZCALLBACK win32_error_file_func(voidpf opaque, voidpf stream)

Overview:
- Verifies that when stream is NULL, the function returns -1.
- Verifies that when stream is non-NULL, the function returns the value of the 'error' member
  inside the WIN32FILE_IOWIN structure pointed to by stream.
- Uses a lightweight, non-terminating test harness (EXPECT_EQ) to maximize code execution coverage
  without depending on any external testing framework (GTest, GoogleMock, etc.).

Notes:
- To avoid requiring the exact internal definition of WIN32FILE_IOWIN (which is defined in iowin32.h),
  tests create a small local struct with an int member named 'error' as the first field and pass its
  address cast to voidpf. The implementation of win32_error_file_func simply casts the stream to
  WIN32FILE_IOWIN* and reads ->error; with a compatible memory layout (first member is 'error'), this
  test exercise is valid without needing the precise type layout beyond the first int field.
- The test uses the physical signature declared in iowin32.h (via included headers) and calls the
  function with different stream and opaque values to validate both branches.

Candidate Keywords (Step 1):
- win32_error_file_func, WIN32FILE_IOWIN, voidpf, stream null check, error member, -1 default,
  opaque parameter, ZCALLBACK, iowin32.h, ioapi.h, zlib.h

Included headers are expected to be available in the project layout (zlib/ioapi/iowin32):
- ioapi.h for voidpf and related types
- iowin32.h for the focal function prototype (ZCALLBACK and win32_error_file_func)

Implementation details:
- A minimal test harness with EXPECT_EQ macro to count passes/fails without aborting on failures.
- Tests:
  1) Null stream returns -1 (branch: stream == NULL)
  2) Non-null stream with error = 0 returns 0
  3) Non-null stream with error = some value (e.g., 7) returns that value
  4) Non-null stream with non-zero opaque should still return the same error (opaque is ignored by function)

Now the code:

*/

#include <iowin32.h>
#include <cstdint>
#include <iostream>
#include <ioapi.h>
#include <zlib.h>
#include <stdlib.h>


// Include zlib IO headers to obtain necessary typedefs and function prototype.
// Assumes the project provides these headers in include paths.
extern "C" {
}

// The focal function is defined in iowin32.c and exposed via iowin32.h.
// We declare it here to enable calling from C++ test code.
// Use a C linkage to avoid name mangling issues when linking with the C implementation.
extern "C" int ZCALLBACK win32_error_file_func(voidpf opaque, voidpf stream);

// Lightweight test harness
static int g_total = 0;
static int g_pass  = 0;
static int g_fail  = 0;

// Non-terminating assertion macro
#define EXPECT_EQ(expected, actual) do {                                \
    ++g_total;                                                             \
    auto _exp = (expected); auto _act = (actual);                         \
    if (_exp == _act) {                                                  \
        ++g_pass;                                                        \
    } else {                                                             \
        ++g_fail;                                                        \
        std::cerr << "Expectation failed at " << __FILE__ << ":" << __LINE__ \
                  << " -> expected: " << _exp << " actual: " << _act << "\n"; \
    }                                                                    \
} while (0)

// Fake stream type to simulate WIN32FILE_IOWIN layout where the first member is 'error'
struct FakeWin32IOWin {
    int error; // Expected to be the first member in WIN32FILE_IOWIN for the cast to work
};

// Test 1: stream == NULL => returns -1
void test_null_stream_returns_minus_one() {
    int ret = win32_error_file_func(nullptr, nullptr);
    EXPECT_EQ(-1, ret);
}

// Test 2: stream != NULL and error == 0 => returns 0
void test_non_null_stream_error_zero() {
    FakeWin32IOWin fake;
    fake.error = 0;
    int ret = win32_error_file_func(nullptr, (void*) &fake);
    EXPECT_EQ(0, ret);
}

// Test 3: stream != NULL and error == 7 => returns 7
void test_non_null_stream_error_seven() {
    FakeWin32IOWin fake;
    fake.error = 7;
    int ret = win32_error_file_func(nullptr, (void*) &fake);
    EXPECT_EQ(7, ret);
}

// Test 4: opaque value is ignored; ensure error is still retrieved from stream
void test_opaque_ignored_by_function() {
    FakeWin32IOWin fake;
    fake.error = 42;
    void* opaque = (void*)0x12345678; // Any non-null opaque value
    int ret = win32_error_file_func(opaque, (void*) &fake);
    EXPECT_EQ(42, ret);
}

// Run all tests and report summary
int main() {
    test_null_stream_returns_minus_one();
    test_non_null_stream_error_zero();
    test_non_null_stream_error_seven();
    test_opaque_ignored_by_function();

    std::cout << "Tests run: " << g_total
              << ", Passed: " << g_pass
              << ", Failed: " << g_fail << std::endl;

    // Return non-zero if any test failed to signal issues to build systems
    return (g_fail == 0) ? 0 : 1;
}