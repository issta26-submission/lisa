// Unit test suite for the focal method: standard_info(pp, pi)
// Based on the provided focal code snippet and its dependencies.
// The goal is to validate the behavior of standard_info by introducing
// a lightweight, self-contained test harness (no GTest; only C++11 standard lib).
// The tests mock the dependent function standard_info_imp to verify the call path
// and parameter propagation, without needing the full libpng library.

#include <cstdint>
#include <cassert>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// Domain-aligned lightweight type definitions to emulate the minimal API surface.
typedef void* png_structp;
typedef void* png_infop;

// Forward declaration of standard_display used by the focal method.
struct standard_display {};

// Lightweight implementation of a mock progressive PNG structure
// that carries a pointer to a standard_display-like object.
struct PngStructMock {
    void* progressive_ptr;
};

// Macro to mimic the original code's voidcast helper.
// In the actual source, voidcast(type, expr) casts the expression to the requested type.
#define voidcast(type, expr) ((type)(expr))

// Mocked API surface to support standard_info's implementation in tests.
// This provides a minimal version of png_get_progressive_ptr(pp).
static PngStructMock* g_mock_pp_container_ptr = nullptr;

static void* png_get_progressive_ptr(png_structp pp) {
    // The test harness expects pp to point to a PngStructMock object.
    return static_cast<PngStructMock*>(pp)->progressive_ptr;
}

// The focal function under test, reproduced here in a self-contained form.
// It mirrors the snippet:
// void standard_info(png_structp pp, png_infop pi) { ... standard_info_imp(dp, pp, pi, 1); }
static void standard_info_imp(standard_display* dp, png_structp pp, png_infop pi, int nImages); // forward

static void standard_info(png_structp pp, png_infop pi) {
    {
        standard_display *dp = voidcast(standard_display*,
            png_get_progressive_ptr(pp));
        /* Call with nImages==1 because the progressive reader can only produce one
         * image.
         */
        standard_info_imp(dp, pp, pi, 1 /*only one image*/);
    }
}

// Global state to capture calls to standard_info_imp for verification.
static int g_call_count = 0;
static standard_display* g_last_dp = nullptr;
static png_structp g_last_pp = nullptr;
static png_infop g_last_pi = nullptr;
static int g_last_nImages = -1;

// Mock implementation of standard_info_imp to observe the interaction.
// It records the arguments it received so tests can assert correctness.
static void standard_info_imp(standard_display* dp, png_structp pp, png_infop pi, int nImages) {
    g_last_dp = dp;
    g_last_pp = pp;
    g_last_pi = pi;
    g_last_nImages = nImages;
    ++g_call_count;
}

// --------- Unit Tests (self-contained, no external test framework) ---------

// Test 1: When progressive_ptr contains a valid standard_display object,
// standard_info should call standard_info_imp with nImages == 1 and pass correct pointers.
static bool test_standard_info_calls_imp_with_one_image() {
    // Reset mock state
    g_call_count = 0;
    g_last_dp = nullptr;
    g_last_pp = nullptr;
    g_last_pi = nullptr;
    g_last_nImages = -1;

    // Prepare mock objects
    standard_display* dp = new standard_display();
    PngStructMock s;
    s.progressive_ptr = dp;

    // The 'pp' parameter points to the mock struct
    png_structp pp = &s;
    png_infop pi = reinterpret_cast<png_infop>(0xABCD);

    // Execute the function under test
    standard_info(pp, pi);

    // Verify expectations
    bool ok = true;
    if (g_call_count != 1) {
        std::cerr << "FAIL: test_standard_info_calls_imp_with_one_image - "
                  << "expected call_count 1, got " << g_call_count << "\n";
        ok = false;
    }
    if (g_last_dp != dp) {
        std::cerr << "FAIL: test_standard_info_calls_imp_with_one_image - "
                  << "dp pointer mismatch\n";
        ok = false;
    }
    if (g_last_pp != pp) {
        std::cerr << "FAIL: test_standard_info_calls_imp_with_one_image - "
                  << "pp pointer mismatch\n";
        ok = false;
    }
    if (g_last_pi != pi) {
        std::cerr << "FAIL: test_standard_info_calls_imp_with_one_image - "
                  << "pi pointer mismatch\n";
        ok = false;
    }
    if (g_last_nImages != 1) {
        std::cerr << "FAIL: test_standard_info_calls_imp_with_one_image - "
                  << "nImages should be 1, got " << g_last_nImages << "\n";
        ok = false;
    }

    delete dp;
    return ok;
}

// Test 2: When progressive_ptr is null, ensure standard_info passes nullptr as dp.
// This verifies coverage of the false-branch path for the internal dp extraction.
static bool test_standard_info_calls_imp_with_null_ptr() {
    // Reset mock state
    g_call_count = 0;
    g_last_dp = reinterpret_cast<standard_display*>(0x0);
    g_last_pp = nullptr;
    g_last_pi = nullptr;
    g_last_nImages = -1;

    // Prepare mock objects
    PngStructMock s;
    s.progressive_ptr = nullptr;

    png_structp pp = &s;
    png_infop pi = reinterpret_cast<png_infop>(0x1234);

    // Execute the function under test
    standard_info(pp, pi);

    // Verify expectations
    bool ok = true;
    if (g_call_count != 1) {
        std::cerr << "FAIL: test_standard_info_calls_imp_with_null_ptr - "
                  << "expected call_count 1, got " << g_call_count << "\n";
        ok = false;
    }
    if (g_last_dp != nullptr) {
        std::cerr << "FAIL: test_standard_info_calls_imp_with_null_ptr - "
                  << "expected dp == nullptr\n";
        ok = false;
    }
    if (g_last_pp != pp) {
        std::cerr << "FAIL: test_standard_info_calls_imp_with_null_ptr - "
                  << "pp pointer mismatch\n";
        ok = false;
    }
    if (g_last_pi != pi) {
        std::cerr << "FAIL: test_standard_info_calls_imp_with_null_ptr - "
                  << "pi pointer mismatch\n";
        ok = false;
    }
    if (g_last_nImages != 1) {
        std::cerr << "FAIL: test_standard_info_calls_imp_with_null_ptr - "
                  << "nImages should be 1, got " << g_last_nImages << "\n";
        ok = false;
    }

    return ok;
}

// Test 3: Ensure multiple consecutive calls to standard_info each trigger a separate
// invocation of standard_info_imp with nImages == 1.
static bool test_standard_info_multiple_calls() {
    g_call_count = 0;
    g_last_dp = nullptr;
    g_last_pp = nullptr;
    g_last_pi = nullptr;
    g_last_nImages = -1;

    // Prepare two different progressive_ptr targets
    PngStructMock s1, s2;
    standard_display d1, d2;
    s1.progressive_ptr = &d1;
    s2.progressive_ptr = &d2;

    png_structp pp1 = &s1;
    png_structp pp2 = &s2;
    png_infop pi1 = reinterpret_cast<png_infop>(0x1111);
    png_infop pi2 = reinterpret_cast<png_infop>(0x2222);

    // First call
    standard_info(pp1, pi1);
    // Second call
    standard_info(pp2, pi2);

    // After two calls, there should be two invocations of standard_info_imp
    bool ok = true;
    if (g_call_count != 2) {
        std::cerr << "FAIL: test_standard_info_multiple_calls - "
                  << "expected call_count 2, got " << g_call_count << "\n";
        ok = false;
    }
    // Validate that the last call corresponded to the second set of arguments
    if (g_last_dp != &d2 || g_last_pp != pp2 || g_last_pi != pi2 || g_last_nImages != 1) {
        std::cerr << "FAIL: test_standard_info_multiple_calls - "
                  << "final call did not reflect the second invocation correctly\n";
        ok = false;
    }

    return ok;
}

// ---------------------------- Test Harness ----------------------------

int main() {
    std::cout << "Running unit tests for focal method: standard_info(pp, pi)\n";

    int total = 0;
    int passed = 0;

    if (test_standard_info_calls_imp_with_one_image()) {
        ++passed;
    } else {
        std::cerr << "Test failed: test_standard_info_calls_imp_with_one_image\n";
    }
    ++total;

    if (test_standard_info_calls_imp_with_null_ptr()) {
        ++passed;
    } else {
        std::cerr << "Test failed: test_standard_info_calls_imp_with_null_ptr\n";
    }
    ++total;

    if (test_standard_info_multiple_calls()) {
        ++passed;
    } else {
        std::cerr << "Test failed: test_standard_info_multiple_calls\n";
    }
    ++total;

    std::cout << "Test results: " << passed << " / " << total << " tests passed.\n";
    if (passed != total) {
        std::cerr << "Some tests failed. See above messages for details.\n";
        return 1;
    }
    return 0;
}