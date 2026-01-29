// Lightweight C++11 unit tests for a simplified, testable replica of the focal method
// main in wtpt.c. The goal is to verify core control flow and dependent interactions
// (InitUtils, HandleSwitches, Help, ShowWhitePoint, and CMS-API-like functions) in
// isolation without requiring the real LittleCMS library.
//
// Important notes:
// - This test suite replicates the essential logic of the focal main() in a
//   testable function: run_wtpt_main(argc, argv). It mocks the external
//   dependencies (InitUtils, HandleSwitches, Help, ShowWhitePoint, cmsOpenProfileFromFile,
//   cmsReadTag, cmsCloseProfile) using simple C linkage stubs.
// - The test harness uses a tiny, self-contained framework (no Google Test).
// - The tests cover true/false branches of the key predicates and some error paths.
// - All tests are contained in a single file for portability, as requested. You can
//   compile this file directly with a C++11 capable compiler.
//
// Build example (assuming you want to run this as a standalone executable):
// g++ -std=c++11 -O2 -Wall wtpt_test.cpp -o wtpt_test
//
// To run tests: ./wtpt_test

#include <cstring>
#include <cstdio>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <utils.h>


// Domain-specific types and constants (mocked to resemble CLS/CMS API shapes)
typedef void* cmsHPROFILE;

typedef struct {
    double x;
    double y;
    double z;
} cmsCIEXYZ;

// Mock CMS tag identifier (value is arbitrary for test purposes)
static const unsigned long cmsSigMediaWhitePointTag = 0x4D575458; // 'MWTX' - placeholder

// Global index used by the focal code to index the argv after InitUtils
int xoptind;

// Forward declarations for the focal method's dependencies (mocked)
extern "C" void InitUtils(const char* programName);
extern "C" void HandleSwitches(int argc, char* argv[]);
extern "C" void Help(void);
extern "C" void ShowWhitePoint(cmsCIEXYZ* WtPt);

// Mocked CMS API signatures
extern "C" cmsHPROFILE cmsOpenProfileFromFile(const char* name, const char* mode);
extern "C" cmsCIEXYZ* cmsReadTag(cmsHPROFILE hProfile, unsigned long tag);
extern "C" void cmsCloseProfile(cmsHPROFILE hProfile);

// Global mock state
static int mock_open_fail = 0;
static int mock_readtag_null = 0;
static cmsCIEXYZ mock_white_point = { 0.3127, 0.3290, 0.3583 }; // typical D65-ish values

// Test-tracking state
static int help_called = 0;
static int show_called = 0;
static double last_seen_wt_x = 0.0;
static double last_seen_wt_y = 0.0;
static double last_seen_wt_z = 0.0;

// Simple, tiny assertion helper (non-terminating)
static int g_test_failures = 0;
#define ASSERT_TRUE(cond, msg)                                         \
    do {                                                               \
        if (!(cond)) {                                               \
            std::cerr << "TEST FAIL: " << (msg) << "\n";             \
            ++g_test_failures;                                       \
        } else {                                                       \
            /* PASS: no output on success */                         \
        }                                                              \
    } while (0)

#define ASSERT_EQ(a, b, msg)                                          \
    do {                                                               \
        if (!((a) == (b))) {                                         \
            std::cerr << "TEST FAIL: " << (msg)                          \
                      << " (expected " << (b) << ", got " << (a) << ")"
                      << "\n";                                        \
            ++g_test_failures;                                       \
        } else {                                                       \
            /* PASS */                                               \
        }                                                              \
    } while (0)

#define RESET_STATE()                                                  \
    do {                                                               \
        mock_open_fail = 0;                                            \
        mock_readtag_null = 0;                                         \
        help_called = 0;                                               \
        show_called = 0;                                               \
        last_seen_wt_x = last_seen_wt_y = last_seen_wt_z = 0.0;       \
        xoptind = 1;                                                   \
    } while (0)

// Implementations of the mocked dependencies

extern "C" void InitUtils(const char* programName) {
    // In tests, mimic behavior: no options parsed; xoptind points to first non-option arg
    (void)programName;
    xoptind = 1;
}

extern "C" void HandleSwitches(int argc, char* argv[]) {
    // Minimal stub: do nothing for test scenarios
    (void)argc;
    (void)argv;
}

// Track Help invocation for test assertions
extern "C" void Help(void) {
    ++help_called;
}

// ShowWhitePoint records the white point for assertions
extern "C" void ShowWhitePoint(cmsCIEXYZ* WtPt) {
    ++show_called;
    if (WtPt) {
        last_seen_wt_x = WtPt->x;
        last_seen_wt_y = WtPt->y;
        last_seen_wt_z = WtPt->z;
    }
}

// Mock CMS Open/Read/Close

extern "C" cmsHPROFILE cmsOpenProfileFromFile(const char* name, const char* mode) {
    (void)mode;
    if (mock_open_fail) {
        return NULL;
    }
    // Return a non-null dummy handle
    return reinterpret_cast<cmsHPROFILE>(static_cast<void*>(reinterpret_cast<uintptr_t>(0xDEADBEEF)));
}

extern "C" cmsCIEXYZ* cmsReadTag(cmsHPROFILE hProfile, unsigned long tag) {
    (void)hProfile;
    (void)tag;
    if (mock_readtag_null) {
        return NULL;
    }
    // Return a pointer to a static local (per-call stable) copy
    static cmsCIEXYZ tmp;
    tmp = mock_white_point;
    return &tmp;
}

extern "C" void cmsCloseProfile(cmsHPROFILE hProfile) {
    (void)hProfile;
    // No-op for tests
}

// The testable replica of the focal main logic.
// We provide a function that mimics the original main behavior so tests can invoke it
// without needing to instantiate the actual program's main symbol.
static int run_wtpt_main(int argc, char* argv[]) {
    int nargs;
    InitUtils("wtpt");

    HandleSwitches(argc, argv);
    nargs = (argc - xoptind);
    if (nargs != 1) {
        Help();
    } else {
        cmsCIEXYZ* WtPt;
        cmsHPROFILE hProfile = cmsOpenProfileFromFile(argv[xoptind], "r");
        if (hProfile == NULL) return 1;
        WtPt = cmsReadTag(hProfile, cmsSigMediaWhitePointTag);
        ShowWhitePoint(WtPt);
        cmsCloseProfile(hProfile);
    }

    return 0;
}

// Helper to construct argv arrays conveniently
static char* make_argv(const std::vector<std::string>& args) {
    // Build a NULL-terminated array of C-strings
    static std::vector<char*> storage;
    storage.clear();
    for (const auto& s : args) {
        storage.push_back(const_cast<char*>(s.c_str()));
    }
    storage.push_back(nullptr);
    return nullptr; // not used; kept for clarity
}

// Simple test suite runner
static void run_all_tests() {
    std::vector<std::string> results;
    RESET_STATE();

    // Test Case 1: No extra arguments (argc == 1) => nargs = 0 => should call Help()
    {
        const char* prog = "wtpt";
        const char* arg0 = prog;
        char* argv0 = const_cast<char*>(arg0);
        char* argv1 = nullptr;
        // argc=1, argv[0]=wtpt
        int argc = 1;
        char* argv[] = { argv0, nullptr };

        RESET_STATE();
        int ret = run_wtpt_main(argc, argv);
        bool ok = (ret == 0) && (help_called == 1);
        results.push_back(ok ? "[PASS] Test 1: Help invoked on no args" :
                         "[FAIL] Test 1: Help invocation on no args");
        ASSERT_TRUE(ok, "Test 1: nargs == 0 should call Help and return 0");
        (void)ret; // explicit
    }

    // Test Case 2: Exactly one non-option argument => open profile, read tag, show point
    {
        RESET_STATE();
        // argc=2; argv[0]=wtpt; argv[1]="dummy_profile.prof"
        int argc = 2;
        char* argv0 = const_cast<char*>("wtpt");
        char* argv1 = const_cast<char*>("dummy_profile.prof");
        char* argv[] = { argv0, argv1, nullptr };

        mock_open_fail = 0;
        mock_readtag_null = 0;
        int ret = run_wtpt_main(argc, argv);
        bool ok = (ret == 0) && (help_called == 0) && (show_called == 1);
        ok = ok && (last_seen_wt_x == mock_white_point.x) &&
             (last_seen_wt_y == mock_white_point.y) &&
             (last_seen_wt_z == mock_white_point.z);
        results.push_back(ok ? "[PASS] Test 2: Successful open/read/show path" :
                         "[FAIL] Test 2: Successful open/read/show path");
        ASSERT_TRUE(ok, "Test 2: Should open profile, read WP tag, and show point");
    }

    // Test Case 3: Open profile fails (cmsOpenProfileFromFile returns NULL)
    {
        RESET_STATE();
        mock_open_fail = 1;
        int argc = 2;
        char* argv0 = const_cast<char*>("wtpt");
        char* argv1 = const_cast<char*>("dummy_profile.prof");
        char* argv[] = { argv0, argv1, nullptr };

        int ret = run_wtpt_main(argc, argv);
        bool ok = (ret == 1);
        results.push_back(ok ? "[PASS] Test 3: Return code 1 when profile open fails" :
                         "[FAIL] Test 3: Return code should be 1 when profile open fails");
        ASSERT_TRUE(ok, "Test 3: When cmsOpenProfileFromFile fails, main should return 1");
    }

    // Test Case 4: Too many arguments (nargs != 1) => should call Help()
    {
        RESET_STATE();
        int argc = 3;
        char* argv0 = const_cast<char*>("wtpt");
        char* argv1 = const_cast<char*>("arg1");
        char* argv2 = const_cast<char*>("arg2");
        char* argv[] = { argv0, argv1, argv2, nullptr };

        int ret = run_wtpt_main(argc, argv);
        bool ok = (ret == 0) && (help_called == 1);
        results.push_back(ok ? "[PASS] Test 4: Help invoked on too many args" :
                         "[FAIL] Test 4: Help invocation on too many args");
        ASSERT_TRUE(ok, "Test 4: nargs != 1 should call Help and return 0");
    }

    // Test Case 5: Read tag returns NULL => ShowWhitePoint should not be called
    {
        RESET_STATE();
        mock_readtag_null = 1;
        int argc = 2;
        char* argv0 = const_cast<char*>("wtpt");
        char* argv1 = const_cast<char*>("dummy_profile.prof");
        char* argv[] = { argv0, argv1, nullptr };

        int ret = run_wtpt_main(argc, argv);
        bool ok = (ret == 0) && (show_called == 0);
        results.push_back(ok ? "[PASS] Test 5: ReadTag NULL -> ShowWhitePoint not called" :
                         "[FAIL] Test 5: ReadTag NULL should prevent ShowWhitePoint");
        ASSERT_TRUE(ok, "Test 5: ReadTag returning NULL should not call ShowWhitePoint");
    }

    // Print summary
    int total = (int)results.size();
    int passed = total - g_test_failures;
    std::cout << "\nTEST SUMMARY: " << passed << " / " << total << " tests passed.\n";
    for (const auto& r : results) {
        // Each test result already indicates PASS/FAIL; print as well
        // (we added PASS/FAIL indicators above)
        if (r.find("[PASS]") != std::string::npos || r.find("[FAIL]") != std::string::npos) {
            // Already captured
            // Print each line for readability
            // (we can skip duplicates, but keeping it simple)
            // std::cout << r << "\n";
        }
    }

    // Return non-zero if any failure was recorded by the harness
    if (g_test_failures > 0) {
        std::cerr << "Some tests failed. Failures: " << g_test_failures << "\n";
        // Exit code indicates failure
        // In a larger framework, you'd propagate this result; here we simply print.
    } else {
        std::cout << "All tests finished successfully.\n";
    }
}

// Entry point for the test executable
int main(int argc, char* argv[]) {
    // If arguments are provided, we still run all tests; this binary acts as a standalone
    (void)argc;
    (void)argv;
    run_all_tests();
    return g_test_failures ? 1 : 0;
}