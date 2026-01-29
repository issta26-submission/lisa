// test_add_identity_matrix.cpp
// Comprehensive unit tests for AddIdentityMatrix in testcms2.c
// This test suite is written to be compiled with C++11, without Google Test.
// It uses a small in-house test harness and the existing Little CMS-style API
// that the focal function relies on (as exposed by testcms2.h and the
// corresponding library during linking).
//
// Note: The test relies on the following (assumed) public API available in the
// project under test. If your environment uses slightly different names or
// signatures, adjust accordingly. The tests focus on behavioral verification
// (no crashes, correct stage insertion), not on internal representation.

#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>


// Bring in C declarations from the project under test.
// We wrap in extern "C" to ensure C linkage for the C headers.
extern "C" {
                       // (Die, DbgThread, cmsPipelineAlloc, cmsPipelineInsertStage, etc.)
}

// If the project exposes additional LittleCMS-style public API for pipelines,
// declare the minimal subset we need to inspect the pipeline in tests.
// These declarations are conservative; they treat types as opaque (we don't access
// internals, only public API to query stage count).
typedef struct cmsPipeline cmsPipeline;
typedef struct cmsStage cmsStage;

extern "C" {
    // Context and basic pipeline/matrix stage creation API
    // (Assumed to exist in the project under test; adjust if your headers expose exact names)
    cmsContext DbgThread(void);
    cmsPipeline* cmsPipelineAlloc(cmsContext ContextID, cmsUInt32Number InputChannels, cmsUInt32Number OutputChannels);
    cmsStage* cmsStageAllocMatrix(cmsContext ContextID, cmsUInt32Number Rows, cmsUInt32Number Columns,
                                  const cmsFloat64Number* Matrix, const cmsFloat64Number* Translation);
    void cmsPipelineInsertStage(cmsPipeline* lut, cmsATEnd, cmsStage* Stage);
    // The actual CMS typedefs/constants may expose cmsAT_END or cmsATEnd as an enum/value.
    // We provide a compatible alias here; if your environment uses a different symbol,
    // replace cmsAT_END with the actual one.
    enum { cmsAT_END = 0 }; // Fallback if not defined in test headers.

    // Pipeline stage traversal (public API in many cms libs)
    cmsStage* cmsPipelineGetFirstStage(const cmsPipeline* lut);
    cmsStage* cmsStageGetNext(const cmsStage* stage);

    // Cleanup (if your environment requires explicit free)
    void cmsPipelineFree(cmsPipeline* lut);
}

// Lightweight test harness
#define TEST_ASSERT(cond, msg)                                      \
    do {                                                              \
        if(!(cond)) {                                                 \
            std::cerr << "FAILED: " << msg << "\n";                   \
            return 1;                                                 \
        }                                                             \
    } while(0)

static int CountStages(const cmsPipeline* lut)
{
    if (!lut) return 0;
    int count = 0;
    cmsStage* s = cmsPipelineGetFirstStage(lut);
    while (s != nullptr) {
        ++count;
        s = cmsStageGetNext(s);
    }
    return count;
}

// Test 1: Verify that AddIdentityMatrix inserts a stage into an initially empty pipeline.
// Rationale: The core behavior we want to validate is that a single identity-matrix
// stage is appended to the LUT when the function is invoked.
static int Test_AddIdentityMatrix_Inserts_One_Stage(void)
{
    // Prepare: a fresh 3x3 to 3x3 pipeline (as used by the focal method)
    cmsPipeline* lut = cmsPipelineAlloc(DbgThread(), 3, 3);
    TEST_ASSERT(lut != nullptr, "Failed to allocate pipeline lut for Test 1");

    // Before: there should be no stages in the empty pipeline
    int beforeCount = CountStages(lut);
    TEST_ASSERT(beforeCount == 0, "Precondition: pipeline must start with 0 stages");

    // Act: invoke the focal method
    AddIdentityMatrix(lut); // Provided by the focal file (testcms2.c)

    // Assert: exactly one stage was added
    int afterCount = CountStages(lut);
    TEST_ASSERT(afterCount == beforeCount + 1,
                "AddIdentityMatrix should insert exactly one stage into an empty LUT");

    // Cleanup
    cmsPipelineFree(lut);

    return 0;
}

// Test 2: Verify that calling AddIdentityMatrix twice results in two stages.
// Rationale: If the function is idempotent per call, repeated invocations
// should cumulatively add stages without crash.
static int Test_AddIdentityMatrix_Multiple_Calls_Increase_Stages(void)
{
    cmsPipeline* lut = cmsPipelineAlloc(DbgThread(), 3, 3);
    TEST_ASSERT(lut != nullptr, "Failed to allocate pipeline lut for Test 2");

    // First call
    AddIdentityMatrix(lut);
    int countAfterFirst = CountStages(lut);
    TEST_ASSERT(countAfterFirst == 1, "After first call, pipeline should have 1 stage");

    // Second call
    AddIdentityMatrix(lut);
    int countAfterSecond = CountStages(lut);
    TEST_ASSERT(countAfterSecond == 2,
                "After second call, pipeline should have 2 stages (incremental)");

    // Cleanup
    cmsPipelineFree(lut);

    return 0;
}

// Test 3: Verify that the pipeline has a stage after AddIdentityMatrix.
// Rationale: A basic sanity check to ensure a stage exists (non-null) after insertion.
static int Test_AddIdentityMatrix_Stage_Not_Null_Post_Insert(void)
{
    cmsPipeline* lut = cmsPipelineAlloc(DbgThread(), 3, 3);
    TEST_ASSERT(lut != nullptr, "Failed to allocate pipeline lut for Test 3");

    // Before: there should be no stages
    cmsStage* firstBefore = cmsPipelineGetFirstStage(lut);
    TEST_ASSERT(firstBefore == nullptr, "Pipeline should be empty before insertion");

    // Insert
    AddIdentityMatrix(lut);

    // After: there should be at least one stage, and the first stage should be non-null
    cmsStage* firstAfter = cmsPipelineGetFirstStage(lut);
    TEST_ASSERT(firstAfter != nullptr, "Pipeline should have a first stage after insertion");

    // Cleanup
    cmsPipelineFree(lut);

    return 0;
}

// Simple driver to run tests from main() (Step 3 requirement)
int main()
{
    // Run tests
    int failures = 0;

    std::vector<std::string> failuresList;

    int r1 = Test_AddIdentityMatrix_Inserts_One_Stage();
    if (r1 != 0) {
        failuresList.push_back("Test_AddIdentityMatrix_Inserts_One_Stage failed");
        ++failures;
    }

    int r2 = Test_AddIdentityMatrix_Multiple_Calls_Increase_Stages();
    if (r2 != 0) {
        failuresList.push_back("Test_AddIdentityMatrix_Multiple_Calls_Increase_Stages failed");
        ++failures;
    }

    int r3 = Test_AddIdentityMatrix_Stage_Not_Null_Post_Insert();
    if (r3 != 0) {
        failuresList.push_back("Test_AddIdentityMatrix_Stage_Not_Null_Post_Insert failed");
        ++failures;
    }

    // Report
    if (failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << failures << " TEST(S) FAILED:\n";
        for (const auto& f : failuresList) {
            std::cerr << " - " << f << "\n";
        }
        return 1;
    }
}

// Explanatory notes for maintainers (embedded as comments):
//
// - The tests focus on observable behavior of AddIdentityMatrix: inserting a stage
//   into a pipeline. Since internal stage representations are opaque to the public API,
//   we verify by counting the number of stages before and after insertion, and by
//   asserting non-null stage presence after insertion.
// - Test 1 ensures a single insertion into an initially empty pipeline.
// - Test 2 checks that multiple invocations accumulate stages (non-idempotent per-call).
// - Test 3 confirms that a stage is present after AddIdentityMatrix is invoked.
// - We intentionally avoid private member access and rely solely on public (or test-provided)
//   APIs to inspect the pipeline structure.
//
// Domain knowledge notes applied:
// - Identity matrix in Little CMS is a 3x4 matrix (3 outputs, 3 inputs, 12 elements) as used
//   by the focal method (Identity array in the source corresponds to a 3x4 identity).
// - The test uses the same public entry points (DbgThread, cmsPipelineAlloc, cmsPipelineInsertStage,
//   cmsStageAllocMatrix) as the focal code to minimize divergence from production usage.
// - The tests are designed to be executable in environments where only standard C++ and the
//   project-provided test harness headers (testcms2.h) are available, without GTest.
// - Static members and internal static helpers within the focal module are not used directly;
//   tests rely on public API boundaries per the guidance.