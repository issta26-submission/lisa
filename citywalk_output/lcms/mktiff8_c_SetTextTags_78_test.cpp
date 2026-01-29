// Test suite for SetTextTags in mktiff8.c
// Note: This test suite uses lightweight in-process mocks for lcms2 functions
// to enable unit testing without requiring the actual Little CMS binary.
// The tests are written in C++11 and do not rely on GTest.
// Compileable in a C++11 environment with the actual mktiff8.c available for linkage.

#include <cstring>
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <lcms2.h>


// Forward declaration of the function under test (from mktiff8.c)
extern "C" cmsBool SetTextTags(cmsHPROFILE hProfile);

// ----------------------
// Mock infrastructure
// ----------------------

static int g_alloc_call_counter = 0;
static int g_alloc_fail_on_call = -1; // -1 means no forced failure
static int g_write_tag_calls = 0;
static cmsTagSignature g_written_sigs[4];
static int g_written_count = 0;

// Reset all mock state before each test
static void resetMocks() {
    g_alloc_call_counter = 0;
    g_alloc_fail_on_call = -1;
    g_write_tag_calls = 0;
    g_written_count = 0;
    // Clear recorded signatures
    for (int i = 0; i < 4; ++i) g_written_sigs[i] = 0;
}

// Mock implementations of lcms2 functions used by SetTextTags
extern "C" {

// Allocate a cmsMLU object. We simulate allocation success/failure based on test state.
cmsMLU* cmsMLUalloc(int, int) {
    ++g_alloc_call_counter;
    if (g_alloc_fail_on_call == g_alloc_call_counter) {
        return NULL; // Simulate allocation failure on the specified call
    }
    // Return a small dummy non-NULL pointer
    return (cmsMLU*)malloc(16);
}

// Free a cmsMLU object
void cmsMLUfree(cmsMLU* ml) {
    if (ml) {
        free(ml);
    }
}

// Set ASCII text in an MLU. Always succeeds if ml is non-NULL
cmsBool cmsMLUsetASCII(cmsMLU* /*ml*/, const char* /*lang*/, const char* /*country*/, const char* /*text*/) {
    // Accept any non-NULL ml
    return ml ? 1 : 0;
}

// Write a tag to the profile. We count calls and record first two signatures.
// We fail if either hProfile or ml (the MLU) is NULL to simulate real-world failure path.
cmsBool cmsWriteTag(cmsHPROFILE hProfile, cmsTagSignature sig, cmsMLU* ml) {
    if (g_written_count < 4) {
        g_written_sigs[g_written_count] = sig;
        ++g_written_count;
    }
    if (hProfile == NULL || ml == NULL) {
        return 0; // simulate failure when profile is NULL or MLU is NULL
    }
    ++g_write_tag_calls;
    return 1;
}
} // extern "C"

// ----------------------
// Test helpers
// ----------------------

// Simple assertion macro that logs failures but does not abort the test
#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[EXPECT_FAIL] " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
        g_failures++; \
    } else { \
        /* optional success log can be added */ \
    } \
} while (0)

static int g_failures = 0;

// Test 1: Success path - valid profile, both allocations succeed, writes occur for two tags
static bool test_SetTextTags_Success() {
    resetMocks();
    g_alloc_fail_on_call = -1; // no allocation failure
    cmsHPROFILE mockProfile = (cmsHPROFILE)0xdeadbeef; // arbitrary non-NULL

    cmsBool rc = SetTextTags(mockProfile);

    bool passed = true;
    EXPECT_TRUE(rc != 0, "SetTextTags should return TRUE on success");
    // Expect exactly two writes for the two tags
    EXPECT_TRUE(g_written_count == 2, "Two tags should be written (Description and Copyright)");
    // Verify the two signatures written correspond to the expected tags
    if (g_written_count >= 2) {
        EXPECT_TRUE(g_written_sigs[0] == cmsSigProfileDescriptionTag, "First write should be Profile Description tag");
        EXPECT_TRUE(g_written_sigs[1] == cmsSigCopyrightTag, "Second write should be Copyright tag");
    } else {
        passed = false;
    }

    // Also ensure no NULL MLU usage leaked or freed incorrectly (rudimentary check)
    // We rely on mocks freeing allocated blocks; no crash implies proper cleanup.
    if (rc == 0) passed = false;
    return passed;
}

// Test 2: Null profile path - cmsWriteTag should fail when hProfile is NULL
static bool test_SetTextTags_NullProfile() {
    resetMocks();
    g_alloc_fail_on_call = -1;
    cmsBool rc = SetTextTags(NULL);
    // Expect failure and no crash
    return (rc == FALSE);
}

// Test 3: Allocation failure on first cmsMLUalloc call (DescriptionMLU)
static bool test_SetTextTags_AllocFailure_First() {
    resetMocks();
    g_alloc_fail_on_call = 1; // fail on first allocation
    cmsBool rc = SetTextTags((cmsHPROFILE)0x1);
    return (rc == FALSE);
}

// Test 4: Allocation failure on second cmsMLUalloc call (CopyrightMLU)
static bool test_SetTextTags_AllocFailure_Second() {
    resetMocks();
    g_alloc_fail_on_call = 2; // fail on second allocation
    cmsBool rc = SetTextTags((cmsHPROFILE)0x1);
    return (rc == FALSE);
}

// ----------------------
// Test runner
// ----------------------

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    int total = 0;
    int passed = 0;

    auto runTest = [&](const char* name, bool (*fn)()) {
        ++total;
        bool ok = fn();
        if (ok) {
            ++passed;
            std::cout << "[PASS] " << name << "\n";
        } else {
            std::cerr << "[FAIL] " << name << "\n";
        }
    };

    // Run tests
    runTest("SetTextTags_Success", test_SetTextTags_Success);
    runTest("SetTextTags_NullProfile", test_SetTextTags_NullProfile);
    runTest("SetTextTags_AllocFailure_First", test_SetTextTags_AllocFailure_First);
    runTest("SetTextTags_AllocFailure_Second", test_SetTextTags_AllocFailure_Second);

    std::cout << "Tests passed: " << passed << " / " << total << "\n";
    // Non-terminating exit code: 0 on success, 1 on failure
    return (passed == total) ? 0 : 1;
}