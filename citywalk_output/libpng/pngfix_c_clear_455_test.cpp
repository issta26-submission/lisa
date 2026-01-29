// This test suite targets the focal method:
//   clear(void *pv, size_t size)
// defined in pngfix.c
// The function uses a static seed and make_random_bytes to fill the provided buffer.
// The tests are written in C++11 without any testing framework (no GTest).
// The code relies on linking against the pngfix.c implementation (which provides clear).
// Each test is a self-contained function returning true on success and false on failure.

#include <cstddef>
#include <setjmp.h>
#include <vector>
#include <string.h>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Declare the focal C function with C linkage so the C++ compiler/linker can resolve it.
extern "C" void clear(void* pv, size_t size);

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_RUN(test_name)                                                     \
    do {                                                                          \
        bool ok = test_name();                                                     \
        ++g_total_tests;                                                          \
        if (!ok) {                                                                \
            ++g_failed_tests;                                                      \
            std::cerr << "FAILED: " #test_name " at " __FILE__ ":" << __LINE__ << "\n"; \
        } else {                                                                    \
            std::cout << "PASSED: " #test_name "\n";                               \
        }                                                                         \
    } while (0)

// Test 1: Basic behavior - after clear, the memory should differ from the initial pattern.
// This validates that clear writes to the memory using the RNG path (even though
// the internal RNG is deterministic, it should produce bytes different from a uniform pattern).
static bool test_clear_changes_memory_basic() {
    const size_t N = 128;
    std::vector<unsigned char> buf(N);
    // Initialize with a non-trivial pattern
    for (size_t i = 0; i < N; ++i) buf[i] = static_cast<unsigned char>(0x55);
    // Invoke the focal method
    clear(buf.data(), buf.size());
    // Check that at least one byte changed from 0x55
    bool all_same = true;
    for (size_t i = 0; i < N; ++i) {
        if (buf[i] != 0x55) {
            all_same = false;
            break;
        }
    }
    // If all bytes remained 0x55, something unexpected happened
    return !all_same;
}

// Test 2: Deterministic but non-identical successive calls.
// Ensure two consecutive clears on the same buffer yield different content
// (the RNG seed is static but should evolve between calls).
static bool test_clear_two_calls_differs() {
    const size_t N = 256;
    std::vector<unsigned char> buf(N, 0xAA); // start with a known pattern
    clear(buf.data(), buf.size());
    std::vector<unsigned char> after_first = buf;
    clear(buf.data(), buf.size());
    // Compare with the result after the first clear
    bool identical = (std::memcmp(after_first.data(), buf.data(), N) == 0);
    // If identical, RNG produced same output twice (unlikely)
    return !identical;
}

// Test 3: Edge case - size == 0 should not modify the buffer.
// The function should handle zero-length writes gracefully.
static bool test_clear_zero_size_no_change() {
    const size_t N = 64;
    std::vector<unsigned char> buf(N);
    // Fill with a distinct pattern
    for (size_t i = 0; i < N; ++i) buf[i] = static_cast<unsigned char>(i & 0xFF);
    std::vector<unsigned char> before = buf;
    clear(buf.data(), 0);
    // Memory should be unchanged
    return (std::memcmp(buf.data(), before.data(), N) == 0);
}

// Test 4: Small size boundary - size == 1 should still modify at least one byte.
static bool test_clear_size_one() {
    const size_t N = 1;
    std::vector<unsigned char> buf(N, 0x00);
    clear(buf.data(), buf.size());
    // Expect at least one byte changed from 0x00
    return buf[0] != 0x00;
}

// Entry point for the test executable
int main() {
    std::cout << "Starting tests for focal method: clear(void*, size_t)\n";

    TEST_RUN(test_clear_changes_memory_basic);
    TEST_RUN(test_clear_two_calls_differs);
    TEST_RUN(test_clear_zero_size_no_change);
    TEST_RUN(test_clear_size_one);

    std::cout << "Tests completed. Passed: "
              << (g_total_tests - g_failed_tests)
              << ", Failed: " << g_failed_tests
              << ", Total: " << g_total_tests << "\n";

    // Non-zero exit code on failure to aid automation
    return g_failed_tests != 0 ? 1 : 0;
}