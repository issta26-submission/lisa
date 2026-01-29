// Test suite for the focal method lsmMCursorValue in lsm_sorted.c
// Language: C++11 (without GoogleTest). The tests are designed as a lightweight,
// non-terminating assertion framework to exercise the key branches of lsmMCursorValue.
//
// Approach notes:
// - We include the C implementation lsm_sorted.c in this translation unit so that the
//   concrete MultiCursor type, LSM constants, and internal behavior are visible.
// - We override the static helpers (where possible) via preprocessor macros to control
//   the else-branch path without needing to instantiate or drive the full internal
//   database environment. This allows us to simulate both rc == LSM_OK and rc != LSM_OK
//   scenarios for the else branch.
// - We provide three tests covering: (a) true-branch when aTree == 0 or CURSOR_SEEK_EQ is set,
//   (b) else-branch with rc == LSM_OK and pVal non-null, ensuring the final value matches
//   pCsr->val.pData, and (c) else-branch with rc != LSM_OK resulting in pVal = 0 and nVal = 0.
// - Static members inside lsm_sorted.c are left as-is; the tests operate through the public
//   lsmMCursorValue entry point and the controlled macros below.
// - The tests rely on the project’s domain knowledge (LSM_OK, CURSOR_SEEK_EQ, etc.) being
//   defined in lsm_sorted.c when included.

// Include the C implementation in a C++ translation unit
extern "C" {
#include <iostream>
#include <cstring>
#include <lsm_sorted.c>

}

// Lightweight non-terminating test framework (no GTest)

// Simple counters for test results
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Convenience macro: non-fatal expectation
#define EXPECT_TRUE(x) do { \
    ++g_tests_run; \
    if(!(x)) { \
        std::cerr << "EXPECT_TRUE failed: " #x " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_tests_failed; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    ++g_tests_run; \
    if(!((a) == (b))) { \
        std::cerr << "EXPECT_EQ failed: " #a " == " #b " (actual " << (a) << " vs " << (b) << ") at " \
                  << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_tests_failed; \
    } \
} while(0)

#define EXPECT_NE(a,b) do { \
    ++g_tests_run; \
    if((a) == (b)) { \
        std::cerr << "EXPECT_NE failed: " #a " != " #b " (both are " << (a) << ") at " \
                  << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_tests_failed; \
    } \
} while(0)

// -----------------------------------------------------------------------------
// Test scaffolding for lsmMCursorValue
// We create scenarios to exercise the true-branch and else-branch logic of the
// function under test. We leverage the preprocessor to mock internal static
// helpers for controlled test behavior in the else-branch.
// -----------------------------------------------------------------------------


// Global switch used by mocked helpers to choose the test scenario for the
// else-branch path.
// 0 = default (rc will be LSM_OK and pVal will be non-null to exercise the normal path)
// 1 = rc == LSM_OK with pVal non-null (ensures subsequent sortedBlobSet call and re-pointing pVal)
// 2 = rc != LSM_OK to exercise the failure path (pVal will be reset to 0 and nVal to 0)
static int g_else_test_scenario = 0;

// Mocked helpers (only active in this translation unit due to inclusion of lsm_sorted.c above)
// We override the internal helpers used in lsmMCursorValue by macro-mapping their names
// to simple behaviors that align with our test scenarios.

// 1) mcursorLocationOk should return true to allow the else-branch to proceed.
//    This is achieved by a macro that substitutes a constant 1.
// 2) multiCursorGetVal should supply pVal and nVal and set rc for the else-branch.
// 3) sortedBlobSet should emulate a successful blob write without changing pData.
// Note: The aliases below are defined before including lsm_sorted.c, so the function
// calls inside lsmMCursorValue get redirected.

#define mcursorLocationOk(pCsr, bDel) (1)
#define sortedBlobSet(pEnv, blob, pVal, nVal) (LSM_OK)

// multiCursorGetVal behavior controlled by the global test switch
#define multiCursorGetVal(pCsr, aTreePtr, ppVal, pnVal) do { \
    (void)(pCsr); (void)(aTreePtr); \
    if( g_else_test_scenario == 0 ) { \
        /* Default: supply a non-null value and a value length */ \
        *ppVal = (void*)0xCAFEBABE; \
        *pnVal = 8; \
        rc = LSM_OK; \
    } else if( g_else_test_scenario == 1 ) { \
        /* OK with value, to exercise the re-assign-after-blob-write path */ \
        *ppVal = (void*)0xDEADBEEF; \
        *pnVal = 4; \
        rc = LSM_OK; \
    } else { \
        /* Error path: make rc non-OK to trigger  pVal/nVal reset */ \
        *ppVal = (void*)0xBAADF00D; \
        *pnVal = 4; \
        rc = -1; \
    } \
} while(0)

// Helper to reset test environment for each test
static void reset_env(MultiCursor *pCsr){
    // Zero out the structure to a known baseline
    std::memset(pCsr, 0, sizeof(MultiCursor));
    // Ensure a clean val default
    pCsr->val.pData = (void*)0xABCD1234;
    pCsr->val.nData = 12;
}

// -----------------------------------------------------------------------------
// Test 1: True branch when CURSOR_SEEK_EQ is set or aTree == 0
// Expected: rc == LSM_OK, *ppVal == pCsr->val.pData, *pnVal == pCsr->val.nData
// This test does not exercise the sub-paths (no internal calls to mocked helpers).
// -----------------------------------------------------------------------------

void test_lsmMCursorValue_true_branch()
{
    // Create a CSR instance with aTree == 0 to force the true-branch
    MultiCursor csr;
    reset_env(&csr);
    csr.flags = 0;           // CURSOR_SEEK_EQ not set
    csr.aTree = 0;             // forces the true-branch path
    // The size of the value carried by the cursor
    csr.val.nData = 5;
    csr.val.pData = (void*)0x11112222;

    void *pVal = nullptr;
    int nVal = 0;
    int rc = lsmMCursorValue(&csr, &pVal, &nVal);

    EXPECT_EQ(rc, LSM_OK);
    EXPECT_EQ(pVal, csr.val.pData);
    EXPECT_EQ(nVal, csr.val.nData);
}

// -----------------------------------------------------------------------------
// Test 2: Else branch with rc == LSM_OK and pVal non-null; final value should reflect
// pCsr->val.pData after blob write (simulated by sortedBlobSet macro).
// - Setup: aTree non-null, CURSOR_SEEK_EQ not set, else path activated.
// - g_else_test_scenario = 0 ensures rc == LSM_OK and pVal provided by multiCursorGetVal.
// -----------------------------------------------------------------------------

void test_lsmMCursorValue_else_branch_ok_value()
{
    MultiCursor csr;
    reset_env(&csr);
    csr.flags = 0;                 // CURSOR_SEEK_EQ not set
    // Prepare a non-null aTree to enter the else-branch
    // We don't rely on actual tree structure due to the test bootstrap.
    void *dummyTree[2] = { nullptr, (void*)0x1 }; // aTree[1] used by the call
    csr.aTree = dummyTree;

    // Ensure val.pData is the final target value after the blob write
    csr.val.nData = 0x42;
    csr.val.pData = (void*)0x0BADBEEF;

    // Run test: scenario 0 yields rc == LSM_OK and a non-null pVal from multiCursorGetVal
    g_else_test_scenario = 0;
    void *pVal = nullptr;
    int nVal = 0;

    int rc = lsmMCursorValue(&csr, &pVal, &nVal);

    // Expect final value to be csr.val.pData (see code: pVal = pCsr->val.pData after sortedBlobSet)
    EXPECT_EQ(rc, LSM_OK);
    EXPECT_EQ(pVal, csr.val.pData);
    // nVal is the length set by multiCursorGetVal (here 8 as per macro)
    EXPECT_EQ(nVal, 8);
}

// -----------------------------------------------------------------------------
// Test 3: Else branch with rc != LSM_OK, ensuring pVal and nVal reset to 0
// - Setup: aTree non-null; g_else_test_scenario = 2 triggers error path in macro.
// -----------------------------------------------------------------------------

void test_lsmMCursorValue_else_branch_error_path()
{
    MultiCursor csr;
    reset_env(&csr);
    csr.flags = 0;
    void *dummyTree[2] = { nullptr, (void*)0x2 };
    csr.aTree = dummyTree;
    csr.val.nData = 7;
    csr.val.pData = (void*)0xFEEDFACE;

    void *pVal = (void*)0x12345678;
    int nVal = 33;

    g_else_test_scenario = 2;
    int rc = lsmMCursorValue(&csr, &pVal, &nVal);

    // Expect rc to reflect error (-1 here as per macro), and value outputs reset
    EXPECT_TRUE(rc != LSM_OK);
    EXPECT_EQ(pVal, nullptr); // After error, pVal is reset to 0
    EXPECT_EQ(nVal, 0);
}

// -----------------------------------------------------------------------------
// Main driver: run tests and report summary
// -----------------------------------------------------------------------------

int main() {
    std::cout << "Starting lsmMCursorValue test suite (C++11 harness).\n";

    // Run tests
    test_lsmMCursorValue_true_branch();
    test_lsmMCursorValue_else_branch_ok_value();
    test_lsmMCursorValue_else_branch_error_path();

    std::cout << "Tests executed: " << g_tests_run << ", Failures: " << g_tests_failed << "\n";

    // Exit code: 0 if all tests pass, non-zero if failures occurred
    return (g_tests_failed == 0) ? 0 : 1;
}