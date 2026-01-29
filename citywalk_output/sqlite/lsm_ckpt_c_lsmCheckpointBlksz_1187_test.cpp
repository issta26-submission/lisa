/*
Unit test suite for the focal method:
  int lsmCheckpointBlksz(u32 *aCkpt)
- This method simply returns the value at aCkpt[CKPT_HDR_BLKSZ] cast to int.
- Candidate Keywords: CKPT_HDR_BLKSZ, aCkpt, lsmCheckpointBlksz, u32
- Domain notes:
  - Use only standard library and provided headers.
  - Do not rely on private/internal static methods.
  - No GTest; use a small custom test harness executed from main.
  - Tests are non-terminating on failure (log and continue) but the main returns non-zero on any failure.
*/

/* Include necessary headers and dependencies.
   1) Import dependencies (lsmInt.h provides types/macros like u32 and CKPT_HDR_BLKSZ).
   2) Declare the focal function with C linkage to ensure proper symbol resolution when linked with the C source.
*/
#include <lsmInt.h>
#include <iostream>
#include <cstdint>
#include <vector>


// Import project header(s) providing types like u32 and macro CKPT_HDR_BLKSZ.
// The following include assumes the project exposes the necessary declarations in lsmInt.h.

#ifdef __cplusplus
extern "C" {
#endif
// If the header already provides a prototype, this is a harmless re-declaration in C linkage.
int lsmCheckpointBlksz(u32 *aCkpt);
#ifdef __cplusplus
}
#endif

// Lightweight assertion helpers (non-terminating).
#define ASSERT_EQ(expected, actual) do { \
    if ((expected) != (actual)) { \
        std::cerr << "[ASSERT_FAIL] " << __FILE__ << ":" << __LINE__ \
                  << " - Expected: " << (expected) << ", Actual: " << (actual) << std::endl; \
        return false; \
    } \
} while(0)

#define ASSERT_TRUE(condition) do { \
    if (!(condition)) { \
        std::cerr << "[ASSERT_FAIL] " << __FILE__ << ":" << __LINE__ \
                  << " - Condition is false." << std::endl; \
        return false; \
    } \
} while(0)

/*
Test Case 1: Basic correctness
- Prepare an aCkpt array large enough to hold CKPT_HDR_BLKSZ index.
- Set aCkpt[CKPT_HDR_BLKSZ] to a known value (e.g., 7).
- Verify lsmCheckpointBlksz returns exactly that value as int.
*/
static bool test_lsmCheckpointBlksz_basic() {
    // Determine required buffer size: CKPT_HDR_BLKSZ + 1 elements.
    size_t n = static_cast<size_t>(CKPT_HDR_BLKSZ) + 1;
    std::vector<u32> aCkpt(n, 0); // initialize all to 0

    // Set a known value at the checkpoint block size index.
    aCkpt[CKPT_HDR_BLKSZ] = 7;

    // Call the focal method.
    int ret = lsmCheckpointBlksz(aCkpt.data());

    // Assert the returned value matches the expected 7.
    ASSERT_EQ(7, ret);
    std::cout << "[PASS] test_lsmCheckpointBlksz_basic" << std::endl;
    return true;
}

/*
Test Case 2: Casting edge case
- When the value at CKPT_HDR_BLKSZ is large (e.g., 0xFFFFFFFF), casting to int may yield a negative value.
- Verify that behavior matches expected C/C++ casting outcome (-1 in 32-bit int scenarios).
*/
static bool test_lsmCheckpointBlksz_castNegative() {
    size_t n = static_cast<size_t>(CKPT_HDR_BLKSZ) + 1;
    std::vector<u32> aCkpt(n, 0);

    // Set to the maximum unsigned 32-bit value to induce a negative int when cast.
    aCkpt[CKPT_HDR_BLKSZ] = 0xFFFFFFFFu;

    int ret = lsmCheckpointBlksz(aCkpt.data());

    // Depending on platform, int is typically 32-bit; 0xFFFFFFFF cast to int yields -1.
    ASSERT_EQ(-1, ret);
    std::cout << "[PASS] test_lsmCheckpointBlksz_castNegative" << std::endl;
    return true;
}

/*
Test Case 3: Zero value at CKPT_HDR_BLKSZ
- Ensure that a zero value is correctly returned as 0.
*/
static bool test_lsmCheckpointBlksz_zeroValue() {
    size_t n = static_cast<size_t>(CKPT_HDR_BLKSZ) + 1;
    std::vector<u32> aCkpt(n, 0);

    int ret = lsmCheckpointBlksz(aCkpt.data());
    ASSERT_EQ(0, ret);
    std::cout << "[PASS] test_lsmCheckpointBlksz_zeroValue" << std::endl;
    return true;
}

/*
Test Case 4: Random non-target indices do not affect result
- Populate indices other than CKPT_HDR_BLKSZ and ensure no side effects occur for the target.
*/
static bool test_lsmCheckpointBlksz_otherIndicesDoNotAffect() {
    size_t n = static_cast<size_t>(CKPT_HDR_BLKSZ) + 5; // extra space beyond required
    std::vector<u32> aCkpt(n, 0);

    // Set various other indices; do not alter CKPT_HDR_BLKSZ.
    for (size_t i = 0; i < n; ++i) {
        if (i == CKPT_HDR_BLKSZ) continue;
        aCkpt[i] = static_cast<u32>(i + 1);
    }

    // Ensure the target index still reads as 0 (initialized to 0).
    int ret = lsmCheckpointBlksz(aCkpt.data());
    ASSERT_EQ(0, ret);
    std::cout << "[PASS] test_lsmCheckpointBlksz_otherIndicesDoNotAffect" << std::endl;
    return true;
}

/*
Main entry: run all tests and report aggregate results.
*/
int main() {
    int tests_total = 0;
    int tests_passed = 0;

    // List of test functions
    typedef bool (*TestFunc)();
    std::vector<TestFunc> tests = {
        test_lsmCheckpointBlksz_basic,
        test_lsmCheckpointBlksz_castNegative,
        test_lsmCheckpointBlksz_zeroValue,
        test_lsmCheckpointBlksz_otherIndicesDoNotAffect
    };

    for (auto f : tests) {
        ++tests_total;
        bool ok = f();
        if (ok) ++tests_passed;
        // Non-terminating: continue running remaining tests even if one fails.
    }

    std::cout << "Test results: " << tests_passed << "/" << tests_total << " tests passed." << std::endl;
    // Return non-zero if any test failed.
    return (tests_passed == tests_total) ? 0 : 1;
}