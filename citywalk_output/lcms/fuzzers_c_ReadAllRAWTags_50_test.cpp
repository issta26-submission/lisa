// Unit test suite for ReadAllRAWTags in fuzzers.c-like environment.
// This test suite provides a minimal, non-GTest framework suitable for C++11.
// It mocks the minimal CMS (lcms) API used by ReadAllRAWTags and validates behavior.
// The goal is to exercise the ReadAllRAWTags function in scenarios with 0, 3, and multiple tags,
// ensuring the loop executes correctly and that the API is called in the expected order.

#include <lcms2_plugin.h>
#include <vector>
#include <iostream>
#include <stdint.h>
#include <cstdint>


// -----------------------------------------------------------------------------
// Lightweight CMS-like type definitions to compile the focal method.
// We provide just enough types to model the API used by ReadAllRAWTags.
// -----------------------------------------------------------------------------

typedef void* cmsHPROFILE;
typedef int   cmsInt32Number;
typedef uint32_t cmsTagSignature;

// Forward declaration of the focal method to be tested.
// In the real project, this would be pulled from fuzzers.c.
void ReadAllRAWTags(cmsHPROFILE h);

// -----------------------------------------------------------------------------
// Focal method (as provided). Kept identical to the original structure.
// -----------------------------------------------------------------------------

void ReadAllRAWTags(cmsHPROFILE h)
{
{
    cmsInt32Number i, n;
    cmsTagSignature sig;
    cmsInt32Number len;
    n = cmsGetTagCount(h);
    for (i = 0; i < n; i++) {
        sig = cmsGetTagSignature(h, i);
        len = cmsReadRawTag(h, sig, NULL, 0);
    }
}
}

// -----------------------------------------------------------------------------
// Mocked CMS API hooks and test scaffolding.
// -----------------------------------------------------------------------------

// Simple fake profile structure used by tests.
struct FakeProfile {
    size_t tagCount;
    cmsTagSignature* signatures;
};

// Global test state to capture API interactions for verification.
static std::vector<cmsTagSignature> g_readLog;          // Signatures passed to cmsReadRawTag
static std::vector<int>          g_signatureIndexLog;  // Indices passed to cmsGetTagSignature
static int                         g_tagCountCalls = 0;    // How many times cmsGetTagCount invoked
static int                         g_totalFailures = 0;    // Non-terminating, accumulate failures

// Accessors that the focal method uses (mock implementations)
cmsInt32Number cmsGetTagCount(cmsHPROFILE h)
{
    ++g_tagCountCalls;
    FakeProfile* p = static_cast<FakeProfile*>(h);
    return static_cast<cmsInt32Number>(p ? p->tagCount : 0);
}

cmsTagSignature cmsGetTagSignature(cmsHPROFILE h, cmsInt32Number i)
{
    g_signatureIndexLog.push_back(static_cast<int>(i));
    FakeProfile* p = static_cast<FakeProfile*>(h);
    if (p && p->signatures && i >= 0 && static_cast<size_t>(i) < p->tagCount)
        return p->signatures[i];
    return 0;
}

cmsInt32Number cmsReadRawTag(cmsHPROFILE h, cmsTagSignature sig, void* buffer, cmsInt32Number len)
{
    (void)buffer; // not used in tests
    (void)len;
    g_readLog.push_back(sig);
    return 0;
}

// Helper macros to provide non-terminating assertions
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        ++g_totalFailures; \
        std::cerr << "EXPECT_TRUE failed: " << #cond << " in " << __FUNCTION__ \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    if((a) != (b)) { \
        ++g_totalFailures; \
        std::cerr << "EXPECT_EQ failed: " << #a << " == " << #b \
                  << " (" << (a) << " != " << (b) << ")" \
                  << " in " << __FUNCTION__ << " at " << __FILE__ << ":" << __LINE__ \
                  << std::endl; \
    } \
} while(0)

// Convenience to reset per-test logs
static void reset_logs() {
    g_readLog.clear();
    g_signatureIndexLog.clear();
    g_tagCountCalls = 0;
}

// -----------------------------------------------------------------------------
// Test 1: Zero tags
// Behavior: When the profile reports 0 tags, ReadAllRAWTags should perform no reads.
// Expected: cmsGetTagCount called once, no cmsReadRawTag invocations, no signatures read.
// -----------------------------------------------------------------------------
void test_ReadAllRAWTags_zeroTags()
{
    std::cout << "BEGIN TEST: ReadAllRAWTags with zero tags" << std::endl;
    int beforeFailures = g_totalFailures;

    reset_logs();
    g_tagCountCalls = 0;
    g_readLog.clear();
    g_signatureIndexLog.clear();

    // Create a fake profile with zero tags
    FakeProfile* prof = new FakeProfile;
    prof->tagCount = 0;
    prof->signatures = nullptr;

    // Call the focal method
    ReadAllRAWTags((cmsHPROFILE)prof);

    // Assertions
    EXPECT_EQ(g_tagCountCalls, 1);           // Called once to fetch count
    EXPECT_EQ((int)g_readLog.size(), 0);      // No raw reads performed
    EXPECT_EQ((int)g_signatureIndexLog.size(), 0); // No signatures fetched

    delete prof;

    if (g_totalFailures == beforeFailures)
        std::cout << "TEST PASSED: ReadAllRAWTags_zeroTags" << std::endl;
    else
        std::cout << "TEST FAILED: ReadAllRAWTags_zeroTags" << std::endl;
}

// -----------------------------------------------------------------------------
// Test 2: Three tags
// Behavior: Ensure the loop iterates for each tag and that signatures are read in order.
// Expected: cmsGetTagCount called once; cmsGetTagSignature called for i=0..2 in order;
// cmsReadRawTag called three times with the provided signatures.
// -----------------------------------------------------------------------------
void test_ReadAllRAWTags_threeTags()
{
    std::cout << "BEGIN TEST: ReadAllRAWTags with three tags" << std::endl;
    int beforeFailures = g_totalFailures;

    reset_logs();
    g_tagCountCalls = 0;
    g_readLog.clear();
    g_signatureIndexLog.clear();

    // Prepare a profile with 3 tags
    cmsTagSignature* sigs = new cmsTagSignature[3];
    sigs[0] = 0x01020304;
    sigs[1] = 0x05060708;
    sigs[2] = 0x090A0B0C;

    FakeProfile* prof = new FakeProfile;
    prof->tagCount = 3;
    prof->signatures = sigs;

    // Call the focal method
    ReadAllRAWTags((cmsHPROFILE)prof);

    // Assertions
    EXPECT_EQ(g_tagCountCalls, 1);                // Called once to fetch count
    EXPECT_EQ((int)g_readLog.size(), 3);          // Three reads performed
    // The signatures fed to cmsReadRawTag should match [sig0, sig1, sig2]
    EXPECT_EQ(g_readLog[0], sigs[0]);
    EXPECT_EQ(g_readLog[1], sigs[1]);
    EXPECT_EQ(g_readLog[2], sigs[2]);

    // The indices to cmsGetTagSignature should be [0,1,2]
    EXPECT_EQ((int)g_signatureIndexLog.size(), 3);
    EXPECT_EQ(g_signatureIndexLog[0], 0);
    EXPECT_EQ(g_signatureIndexLog[1], 1);
    EXPECT_EQ(g_signatureIndexLog[2], 2);

    delete[] sigs;
    delete prof;

    if (g_totalFailures == beforeFailures)
        std::cout << "TEST PASSED: ReadAllRAWTags_threeTags" << std::endl;
    else
        std::cout << "TEST FAILED: ReadAllRAWTags_threeTags" << std::endl;
}

// -----------------------------------------------------------------------------
// Test 3: Multiple invocations (sanity across calls)
// Behavior: Ensure subsequent calls also properly reset and report their own interactions.
// This validates that the API is stateless across separate calls to the focal method.
// -----------------------------------------------------------------------------
void test_ReadAllRAWTags_multipleInvocations()
{
    std::cout << "BEGIN TEST: ReadAllRAWTags multiple invocations" << std::endl;
    int beforeFailures = g_totalFailures;

    reset_logs();
    g_tagCountCalls = 0;
    g_readLog.clear();
    g_signatureIndexLog.clear();

    // First profile with 2 tags
    cmsTagSignature* sigs1 = new cmsTagSignature[2];
    sigs1[0] = 0xAAAABBBB;
    sigs1[1] = 0xCCCCDDDD;

    FakeProfile* prof1 = new FakeProfile;
    prof1->tagCount = 2;
    prof1->signatures = sigs1;

    ReadAllRAWTags((cmsHPROFILE)prof1);

    // Assertions for first run
    EXPECT_EQ(g_tagCountCalls, 1);
    EXPECT_EQ((int)g_readLog.size(), 2);
    EXPECT_EQ(g_readLog[0], sigs1[0]);
    EXPECT_EQ(g_readLog[1], sigs1[1]);

    delete[] sigs1;
    delete prof1;

    // Second profile with 1 tag
    cmsTagSignature* sigs2 = new cmsTagSignature[1];
    sigs2[0] = 0xDEADBEEF;

    FakeProfile* prof2 = new FakeProfile;
    prof2->tagCount = 1;
    prof2->signatures = sigs2;

    ReadAllRAWTags((cmsHPROFILE)prof2);

    // Assertions for second run
    EXPECT_EQ(g_tagCountCalls, 2);           // Now two separate calls were made
    EXPECT_EQ((int)g_readLog.size(), 3);     // 2 from first run + 1 from second run
    EXPECT_EQ(g_readLog[2], sigs2[0]);

    delete[] sigs2;
    delete prof2;

    if (g_totalFailures == beforeFailures)
        std::cout << "TEST PASSED: ReadAllRAWTags_multipleInvocations" << std::endl;
    else
        std::cout << "TEST FAILED: ReadAllRAWTags_multipleInvocations" << std::endl;
}

// -----------------------------------------------------------------------------
// Main: Run all tests
// -----------------------------------------------------------------------------
int main()
{
    std::cout << "Starting unit tests for ReadAllRAWTags (mocked CMS API)..." << std::endl;

    test_ReadAllRAWTags_zeroTags();
    test_ReadAllRAWTags_threeTags();
    test_ReadAllRAWTags_multipleInvocations();

    std::cout << "Unit test finished. Total failures: " << g_totalFailures << std::endl;
    if (g_totalFailures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}