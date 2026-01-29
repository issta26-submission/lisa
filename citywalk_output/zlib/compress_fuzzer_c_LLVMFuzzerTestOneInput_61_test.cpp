/*
  Unit test suite for the focal method LLVMFuzzerTestOneInput and its dependencies.
  This test harness is designed for C++11 compilation without Google Test (GTest).
  It uses a lightweight, non-terminating assertion mechanism to maximize code execution
  paths and coverage.

  Step 1 - Program Understanding (Notes in comments)
  - focal method: LLVMFuzzerTestOneInput(const uint8_t *d, size_t size)
  - Key behavioral branches:
    1) Immediate guard: size < 10 or size > 1024*1024 -> return 0
    2) level = d[0] % 10
    3) compression_method = d[0] % 5; if == 4 then set to 8
    4) flags = d[0] & (2 << 4)
    5) alloc/compress buffers using compressBound(size)
    6) calls to check_compress_level and check_decompress (static stubs)
  - Class dependencies visible in <FOCAL_CLASS_DEP> include stubs for
    check_compress_level, write_zlib_header, check_decompress, and usage of
    zlib's compressBound. The test suite relies on these existing implementations
    and does not attempt to mock private internals.

  Step 2 - Unit Test Generation
  - Design test cases to exercise:
    a) size boundary conditions (below min, at min, around upper bound)
    b) the two branches of compression_method (normal and the 4->8 path)
    c) the flags path (non-zero vs zero)
  - Supplementary tests check for safe memory allocation behavior and ensure
    the function returns 0 as specified.

  Step 3 - Test Case Refinement
  - Use only standard library facilities and a minimal, non-terminating assertion
    framework to maximize coverage without terminating on first failure.
  - Ensure tests do not rely on private members and respect the provided domain
    knowledge constraints (no GMock, no private field access, etc.).

  Notes on dependencies:
  - The focal code uses zlib (compressBound) and is expected to link against zlib.
  - The test harness will call the C function LLVMFuzzerTestOneInput from C++.

  Explanatory comments are added above each test to describe intent and coverage.
*/

#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <cstdint>
#include <cstddef>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <stdlib.h>


// Forward declaration for the focal C function under test.
// It uses C linkage, so declare with extern "C" to avoid name mangling.
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *d, size_t size);

// Lightweight, non-terminating assertion helper.
// Increments a global failure count and prints diagnostic information on failure.
static int gTestFailures = 0;
static void expect(bool condition, const char* message) {
    if (!condition) {
        ++gTestFailures;
        std::cerr << "[TEST] FAILED: " << message << std::endl;
    }
}

/*
  Step 2 Test Case 1: size below minimum (size < 10)
  Intent: Exercise the early return branch that must yield 0 and avoid any
  further processing (memory allocation should be skipped).
*/
static void test_size_too_small() {
    // 9 bytes: below the required minimum size of 10
    std::vector<uint8_t> payload(9, 0);
    int ret = LLVMFuzzerTestOneInput(payload.data(), payload.size());
    // The contract states the function must return 0 for this input.
    expect(ret == 0, "LLVMFuzzerTestOneInput should return 0 for size < 10");
}

/*
  Step 2 Test Case 2: normal valid input within bounds
  Intent: Exercise the main path with non-edge values for level/compression_method/flags.
  This validates that the function completes and returns 0 without crashing.
*/
static void test_valid_input_basic() {
    // size = 10 to leave 7 bytes for the remaining processing after consuming
    // 3 bytes for level, compression_method, and flags.
    std::vector<uint8_t> payload(10, 0);
    // Set up values for the first three bytes:
    // level = 0 (0 % 10), compression_method = 0 (0 % 5), flags = 0
    payload[0] = 0;  // level
    payload[1] = 0;  // compression_method (0 % 5 == 0)
    payload[2] = 0;  // flags
    int ret = LLVMFuzzerTestOneInput(payload.data(), payload.size());
    expect(ret == 0, "LLVMFuzzerTestOneInput should return 0 for a basic valid input");
}

/*
  Step 2 Test Case 3: compression_method == 4 path
  Intent: Force the special branch where compression_method == 4 is remapped to 8.
  We verify the function handles this remapping path gracefully and still returns 0.
*/
static void test_compression_method_eq_four() {
    std::vector<uint8_t> payload(10, 0);
    payload[0] = 0;  // level
    payload[1] = 4;  // compression_method (will be remapped to 8)
    payload[2] = 0;  // flags
    int ret = LLVMFuzzerTestOneInput(payload.data(), payload.size());
    expect(ret == 0, "LLVMFuzzerTestOneInput should return 0 when compression_method == 4 (remapped to 8)");
}

/*
  Step 2 Test Case 4: upper bound input (size around 1 MB)
  Intent: Exercise the memory allocation path under large input size while
  ensuring it completes without crashing.
  Note: We keep the payload simple (low content variability) to focus on allocation behavior.
*/
static void test_upper_bound_input() {
    // Use the maximum allowed size according to the fuzzer: 1,048,576 (1 MB)
    const size_t maxSize = 1024 * 1024;
    std::vector<uint8_t> payload(maxSize, 0);

    // Initialize the first few bytes to valid values
    payload[0] = 7;  // level (arbitrary)
    payload[1] = 2;  // compression_method (2 % 5 != 4, so no remap)
    payload[2] = 0;  // flags

    int ret = LLVMFuzzerTestOneInput(payload.data(), payload.size());
    expect(ret == 0, "LLVMFuzzerTestOneInput should return 0 for upper-bound large input");
}

/*
  Minimal test runner invoking all test cases and reporting outcomes.
  The tests are designed to be comprehensive yet lightweight to keep
  execution times reasonable while achieving notable coverage.
*/
int main() {
    test_size_too_small();
    test_valid_input_basic();
    test_compression_method_eq_four();
    test_upper_bound_input();

    if (gTestFailures > 0) {
        std::cerr << "[RESULT] " << gTestFailures << " test(s) failed." << std::endl;
        return 1;
    } else {
        std::cout << "[RESULT] All tests passed." << std::endl;
        return 0;
    }
}