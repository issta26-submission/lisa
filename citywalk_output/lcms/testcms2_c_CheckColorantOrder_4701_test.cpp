// Lightweight C++11 unit test suite for the focal method: CheckColorantOrder
// This test suite provides a minimal mock of the CMS-like API used by the focal method,
// along with a self-contained test harness (no external testing framework required).

#include <cstring>
#include <vector>
#include <iostream>
#include <map>
#include <testcms2.h>


// Domain constants (mocked for unit testing)
#define cmsMAXCHANNELS 4                 // Domain adaptation: keep small for testability
typedef int cmsInt32Number;
typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsTagSignature;   // 4-byte signature commonly used in CMS
typedef void* cmsHPROFILE;
typedef bool cmsBool;
typedef double cmsFloat64Number;

// Forward declaration of the focal method (as in the provided source)
cmsInt32Number CheckColorantOrder(cmsInt32Number Pass, cmsHPROFILE hProfile, cmsTagSignature tag);

// Minimal fake CMS profile to back tag storage
struct FakeProfile {
    std::map<cmsTagSignature, std::vector<cmsUInt8Number>> tags;
};

// Mocked CMS API (minimal, self-contained)
cmsInt32Number cmsWriteTag(cmsHPROFILE hProfile, cmsTagSignature tag, void* data) {
    if (hProfile == nullptr || data == nullptr) return 0;
    FakeProfile* prof = static_cast<FakeProfile*>(hProfile);
    cmsUInt8Number* src = static_cast<cmsUInt8Number*>(data);

    std::vector<cmsUInt8Number> vec(cmsMAXCHANNELS);
    for (int i = 0; i < cmsMAXCHANNELS; ++i) vec[i] = src[i];
    prof->tags[tag] = vec;
    return 1;
}

void* cmsReadTag(cmsHPROFILE hProfile, cmsTagSignature tag) {
    if (hProfile == nullptr) return NULL;
    FakeProfile* prof = static_cast<FakeProfile*>(hProfile);
    auto it = prof->tags.find(tag);
    if (it == prof->tags.end()) return NULL;
    cmsUInt8Number* out = new cmsUInt8Number[cmsMAXCHANNELS];
    for (int i = 0; i < cmsMAXCHANNELS; ++i) out[i] = it->second[i];
    return static_cast<void*>(out);
}

// Implementation of the focal method (copied from the provided snippet)
cmsInt32Number CheckColorantOrder(cmsInt32Number Pass,  cmsHPROFILE hProfile, cmsTagSignature tag)
{
{
    cmsUInt8Number *Pt, c[cmsMAXCHANNELS];
    cmsInt32Number i;
    switch (Pass) {
        case 1:
            for (i=0; i < cmsMAXCHANNELS; i++) c[i] = (cmsUInt8Number) (cmsMAXCHANNELS - i - 1);
            return cmsWriteTag(hProfile, tag, c);

        case 2:
            Pt = (cmsUInt8Number *) cmsReadTag(hProfile, tag); 
            if (Pt == NULL) return 0;
            for (i=0; i < cmsMAXCHANNELS; i++) {
                if (Pt[i] != ( cmsMAXCHANNELS - i - 1 )) return 0;
            }
            return 1;
        default:
            return 0;
    }
}
}

// Simple test harness (no GTest). Uses non-terminating assertions to maximize coverage.
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define ASSERT_EQ(actual, expected, msg)                                   \
    do {                                                                   \
        if ((actual) != (expected)) {                                    \
            std::cerr << "ASSERT FAILED: " << (msg)                         \
                      << " | expected: " << (expected)                     \
                      << ", actual: " << (actual) << std::endl;          \
            ++g_tests_failed;                                            \
        } else {                                                         \
            ++g_tests_passed;                                            \
        }                                                                \
    } while (0)

#define ASSERT_TRUE(cond, msg)                                              \
    do {                                                                     \
        if (!(cond)) {                                                       \
            std::cerr << "ASSERT FAILED: " << (msg) << std::endl;           \
            ++g_tests_failed;                                              \
        } else {                                                           \
            ++g_tests_passed;                                              \
        }                                                                  \
    } while (0)

// Helper to create a fresh profile
FakeProfile* CreateProfile() {
    return new FakeProfile();
}

// Helper to clean profile
void DestroyProfile(FakeProfile* p) {
    delete p;
}

// Test 1: Ensure that a colorant order can be written (Pass = 1) and then read back (Pass = 2) successfully.
void test_CheckColorantOrder_WriteThenRead_Success() {
    // Description: Write a properly ordered colorant via Pass=1, then read via Pass=2 and expect success.
    FakeProfile* prof = CreateProfile();
    const cmsTagSignature TAG = 0x74616701; // arbitrary tag signature

    cmsInt32Number res1 = CheckColorantOrder(1, static_cast<cmsHPROFILE>(prof), TAG);
    ASSERT_EQ(res1, 1, "Pass=1 should write the tag successfully");

    cmsInt32Number res2 = CheckColorantOrder(2, static_cast<cmsHPROFILE>(prof), TAG);
    ASSERT_EQ(res2, 1, "Pass=2 should read back correct colorant order");

    DestroyProfile(prof);
}

// Test 2: Ensure that reading a non-existent tag returns failure (Pt == NULL path).
void test_CheckColorantOrder_ReadMissingTag_Returns0() {
    FakeProfile* prof = CreateProfile();
    const cmsTagSignature TAG_WRITTEN = 0x74616702;
    const cmsTagSignature TAG_MISSING = 0x74616703;

    // Write one tag first to ensure the profile is functional
    cmsInt32Number resWrite = CheckColorantOrder(1, static_cast<cmsHPROFILE>(prof), TAG_WRITTEN);
    ASSERT_EQ(resWrite, 1, "Write should succeed for TAG_WRITTEN");

    // Now try to read a different, non-existent tag
    cmsInt32Number resReadMissing = CheckColorantOrder(2, static_cast<cmsHPROFILE>(prof), TAG_MISSING);
    ASSERT_EQ(resReadMissing, 0, "Read of missing tag should fail (return 0)");

    DestroyProfile(prof);
}

// Test 3: Ensure that invalid Pass values yield 0 (default branch).
void test_CheckColorantOrder_InvalidPass_Returns0() {
    FakeProfile* prof = CreateProfile();
    const cmsTagSignature TAG = 0x74616704;

    // Attempt with an invalid Pass value
    cmsInt32Number res = CheckColorantOrder(3, static_cast<cmsHPROFILE>(prof), TAG);
    ASSERT_EQ(res, 0, "Invalid Pass value should return 0");

    DestroyProfile(prof);
}

// Test 4: Ensure that multiple independent profiles behave independently.
void test_CheckColorantOrder_IndependentProfiles() {
    // Profile A: write and read success path
    FakeProfile* a = CreateProfile();
    const cmsTagSignature TAG_A = 0x74616705;
    cmsInt32Number wa = CheckColorantOrder(1, static_cast<cmsHPROFILE>(a), TAG_A);
    cmsInt32Number ra = CheckColorantOrder(2, static_cast<cmsHPROFILE>(a), TAG_A);
    ASSERT_EQ(wa, 1, "Profile A: write should succeed");
    ASSERT_EQ(ra, 1, "Profile A: read should succeed");

    // Profile B: ensure reading before writing returns 0
    FakeProfile* b = CreateProfile();
    const cmsTagSignature TAG_B = 0x74616706;
    cmsInt32Number rb_before = CheckColorantOrder(2, static_cast<cmsHPROFILE>(b), TAG_B);
    ASSERT_EQ(rb_before, 0, "Profile B: read before write should fail");

    DestroyProfile(a);
    DestroyProfile(b);
}

// Entry point
int main() {
    std::cout << "Running CheckColorantOrder unit tests (mock CMS)..." << std::endl;

    test_CheckColorantOrder_WriteThenRead_Success();
    test_CheckColorantOrder_ReadMissingTag_Returns0();
    test_CheckColorantOrder_InvalidPass_Returns0();
    test_CheckColorantOrder_IndependentProfiles();

    int total = g_tests_passed + g_tests_failed;
    std::cout << "Tests run: " << total
              << " | Passed: " << g_tests_passed
              << " | Failed: " << g_tests_failed << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}