// Self-contained unit tests for the focal method _cmsLinkProfiles
// Note: This test harness provides minimal in-place mocks for the internal
// dependencies used by _cmsLinkProfiles in order to exercise its control flow
// (range checks, BPC toggling, intent lookup, and successful linking).
// The tests do not rely on GTest or any external testing framework; a simple
// main() driver runs the test cases and prints results.
//
// The implementation mirrors the logic shown in the focal method and class
// dependencies, but uses lightweight, test-specific mocks to avoid external
// dependencies. All test code is contained in this single file and is suitable
// for compilation with a C++11 toolchain.

#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <cstdarg>
#include <cassert>
#include <lcms2_internal.h>


// Domain-specific constants and types (mocked for testing)
using cmsContext = void*;
using cmsUInt32Number = unsigned int;
using cmsBool = int;
using cmsFloat64Number = double;

// Opaque handle type for a color pipeline (mock)
struct cmsPipeline { int dummy; };

// Forward declaration of the focal method under test
cmsPipeline* _cmsLinkProfiles(cmsContext     ContextID,
                             cmsUInt32Number nProfiles,
                             cmsUInt32Number TheIntents[],
                             cmsHPROFILE     hProfiles[],
                             cmsBool         BPC[],
                             cmsFloat64Number AdaptationStates[],
                             cmsUInt32Number dwFlags);

// Typedef for a profile handle (mock)
struct _MockProfile;
using cmsHPROFILE = _MockProfile*;

// Mock profile object with an encoded ICC version
struct _MockProfile {
    unsigned int encodedICCVersion;
};

// Global state to capture error signaling (mock)
static int g_LastErrorCode = 0;
static std::string g_LastErrorMessage;

// Global state to capture intent handler behavior (mock)
static int g_SupportedIntentValue = 1; // default supported intent
static cmsPipeline g_SentinelPipeline;  // used by successful Link path
static cmsIntentsList* g_RegisteredIntent = nullptr;

// Forward declare mock of the intent lookup
struct cmsIntentsList {
    cmsIntentsList* Next;
    cmsPipeline* (*Link)(cmsContext ContextID,
                         cmsUInt32Number nProfiles,
                         cmsUInt32Number TheIntents[],
                         cmsHPROFILE hProfiles[],
                         cmsBool BPC[],
                         cmsFloat64Number AdaptationStates[],
                         cmsUInt32Number dwFlags);
};

// Helper: Convert formatted message into g_LastErrorMessage
void cmsSignalError(cmsContext ContextID, int Error, const char* Message, ...)
{
    g_LastErrorCode = Error;

    // Capture formatted message
    char buffer[512];
    va_list args;
    va_start(args, Message);
    vsnprintf(buffer, sizeof(buffer), Message, args);
    va_end(args);
    g_LastErrorMessage = buffer;
}

// Mock: cmsGetEncodedICCversion(profile)
unsigned int cmsGetEncodedICCversion(cmsHPROFILE hProfile)
{
    if (hProfile == nullptr) return 0;
    return hProfile->encodedICCVersion;
}

// Mock: SearchIntent(ContextID, Intent)
cmsIntentsList* SearchIntent(cmsContext ContextID, cmsUInt32Number Intent);

// Constants for intents (mock values)
const cmsUInt32Number INTENT_ABSOLUTE_COLORIMETIC = 100;
const cmsUInt32Number INTENT_PERCEPTUAL = 101;
const cmsUInt32Number INTENT_SATURATION = 102;

// Version threshold used by the code to decide BPC applicability
// (0x04000000 hex as in the focal method)
const unsigned int ICC_VERSION_THRESHOLD = 0x04000000;

// Forward declaration of the test-specific Link function
cmsPipeline* TestIntentLink(cmsContext ContextID,
                            cmsUInt32Number nProfiles,
                            cmsUInt32Number TheIntents[],
                            cmsHPROFILE hProfiles[],
                            cmsBool BPC[],
                            cmsFloat64Number AdaptationStates[],
                            cmsUInt32Number dwFlags);

// Implementation of SearchIntent used by _cmsLinkProfiles
cmsIntentsList* SearchIntent(cmsContext ContextID, cmsUInt32Number Intent)
{
    // Simple mapping: if the requested Intent matches the globally configured
    // supported intent value, return a mock Intent with a Link function.
    if (Intent == (cmsUInt32Number)g_SupportedIntentValue && g_RegisteredIntent != nullptr) {
        return g_RegisteredIntent;
    }
    // Otherwise, no handler found
    return nullptr;
}

// Helper: Create and register a mock intent with a Link function
void RegisterMockIntent()
{
    // Static to ensure a stable address across calls
    static cmsIntentsList s_intent;
    s_intent.Next = nullptr;
    s_intent.Link = &TestIntentLink;
    g_RegisteredIntent = &s_intent;
    // Reset sentinel for each test run
    g_SentinelPipeline.dummy = 0;
}

// Mock Link function: records invocation and returns a sentinel pipeline
cmsPipeline* TestIntentLink(cmsContext ContextID,
                            cmsUInt32Number nProfiles,
                            cmsUInt32Number TheIntents[],
                            cmsHPROFILE hProfiles[],
                            cmsBool BPC[],
                            cmsFloat64Number AdaptationStates[],
                            cmsUInt32Number dwFlags)
{
    // For the purposes of tests, simply return a pointer to a sentinel pipeline
    (void)ContextID; (void)nProfiles; (void)TheIntents;
    (void)hProfiles; (void)BPC; (void)AdaptationStates; (void)dwFlags;
    g_SentinelPipeline.dummy = 1;
    return &g_SentinelPipeline;
}

// The focal method under test (reproduced here for isolated unit testing)
cmsPipeline* _cmsLinkProfiles(cmsContext     ContextID,
                              cmsUInt32Number nProfiles,
                              cmsUInt32Number TheIntents[],
                              cmsHPROFILE     hProfiles[],
                              cmsBool         BPC[],
                              cmsFloat64Number AdaptationStates[],
                              cmsUInt32Number dwFlags)
{
{
    cmsUInt32Number i;
    cmsIntentsList* Intent;
    // Make sure a reasonable number of profiles is provided
    if (nProfiles <= 0 || nProfiles > 255) {
         cmsSignalError(ContextID, cmsERROR_RANGE, "Couldn't link '%d' profiles", nProfiles);
        return NULL;
    }
    for (i=0; i < nProfiles; i++) {
        // Check if black point is really needed or allowed. Note that
        // following Adobe's document:
        // BPC does not apply to devicelink profiles, nor to abs colorimetric,
        // and applies always on V4 perceptual and saturation.
        if (TheIntents[i] == INTENT_ABSOLUTE_COLORIMETIC)
            BPC[i] = FALSE;
        if (TheIntents[i] == INTENT_PERCEPTUAL || TheIntents[i] == INTENT_SATURATION) {
            // Force BPC for V4 profiles in perceptual and saturation
            if (cmsGetEncodedICCversion(hProfiles[i]) >= 0x4000000)
                BPC[i] = TRUE;
        }
    }
    // Search for a handler. The first intent in the chain defines the handler. That would
    // prevent using multiple custom intents in a multiintent chain, but the behaviour of
    // this case would present some issues if the custom intent tries to do things like
    // preserve primaries. This solution is not perfect, but works well on most cases.
    Intent = SearchIntent(ContextID, TheIntents[0]);
    if (Intent == NULL) {
        cmsSignalError(ContextID, cmsERROR_UNKNOWN_EXTENSION, "Unsupported intent '%d'", TheIntents[0]);
        return NULL;
    }
    // Call the handler
    return Intent ->Link(ContextID, nProfiles, TheIntents, hProfiles, BPC, AdaptationStates, dwFlags);
}
}

// Mock: Error codes (subset)
enum {
    cmsERROR_RANGE = 1,
    cmsERROR_UNKNOWN_EXTENSION = 2
};

// Test helpers
static bool g_TestFailed = false;
static int g_TestNumber = 0;

#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        printf("Test %d FAILED: %s\n", ++g_TestNumber, msg); \
        g_TestFailed = true; \
    } else { \
        printf("Test %d PASSED: %s\n", ++g_TestNumber, msg); \
    } \
} while(0)

// Test case 1: nProfiles <= 0 should trigger cmsERROR_RANGE and return NULL
void test_case_range_negative_or_zero()
{
    g_TestNumber = 0;
    g_TestFailed = false;
    // Prepare inputs
    cmsContext ctx = (cmsContext)0x1;
    cmsUInt32Number nProfiles = 0;
    cmsUInt32Number intents[1] = { INTENT_PERCEPTUAL };
    cmsHPROFILE profiles[1] = { nullptr }; // not used due to early exit
    cmsBool BPC[1] = { 0 };
    cmsFloat64Number AdaptationStates[1] = { 0.0f };

    cmsPipeline* res = _cmsLinkProfiles(ctx, nProfiles, intents, profiles, BPC, AdaptationStates, 0);

    CHECK(res == nullptr, "Return NULL when nProfiles <= 0 should happen");
    CHECK(g_LastErrorCode == cmsERROR_RANGE, "Expected cmsERROR_RANGE for invalid nProfiles");
    printf("Error message: %s\n", g_LastErrorMessage.c_str());
}

// Test case 2: nProfiles > 255 should trigger cmsERROR_RANGE
void test_case_range_overflow()
{
    g_TestNumber = 0;
    g_TestFailed = false;

    cmsContext ctx = (cmsContext)0x2;
    cmsUInt32Number nProfiles = 256; // overflow
    cmsUInt32Number intents[2] = { INTENT_PERCEPTUAL, INTENT_ABSOLUTE_COLORIMETIC };
    // Two mock profiles
    _MockProfile p1 = { 0x00010000 }; // version below threshold
    _MockProfile p2 = { 0x00010000 };
    cmsHPROFILE profiles[2] = { &p1, &p2 };
    cmsBool BPC[2] = { 0, 0 };
    cmsFloat64Number AdaptationStates[2] = { 0.0, 0.0 };

    cmsPipeline* res = _cmsLinkProfiles(ctx, nProfiles, intents, profiles, BPC, AdaptationStates, 0);

    CHECK(res == nullptr, "Return NULL when nProfiles > 255 should happen");
    CHECK(g_LastErrorCode == cmsERROR_RANGE, "Expected cmsERROR_RANGE for overflow nProfiles");
}

// Test case 3: BPC toggling logic for perceptual with high ICC version
void test_case_bpc_perceptual_version_threshold()
{
    g_TestNumber = 0;
    g_TestFailed = false;

    // Prepare inputs: single profile with perceptual intent
    cmsContext ctx = (cmsContext)0x3;
    cmsUInt32Number nProfiles = 1;
    cmsUInt32Number intents[1] = { INTENT_PERCEPTUAL };
    _MockProfile prof = { ICC_VERSION_THRESHOLD }; // >= threshold
    cmsHPROFILE profiles[1] = { &prof };
    cmsBool BPC[1] = { 0 }; // initially false
    cmsFloat64Number AdaptationStates[1] = { 0.0 };

    cmsPipeline* res = _cmsLinkProfiles(ctx, nProfiles, intents, profiles, BPC, AdaptationStates, 0);

    CHECK(res != nullptr, "Link should succeed for perceptual with valid handler");
    CHECK(BPC[0] == 1, "BPC should be forced to TRUE for perceptual with ICC version >= threshold");
}

// Test case 4: Unsupported intent should trigger cmsERROR_UNKNOWN_EXTENSION
void test_case_unsupported_intent()
{
    g_TestNumber = 0;
    g_TestFailed = false;

    // Ensure no handler matches
    g_SupportedIntentValue = 9999; // any value not handled by SearchIntent
    // Prepare inputs
    cmsContext ctx = (cmsContext)0x4;
    cmsUInt32Number nProfiles = 1;
    cmsUInt32Number intents[1] = { 9999 }; // unsupported
    _MockProfile prof = { 0x00000001 };
    cmsHPROFILE profiles[1] = { &prof };
    cmsBool BPC[1] = { 0 };
    cmsFloat64Number AdaptationStates[1] = { 0.0 };

    cmsPipeline* res = _cmsLinkProfiles(ctx, nProfiles, intents, profiles, BPC, AdaptationStates, 0);

    CHECK(res == nullptr, "Return NULL when intent is unsupported");
    CHECK(g_LastErrorCode == cmsERROR_UNKNOWN_EXTENSION, "Expected cmsERROR_UNKNOWN_EXTENSION for unsupported intent");
}

// Test case 5: Successful linking path uses the first intent's handler
void test_case_successful_link()
{
    g_TestNumber = 0;
    g_TestFailed = false;

    // Prepare inputs: supported intent and valid handler
    g_SupportedIntentValue = 1;
    RegisterMockIntent();

    cmsContext ctx = (cmsContext)0x5;
    cmsUInt32Number nProfiles = 2;
    cmsUInt32Number intents[2] = { 1, 100 }; // The first intent is the one used for lookup
    _MockProfile p1 = { 0x00000001 };
    _MockProfile p2 = { 0x00000000 };
    cmsHPROFILE profiles[2] = { &p1, &p2 };
    cmsBool BPC[2] = { 0, 0 };
    cmsFloat64Number AdaptationStates[2] = { 0.0, 0.0 };

    cmsPipeline* res = _cmsLinkProfiles(ctx, nProfiles, intents, profiles, BPC, AdaptationStates, 0);

    CHECK(res != nullptr, "Link should succeed and return a valid pipeline");
    // Also verify that the Link function (TestIntentLink) was invoked by checking sentinel
    CHECK(g_SentinelPipeline.dummy == 1, "Intent Link function should be invoked and return sentinel");
}

// Test case 6: Multiple profiles and BPC resets per-intent, including absolute colorimetric case
void test_case_multiple_profiles_bpc_behavior()
{
    g_TestNumber = 0;
    g_TestFailed = false;

    // Setup: first intent perceptual to potentially set BPC, second is absolute colorimetric to force BPC off
    g_SupportedIntentValue = 1;
    RegisterMockIntent();

    cmsContext ctx = (cmsContext)0x6;
    cmsUInt32Number nProfiles = 2;
    cmsUInt32Number intents[2] = { INTENT_PERCEPTUAL, INTENT_ABSOLUTE_COLORIMETIC };
    _MockProfile p1 = { ICC_VERSION_THRESHOLD }; // will cause BPC[0] to be TRUE
    _MockProfile p2 = { 0x00000000 };            // Absolute colorimetic => BPC[1] forced to FALSE
    cmsHPROFILE profiles[2] = { &p1, &p2 };
    cmsBool BPC[2] = { 0, 1 }; // pre-set, first becomes TRUE due to perceptual+threshold, second forced to FALSE
    cmsFloat64Number AdaptationStates[2] = { 0.0, 0.0 };

    cmsPipeline* res = _cmsLinkProfiles(ctx, nProfiles, intents, profiles, BPC, AdaptationStates, 0);

    CHECK(res != nullptr, "Link should succeed for multi-profile path");
    CHECK(BPC[0] == 1, "BPC[0] should be set to TRUE for perceptual with threshold");
    CHECK(BPC[1] == 0, "BPC[1] should be forced to FALSE for absolute colorimetric");
}

// Simple runner
int main()
{
    // Run each test
    test_case_range_negative_or_zero();
    test_case_range_overflow();
    test_case_bpc_perceptual_version_threshold();
    test_case_unsupported_intent();
    test_case_successful_link();
    test_case_multiple_profiles_bpc_behavior();

    if (g_TestFailed) {
        printf("Some tests FAILED.\n");
        return 1;
    } else {
        printf("All tests PASSED.\n");
        return 0;
    }
}