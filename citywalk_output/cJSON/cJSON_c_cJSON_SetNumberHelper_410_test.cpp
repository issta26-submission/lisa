// Candidate Keywords extracted from the focal method and its dependencies:
// - cJSON_SetNumberHelper, cJSON, valueint, valuedouble
// - INT_MAX, INT_MIN, number, object, return value
// - truncation of double to int, boundary handling, conditional branches
// - dependencies: cJSON.h, cJSON public API, internal helpers are not directly used in tests

#include <cmath>
#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Simple non-terminating test harness (no GTest, no Google mocks)
static int g_pass = 0;
static int g_fail = 0;

static void log_pass(const char* testName) {
    ++g_pass;
    printf("PASS: %s\n", testName);
}
static void log_fail(const char* testName, const char* reason) {
    ++g_fail;
    fprintf(stderr, "FAIL: %s - %s\n", testName, reason);
}

// Helper for double comparisons
static bool nearly_equal(double a, double b, double eps = 1e-12) {
    return std::fabs(a - b) <= eps;
}

// Test 1: Basic positive non-integer input should set valueint to trunc(number) and valuedouble to number
// This validates the core conversion path when number is within int range.
static void test_SetNumberHelper_basic_positive_truncation() {
    const char* testName = "test_SetNumberHelper_basic_positive_truncation";
    cJSON object;
    object.valueint = 0;
    object.valuedouble = 0.0;

    double number = 5.5;
    double ret = cJSON_SetNumberHelper(&object, number);

    if (object.valueint != (int)number) {
        log_fail(testName, "valueint did not equal (int)number");
        // fall through to check other assertions
    } else if (!nearly_equal(object.valuedouble, number)) {
        log_fail(testName, "valuedouble did not equal input number");
    } else if (!nearly_equal(ret, number)) {
        log_fail(testName, "return value did not equal input number");
    } else {
        log_pass(testName);
        return;
    }
    // In case of failure, still report that the test completed (non-terminating)
    if (object.valueint != (int)number) {
        // extra diagnostic
        fprintf(stderr, "  Diagnostic: valueint=%d, expected=%d\n", object.valueint, (int)number);
    }
    if (!nearly_equal(object.valuedouble, number)) {
        fprintf(stderr, "  Diagnostic: valuedouble=%f, expected=%f\n", object.valuedouble, number);
    }
    if (!nearly_equal(ret, number)) {
        fprintf(stderr, "  Diagnostic: return=%f, expected=%f\n", ret, number);
    }
}

// Test 2: Negative non-integer input should truncate toward zero and set valuedouble accordingly
static void test_SetNumberHelper_negative_truncation() {
    const char* testName = "test_SetNumberHelper_negative_truncation";
    cJSON object;
    object.valueint = 0;
    object.valuedouble = 0.0;

    double number = -3.7; // (int)number should be -3
    double ret = cJSON_SetNumberHelper(&object, number);

    if (object.valueint != -3) {
        log_fail(testName, "valueint did not equal -3 for number -3.7");
    } else if (!nearly_equal(object.valuedouble, number)) {
        log_fail(testName, "valuedouble did not equal input number (-3.7)");
    } else if (!nearly_equal(ret, number)) {
        log_fail(testName, "return value did not equal input number (-3.7)");
    } else {
        log_pass(testName);
        return;
    }
    if (object.valueint != -3) {
        fprintf(stderr, "  Diagnostic: valueint=%d, expected=%d\n", object.valueint, -3);
    }
    if (!nearly_equal(object.valuedouble, number)) {
        fprintf(stderr, "  Diagnostic: valuedouble=%f, expected=%f\n", object.valuedouble, number);
    }
    if (!nearly_equal(ret, number)) {
        fprintf(stderr, "  Diagnostic: return=%f, expected=%f\n", ret, number);
    }
}

// Test 3: Exact upper boundary should clamp valueint to INT_MAX and set valuedouble to the input number
static void test_SetNumberHelper_upper_bound_exact() {
    const char* testName = "test_SetNumberHelper_upper_bound_exact";
    cJSON object;
    object.valueint = 0;
    object.valuedouble = 0.0;

    double number = (double)INT_MAX;
    double ret = cJSON_SetNumberHelper(&object, number);

    if (object.valueint != INT_MAX) {
        log_fail(testName, "valueint did not equal INT_MAX on upper boundary");
    } else if (!nearly_equal(object.valuedouble, number)) {
        log_fail(testName, "valuedouble did not equal input number INT_MAX");
    } else if (!nearly_equal(ret, number)) {
        log_fail(testName, "return value did not equal input number INT_MAX");
    } else {
        log_pass(testName);
        return;
    }
    if (object.valueint != INT_MAX) {
        fprintf(stderr, "  Diagnostic: valueint=%d, expected=%d\n", object.valueint, INT_MAX);
    }
    if (!nearly_equal(object.valuedouble, number)) {
        fprintf(stderr, "  Diagnostic: valuedouble=%f, expected=%f\n", object.valuedouble, number);
    }
    if (!nearly_equal(ret, number)) {
        fprintf(stderr, "  Diagnostic: return=%f, expected=%f\n", ret, number);
    }
}

// Test 4: Upper boundary just below INT_MAX should cast to INT_MAX-1 for valueint
static void test_SetNumberHelper_upper_bound_below_exact() {
    const char* testName = "test_SetNumberHelper_upper_bound_below_exact";
    cJSON object;
    object.valueint = 0;
    object.valuedouble = 0.0;

    double number = (double)INT_MAX - 0.4; // casting should yield INT_MAX - 1
    double ret = cJSON_SetNumberHelper(&object, number);

    if (object.valueint != (INT_MAX - 1)) {
        log_fail(testName, "valueint did not equal INT_MAX - 1 for number just below INT_MAX");
    } else if (!nearly_equal(object.valuedouble, number)) {
        log_fail(testName, "valuedouble did not equal input number just below INT_MAX");
    } else if (!nearly_equal(ret, number)) {
        log_fail(testName, "return value did not equal input number just below INT_MAX");
    } else {
        log_pass(testName);
        return;
    }
    if (object.valueint != (INT_MAX - 1)) {
        fprintf(stderr, "  Diagnostic: valueint=%d, expected=%d\n", object.valueint, INT_MAX - 1);
    }
    if (!nearly_equal(object.valuedouble, number)) {
        fprintf(stderr, "  Diagnostic: valuedouble=%f, expected=%f\n", object.valuedouble, number);
    }
    if (!nearly_equal(ret, number)) {
        fprintf(stderr, "  Diagnostic: return=%f, expected=%f\n", ret, number);
    }
}

// Test 5: Exact lower boundary should clamp valueint to INT_MIN
static void test_SetNumberHelper_lower_bound_exact() {
    const char* testName = "test_SetNumberHelper_lower_bound_exact";
    cJSON object;
    object.valueint = 0;
    object.valuedouble = 0.0;

    double number = (double)INT_MIN;
    double ret = cJSON_SetNumberHelper(&object, number);

    if (object.valueint != INT_MIN) {
        log_fail(testName, "valueint did not equal INT_MIN on lower boundary");
    } else if (!nearly_equal(object.valuedouble, number)) {
        log_fail(testName, "valuedouble did not equal input number INT_MIN");
    } else if (!nearly_equal(ret, number)) {
        log_fail(testName, "return value did not equal input number INT_MIN");
    } else {
        log_pass(testName);
        return;
    }
    if (object.valueint != INT_MIN) {
        fprintf(stderr, "  Diagnostic: valueint=%d, expected=%d\n", object.valueint, INT_MIN);
    }
    if (!nearly_equal(object.valuedouble, number)) {
        fprintf(stderr, "  Diagnostic: valuedouble=%f, expected=%f\n", object.valuedouble, number);
    }
    if (!nearly_equal(ret, number)) {
        fprintf(stderr, "  Diagnostic: return=%f, expected=%f\n", ret, number);
    }
}

// Test 6: Lower boundary just below INT_MIN should clamp to INT_MIN as well
static void test_SetNumberHelper_lower_bound_below_exact() {
    const char* testName = "test_SetNumberHelper_lower_bound_below_exact";
    cJSON object;
    object.valueint = 0;
    object.valuedouble = 0.0;

    double number = (double)INT_MIN - 1.0;
    double ret = cJSON_SetNumberHelper(&object, number);

    if (object.valueint != INT_MIN) {
        log_fail(testName, "valueint did not equal INT_MIN for number below INT_MIN");
    } else if (!nearly_equal(object.valuedouble, number)) {
        log_fail(testName, "valuedouble did not equal input number below INT_MIN");
    } else if (!nearly_equal(ret, number)) {
        log_fail(testName, "return value did not equal input number below INT_MIN");
    } else {
        log_pass(testName);
        return;
    }
    if (object.valueint != INT_MIN) {
        fprintf(stderr, "  Diagnostic: valueint=%d, expected=%d\n", object.valueint, INT_MIN);
    }
    if (!nearly_equal(object.valuedouble, number)) {
        fprintf(stderr, "  Diagnostic: valuedouble=%f, expected=%f\n", object.valuedouble, number);
    }
    if (!nearly_equal(ret, number)) {
        fprintf(stderr, "  Diagnostic: return=%f, expected=%f\n", ret, number);
    }
}

// Test 7: Zero should result in valueint 0 and valuedouble 0.0
static void test_SetNumberHelper_zero() {
    const char* testName = "test_SetNumberHelper_zero";
    cJSON object;
    object.valueint = 123;
    object.valuedouble = 1.23;

    double number = 0.0;
    double ret = cJSON_SetNumberHelper(&object, number);

    if (object.valueint != 0) {
        log_fail(testName, "valueint did not become 0 when number is 0.0");
    } else if (!nearly_equal(object.valuedouble, number)) {
        log_fail(testName, "valuedouble did not become 0.0 when number is 0.0");
    } else if (!nearly_equal(ret, number)) {
        log_fail(testName, "return value did not equal 0.0");
    } else {
        log_pass(testName);
        return;
    }
    if (object.valueint != 0) {
        fprintf(stderr, "  Diagnostic: valueint=%d, expected=%d\n", object.valueint, 0);
    }
    if (!nearly_equal(object.valuedouble, number)) {
        fprintf(stderr, "  Diagnostic: valuedouble=%f, expected=%f\n", object.valuedouble, number);
    }
    if (!nearly_equal(ret, number)) {
        fprintf(stderr, "  Diagnostic: return=%f, expected=%f\n", ret, number);
    }
}

// Main: run all tests
int main() {
    // Step 2: Unit Test Generation
    // Coverage includes true/false branches of the core comparisons in cJSON_SetNumberHelper:
    // - number >= INT_MAX
    // - number <= INT_MIN
    // - else path
    // Boundary, truncation, and return value of the function are verified.

    test_SetNumberHelper_basic_positive_truncation();
    test_SetNumberHelper_negative_truncation();
    test_SetNumberHelper_upper_bound_exact();
    test_SetNumberHelper_upper_bound_below_exact();
    test_SetNumberHelper_lower_bound_exact();
    test_SetNumberHelper_lower_bound_below_exact();
    test_SetNumberHelper_zero();

    // Summary
    printf("Summary: %d passed, %d failed\n", g_pass, g_fail);
    // Exit code 0 if all tests pass, otherwise non-zero to indicate failures
    return (g_fail == 0) ? 0 : 1;
}