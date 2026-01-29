// Lightweight unit test suite for LabCLUTAlloc without using GTest.
// This test suite uses a small in-process test harness with non-terminating assertions.
// It relies on the project providing the declarations in fast_float_internal.h
// and will mock the underlying allocation function _cmsMallocZero to simulate success/failure.

#include <fast_float_internal.h>
#include <vector>
#include <cstddef>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>


// Forward-declare the dependencies as provided by the production code.
// Include the header that declares LabCLUTAlloc and related types.
// The actual project should provide the correct paths; adjust if necessary.

// Global test state to simulate allocation behavior and collect failures.
static bool gFailNextMalloc = false;  // If true, the next allocation should fail (simulate NULL).
static std::vector<std::string> gFailures;  // Collect non-terminating failures for reporting.

// Mock implementation to override the production _cmsMallocZero used inside LabCLUTAlloc.
// This mock enables deterministic testing by toggling failure mode.
extern "C" void* _cmsMallocZero(cmsContext ContextID, size_t Size)
{
    // Simulate allocation failure when requested.
    if (gFailNextMalloc) {
        gFailNextMalloc = false; // Reset for subsequent calls.
        return NULL;
    }
    // Allocate zero-initialized memory to mimic _cmsMallocZero behavior.
    void* p = std::calloc(1, Size);
    return p;
}

// Helper: record a non-terminating failure message.
static void recordFailure(const std::string& msg)
{
    gFailures.push_back(msg);
}

// Test 1: Success path for LabCLUTAlloc
// - Allocates LabCLUTdata successfully
// - Verifies ContextID and p fields are assigned correctly
// - Verifies that sigmoid tables have been initialized (basic sanity: finite values)
static void test_LabCLUTAlloc_success()
{
    // Description for readability in output
    const char* testName = "test_LabCLUTAlloc_success";

    // Prepare input
    cmsContext testCtx = static_cast<cmsContext>(0x1234); // arbitrary non-zero value
    const cmsInterpParams* testP = reinterpret_cast<const cmsInterpParams*>(0xDEADBEEF);

    // Ensure we are not simulating allocation failure this time
    gFailNextMalloc = false;

    // Call the function under test
    LabCLUTdata* fd = LabCLUTAlloc(testCtx, testP);

    if (fd == NULL) {
        recordFailure(std::string(testName) + ": LabCLUTAlloc returned NULL on success path.");
        return;
    }

    // Validate that ContextID and p were assigned correctly
    if (fd->ContextID != testCtx) {
        recordFailure(std::string(testName) + ": ContextID mismatch. Expected " +
                      std::to_string(static_cast<long long>(testCtx)) + " got " +
                      std::to_string(static_cast<long long>(fd->ContextID)));
    }

    if (fd->p != testP) {
        recordFailure(std::string(testName) + ": p pointer mismatch. Expected " +
                      std::to_string(reinterpret_cast<uintptr_t>(testP)) + " got " +
                      std::to_string(reinterpret_cast<uintptr_t>(fd->p)));
    }

    // Basic sanity check on initialized sigmoid tables.
    // We assume tabulateSigmoid fills fd->sigmoidIn and fd->sigmoidOut with finite numbers.
    // If the layout is different, this will catch obvious issues early.
    if (fd->sigmoidIn == nullptr || fd->sigmoidOut == nullptr) {
        recordFailure(std::string(testName) + ": sigmoid tables were not allocated properly (null pointers).");
    } else {
        // Check a few representative elements for finiteness.
        // Access the first element if SIGMOID_POINTS > 0; otherwise skip.
        // Guard against potential misdefinition of SIGMOID_POINTS by verifying count > 0 via first element.
        if (SIGMOID_POINTS > 0) {
            if (!std::isfinite(fd->sigmoidIn[0])) {
                recordFailure(std::string(testName) + ": sigmoidIn[0] is not finite.");
            }
            if (!std::isfinite(fd->sigmoidOut[0])) {
                recordFailure(std::string(testName) + ": sigmoidOut[0] is not finite.");
            }
        } else {
            recordFailure(std::string(testName) + ": SIGMOID_POINTS <= 0, cannot validate sigmoid tables.");
        }
    }

    // Note: LabCLUTFree is effectively a no-op in this codebase (empty body). If needed, invoke to mirror lifecycle.
    // Clean-up is not strictly necessary here as allocation lifetime extends beyond this test.
}

// Test 2: Failure path for LabCLUTAlloc when memory allocation fails
// - Simulates _cmsMallocZero returning NULL
// - Verifies that LabCLUTAlloc returns NULL and does not crash
static void test_LabCLUTAlloc_failure()
{
    const char* testName = "test_LabCLUTAlloc_failure";

    // Prepare input
    cmsContext testCtx = static_cast<cmsContext>(0x5678);
    const cmsInterpParams* testP = nullptr;

    // Trigger allocation failure on next _cmsMallocZero call
    gFailNextMalloc = true;  // Next allocation attempt should fail

    LabCLUTdata* fd = LabCLUTAlloc(testCtx, testP);

    if (fd != NULL) {
        recordFailure(std::string(testName) + ": LabCLUTAlloc should return NULL on allocation failure.");
        // If not NULL, don't attempt to free as we don't know the allocator behavior in this path.
    }
}

// Simple test runner to execute all tests and report results.
// This approach matches the constraint of not using GTest and relies on a small custom harness.
static void run_all_tests()
{
    test_LabCLUTAlloc_success();
    test_LabCLUTAlloc_failure();
}

// Entry point
int main()
{
    // Run tests
    run_all_tests();

    // Report results
    if (gFailures.empty()) {
        std::cout << "[TESTS PASSED] All LabCLUTAlloc tests succeeded." << std::endl;
        return 0;
    } else {
        std::cout << "[TESTS FAILED] LabCLUTAlloc test failures:" << std::endl;
        for (const auto& msg : gFailures) {
            std::cout << " - " << msg << std::endl;
        }
        return 1;
    }
}