// Lightweight C++11 test harness for the focal method HandleSwitches (wtpt.c)
// This test suite uses a minimal, non-GTest approach with simple, non-terminating checks.
// It assumes the project provides the following C symbols (via extern "C"):
//   - void HandleSwitches(int argc, char *argv[]);
//   - int lShowLab, lShowLCh, lShowXYZ;           // global flags used by the focal method
//   - int xgetopt(int argc, char *argv[], const char *optstring); // option parser
//   - void FatalError(const char*);                 // error reporter used by default case
//
// The test validates the behavior of HandleSwitches with different argv inputs,
// ensuring the core option handling ('l', 'c', 'x') toggles the corresponding flags.
// It does not attempt to mock private/private static members or private methods, as per the
// constraints. Static/global state is accessed via extern, and we only read/write the
// globals that are defined in the implementation under test.

#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <cassert>
#include <utils.h>


// Bring the focal function into scope (C linkage to match the project's symbol)
extern "C" void HandleSwitches(int argc, char *argv[]);

// Shared global flags from the focal module (extern to use the project-defined instances)
extern "C" int lShowLab;
extern "C" int lShowLCh;
extern "C" int lShowXYZ;

// xgetopt and FatalError are provided by the project (we do not implement them here)
extern "C" int xgetopt(int argc, char *argv[], const char *optstring);
extern "C" void FatalError(const char*);

// Simple, non-terminating assertion helper.
// It records failures but continues execution to maximize coverage.
struct SimpleAssert {
    int failures = 0;

    template <typename T, typename U>
    void expect_eq(const T& a, const U& b, const char* expr) {
        if (!(a == b)) {
            ++failures;
            std::cerr << "Assertion failed: " << expr
                      << " (actual: " << a << ", expected: " << b << ")\n";
        }
    }

    template <typename T, typename U>
    void assert_true(const T& a, const char* expr) {
        expect_eq<bool, bool>(static_cast<bool>(a), true, expr);
    }

    template <typename T, typename U>
    void assert_false(const T& a, const char* expr) {
        expect_eq<bool, bool>(static_cast<bool>(a), false, expr);
    }

    bool ok() const { return failures == 0; }
};

// Utility to run a single test case.
// It resets the global flags to a known baseline, invokes HandleSwitches with provided argv,
// and compares resulting flag values against expected ones.
void runTestCase(const std::string& testName,
                 const std::vector<std::string>& args,
                 int expectedLab, int expectedLCh, int expectedXYZ,
                 SimpleAssert& sa)
{
    // Reset globals to well-defined baseline before each test
    // Baseline: lab = 0, lCh = 0, XYZ = 1 (as a neutral starting point)
    // We assume the actual project defines these as ints (not bools) compatible with 0/1.
    lShowLab = 0;
    lShowLCh = 0;
    lShowXYZ = 1;

    // Build argv array compatible with C-style function signature
    // We'll allocate a contiguous array of C-strings backed by std::string storage.
    std::vector<std::string> argStrings;
    argStrings.reserve(args.size() + 1);
    argStrings.push_back("prog"); // program name placeholder
    for (const auto& s : args) {
        argStrings.push_back(s);
    }

    int argc = static_cast<int>(argStrings.size());
    // Create non-const char* array for argv (as required by the signature)
    std::vector<char*> argvPtrs;
    argvPtrs.reserve(argc);
    for (auto& s : argStrings) {
        // const_cast is used because xgetopt and HandleSwitches expect non-const char*
        argvPtrs.push_back(const_cast<char*>(s.data()));
    }

    // Invoke the focal method under test
    HandleSwitches(argc, argvPtrs.data());

    // Validate outcomes
    sa.expect_eq("lShowLab", lShowLab, expectedLab);
    sa.expect_eq("lShowLCh", lShowLCh, expectedLCh);
    sa.expect_eq("lShowXYZ", lShowXYZ, expectedXYZ);
}

// Entry point for the test suite
int main() {
    SimpleAssert sa;

    // Domain knowledge alignment notes:
    // - Each test focuses on one or more flags toggled by respective options 'l', 'c', 'x'.
    // - 'l' should set lShowLab to TRUE (non-zero); 'c' should set lShowLCh to TRUE; 'x'
    //   should set lShowXYZ to FALSE (0).
    // - We assume the default baseline is lShowLab=0, lShowLCh=0, lShowXYZ=1 before test.
    // - Tests should also cover a combination of options in a single invocation.

    // Test 1: Only -l is provided
    // Expected: lShowLab becomes TRUE (non-zero), other flags unchanged (Lab=1, LCh=0, XYZ=1)
    runTestCase("Test 1: only -l",
                { "-l" },
                1 /* Lab expected */, 0 /* LCh expected */, 1 /* XYZ expected */, sa);

    // Test 2: Only -c is provided
    // Expected: lShowLCh becomes TRUE, others unchanged (Lab=0, LCh=1, XYZ=1)
    runTestCase("Test 2: only -c",
                { "-c" },
                0, 1, 1, sa);

    // Test 3: Only -x is provided
    // Expected: lShowXYZ becomes FALSE, Lab and LCh remain 0 (Lab=0, LCh=0, XYZ=0)
    runTestCase("Test 3: only -x",
                { "-x" },
                0, 0, 0, sa);

    // Test 4: Combination -l and -c
    // Expected: Lab=1, LCh=1, XYZ remains 1
    runTestCase("Test 4: -l and -c",
                { "-l", "-c" },
                1, 1, 1, sa);

    // Test 5: No arguments (default behavior)
    // Expected: Lab=0, LCh=0, XYZ=1 (baseline preserved)
    runTestCase("Test 5: no args",
                { },
                0, 0, 1, sa);

    // Summary
    if (sa.ok()) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cerr << "Some tests FAILED. Failures: " << sa.failures << "\n";
        return 1;
    }
}

// Note on domain knowledge usage:
// - The tests are designed to maximize coverage of the option-switching logic by asserting
//   outcomes for each individual and combined option scenario.
// - We rely on the project's implementation of xgetopt and FatalError; tests avoid triggering FatalError
//   by not including unknown options. If an unknown option is added, FatalError would be invoked;
//   this test suite does not mock FatalError to avoid altering the original behavior, but a separate
//   test could be added if a safe, non-terminating FatalError is provided by the project.