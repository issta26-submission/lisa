// High-quality C++11 unit tests for EvalNamedColorPCS in cmsnamed.c
// This test suite does not rely on Google Test. It uses a minimal in-file test harness
// with non-terminating EXPECT-style assertions to maximize code coverage.

#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Include the library's public and internal headers to access the needed types.
// Depending on your build setup, you may need to adjust include paths.

// Simple non-terminating test harness
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "[TEST FAILED] " << (msg) << std::endl; \
            ++g_failures; \
        } \
    } while (0)

#define EXPECT_NEAR(a, b, tol, msg) \
    do { \
        if (std::fabs((a) - (b)) > (tol)) { \
            std::cerr << "[TEST FAILED] " << (msg) \
                      << " | expected: " << (b) << ", actual: " << (a) \
                      << ", tol: " << (tol) << std::endl; \
            ++g_failures; \
        } \
    } while (0)


// Helper to create a Named Color List in memory compatible with cmsNAMEDCOLORLIST layout.
// We rely on the internal layout provided by lcms2_internal.h.
// This assumes the library uses a struct similar to:
// typedef struct { cmsUInt16Number nColors; cmsNAMEDCOLOR List[]; cmsContext ContextID; } cmsNAMEDCOLORLIST;
static cmsNAMEDCOLORLIST* CreateNamedColorList(int nColors,
                                               const cmsUInt16Number pcs[][3],
                                               cmsContext context)
{
    size_t allocSize = sizeof(cmsNAMEDCOLORLIST) + (nColors - 1) * sizeof(cmsNAMEDCOLOR);
    cmsNAMEDCOLORLIST* list = (cmsNAMEDCOLORLIST*) std::calloc(1, allocSize);
    if (list == NULL) return NULL;

    list->nColors = (cmsUInt16Number) nColors;
    list->ContextID = context;
    for (int i = 0; i < nColors; ++i) {
        list->List[i].PCS[0] = pcs[i][0];
        list->List[i].PCS[1] = pcs[i][1];
        list->List[i].PCS[2] = pcs[i][2];
    }
    return list;
}

// Lightweight wrapper to run EvalNamedColorPCS and collect outputs
static void RunEvalNamedColorPCS(const cmsFloat32Number inVal,
                                 cmsFloat32Number out[3],
                                 cmsNAMEDCOLORLIST* namedList)
{
    cmsStage mpe;
    // The EvalNamedColorPCS expects a cmsStage with a Data member pointing to the named color list.
    // We rely on the library's cmsStage structure having a Data field that we can assign.
    mpe.Data = (void*) namedList;

    cmsFloat32Number In[1] = { inVal };
    cmsFloat32Number Out[3] = { 0.0f, 0.0f, 0.0f };

    EvalNamedColorPCS(In, Out, &mpe);

    out[0] = Out[0];
    out[1] = Out[1];
    out[2] = Out[2];
}

// Test 1: Index 0 mapping to first PCS color
static void test_EvalNamedColorPCS_Index0_Color0()
{
    // Prepare a named color list with 3 entries
    // PCS values chosen for deterministic results
    // Entry 0: PCS = [0, 32768, 65535]  => [0.0, ~0.5, 1.0]
    // Entry 1: PCS = [1, 2, 3]
    // Entry 2: PCS = [654, 655, 676]
    const int nColors = 3;
    cmsUInt16Number pcs[3][3] = {
        { 0,     32768, 65535 },
        { 1,     2,     3     },
        { 654,   655,   676   }
    };

    cmsNAMEDCOLORLIST* list = CreateNamedColorList(nColors, pcs, 0);
    if (list == NULL) {
        std::cerr << "[TEST SETUP FAILED] Could not allocate NamedColorList" << std::endl;
        ++g_failures;
        return;
    }

    cmsFloat32Number Out[3];
    RunEvalNamedColorPCS(0.0f, Out, list);

    // Expected results
    EXPECT_NEAR((Out[0]), (float)pcs[0][0] / 65535.0f, 1e-6, "Out[0] for index 0 should be PCS[0]/65535");
    EXPECT_NEAR((Out[1]), (float)pcs[0][1] / 65535.0f, 1e-6, "Out[1] for index 0 should be PCS[1]/65535");
    EXPECT_NEAR((Out[2]), (float)pcs[0][2] / 65535.0f, 1e-6, "Out[2] for index 0 should be PCS[2]/65535");

    std::free(list);
}

// Test 2: Out-of-range index (In = 1.0 -> index = 65535, which is >= nColors)
static void test_EvalNamedColorPCS_OutOfRange_Index1()
{
    const int nColors = 3;
    cmsUInt16Number pcs[3][3] = {
        { 0, 0, 0 },
        { 1, 2, 3 },
        { 654, 655, 676 }
    };

    cmsNAMEDCOLORLIST* list = CreateNamedColorList(nColors, pcs, 0);
    if (list == NULL) {
        std::cerr << "[TEST SETUP FAILED] Could not allocate NamedColorList" << std::endl;
        ++g_failures;
        return;
    }

    cmsFloat32Number Out[3];
    RunEvalNamedColorPCS(1.0f, Out, list);

    // Expect out to be all zeros due to out-of-range index
    EXPECT_NEAR(Out[0], 0.0f, 1e-6, "Out[0] should be 0 when out of range");
    EXPECT_NEAR(Out[1], 0.0f, 1e-6, "Out[1] should be 0 when out of range");
    EXPECT_NEAR(Out[2], 0.0f, 1e-6, "Out[2] should be 0 when out of range");

    std::free(list);
}

// Test 3: Index 2 mapping to color at index 2
static void test_EvalNamedColorPCS_Index2_Color2()
{
    const int nColors = 3;
    cmsUInt16Number pcs[3][3] = {
        { 0, 0, 0 },
        { 32768, 49152, 65535 }, // mid values
        { 12345, 23456, 34567 }  // intended color at index 2
    };

    cmsNAMEDCOLORLIST* list = CreateNamedColorList(nColors, pcs, 0);
    if (list == NULL) {
        std::cerr << "[TEST SETUP FAILED] Could not allocate NamedColorList" << std::endl;
        ++g_failures;
        return;
    }

    cmsFloat32Number Out[3];
    // In maps to index 2: In = 2/65535
    RunEvalNamedColorPCS((cmsFloat32Number)2.0f / 65535.0f, Out, list);

    EXPECT_NEAR((Out[0]), (float)pcs[2][0] / 65535.0f, 1e-6, "Out[0] for index 2 should be PCS[0]/65535");
    EXPECT_NEAR((Out[1]), (float)pcs[2][1] / 65535.0f, 1e-6, "Out[1] for index 2 should be PCS[1]/65535");
    EXPECT_NEAR((Out[2]), (float)pcs[2][2] / 65535.0f, 1e-6, "Out[2] for index 2 should be PCS[2]/65535");

    std::free(list);
}

// Test 4: Boundary case with In leading to index 3 (out-of-range)
static void test_EvalNamedColorPCS_Boundary_OutOfRange_Index3()
{
    const int nColors = 3;
    cmsUInt16Number pcs[3][3] = {
        { 0, 0, 0 },
        { 100, 200, 300 },
        { 400, 500, 600 }
    };

    cmsNAMEDCOLORLIST* list = CreateNamedColorList(nColors, pcs, 0);
    if (list == NULL) {
        std::cerr << "[TEST SETUP FAILED] Could not allocate NamedColorList" << std::endl;
        ++g_failures;
        return;
    }

    cmsFloat32Number Out[3];
    // In maps to index 3: In = 3/65535
    RunEvalNamedColorPCS((cmsFloat32Number)3.0f / 65535.0f, Out, list);

    // Expect zeros due to out-of-range
    EXPECT_NEAR(Out[0], 0.0f, 1e-6, "Out[0] should be 0 when index >= nColors");
    EXPECT_NEAR(Out[1], 0.0f, 1e-6, "Out[1] should be 0 when index >= nColors");
    EXPECT_NEAR(Out[2], 0.0f, 1e-6, "Out[2] should be 0 when index >= nColors");

    std::free(list);
}

// Main: run all tests and report summary
int main() {
    std::cout << "Running EvalNamedColorPCS unit tests..." << std::endl;

    test_EvalNamedColorPCS_Index0_Color0();
    test_EvalNamedColorPCS_OutOfRange_Index1();
    test_EvalNamedColorPCS_Index2_Color2();
    test_EvalNamedColorPCS_Boundary_OutOfRange_Index3();

    if (g_failures == 0) {
        std::cout << "[TEST SUCCEEDED] All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "[TEST FAILED] " << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}