/*
Step 1: Program Understanding and Candidate Keywords
- Focal method: adler32_combine64
- Dependencies visible in the focal class section:
  - adler32_combine_(uLong, uLong, z_off64_t)  // internal/local helper
  - adler32_z, adler32, adler32_combine (external wrappers)
  - zutil.h, z_off64_t, uLong, Bytef, z_size_t
- Candidate Keywords representing core components to consider in tests:
  adler32_combine64, adler32_combine_, adler32, adler32_z, adler32_combine, z_off64_t, uLong, Bytef
- The focal method is a thin wrapper returning the value of adler32_combine_(adler1, adler2, len2).
- Since adler32_combine_ is internal, tests should focus on end-to-end behavior of adler32_combine64 as it would affect concatenation results, using a known-good adler32 implementation for cross-checks.
- We will implement a small, self-contained Adler-32 computation in the test code to verify that adler32_combine64 produces the same result as concatenating the inputs and computing the Adler-32 checksum directly.

Step 2: Unit Test Generation
The test suite targets adler32_combine64 by validating:
- Basic concatenation: adler32 of s1+s2 equals adler32_combine64(adler32(s1), adler32(s2), len(s2)).
- Edge cases: empty second block, empty first block, longer strings.
- Domain constraints: correct types passed to the C function, cross-check with a local Adler-32 implementation.

Step 3: Test Case Refinement
- We implement a lightweight, self-contained Adler-32 function (mod 65521) compatible with zlib’s algorithm to derive expected results for concatenation.
- We create multiple test scenarios to improve coverage:
  1) Typical small strings
  2) Non-empty first, empty second
  3) Empty first, non-empty second
  4) Longer strings to exercise code paths
- We implement a simple test harness (no GTest) with non-terminating assertions (EXPECT_*) to maximize execution coverage in a single run.
- Tests assume linking against adler32.c (the focal 64-bit adler32_combine64 function) and verify results against the standalone Adler-32 reference in the test.

The test suite (C++11, no GTest) is provided below.

Note: The tests are designed to be linked with the C source adler32.c that provides adler32_combine64. The test harness assumes a POSIX-like environment and 64-bit capable build.

Code: (C++11 unit test suite for adler32_combine64) 
*/

#include <cstring>
#include <string>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>
#include <zutil.h>


// Forward declaration of the C function under test.
// We assume the function is provided by adler32.c and linked together.
// Use C linkage to avoid name mangling issues when linking with C code.
extern "C" {
    // Signature as per the focal method: uLong ZEXPORT adler32_combine64(uLong adler1, uLong adler2, z_off64_t len2);
    // We map z_off64_t to long long for the 64-bit offset type used in tests.
    unsigned long adler32_combine64(unsigned long adler1, unsigned long adler2, long long len2);
}

// Local self-contained Adler-32 implementation for test validation.
// This mirrors the standard Adler-32 algorithm with MOD = 65521.
static uint32_t adler32_reference(const uint8_t* data, size_t len) {
    const uint32_t MOD = 65521;
    uint32_t a = 1;
    uint32_t b = 0;
    for (size_t i = 0; i < len; ++i) {
        a += data[i];
        if (a >= MOD) a -= MOD;
        b += a;
        if (b >= MOD) b -= MOD;
    }
    return (b << 16) | a;
}

// Overload for std::string convenience
static uint32_t adler32_reference(const std::string& s) {
    return adler32_reference(reinterpret_cast<const uint8_t*>(s.data()), s.size());
}

// Global test failure collector (non-terminating assertions)
static std::vector<std::string> g_failures;

// Helpers to report failures without terminating tests
#define TEST_EXPECT_TRUE(cond, msg) do { if (!(cond)) { g_failures.push_back(std::string("EXPECT_TRUE failed: ") + (msg)); } } while(0)
#define TEST_EXPECT_EQ(actual, expected, msg) do { if ((actual) != (expected)) { g_failures.push_back(std::string("EXPECT_EQ failed: ") + (msg) + " | actual=" + std::to_string(static_cast<uint64_t>(actual)) + ", expected=" + std::to_string(static_cast<uint64_t>(expected)); } } while(0)
#define TEST_EXPECT_STR_EQ(actual, expected, msg) do { if ((actual) != (expected)) { g_failures.push_back(std::string("EXPECT_STR_EQ failed: ") + (msg) + " | actual=\"" + std::string(actual) + "\", expected=\"" + std::string(expected) + "\""); } } while(0)

// Small wrapper to run all tests and summarize
static void report_and_clear_failures() {
    if (g_failures.empty()) {
        std::cout << "[TEST RESULT] All tests passed.\n";
    } else {
        std::cout << "[TEST RESULT] Failures: " << g_failures.size() << "\n";
        for (size_t i = 0; i < g_failures.size(); ++i) {
            std::cout << "  " << i + 1 << ". " << g_failures[i] << "\n";
        }
        // Do not exit; non-terminating assertions allow test to continue
    }
    g_failures.clear();
}

// Utility: compute Adler-32 using the reference implementation
static uint32_t compute_adler32_ref(const std::string& s) {
    return adler32_reference(s);
}

// Test 1: Basic concatenation correctness
// s1 + s2 should produce the same Adler-32 as adler32_combine64(adler32(s1), adler32(s2), len(s2))
static void test_adler32_combine64_basic_concatenation() {
    // s1, s2 are non-empty
    const std::string s1 = "abc";
    const std::string s2 = "defgh";

    uint32_t adler1 = adler32_reference(s1);
    uint32_t adler2 = adler32_reference(s2);
    long long len2 = static_cast<long long>(s2.size());

    unsigned long combined = adler32_combine64(static_cast<unsigned long>(adler1),
                                               static_cast<unsigned long>(adler2),
                                               len2);

    std::string concatenated = s1 + s2;
    uint32_t expected = adler32_reference(concatenated);

    // Non-terminating assertion: record failure but continue
    TEST_EXPECT_EQ(static_cast<uint32_t>(combined), expected,
                   "adler32_combine64 should equal Adler-32 of concatenated data");
}

// Test 2: Non-empty first block, empty second block
// The result should equal Adler-32 of the first block
static void test_adler32_combine64_empty_second() {
    const std::string s1 = "hello world";
    const std::string s2 = "";

    uint32_t adler1 = adler32_reference(s1);
    uint32_t adler2 = adler32_reference(s2); // should be 1
    long long len2 = static_cast<long long>(s2.size());

    unsigned long combined = adler32_combine64(static_cast<unsigned long>(adler1),
                                               static_cast<unsigned long>(adler2),
                                               len2);

    uint32_t expected = adler32_reference(s1); // s1 alone
    TEST_EXPECT_EQ(static_cast<uint32_t>(combined), expected,
                   "When second block is empty, result should equal Adler-32 of first block");
}

// Test 3: Empty first block, non-empty second block
// The result should equal Adler-32 of the second block
static void test_adler32_combine64_empty_first() {
    const std::string s1 = "";
    const std::string s2 = "abcdef";

    uint32_t adler1 = adler32_reference(s1); // should be 1
    uint32_t adler2 = adler32_reference(s2);
    long long len2 = static_cast<long long>(s2.size());

    unsigned long combined = adler32_combine64(static_cast<unsigned long>(adler1),
                                               static_cast<unsigned long>(adler2),
                                               len2);

    uint32_t expected = adler32_reference(s2);
    TEST_EXPECT_EQ(static_cast<uint32_t>(combined), expected,
                   "When first block is empty, result should equal Adler-32 of second block");
}

// Test 4: Longer strings to exercise typical usage
static void test_adler32_combine64_long_strings() {
    const std::string s1 = "The quick brown fox jumps over the lazy dog";
    const std::string s2 = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    uint32_t adler1 = adler32_reference(s1);
    uint32_t adler2 = adler32_reference(s2);
    long long len2 = static_cast<long long>(s2.size());

    unsigned long combined = adler32_combine64(static_cast<unsigned long>(adler1),
                                               static_cast<unsigned long>(adler2),
                                               len2);

    std::string concatenated = s1 + s2;
    uint32_t expected = adler32_reference(concatenated);
    TEST_EXPECT_EQ(static_cast<uint32_t>(combined), expected,
                   "Long strings: combined Adler-32 should match Adler-32 of concatenation");
}

// Helper to run all tests
static void run_all_tests() {
    // Candidate Keywords (for traceability in test logs)
    // adler32_combine64, adler32_combine_, adler32, adler32_z, adler32_combine, z_off64_t, uLong
    test_adler32_combine64_basic_concatenation();
    test_adler32_combine64_empty_second();
    test_adler32_combine64_empty_first();
    test_adler32_combine64_long_strings();

    // Report results
    report_and_clear_failures();
}

// Main function: entry point for the test suite
int main() {
    // Run the test suite
    run_all_tests();

    return 0;
}

/*
Explanatory comments for each unit test:
- test_adler32_combine64_basic_concatenation
  Purpose: Validate that adler32_combine64 produces the same checksum as directly computing
  Adler-32 on the concatenation of s1 and s2. This checks the core correctness of the combination logic
  under normal conditions.

- test_adler32_combine64_empty_second
  Purpose: Ensure that if the second data block is empty (len2 == 0), adler32_combine64 returns
  the Adler-32 of the first block (i.e., combining with an empty second block should not modify the result).

- test_adler32_combine64_empty_first
  Purpose: Ensure that if the first data block is empty (adler1 corresponds to an empty input),
  adler32_combine64 returns the Adler-32 of the second block, validating the approach when starting
  with an initial adler corresponding to empty data.

- test_adler32_combine64_long_strings
  Purpose: Validate correctness with longer inputs to exercise typical usage and ensure that the
  combination logic scales to larger inputs. The expected value is computed by re-running Adler-32
  on the full concatenated data.

Notes:
- The tests rely on linking with the C implementation of adler32.c that provides adler32_combine64.
- All tests use non-terminating assertions (they accumulate failures) to maximize coverage in a single run.
- No GTest or mocking framework is used; a lightweight test harness is implemented directly in C++11.
- Static internal helper adler32_combine_ is not directly accessible from tests; verification is done
  against independent Adler-32 results computed in test code, confirming end-to-end behavior.
*/