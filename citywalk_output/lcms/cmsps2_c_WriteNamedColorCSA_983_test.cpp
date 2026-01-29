/*******************************************************
 * Lightweight C++11 Test Suite for
 * WriteNamedColorCSA (cmsps2.c)
 *
 * Notes:
 * - This test targets the focal method WriteNamedColorCSA.
 * - We avoid GTest; a minimal, non-terminating style is used.
 * - The test exercises at least one code path: the early return
 *   when the transform (xform) cannot be created (e.g., hNamedColor is NULL).
 * - We rely on the actual Little CMS (lcms2) library types and API.
 * - The test is designed to compile and run with C++11 and without
 *   external mocking frameworks.
 *
 * How to run (example):
 *   g++ -std=c++11 -lcms2 -o test_write_named_color_csa test_write_named_color_csa.cpp
 *   ./test_write_named_color_csa
 *
 * This file is self-contained and provides explanatory comments for each unit test.
 *******************************************************/

#include <cstring>
#include <cstdio>
#include <functional>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include Little CMS headers (adjust include path if necessary)

// Ensure the focal function is visible to the test binary.
// The function is defined in cmsps2.c. We declare it here to allow linking.
extern "C" int WriteNamedColorCSA(cmsIOHANDLER* m, cmsHPROFILE hNamedColor, cmsUInt32Number Intent);

// Simple non-terminating test harness utilities
struct TestCase {
    std::string name;
    std::function<bool()> func;
    TestCase(const std::string& n, std::function<bool()> f) : name(n), func(f) {}
};

static bool report_result(const TestCase& tc, bool result) {
    std::cout << "[Test] " << tc.name << " -> " << (result ? "PASS" : "FAIL") << std::endl;
    return result;
}

// Helper to create a minimal CMS IO handler that writes into a memory buffer.
// This leverages the library's public function cmsOpenIOhandlerFromMem.
static cmsIOHANDLER* CreateMemoryIOHandler(cmsContext Context, unsigned char* buffer, size_t bufferLen) {
    // Use the library-provided helper to create an in-memory IO handler.
    // If the header/signature changes in the library, this call might need adjustment.
    return cmsOpenIOhandlerFromMem(Context, buffer, (cmsUInt32Number)bufferLen, "w");
}

// Helper to safely cleanup IO handler if created
static void CleanupIOHandler(cmsIOHANDLER* io) {
    if (io) cmsCloseIOhandler(io);
}

// Test 1: Ensure early exit when the transform cannot be created (xform == NULL)
// Rationale:
// - This exercises the code path that immediately returns 0 when cmsCreateTransform
//   fails due to an invalid input (e.g., NULL hNamedColor).
static bool test_WriteNamedColorCSA_Returns0_When_XformIsNull() {
    // Create a context for IO and color processing
    cmsContext Context = cmsCreateContext(NULL, NULL);
    if (!Context) {
        std::cerr << "[Test] Failed to create CMS context." << std::endl;
        return false;
    }

    // Prepare an in-memory IO handler to capture any potential output.
    unsigned char buffer[4096];
    cmsIOHANDLER* m = CreateMemoryIOHandler(Context, buffer, sizeof(buffer));
    if (m == nullptr) {
        std::cerr << "[Test] Failed to create in-memory IO handler." << std::endl;
        // Best-effort cleanup; context may not be created, but we can exit early.
        // If cmsCreateContext() succeeded, we should clean it up if possible.
        // The exact cleanup call varies by cms version; skip if uncertain.
        return false;
    }

    // Call the focal function with a NULL NamedColor profile to force xform == NULL path.
    int rc = WriteNamedColorCSA(m, nullptr, 0);

    // Cleanup
    CleanupIOHandler(m);
    // Do not rely on cmsDeleteContext; best-effort cleanup (OS will reclaim on program exit).

    // Non-terminating assertion: we record the result and return.
    const bool result = (rc == 0);
    return result;
}

// Main test runner
int main() {
    std::vector<TestCase> tests;

    // Explanatory comments accompany each test in code; here we add tests to the suite.
    tests.emplace_back("WriteNamedColorCSA_Returns0_When_XformIsNull",
                       test_WriteNamedColorCSA_Returns0_When_XformIsNull);

    int passed = 0;
    int total = (int)tests.size();

    // Run tests
    for (auto &t : tests) {
        bool ok = false;
        try {
            ok = t.func();
        } catch (...) {
            ok = false;
            std::cerr << "[Test] Exception in test: " << t.name << std::endl;
        }
        if (report_result(t, ok)) ++passed;
    }

    std::cout << "Test suite summary: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}