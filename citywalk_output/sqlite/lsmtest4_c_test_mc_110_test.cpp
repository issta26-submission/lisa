// C++11 test harness for the focal method test_mc in lsmtest4.c
// This harness uses the existing C test infrastructure (lsmtest.h) and
// calls test_mc indirectly. It avoids GTest and relies on non-terminating
//, lightweight checks with descriptive comments for each test case.

#include <iostream>
#include <lsmtest.h>
#include <string>


// Bring in the C declarations for the testing framework.
// The C headers expose testCaseBegin, testCaseFinish, ArraySize, Mctest, etc.
extern "C" {
}

// Declares the focal function from the C source so we can invoke it from C++.
extern "C" void test_mc(const char *zSystem, const char *zPattern, int *pRc);

// Simple non-terminating assertion helpers (not using a test framework).
// They print results and do not exit, enabling multiple tests in a single run.
static int s_total = 0;
static int s_pass = 0;

// Run a single test case by invoking test_mc with given parameters.
// Returns true if the test is considered a pass (rc == 0), false otherwise.
static bool runOneTest(const std::string& zSystem,
                       const std::string& zPattern,
                       const std::string& label)
{
    int rc = 0;

    // Call the focal method from the C source.
    // The function is expected to mutate rc via the pRc pointer.
    test_mc(zSystem.c_str(), zPattern.c_str(), &rc);

    // Non-terminating assertion: consider rc == 0 as a success path.
    bool ok = (rc == 0);

    // Informative output to aid debugging and coverage verification.
    std::cout << "[TEST] " << label
              << " | zSystem=" << zSystem
              << " | zPattern=" << zPattern
              << " | rc=" << rc
              << " | result=" << (ok ? "PASS" : "FAIL") << std::endl;

    ++s_total;
    if (ok) ++s_pass;
    return ok;
}

int main() {
    // Test 1: Pattern matches a single, known test case.
    // This exercises the true-branch of the internal condition predicate.
    // Expects that at least one test case is executed for the given system/pattern.
    runOneTest("sqlite", "mc1.sqlite.0", "mc1.sqlite.0 (pattern match)");

    // Test 2: Pattern that matches no test case.
    // This exercises the false-branch where the inner loop is skipped.
    // We still expect the function to return normally without crash.
    runOneTest("sqlite", "nomatch", "mc1.no_match (pattern no-match)");

    // Test 3: Different system name to exercise formatting and multiple system handling.
    // Ensures the test harness covers another system value for the same "i" index.
    runOneTest("postgres", "mc1.postgres.0", "mc1.postgres.0 (different system)");

    // Summary of results
    std::cout << "\nSUMMARY: " << s_pass << " / " << s_total << " tests passed." << std::endl;

    // If at least one test ran and all reported PASS, return 0; otherwise non-zero.
    return (s_total > 0 && s_pass == s_total) ? 0 : 1;
}