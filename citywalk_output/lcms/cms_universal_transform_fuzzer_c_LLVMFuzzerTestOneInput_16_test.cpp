/*
  Lightweight C++11 test suite for the focal function LLVMFuzzerTestOneInput
  (no GoogleTest; uses a small, non-terminating assertion/reporting style)
  - Step 1: Understand the function: it splits input into two halves, opens two icc profiles
    from memory, tries to create a color transform, then closes profiles and deletes the transform
    if created. It always returns 0.
  - Step 2: Provide tests that exercise the key branches:
      - size < 2 (early return)
      - size >= 2 (main path; may or may not yield a transform depending on data)
  - Step 3: Use domain-knowledge style test structure (clear, self-contained, and robust to
    environments lacking certain ICC data). We avoid terminating assertions to maximize code execution.
*/

#include <vector>
#include <cstddef>
#include <iostream>
#include <stdint.h>
#include <lcms2.h>
#include <cstdint>


// Forward declare the focal C function (C linkage)
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size);

// Simple non-terminating assertion macro (prints on failure but does not abort)
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "ASSERT FAILED: " << (msg) \
                  << " [File: " << __FILE__ << " Line: " << __LINE__ << "]\n"; \
    } \
} while(0)

/*
  Test 1: size = 0 should trigger early return path.
  Rationale: exercises the guard condition "if (size < 2) return 0;".
*/
static bool test_size_zero() {
    const size_t size = 0;
    const uint8_t *data = nullptr;
    int ret = LLVMFuzzerTestOneInput(data, size);
    bool ok = (ret == 0);
    TEST_ASSERT(ok, "Expected return 0 for size 0");
    if(!ok) {
        std::cerr << "Test test_size_zero failed: LLVMFuzzerTestOneInput returned " << ret << "\n";
    }
    return ok;
}

/*
  Test 2: size = 1 should trigger early return path as well.
  Rationale: second edge case for the guard.
*/
static bool test_size_one() {
    const uint8_t bytes[1] = {0xAA};
    const size_t size = 1;
    int ret = LLVMFuzzerTestOneInput(bytes, size);
    bool ok = (ret == 0);
    TEST_ASSERT(ok, "Expected return 0 for size 1");
    if(!ok) {
        std::cerr << "Test test_size_one failed: LLVMFuzzerTestOneInput returned " << ret << "\n";
    }
    return ok;
}

/*
  Test 3: size = 2 creates two halves of exactly one byte each.
  Rationale: exercises the main path setup; however, success depends on external library
  behavior (profiles/transforms). We simply verify no crash and return value 0.
*/
static bool test_size_two() {
    uint8_t data[2] = {0x00, 0x00};
    size_t size = 2;
    int ret = LLVMFuzzerTestOneInput(data, size);
    bool ok = (ret == 0);
    TEST_ASSERT(ok, "Expected return 0 for size 2");
    if(!ok) {
        std::cerr << "Test test_size_two failed: LLVMFuzzerTestOneInput returned " << ret << "\n";
    }
    return ok;
}

/*
  Test 4: size = 3 exercises odd-sized input: mid = 1, halves of sizes 1 and 2.
  Rationale: ensures function handles non-even splits and follows through to the same cleanup path.
*/
static bool test_size_three() {
    uint8_t data[3] = {0x11, 0x22, 0x33};
    size_t size = 3;
    int ret = LLVMFuzzerTestOneInput(data, size);
    bool ok = (ret == 0);
    TEST_ASSERT(ok, "Expected return 0 for size 3");
    if(!ok) {
        std::cerr << "Test test_size_three failed: LLVMFuzzerTestOneInput returned " << ret << "\n";
    }
    return ok;
}

/*
  Test 5: Large input to maximize code execution paths.
  Rationale: ensure function handles substantial data without crashing and returns 0.
*/
static bool test_large_input() {
    const size_t size = 256;
    std::vector<uint8_t> data(size, 0xAB); // fill with non-zero bytes
    int ret = LLVMFuzzerTestOneInput(data.data(), data.size());
    bool ok = (ret == 0);
    TEST_ASSERT(ok, "Expected return 0 for large input (size 256)");
    if(!ok) {
        std::cerr << "Test test_large_input failed: LLVMFuzzerTestOneInput returned " << ret << "\n";
    }
    return ok;
}

/*
  Main test runner:
  - Executes all tests
  - Reports a final summary
  - Uses non-terminating assertions so all tests run
*/
int main() {
    std::cout << "Starting unit tests for LLVMFuzzerTestOneInput...\n";

    int total = 5;
    int failures = 0;

    if(!test_size_zero()) ++failures;
    if(!test_size_one()) ++failures;
    if(!test_size_two()) ++failures;
    if(!test_size_three()) ++failures;
    if(!test_large_input()) ++failures;

    int passed = total - failures;
    std::cout << "Tests completed. Passed: " << passed << " / " << total
              << ". Failures: " << failures << "\n";

    return failures; // non-zero if any test failed
}