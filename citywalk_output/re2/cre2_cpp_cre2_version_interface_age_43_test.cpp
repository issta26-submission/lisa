// Test suite for cre2_version_interface_age
// This file provides a small, self-contained test harness (no GTest) for the focal method.
// It relies on the library's cre2.h header being available in the include path.

#include <cre2.h>
#include <re2/re2.h>
#include <cstdlib>
#include <type_traits>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Simple test harness state
static int g_failures = 0;

// Lightweight, non-terminating assertion macro
// Logs the failure and continues to run remaining tests.
#define TEST_ASSERT_EQ(a, b) do {                                  \
    auto _a = (a);                                                  \
    auto _b = (b);                                                  \
    if (!(_a == _b)) {                                             \
        std::cerr << "Assertion failed: " #a " == " #b                \
                  << " (actual: " << _a << ", expected: " << _b    \
                  << ") at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failures;                                               \
    }                                                               \
} while (0)

// Compile-time check: ensure the return type of cre2_version_interface_age is integral.
// This uses a static assertion based on the function's return type deduced via decltype.
static_assert(std::is_integral<decltype(cre2_version_interface_age())>::value,
              "Return type of cre2_version_interface_age must be an integral type");

/*
  Test 1: Return value matches the macro-defined constant cre2_VERSION_INTERFACE_AGE.
  Rationale: cre2_version_interface_age is expected to return the constant that
  defines the interface age, ensuring compatibility/versioning alignment.
*/
void test_returns_macro_value() {
    int actual = cre2_version_interface_age();
    int expected = cre2_VERSION_INTERFACE_AGE;

    // Non-terminating assertion to allow full test run even if a failure occurs.
    TEST_ASSERT_EQ(actual, expected);
}

/*
  Test 2: Multiple invocations yield a consistent value.
  Rationale: The function is a simple accessor to a macro constant; repeated calls
  should produce identical results.
*/
void test_consistency_of_calls() {
    int a = cre2_version_interface_age();
    int b = cre2_version_interface_age();

    TEST_ASSERT_EQ(a, b);
}

int main() {
    // Run tests
    test_returns_macro_value();
    test_consistency_of_calls();

    // Report result
    if (g_failures == 0) {
        std::cout << "All tests passed for cre2_version_interface_age." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed for cre2_version_interface_age." << std::endl;
        return 1;
    }
}