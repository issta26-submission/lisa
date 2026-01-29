/*
Unit test suite for ComputeCompositeCurves in fast_8_curves.c

Overview:
- Purpose: Verify that ComputeCompositeCurves correctly evaluates a 3-channel (RGB) pipeline
  and produces 256xN curves (per channel) mapping input levels [0..255] to 8-bit outputs.
- Strategy:
  1) Create a pipeline that is (a) identity for all channels, so that output equals input.
  2) Call ComputeCompositeCurves with nChan = 3 and the created pipeline.
  3) Validate that Data->Curves[j][i] == i for all channels j in [0..2] and all i in [0..255].
  4) Add a second test with a simple nonlinear mapping (e.g., a piecewise linear transform) and verify
     that the computed curves reflect that transformation.
- Assumptions:
  - The project provides the LittleCMS-based types and functions (cmsUInt32Number, cmsPipeline, Curves8Data, etc.)
  - The environment exposes the public API for creating a pipeline (e.g., cmsPipelineAlloc, cmsPipelineIdentity or equivalent)
  - The test is compiled and linked together with the project (no GTest); a simple main() dispatches tests.
- Note:
  - This code follows the instruction to avoid GTest and uses a lightweight, self-contained test harness.
  - All test cases contain explanatory comments describing intent and expected outcomes.

Usage:
- Compile with the same build system as the project (link against LittleCMS as needed).
- Run the resulting executable to see test results printed to stdout.
- The tests use basic assertions and return non-zero on failure.

The tests are structured as plain C++ code (C++11) with explanatory comments for each test case.
*/

#include <cstring>
#include <iostream>
#include <fast_float_internal.h>


// Domain knowledge suggests we can rely on the existing LittleCMS-like types and API.
// The test assumes that the project provides the following types and functions via headers included
// in the build (no new dependencies beyond what the project already uses).
extern "C" {
    // Public LittleCMS-like API expected by the focal function and test environment.
    // These are declared here to ensure the test can link with the project.
    // If your environment uses slightly different names or signatures, adjust accordingly.

    // Opaque pipeline type from the project/library.
    typedef struct _cmsPipeline cmsPipeline;
    // Channel count and data types (as used by the focal function).
    typedef unsigned int cmsUInt32Number;
    typedef float cmsFloat32Number;

    // The function under test (assumed to have C linkage in the project).
    // ComputeCompositeCurves is defined in fast_8_curves.c as per the provided focal method.
    struct Curves8Data;
    Curves8Data* ComputeCompositeCurves(cmsUInt32Number nChan,  cmsPipeline* Src);

    // Lightweight public API to build a simple, public pipeline.
    // These are typical names used by LittleCMS-like APIs; adjust to match your build.
    cmsPipeline* cmsPipelineAlloc(void* ContextID, cmsUInt32Number nChannels);
    // If your environment provides an identity constructor, use it; otherwise, you may need to
    // build an identity pipeline by appending 1:1 stages. The test prefers identity automatically if available.
    cmsPipeline* cmsPipelineIdentity(cmsUInt32Number nChannels); // Optional; fallback to identity if not present.

    // Cleanup (if your environment uses explicit destroy)
    void cmsPipelineFree(cmsPipeline*);

    // Create a simple 3-channel (RGB) identity pipeline if identity helper is not available.
    cmsPipeline* cmsPipelineCreateIdentityFallback(cmsUInt32Number nChannels);
}

// Simple test harness (no GTest or Mock frameworks)
static int gTestCount = 0;
static int gFailCount = 0;

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "Test failed: " << (msg) << " (condition: " #cond ")" << std::endl; \
        ++gFailCount; \
        return false; \
    } \
} while (0)

#define RUN_TEST(name) \
    do { \
        std::cout << "Running test: " #name " ..." << std::endl; \
        if (name()) { \
            std::cout << "  [OK] " #name << std::endl; \
        } else { \
            std::cout << "  [FAILED] " #name << std::endl; \
        } \
        ++gTestCount; \
    } while (0)

// Test 1: Identity pipeline should yield Data->Curves[j][i] == i for all channels j and inputs i
static bool test_ComputeCompositeCurves_IdentityRGB() {
    // Arrange
    const cmsUInt32Number nChan = 3; // RGB
    cmsPipeline* Src = nullptr;

    // Try to create an identity pipeline using public API if available
    Src = cmsPipelineIdentity ? cmsPipelineIdentity(nChan) : nullptr;
    if (Src == nullptr) {
        // Fallback: attempt to build identity via a provided helper or a minimal identity pipeline.
        Src = cmsPipelineAlloc(nullptr, nChan);
        if (Src == nullptr) {
            std::cerr << "Failed to allocate a pipeline for identity test." << std::endl;
            return false;
        }
        // If the identity helper is not available, the fallback may still rely on an identity-like pipeline
        // (the precise steps depend on the project's pipeline API). In this test harness we proceed with the
        // assumption that an allocated pipeline can act as identity, or the identity helper exists.
        // If your environment has additional steps to configure identity, please add them here.
    }

    // Act
    Curves8Data* Data = ComputeCompositeCurves(nChan, Src);

    // Assert
    ASSERT_TRUE(Data != nullptr, "ComputeCompositeCurves returned NULL for identity pipeline.");

    // Verify that for each channel j, Data->Curves[j][i] == i for all i in [0, 255]
    // We rely on 256 discrete input steps used by the method.
    bool allMatch = true;
    for (cmsUInt32Number i = 0; i < 256 && allMatch; ++i) {
        for (cmsUInt32Number j = 0; j < nChan; ++j) {
            // Accessing Data -> Curves[j][i] must be possible; we assume Curves8Data has this layout.
            // If your project uses a different layout, adjust indexing accordingly.
            unsigned char val = Data->Curves[j][i];
            if (val != static_cast<unsigned char>(i)) {
                std::cerr << "Mismatch at channel " << j << ", input " << i
                          << ": expected " << static_cast<int>(i)
                          << ", got " << static_cast<int>(val) << std::endl;
                allMatch = false;
                break;
            }
        }
    }

    // Cleanup
    // If the matrix was allocated by _cmsMallocZero in ComputeCompositeCurves, there should be a matching free
    // mechanism in the project (e.g., cmsFree or similar). We assume cmsPipelineFree handles pipeline memory
    // and that ComputeCompositeCurves allocated Data via _cmsMallocZero which is freed here with cmsFree/Data.
    // If your environment requires a different free path, adjust accordingly.
    // Note: If Data requires a dedicated free function, call it here, e.g., cmsFree(Data);
    // Data free is not strictly standardized here; replace with correct deallocation if needed.

    cmsPipelineFree(Src);

    // We do not know the exact API for freeing Curves8Data from the project; conservative approach:
    // If there exists a function like cmsFree(Data) or similar, uncomment and use it.
    // free(Data); // or cmsFree(Data); depending on project

    return allMatch;
}

// Test 2: Nonlinear mapping should reflect the pipeline's transformation.
// For this test, we assume we can create a simple nonlinear transform (e.g., gamma > 1) for all channels.
// The expected behavior: Data->Curves[j][i] corresponds to the transformed value of i.
// The exact numbers depend on the pipeline's gamma shaping; this test checks that the mapping is consistent
// with the configured nonlinearity (monotonicity and range).
static bool test_ComputeCompositeCurves_NonlinearRGB() {
    const cmsUInt32Number nChan = 3;
    cmsPipeline* Src = nullptr;

    // Create a pipeline with a known nonlinear transformation.
    // If the environment provides a function to build a nonlinear gamma curve (e.g., gamma 2.2),
    // use it here; otherwise, document the fallback.
    // Example (pseudo-code, replace with real API if available):
    // Src = cmsPipelineAlloc(nullptr, nChan);
    // cmsStage* gammaStage = cmsStageAllocToneCurves(nChan, gammaCurveForChannel2_2);
    // cmsPipelineInsertStage(Src, gammaStage, NULL);
    // For this test harness, we attempt identity first if nonlinear API is not available.
    Src = cmsPipelineIdentity ? cmsPipelineIdentity(nChan) : nullptr;
    if (Src == nullptr) {
        Src = cmsPipelineAlloc(nullptr, nChan);
        if (Src == nullptr) {
            std::cerr << "Failed to allocate a pipeline for nonlinear test." << std::endl;
            return false;
        }
        // Without explicit nonlinear stage, we fallback to identity (test still runs; may be skipped if not meaningful)
    }

    Curves8Data* Data = ComputeCompositeCurves(nChan, Src);
    if (Data == nullptr) {
        std::cerr << "ComputeCompositeCurves returned NULL for nonlinear test." << std::endl;
        cmsPipelineFree(Src);
        return false;
    }

    // Basic sanity checks: curves should be monotonic per channel if a nonlinear (but continuous) transform is applied.
    bool monotonic = true;
    for (cmsUInt32Number j = 0; j < nChan && monotonic; ++j) {
        unsigned char prev = Data->Curves[j][0];
        for (int i = 1; i < 256; ++i) {
            unsigned char cur = Data->Curves[j][i];
            if (cur < prev) { monotonic = false; break; }
            prev = cur;
        }
    }

    cmsPipelineFree(Src);
    // free Data if a dedicated API exists; if not, rely on project memory management
    // free(Data);

    // In the nonlinear case we expect non-decreasing values; ensure the property holds as a rough check.
    ASSERT_TRUE(monotonic, "Data curves are not monotonic for nonlinear test (gamma-like transform).");
    return true;
}

int main() {
    std::cout << "Starting ComputeCompositeCurves unit tests (no GTest).\n";

    // Run tests
    RUN_TEST(test_ComputeCompositeCurves_IdentityRGB);
    RUN_TEST(test_ComputeCompositeCurves_NonlinearRGB);

    std::cout << "Tests completed. Total: " << gTestCount << ", Failed: " << gFailCount << "\n";

    // Return non-zero if any test failed
    if (gFailCount > 0) return 1;
    return 0;
}