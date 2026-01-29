// Lightweight C++11 test harness for the CHAD tag checker (CheckCHAD).
// This test reproduces a minimal environment to exercise the focal method
// CheckCHAD as provided in the source snippet, without using GTest.
// It uses simple inline stubs for the CMS API (cmsWriteTag/cmsReadTag/IsGoodFixed15_16)
// to enable unit testing in a standalone manner.

#include <array>
#include <unordered_map>
#include <iostream>
#include <testcms2.h>
#include <cassert>
#include <cmath>


// Domain/Compatibility typedefs (simplified for tests)
using cmsInt32Number  = int;
using cmsUInt32Number = unsigned int;
using cmsFloat64Number = double;
using cmsBool = bool;
using cmsHPROFILE = void*;
using cmsTagSignature = unsigned int;

// Lightweight fake CMS storage to emulate tag writes/reads per profile
struct FakeProfile {
    std::unordered_map<cmsTagSignature, std::array<cmsFloat64Number, 9>> tags;
};

// Global storage: profile pointer -> FakeProfile
static std::unordered_map<void*, FakeProfile> g_profiles;

// Helper: create a new fake profile handle
static void* CreateFakeProfile() {
    void* h = new int(0); // opaque handle
    g_profiles[h] = FakeProfile();
    return h;
}

// Helper: access internal data for verification
static const std::array<cmsFloat64Number, 9>& GetTagData(void* hProfile, cmsTagSignature tag) {
    return g_profiles[hProfile].tags[tag];
}

// cmsWriteTag stub: store a 9-element double array as the tag value
extern "C" cmsInt32Number cmsWriteTag(void* hProfile, cmsTagSignature tag, cmsFloat64Number* data) {
    if (!hProfile) return 0;
    auto &pf = g_profiles[hProfile];
    std::array<cmsFloat64Number, 9> arr;
    for (int i = 0; i < 9; ++i) arr[i] = data[i];
    pf.tags[tag] = arr;
    return 1;
}

// cmsReadTag stub: return a newly allocated copy of the tag data, or NULL if missing
extern "C" cmsFloat64Number* cmsReadTag(void* hProfile, cmsTagSignature tag) {
    if (!hProfile) return nullptr;
    auto itProf = g_profiles.find(hProfile);
    if (itProf == g_profiles.end()) return nullptr;
    auto itTag = itProf->second.tags.find(tag);
    if (itTag == itProf->second.tags.end()) return nullptr;

    cmsFloat64Number* p = new cmsFloat64Number[9];
    for (int i = 0; i < 9; ++i) p[i] = (itTag->second)[i];
    return p;
}

// IsGoodFixed15_16: compare two fixed-point values with tolerance
extern "C" cmsBool IsGoodFixed15_16(const char* /*title*/, cmsFloat64Number in, cmsFloat64Number out) {
    const cmsFloat64Number eps = 1e-9;
    return (std::fabs(in - out) <= eps);
}

// Focal method under test: CheckCHAD
cmsInt32Number CheckCHAD(cmsInt32Number Pass,  cmsHPROFILE hProfile, cmsTagSignature tag)
{
{
    cmsFloat64Number *Pt;
    cmsFloat64Number CHAD[] = { 0, .1, .2, .3, .4, .5, .6, .7, .8 };
    cmsInt32Number i;
    switch (Pass) {
        case 1:
            return cmsWriteTag(hProfile, tag, CHAD);

        case 2:
            Pt = (cmsFloat64Number *) cmsReadTag(hProfile, tag); 
            if (Pt == NULL) return 0;
            for (i=0; i < 9; i++) {
                if (!IsGoodFixed15_16("CHAD", Pt[i], CHAD[i])) return 0;
            }
            return 1;
        default:
            return 0;
    }
}
}

// Helper: small test harness utilities
static int g_failures = 0;
#define CHECK(cond, msg) do { if (!(cond)) { \
    std::cerr << "[FAIL] " << (msg) << std::endl; ++g_failures; } \
} while(0)

static void test_description(const char* d) {
    std::cout << "TEST: " << d << std::endl;
}

// Corrupt internal tag data to simulate mismatch
static void CorruptTagData(void* hProfile, cmsTagSignature tag) {
    auto itProf = g_profiles.find(hProfile);
    if (itProf == g_profiles.end()) return;
    auto itTag = itProf->second.tags.find(tag);
    if (itTag == itProf->second.tags.end()) return;
    // Set all values to zero to easily trigger mismatch against CHAD[]
    for (int i = 0; i < 9; ++i) (itTag->second)[i] = 0.0;
}

// Test 1: Ensure Write path stores the CHAD array correctly
static bool test_CheckCHAD_WriteTag_WritesCorrectly() {
    test_description("CheckCHAD with Pass=1 should write CHAD array to the profile-tag.");
    void* ph = CreateFakeProfile();
    cmsTagSignature TAG = 0xABCDEF01;

    cmsInt32Number res = CheckCHAD(1, ph, TAG);
    CHECK(res == 1, "CheckCHAD(1, ...) should return 1 (successful write).");

    // Verify internal storage contains the correct CHAD values
    const std::array<cmsFloat64Number, 9> CHAD_EXPECT = { 0, .1, .2, .3, .4, .5, .6, .7, .8 };
    auto itDeep = g_profiles.find(ph);
    CHECK(itDeep != g_profiles.end(), "Profile should exist after write.");
    if (itDeep != g_profiles.end()) {
        auto itTag = itDeep->second.tags.find(TAG);
        CHECK(itTag != itDeep->second.tags.end(), "Tag should exist after write.");
        if (itTag != itDeep->second.tags.end()) {
            for (int i=0; i<9; ++i) {
                cmsFloat64Number got = (itTag->second)[i];
                cmsFloat64Number exp = CHAD_EXPECT[i];
                bool eq = std::fabs(got - exp) <= 1e-9;
                CHECK(eq, "Written CHAD value should match expected.");
            }
        }
    }
    // Cleanup
    delete ph;
    return g_failures == 0;
}

// Test 2: Ensure Read path returns success when data matches CHAD[]
static bool test_CheckCHAD_ReadTag_MatchesCHAD() {
    test_description("CheckCHAD with Pass=2 should read back the stored CHAD data and compare.");
    void* ph = CreateFakeProfile();
    cmsTagSignature TAG = 0x12345678;

    // First write the CHAD array
    cmsInt32Number wres = CheckCHAD(1, ph, TAG);
    CHECK(wres == 1, "Precondition: write should succeed.");

    // Now read and compare
    cmsInt32Number pres = CheckCHAD(2, ph, TAG);
    CHECK(pres == 1, "CheckCHAD(2, ...) should return 1 when data matches CHAD[].");

    delete ph;
    return g_failures == 0;
}

// Test 3: Ensure Read path returns 0 when no data exists (Pt == NULL)
static bool test_CheckCHAD_ReadTag_NullPtr() {
    test_description("CheckCHAD with Pass=2 should return 0 when no tag data exists (Pt==NULL).");
    void* ph = CreateFakeProfile();
    cmsTagSignature TAG = 0xDEADBEEF;

    cmsInt32Number pres = CheckCHAD(2, ph, TAG);
    CHECK(pres == 0, "CheckCHAD(2, ...) should return 0 if there is no tag data.");

    delete ph;
    return g_failures == 0;
}

// Test 4: Ensure Read path detects mismatch and returns 0
static bool test_CheckCHAD_ReadTag_MismatchDetected() {
    test_description("CheckCHAD should return 0 when CHAD data read does not match expected values.");
    void* ph = CreateFakeProfile();
    cmsTagSignature TAG = 0xFEDCBA98;

    // Write correct CHAD data
    cmsInt32Number wres = CheckCHAD(1, ph, TAG);
    CHECK(wres == 1, "Precondition: write should succeed.");

    // Corrupt internal data for mismatch
    CorruptTagData(ph, TAG);

    cmsInt32Number pres = CheckCHAD(2, ph, TAG);
    CHECK(pres == 0, "CheckCHAD(2, ...) should return 0 on mismatch.");

    delete ph;
    return g_failures == 0;
}

// Test 5: Ensure default branch returns 0 for unsupported Pass values
static bool test_CheckCHAD_DefaultCaseReturns0() {
    test_description("CheckCHAD with an unsupported Pass value (default case) should return 0.");
    void* ph = CreateFakeProfile();
    cmsTagSignature TAG = 0x0F0F0F0F;

    cmsInt32Number pres = CheckCHAD(3, ph, TAG); // default branch
    CHECK(pres == 0, "CheckCHAD with default/unsupported Pass must return 0.");

    delete ph;
    return g_failures == 0;
}

// Main: run all tests
int main() {
    bool all_ok = true;
    all_ok &= test_CheckCHAD_WriteTag_WritesCorrectly();
    all_ok &= test_CheckCHAD_ReadTag_MatchesCHAD();
    all_ok &= test_CheckCHAD_ReadTag_NullPtr();
    all_ok &= test_CheckCHAD_ReadTag_MismatchDetected();
    all_ok &= test_CheckCHAD_DefaultCaseReturns0();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}