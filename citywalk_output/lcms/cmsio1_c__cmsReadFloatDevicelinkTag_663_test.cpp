// Test suite for the focal method: _cmsReadFloatDevicelinkTag
// Approach: Provide a self-contained mock environment that mimics just enough of the Little CMS
// types and functions to exercise the logic inside _cmsReadFloatDevicelinkTag without requiring
// the full library. This enables deterministic unit tests in a single translation unit using C++11.

// NOTE: This is a pure-in-C++11 in-memory test harness tailored to the focal function.
// It mocks the necessary dependencies (profiles, pipelines, stages) and exercises
// true/false branches as requested. It is not using GTest.

#include <lcms2_internal.h>
#include <iostream>
#include <string>


// -----------------------------
// Minimal type and constant mock
// -----------------------------

// Mocked opaque types to resemble cms types in a lightweight way
typedef void* cmsHPROFILE;
typedef unsigned int cmsTagSignature;
typedef unsigned int cmsContext;
typedef unsigned int cmsColorSpaceSignature;

// Faux public signatures used by the focal function
#define cmsAT_BEGIN 0
#define cmsAT_END   1

// Mock color space signatures (values chosen for testing)
#define cmsSigLabData 1
#define cmsSigXYZData 2

// Forward declare the focal function to test (our test harness provides the implementation)
extern "C" {

    // Prototype matching the focal function signature
    struct cmsPipeline; // forward declare (we provide our own definition below)
    typedef struct cmsPipeline cmsPipeline;

    cmsPipeline* _cmsReadFloatDevicelinkTag(cmsHPROFILE hProfile, cmsTagSignature tagFloat);
}

// -----------------------------
// Mock profile and pipeline implementation
// -----------------------------

// Lightweight mock pipeline that keeps track of inserted stages
struct cmsPipeline {
    int beginInserted;
    int endInserted;
    cmsPipeline() : beginInserted(0), endInserted(0) {}
};

// Mock profile structure
struct MockProfile {
    cmsContext contextId;
    cmsColorSpaceSignature spc; // color space of the profile (Lab or XYZ)
    cmsColorSpaceSignature PCS; // profile connection space (Lab or XYZ)
    bool hasTag; // whether the requested tag exists
    cmsPipeline* tagValue; // the pipeline provided by cmsReadTag when hasTag==true
};

// Helper to cast profile handle to our MockProfile
static MockProfile* PROFILE_CAST(cmsHPROFILE h) { return reinterpret_cast<MockProfile*>(h); }

// -----------------------------
// Mocks for dependency functions used by the focal method
// ----------------------------

// Get the profile context
static cmsContext cmsGetProfileContextID(cmsHPROFILE hProfile) {
    if (!hProfile) return 0;
    return PROFILE_CAST(hProfile)->contextId;
}

// Duplicate a pipeline (simple shallow/deep copy for the test's needs)
static cmsPipeline* cmsPipelineDup(const cmsPipeline* p) {
    if (!p) return nullptr;
    cmsPipeline* q = new cmsPipeline();
    q->beginInserted = p->beginInserted;
    q->endInserted = p->endInserted;
    return q;
}

// Read the profile's color space (Lab/XYZ)
static cmsColorSpaceSignature cmsGetColorSpace(cmsHPROFILE hProfile) {
    if (!hProfile) return cmsSigLabData;
    return PROFILE_CAST(hProfile)->spc;
}

// Read the profile's PCS (Lab/XYZ)
static cmsColorSpaceSignature cmsGetPCS(cmsHPROFILE hProfile) {
    if (!hProfile) return cmsSigLabData;
    return PROFILE_CAST(hProfile)->PCS;
}

// Read a tag from the profile; in this mock, we only care about whether the tag exists
static cmsPipeline* cmsReadTag(cmsHPROFILE hProfile, cmsTagSignature tag) {
    (void)tag; // tag value is ignored in this mock; the presence is determined by the profile
    if (!hProfile) return nullptr;
    MockProfile* p = PROFILE_CAST(hProfile);
    return p->hasTag ? p->tagValue : nullptr;
}

// Insert a stage into the pipeline; track insertion counts. Optional failure simulation.
static int gForceFailure = 0; // when non-zero, simulate an insertion failure
static int cmsPipelineInsertStage(cmsPipeline* Lut, int stage, void* /*stagePtr*/) {
    if (!Lut) return 0;
    if (gForceFailure) return 0;
    if (stage == cmsAT_BEGIN) {
        Lut->beginInserted++;
    } else if (stage == cmsAT_END) {
        Lut->endInserted++;
    }
    return 1; // success
}

// Normalize stage constructors (return dummy non-null pointers)
static void* _cmsStageNormalizeToLabFloat(cmsContext)
{ return (void*)0x01; }

static void* _cmsStageNormalizeToXyzFloat(cmsContext)
{ return (void*)0x02; }

static void* _cmsStageNormalizeFromLabFloat(cmsContext)
{ return (void*)0x03; }

static void* _cmsStageNormalizeFromXyzFloat(cmsContext)
{ return (void*)0x04; }

// Free a pipeline (simple delete)
static void cmsPipelineFree(cmsPipeline* p) {
    delete p;
}

// -----------------------------
// The focal function implementation (copied/adapted for the test harness)
// -----------------------------

// Note: The function signature uses C linkage in the original code; we provide a compatible
// implementation here with the test harness' mocks.
extern "C" cmsPipeline* _cmsReadFloatDevicelinkTag(cmsHPROFILE hProfile, cmsTagSignature tagFloat)
{
    {
        cmsContext ContextID = cmsGetProfileContextID(hProfile);
        cmsPipeline* Lut = cmsPipelineDup((cmsPipeline*)cmsReadTag(hProfile, tagFloat));
        cmsColorSpaceSignature PCS = cmsGetPCS(hProfile);
        cmsColorSpaceSignature spc = cmsGetColorSpace(hProfile);
        if (Lut == NULL) return NULL;
        if (spc == cmsSigLabData)
        {
            if (!cmsPipelineInsertStage(Lut, cmsAT_BEGIN, _cmsStageNormalizeToLabFloat(ContextID)))
                goto Error;
        }
        else
            if (spc == cmsSigXYZData)
            {
                if (!cmsPipelineInsertStage(Lut, cmsAT_BEGIN, _cmsStageNormalizeToXyzFloat(ContextID)))
                    goto Error;
            }
        if (PCS == cmsSigLabData)
        {
            if (!cmsPipelineInsertStage(Lut, cmsAT_END, _cmsStageNormalizeFromLabFloat(ContextID)))
                goto Error;
        }
        else
            if (PCS == cmsSigXYZData)
            {
                if (!cmsPipelineInsertStage(Lut, cmsAT_END, _cmsStageNormalizeFromXyzFloat(ContextID)))
                    goto Error;
            }
        return Lut;
    Error:
        cmsPipelineFree(Lut);
        return NULL;
    }
}

// -----------------------------
// Test harness
// -----------------------------

// Simple test harness structure
static int gTotalTests = 0;
static int gFailures = 0;

// Helpers for tests
#define EXPECT_TRUE(cond, msg) do { \
    ++gTotalTests; \
    if (cond) { \
        std::cout << "[PASS] " << msg << "\n"; \
    } else { \
        ++gFailures; \
        std::cout << "[FAIL] " << msg << "\n"; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    ++gTotalTests; \
    if ((a) == (b)) { \
        std::cout << "[PASS] " << msg << "\n"; \
    } else { \
        ++gFailures; \
        std::cout << "[FAIL] " << msg << " (expected: " << (b) << ", got: " << (a) << ")\n"; \
    } \
} while(0)

// Helpers to create a mock profile
static cmsHPROFILE makeProfile(cmsContext ctx, cmsColorSpaceSignature spc, cmsColorSpaceSignature pcs, bool hasTag, cmsPipeline* tagValue) {
    MockProfile* p = new MockProfile();
    p->contextId = ctx;
    p->spc = spc;
    p->PCS = pcs;
    p->hasTag = hasTag;
    p->tagValue = tagValue;
    return reinterpret_cast<cmsHPROFILE>(p);
}

// Helper to clean up a profile
static void destroyProfile(cmsHPROFILE h) {
    delete PROFILE_CAST(h);
}

// Tests

// Test 1: When the tag is not present, Lut becomes NULL and function returns NULL
static void test_LutIsNullWhenTagMissing() {
    // Profile Lab space, tag missing
    cmsPipeline* dummy = nullptr;
    MockProfile* mp = new MockProfile();
    mp->contextId = 7;
    mp->spc = cmsSigLabData;
    mp->PCS = cmsSigLabData;
    mp->hasTag = false;
    mp->tagValue = nullptr;
    cmsHPROFILE prof = reinterpret_cast<cmsHPROFILE>(mp);

    // Call focal function
    cmsPipeline* result = _cmsReadFloatDevicelinkTag(prof, 0x00000000);

    // Expect NULL
    EXPECT_TRUE(result == nullptr, "test_LutIsNullWhenTagMissing: returned NULL when tag absent");

    // Cleanup
    destroyProfile(prof);
    if (result) cmsPipelineFree(result); // should be NULL, but guard anyway
}

// Test 2: Lab spc and Lab PCS with tag present -> both BEGIN and END stages inserted
static void test_LabLab_BegEndInserted() {
    // Create an initial tag pipeline with default counts 0
    cmsPipeline* initial = new cmsPipeline();
    initial->beginInserted = 0;
    initial->endInserted = 0;

    MockProfile* mp = new MockProfile();
    mp->contextId = 11;
    mp->spc = cmsSigLabData; // Lab space
    mp->PCS = cmsSigLabData; // Lab PCS
    mp->hasTag = true;
    mp->tagValue = initial;

    cmsHPROFILE prof = reinterpret_cast<cmsHPROFILE>(mp);

    cmsPipeline* result = _cmsReadFloatDevicelinkTag(prof, 0x1234);

    // Expect a non-null Lut and 1 begin, 1 end insertion
    bool ok = (result != nullptr) && (result->beginInserted == 1) && (result->endInserted == 1);

    EXPECT_TRUE(ok, "test_LabLab_BegEndInserted: Lab spc and Lab PCS with tag present inserts BEGIN and END");

    // Cleanup
    destroyProfile(prof);
    if (result) cmsPipelineFree(result);
}

// Test 3: XYZ spc and XYZ PCS with tag present -> both BEGIN and END stages inserted
static void test_XyzXyz_BegEndInserted() {
    cmsPipeline* initial = new cmsPipeline();
    initial->beginInserted = 0;
    initial->endInserted = 0;

    MockProfile mp;
    mp.contextId = 21;
    mp.spc = cmsSigXYZData;
    mp.PCS = cmsSigXYZData;
    mp.hasTag = true;
    mp.tagValue = initial;

    cmsHPROFILE prof = reinterpret_cast<cmsHPROFILE>(&mp);

    cmsPipeline* result = _cmsReadFloatDevicelinkTag(prof, 0xABCD);

    bool ok = (result != nullptr) && (result->beginInserted == 1) && (result->endInserted == 1);

    EXPECT_TRUE(ok, "test_XyzXyz_BegEndInserted: XYZ spc and XYZ PCS with tag present inserts BEGIN and END");

    // Cleanup
    destroyProfile(prof);
    if (result) cmsPipelineFree(result);
}

// Test 4: Error path: simulate an insertion failure to trigger goto Error and NULL result
static void test_InsertionFailureReturnsNull() {
    // Prepare profile with Lab Lab and a tag
    cmsPipeline* initial = new cmsPipeline();
    initial->beginInserted = 0;
    initial->endInserted = 0;

    MockProfile* mp = new MockProfile();
    mp->contextId = 99;
    mp->spc = cmsSigLabData;
    mp->PCS = cmsSigLabData;
    mp->hasTag = true;
    mp->tagValue = initial;

    cmsHPROFILE prof = reinterpret_cast<cmsHPROFILE>(mp);

    // Force the insertion to fail
    gForceFailure = 1;

    cmsPipeline* result = _cmsReadFloatDevicelinkTag(prof, 0xDEAD);

    // Expect NULL due to simulated failure
    EXPECT_TRUE(result == nullptr, "test_InsertionFailureReturnsNull: forced insertion failure yields NULL");

    // Reset failure flag for other tests
    gForceFailure = 0;

    // Cleanup
    destroyProfile(prof);
    if (result) cmsPipelineFree(result);
}

// -----------------------------
// Main
// -----------------------------

int main() {
    std::cout << "Starting _cmsReadFloatDevicelinkTag unit tests (mocked environment)\n";

    test_LutIsNullWhenTagMissing();
    test_LabLab_BegEndInserted();
    test_XyzXyz_BegEndInserted();
    test_InsertionFailureReturnsNull();

    std::cout << "Tests completed. Total: " << gTotalTests << ", Failures: " << gFailures << "\n";

    // Return non-zero if any test failed
    return gFailures != 0 ? 1 : 0;
}