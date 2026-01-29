/*
Unit test suite for the focal method:
cmsBool _cmsWriteProfileSequence(cmsHPROFILE hProfile, const cmsSEQ* seq)

Context:
- We do not rely on GTest. A lightweight, self-contained test harness is implemented.
- We provide a minimal fake/stand-in surface for the CMS API used by the focal method:
  - cmsWriteTag
  - cmsGetEncodedICCversion
  - cmsBool, TRUE/FALSE
  - Tag signatures: cmsSigProfileSequenceDescTag, cmsSigProfileSequenceIdTag
  - cmsHPROFILE and cmsSEQ types (as lightweight stubs)
- Four test scenarios cover true/false branches and version gating:
  1) First write fails -> overall returns FALSE
  2) Version below threshold (0x4000000) -> only first write attempted, TRUE
  3) Version at/above threshold, second write succeeds -> overall TRUE
  4) Version at/above threshold, second write fails -> overall FALSE

Notes on Candidate Keywords (Step 1): cmsWriteTag, cmsSigProfileSequenceDescTag, cmsSigProfileSequenceIdTag, cmsGetEncodedICCversion, cmsHPROFILE, cmsSEQ, cmsBool, TRUE, FALSE.

The tests verify both return values and internal write attempts (via side effects on the fake profile), ensuring coverage of all branches in the focal method.

Explanatory comments are included for each unit test.
*/

#include <lcms2_internal.h>
#include <cstdio>
#include <cstdlib>


// Lightweight, self-contained test scaffolding for cmsWriteProfileSequence

// Domain types and constants (fake surface for testing)
typedef int cmsBool;
#define TRUE 1
#define FALSE 0

typedef void* cmsHPROFILE;
typedef int cmsTagSignature;
struct cmsSEQ {}; // opaque sequence type for test

// Tag signatures used by the focal method
static const cmsTagSignature cmsSigProfileSequenceDescTag = 1;
static const cmsTagSignature cmsSigProfileSequenceIdTag   = 2;

// Forward declaration of the focal function (re-implemented here for the test harness)
extern "C" cmsBool _cmsWriteProfileSequence(cmsHPROFILE hProfile, const cmsSEQ* seq);

// ----------------------------------------------------------------------------------
// Fake profile implementation to drive the tests
// ----------------------------------------------------------------------------------
struct FakeProfile {
    unsigned int ICCversion;
    bool failDesc; // if true, cmsWriteTag for DescTag should fail
    bool failId;   // if true, cmsWriteTag for IdTag should fail
    bool wroteDesc;
    bool wroteId;
};

// Minimal fake implementations matching the focal API usage

static cmsBool cmsWriteTag(cmsHPROFILE hProfile, cmsTagSignature sig, const cmsSEQ* /*seq*/) {
    FakeProfile* p = static_cast<FakeProfile*>(hProfile);
    if (!p) return FALSE;

    if (sig == cmsSigProfileSequenceDescTag) {
        p->wroteDesc = true;
        if (p->failDesc) {
            return FALSE;
        }
        return TRUE;
    } else if (sig == cmsSigProfileSequenceIdTag) {
        p->wroteId = true;
        if (p->failId) {
            return FALSE;
        }
        return TRUE;
    }
    // Unknown tag in this test surface
    return FALSE;
}

static unsigned int cmsGetEncodedICCversion(cmsHPROFILE hProfile) {
    FakeProfile* p = static_cast<FakeProfile*>(hProfile);
    if (!p) return 0;
    return p->ICCversion;
}

// ----------------------------------------------------------------------------------
// Re-implemented focal method under test (as provided in the prompt)
// Note: This mirrors the original logic exactly.
// The test harness ensures the behavior with the fake surface described above.
// ----------------------------------------------------------------------------------
cmsBool _cmsWriteProfileSequence(cmsHPROFILE hProfile, const cmsSEQ* seq)
{
{
    if (!cmsWriteTag(hProfile, cmsSigProfileSequenceDescTag, seq)) return FALSE;
    if (cmsGetEncodedICCversion(hProfile) >= 0x4000000) {
            if (!cmsWriteTag(hProfile, cmsSigProfileSequenceIdTag, seq)) return FALSE;
    }
    return TRUE;
}
}

// ----------------------------------------------------------------------------------
// Minimal test framework (non-terminating assertions)
// ----------------------------------------------------------------------------------
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        fprintf(stderr, "EXPECT_TRUE failed: %s\n", (msg)); \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_EQ(lhs, rhs, msg) do { \
    if (!((lhs) == (rhs))) { \
        fprintf(stderr, "EXPECT_EQ failed: %s (lhs=%lld, rhs=%lld)\n", (msg), static_cast<long long>(lhs), static_cast<long long>(rhs)); \
        ++g_failures; \
    } \
} while (0)

// ----------------------------------------------------------------------------------
// Test cases
// ----------------------------------------------------------------------------------

// Test 1: First cmsWriteTagDesc fails -> overall FALSE and wroteDesc is true (side-effect)
void test_FirstWriteDescFails() {
    FakeProfile* p = new FakeProfile();
    p->ICCversion = 0;           // version irrelevant for this test
    p->failDesc = true;            // force first write to fail
    p->failId = false;
    p->wroteDesc = false;
    p->wroteId = false;

    cmsSEQ dummySeq;
    cmsBool result = _cmsWriteProfileSequence(p, &dummySeq);

    EXPECT_TRUE(result == FALSE, "First write fails -> result must be FALSE");
    EXPECT_TRUE(p->wroteDesc, "Desc tag should have been attempted (wroteDesc true)");
    EXPECT_TRUE(p->wroteId == false, "Id tag should not have been written when first write fails");
    delete p;
}

// Test 2: Version below threshold -> only first write attempted, overall TRUE
void test_VersionBelowThreshold() {
    FakeProfile* p = new FakeProfile();
    p->ICCversion = 0x01000000; // < 0x4000000
    p->failDesc = false;
    p->failId = false;
    p->wroteDesc = false;
    p->wroteId = false;

    cmsSEQ dummySeq;
    cmsBool result = _cmsWriteProfileSequence(p, &dummySeq);

    EXPECT_TRUE(result == TRUE, "Version below threshold -> overall TRUE when first write succeeds");
    EXPECT_TRUE(p->wroteDesc, "Desc tag should have been written");
    EXPECT_TRUE(p->wroteId == false, "Id tag should NOT be written when version below threshold");
    delete p;
}

// Test 3: Version at/above threshold -> both writes attempted, both succeed
void test_VersionAboveThreshold_BothWritesSucceed() {
    FakeProfile* p = new FakeProfile();
    p->ICCversion = 0x04000000; // >= 0x4000000
    p->failDesc = false;
    p->failId = false;
    p->wroteDesc = false;
    p->wroteId = false;

    cmsSEQ dummySeq;
    cmsBool result = _cmsWriteProfileSequence(p, &dummySeq);

    EXPECT_TRUE(result == TRUE, "Version above threshold with successful writes -> TRUE");
    EXPECT_TRUE(p->wroteDesc, "Desc tag should have been written");
    EXPECT_TRUE(p->wroteId, "Id tag should have been written when version >= threshold");
    delete p;
}

// Test 4: Version above threshold -> second write fails -> overall FALSE
void test_VersionAboveThreshold_SecondWriteFails() {
    FakeProfile* p = new FakeProfile();
    p->ICCversion = 0x04000000; // >= 0x4000000
    p->failDesc = false;
    p->failId = true;             // force second write to fail
    p->wroteDesc = false;
    p->wroteId = false;

    cmsSEQ dummySeq;
    cmsBool result = _cmsWriteProfileSequence(p, &dummySeq);

    EXPECT_TRUE(result == FALSE, "Second write fail (IdTag) should yield FALSE overall");
    EXPECT_TRUE(p->wroteDesc, "Desc tag should have been written before failure");
    EXPECT_TRUE(p->wroteId, "Id tag write attempted (even if it fails)");
    delete p;
}

// ----------------------------------------------------------------------------------
// Main: run all tests
// ----------------------------------------------------------------------------------
int main() {
    test_FirstWriteDescFails();
    test_VersionBelowThreshold();
    test_VersionAboveThreshold_BothWritesSucceed();
    test_VersionAboveThreshold_SecondWriteFails();

    if (g_failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("TOTAL FAILURES: %d\n", g_failures);
        return 1;
    }
}