// Unit tests for the focal method load_file from makepng.c
// This test harness is written in C++11 without using GTest.
// It assumes linkage against a library/object that provides the C function:
//   size_t load_file(const char* name, unsigned char** result);
// The actual implementation (load_file) is provided by the project under test.
// To avoid conflicting mains, link this test against a compiled library that exports load_file
// but does not include the makepng.c main symbol, or adapt your build system accordingly.

#include <cstddef>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <fstream>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Declaration of the focal function with C linkage.
// We match the expected signature: size_t load_file(const char* name, unsigned char** result)
extern "C" size_t load_file(const char* name, unsigned char** result);

// Global counters for simple test reporting
static int tests_passed = 0;
static int tests_failed = 0;

// Helper macro to record test results with a message
#define ASSERT(cond, msg)                                  \
    do {                                                     \
        if (cond) {                                          \
            ++tests_passed;                                  \
        } else {                                             \
            ++tests_failed;                                  \
            std::cerr << "[FAIL] " << msg << "\n";          \
        }                                                    \
    } while(0)


// Test 1: Load a small 4-byte file
// Purpose: Verify normal successful path where total == 4 and data is copied correctly.
// Rationale: When total is a multiple of 4, the allocation size equals total.
// We also ensure the content read matches the input bytes.
void test_load_file_small_exact_multiple_of_4() {
    const char* fname = "test_input_exact4.bin";
    // Prepare test payload: 4 bytes
    const unsigned char input[4] = { 0xDE, 0xAD, 0xBE, 0xEF };

    // Write test file
    {
        std::ofstream f(fname, std::ios::binary);
        f.write(reinterpret_cast<const char*>(input), 4);
        f.close();
    }

    // Call the focal function
    unsigned char* result_buf = nullptr;
    size_t total = load_file(fname, &result_buf);

    // Assertions
    ASSERT(total == 4, "load_file should return total == 4 for 4-byte input");
    ASSERT(result_buf != nullptr, "load_file should allocate a buffer for 4-byte input");
    if (result_buf != nullptr) {
        ASSERT(std::memcmp(result_buf, input, 4) == 0, "buffer content should match input bytes");
        std::free(result_buf);
    }

    // Cleanup
    std::remove(fname);
}

// Test 2: Load a small 3-byte file
// Purpose: Verify that the function rounds up to a 4-byte boundary for internal allocation
//          and still returns the correct total and data content.
// Rationale: total == 3 should allocate 4 bytes ( (3+3) & ~3 == 4 ) but only first 3 bytes
//            are valid data; the test ensures data integrity.
void test_load_file_small_not_multiple_of_4() {
    const char* fname = "test_input_exact3.bin";
    // Prepare test payload: 3 bytes
    const unsigned char input[3] = { 0x11, 0x22, 0x33 };

    // Write test file
    {
        std::ofstream f(fname, std::ios::binary);
        f.write(reinterpret_cast<const char*>(input), 3);
        f.close();
    }

    // Call the focal function
    unsigned char* result_buf = nullptr;
    size_t total = load_file(fname, &result_buf);

    // Assertions
    ASSERT(total == 3, "load_file should return total == 3 for 3-byte input");
    ASSERT(result_buf != nullptr, "load_file should allocate a buffer for 3-byte input");
    if (result_buf != nullptr) {
        ASSERT(std::memcmp(result_buf, input, 3) == 0, "buffer content should match input bytes (first 3 bytes)");
        std::free(result_buf);
    }

    // Cleanup
    std::remove(fname);
}

// Test 3: Load a larger file (e.g., 10 bytes) to ensure general correctness
// Purpose: Validate that the function can handle larger payloads and returns
//          the correct length with accurate data copy.
void test_load_file_large_payload() {
    const char* fname = "test_input_large.bin";
    // Prepare test payload: 10 bytes
    unsigned char input[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // Write test file
    {
        std::ofstream f(fname, std::ios::binary);
        f.write(reinterpret_cast<const char*>(input), 10);
        f.close();
    }

    // Call the focal function
    unsigned char* result_buf = nullptr;
    size_t total = load_file(fname, &result_buf);

    // Assertions
    ASSERT(total == 10, "load_file should return total == 10 for 10-byte input");
    ASSERT(result_buf != nullptr, "load_file should allocate a buffer for 10-byte input");
    if (result_buf != nullptr) {
        ASSERT(std::memcmp(result_buf, input, 10) == 0, "buffer content should match input bytes (first 10 bytes)");
        std::free(result_buf);
    }

    // Cleanup
    std::remove(fname);
}


// Entry point: run all tests and report summary
int main() {
    std::cout << "Starting load_file unit tests (C++11, no GTest)...\n";

    test_load_file_small_exact_multiple_of_4();
    test_load_file_small_not_multiple_of_4();
    test_load_file_large_payload();

    int total_tests = tests_passed + tests_failed;
    std::cout << "Tests completed. Passed: " << tests_passed
              << ", Failed: " << tests_failed
              << ", Total: " << total_tests << "\n";

    // Non-terminating tests approach: do not exit early on first failure.
    // Return non-zero if any test failed.
    return (tests_failed == 0) ? 0 : 1;
}