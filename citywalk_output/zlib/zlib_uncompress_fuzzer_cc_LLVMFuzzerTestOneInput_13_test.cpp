/*
Step 1: Program Understanding
- Focal method under test: LLVMFuzzerTestOneInput
- Behavior: Attempts to uncompress data into a pre-existing global buffer using zlib's uncompress.
  - Creates a local buffer_length based on sizeof(buffer).
  - Calls uncompress(buffer, &buffer_length, data, size).
  - If uncompress does not return Z_OK, returns 0; otherwise returns 0 as well.
- Key dependent components:
  - Global destination buffer: buffer[]
  - zlib uncompress API: uncompress, Z_OK
  - Data input: const uint8_t* data, size_t size
- Implication for tests: We should exercise both paths of the conditional (Z_OK vs not Z_OK) by supplying valid and invalid compressed data. Since the function always returns 0, observable outcomes are limited; the goal is to exercise internal branching and the uncompress call itself.

Step 2: Unit Test Generation
- Candidate Keywords observed from the focal code:
  - buffer, buffer_length, uncompress, Z_OK, Z_BUF_ERROR, Z_DATA_ERROR, Z_STREAM_ERROR
  - data, size (input)
  - zlib.h integration
  - extern "C" linkage for LLVMFuzzerTestOneInput
- Intended tests to cover true/false branches:
  - Test with valid compressed data (uncompress returns Z_OK)
  - Test with invalid/random data (uncompress returns not Z_OK)
  - Test with edge case: empty input (size = 0)
- Dependencies for tests:
  - zlib
  - extern unsigned char buffer[] defined in the project (we declare extern to reuse the same buffer)
  - Use only standard library and provided interfaces
- Intention Contexts:
  - The test aims to validate the focal function's ability to call uncompress with varying inputs without crashing and to traverse both branches of the conditional.

Step 3: Test Case Refinement
- Use C++11 standard library only (no Google Test)
- Implement a lightweight test harness that continues execution after failures (non-terminating checks)
- Create three tests:
  - test_valid_input: compress a known string with zlib at run-time to obtain valid compressed data, feed to LLVMFuzzerTestOneInput
  - test_invalid_input: feed random bytes to trigger non-Z_OK from uncompress
  - test_empty_input: pass a non-null buffer with size 0 to exercise edge case
- All tests run from main; failures are recorded but do not terminate test execution
- Provide explanatory comments for each unit test

Code (self-contained test suite, designed to be compiled with the project containing zlib_uncompress_fuzzer.cc)
*/

#include <cstring>
#include <stddef.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <zlib.h>


// Focal function declaration (as it would be linked from the focal module)
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size);

// The focal code references a global destination buffer named 'buffer'.
// We declare it here to link against the existing definition in the focal TU.
extern unsigned char buffer[];

//
// Lightweight test harness (non-terminating assertions) 
// - Each test registers via logs but does not abort on failure.
// - Summary is printed at the end.
// - This harness is designed to maximize coverage by continuing after failures.
//
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_result(const char* name, bool pass, const std::string& note = "")
{
    ++g_total_tests;
    if (!pass) ++g_failed_tests;
    std::cout << (pass ? "[PASS] " : "[FAIL] ") << name;
    if (!note.empty()) std::cout << " - " << note;
    std::cout << std::endl;
}

// Test 1: Valid compressed input should trigger Z_OK path inside LLVMFuzzerTestOneInput
static void test_valid_input()
{
    // Prepare some known content to compress
    const std::string original = "The quick brown fox jumps over the lazy dog";
    uLongf orig_len = static_cast<uLongf>(original.size());

    // Obtain maximum compressed size and perform compression
    uLongf bound = compressBound(orig_len);
    std::vector<uint8_t> compressed(bound);
    int res = compress2(compressed.data(), &bound,
                        reinterpret_cast<const Bytef*>(original.data()),
                        orig_len, Z_BEST_COMPRESSION);
    if (res != Z_OK) {
        log_result("test_valid_input", false, "compression failed to prepare valid input");
        return;
    }

    // Call the focal function with valid compressed data
    LLVMFuzzerTestOneInput(compressed.data(), static_cast<size_t>(bound));

    // If we reach here, the function call completed without a crash (best effort check)
    log_result("test_valid_input", true);
}

// Test 2: Invalid/random data should exercise the non-Z_OK branch of the focal function
static void test_invalid_input()
{
    // Create deterministic random-like data which is unlikely to decompress
    const size_t N = 256;
    std::vector<uint8_t> data(N);
    uint32_t seed = 0xDEADBEEF;
    for (size_t i = 0; i < N; ++i) {
        seed = seed * 1664525u + 1013904223u;
        data[i] = static_cast<uint8_t>(seed);
    }

    // Feed invalid data into the fuzzer function
    LLVMFuzzerTestOneInput(data.data(), data.size());

    // No return value to assert; the objective is to cover path
    log_result("test_invalid_input", true);
}

// Test 3: Edge case with empty input (size = 0) to exercise boundary handling
static void test_empty_input()
{
    // Use a non-null pointer with size 0
    static uint8_t empty[1] = {0};
    LLVMFuzzerTestOneInput(empty, 0);

    log_result("test_empty_input", true);
}

// Entry point
int main()
{
    // Step 2/3: Run the tests to exercise the focal function and its dependencies
    test_valid_input();
    test_invalid_input();
    test_empty_input();

    // Summary
    std::cout << "Test summary: " 
              << (g_total_tests - g_failed_tests) << " passed, "
              << g_failed_tests << " failed, out of "
              << g_total_tests << " tests." << std::endl;

    return (g_failed_tests == 0) ? 0 : 1;
}