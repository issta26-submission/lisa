// Minimal C++11 test suite for the focal function FastEvaluateCurves16
// This test suite is self-contained and does not rely on Google Test.
// It uses a tiny, custom test harness with non-terminating assertions to maximize coverage.

#include <vector>
#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Lightweight, non-GTest testing framework (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) do {                                 \
    ++g_total_tests;                                                   \
    if(!(cond)) {                                                     \
        ++g_failed_tests;                                             \
        std::cerr << "[TEST FAILED] " << msg                          \
                  << " (line " << __LINE__ << ")" << std::endl;     \
    }                                                                 \
} while(0)

#define EXPECT_EQ(a, b, msg) do {                                    \
    ++g_total_tests;                                                   \
    if(!((a) == (b))) {                                              \
        ++g_failed_tests;                                             \
        std::cerr << "[TEST FAILED] " << msg                          \
                  << " | expected: " << (b)                          \
                  << ", actual: " << (a) << " (line " << __LINE__ << ")" \
                  << std::endl;                                       \
    }                                                                 \
} while(0)

#define EXPECT_NEQ(a, b, msg) do {                                   \
    ++g_total_tests;                                                   \
    if((a) == (b)) {                                                  \
        ++g_failed_tests;                                             \
        std::cerr << "[TEST FAILED] " << msg                          \
                  << " | unexpected equality with: " << (a) << " (line " << __LINE__ << ")" \
                  << std::endl;                                       \
    }                                                                 \
} while(0)

// The focal function is compiled elsewhere (cmsopt.c). We declare it for C linkage.
extern "C" {
    // Basic type aliases used by the function signature in the source
    typedef unsigned short cmsUInt16Number;
    typedef unsigned int   cmsUInt32Number;
    typedef void             cmsContext;

    // The function under test (signature from the provided snippet)
    // In practice this is implemented in cmsopt.c; we only declare it here.
    // The CMSREGISTER macro is typically a register hint; it's defined as empty here.
    #define CMSREGISTER
    void FastEvaluateCurves16(CMSREGISTER const cmsUInt16Number In[],
                            CMSREGISTER cmsUInt16Number Out[],
                            CMSREGISTER const void* D);
}

// Minimal Curves16Data structure used to pass curve data to the function.
// This must mirror the memory layout expected by FastEvaluateCurves16 (nCurves, Curves[]).
struct Curves16Data {
    cmsUInt32Number nCurves;
    cmsUInt16Number** Curves;
};

// Helper to allocate and populate a Curves16Data instance
static Curves16Data* build_curves16_data(const std::vector<std::vector<cmsUInt16Number>>& curves) {
    Curves16Data* data = new Curves16Data;
    data->nCurves = static_cast<cmsUInt32Number>(curves.size());
    data->Curves = new cmsUInt16Number*[curves.size()];
    for (size_t i = 0; i < curves.size(); ++i) {
        data->Curves[i] = new cmsUInt16Number[curves[i].size()];
        std::memcpy(data->Curves[i], curves[i].data(), curves[i].size() * sizeof(cmsUInt16Number));
    }
    return data;
}

// Helper to free a Curves16Data instance
static void free_curves16_data(Curves16Data* data) {
    if (!data) return;
    for (cmsUInt32Number i = 0; i < data->nCurves; ++i) {
        delete[] data->Curves[i];
    }
    delete[] data->Curves;
    delete data;
}

// Explanatory unit test 1: Basic mapping with two simple curves
// Verifies that FastEvaluateCurves16 selects the correct elements from each curve.
static void test_basic_mapping() {
    // Curve 0: [0,1,2,3], Curve 1: [10,11,12,13]
    Curves16Data* data = build_curves16_data({
        {0, 1, 2, 3},
        {10, 11, 12, 13}
    });

    cmsUInt16Number In[2]  = { 2, 3 };   // Pick 2 from curve0, 3 from curve1
    cmsUInt16Number Out[2] = { 0xFFFF, 0xFFFF };

    FastEvaluateCurves16(In, Out, data);

    EXPECT_EQ(Out[0], (cmsUInt16Number)2, "Out[0] should be data->Curves[0][In[0]]");
    EXPECT_EQ(Out[1], (cmsUInt16Number)13, "Out[1] should be data->Curves[1][In[1]]");

    free_curves16_data(data);
}

// Explanatory unit test 2: Zero curves
// Ensures that when nCurves == 0, the function does not crash.
// Out content is not guaranteed to change; we only verify that the call completes safely.
static void test_zero_curves_no_op() {
    Curves16Data* data = new Curves16Data;
    data->nCurves = 0;
    data->Curves = nullptr;

    // In and Out can be empty; we use vector-based buffers to avoid zero-length arrays
    std::vector<cmsUInt16Number> InVec;  // empty
    std::vector<cmsUInt16Number> OutVec; // empty

    FastEvaluateCurves16(InVec.data(), OutVec.data(), data);

    // If we reach here without crash, the test passes.
    EXPECT_TRUE(true, "Zero-curve case should not crash and should be a no-op");

    free_curves16_data(data);
}

// Explanatory unit test 3: Multiple curves with varying lengths
// Verifies correct indexing even when curves differ in length.
static void test_varying_curve_lengths() {
    // Curve 0 length 3: [5,6,7]
    // Curve 1 length 4: [10,11,12,13]
    // Curve 2 length 2: [20,21]
    Curves16Data* data = build_curves16_data({
        {5, 6, 7},
        {10, 11, 12, 13},
        {20, 21}
    });

    cmsUInt16Number In[3]  = { 0, 1, 1 }; // pick 0->5, 1->11, 1->21
    cmsUInt16Number Out[3] = { 0xFFFF, 0xFFFF, 0xFFFF };

    FastEvaluateCurves16(In, Out, data);

    EXPECT_EQ(Out[0], (cmsUInt16Number)5, "Out[0] should be 5");
    EXPECT_EQ(Out[1], (cmsUInt16Number)11, "Out[1] should be 11");
    EXPECT_EQ(Out[2], (cmsUInt16Number)21, "Out[2] should be 21");

    free_curves16_data(data);
}

// Explanatory unit test 4: Out array boundary test (only first nCurves elements should be written)
// Verifies that elements beyond nCurves are not modified by the function.
static void test_out_boundary_protection() {
    // Curve 0: [1,2], Curve 1: [3,4,5]
    Curves16Data* data = build_curves16_data({
        {1, 2},
        {3, 4, 5}
    });

    cmsUInt16Number In[2]  = { 1, 2 }; // pick 1 from curve0 -> 2, 2 from curve1 -> 5
    cmsUInt16Number Out[4] = { 0xAA, 0xBB, 0xCC, 0xDD }; // pre-fill with sentinels

    FastEvaluateCurves16(In, Out, data);

    // Only first two elements should be modified
    EXPECT_EQ(Out[0], (cmsUInt16Number)2, "Out[0] should be 2");
    EXPECT_EQ(Out[1], (cmsUInt16Number)5, "Out[1] should be 5");
    // Out[2] and Out[3] should remain unchanged
    EXPECT_EQ(Out[2], (cmsUInt16Number)0xCC, "Out[2] should remain unchanged");
    EXPECT_EQ(Out[3], (cmsUInt16Number)0xDD, "Out[3] should remain unchanged");

    free_curves16_data(data);
}

// Run all tests
static void run_all_tests() {
    test_basic_mapping();
    test_zero_curves_no_op();
    test_varying_curve_lengths();
    test_out_boundary_protection();

    std::cout << "Total tests run: " << g_total_tests << std::endl;
    if (g_failed_tests == 0) {
        std::cout << "All tests PASSED." << std::endl;
    } else {
        std::cout << "Tests failed: " << g_failed_tests << std::endl;
    }
}

// Entry point
int main() {
    run_all_tests();
    return (g_failed_tests == 0) ? 0 : 1;
}