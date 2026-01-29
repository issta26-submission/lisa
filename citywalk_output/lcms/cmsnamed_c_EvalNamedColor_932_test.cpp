// High-quality C++11 unit tests for EvalNamedColor in cmsnamed.c
// This test suite avoids GTest and uses a lightweight, non-terminating assertion framework.
// It constructs a minimal cmsNAMEDCOLORLIST and cmsStage to exercise both the in-range
// and out-of-range branches of EvalNamedColor.

// NOTE: This test relies on the public LittleCMS C API headers (lcms2.h) to define
// cmsNAMEDCOLORLIST, cmsNAMEDCOLOR, cmsStage, and related types.
// The tests allocate memory for the NamedColorList and its color entries, populate
// them, invoke EvalNamedColor, and verify the Out values.

#include <cstring>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Public LittleCMS headers

extern "C" void EvalNamedColor(const cmsFloat32Number In[], cmsFloat32Number Out[], const cmsStage *mpe);

// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do {                                 \
    ++g_tests_run;                                                     \
    if (!(cond)) {                                                     \
        std::cerr << "[FAILED] " << __FUNCTION__ << ": " << (msg) << "\n"; \
        ++g_tests_failed;                                              \
    } else {                                                           \
        std::cout << "[PASSED] " << __FUNCTION__ << ": " << (msg) << "\n"; \
    }                                                                  \
} while (0)

#define EXPECT_NEAR(a, b, tol, msg) do {                              \
    ++g_tests_run;                                                       \
    if (std::fabs((a) - (b)) > (tol)) {                                \
        std::cerr << "[FAILED] " << __FUNCTION__ << ": " << (msg) << " (got " \
                  << (a) << ", expected " << (b) << " +/- " << (tol) << ")\n"; \
        ++g_tests_failed;                                              \
    } else {                                                           \
        std::cout << "[PASSED] " << __FUNCTION__ << ": " << (msg) << "\n"; \
    }                                                                  \
} while (0)

static void test_EvalNamedColor_InRange_Index0() {
    // Purpose: Verify the in-range path for index 0 (In[0] == 0.0).
    // Expect Out[0] == 0.0 when DeviceColorant[0] == 0.
    cmsUInt32Number nColors = 2;
    cmsUInt16Number colorantCount = 1; // keep simple: single colorant to avoid unknown array sizes
    // Allocate header
    cmsNAMEDCOLORLIST* list = (cmsNAMEDCOLORLIST*) std::malloc(sizeof(cmsNAMEDCOLORLIST));
    list->ContextID = 0;
    list->nColors = nColors;
    list->ColorantCount = colorantCount;
    // Allocate color entries
    list->List = (cmsNAMEDCOLOR*) std::malloc(nColors * sizeof(cmsNAMEDCOLOR));

    // Fill color data
    // First color (index 0): DeviceColorant[0] = 0
    list->List[0].DeviceColorant[0] = 0;
    // Second color (index 1): DeviceColorant[0] = 32768 (0.5 in 0..1 range)
    list->List[1].DeviceColorant[0] = 32768;

    // Prepare stage
    cmsStage stage;
    stage.Data = (void*)list;

    // Call EvalNamedColor with In[0] = 0.0f
    cmsFloat32Number In[1] = { 0.0f };
    cmsFloat32Number Out[1] = { 0.0f };

    EvalNamedColor(In, Out, &stage);

    // Expect 0.0
    EXPECT_NEAR((double)Out[0], 0.0, 1e-6, "Out[0] should be 0.0 for index 0");
    
    // Cleanup
    std::free(list->List);
    std::free(list);
}

static void test_EvalNamedColor_InRange_Index1() {
    // Purpose: Verify the in-range path for index 1 (In[0] ~ 1/65535).
    // Expect Out[0] == 32768/65535 ~ 0.5
    cmsUInt32Number nColors = 2;
    cmsUInt16Number colorantCount = 1;
    cmsNAMEDCOLORLIST* list = (cmsNAMEDCOLORLIST*) std::malloc(sizeof(cmsNAMEDCOLORLIST));
    list->ContextID = 0;
    list->nColors = nColors;
    list->ColorantCount = colorantCount;
    list->List = (cmsNAMEDCOLOR*) std::malloc(nColors * sizeof(cmsNAMEDCOLOR));

    list->List[0].DeviceColorant[0] = 0;       // index 0
    list->List[1].DeviceColorant[0] = 32768;   // index 1

    cmsStage stage;
    stage.Data = (void*)list;

    // In[0] such that index = 1
    cmsFloat32Number In[1] = { 1.0f / 65535.0f };
    cmsFloat32Number Out[1] = { 0.0f };

    EvalNamedColor(In, Out, &stage);

    double expected = 32768.0 / 65535.0;
    EXPECT_NEAR((double)Out[0], expected, 1e-6, "Out[0] should equal 32768/65535 (~0.5) for index 1");

    std::free(list->List);
    std::free(list);
}

static void test_EvalNamedColor_OutOfRange() {
    // Purpose: Verify the out-of-range path when index >= nColors.
    // Use In[0] to produce index 2 (e.g., In[0] = 2/65535)
    cmsUInt32Number nColors = 1;      // only one color, so index 1 or more is out of range
    cmsUInt16Number colorantCount = 1;
    cmsNAMEDCOLORLIST* list = (cmsNAMEDCOLORLIST*) std::malloc(sizeof(cmsNAMEDCOLORLIST));
    list->ContextID = 0;
    list->nColors = nColors;
    list->ColorantCount = colorantCount;
    list->List = (cmsNAMEDCOLOR*) std::malloc(nColors * sizeof(cmsNAMEDCOLOR));

    list->List[0].DeviceColorant[0] = 12345; // arbitrary
    cmsStage stage;
    stage.Data = (void*)list;

    // In[0] to trigger index 2 -> out of range
    cmsFloat32Number In[1] = { 2.0f / 65535.0f };
    cmsFloat32Number Out[1] = { 123.0f }; // prefill to ensure overwritten

    EvalNamedColor(In, Out, &stage);

    // Out should be reset to 0.0
    EXPECT_NEAR((double)Out[0], 0.0, 1e-6, "Out[0] should be 0.0 when color index is out of range");

    std::free(list->List);
    std::free(list);
}

int main() {
    // Run tests
    std::cout << "Starting EvalNamedColor unit tests (no GTest)..." << std::endl;

    test_EvalNamedColor_InRange_Index0();
    test_EvalNamedColor_InRange_Index1();
    test_EvalNamedColor_OutOfRange();

    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << std::endl;
    return g_tests_failed ? EXIT_FAILURE : EXIT_SUCCESS;
}