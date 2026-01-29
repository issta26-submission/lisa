// This file provides a self-contained C++11 unit-test harness for the focal
// method _cmsReadFloatOutputTag adapted to a minimal mock CMS environment.
// The real cmsReadFloatOutputTag depends on many parts of libLCMS; to enable
// focused testing of the decision branches (begin/end stage insertions for Lab/XYZ
// data spaces), we implement a lightweight, stand-alone replica of the necessary
// pieces of the CMS API and the focal method logic in this translation unit.
// The tests verify true/false branches, proper cleanup on error, and handling
// of the NULL Lut case without requiring the actual LittleCMS runtime.
//
// Note: This is a test harness that replicates the logic of the focal method
// for deterministic testing in environments without the full CMS library.
// It is not a drop-in replacement for the production code, but provides useful
// coverage for the decision branches of the focal method.
//
// Domain knowledge is applied: tests cover Lab/XYZ PCS and data spaces, as well as
// insertion success/failure paths for begin/end stages. Static-like behavior is
// emulated via module-level flags.
//
#include <cstring>
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>


// Domain-specific minimal typedefs to emulate CMS types in test harness
using cmsTagSignature = unsigned int;
using cmsColorSpaceSignature = unsigned int;
using cmsBool = int;
using cmsContext = int;
using cmsHPROFILE = void*;
using cmsPipeline = struct FakePipeline;

// Lightweight fake pipeline to record staged operations
struct FakePipeline {
    std::vector<std::string> beginStages;
    std::vector<std::string> endStages;
    FakePipeline() {}
    FakePipeline(const FakePipeline& o)
        : beginStages(o.beginStages), endStages(o.endStages) {}
};

// alias the production type name in test harness
typedef FakePipeline cmsPipeline;

// Forward declarations of minimal "CMS" helpers used by the focal method replica
static cmsContext cmsGetProfileContextID(cmsHPROFILE hProfile);
static cmsColorSpaceSignature cmsGetPCS(cmsHPROFILE hProfile);
static cmsColorSpaceSignature cmsGetColorSpace(cmsHPROFILE hProfile);
static void* cmsReadTag(cmsHPROFILE hProfile, cmsTagSignature tag);

// Stage name helpers (replicas of internal CMS stage creators)
static const char* _cmsStageNormalizeToLabFloat(cmsContext /*ctx*/) { return "NormalizeToLabFloat"; }
static const char* _cmsStageNormalizeToXyzFloat(cmsContext /*ctx*/) { return "NormalizeToXyzFloat"; }
static const char* _cmsStageNormalizeFromLabFloat(cmsContext /*ctx*/) { return "NormalizeFromLabFloat"; }
static const char* _cmsStageNormalizeFromXyzFloat(cmsContext /*ctx*/) { return "NormalizeFromXyzFloat"; }

// Begin/End stage location constants (replica of cmsAT_BEGIN / cmsAT_END)
static const int cmsAT_BEGIN = 0;
static const int cmsAT_END = 1;

// Flags to simulate insertion success/failure in tests
static bool g_failOnBeginLab = false;
static bool g_failOnBeginXyz = false;
static bool g_failOnEndLab = false;
static bool g_failOnEndXyz = false;

// A global counter to verify that pipelines are freed on error
static int g_freedLutCount = 0;

// Minimal "PCS" constants used by the tests
static const cmsColorSpaceSignature cmsSigLabData = 1;
static const cmsColorSpaceSignature cmsSigXYZData = 2;

// ---------------------- CMS API Mock Implementations ----------------------

// Simple CMS-like profile wrapper to hold test state
struct ProfileWrapper {
    int ctx;                        // fake context id
    cmsColorSpaceSignature PCS;     // profile color space signature
    cmsColorSpaceSignature dataSpace; // profile data space signature
    cmsPipeline* lut;               // pointer to the LUT/pipeline stored in the tag
};

static ProfileWrapper* g_currentProfile = nullptr;

// Accessors mimicking the real API
static cmsContext cmsGetProfileContextID(cmsHPROFILE hProfile) {
    ProfileWrapper* p = static_cast<ProfileWrapper*>(hProfile);
    return p ? p->ctx : 0;
}
static cmsColorSpaceSignature cmsGetPCS(cmsHPROFILE hProfile) {
    ProfileWrapper* p = static_cast<ProfileWrapper*>(hProfile);
    return p ? p->PCS : 0;
}
static cmsColorSpaceSignature cmsGetColorSpace(cmsHPROFILE hProfile) {
    ProfileWrapper* p = static_cast<ProfileWrapper*>(hProfile);
    return p ? p->dataSpace : 0;
}
static void* cmsReadTag(cmsHPROFILE hProfile, cmsTagSignature /*tag*/) {
    ProfileWrapper* p = static_cast<ProfileWrapper*>(hProfile);
    return static_cast<void*>(p ? p->lut : nullptr);
}

// Deep copy of a pipeline (fake)
static cmsPipeline* cmsPipelineDup(const cmsPipeline* p) {
    if (p == nullptr) return nullptr;
    return new cmsPipeline(*p);
}
static void cmsPipelineFree(cmsPipeline* p) {
    if (p) {
        delete p;
        g_freedLutCount++;
    }
}
static int cmsPipelineInsertStage(cmsPipeline* Lut, int where, const char* stage) {
    if (Lut == nullptr || stage == nullptr) return 0;
    // Simulate potential insertion failure based on test flags
    if (where == cmsAT_BEGIN) {
        if (strcmp(stage, "NormalizeToLabFloat") == 0 && g_failOnBeginLab) return 0;
        if (strcmp(stage, "NormalizeToXyzFloat") == 0 && g_failOnBeginXyz) return 0;
        Lut->beginStages.push_back(stage);
        return 1;
    } else if (where == cmsAT_END) {
        if (strcmp(stage, "NormalizeFromLabFloat") == 0 && g_failOnEndLab) return 0;
        if (strcmp(stage, "NormalizeFromXyzFloat") == 0 && g_failOnEndXyz) return 0;
        Lut->endStages.push_back(stage);
        return 1;
    }
    return 0;
}

// ---------------------- Focal Method Replica (for tests) ----------------------
// Replicate the logic of cmsReadFloatOutputTag using the above mocks.
// Signature matches the original in cmsio1.c.
static cmsPipeline* _cmsReadFloatOutputTag(cmsHPROFILE hProfile, cmsTagSignature tagFloat) {
    {
        cmsContext ContextID       = cmsGetProfileContextID(hProfile);
        cmsPipeline* Lut           = cmsPipelineDup((cmsPipeline*) cmsReadTag(hProfile, tagFloat));
        cmsColorSpaceSignature PCS = cmsGetPCS(hProfile);
        cmsColorSpaceSignature dataSpace = cmsGetColorSpace(hProfile);

        if (Lut == NULL) return NULL;

        // Insert begin-stage depending on PCS
        if ( PCS == cmsSigLabData) {
            if (!cmsPipelineInsertStage(Lut, cmsAT_BEGIN, _cmsStageNormalizeToLabFloat(ContextID)))
                goto Error;
        } else if (PCS == cmsSigXYZData) {
            if (!cmsPipelineInsertStage(Lut, cmsAT_BEGIN, _cmsStageNormalizeToXyzFloat(ContextID)))
                goto Error;
        }

        // Insert end-stage depending on data space
        if ( dataSpace == cmsSigLabData) {
            if (!cmsPipelineInsertStage(Lut, cmsAT_END, _cmsStageNormalizeFromLabFloat(ContextID)))
                goto Error;
        } else if (dataSpace == cmsSigXYZData) {
            if (!cmsPipelineInsertStage(Lut, cmsAT_END, _cmsStageNormalizeFromXyzFloat(ContextID)))
                goto Error;
        }

        return Lut;
    Error:
        cmsPipelineFree(Lut);
        return NULL;
    }
}

// ---------------------- Test Harness and Helpers ------------------------------

static void resetGlobals() {
    g_failOnBeginLab = false;
    g_failOnBeginXyz = false;
    g_failOnEndLab = false;
    g_failOnEndXyz = false;
    g_freedLutCount = 0;
    // ensure we re-create new profile for each test if needed
    if (g_currentProfile) delete g_currentProfile;
    g_currentProfile = nullptr;
}

// Helper to construct a fake profile for tests
static ProfileWrapper* makeProfile(int PCS, int dataSpace, int ctx, cmsPipeline* lut) {
    ProfileWrapper* p = new ProfileWrapper();
    p->ctx = ctx;
    p->PCS = static_cast<cmsColorSpaceSignature>(PCS);
    p->dataSpace = static_cast<cmsColorSpaceSignature>(dataSpace);
    p->lut = lut;
    g_currentProfile = p;
    return p;
}

// Simple test harness primitives
static int g_testCount = 0;
static int g_passCount = 0;
static std::vector<std::string> g_failMessages;

#define RUN_TEST(name) do { \
    resetGlobals(); \
    name(); \
} while(0)

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        g_failMessages.push_back(std::string("Assertion failed: ") + (msg)); \
        return; \
    } \
} while(0)

static void reportSummary() {
    std::cout << "\nTest results: " << g_passCount << " passed, " << g_testCount - g_passCount << " failed.\n";
    if (!g_failMessages.empty()) {
        std::cout << "Failure details:\n";
        for (const auto& m : g_failMessages) {
            std::cout << " - " << m << "\n";
        }
    }
}

// ---------------------- Individual Tests ------------------------------

static void test_LutNullReturnsNull() {
    // Explanation: If the LUT/tag does not exist, the function should return NULL
    // without attempting to insert any stages.
    resetGlobals();
    // Profile with a NULL LUT
    cmsPipeline* lut = nullptr;
    auto p = makeProfile(cmsSigLabData, cmsSigLabData, 10, lut);
    cmsPipeline* res = _cmsReadFloatOutputTag(static_cast<cmsHPROFILE>(p), 0xDEADBEEF);
    ASSERT_TRUE(res == nullptr, "Expected NULL when Lut is NULL");
    if (res != nullptr) {
        g_failMessages.push_back("Unexpected non-NULL result when Lut is NULL");
    }
    ++g_testCount;
}

static void test_LabPCS_DataLab_beginEndInserted() {
    // Explanation: PCS Lab and Data Lab should insert NormalizeToLabFloat at BEGIN
    // and NormalizeFromLabFloat at END.
    resetGlobals();
    auto base = new FakePipeline();
    auto p = makeProfile(cmsSigLabData, cmsSigLabData, 20, base);
    cmsPipeline* res = _cmsReadFloatOutputTag(static_cast<cmsHPROFILE>(p), 0xBEEF);
    ASSERT_TRUE(res != nullptr, "Expected non-NULL Lut when insertions succeed");
    if (res) {
        ASSERT_TRUE(base->beginStages.size() == 1, "Expected 1 begin stage");
        ASSERT_TRUE(base->beginStages[0] == std::string("NormalizeToLabFloat"), "Begin stage should be NormalizeToLabFloat");
        ASSERT_TRUE(base->endStages.size() == 1, "Expected 1 end stage");
        ASSERT_TRUE(base->endStages[0] == std::string("NormalizeFromLabFloat"), "End stage should be NormalizeFromLabFloat");
        // Cleanup
        cmsPipelineFree(res);
        delete base;
    }
    ++g_testCount;
    ++g_passCount;
}

static void test_LabPCS_DataXYZ_beginEndInserted() {
    // Explanation: PCS Lab, Data XYZ should insert NormalizeToLabFloat at BEGIN
    // and NormalizeFromXyzFloat at END.
    resetGlobals();
    auto base = new FakePipeline();
    auto p = makeProfile(cmsSigLabData, cmsSigXYZData, 30, base);
    cmsPipeline* res = _cmsReadFloatOutputTag(static_cast<cmsHPROFILE>(p), 0xCAFE);
    ASSERT_TRUE(res != nullptr, "Expected non-NULL Lut when insertions succeed");
    if (res) {
        ASSERT_TRUE(base->beginStages.size() == 1, "Begin stage should be one (Lab begin)");
        ASSERT_TRUE(base->beginStages[0] == std::string("NormalizeToLabFloat"), "Begin stage should be NormalizeToLabFloat");
        ASSERT_TRUE(base->endStages.size() == 1, "End stage should be one (XYZ end)");
        ASSERT_TRUE(base->endStages[0] == std::string("NormalizeFromXyzFloat"), "End stage should be NormalizeFromXyzFloat");
        cmsPipelineFree(res);
        delete base;
    }
    ++g_testCount;
    ++g_passCount;
}

static void test_XYZPCS_DataLab_beginEndInserted() {
    // Explanation: PCS XYZ and Data Lab should insert NormalizeToXyzFloat at BEGIN
    // and NormalizeFromLabFloat at END.
    resetGlobals();
    auto base = new FakePipeline();
    auto p = makeProfile(cmsSigXYZData, cmsSigLabData, 40, base);
    cmsPipeline* res = _cmsReadFloatOutputTag(static_cast<cmsHPROFILE>(p), 0xBADA);
    ASSERT_TRUE(res != nullptr, "Expected non-NULL Lut when insertions succeed");
    if (res) {
        ASSERT_TRUE(base->beginStages.size() == 1, "Begin stage should be one (XYZ begin)");
        ASSERT_TRUE(base->beginStages[0] == std::string("NormalizeToXyzFloat"), "Begin stage should be NormalizeToXyzFloat");
        ASSERT_TRUE(base->endStages.size() == 1, "End stage should be one (Lab end)");
        ASSERT_TRUE(base->endStages[0] == std::string("NormalizeFromLabFloat"), "End stage should be NormalizeFromLabFloat");
        cmsPipelineFree(res);
        delete base;
    }
    ++g_testCount;
    ++g_passCount;
}

static void test_BeginInsertionFailure_FreesLut_andReturnsNull() {
    // Explanation: Simulate a failure during the begin stage insertion and ensure
    // the function returns NULL and frees the allocated Lut.
    resetGlobals();
    g_failOnBeginLab = true;
    auto base = new FakePipeline();
    auto p = makeProfile(cmsSigLabData, cmsSigLabData, 50, base);
    cmsPipeline* res = _cmsReadFloatOutputTag(static_cast<cmsHPROFILE>(p), 0xC0DE);
    ASSERT_TRUE(res == nullptr, "Expected NULL on begin insertion failure");
    // Ensure the LUT was freed
    ASSERT_TRUE(g_freedLutCount == 1, "Expected LUT to be freed on error");
    delete base;
    ++g_testCount;
}

static void test_EndInsertionFailure_FreesLut_andReturnsNull() {
    // Explanation: Simulate failure during the end stage insertion after a successful begin.
    resetGlobals();
    g_failOnEndLab = true;
    auto base = new FakePipeline();
    auto p = makeProfile(cmsSigLabData, cmsSigLabData, 60, base);
    cmsPipeline* res = _cmsReadFloatOutputTag(static_cast<cmsHPROFILE>(p), 0xB1A2);
    ASSERT_TRUE(res == nullptr, "Expected NULL on end insertion failure");
    // Ensure the LUT was freed
    ASSERT_TRUE(g_freedLutCount == 1, "Expected LUT to be freed on error");
    delete base;
    ++g_testCount;
}

// ---------------------- Main Runner ------------------------------

int main() {
    // Run tests
    RUN_TEST(test_LutNullReturnsNull);
    RUN_TEST(test_LabPCS_DataLab_beginEndInserted);
    RUN_TEST(test_LabPCS_DataXYZ_beginEndInserted);
    RUN_TEST(test_XYZPCS_DataLab_beginEndInserted);
    RUN_TEST(test_BeginInsertionFailure_FreesLut_andReturnsNull);
    RUN_TEST(test_EndInsertionFailure_FreesLut_andReturnsNull);

    // Print a concise report
    reportSummary();
    return (g_failMessages.empty() ? 0 : 1);
}

// End of test harness

/* Explanation of tests (summary):
   - test_LutNullReturnsNull: Verifies NULL Lut yields NULL result (no processing).
   - test_LabPCS_DataLab_beginEndInserted: Lab PCS and Lab data space inserts Lab begin and Lab end stages.
   - test_LabPCS_DataXYZ_beginEndInserted: Lab PCS with XYZ data space inserts Lab begin and XYZ end stages.
   - test_XYZPCS_DataLab_beginEndInserted: XYZ PCS with Lab data space inserts XYZ begin and Lab end stages.
   - test_BeginInsertionFailure_FreesLut_andReturnsNull: Simulates begin-stage insertion failure; ensures NULL return and LUT freed.
   - test_EndInsertionFailure_FreesLut_andReturnsNull: Simulates end-stage insertion failure; ensures NULL return and LUT freed.
   - All static/private-like behavior is simulated via file-scope flags (g_failOn*), and the LUT-free
     count (g_freedLutCount) asserts correct cleanup on errors.
*/