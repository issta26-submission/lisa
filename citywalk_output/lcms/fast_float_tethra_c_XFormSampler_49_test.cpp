/*
This is a self-contained C++11 test harness for the focal method:
int XFormSampler(CMSREGISTER const cmsFloat32Number In[], CMSREGISTER cmsFloat32Number Out[], CMSREGISTER void* Cargo)

The test mocks the external dependency:
- cmsPipelineEvalFloat(In, Out, Lut)

to verify that XFormSampler correctly forwards parameters to the pipeline evaluator
and returns TRUE as expected.

Notes:
- The test does not depend on Google Test (GTest). It uses a small, non-terminating assertion approach.
- All tests are executed from main(), per the provided guidance.
- The test suite focuses on coverage of:
  - Normal usage: valid In/Out and non-null Cargo
  - Null In pointer
  - Null Out pointer
  - Null Cargo (LUT) pointer
- This harness includes a minimal stand-in for the required types and macros to compile in isolation.
*/

#include <iostream>
#include <fast_float_internal.h>


// Minimal domain types and macros to emulate the environment of the focal method.
// These are intentionally lightweight to keep the test self-contained.
typedef float cmsFloat32Number;
typedef void cmsPipeline;
typedef int cmsBool;
#define TRUE 1
#define FALSE 0
#define CMSREGISTER

// Forward declaration to match the signature used by XFormSampler.
// The test provides a minimal mock for cmsPipelineEvalFloat.
extern "C" int cmsPipelineEvalFloat(const cmsFloat32Number In[], cmsFloat32Number Out[], cmsPipeline* Lut);

// Global mocks to record the last call parameters to cmsPipelineEvalFloat.
static const cmsFloat32Number* g_LastIn = nullptr;
static cmsFloat32Number* g_LastOut = nullptr;
static cmsPipeline* g_LastLut = nullptr;

// A dummy pipeline object to pass as Cargo/LUT.
static cmsPipeline g_dummyPipeline;

// Mock implementation of cmsPipelineEvalFloat to capture arguments without performing real work.
// This function uses C linkage to mimic the real CMS library symbol.
extern "C" int cmsPipelineEvalFloat(const cmsFloat32Number In[], cmsFloat32Number Out[], cmsPipeline* Lut) {
    g_LastIn = In;
    g_LastOut = Out;
    g_LastLut = Lut;
    // Do not mutate In/Out; the test exercises that XFormSampler passes the pointers correctly.
    return 1; // Simulate a successful evaluation (TRUE)
}

// The focal method under test, copied verbatim from the provided snippet.
// Note: CMSREGISTER is a no-op macro in this harness.
int XFormSampler(CMSREGISTER const cmsFloat32Number In[], CMSREGISTER cmsFloat32Number Out[], CMSREGISTER void* Cargo)
{    
{    
    cmsPipelineEvalFloat(In, Out, (cmsPipeline*) Cargo);
    return TRUE;
}
}

// Simple, non-terminating test assertion utilities.
// They log failures but do not abort test execution, enabling higher code coverage.
static int gTestCount = 0;
static int gFailures = 0;

#define EXPECT_EQ_INT(expected, actual) do { \
    ++gTestCount; \
    if( (int)(expected) != (int)(actual) ) { \
        ++gFailures; \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << "  Expected int " << (int)(expected) \
                  << " but got " << (int)(actual) << std::endl; \
    } \
} while(0)

#define EXPECT_PTR_EQ(expected, actual) do { \
    ++gTestCount; \
    if( (expected) != (actual) ) { \
        ++gFailures; \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << "  Expected pointer " << expected \
                  << " but got " << actual << std::endl; \
    } \
} while(0)

#define EXPECT_EQ_FLOAT(expected, actual) do { \
    ++gTestCount; \
    if( (float)(expected) != (float)(actual) ) { \
        ++gFailures; \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << "  Expected float " << (float)(expected) \
                  << " but got " << (float)(actual) << std::endl; \
    } \
} while(0)

// Test 1: Normal usage with valid In, Out, and Cargo
// Verifies that:
// - XFormSampler returns TRUE
// - cmsPipelineEvalFloat is invoked with In, Out, and the provided Cargo
// - The recorded arguments match the inputs
void test_normal_case() {
    // Prepare test data
    const cmsFloat32Number InArr[4]  = {1.0f, 2.0f, 3.0f, 4.0f};
    cmsFloat32Number OutArr[4]       = { -1.0f, -1.0f, -1.0f, -1.0f }; // seed values to detect any unexpected writes

    // Reset mock tracking
    g_LastIn = nullptr;
    g_LastOut = nullptr;
    g_LastLut = nullptr;

    // Execute
    int result = XFormSampler(InArr, OutArr, (void*)&g_dummyPipeline);

    // Assertions
    EXPECT_EQ_INT(TRUE, result);
    EXPECT_PTR_EQ(InArr, g_LastIn);
    EXPECT_PTR_EQ(OutArr, g_LastOut);
    EXPECT_PTR_EQ(&g_dummyPipeline, g_LastLut);

    // Ensure Out was not modified by the mock (since cmsPipelineEvalFloat is a no-op here)
    // We expect the first element to remain -1.0f
    EXPECT_EQ_FLOAT(-1.0f, OutArr[0]);
}

// Test 2: Null In pointer
// Verifies that XFormSampler forwards a NULL In and still calls the mock with NULL In.
void test_null_in() {
    cmsFloat32Number OutArr[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

    g_LastIn = nullptr;
    g_LastOut = nullptr;
    g_LastLut = nullptr;

    int result = XFormSampler(nullptr, OutArr, (void*)&g_dummyPipeline);

    EXPECT_EQ_INT(TRUE, result);
    EXPECT_PTR_EQ(nullptr, g_LastIn);
    EXPECT_PTR_EQ(OutArr, g_LastOut);
    EXPECT_PTR_EQ(&g_dummyPipeline, g_LastLut);
}

// Test 3: Null Out pointer
// Verifies that XFormSampler forwards a NULL Out and still calls the mock with NULL Out.
void test_null_out() {
    const cmsFloat32Number InArr[4] = { 5.0f, 6.0f, 7.0f, 8.0f };

    g_LastIn = nullptr;
    g_LastOut = nullptr;
    g_LastLut = nullptr;

    int result = XFormSampler(InArr, nullptr, (void*)&g_dummyPipeline);

    EXPECT_EQ_INT(TRUE, result);
    EXPECT_PTR_EQ(InArr, g_LastIn);
    EXPECT_PTR_EQ(nullptr, g_LastOut);
    EXPECT_PTR_EQ(&g_dummyPipeline, g_LastLut);
}

// Test 4: Null Cargo (LUT) pointer
// Verifies that a NULL Cargo results in a NULL Lut being passed to cmsPipelineEvalFloat.
void test_null_cargo() {
    const cmsFloat32Number InArr[2] = {1.1f, 2.2f};
    cmsFloat32Number OutArr[2] = {0.0f, 0.0f};

    g_LastIn = nullptr;
    g_LastOut = nullptr;
    g_LastLut = reinterpret_cast<cmsPipeline*>(0xDEADBEEF); // sentinel to ensure we see change if not careful

    int result = XFormSampler(InArr, OutArr, nullptr);

    EXPECT_EQ_INT(TRUE, result);
    EXPECT_PTR_EQ(InArr, g_LastIn);
    EXPECT_PTR_EQ(OutArr, g_LastOut);
    EXPECT_PTR_EQ(nullptr, g_LastLut);
}

// Run all tests
void RunAllTests() {
    test_normal_case();
    test_null_in();
    test_null_out();
    test_null_cargo();
    std::cout << "Tests run: " << gTestCount << ", Failures: " << gFailures << std::endl;
}

// Main entry point
int main() {
    RunAllTests();
    return (gFailures != 0) ? 1 : 0;
}