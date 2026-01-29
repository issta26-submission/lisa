// Unit test suite for FreeCargo (as defined in mkcmy.c)
// Notes:
// - This test harness assumes the environment provides the lcms.h header
//   and the FreeCargo, LPCARGO, WORD, LPVOID and related types as used by mkcmy.c.
// - To enable function interception (mocking cmsDeleteTransform and cmsCloseProfile),
//   we provide C linkage definitions in this test file. The test uses a lightweight
//   memory-layout-based cargo object (MockCargo) to simulate the real LPCARGO layout.
// - The tests are designed to avoid terminating on failures (non-terminating assertions),
//   and instead accumulate failures and report a summary from main.
// - The test code is self-contained and intended to be compiled together with mkcmy.c.
//   Examples of how to build (assuming a typical environment):
//       g++ -std=c++11 test_mkcmy.cpp mkcmy.c -o test_mkcmy
//   or with appropriate include paths to lcms.h if needed.

#include <vector>
#include <iostream>
#include <cstdint>
#include <lcms.h>


// -----------------------------------------------------------------------------
// Lightweight test scaffolding: non-terminating assertions
// -----------------------------------------------------------------------------
static int g_failures = 0;

#define TEST_CHECK(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "Test failure: " << (msg) << " [Line " << __LINE__ << "]" << std::endl; \
        ++g_failures; \
    } \
} while (0)

static void test_report_summary(const std::string& test_name, int failures_before) {
    // No-op; summary is printed by main after running all tests.
    (void)test_name;
    (void)failures_before;
}

// -----------------------------------------------------------------------------
// Mocking and test scaffolding for FreeCargo
// -----------------------------------------------------------------------------
// We rely on the actual cargo layout (LPCARGO) defined by lcms.h. To enable
// observing what FreeCargo calls, we provide a mock cargo layout that matches
// the expected memory layout: Lab2RGB, RGB2Lab, hLab, hRGB in that order.
// This is used only within tests; the real FreeCargo will interpret the fields
// according to the real LPCARGO definition from lcms.h.
//
// The following mocks assume:
struct MockCargo {
    void* Lab2RGB;
    void* RGB2Lab;
    void* hLab;
    void* hRGB;
};

// Expose a compatible LPCARGO type by reinterpret_cast in tests.
// We do not rely on knowledge of the actual internal struct name; we cast
// the address of MockCargo to LPCARGO when calling FreeCargo.
typedef MockCargo* LPCARGO;

// We will intercept CMS library calls by providing our own definitions.
// They are declared with C linkage to match the C linkage expected by mkcmy.c.
extern "C" {
    // Vectors to record the sequence of calls and arguments
    std::vector<void*> g_delete_calls;
    std::vector<void*> g_close_calls;

    void cmsDeleteTransform(void* tran) {
        g_delete_calls.push_back(tran);
    }

    void cmsCloseProfile(void* prof) {
        g_close_calls.push_back(prof);
    }

    // Expose FreeCargo prototype so we can call it from C++ suite.
    // The actual FreeCargo is defined in mkcmy.c with C linkage.
    void FreeCargo(LPCARGO Cargo);
}

// Helper to reset mocks before each test
static void reset_mocks() {
    g_delete_calls.clear();
    g_close_calls.clear();
}

// Helper to print a compact summary for a single test
static void log_test_header(const std::string& name) {
    std::cout << "Running test: " << name << std::endl;
}

// -----------------------------------------------------------------------------
// Step 2: Unit Test Generation (FreeCargo)
// -----------------------------------------------------------------------------

// Test 1: Normal case - all cargo fields are non-null.
// We expect FreeCargo to call cmsDeleteTransform for Lab2RGB and RGB2Lab,
// and cmsCloseProfile for hLab and hRGB, exactly once each.
void test_FreeCargo_Normal() {
    log_test_header("FreeCargo_Normal (non-null fields)");
    reset_mocks();

    // Prepare a MockCargo instance with distinct sentinel pointers
    MockCargo m;
    m.Lab2RGB = reinterpret_cast<void*>(0x1001);
    m.RGB2Lab = reinterpret_cast<void*>(0x1002);
    m.hLab   = reinterpret_cast<void*>(0x2001);
    m.hRGB   = reinterpret_cast<void*>(0x2002);

    LPCARGO cargo = &m;

    // Invoke the focal method
    FreeCargo(cargo);

    // Validate that both transforms were deleted
    TEST_CHECK(g_delete_calls.size() == 2, "cmsDeleteTransform should be called twice");
    // Check that the two expected transforms were passed (order not guaranteed)
    bool hasLab2RGB = false;
    bool hasRGB2Lab = false;
    for (void* p : g_delete_calls) {
        if (p == m.Lab2RGB) hasLab2RGB = true;
        if (p == m.RGB2Lab) hasRGB2Lab = true;
    }
    TEST_CHECK(hasLab2RGB, "cmsDeleteTransform should be called with Lab2RGB");
    TEST_CHECK(hasRGB2Lab, "cmsDeleteTransform should be called with RGB2Lab");

    // Validate that both profiles were closed
    TEST_CHECK(g_close_calls.size() == 2, "cmsCloseProfile should be called twice");
    bool hasHLab = false;
    bool hasHRGB = false;
    for (void* p : g_close_calls) {
        if (p == m.hLab) hasHLab = true;
        if (p == m.hRGB) hasHRGB = true;
    }
    TEST_CHECK(hasHLab, "cmsCloseProfile should be called with hLab");
    TEST_CHECK(hasHRGB, "cmsCloseProfile should be called with hRGB");

    if (g_failures) {
        std::cout << "Test FreeCargo_Normal: FAILED with " << g_failures << " issue(s)." << std::endl;
    } else {
        std::cout << "Test FreeCargo_Normal: PASSED" << std::endl;
    }
}

// Test 2: Null fields - cargo has all fields set to NULL.
// We expect cmsDeleteTransform to be called with NULL for each field and cmsCloseProfile with NULL as well.
void test_FreeCargo_NullFields() {
    log_test_header("FreeCargo_NullFields (null fields)");
    reset_mocks();

    MockCargo m;
    m.Lab2RGB = nullptr;
    m.RGB2Lab = nullptr;
    m.hLab   = nullptr;
    m.hRGB   = nullptr;

    LPCARGO cargo = &m;
    FreeCargo(cargo);

    // Expect four calls in total (two deletes and two closes), all with NULL
    TEST_CHECK(g_delete_calls.size() == 2, "cmsDeleteTransform should be called twice (NULLs)");
    TEST_CHECK(g_close_calls.size() == 2, "cmsCloseProfile should be called twice (NULLs)");
    for (void* p : g_delete_calls) {
        TEST_CHECK(p == nullptr, "cmsDeleteTransform should be called with NULL when Lab2RGB/RGB2Lab are NULL");
    }
    for (void* p : g_close_calls) {
        TEST_CHECK(p == nullptr, "cmsCloseProfile should be called with NULL when hLab/hRGB are NULL");
    }

    if (g_failures) {
        std::cout << "Test FreeCargo_NullFields: FAILED with " << g_failures << " issue(s)." << std::endl;
    } else {
        std::cout << "Test FreeCargo_NullFields: PASSED" << std::endl;
    }
}

// Step 3: Test Case Refinement
// - Additional refinements could include verifying that FreeCargo does not crash when
//   only one of Lab2RGB / RGB2Lab is NULL, or when hLab/hRGB are swapped. The current
//   tests cover the simplest non-null and all-null scenarios to maximize coverage
//   without introducing external dependencies.
// - As per domain knowledge, we focus on using the real API types from lcms.h, keeping
//   tests executable in a basic C++11 environment without GTest.

// -----------------------------------------------------------------------------
// Step 3: Test Runner
// -----------------------------------------------------------------------------

static void run_all_tests() {
    test_FreeCargo_Normal();
    test_FreeCargo_NullFields();
}

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
int main(void) {
    // Run the test suite
    run_all_tests();

    int total_tests = 2;
    int total_failures = g_failures;
    std::cout << "Test run complete. Total tests: " << total_tests
              << ", Failures: " << total_failures << std::endl;
    return total_failures == 0 ? 0 : 1;
}