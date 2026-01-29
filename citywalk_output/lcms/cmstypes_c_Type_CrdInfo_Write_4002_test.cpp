/*
  Unit Test Suite for Type_CrdInfo_Write (cmstypes.c)

  Important notes and how this test works:
  - The actual Type_CrdInfo_Write function depends on an external helper
    WriteCountAndString(self, io, mlu, Section). To test the control flow
    (the true/false branches), we intercept the calls to WriteCountAndString
    by using the linker wrap feature.
  - Build this test together with the original cmstypes.c (the focal code)
    and link with the wrapper mechanism:
      g++ test_cmstypes_crdinfo.cpp cmstypes.c -o test_crdinfo -Wl,-wrap=WriteCountAndString
  - The wrapper function __wrap_WriteCountAndString is implemented in this test
    file and decides whether to return TRUE or FALSE based on the Section
    argument (nm, #0, #1, #2, #3). This lets us exercise both true and
    false branches of Type_CrdInfo_Write without having to depend on the
    internal cmsMLU layout or actual IO layer implementation.
  - The tests use minimal, opaque forward-declared types to avoid requiring
    full, project-specific type definitions in the test translation unit.
  - Tests are non-terminating assertions style (they print PASS/FAIL and continue),
  - All static/class members in the tested code are accessed through normal
    compilation (no direct access to private/internal data). Static internals
    of the focal file remain in the original translation unit.

  How to run (example):
    g++ test_cmstypes_crdinfo.cpp cmstypes.c -o test_crdinfo -Wl,-wrap=WriteCountAndString
    ./test_crdinfo

  This file contains:
  - Lightweight scaffolding to invoke Type_CrdInfo_Write under wrapped WriteCountAndString
  - Four test scenarios:
      1) All five WriteCountAndString calls succeed (should yield TRUE)
      2) nm write fails (should yield FALSE)
      3) #0 write fails (should yield FALSE)
      4) #1, #2, #3 succeed but an earlier one fails (coverage of mid-branch)
  - Explanatory comments for each unit test.
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Lightweight forward declarations to avoid depending on the full cms headers.
// These types are opaque for the purposes of the wrapper tests.
typedef int cmsBool;            // Assume cmsBool is int-like in your build
struct _cms_typehandler_struct;   // forward declaration for self
struct cmsIOHANDLER;              // opaque IO handler
struct cmsMLU;                      // multi-language text unit
typedef unsigned int cmsUInt32Number; // size/count types used by the API

// The focal function under test is declared in the library cmstypes.c.
// We declare it here so the linker can resolve it properly when building.
// Signature mirrors the one in the source snippet you provided.
extern "C" cmsBool Type_CrdInfo_Write(struct _cms_typehandler_struct* self,
                                     cmsIOHANDLER* io,
                                     void* Ptr,
                                     cmsUInt32Number nItems);

// ---------------------------------------------------------------------------
// Wrapper to intercept WriteCountAndString calls via linker wrap.
// This allows us to simulate both success and failure branches without
// depending on the real internal CMSMLU layout.
// IMPORTANT: Build with -Wl,-wrap=WriteCountAndString to enable this.
// ---------------------------------------------------------------------------

// Forward declare the wrapped function (we don't provide a real definition here
// because the real implementation lives in cmstypes.c; the linker wrap will
// replace calls to WriteCountAndString with this wrapper).
extern "C" cmsBool __wrap_WriteCountAndString(struct _cms_typehandler_struct* self,
                                             cmsIOHANDLER* io,
                                             cmsMLU* mlu,
                                             const char* Section);

// Global test control: which Section should fail (index mapping defined below).
// If gFailIndex == -1, no failure is simulated.
static int gFailIndex = -1; // -1 = no fail, 0..4 correspond to sections nm, #0, #1, #2, #3

// Helper to map Section string to an index.
// nm -> 0, #0 -> 1, #1 -> 2, #2 -> 3, #3 -> 4
static int SectionToIndex(const char* Section) {
    if (Section == nullptr) return -1;
    if (strcmp(Section, "nm") == 0) return 0;
    if (strcmp(Section, "#0") == 0) return 1;
    if (strcmp(Section, "#1") == 0) return 2;
    if (strcmp(Section, "#2") == 0) return 3;
    if (strcmp(Section, "#3") == 0) return 4;
    return -1;
}

// Implementation of the wrapper (only does not dereference CMS types).
// It simulates the behavior: return FALSE for the designated failing section,
// otherwise return TRUE.
extern "C" cmsBool __wrap_WriteCountAndString(struct _cms_typehandler_struct* self,
                                             cmsIOHANDLER* io,
                                             cmsMLU* mlu,
                                             const char* Section) {
    (void)self; // unused in test wrapper
    (void)io;
    (void)mlu;
    if (Section == nullptr) return 1; // treat null as success by default

    int idx = SectionToIndex(Section);
    if (idx < 0) {
        // Unknown section => treat as success to avoid false negatives
        return 1;
    }

    if (gFailIndex == idx) {
        // Simulated failure for this section
        return 0;
    }

    // Otherwise success
    return 1;
}

// ---------------------------------------------------------------------------
// Test harness
// We purposely keep the test code self-contained and independent from the
// real CMS internals. The Type_CrdInfo_Write function will be exercised
// via the wrapper to verify branch behavior.
// ---------------------------------------------------------------------------

static void runTest_AllSectionsSucceed() {
    gFailIndex = -1; // no failure
    cmsBool res = Type_CrdInfo_Write(nullptr, nullptr, nullptr, 0);
    if (res) {
        std::cout << "[PASS] Type_CrdInfo_Write: all sections succeed (TRUE returned)" << std::endl;
    } else {
        std::cout << "[FAIL] Type_CrdInfo_Write: expected TRUE when all sections succeed" << std::endl;
    }
}

static void runTest_FailNm() {
    gFailIndex = 0; // fail "nm"
    cmsBool res = Type_CrdInfo_Write(nullptr, nullptr, nullptr, 0);
    if (!res) {
        std::cout << "[PASS] Type_CrdInfo_Write: nm failure correctly returns FALSE" << std::endl;
    } else {
        std::cout << "[FAIL] Type_CrdInfo_Write: expected FALSE when nm write fails" << std::endl;
    }
}

static void runTest_FailHash0() {
    gFailIndex = 1; // fail "#0"
    cmsBool res = Type_CrdInfo_Write(nullptr, nullptr, nullptr, 0);
    if (!res) {
        std::cout << "[PASS] Type_CrdInfo_Write: #0 failure correctly returns FALSE" << std::endl;
    } else {
        std::cout << "[FAIL] Type_CrdInfo_Write: expected FALSE when #0 write fails" << std::endl;
    }
}

static void runTest_FailHash1Or3() {
    // We'll test #1 failure path (and we could expand to #2, #3 similarly)
    gFailIndex = 2; // fail "#1"
    cmsBool res = Type_CrdInfo_Write(nullptr, nullptr, nullptr, 0);
    if (!res) {
        std::cout << "[PASS] Type_CrdInfo_Write: #1 failure correctly returns FALSE" << std::endl;
    } else {
        std::cout << "[FAIL] Type_CrdInfo_Write: expected FALSE when #1 write fails" << std::endl;
    }
}

// Entry point
int main() {
    // Explain what is being tested
    std::cout << "Running unit tests for Type_CrdInfo_Write (cmstypes.c) via WriteCountAndString wrapper" << std::endl;

    // Test 1: all five calls succeed -> TRUE
    runTest_AllSectionsSucceed();

    // Test 2: nm fails -> FALSE
    runTest_FailNm();

    // Test 3: #0 fails -> FALSE
    runTest_FailHash0();

    // Test 4: #1 fails -> FALSE
    runTest_FailHash1Or3();

    // If you want to add more tests, you can reuse the same approach by changing gFailIndex
    // to 3 (#2) or 4 (#3) and asserting the expected FALSE result.

    return 0;
}

// Explanatory notes for developers (not part of execution):
// - The test relies on linker wrapping to intercept the internal call to WriteCountAndString.
// - Build command (example):
//     g++ test_cmstypes_crdinfo.cpp cmstypes.c -o test_crdinfo -Wl,-wrap=WriteCountAndString
// - In a production environment, you may want to extend tests to cover more edge cases,
//   verify that nItems is indeed ignored (as per the function body), and add tests
//   for multiple successive failures or different calling sequences.
// - This test intentionally uses minimal, opaque type declarations to avoid coupling
//   to private/internal structures of cmsmlu and cmsIOHANDLER. The wrapper controls
//   the behavior of WriteCountAndString to exercise both TRUE and FALSE branches.