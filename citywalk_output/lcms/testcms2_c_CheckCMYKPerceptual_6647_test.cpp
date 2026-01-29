// Comprehensive unit test suite for the focal method
// Step 1 (Program Understanding):
// - Focal method under test: cmsInt32Number CheckCMYKPerceptual(void)
// - It simply forwards to: CheckCMYK(INTENT_PERCEPTUAL, "test1.icc", "test2.icc")
// - Core dependencies: CheckCMYK(), INTENT_PERCEPTUAL, and ICC profile file names "test1.icc"/"test2.icc"
// - Candidate Keywords: CheckCMYKPerceptual, CheckCMYK, INTENT_PERCEPTUAL, "test1.icc", "test2.icc"

// Step 2 (Unit Test Generation):
// - We will build a small C++11 test harness that includes the project test header
//   (testcms2.h) to obtain the correct declarations (types, constants, and function prototypes).
// - The tests will:
//   1) Verify that CheckCMYKPerceptual() returns the same value as a direct call to
//      CheckCMYK(INTENT_PERCEPTUAL, "test1.icc", "test2.icc").
//   2) Verify idempotence: multiple consecutive calls to CheckCMYKPerceptual() yield the same result.
// - The test suite uses simple non-terminating assertions: it prints PASS/FAIL messages
//   and continues execution (no GTest required as requested).

// Step 3 (Test Case Refinement):
// - The tests rely on the provided testcms2.h header so all symbols (types and constants)
//   are resolved by the compiler and linker in the target environment.
// - We provide explanatory comments for each unit test to clarify intent and
//   the Candidate Keywords being exercised.

#include <iostream>
#include <string>
#include <testcms2.h>


// Include the project's test header to obtain the correct declarations.
// This header is expected to declare cmsInt32Number, INTENT_PERCEPTUAL, and the
// focal function CheckCMYKPerceptual(), as well as CheckCMYK().
extern "C" {
}

// Helper macros to keep test output uniform and non-terminating
#define LOG_PASS(msg)  do { std::cout << "[PASS] " << msg << std::endl; } while(0)
#define LOG_FAIL(msg)  do { std::cerr << "[FAIL] " << msg << std::endl; } while(0)

// Test 1: Validate that CheckCMYKPerceptual() returns the same value as a direct call
// to CheckCMYK(INTENT_PERCEPTUAL, "test1.icc", "test2.icc").
// Rationale: CheckCMYKPerceptual is a thin wrapper around this call; consistency confirms
// the wrapper's correctness in forwarding the parameters.
bool test_CheckCMYKPerceptual_Equals_Direct_CheckCMYK() {
    // Focal method under test
    cmsInt32Number resultPerceptual = CheckCMYKPerceptual();

    // Direct invocation using the same parameters
    cmsInt32Number resultDirect = CheckCMYK(INTENT_PERCEPTUAL, "test1.icc", "test2.icc");

    if (resultPerceptual == resultDirect) {
        LOG_PASS("CheckCMYKPerceptual() equals direct CheckCMYK(INTENT_PERCEPTUAL, 'test1.icc', 'test2.icc')");
        return true;
    } else {
        LOG_FAIL("CheckCMYKPerceptual() does not equal direct CheckCMYK(...)");
        return false;
    }
}

// Test 2: Idempotence / stability test
// Rationale: Ensure repeated invocations of CheckCMYKPerceptual() yield identical results,
// indicating no internal state mutation that could affect subsequent calls.
bool test_CheckCMYKPerceptual_Idempotence() {
    cmsInt32Number first = CheckCMYKPerceptual();
    cmsInt32Number second = CheckCMYKPerceptual();

    if (first == second) {
        LOG_PASS("CheckCMYKPerceptual() is idempotent across multiple invocations");
        return true;
    } else {
        LOG_FAIL("CheckCMYKPerceptual() produced different results on successive calls");
        return false;
    }
}

// Main runner
int main(int argc, char* argv[]) {
    // Note: The tests rely on test1.icc and test2.icc presence in the running directory
    // as used by the focal function CheckCMYKPerceptual() and the direct call below.

    // Step 2: Execute test cases
    int total = 0;
    int passed = 0;

    // Run Test 1
    total++;
    if (test_CheckCMYKPerceptual_Equals_Direct_CheckCMYK()) {
        passed++;
    }

    // Run Test 2
    total++;
    if (test_CheckCMYKPerceptual_Idempotence()) {
        passed++;
    }

    // Summary
    std::cout << "\nTest Summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Return non-zero if any test failed to aid quick diagnosis when run in scripts
    return (passed == total) ? 0 : 1;
}