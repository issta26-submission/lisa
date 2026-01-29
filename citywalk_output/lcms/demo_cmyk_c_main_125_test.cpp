/********** Unit Test Suite for demo_cmyk.c main (C++11) **********/

/*
Step 1 (Program Understanding -- Candidate Keywords)
- cmsFloat64Number: floating-point type used for megapixels/sec metrics.
- speed_test(): core timing routine that returns megapixels/sec estimate.
- cmsPlugin(void*): API to install plugins (used to enable the fast float plugin).
- cmsFastFloatExtensions(): plugin descriptor/factory for the fast float plugin.
- main/demo flow: prints diagnostic messages and metrics (used to validate plugin behavior).
- stdout prints: header text and per-step messages (we focus on functional outputs and numerical invariants).
These keywords represent the core dependent components necessary to validate the focal main() behavior without modifying the production code.
*/

/*
Note on test strategy (non-GTest, self-contained):
- We implement a lightweight, non-terminating assertion framework suitable for a C++11 build with no external test framework.
- We exercise the same sequence as the focal main() (cache warmup, without_plugin speed, enable plugin, with_plugin speed, ratio).
- We validate basic invariants (positive, finite values) rather than exact numeric values to accommodate environmental variability.
- Tests are invoked from main(), and failures are reported without aborting the test run.
- We avoid touching private/static internals; we interact with the public C API (speed_test, cmsPlugin, cmsFastFloatExtensions).
*/

#include <lcms2_fast_float.h>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <cmath>


// Declaration of the C API from the focal project (best-effort compatibility for linking in C++).
extern "C" {
    // The focal code uses a floating type alias; we map to double for test purposes.
    typedef double cmsFloat64Number;

    // Plugin system entry points (assumed compatible with the real library API)
    // cmsFastFloatExtensions returns a pointer to the plugin descriptor (opaque to us here).
    const void* cmsFastFloatExtensions(void);

    // Register the provided plugin with LittleCMS
    void cmsPlugin(const void*);

    // The performance measurement function used in main()
    cmsFloat64Number speed_test(void);
}

// Lightweight, non-terminating assertion helpers
static int g_test_failures = 0;

#define T_ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "[TEST FAILURE] " << (msg) \
                      << " (Location: " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
            ++g_test_failures; \
        } \
    } while (0)

#define T_REQUIRE(cond) \
    T_ASSERT((cond), "Requirement failed: " #cond)


// Test 1: Validate the basic demonstration flow (cache warmup, without_plugin, plugin, with_plugin, ratio)
static void test_demo_flow_basic() {
    // Warm-up (simulate CPU cache stabilization)
    speed_test();

    // Without plugin
    cmsFloat64Number without_plugin = speed_test();

    // Enable the fast float plugin
    cmsPlugin(cmsFastFloatExtensions());

    // With plugin
    cmsFloat64Number with_plugin = speed_test();

    // Derived ratio
    cmsFloat64Number ratio = 0.0;
    if (without_plugin != 0.0) {
        ratio = with_plugin / without_plugin;
    }

    // Assertions: basic invariants and sane ratio
    T_REQUIRE(without_plugin > 0.0);
    T_REQUIRE(with_plugin > 0.0);
    T_REQUIRE(std::isfinite(without_plugin));
    T_REQUIRE(std::isfinite(with_plugin));
    T_REQUIRE(std::isfinite((double)ratio));
    // Allow for environmental variation: ratio should be a positive finite number
    T_REQUIRE(ratio > 0.0);
    // Optional sanity cap to avoid pathological environments
    T_REQUIRE(ratio < 1e6);
}


// Test 2: Stress-test repeated calls to speed_test to ensure stability (no crashes, monotonic-ish behavior not required)
static void test_speed_test_stability() {
    // Perform several successive measurements; ensure each is valid
    for (int i = 0; i < 5; ++i) {
        cmsFloat64Number v = speed_test();
        T_REQUIRE(v >= 0.0);
        T_REQUIRE(std::isfinite((double)v));
    }
}


// Test 3: Verify that plugin registration path does not crash when called repeatedly
static void test_plugin_registration_idempotent() {
    // Repeatedly register the plugin a few times; in well-behaved libraries this should be safe
    // Even if internal state ignores duplicates, the call should not crash.
    cmsPlugin(cmsFastFloatExtensions());
    cmsPlugin(cmsFastFloatExtensions());
    cmsPlugin(cmsFastFloatExtensions());
    // If we reach here without a crash, we consider it non-failing for this lightweight test
}


// Run all tests and report summary
static void run_all_tests() {
    test_demo_flow_basic();
    test_speed_test_stability();
    test_plugin_registration_idempotent();

    if (g_test_failures == 0) {
        std::cout << "[TEST SUMMARY] All tests PASSED." << std::endl;
    } else {
        std::cout << "[TEST SUMMARY] " << g_test_failures << " test(s) FAILED." << std::endl;
    }
}


// Entry point for the test executable
int main(void) {
    // Run the suite
    run_all_tests();

    // Return non-zero if any test failed
    return (g_test_failures == 0) ? 0 : 1;
}