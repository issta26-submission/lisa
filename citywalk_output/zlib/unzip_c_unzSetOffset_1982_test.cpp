// Test suite for the focal method: unzSetOffset in unzip.c
// This test uses a linker wrap approach to intercept the underlying unzSetOffset64
// implementation. Build notes:
// - Ensure unzip.c is compiled into the executable (e.g., g++ test.cpp unzip.c ...)
// - Link with a wrapper for unzSetOffset64 to capture calls:
//     g++ -std=c++11 test_unzSetOffset.cpp unzip.c -Wl,--wrap=unzSetOffset64 -o test_unzSetOffset
// - The wrapper __wrap_unzSetOffset64 will intercept calls and return a sentinel value.
// - We then verify that unzSetOffset forwards to unzSetOffset64 and that parameters
//   and return values are propagated correctly.

#include <string.h>
#include <cstdint>
#include <stdio.h>
#include <crypt.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <unzip.h>


// Domain: We model only what's necessary to call into the focal function.
// Provide minimal type aliases to avoid depending on the full unzip headers in this test.

typedef void* unzFile;
typedef unsigned long uLong;
typedef unsigned long long ZPOS64_T;

// Prototype of the function under test (provided by unzip.c / unzip.h in the real project)
extern "C" int unzSetOffset(unzFile file, uLong pos);

// The following globals support the wrapper-based interception. They are used by the wrapper
// to record how the function was invoked.
int g_wrap_calls = 0;
ZPOS64_T g_last_pos = 0;

// Wrapper for unzSetOffset64. In build, link with: -Wl,--wrap=unzSetOffset64
extern "C" int __wrap_unzSetOffset64(unzFile file, ZPOS64_T pos) {
    // Record invocation details for assertions in tests
    ++g_wrap_calls;
    g_last_pos = pos;
    // Return a sentinel value so that tests can verify the wrapper is indeed invoked.
    return 12345;
}

// Simple, lightweight test framework (no Google Test). Tests return true on success.
bool test_basic_forwarding_single_call() {
    g_wrap_calls = 0;
    g_last_pos = 0;

    // Call the wrapper-wrapped path via the focal function.
    // The actual unzSetOffset64 will be intercepted by __wrap_unzSetOffset64.
    int ret = unzSetOffset(nullptr, 7);

    if (ret != 12345) {
        std::cerr << "[FAIL] test_basic_forwarding_single_call: unexpected return value: "
                  << ret << " (expected 12345)\n";
        return false;
    }
    if (g_wrap_calls != 1) {
        std::cerr << "[FAIL] test_basic_forwarding_single_call: wrap_calls=" << g_wrap_calls
                  << " (expected 1)\n";
        return false;
    }
    if (g_last_pos != 7) {
        std::cerr << "[FAIL] test_basic_forwarding_single_call: last_pos=" << g_last_pos
                  << " (expected 7)\n";
        return false;
    }
    return true;
}

bool test_basic_forwarding_multiple_calls() {
    g_wrap_calls = 0;
    g_last_pos = 0;

    // First call
    int ret1 = unzSetOffset(nullptr, 0);
    // Second call with a large 64-bit position
    int ret2 = unzSetOffset(nullptr, 1234567890123ULL);

    if (ret1 != 12345 || ret2 != 12345) {
        std::cerr << "[FAIL] test_basic_forwarding_multiple_calls: unexpected return values: "
                  << ret1 << ", " << ret2 << " (expected 12345, 12345)\n";
        return false;
    }
    if (g_wrap_calls != 2) {
        std::cerr << "[FAIL] test_basic_forwarding_multiple_calls: wrap_calls=" << g_wrap_calls
                  << " (expected 2)\n";
        return false;
    }
    if (g_last_pos != 1234567890123ULL) {
        std::cerr << "[FAIL] test_basic_forwarding_multiple_calls: last_pos=" << g_last_pos
                  << " (expected 1234567890123)\n";
        return false;
    }
    return true;
}

int main() {
    int total = 0;
    int passed = 0;

    std::cout << "Starting tests for unzSetOffset forwarding wrapper...\n";

    if (test_basic_forwarding_single_call()) { ++passed; } else { /* no-op */ }
    ++total;

    if (test_basic_forwarding_multiple_calls()) { ++passed; } else { /* no-op */ }
    ++total;

    std::cout << "Tests passed: " << passed << " / " << total << "\n";

    return (passed == total) ? 0 : 1;
}