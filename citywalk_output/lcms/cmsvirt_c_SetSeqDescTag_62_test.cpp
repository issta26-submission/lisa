// Unit tests for the focal method: cmsBool SetSeqDescTag(cmsHPROFILE hProfile, const char* Model)
// This test suite targets the behavior of writing a single profile sequence description
// into a color profile and then validating that the Manufacturer and Model fields are set
// as expected. The tests assume Little CMS public API is available and linked
// (lcms2.h / liblcms2). No external testing framework (GTest) is used; a lightweight
// test harness is implemented here.

#include <lcms2_internal.h>
#include <iostream>
#include <lcms2.h>
#include <cstring>


// Include Little CMS public API

// Simple lightweight test harness
static bool g_TestPass = true;
static int g_TestCount = 0;
static int g_PassCount = 0;

#define TEST_ASSERT(cond, msg)                                  \
    do {                                                        \
        ++g_TestCount;                                          \
        if (!(cond)) {                                          \
            std::cerr << "Test failure: " << (msg) << std::endl; \
            g_TestPass = false;                                 \
        } else {                                                \
            ++g_PassCount;                                      \
        }                                                       \
    } while(0)

// Helper to print a summary
static void PrintSummary() {
    std::cout << "Tests run: " << g_TestCount
              << ", Passed: " << g_PassCount
              << ", Failed: " << (g_TestCount - g_PassCount) << std::endl;
}

// Test 1: Positive path
// - Create a fresh profile
// - Invoke SetSeqDescTag with a sample Model string
// - Read back the Profile Sequence Description tag and verify Manufacturer and Model
static bool Test_SetSeqDescTag_Positive_Readback() {
    // Create a simple, in-memory profile to attach the sequence to.
    // Context is NULL here to use the default context; this matches typical usage
    // in LCMS examples.
    cmsContext ContextID = NULL;
    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(ContextID);
    if (hProfile == NULL) {
        std::cerr << "Test_SetSeqDescTag_Positive_Readback: Failed to create placeholder profile." << std::endl;
        return false;
    }

    const char* Model = "TestModel_Positive";
    cmsBool rc = SetSeqDescTag(hProfile, Model);
    TEST_ASSERT(rc == TRUE, "SetSeqDescTag should return TRUE on valid profile");

    // Read back the profile sequence description tag
    cmsSEQ* Seq = (cmsSEQ*) cmsReadTag(hProfile, cmsSigProfileSequenceDescTag);
    TEST_ASSERT(Seq != NULL, "Profile should contain a valid ProfileSequenceDesc tag after SetSeqDescTag");

    // Read Manufacturer and Model via CMS MLU accessors
    // Manufacturer should be "Little CMS" (as set in the focal method)
    char ManufacturerBuf[256] = {0};
    cmsMLUgetASCII(&Seq->seq[0].Manufacturer, cmsNoLanguage, cmsNoCountry, ManufacturerBuf);

    char ModelBuf[256] = {0};
    cmsMLUgetASCII(&Seq->seq[0].Model, cmsNoLanguage, cmsNoCountry, ModelBuf);

    TEST_ASSERT(std::strcmp(ManufacturerBuf, "Little CMS") == 0,
                "Manufacturer should be 'Little CMS' in the profile sequence tag");
    TEST_ASSERT(std::strcmp(ModelBuf, Model) == 0,
                "Model in the profile sequence tag should match the input Model string");

    // Cleanup
    cmsCloseProfile(hProfile);
    return true;
}

// Test 2: Empty Model string
// - Ensure that an empty Model string still writes the tag and preserves Manufacturer
// - This exercises the path where Model content is an empty ASCII string
static bool Test_SetSeqDescTag_EmptyModel() {
    cmsContext ContextID = NULL;
    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(ContextID);
    if (hProfile == NULL) {
        std::cerr << "Test_SetSeqDescTag_EmptyModel: Failed to create placeholder profile." << std::endl;
        return false;
    }

    const char* Model = "";
    cmsBool rc = SetSeqDescTag(hProfile, Model);
    TEST_ASSERT(rc == TRUE, "SetSeqDescTag should return TRUE even with empty Model string");

    cmsSEQ* Seq = (cmsSEQ*) cmsReadTag(hProfile, cmsSigProfileSequenceDescTag);
    TEST_ASSERT(Seq != NULL, "Profile should still contain a ProfileSequenceDesc tag after SetSeqDescTag with empty Model");

    char ManufacturerBuf[256] = {0};
    cmsMLUgetASCII(&Seq->seq[0].Manufacturer, cmsNoLanguage, cmsNoCountry, ManufacturerBuf);
    char ModelBuf[256] = {0};
    cmsMLUgetASCII(&Seq->seq[0].Model, cmsNoLanguage, cmsNoCountry, ModelBuf);

    TEST_ASSERT(std::strcmp(ManufacturerBuf, "Little CMS") == 0,
                "Manufacturer should be 'Little CMS' even when Model is empty");
    TEST_ASSERT(std::strcmp(ModelBuf, Model) == 0,
                "Model in the profile sequence tag should match the empty Model string");

    cmsCloseProfile(hProfile);
    return true;
}

// Main driver: run all tests and report
int main() {
    // Run tests
    if (Test_SetSeqDescTag_Positive_Readback()) {
        // nothing -> success tracked by harness
    }
    if (Test_SetSeqDescTag_EmptyModel()) {
        // nothing -> success tracked by harness
    }

    PrintSummary();
    // Return 0 if all tests passed, non-zero otherwise
    return (g_TestCount == g_PassCount) ? 0 : 1;
}