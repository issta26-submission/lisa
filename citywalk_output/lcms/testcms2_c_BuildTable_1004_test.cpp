/*
Step 0 - Overview:
This is a self-contained C++11 test suite for the focal method BuildTable found in testcms2.c.
The tests exercise the two branches of the Descending flag (ascending vs descending placement)
and validate the mathematical rounding behavior implemented by BuildTable.

Notes aligned with the provided instructions:
- No gtest; a lightweight in-file harness is used.
- The tests import only the minimal surface of types and the BuildTable symbol.
- We mock only what is necessary; static file-scope helpers in testcms2.c are not used directly.
- True/false branches are covered for both Descending = 0 and Descending = 1.
- Tests are non-terminating in the sense they log failures and continue to maximize coverage.
- The test suite can be compiled in C++11 with a C linkage for BuildTable.

Important assumptions:
- The focal BuildTable has the following C-like signature in the linked C file:
  void BuildTable(int n, unsigned short Tab[], int Descending);
  and is implemented with a behavior identical to the snippet provided.
- We declare BuildTable with C linkage to link correctly with the C object code.
- cmsInt32Number -> int, cmsUInt16Number -> unsigned short, cmsBool -> int (at call site).
*/

#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <cmath>
#include <cstdint>


// Step 1 (Implementation detail): declare the focal function with C linkage.
// The actual project may define cmsBool as int; using int for Descending compatibility.
extern "C" void BuildTable(int n, unsigned short Tab[], int Descending);

// Simple in-file test harness (non-terminating assertions)
static int g_failures = 0;

// Reports a mismatch but does not terminate the test execution
void log_fail(const std::string& msg) {
    std::cerr << "FAIL: " << msg << std::endl;
    ++g_failures;
}

// Asserts equality for unsigned short values
void expect_eq_u16(const std::string& label, unsigned short expected, unsigned short actual) {
    if (expected != actual) {
        log_fail(label + " - expected: " + std::to_string(expected) +
                 ", actual: " + std::to_string(actual));
    }
}

// Asserts general boolean condition
void expect_true(bool cond, const std::string& label) {
    if (!cond) {
        log_fail("CHECK_FAILED: " + label);
    }
}

// Helper to print a summary
void print_summary() {
    if (g_failures == 0) {
        std::cout << "ALL_TESTS_PASSED" << std::endl;
    } else {
        std::cout << "TOTAL_FAILURES: " << g_failures << std::endl;
    }
}

// Step 2 - Test Suite for BuildTable
// Test 1: Ascending order for a small n (n=5)
void test_BuildTable_Ascending5() {
    const int n = 5;
    unsigned short Tab[n];
    BuildTable(n, Tab, 0); // Descending = 0 -> ascending placement

    // Expected values for n=5, as computed:
    // i: 0 1 2 3 4
    // v: 0, 16383.75, 32767.5, 49151.25, 65535
    // rounded: 0, 16384, 32768, 49151, 65535
    unsigned short expected[n] = {0, 16384, 32768, 49151, 65535};

    for (int i = 0; i < n; ++i) {
        expect_eq_u16("Ascending5.Tab[" + std::to_string(i) + "]",
                      expected[i], Tab[i]);
    }
}

// Test 2: Descending order for a small n (n=5)
void test_BuildTable_Descending5() {
    const int n = 5;
    unsigned short Tab[n];
    BuildTable(n, Tab, 1); // Descending = 1 -> reversed placement

    // Expected values for n=5, Descending = true:
    // Ascending sequence: [0, 16384, 32768, 49151, 65535]
    // Reversed into Tab: [65535, 49151, 32768, 16384, 0]
    unsigned short expected[n] = {65535, 49151, 32768, 16384, 0};

    for (int i = 0; i < n; ++i) {
        expect_eq_u16("Descending5.Tab[" + std::to_string(i) + "]",
                      expected[i], Tab[i]);
    }
}

// Test 3: Small n with both branches to ensure exact edge behavior (n=2)
void test_BuildTable_EdgeN2() {
    const int n = 2;
    unsigned short TabAsc[n];
    BuildTable(n, TabAsc, 0);

    // For n=2, ascending: i=0 -> index 0 => 0
    // i=1 -> index 1 => 65535
    unsigned short expectedAsc[n] = {0, 65535};
    for (int i = 0; i < n; ++i) {
        expect_eq_u16("EdgeN2.Asc.Tab[" + std::to_string(i) + "]",
                      expectedAsc[i], TabAsc[i]);
    }

    unsigned short TabDesc[n];
    BuildTable(n, TabDesc, 1);
    // Descending: i=0 -> index 1 => 0; i=1 -> index 0 => 65535
    unsigned short expectedDesc[n] = {65535, 0};
    for (int i = 0; i < n; ++i) {
        expect_eq_u16("EdgeN2.Desc.Tab[" + std::to_string(i) + "]",
                      expectedDesc[i], TabDesc[i]);
    }
}

// Test 4: Larger n to verify monotonicity and boundary values (n=100)
void test_BuildTable_LargeN_MonotonicAscending() {
    const int n = 100;
    unsigned short Tab[n];
    BuildTable(n, Tab, 0); // Ascending

    // Boundary checks
    expect_eq_u16("LargeN.Asc.First", 0, Tab[0]);
    expect_eq_u16("LargeN.Asc.Last",  65535, Tab[n-1]);

    // Monotonic non-decreasing
    for (int i = 0; i < n-1; ++i) {
        if (Tab[i] > Tab[i+1]) {
            log_fail("LargeN.Asc.NotMonotonic at index " + std::to_string(i) +
                     " value " + std::to_string(Tab[i]) + " > " + std::to_string(Tab[i+1]));
            break;
        }
    }
}

// Test 5: Large n with Descending to ensure reverse monotonicity
void test_BuildTable_LargeN_MonotonicDescending() {
    const int n = 100;
    unsigned short Tab[n];
    BuildTable(n, Tab, 1); // Descending

    // Boundary checks
    expect_eq_u16("LargeN.Desc.First", 65535, Tab[0]);
    expect_eq_u16("LargeN.Desc.Last", 0, Tab[n-1]);

    // Monotonic non-increasing
    for (int i = 0; i < n-1; ++i) {
        if ( Tab[i] < Tab[i+1] ) {
            log_fail("LargeN.Desc.NotMonotonic at index " + std::to_string(i) +
                     " value " + std::to_string(Tab[i]) + " < " + std::to_string(Tab[i+1]));
            break;
        }
    }
}

// Step 3 - Test Runner
void run_all_tests() {
    test_BuildTable_Ascending5();
    test_BuildTable_Descending5();
    test_BuildTable_EdgeN2();
    test_BuildTable_LargeN_MonotonicAscending();
    test_BuildTable_LargeN_MonotonicDescending();
}

// Entry point
int main() {
    // Run the suite
    run_all_tests();

    // Print summary to stdout
    print_summary();

    // Return non-zero if any test failed
    return (g_failures == 0) ? 0 : 1;
}

/*
Step 4 - Notes for maintainers:
- The tests assume the focal BuildTable behavior uses integer Descending as a boolean flag.
- They rely on the exact rounding semantics via floor(v + 0.5).
- If the actual project defines cmsBool differently (e.g., as unsigned int), adjust the BuildTable prototype in extern "C" accordingly.
- The test suite is designed to be easily portable to environments without GoogleTest by using a simple harness and logging failures rather than aborting.
- To run in your build system, ensure test_buildtable.cpp is linked with testcms2.c (the source containing BuildTable) to exercise the real implementation.
*/