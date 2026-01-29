// Fast ICC loadProfile Unit Tests (C++11, without GTest)
// This test suite targets the focal function: cmsHPROFILE loadProfile(const char* name)
// located in fast_float_testbed.c. We provide lightweight C stubs for external
// dependencies so that the tests run without the real liblcms and without
// terminating the process on failures.
//
// Test goals (mapped to Candidate Keywords):
// - "*" predicates and builtin branches: *lab, *xyz, *curves
// - Fallback to file-based open when name does not start with '*'
// - Unknown builtin path triggers the builtin branch (but not a hard exit)
// - Coverage of both true/false branches via simple state tracking
//
// Notes:
// - We do not rely on exact pointer values returned by the library; instead, we
//   expose a small set of stubs that record which path was taken via a global flag.
// - All tests use non-terminating assertions: they record failures and continue.

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <iostream>
#include <cstdarg>
#include <memory.h>
#include <stdlib.h>


// We assume the focal function uses C linkage and returns a pointer type.
// For the test harness, we expose a C-compatible prototype that matches the
// common alias for cmsHPROFILE (likely void* in the real liblcms).
extern "C" void* loadProfile(const char* name);

// Sentinel stubs and tracking variables (shared across translation units)
extern "C" {

typedef void* cmsHPROFILE;

// Global markers for the stubs to return for different builtin paths.
// They are simple data addresses that are harmless to take as pointers.
int lab_marker   = 0;
int xyz_marker   = 0;
int curves_marker = 0;
int file_marker   = 0;

// State tracker to know which stub function was invoked by loadProfile.
int g_last_called = 0;

// Information captured when attempting to open a file path.
const char* g_last_open_name = nullptr;
const char* g_last_open_mode = nullptr;

// Prototypes for the stubs to satisfy the linker (even if not directly used by tests).
cmsHPROFILE cmsCreateLab4Profile(void*);
cmsHPROFILE cmsCreateXYZProfile(void);
cmsHPROFILE CreateCurves(void);

// Fail is called by the focal function on unknown builtin; implement as a no-op with logging.
void Fail(const char* frm, ...);

// cmsOpenProfileFromFile should be a test-friendly stub: returns non-null only for known test file.
cmsHPROFILE cmsOpenProfileFromFile(const char* name, const char* mode);
}

// Implementations of the stubs (C linkage) to replace real library calls

extern "C" {

cmsHPROFILE cmsCreateLab4Profile(void*)
{
    g_last_called = 1;
    return (cmsHPROFILE)&lab_marker;
}
cmsHPROFILE cmsCreateXYZProfile(void)
{
    g_last_called = 2;
    return (cmsHPROFILE)&xyz_marker;
}
cmsHPROFILE CreateCurves(void)
{
    g_last_called = 3;
    return (cmsHPROFILE)&curves_marker;
}

// Simple variadic Fail: counts how many times it's invoked (for coverage signaling)
static int g_fail_calls = 0;
void Fail(const char* frm, ...)
{
    (void)frm;
    va_list ap;
    va_start(ap, frm);
    (void)ap;
    va_end(ap);
    ++g_fail_calls;
}

// Open file profile stub: only recognizes a known test file, to allow deterministic tests.
// For any other name, it returns NULL to simulate missing/invalid ICC files.
cmsHPROFILE cmsOpenProfileFromFile(const char* name, const char* mode)
{
    g_last_open_name = name;
    g_last_open_mode = mode;
    if (name != nullptr && std::strcmp(name, "existing.profile") == 0)
    {
        g_last_called = 4;
        return (cmsHPROFILE)&file_marker;
    }
    // Unknown or non-existent file
    return nullptr;
}
} // extern "C"

// Local test framework (non-GTest, non-GMock style)

static int total_tests = 0;
static int passed_tests = 0;

// Simple verbose tester helper
static void log_result(const char* testName, bool ok)
{
    ++total_tests;
    if (ok)
    {
        ++passed_tests;
        std::cout << "[PASS] " << testName << "\n";
    }
    else
    {
        std::cout << "[FAIL] " << testName << "\n";
    }
}

// Test 1: Star path "*lab" should route to cmsCreateLab4Profile (g_last_called == 1)
static bool test_loadProfile_lab_builtin()
{
    g_last_called = 0;
    void* p = loadProfile("*lab");
    bool ok_ptr = (p != nullptr);
    bool ok_branch = (g_last_called == 1);
    // Explanatory comment:
    // - Verifies that the builtin "*lab" path is taken and a non-null profile is returned.
    // - Also asserts that the correct stub (lab) was invoked.
    return ok_ptr && ok_branch;
}

// Test 2: Star path "*xyz" should route to cmsCreateXYZProfile (g_last_called == 2)
static bool test_loadProfile_xyz_builtin()
{
    g_last_called = 0;
    void* p = loadProfile("*xyz");
    bool ok_ptr = (p != nullptr);
    bool ok_branch = (g_last_called == 2);
    // Explanatory comment:
    // - Verifies that the builtin "*xyz" path is taken and a non-null profile is returned.
    // - Confirms invocation of the xyz stub.
    return ok_ptr && ok_branch;
}

// Test 3: Star path "*curves" should route to CreateCurves (g_last_called == 3)
static bool test_loadProfile_curves_builtin()
{
    g_last_called = 0;
    void* p = loadProfile("*curves");
    bool ok_ptr = (p != nullptr);
    bool ok_branch = (g_last_called == 3);
    // Explanatory comment:
    // - Verifies that the builtin "*curves" path is taken and a non-null profile is returned.
    // - Confirms invocation of the curves stub.
    return ok_ptr && ok_branch;
}

// Test 4: Non-star path pointing to a known file "existing.profile" should call cmsOpenProfileFromFile
static bool test_loadProfile_existing_file_path()
{
    g_last_called = 0;
    void* p = loadProfile("existing.profile");
    bool ok_ptr = (p != nullptr);
    bool ok_branch = (g_last_called == 4);
    // Explanatory comment:
    // - Verifies the file-open path when a normal file name is provided.
    // - Expects a non-null result and the file-open stub to have been invoked.
    return ok_ptr && ok_branch;
}

// Test 5: Non-existing or unknown builtin path "*unknown" should attempt file open and return NULL
static bool test_loadProfile_unknown_builtin_fallback()
{
    g_last_called = 0;
    // Reset file-open tracking
    g_last_open_name = nullptr;
    void* p = loadProfile("*unknown");
    bool ok_ptr = (p == nullptr);
    bool ok_file_lookup = (g_last_open_name != nullptr) && (std::strcmp(g_last_open_name, "*unknown") == 0);
    // Explanatory comment:
    // - Ensures that unknown builtin triggers the fallback behavior and attempts to open a file with the given name.
    // - We expect NULL since "*unknown" should not map to a real ICC file in our test harness.
    return ok_ptr && ok_file_lookup;
}

// Main entry point: execute all tests and report summary
int main()
{
    test_loadProfile_lab_builtin()     ? log_result("loadProfile(*) -> *lab", true)
                                      : log_result("loadProfile(*) -> *lab", false);
    test_loadProfile_xyz_builtin()     ? log_result("loadProfile(*) -> *xyz", true)
                                      : log_result("loadProfile(*) -> *xyz", false);
    test_loadProfile_curves_builtin()  ? log_result("loadProfile(*) -> *curves", true)
                                      : log_result("loadProfile(*) -> *curves", false);
    test_loadProfile_existing_file_path() ? log_result("loadProfile(non-star) -> existing.profile", true)
                                           : log_result("loadProfile(non-star) -> existing.profile", false);
    test_loadProfile_unknown_builtin_fallback() ? log_result("loadProfile(*) -> unknown builtin fallback", true)
                                                : log_result("loadProfile(*) -> unknown builtin fallback", false);

    std::cout << "\nTest Summary: " << passed_tests << " / " << total_tests << " tests passed.\n";

    // Return non-zero if any test failed
    return (passed_tests == total_tests) ? 0 : 1;
}