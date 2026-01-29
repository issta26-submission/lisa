/*
 * Lightweight unit tests for the SetLinks function in cmsio0.c
 * - No GTest usage; a small custom test harness is implemented.
 * - Tests rely on internal CMS structures exposed by lcms2_internal.h.
 * - Uses extern "C" for the SetLinks function to avoid C++ name mangling.
 * - Tests cover true/false branches of conditional predicates in SetLinks.
 *
 * Build notes (example):
 *   g++ -std=c++11 -I/path/to/include -L/path/to/lib -llcms2 -lm -o test_SetLinks test_SetLinks.cpp
 *
 * Important: This test assumes access to the internal _cmsICCPROFILE structure
 * and its fields used by SetLinks. It uses the public internal header to reach
 * those definitions. The tests allocate a minimal _cmsICCPROFILE with only the
 * fields required by SetLinks.
 */

#include <cstring>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cstdint>


// Use extern "C" for C linkage to link with the library's SetLinks function.
extern "C" {
    // Include internal CMS definitions to access _cmsICCPROFILE structure and types.
    // This header is part of the library's private API and may vary between versions.
    #include "lcms2_internal.h"

    // Prototype for the function under test (from cmsio0.c)
    cmsBool SetLinks(_cmsICCPROFILE* Icc);
}

// Helper: minimal test framework (non-terminating assertions).
static int g_testFailures = 0;
#define RUN_TEST(name) do { \
    if (!(name)) { \
        std::cerr << "Test failed: " #name "\n"; \
        ++g_testFailures; \
    } else { \
        std::cout << "Test passed: " #name "\n"; \
    } \
} while (0)


// Utility to allocate and initialize a minimal Icc structure for testing.
static _cmsICCPROFILE* CreateTestIcc(size_t count) {
    _cmsICCPROFILE* Icc = reinterpret_cast<_cmsICCPROFILE*>(std::calloc(1, sizeof(_cmsICCPROFILE)));
    if (!Icc) return nullptr;

    // Initialize the required fields for SetLinks.
    Icc->TagCount = static_cast<CmsUInt32Number>(count);

    // Allocate arrays used by SetLinks and related internal functions.
    Icc->TagLinked = reinterpret_cast<cmsTagSignature*>(std::calloc(count, sizeof(cmsTagSignature)));
    Icc->TagOffsets = reinterpret_cast< cmsUInt32Number* >(std::calloc(count, sizeof(cmsUInt32Number)));
    Icc->TagSizes   = reinterpret_cast< cmsUInt32Number* >(std::calloc(count, sizeof(cmsUInt32Number)));
    Icc->TagNames   = reinterpret_cast<cmsTagSignature*>(std::calloc(count, sizeof(cmsTagSignature)));

    // Initialize a minimal context-like field if present (to be safe).
    Icc->ContextID = 0;
    // Other fields not needed for SetLinks in these tests.

    return Icc;
}

// Cleanup utility for the test Icc structure.
static void DestroyTestIcc(_cmsICCPROFILE* Icc) {
    if (!Icc) return;
    if (Icc->TagLinked) std::free(Icc->TagLinked);
    if (Icc->TagOffsets) std::free(Icc->TagOffsets);
    if (Icc->TagSizes)   std::free(Icc->TagSizes);
    if (Icc->TagNames)   std::free(Icc->TagNames);
    std::free(Icc);
}


// Test 1: Normal operation where at least one linked tag resolves to an existing tag.
// Expected: TagOffsets[i] and TagSizes[i] for linked i are updated to those of the
// destination tag j (where TagNames[j] matches TagLinked[i]).
static bool test_SetLinks_NormalCopy() {
    // Create a small ICC with 3 tags.
    _cmsICCPROFILE* Icc = CreateTestIcc(3);
    if (!Icc) {
        std::cerr << "Failed to allocate test ICC in NormalCopy test.\n";
        return false;
    }

    // Configure tags:
    // - i = 0: linked to 0x1111, tag 0 itself has name 0x1111
    // - i = 1: not linked (TagLinked[1] == 0)
    // - i = 2: linked to 0x1111 (same as tag 0)
    Icc->TagCount = 3;
    Icc->TagLinked[0] = 0x1111;
    Icc->TagLinked[1] = 0;
    Icc->TagLinked[2] = 0x1111;

    // Destination tag is tag 0
    Icc->TagOffsets[0] = 5;
    Icc->TagSizes[0]   = 10;

    // Pre-fill other tags with different values to ensure they can be overwritten.
    Icc->TagOffsets[1] = 7;
    Icc->TagSizes[1]   = 20;

    // Tag 2 will expect to copy from tag 0 after linking
    Icc->TagOffsets[2] = 0;
    Icc->TagSizes[2]   = 0;

    // TagNames: ensure a match exists for lnk = 0x1111
    Icc->TagNames[0] = 0x1111;
    Icc->TagNames[1] = 0x2222;
    Icc->TagNames[2] = 0x3333; // does not matter for matching

    // Call the function under test
    cmsBool res = SetLinks(Icc);

    // Validate: function should return TRUE
    if (res != TRUE) {
        DestroyTestIcc(Icc);
        return false;
    }

    // Validate that TagOffsets[2] and TagSizes[2] now match those of tag 0
    bool ok = (Icc->TagOffsets[2] == Icc->TagOffsets[0]) &&
              (Icc->TagSizes[2]   == Icc->TagSizes[0]);

    // Validate that unrelated tag (index 1) remained unchanged
    ok = ok && (Icc->TagOffsets[1] == 7) && (Icc->TagSizes[1] == 20);

    DestroyTestIcc(Icc);
    return ok;
}


// Test 2: Branch where lnk == 0 should not trigger any update for that i.
// All TagLinked entries are zero; nothing should be modified.
static bool test_SetLinks_NoLinkedTags() {
    _cmsICCPROFILE* Icc = CreateTestIcc(3);
    if (!Icc) {
        std::cerr << "Failed to allocate test ICC in NoLinkedTags test.\n";
        return false;
    }

    Icc->TagCount = 3;
    Icc->TagLinked[0] = 0;
    Icc->TagLinked[1] = 0;
    Icc->TagLinked[2] = 0;

    Icc->TagOffsets[0] = 11;
    Icc->TagSizes[0]   = 22;

    Icc->TagOffsets[1] = 33;
    Icc->TagSizes[1]   = 44;

    Icc->TagOffsets[2] = 55;
    Icc->TagSizes[2]   = 66;

    Icc->TagNames[0] = 0xAAAA;
    Icc->TagNames[1] = 0xBBBB;
    Icc->TagNames[2] = 0xCCCC;

    cmsBool res = SetLinks(Icc);

    bool ok = (res == TRUE) &&
              (Icc->TagOffsets[0] == 11) && (Icc->TagSizes[0] == 22) &&
              (Icc->TagOffsets[1] == 33) && (Icc->TagSizes[1] == 44) &&
              (Icc->TagOffsets[2] == 55) && (Icc->TagSizes[2] == 66);

    DestroyTestIcc(Icc);
    return ok;
}


// Test 3: Linked tag with no corresponding destination (j < 0).
// Here we intentionally use a lnk value that does not exist in TagNames.
// Expect no modifications to any TagOffsets/TagSizes.
static bool test_SetLinks_NoMatchingLinkedTag() {
    _cmsICCPROFILE* Icc = CreateTestIcc(2);
    if (!Icc) {
        std::cerr << "Failed to allocate test ICC in NoMatchingLinkedTag test.\n";
        return false;
    }

    Icc->TagCount = 2;
    Icc->TagLinked[0] = 0xDEAD; // not present in TagNames
    Icc->TagLinked[1] = 0;      // unlinked

    Icc->TagOffsets[0] = 9;
    Icc->TagSizes[0]   = 18;
    Icc->TagOffsets[1] = 27;
    Icc->TagSizes[1]   = 54;

    Icc->TagNames[0] = 0xBEEF;
    Icc->TagNames[1] = 0xFEED;

    cmsBool res = SetLinks(Icc);

    bool ok = (res == TRUE) &&
              (Icc->TagOffsets[0] == 9) && (Icc->TagSizes[0] == 18) &&
              (Icc->TagOffsets[1] == 27) && (Icc->TagSizes[1] == 54);

    DestroyTestIcc(Icc);
    return ok;
}


int main() {
    std::cout << "Running SetLinks unit tests (cmsio0.c) - C++11 test harness\n";

    RUN_TEST(test_SetLinks_NormalCopy);
    RUN_TEST(test_SetLinks_NoLinkedTags);
    RUN_TEST(test_SetLinks_NoMatchingLinkedTag);

    if (g_testFailures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_testFailures << " test(s) failed.\n";
        return 1;
    }
}