// C++11 unit test suite for cJSON_CreateNumber (from cJSON.c)
// This test suite exercises normal, boundary, and overflow/underflow paths of
// the function under test, without using GTest. It uses a lightweight,
// self-contained test harness that logs pass/fail results and continues
// execution to improve coverage.

#include <cmath>
#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <climits>
#include <limits.h>


extern "C" {
}

// Lightweight helper: compare doubles with tolerance
static bool almost_equal(double a, double b, double tol = 1e-9) {
    return std::fabs(a - b) <= tol;
}

// Test 1: Normal number conversion
// - Verifies type, valuedouble, and valueint for a typical positive number.
static bool test_CreateNumber_Normal() {
    bool ok = true;
    const double input = 3.5;

    cJSON *item = cJSON_CreateNumber(input);
    if (!item) {
        std::cerr << "[test_CreateNumber_Normal] Failure: cJSON_CreateNumber returned NULL\n";
        return false;
    }

    // Check type
    if (item->type != cJSON_Number) {
        std::cerr << "[test_CreateNumber_Normal] Failure: type is not cJSON_Number\n";
        ok = false;
    }

    // Check valuedouble
    if (!almost_equal(item->valuedouble, input)) {
        std::cerr << "[test_CreateNumber_Normal] Failure: valuedouble mismatch. Expected " << input
                  << ", got " << item->valuedouble << "\n";
        ok = false;
    }

    // Check valueint (truncation toward zero for positive numbers)
    if (item->valueint != (int)input) {
        std::cerr << "[test_CreateNumber_Normal] Failure: valueint mismatch. Expected " << (int)input
                  << ", got " << item->valueint << "\n";
        ok = false;
    }

    cJSON_Delete(item);
    return ok;
}

// Test 2: Overflow path (num >= INT_MAX)
// - Verifies that valueint saturates to INT_MAX and valuedouble retains input.
static bool test_CreateNumber_Overflow() {
    bool ok = true;
    // Use a number clearly above INT_MAX
    double input = static_cast<double>(INT_MAX) + 1000.0;

    cJSON *item = cJSON_CreateNumber(input);
    if (!item) {
        std::cerr << "[test_CreateNumber_Overflow] Failure: cJSON_CreateNumber returned NULL\n";
        return false;
    }

    // valueint should saturate to INT_MAX
    if (item->valueint != INT_MAX) {
        std::cerr << "[test_CreateNumber_Overflow] Failure: valueint should be INT_MAX, got "
                  << item->valueint << "\n";
        ok = false;
    }

    // valuedouble should reflect the original input (not saturated)
    if (!almost_equal(item->valuedouble, input)) {
        std::cerr << "[test_CreateNumber_Overflow] Failure: valuedouble mismatch. Expected " << input
                  << ", got " << item->valuedouble << "\n";
        ok = false;
    }

    cJSON_Delete(item);
    return ok;
}

// Test 3: Underflow path (num <= INT_MIN)
// - Verifies that valueint saturates to INT_MIN.
static bool test_CreateNumber_Underflow() {
    bool ok = true;
    double input = static_cast<double>(INT_MIN) - 1000.0;

    cJSON *item = cJSON_CreateNumber(input);
    if (!item) {
        std::cerr << "[test_CreateNumber_Underflow] Failure: cJSON_CreateNumber returned NULL\n";
        return false;
    }

    // valueint should saturate to INT_MIN
    if (item->valueint != INT_MIN) {
        std::cerr << "[test_CreateNumber_Underflow] Failure: valueint should be INT_MIN, got "
                  << item->valueint << "\n";
        ok = false;
    }

    // valuedouble should reflect the original input
    if (!almost_equal(item->valuedouble, input)) {
        std::cerr << "[test_CreateNumber_Underflow] Failure: valuedouble mismatch. Expected " << input
                  << ", got " << item->valuedouble << "\n";
        ok = false;
    }

    cJSON_Delete(item);
    return ok;
}

// Test 4: Boundary cases for INT_MAX and INT_MIN
// - Verifies exact boundary handling (no overflow/underflow) when input equals bounds.
static bool test_CreateNumber_Bounds() {
    bool ok = true;

    // Boundary: INT_MAX
    double input_max = static_cast<double>(INT_MAX);
    cJSON *item_max = cJSON_CreateNumber(input_max);
    if (!item_max) {
        std::cerr << "[test_CreateNumber_Bounds] Failure: cJSON_CreateNumber(INT_MAX) returned NULL\n";
        ok = false;
    } else {
        if (item_max->valueint != INT_MAX) {
            std::cerr << "[test_CreateNumber_Bounds] Failure: INT_MAX valueint mismatch. Expected "
                      << INT_MAX << ", got " << item_max->valueint << "\n";
            ok = false;
        }
        if (!almost_equal(item_max->valuedouble, input_max)) {
            std::cerr << "[test_CreateNumber_Bounds] Failure: INT_MAX valuedouble mismatch. Expected "
                      << input_max << ", got " << item_max->valuedouble << "\n";
            ok = false;
        }
        cJSON_Delete(item_max);
    }

    // Boundary: INT_MIN
    double input_min = static_cast<double>(INT_MIN);
    cJSON *item_min = cJSON_CreateNumber(input_min);
    if (!item_min) {
        std::cerr << "[test_CreateNumber_Bounds] Failure: cJSON_CreateNumber(INT_MIN) returned NULL\n";
        ok = false;
    } else {
        if (item_min->valueint != INT_MIN) {
            std::cerr << "[test_CreateNumber_Bounds] Failure: INT_MIN valueint mismatch. Expected "
                      << INT_MIN << ", got " << item_min->valueint << "\n";
            ok = false;
        }
        if (!almost_equal(item_min->valuedouble, input_min)) {
            std::cerr << "[test_CreateNumber_Bounds] Failure: INT_MIN valuedouble mismatch. Expected "
                      << input_min << ", got " << item_min->valuedouble << "\n";
            ok = false;
        }
        cJSON_Delete(item_min);
    }

    return ok;
}

// Test 5: Negative fractional number
// - Verifies truncation toward zero for negative fractional inputs.
static bool test_CreateNumber_NegativeFractional() {
    bool ok = true;
    double input = -2.9; // negative fractional

    cJSON *item = cJSON_CreateNumber(input);
    if (!item) {
        std::cerr << "[test_CreateNumber_NegativeFractional] Failure: cJSON_CreateNumber returned NULL\n";
        return false;
    }

    // valueint should be truncated toward zero: -2
    if (item->valueint != -2) {
        std::cerr << "[test_CreateNumber_NegativeFractional] Failure: valueint mismatch. Expected -2, got "
                  << item->valueint << "\n";
        ok = false;
    }

    // valuedouble should equal input
    if (!almost_equal(item->valuedouble, input)) {
        std::cerr << "[test_CreateNumber_NegativeFractional] Failure: valuedouble mismatch. Expected "
                  << input << ", got " << item->valuedouble << "\n";
        ok = false;
    }

    cJSON_Delete(item);
    return ok;
}

int main() {
    bool overall_ok = true;

    printf("Running tests for cJSON_CreateNumber...\n");

    if (!test_CreateNumber_Normal()) {
        overall_ok = false;
        printf("[Result] Test 1: Normal number test: FAILED\n");
    } else {
        printf("[Result] Test 1: Normal number test: PASSED\n");
    }

    if (!test_CreateNumber_Overflow()) {
        overall_ok = false;
        printf("[Result] Test 2: Overflow path test: FAILED\n");
    } else {
        printf("[Result] Test 2: Overflow path test: PASSED\n");
    }

    if (!test_CreateNumber_Underflow()) {
        overall_ok = false;
        printf("[Result] Test 3: Underflow path test: FAILED\n");
    } else {
        printf("[Result] Test 3: Underflow path test: PASSED\n");
    }

    if (!test_CreateNumber_Bounds()) {
        overall_ok = false;
        printf("[Result] Test 4: Boundary handling test: FAILED\n");
    } else {
        printf("[Result] Test 4: Boundary handling test: PASSED\n");
    }

    if (!test_CreateNumber_NegativeFractional()) {
        overall_ok = false;
        printf("[Result] Test 5: Negative fractional test: FAILED\n");
    } else {
        printf("[Result] Test 5: Negative fractional test: PASSED\n");
    }

    if (overall_ok) {
        printf("All tests PASSED.\n");
        return 0;
    } else {
        printf("Some tests FAILED. See details above.\n");
        return 1;
    }
}