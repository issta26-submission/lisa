// Unit test suite for cmsStage* _cmsStageNormalizeFromLabFloat(cmsContext ContextID)
// This test suite targets the focal method described in the provided source.
// It avoids GTest and uses a lightweight in-project test harness with non-terminating assertions.
// The tests are designed to be compiled and run in a C++11 environment and rely on the
// LittleCMS (lcms2) API available in the project.

#include <limits>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Forward declarations for the focal function and related helper used in tests.
// These functions are defined in the project (cmslut.c) and are normally provided through ld/ms linker.
// We declare them with C linkage to avoid name mangling issues when linking from C++.
extern "C" {
    // Primary function under test: creates a 3x3 matrix-based stage mapping Lab to a float PCS
    cmsStage* _cmsStageNormalizeFromLabFloat(cmsContext ContextID);

    // Helper used in tests to apply the matrix stage to an input (In) and obtain Out
    void EvaluateMatrix(const cmsFloat32Number In[],
                        cmsFloat32Number Out[],
                        const cmsStage *mpe);
}

// Include core LittleCMS headers to ensure type definitions (cmsStage, cmsContext, cmsSigLab2FloatPCS, etc.) are available.
// Adjust the include path if your project places lcms2 headers in a custom location.

/* Lightweight test harness
   - Non-terminating assertions: tests record failures but continue execution.
   - After running all tests, the program prints a summary and returns a non-zero exit code on any failure.
*/

// Simple assertion macro that does not abort on failure.
// It prints detailed message and increments a global failure counter.
static int g_failures = 0;
#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << msg << std::endl; \
        ++g_failures; \
    } else { \
        std::cout << "[PASS] " << msg << std::endl; \
    } \
} while (0)

// Helper: compare two floating numbers with tolerance
static bool almost_equal_float(float a, float b, float tol = 1e-5f) {
    return std::fabs(a - b) <= tol;
}

// Test 1: Verify that _cmsStageNormalizeFromLabFloat returns a non-null stage,
// that the Implements field is Lab-to-Float PCS, and that a simple matrix evaluation
// matches the expected 3x3 matrix and offset (a1 and o1 constants in the function).
static void test_NormalizeFromLabFloat_ReturnsStageAndMatrixMatches() {
    std::cout << "Running Test 1: NormalizeFromLabFloat returns valid stage and matrix matches." << std::endl;

    // Act: Call the focal function with a NULL/default context
    cmsStage* stage = _cmsStageNormalizeFromLabFloat(nullptr);
    TEST_ASSERT(stage != nullptr, "Stage should be non-NULL when memory allocation succeeds.");

    if (stage != nullptr) {
        // Assert: Implements should be Lab to Float PCS
        TEST_ASSERT(stage->Implements == cmsSigLab2FloatPCS,
                    "Stage Implements should be cmsSigLab2FloatPCS.");

        // Prepare an input vector and apply the matrix stage with EvaluateMatrix
        cmsFloat32Number In[3] = { 1.0f, 2.0f, 3.0f };
        cmsFloat32Number Out[3] = { 0.0f, 0.0f, 0.0f };

        EvaluateMatrix(In, Out, stage);

        // Expected results based on the constants in _cmsStageNormalizeFromLabFloat:
        // a1 = [ [1/100, 0, 0],
        //        [0, 1/255, 0],
        //        [0, 0, 1/255] ]
        // o1 = [0, 128/255, 128/255]
        // Out = a1 * In + o1
        // Out[0] = (1/100)*1 + 0 = 0.01
        // Out[1] = (1/255)*2 + 128/255 ≈ 0.0078431373 + 0.5019607843 ≈ 0.5098039216
        // Out[2] = (1/255)*3 + 128/255 ≈ 0.0117647059 + 0.5019607843 ≈ 0.5137254902

        const float exp0 = 0.01f;
        const float exp1 = 0.5098039216f;
        const float exp2 = 0.5137254902f;

        TEST_ASSERT(almost_equal_float(Out[0], exp0),
                    "EvaluateMatrix() should produce Out[0] ≈ 0.01");
        TEST_ASSERT(almost_equal_float(Out[1], exp1),
                    "EvaluateMatrix() should produce Out[1] ≈ 0.5098039216");
        TEST_ASSERT(almost_equal_float(Out[2], exp2),
                    "EvaluateMatrix() should produce Out[2] ≈ 0.5137254902");
    }

    // Cleanup: Free the allocated stage if possible to avoid memory leaks.
    // The project usually exposes a corresponding free function, e.g., cmsStageFree.
    // We attempt to free gracefully if available.
    if (stage != nullptr) {
        // Best effort: some builds provide cmsStageFree or cmsPipelineFree, adapt as needed.
        // If unavailable, we ignore to avoid compilation errors.
        #if defined(CMS_STAGE_FREE) && CMS_STAGE_FREE
        cmsStageFree(stage);
        #endif
        // If no macro is defined, we skip freeing to keep test portable.
    }

    std::cout << "Test 1 completed." << std::endl << std::endl;
}

// Test 2: Validate that _cmsStageNormalizeFromLabFloat can handle a second input and
// that zero input yields the offset terms (o1) as the output for each channel.
// This reinforces that the offset component is correctly incorporated.
static void test_NormalizeFromLabFloat_OffsetOnlyInput() {
    std::cout << "Running Test 2: NormalizeFromLabFloat offset behavior with zero input." << std::endl;

    cmsStage* stage = _cmsStageNormalizeFromLabFloat(nullptr);
    TEST_ASSERT(stage != nullptr, "Stage should be non-NULL for offset test.");

    if (stage != nullptr) {
        // In = {0, 0, 0} should produce Out = o1 = {0, 128/255, 128/255}
        cmsFloat32Number In[3] = { 0.0f, 0.0f, 0.0f };
        cmsFloat32Number Out[3] = { 0.0f, 0.0f, 0.0f };

        EvaluateMatrix(In, Out, stage);

        const float exp0 = 0.0f;
        const float exp1 = 128.0f / 255.0f;
        const float exp2 = 128.0f / 255.0f;

        TEST_ASSERT(almost_equal_float(Out[0], exp0),
                    "Offset test: Out[0] should be 0.0");
        TEST_ASSERT(almost_equal_float(Out[1], exp1),
                    "Offset test: Out[1] should be 128/255 ≈ 0.501960784");
        TEST_ASSERT(almost_equal_float(Out[2], exp2),
                    "Offset test: Out[2] should be 128/255 ≈ 0.501960784");
    }

    // Cleanup (as in Test 1)
    if (stage != nullptr) {
        #if defined(CMS_STAGE_FREE) && CMS_STAGE_FREE
        cmsStageFree(stage);
        #endif
    }

    std::cout << "Test 2 completed." << std::endl << std::endl;
}

// Main: Run all tests
int main() {
    std::cout << "Starting cmsStageNormalizeFromLabFloat unit tests (C++11, no GTest)." << std::endl << std::endl;

    // Run individual tests
    test_NormalizeFromLabFloat_ReturnsStageAndMatrixMatches();
    test_NormalizeFromLabFloat_OffsetOnlyInput();

    // Summary
    if (g_failures == 0) {
        std::cout << "[SUMMARY] ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "[SUMMARY] " << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}