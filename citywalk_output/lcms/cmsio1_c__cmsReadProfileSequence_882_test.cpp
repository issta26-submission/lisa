/*
Unit Test Suite for cmsSEQ* _cmsReadProfileSequence(cmsHPROFILE hProfile)
- Approach: Provide a set of test doubles (overriding the library's cmsReadTag, cmsDupProfileSequenceDescription,
  and cmsMLUdup) to simulate various scenarios without requiring an actual ICC profile file.
- Strategy: The focal function relies on two tags: ProfileSequenceDescTag and ProfileSequenceIdTag.
  We override these tag fetches to return pre-built cmsSEQ structures, then verify
  all branches (nulls, single/multiple sequences, and mixed merging).
- Environment: C++11, no GoogleTest. Tests run from main, with custom non-terminating assertions.
*/

#include <cstring>
#include <cstdio>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include liblcms types and tags for compatibility.
// This test uses internal types like cmsHPROFILE, cmsSEQ, cmsProfileID, cmsMLU, etc.
// The exact layout is assumed consistent with the library headers.
extern "C" {
}

// Forward declaration of the focal function (as it would be linked from cmsio1.c)
extern "C" cmsSEQ* _cmsReadProfileSequence(cmsHPROFILE hProfile);

// Global test doubles and state
static cmsSEQ* g_ProfileDescForRead = nullptr; // To be returned for cmsSigProfileSequenceDescTag
static cmsSEQ* g_ProfileIdForRead  = nullptr; // To be returned for cmsSigProfileSequenceIdTag

// Simple sentinels for cmsMLU pointers (no deep content required for current tests)
static cmsMLU g_DescSentinelA;
static cmsMLU g_DescSentinelB;
static cmsMLU g_DescSentinelC;

// Utility: Non-terminating test assertions
static int g_testFailures = 0;
static void test_report_failure(const char* testName, const char* msg) {
    std::fprintf(stderr, "TEST FAIL: %s - %s\n", testName, msg);
    ++g_testFailures;
}
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { test_report_failure(__FUNCTION__, "Expectation failed: " #cond); } \
} while(0)
#define EXPECT_EQ(a, b) do { \
    if((a) != (b)) { test_report_failure(__FUNCTION__, "Expectation failed: " #a " == " #b); } \
} while(0)
#define EXPECT_MEM_EQ(a, b, size) do { \
    if(0 != std::memcmp((a), (b), (size))) { test_report_failure(__FUNCTION__, "Memory content mismatch: " #a " vs " #b); } \
} while(0)

// Helper: Allocate a cmsSEQ with n elements, and fill ProfileID bytes with a pattern.
// Also assigns non-null Description pointers (sentinels) to enable merging tests.
static cmsSEQ* alloc_seq(int n, unsigned char pattern, cmsMLU* descPtrs[])
{
    // Compute size for cmsSEQ with n elements (seq[] is a flexible array of at least 1)
    size_t elemSize = sizeof(((cmsSEQ*)0)->seq[0]);
    size_t size = sizeof(cmsSEQ) + (size_t)(n - 1) * elemSize;
    cmsSEQ* seq = (cmsSEQ*)std::malloc(size);
    if (seq == nullptr) return nullptr;
    seq->n = (cmsUInt32Number)n;
    for (int i = 0; i < n; ++i) {
        // Fill ProfileID bytes with a deterministic pattern
        unsigned char* dst = (unsigned char*)(&(seq->seq[i].ProfileID));
        for (size_t b = 0; b < sizeof(cmsProfileID); ++b) {
            dst[b] = (unsigned char)(pattern + (unsigned char)i + (unsigned char)b);
        }
        // Assign Description pointer
        seq->seq[i].Description = descPtrs ? descPtrs[i] : nullptr;
    }
    return seq;
}

// Overridden library functions to simulate tag reads and duplications.
// These will be resolved by the linker before the library-provided implementations.
extern "C" {

// cmsReadTag override: return prepared tag payloads based on the requested signature.
void* cmsReadTag(cmsHPROFILE hProfile, cmsTagSignature sig)
{
    (void)hProfile; // Unused in tests; hProfile is a dummy handle
    if (sig == cmsSigProfileSequenceDescTag) {
        return (void*)g_ProfileDescForRead;
    } else if (sig == cmsSigProfileSequenceIdTag) {
        return (void*)g_ProfileIdForRead;
    }
    return nullptr;
}

// cmsDupProfileSequenceDescription override: deep-copy the provided cmsSEQ object.
cmsSEQ* cmsDupProfileSequenceDescription(const cmsSEQ* seq)
{
    if (seq == nullptr) return nullptr;
    // Allocate a new cmsSEQ with same n
    size_t elemSize = sizeof(((cmsSEQ*)0)->seq[0]);
    size_t size = sizeof(cmsSEQ) + (size_t)(seq->n - 1) * elemSize;
    cmsSEQ* copy = (cmsSEQ*)std::malloc(size);
    if (copy == nullptr) return nullptr;
    copy->n = seq->n;
    // Shallow copy of seq array (ProfileID is copied as bytes; Description pointer is copied)
    std::memcpy(copy, seq, size);
    return copy;
}

// cmsMLUdup override: For test simplicity, return the same pointer (simulate a shallow "dup")
// so that we can test pointer equality semantics in a deterministic way.
cmsMLU* cmsMLUdup(const cmsMLU* src)
{
    // In tests, return the same pointer to allow pointer equality checks.
    return (cmsMLU*)src;
}
} // extern "C"

// Helper: Build test sequences for readability
static void prepare_test_case_1_null_all()
{
    g_ProfileDescForRead = nullptr;
    g_ProfileIdForRead   = nullptr;
}
static void prepare_test_case_2_only_id()
{
    // ProfileSequenceDescTag => NULL, ProfileSequenceIdTag => non-NULL sequence
    g_ProfileDescForRead = nullptr;
    // Build a tiny ProfileId sequence (n=1)
    static cmsSEQ idSeq;
    static cmsMLU* descPtrs[1] = { &g_DescSentinelA };
    idSeq.n = 1;
    // Fill ProfileID bytes with a unique pattern for comparison
    unsigned char* dst = (unsigned char*)(&(idSeq.seq[0].ProfileID));
    for (size_t b = 0; b < sizeof(cmsProfileID); ++b) dst[b] = (unsigned char)(0x11 + b);
    idSeq.seq[0].Description = &g_DescSentinelA;
    // Copy into global, ensuring the instance persists
    g_ProfileIdForRead = &idSeq;
}
static void prepare_test_case_3_only_desc()
{
    // ProfileSequenceDescTag => non-NULL, ProfileSequenceIdTag => NULL
    // Build a tiny ProfileSeq sequence (n=1)
    static cmsSEQ descSeq;
    static cmsMLU* descPtrs[1] = { &g_DescSentinelB };
    descSeq.n = 1;
    unsigned char* dst = (unsigned char*)(&(descSeq.seq[0].ProfileID));
    for (size_t b = 0; b < sizeof(cmsProfileID); ++b) dst[b] = (unsigned char)(0x22 + b);
    descSeq.seq[0].Description = &g_DescSentinelB;
    g_ProfileDescForRead = &descSeq;
    g_ProfileIdForRead   = nullptr;
}
static void prepare_test_case_4_mismatch_n()
{
    // ProfileSequenceDescTag => n=2, ProfileSequenceIdTag => n=3
    static cmsSEQ descSeq;
    descSeq.n = 2;
    for (int i = 0; i < 2; ++i) {
        unsigned char* dst = (unsigned char*)(&(descSeq.seq[i].ProfileID));
        for (size_t b = 0; b < sizeof(cmsProfileID); ++b) dst[b] = (unsigned char)(0x30 + i + b);
        descSeq.seq[i].Description = (i == 0) ? &g_DescSentinelA : &g_DescSentinelB;
    }

    static cmsSEQ idSeq;
    idSeq.n = 3;
    for (int i = 0; i < 3; ++i) {
        unsigned char* dst = (unsigned char*)(&(idSeq.seq[i].ProfileID));
        for (size_t b = 0; b < sizeof(cmsProfileID); ++b) dst[b] = (unsigned char)(0x60 + i + b);
        idSeq.seq[i].Description = (i == 0) ? &g_DescSentinelC : &g_DescSentinelA;
    }

    g_ProfileDescForRead = &descSeq;
    g_ProfileIdForRead   = &idSeq;
}
static void prepare_test_case_5_equal_n()
{
    // Both present with equal n=2. We test merging logic and pointer copying of Description.
    static cmsSEQ descSeq;
    descSeq.n = 2;
    // Set distinct patterns for two elements
    unsigned char* p0 = (unsigned char*)(&(descSeq.seq[0].ProfileID));
    for (size_t b = 0; b < sizeof(cmsProfileID); ++b) p0[b] = (unsigned char)(0xA0 + b);
    unsigned char* p1 = (unsigned char*)(&(descSeq.seq[1].ProfileID));
    for (size_t b = 0; b < sizeof(cmsProfileID); ++b) p1[b] = (unsigned char)(0xB0 + b);
    descSeq.seq[0].Description = &g_DescSentinelA;
    descSeq.seq[1].Description = &g_DescSentinelB;

    static cmsSEQ idSeq;
    idSeq.n = 2;
    unsigned char* q0 = (unsigned char*)(&(idSeq.seq[0].ProfileID));
    unsigned char* q1 = (unsigned char*)(&(idSeq.seq[1].ProfileID));
    for (size_t b = 0; b < sizeof(cmsProfileID); ++b) q0[b] = (unsigned char)(0xC0 + b);
    for (size_t b = 0; b < sizeof(cmsProfileID); ++b) q1[b] = (unsigned char)(0xD0 + b);
    idSeq.seq[0].Description = &g_DescSentinelC;
    idSeq.seq[1].Description = &g_DescSentinelA;

    g_ProfileDescForRead = &descSeq;
    g_ProfileIdForRead   = &idSeq;
}

// Test runners
static void test_case_1_both_null()
{
    // Branch: both tags null -> expect NULL return
    prepare_test_case_1_null_all();
    cmsSEQ* res = _cmsReadProfileSequence((cmsHPROFILE)0xDEADBEEF);
    EXPECT_TRUE(res == nullptr);
}
static void test_case_2_profileid_only()
{
    prepare_test_case_2_only_id();
    cmsSEQ* res = _cmsReadProfileSequence(( cmsHPROFILE )0xBADC0DE);
    // Should be a copy of ProfileId (which has n=1)
    if (res == nullptr) {
        test_report_failure("test_case_2_profileid_only", "Expected non-NULL result");
        return;
    }
    EXPECT_TRUE(res->n == g_ProfileIdForRead->n);
    // Verify ProfileID bytes were copied
    EXPECT_MEM_EQ(&(res->seq[0].ProfileID), &(g_ProfileIdForRead->seq[0].ProfileID), sizeof(cmsProfileID));
    // Description should be copied as pointer (cmsDupProfileSequenceDescription copies the struct)
    EXPECT_EQ(res->seq[0].Description, g_ProfileIdForRead->seq[0].Description);
}
static void test_case_3_profiledesc_only()
{
    prepare_test_case_3_only_desc();
    cmsSEQ* res = _cmsReadProfileSequence((cmsHPROFILE)0xFACEB00C);
    if (res == nullptr) {
        test_report_failure("test_case_3_profiledesc_only", "Expected non-NULL result");
        return;
    }
    EXPECT_TRUE(res->n == g_ProfileDescForRead->n);
    EXPECT_MEM_EQ(&(res->seq[0].ProfileID), &(g_ProfileDescForRead->seq[0].ProfileID), sizeof(cmsProfileID));
    EXPECT_EQ(res->seq[0].Description, g_ProfileDescForRead->seq[0].Description);
}
static void test_case_4_mismatched_n()
{
    prepare_test_case_4_mismatch_n();
    cmsSEQ* res = _cmsReadProfileSequence((cmsHPROFILE)0x0D15EA5E);
    if (res == nullptr) {
        test_report_failure("test_case_4_mismatched_n", "Expected non-NULL result");
        return;
    }
    // Expect NewSeq to be a copy of ProfileSeq (n=2)
    EXPECT_TRUE(res->n == g_ProfileDescForRead->n);
}
static void test_case_5_equal_n_merge()
{
    prepare_test_case_5_equal_n();
    cmsSEQ* res = _cmsReadProfileSequence((cmsHPROFILE)0x0F0F0F0F);
    if (res == nullptr) {
        test_report_failure("test_case_5_equal_n_merge", "Expected non-NULL result");
        return;
    }
    // Check that for each i, ProfileID in NewSeq equals ProfileId's corresponding
    for (int i = 0; i < (int)res->n; ++i) {
        EXPECT_MEM_EQ(&(res->seq[i].ProfileID), &(g_ProfileIdForRead->seq[i].ProfileID), sizeof(cmsProfileID));
        // cmsMLUdup returned the same pointer in our test harness; verify pointer equality
        EXPECT_EQ(res->seq[i].Description, g_ProfileIdForRead->seq[i].Description);
    }
}

// Warn if any test fails
static void summarize_results()
{
    if (g_testFailures == 0) {
        std::printf("ALL TESTS PASSED\n");
    } else {
        std::fprintf(stderr, "%d TEST(S) FAILED\n", g_testFailures);
    }
}

// Entry point
int main()
{
    // Initialize sentinel MLUs (no real content needed for these tests)
    (void)g_DescSentinelA; (void)g_DescSentinelB; (void)g_DescSentinelC;

    // Run tests
    test_case_1_both_null();
    test_case_2_profileid_only();
    test_case_3_profiledesc_only();
    test_case_4_mismatched_n();
    test_case_5_equal_n_merge();

    summarize_results();
    return (g_testFailures == 0) ? 0 : 1;
}