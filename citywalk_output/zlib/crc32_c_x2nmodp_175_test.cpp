// Unit tests for the focal method x2nmodp in crc32.c
// This test suite is designed for a C++11 environment without GoogleTest.
// It uses a tiny, self-contained testing approach with non-terminating assertions.
// The tests rely on the actual CRC32 C implementation in crc32.c.
// Explanatory comments are provided for each test case.
//
// Key elements (Candidate Keywords) derived from the focal method and dependencies:
// - x2nmodp: the function under test
// - x2n_table: lookup table indexed by k & 31
// - multmodp: helper function used inside x2nmodp
// - z_crc_t: 32/64-bit CRC type used for computations
// - z_off64_t: 64-bit offset type for n
// - Behavior: base case when n == 0, bit-iteration loop, and k wrapping via k & 31

#include <cmath>
#include <limits>
#include <stdatomic.h>
#include <string>
#include <cstdint>
#include <iostream>
#include <vector>
#include <zutil.h>
#include <crc32.h>
#include <iomanip>
#include <cstdlib>


// Include the C headers that declare z_crc_t, z_off64_t, and the function under test.
// We wrap C headers with extern "C" to ensure proper linkage from C++.
extern "C" {
}

// Declaration of the focal function (to ensure linkage in C++ test code)
extern "C" z_crc_t x2nmodp(z_off64_t n, unsigned k);

// Simple in-file test framework: non-terminating assertions
static int g_total_tests = 0;
static int g_failures = 0;

static void log_failure(const std::string& msg)
{
    std::cerr << "TEST_FAIL: " << msg << std::endl;
}

static void log_success(const std::string& msg)
{
    std::cout << "TEST_PASS: " << msg << std::endl;
}

// Compare two values of potentially different integral types
template <typename T, typename U>
static bool expect_eq(const T& a, const U& b, const std::string& label)
{
    ++g_total_tests;
    if (static_cast<uint64_t>(a) != static_cast<uint64_t>(b)) {
        ++g_failures;
        std::ostringstream oss;
        oss << label << " | expected: " << std::uppercase << std::hex
            << std::showbase << static_cast<uint64_t>(b)
            << ", actual: " << static_cast<uint64_t>(a);
        log_failure(oss.str());
        return false;
    }
    log_success(label);
    return true;
}

// Helper to print z_crc_t nicely (as unsigned 64-bit for portability)
static std::uint64_t to_u64(z_crc_t v)
{
    return static_cast<std::uint64_t>(v);
}

// Test 1: Base case: x2nmodp(0, k) should return (1 << 31) for any k
static void test_x2nmodp_zero_n_base_case()
{
    // (z_crc_t)1 << 31 is the initial p value in the function
    z_crc_t expected = ((z_crc_t)1) << 31;
    z_crc_t r1 = x2nmodp(0, 0);
    expect_eq(r1, expected, "x2nmodp(0,0) returns (1<<31)");
    z_crc_t r2 = x2nmodp(0, 1);
    expect_eq(r2, expected, "x2nmodp(0,1) returns (1<<31)");
    z_crc_t r3 = x2nmodp(0, 32);
    expect_eq(r3, expected, "x2nmodp(0,32) returns (1<<31)");
}

// Test 2: When n == 0, multiple k values should yield identical results
static void test_x2nmodp_zero_n_k_variants()
{
    z_crc_t expected = ((z_crc_t)1) << 31;
    std::vector<unsigned> ks = {0, 5, 31, 32, 64, 1000};
    for (unsigned k : ks) {
        z_crc_t r = x2nmodp(0, k);
        expect_eq(r, expected, "x2nmodp(0,k) consistency across k");
    }
}

// Test 3: For n with a single set bit (n == 1), the loop runs exactly once.
// k and k+32 should produce the same result due to table index wrap (k & 31).
static void test_x2nmodp_one_bit_n_wrap_behavior()
{
    // For n == 1, there is exactly one iteration where (n & 1) is true.
    // The update uses x2n_table[(k & 31)]. Therefore, k and k+32 should be equivalent.
    std::vector<unsigned> ks = {0, 1, 2, 5, 31, 32, 33, 64};
    for (unsigned k : ks)
    {
        z_crc_t r1 = x2nmodp(1, k);
        z_crc_t r2 = x2nmodp(1, k + 32);
        std::ostringstream label;
        label << "x2nmodp(1," << k << ") == x2nmodp(1," << k + 32 << ")";
        expect_eq(r1, r2, label.str());
    }
}

// Test 4: Non-zero n values should produce results that differ from the base (n==0)
// in at least one tested combination, ensuring the loop executes and alters p.
static void test_x2nmodp_nonzero_n_changes_from_base()
{
    const z_crc_t base = ((z_crc_t)1) << 31;
    bool any_diff = false;

    std::vector<std::pair<z_off64_t, unsigned>> cases = {
        {1, 0}, {2, 1}, {3, 0}, {5, 7}, {7, 3}
    };

    for (const auto& c : cases) {
        z_crc_t r = x2nmodp(c.first, c.second);
        if (r != base) {
            any_diff = true;
        }
        std::ostringstream label;
        label << "x2nmodp(n=" << c.first << ", k=" << c.second << ") != base";
        // Use explicit check: if it's equal to base, we'll log a warning but continue
        if (r == base) {
            log_failure(label.str() + " (unexpected: equals base)");
        } else {
            log_success(label.str());
        }
    }

    if (!any_diff) {
        log_failure("No non-base results differed from base across tested nonzero n values (unexpected).");
        ++g_failures;
    } else {
        log_success("At least one nonzero-n case produced a value different from base (expected).");
    }
}

// Main test runner
int main()
{
    // Header
    std::cout << "Starting tests for x2nmodp in crc32.c (C, tested via C++ harness)" << std::endl;

    // Run tests
    test_x2nmodp_zero_n_base_case();
    test_x2nmodp_zero_n_k_variants();
    test_x2nmodp_one_bit_n_wrap_behavior();
    test_x2nmodp_nonzero_n_changes_from_base();

    // Summary
    std::cout << std::endl;
    std::cout << "Test summary: " << g_total_tests << " tests -> "
              << g_failures << " failures." << std::endl;

    return g_failures > 0 ? 1 : 0;
}