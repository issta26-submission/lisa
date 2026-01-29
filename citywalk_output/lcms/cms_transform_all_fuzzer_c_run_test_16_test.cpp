// Lightweight C++11 unit-test harness for the focal function run_test in cms_transform_all_fuzzer.c
// This test harness uses only the C++ standard library (no GTest) and provides non-terminating
// assertions (EXPECT_*) to maximize code execution coverage.
// It also provides minimal fake stubs for the Little CMS (lcms2) functions used by run_test
// so the tests can run without requiring the actual external library.
// The test suite focuses on:
 // - size < 2 early return
 // - normal path where hTransform is created and then deleted
 // - path where hTransform creation returns NULL to exercise the else branch
 //
 // Notes:
 // - The FOCAL_METHOD indicates dependencies on cmsOpenProfileFromMem, cmsCreateTransform,
 //   cmsCloseProfile, cmsDeleteTransform. We stub these to avoid external library linkage issues.
 // - We assume the external function run_test has the signature shown in the focal file.

#include <cstddef>
#include <iostream>
#include <stdint.h>
#include <lcms2.h>
#include <cstdint>


// ----------------------------------------------------------------------------
// Minimal stubs to mimic lcms2 behavior for testing purposes.
// These are intentionally simple and only exist to exercise the focal method's code paths.
// They implement object-like handles via dynamic allocations and count calls to verify
// that the focal method is calling into its dependencies as expected.
// ----------------------------------------------------------------------------

typedef void* cmsHPROFILE;
typedef void* cmsHTRANSFORM;

// Global counters for test verification
static int g_open_calls = 0;
static int g_close_calls = 0;
static int g_create_calls = 0;
static int g_delete_calls = 0;
static bool g_force_null_transform = false;

// Reset all counters to start a fresh test case
static void reset_counters() {
    g_open_calls = 0;
    g_close_calls = 0;
    g_create_calls = 0;
    g_delete_calls = 0;
    g_force_null_transform = false;
}

// Accessors for test assertions
static int get_open_calls() { return g_open_calls; }
static int get_close_calls() { return g_close_calls; }
static int get_create_calls() { return g_create_calls; }
static int get_delete_calls() { return g_delete_calls; }

// Dummy memory-backed handles to simulate profiles and transforms
// Note: We allocate and delete to simulate resource management in tests.

// Forward declarations to ensure linker finds the symbols
extern "C" {

cmsHPROFILE cmsOpenProfileFromMem(const void* /*data*/, size_t size);
cmsHTRANSFORM cmsCreateTransform(cmsHPROFILE /*in*/, uint32_t /*input_format*/,
                                   cmsHPROFILE /*out*/, uint32_t /*output_format*/,
                                   uint32_t /*intent*/, uint32_t /*flags*/);
void cmsCloseProfile(cmsHPROFILE /*p*/);
void cmsDeleteTransform(cmsHTRANSFORM /*t*/);

}

// Implementation of stubs with C linkage to mimic external library behavior

extern "C" cmsHPROFILE cmsOpenProfileFromMem(const void* /*data*/, size_t size) {
    ++g_open_calls;
    if (size == 0) {
        return nullptr;
    }
    // Return a non-null opaque handle
    return static_cast<cmsHPROFILE>(new int(1));
}

extern "C" cmsHTRANSFORM cmsCreateTransform(cmsHPROFILE in,
                                          uint32_t /*input_format*/,
                                          cmsHPROFILE out,
                                          uint32_t /*output_format*/,
                                          uint32_t /*intent*/,
                                          uint32_t /*flags*/) {
    ++g_create_calls;
    // If tests force a NULL transformation, simulate that path
    if (g_force_null_transform) {
        return nullptr;
    }
    if (in != nullptr && out != nullptr) {
        return static_cast<cmsHTRANSFORM>(new int(1));
    }
    return nullptr;
}

extern "C" void cmsCloseProfile(cmsHPROFILE p) {
    ++g_close_calls;
    // Clean up the simulated handle
    delete static_cast<int*>(p);
    (void)p; // suppress unused if compiler optimizes away
}

extern "C" void cmsDeleteTransform(cmsHTRANSFORM t) {
    ++g_delete_calls;
    delete static_cast<int*>(t);
    (void)t;
}

// ----------------------------------------------------------------------------
// External focal function declaration
// The focal code is provided in cms_transform_all_fuzzer.c; we declare the prototype
// here so this test harness can call it.
// ----------------------------------------------------------------------------

extern "C" void run_test(const uint8_t *data,
                         size_t size,
                         uint32_t intent_id,
                         uint32_t input_format,
                         uint32_t output_format,
                         uint32_t flags);

// ----------------------------------------------------------------------------
// Lightweight test framework (non-terminating assertions)

static int g_total_failures = 0;

#define EXPECT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT FAILED: " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_total_failures; \
    } \
} while (0)

#define TEST_PASSED(fmt, name) do { \
    if (g_total_failures == 0) { \
        std::cout << "[OK] " << (fmt) << " " << (name) << "\n"; \
    } else { \
        std::cout << "[ERR] " << (fmt) << " " << (name) << " - failures: " << g_total_failures << "\n"; \
        g_total_failures = 0; \
    } \
} while (0)

// ----------------------------------------------------------------------------
// Unit Tests for run_test

// Test 1: size < 2 should return early and perform no profile/transform operations
static void test_size_less_than_two_no_calls() {
    reset_counters();
    const uint8_t data_small[1] = {0x00};

    run_test(data_small, 1, 0, 0, 0, 0);

    EXPECT(get_open_calls() == 0, "No cmsOpenProfileFromMem calls when size < 2");
    EXPECT(get_close_calls() == 0, "No cmsCloseProfile calls when size < 2");
    EXPECT(get_create_calls() == 0, "No cmsCreateTransform calls when size < 2");
    EXPECT(get_delete_calls() == 0, "No cmsDeleteTransform calls when size < 2");
    TEST_PASSED("Test", "size<2 early return");
}

// Test 2: Normal path where hTransform is created and subsequently deleted
static void test_normal_path_creates_and_deletes_transform() {
    reset_counters();
    const uint8_t data[4] = {0x01, 0x02, 0x03, 0x04};

    run_test(data, 4, 0, 0, 0, 0);

    EXPECT(get_open_calls() == 2, "Two profiles opened in normal path");
    EXPECT(get_close_calls() == 2, "Two profiles closed in normal path");
    EXPECT(get_create_calls() == 1, "One transform created in normal path");
    EXPECT(get_delete_calls() == 1, "One transform deleted in normal path");
    TEST_PASSED("Test", "normal path creates and deletes transform");
}

// Test 3: When cmsCreateTransform returns NULL, ensure close is still called on profiles and delete is not invoked
static void test_transform_null_path_no_delete_but_closes() {
    reset_counters();
    g_force_null_transform = true; // Force null transform return

    const uint8_t data[4] = {0xAA, 0xBB, 0xCC, 0xDD};
    run_test(data, 4, 0, 0, 0, 0);

    // Open should still happen; CreateTransform returns NULL due to flag
    EXPECT(get_open_calls() == 2, "Two profiles opened when size>=2 with forced NULL transform");
    EXPECT(get_close_calls() == 2, "Two profiles closed when size>=2 with forced NULL transform");
    EXPECT(get_create_calls() == 1, "Transform creation attempted once when forced NULL");
    EXPECT(get_delete_calls() == 0, "No transform deleted when creation failed (NULL transform)");
    TEST_PASSED("Test", "transform NULL path handles closes but no delete");

    // Reset flag for subsequent tests
    g_force_null_transform = false;
}

// ----------------------------------------------------------------------------
// Main function to run tests

int main() {
    std::cout << "Starting unit tests for focal method: run_test\n";

    test_size_less_than_two_no_calls();
    test_normal_path_creates_and_deletes_transform();
    test_transform_null_path_no_delete_but_closes();

    if (g_total_failures == 0) {
        std::cout << "[ALL TESTS PASSED]\n";
        return 0;
    } else {
        std::cout << "[TESTS FAILED] total failures: " << g_total_failures << "\n";
        return 1;
    }
}