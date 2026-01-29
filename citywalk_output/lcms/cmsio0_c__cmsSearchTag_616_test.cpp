/*
 * Unit tests for the focal method: _cmsSearchTag
 * File under test: cmsio0.c
 *
 * Approach:
 * - Build a minimal in-process _cmsICCPROFILE object layout sufficient for
 *   exercising _cmsSearchTag via its dependent function SearchOneTag.
 * - Use public memory manipulations of TagLinked and TagNames to craft
 *   scenarios that cover:
 *     - Not found (-1 return)
 *     - Found without following links (lFollowLinks = FALSE)
 *     - Found with a single link followed
 *     - Found with a chain of two links followed
 * - Do not rely on any GTest; provide a tiny, non-terminating assertion style.
 * - Compile together with cmsio0.c (the focal implementation).
 *
 * Notes:
 * - We rely on the internal structure _cmsICCPROFILE as defined by
 *   lcms2_internal.h for TagLinked and TagNames arrays.
 * - We declare the focal function prototype to allow direct calls from C++ test.
 * - The tests are designed to be self-contained and not depend on file IO or
 *   mutex behavior; we only exercise the logical path inside _cmsSearchTag.
 *
 * Important: This test suite is written to be compiled in a C++11 environment
 * and linked with the cmsio0.c source (and its dependencies) in the same
 * executable.
 */

#include <vector>
#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Domain: include internal headers to obtain internal types
extern "C" {
}

// Focal function prototype (as it would be visible to external units).
extern "C" int _cmsSearchTag(_cmsICCPROFILE* Icc, cmsTagSignature sig, cmsBool lFollowLinks);

// Simple non-terminating assertion helpers
static int g_test_passed = 0;
static int g_test_failed = 0;

#define ASSERT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        std::cout << "[FAIL] " << msg << " | Expected: " << (b) << "  Got: " << (a) << "\n"; \
        ++g_test_failed; \
    } else { \
        std::cout << "[PASS] " << msg << "\n"; \
        ++g_test_passed; \
    } \
} while (0)

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cout << "[FAIL] " << msg << "\n"; \
        ++g_test_failed; \
    } else { \
        std::cout << "[PASS] " << msg << "\n"; \
        ++g_test_passed; \
    } \
} while (0)

// Helpers to create a small in-test ICC profile layout
// We directly manipulate _cmsICCPROFILE fields that are used by the focal function.
// This approach avoids depending on the entire profile lifecycle and focuses
// on the tag-search/link logic.

static _cmsICCPROFILE* make_profile(size_t tag_count,
                                    const cmsTagSignature* names,
                                    const cmsTagSignature* links)
{
    // Allocate the core profile object
    _cmsICCPROFILE* Icc = new _cmsICCPROFILE;
    if (!Icc) return nullptr;
    // Zero initialize to be safe
    std::memset(Icc, 0, sizeof(_cmsICCPROFILE));

    // Allocate TagNames and TagLinked arrays with given tag_count
    cmsTagSignature* tnames = new cmsTagSignature[tag_count];
    cmsTagSignature* tlinks = new cmsTagSignature[tag_count];
    if (!tnames || !tlinks) {
        delete Icc;
        delete[] tnames;
        delete[] tlinks;
        return nullptr;
    }
    // Copy provided data
    for (size_t i = 0; i < tag_count; ++i) {
        tnames[i] = names ? names[i] : 0;
        tlinks[i] = links ? links[i] : 0;
    }

    // Attach arrays to the profile
    Icc->TagNames = tnames;
    Icc->TagLinked = tlinks;

    // Note: We intentionally do not initialize other internal members (like IOhandler)
    // because _cmsSearchTag only uses TagLinked in this test path, and SearchOneTag
    // uses TagNames to locate tags. This minimal setup is sufficient for our purposes.

    // Return the constructed profile
    return Icc;
}

// Cleanup helper
static void free_profile(_cmsICCPROFILE* Icc)
{
    if (!Icc) return;
    delete[] Icc->TagNames;
    delete[] Icc->TagLinked;
    delete Icc;
}

// Test 1: Not found scenario
// - No tag matching sig exists in TagNames
static void test_not_found()
{
    // Tag set: A, B; no C. Attempt to search for non-existing tag 0xDEADBEEF.
    cmsTagSignature A = 0x111111;
    cmsTagSignature B = 0x222222;
    cmsTagSignature not_present = 0xDEADBEEF;

    cmsTagSignature names[2] = { A, B };
    cmsTagSignature links[2] = { 0, 0 }; // no link behavior

    _cmsICCPROFILE* Icc = make_profile(2, names, links);
    if (!Icc) {
        std::cout << "[WARN] test_not_found: failed to allocate profile.\n";
        ++g_test_failed;
        return;
    }

    int res = _cmsSearchTag(Icc, not_present, /*lFollowLinks=*/1);
    ASSERT_EQ(res, -1, "test_not_found: should return -1 when tag not present");

    free_profile(Icc);
}

// Test 2: Found without following links (lFollowLinks = false)
// - Tag A exists at index 0 and links to B; since lFollowLinks is false, we return the index of A.
static void test_found_no_follow()
{
    cmsTagSignature A = 0x101010;
    cmsTagSignature B = 0x202020;

    cmsTagSignature names[2] = { A, B };
    cmsTagSignature links[2] = { B, 0 }; // A links to B

    _cmsICCPROFILE* Icc = make_profile(2, names, links);
    if (!Icc) {
        std::cout << "[WARN] test_found_no_follow: failed to allocate profile.\n";
        ++g_test_failed;
        return;
    }

    int res = _cmsSearchTag(Icc, A, /*lFollowLinks=*/0);
    ASSERT_EQ(res, 0, "test_found_no_follow: should return index 0 for A when not following links");

    free_profile(Icc);
}

// Test 3: Found with a single link followed (A -> B, B has no further link)
// - Expect to land on B's index after following one link.
static void test_found_follow_once()
{
    cmsTagSignature A = 0xA0A0A0;
    cmsTagSignature B = 0xB0B0B0;

    cmsTagSignature names[2] = { A, B };
    cmsTagSignature links[2] = { B, 0 }; // A links to B; B has no link

    _cmsICCPROFILE* Icc = make_profile(2, names, links);
    if (!Icc) {
        std::cout << "[WARN] test_found_follow_once: failed to allocate profile.\n";
        ++g_test_failed;
        return;
    }

    int res = _cmsSearchTag(Icc, A, /*lFollowLinks=*/1);
    ASSERT_EQ(res, 1, "test_found_follow_once: should return index 1 (B) after following link from A");

    free_profile(Icc);
}

// Test 4: Chain of two links (A -> B -> C)
// - Expect to land on C's index after following two links.
static void test_found_follow_chain_two()
{
    cmsTagSignature A = 0x1A1A1A;
    cmsTagSignature B = 0x2B2B2B;
    cmsTagSignature C = 0x3C3C3C;

    cmsTagSignature names[3] = { A, B, C };
    cmsTagSignature links[3] = { B, C, 0 }; // A->B, B->C, C has no link

    _cmsICCPROFILE* Icc = make_profile(3, names, links);
    if (!Icc) {
        std::cout << "[WARN] test_found_chain_two: failed to allocate profile.\n";
        ++g_test_failed;
        return;
    }

    int res = _cmsSearchTag(Icc, A, /*lFollowLinks=*/1);
    ASSERT_EQ(res, 2, "test_found_chain_two: should return index 2 (C) after following two links");

    free_profile(Icc);
}

// Step 3: Run all tests and summarize
int main()
{
    std::cout << "Starting tests for _cmsSearchTag (cmsio0.c focal method)\n";

    test_not_found();
    test_found_no_follow();
    test_found_follow_once();
    test_found_follow_chain_two();

    std::cout << "\nTest summary: "
              << g_test_passed << " passed, "
              << g_test_failed << " failed.\n";

    // Return non-zero if any test failed
    return g_test_failed ? 1 : 0;
}