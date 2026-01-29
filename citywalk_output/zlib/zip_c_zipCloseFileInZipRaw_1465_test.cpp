// C++11 based unit tests for the focal method zipCloseFileInZipRaw
// This test suite uses a linker wrap technique to intercept calls to
// the underlying 64-bit implementation (zipCloseFileInZipRaw64).
// Build note: Compile and link against the libzip sources, and use:
//   g++ -std=c++11 -O2 -Wall -Wextra -Wpedantic test_zipCloseFileInZipRaw.cpp zip.c -c
//   g++ -std=c++11 test_zipCloseFileInZipRaw.o zip.o -Wl,--wrap=zipCloseFileInZipRaw64 -o test_zipCloseFileInZipRaw
// The --wrap option makes all calls to zipCloseFileInZipRaw64 be redirected to
// __wrap_zipCloseFileInZipRaw64, which we implement here to observe the forwarding.
// Externally, zipCloseFileInZipRaw is the wrapper under test and is expected to forward
// its arguments unchanged to zipCloseFileInZipRaw64.

#include <cstdio>
#include <string.h>
#include <time.h>
#include <string>
#include <cstdint>
#include <functional>
#include <stdio.h>
#include <stdint.h>
#include <crypt.h>
#include <vector>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <zip.h>
#include <cstdlib>


// Include the public C API declarations (zip.h, zlib.h) with C linkage in C++.
extern "C" {
}

// ----------------------------------------------------------------------------------
// Test harness scaffolding
// ----------------------------------------------------------------------------------

// Global state used by the wrap function to report captured arguments back to tests.
static void* g_wrap_last_file = nullptr;
static ZPOS64_T g_wrap_last_uncompressed_size = 0;
static uLong g_wrap_last_crc32 = 0;
static int   g_wrap_last_call_seen = 0; // how many times wrap was invoked

// What value to return from the wrapped 64-bit function (to verify forwarding)
static int g_wrap_ret_value = 0;

// Reset the captured state before each test
static void wrap_reset_state() {
    g_wrap_last_file = nullptr;
    g_wrap_last_uncompressed_size = 0;
    g_wrap_last_crc32 = 0;
    g_wrap_last_call_seen = 0;
    // Do not reset g_wrap_ret_value; tests set it explicitly as needed
}

// The wrap (interceptor) for zipCloseFileInZipRaw64
// The linker will replace calls to zipCloseFileInZipRaw64 with __wrap_zipCloseFileInZipRaw64
// in the compilation unit that references zipCloseFileInZipRaw64 (via -Wl,--wrap=zipCloseFileInZipRaw64).
extern "C" int __wrap_zipCloseFileInZipRaw64(zipFile file, ZPOS64_T uncompressed_size, uLong crc32) {
    // Record the arguments for assertions in tests
    g_wrap_last_file = static_cast<void*>(file);
    g_wrap_last_uncompressed_size = uncompressed_size;
    g_wrap_last_crc32 = crc32;
    g_wrap_last_call_seen++;
    return g_wrap_ret_value;
}

// ----------------------------------------------------------------------------------
// Lightweight test framework (non-terminating assertions)
// We accumulate failures and report at the end to maximize code coverage execution.
// This aligns with the domain knowledge request to use non-terminating assertions.
//
static std::vector<std::string> g_failures;
static std::string g_current_test;

// Simple assertion macro that records failures but does not abort tests.
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        g_failures.push_back("[" + g_current_test + "] " + std::string(msg)); \
    } \
} while(0)

static void print_summary(size_t total_tests) {
    if(g_failures.empty()) {
        std::cout << "[TEST] All " << total_tests << " tests passed.\n";
    } else {
        std::cout << "[TEST] " << g_failures.size() << " failures out of " << total_tests << " tests:\n";
        for(const auto& f : g_failures) {
            std::cout << "  " << f << "\n";
        }
    }
}

// ----------------------------------------------------------------------------------
// Step 2: Unit Test Generation for zipCloseFileInZipRaw
// Tests focus on forwarding behavior: ensuring zipCloseFileInZipRaw forwards its
// parameters unchanged to zipCloseFileInZipRaw64, and that the wrapper's return value
// matches the 64-bit function's return value (which we control via g_wrap_ret_value).
//
// The tests rely on the linker wrap feature described in the build notes above.
// Each test sets up the expected return value, calls the wrapper, and asserts:
// - The return value equals the wrapped function's return value.
// - The wrapped function received the exact arguments provided to the wrapper.
// - The wrapper forwards all argument values without modification.
//
// Tests included (by intent):
// 1. Forwarding with zero/uncompressed_size and zero CRC
// 2. Forwarding with non-zero, typical values
// 3. Forwarding with large/unusual values to test type wide-compatibility
// 4. Multiple consecutive calls validate state resets and forwarding consistency
// 5. Boundary checks for NULL file pointer forwarding
// ----------------------------------------------------------------------------------

// Helper to run a test and report outcome
static void run_test(const std::string& name, const std::function<void()>& fn) {
    g_current_test = name;
    wrap_reset_state();
    g_failures.clear();
    // Execute test
    fn();
    // Report per-test summary
    if(g_failures.empty()) {
        std::cout << "[TEST] PASSED: " << name << "\n";
    } else {
        std::cout << "[TEST] FAILED: " << name << " (see details below)\n";
    }
}

// Test 1: Forwarding basic values (0, 0) and NULL file
static void test_zipCloseFileInZipRaw_forwards_zero_values() {
    // Configure wrap return value and reset capture state
    g_wrap_ret_value = 7;
    wrap_reset_state();

    // Call the wrapper with NULL file and zero values
    zipFile dummy = nullptr;
    int ret = zipCloseFileInZipRaw(dummy, 0, 0);

    // Assertions
    TEST_ASSERT(ret == g_wrap_ret_value, "Wrapper did not return the wrapped function's return value");
    TEST_ASSERT(g_wrap_last_call_seen == 1, "Wrapped function should be called exactly once");
    TEST_ASSERT(g_wrap_last_file == dummy, "Wrapped function should receive the same file pointer");
    TEST_ASSERT(g_wrap_last_uncompressed_size == 0, "Wrapped function should receive unchanged uncompressed_size");
    TEST_ASSERT(g_wrap_last_crc32 == 0, "Wrapped function should receive unchanged crc32");
}

// Test 2: Forwarding with non-zero, typical values
static void test_zipCloseFileInZipRaw_forwards_typical_values() {
    g_wrap_ret_value = 42;
    wrap_reset_state();

    // Use a non-NULL dummy file value
    zipFile dummy = reinterpret_cast<zipFile>(reinterpret_cast<uintptr_t>(0xDEADBEEF));
    uLong crc = 0xABCDEF01;
    ZPOS64_T uncompressed = 123456789ULL;

    int ret = zipCloseFileInZipRaw(dummy, static_cast<uLong>(uncompressed & 0xFFFFFFFFULL), crc);

    TEST_ASSERT(ret == g_wrap_ret_value, "Wrapper return value mismatch for typical values");
    TEST_ASSERT(g_wrap_last_call_seen == 1, "Wrapped function should be called once for typical values");
    TEST_ASSERT(g_wrap_last_file == dummy, "File pointer mismatch for typical values");
    TEST_ASSERT(g_wrap_last_uncompressed_size == static_cast<ZPOS64_T>(uncompressed), "Uncompressed size mismatch for typical values");
    TEST_ASSERT(g_wrap_last_crc32 == crc, "CRC mismatch for typical values");
}

// Test 3: Forwarding with large values to exercise type boundaries
static void test_zipCloseFileInZipRaw_forwards_large_values() {
    g_wrap_ret_value = 99;
    wrap_reset_state();

    zipFile dummy = reinterpret_cast<zipFile>(reinterpret_cast<void*>(0xCAFEBABE));
    ZPOS64_T uncompressed_large = 0x0000000100000000ULL; // large 64-bit sized pretend value
    uLong crc_large = 0xFFFFFFFF;

    int ret = zipCloseFileInZipRaw(dummy, static_cast<uLong>(static_cast<uint64_t>(uncompressed_large) & 0xFFFFFFFFULL), crc_large);

    TEST_ASSERT(ret == g_wrap_ret_value, "Wrapper return value mismatch for large values");
    TEST_ASSERT(g_wrap_last_call_seen == 1, "Wrapped function should be called for large values");
    TEST_ASSERT(g_wrap_last_file == dummy, "File pointer mismatch for large values");
    TEST_ASSERT(g_wrap_last_uncompressed_size == static_cast<ZPOS64_T>(static_cast<uint64_t>(uncompressed_large) & 0xFFFFFFFFULL),
               "Uncompressed size value should be forwarded unchanged (low 32 bits checked)");
    TEST_ASSERT(g_wrap_last_crc32 == crc_large, "CRC mismatch for large values");
}

// Test 4: Multiple consecutive calls ensure forwarding consistency and state reset
static void test_zipCloseFileInZipRaw_consecutive_calls() {
    // First call
    g_wrap_ret_value = 11;
    wrap_reset_state();
    zipFile a = reinterpret_cast<zipFile>(reinterpret_cast<void*>(0x1001));
    int ret1 = zipCloseFileInZipRaw(a, 1, 1);
    TEST_ASSERT(ret1 == g_wrap_ret_value, "First call: return value mismatch");
    TEST_ASSERT(g_wrap_last_call_seen == 1, "First call: wrapped function should be invoked once");

    // Second call with different values
    g_wrap_ret_value = 22;
    wrap_reset_state();
    zipFile b = reinterpret_cast<zipFile>(reinterpret_cast<void*>(0x2002));
    int ret2 = zipCloseFileInZipRaw(b, 2, 2);
    TEST_ASSERT(ret2 == g_wrap_ret_value, "Second call: return value mismatch");
    TEST_ASSERT(g_wrap_last_call_seen == 1, "Second call: wrapped function should be invoked once");
    TEST_ASSERT(g_wrap_last_file == b, "Second call: file pointer mismatch");
    TEST_ASSERT(g_wrap_last_uncompressed_size == 2, "Second call: uncompressed size mismatch");
    TEST_ASSERT(g_wrap_last_crc32 == 2, "Second call: crc32 mismatch");
}

// Test 5: Boundary check: forwarding with NULL file pointer and large CRC
static void test_zipCloseFileInZipRaw_null_file_boundary() {
    g_wrap_ret_value = 77;
    wrap_reset_state();

    zipFile nullfile = nullptr;
    uLong large_crc = 0xDEADBEEF;
    ZPOS64_T uncompressed = 0x7FFFFFFFFFFFFFFFULL; // large value within ZPOS64_T

    int ret = zipCloseFileInZipRaw(nullfile, static_cast<uLong>(uncompressed & 0xFFFFFFFFULL), large_crc);

    TEST_ASSERT(ret == g_wrap_ret_value, "Boundary test: return value mismatch");
    TEST_ASSERT(g_wrap_last_call_seen == 1, "Boundary test: wrapped function called once");
    TEST_ASSERT(g_wrap_last_file == nullfile, "Boundary test: file pointer should be NULL");
    TEST_ASSERT(g_wrap_last_uncompressed_size == static_cast<ZPOS64_T>(static_cast<uint64_t>(uncompressed) & 0xFFFFFFFFULL),
               "Boundary test: uncompressed size not forwarded correctly (lower 32 bits)");
    TEST_ASSERT(g_wrap_last_crc32 == large_crc, "Boundary test: crc32 mismatch");
}

// ----------------------------------------------------------------------------------
// Step 3: Test Case Refinement (explanatory comments present in code above)
// The tests are designed to maximize code coverage by exercising forwarding logic
// across a variety of inputs, including edge values, NULL pointers, and multiple calls.
// They use a non-terminating assertion approach to allow all tests to run and collect
// failures before reporting results.
// ----------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------
// Main: execute all tests
// ----------------------------------------------------------------------------------
int main() {
    // Version of tests to run
    size_t total_tests = 0;

    run_test("zipCloseFileInZipRaw_forwards_zero_values", []{
        test_zipCloseFileInZipRaw_forwards_zero_values();
    });
    total_tests++;

    run_test("zipCloseFileInZipRaw_forwards_typical_values", []{
        test_zipCloseFileInZipRaw_forwards_typical_values();
    });
    total_tests++;

    run_test("zipCloseFileInZipRaw_forwards_large_values", []{
        test_zipCloseFileInZipRaw_forwards_large_values();
    });
    total_tests++;

    run_test("zipCloseFileInZipRaw_consecutive_calls", []{
        test_zipCloseFileInZipRaw_consecutive_calls();
    });
    total_tests++;

    run_test("zipCloseFileInZipRaw_null_file_boundary", []{
        test_zipCloseFileInZipRaw_null_file_boundary();
    });
    total_tests++;

    // Report a final summary
    print_summary(total_tests);

    // If there were failures, signal non-zero exit
    if(!g_failures.empty()) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}