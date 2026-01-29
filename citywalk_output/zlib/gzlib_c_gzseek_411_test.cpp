/*
Unit test suite for the focal method: gzseek
Context:
- We implement a minimal, self-contained test harness in C++11 that mimics the essential behavior
  of gzseek and its dependency gzseek64 without requiring the full zlib/gzlib build.
- The test uses a mock for gzseek64 to verify:
  1) The "fit" path where returned value fits into z_off_t (positive/normal path).
  2) The "overflow" path where the return value does not fit into z_off_t, exercising the -1 return.
  3) Correct propagation of offset and whence arguments to gzseek64.
- Non-terminating assertions are used (EXPECT_EQ/EXPECT_TRUE) so all tests run and report failures.
*/

#include <iostream>
#include <gzguts.h>
#include <cstdint>


// Lightweight type aliases to simulate the library types used by gzseek and gzseek64
using z_off_t  = int32_t;   // 32-bit signed truncated type (as used by the wrapper)
using z_off64_t = int64_t;  // 64-bit signed full precision type

// Opaque file handle used by the API (mock)
typedef void* gzFile;

// Forward declarations to mirror the focal API.
// gzseek is the function under test; gzseek64 is the dependency we mock.
extern "C" {
    z_off_t gzseek(gzFile file, z_off_t offset, int whence);
    z_off64_t gzseek64(gzFile file, z_off64_t offset, int whence);
}

// Internal static-like mock state to observe calls and control return values.
// In a real unit test, you would use proper mocking frameworks. Here we implement a light-weight
// inline mock to drive and observe the behavior.
struct GzSeekMockState {
    z_off64_t mock_ret;     // Value that gzseek64 will return
    z_off64_t last_offset;  // Last offset passed to gzseek64
    int       last_whence;    // Last whence passed to gzseek64
    bool      called;         // Was gzseek64 called
};

static GzSeekMockState g_mock = {0, 0, 0, false};

// Helper to set the mock return value for gzseek64
extern "C" void set_gzseek64_ret(z_off64_t ret) {
    g_mock.mock_ret = ret;
}

// Mock implementation of gzseek64 used by the focal gzseek wrapper.
// This captures input parameters so tests can verify correct forwarding of args,
// and returns a controllable value to exercise different branches of gzseek.
extern "C" z_off64_t gzseek64(gzFile file, z_off64_t offset, int whence) {
    (void)file; // unused in this mock, but kept to mirror real signature
    g_mock.last_offset = offset;
    g_mock.last_whence = whence;
    g_mock.called = true;
    return g_mock.mock_ret;
}

// Implementation of the focal method under test.
// This mirrors:
// z_off_t gzseek(gzFile file, z_off_t offset, int whence) {
//     z_off64_t ret;
//     ret = gzseek64(file, (z_off64_t)offset, whence);
//     return ret == (z_off_t)ret ? (z_off_t)ret : -1;
// }
extern "C" z_off_t gzseek(gzFile file, z_off_t offset, int whence) {
    z_off64_t ret;
    ret = gzseek64(file, (z_off64_t)offset, whence);
    return ret == (z_off_t)ret ? (z_off_t)ret : -1;
}

// Simple non-terminating test harness helpers

static int g_failures = 0; // Count of failed expectations

#define EXPECT_EQ(expected, actual) do { \
    if ((static_cast<long long>(expected)) != (static_cast<long long>(actual))) { \
        std::cerr << "EXPECT_EQ failed at line " << __LINE__ \
                  << ": expected " << static_cast<long long>(expected) \
                  << ", got " << static_cast<long long>(actual) << "\n"; \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed at line " << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while (0)

// Test 1: Basic fit path where ret fits into z_off_t.
// - Verifies that gzseek forwards offset/whence to gzseek64 and returns the same value.
// - Ensures the wrap logic returns correct value when truncation does not occur.
bool test_gzseek_basic_fit() {
    int before = g_failures;
    // Configure the mock to return a small value that definitely fits into z_off_t
    set_gzseek64_ret(123); // ret will be 123
    z_off_t result = gzseek(nullptr, 7, 0); // offset=7, whence=0
    EXPECT_EQ((z_off_t)123, result);
    // Ensure the wrapper passed the cast of offset correctly to gzseek64
    EXPECT_EQ((z_off64_t)7, g_mock.last_offset);
    EXPECT_EQ(0, g_mock.last_whence);
    return g_failures == before;
}

// Test 2: Overflow path where ret does not fit into z_off_t.
// - Verifies that gzseek returns -1 in this case.
// - Confirms that the value is truncated in the comparison path and triggers the -1 branch.
bool test_gzseek_overflow() {
    int before = g_failures;
    // Choose a value that cannot fit into z_off_t (32-bit). 0x80000000 is -2147483648 in 32-bit signed int.
    // The 64-bit ret won't equal the 32-bit cast, so gzseek should return -1.
    set_gzseek64_ret(0x80000000LL);
    z_off_t result = gzseek(nullptr, 0, 0);
    EXPECT_EQ((z_off_t)(-1), result);
    // Ensure the offset/whence passed through
    EXPECT_EQ((z_off64_t)0, g_mock.last_offset);
    EXPECT_EQ(0, g_mock.last_whence);
    return g_failures == before;
}

// Test 3: Negative offset and non-zero whence to exercise edge cases of casting behavior.
// - Verifies that negative values are handled correctly when they fit.
// - Ensures the offset is passed to gzseek64 as a 64-bit value and that the wrapper preserves sign.
bool test_gzseek_negative_offset_and_whence() {
    int before = g_failures;
    set_gzseek64_ret(-5);              // ret is -5, fits in z_off_t
    z_off_t result = gzseek(nullptr, -5, 1); // whence=1 (SEEK_CUR)
    EXPECT_EQ((z_off_t)(-5), result);
    EXPECT_EQ((z_off64_t)(-5), g_mock.last_offset);
    EXPECT_EQ(1, g_mock.last_whence);
    return g_failures == before;
}

// Main entry point to run tests
int main() {
    std::cout << "Starting gzseek unit tests (single-translation-unit mock)\n";

    if (!test_gzseek_basic_fit()) {
        std::cerr << "[TEST] test_gzseek_basic_fit encountered failures.\n";
    } else {
        std::cout << "[TEST] test_gzseek_basic_fit passed.\n";
    }

    if (!test_gzseek_overflow()) {
        std::cerr << "[TEST] test_gzseek_overflow encountered failures.\n";
    } else {
        std::cout << "[TEST] test_gzseek_overflow passed.\n";
    }

    if (!test_gzseek_negative_offset_and_whence()) {
        std::cerr << "[TEST] test_gzseek_negative_offset_and_whence encountered failures.\n";
    } else {
        std::cout << "[TEST] test_gzseek_negative_offset_and_whence passed.\n";
    }

    std::cout << "gzseek tests completed. Failures: " << g_failures << "\n";

    // Return non-zero if any test failed
    return g_failures ? 1 : 0;
}