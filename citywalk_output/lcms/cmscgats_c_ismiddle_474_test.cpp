// Test suite for cmsBool ismiddle(int c) in cmscgats.c
// This test suite is designed for C++11 without GoogleTest.
// It uses a lightweight, non-terminating assertion mechanism.
// The focal function ismiddle relies on isseparator(int c).
// We declare the function with C linkage to match the original C symbol.

#include <lcms2_internal.h>
#include <iostream>
#include <string>


// Step 1 (Conceptual): Candidate Keywords representing core dependencies
// - isseparator(int): external dependency used by ismiddle's predicate
// - c: input character under test
// - '#', '\"', '\'': characters explicitly excluded by ismiddle
// - 32, 127 boundary: ASCII range checks (>32 and <127)
// - cmsBool / int: return type compatibility (treated as int in C++ test)

// External function under test (C linkage)
extern "C" int ismiddle(int c);

// Lightweight test harness
namespace TestHarness {

    // Simple assertion helper (non-terminating): returns true if ok, false otherwise
    // We intentionally avoid terminating the program on first failure to maximize coverage.
    bool assert_bool(const std::string& testName, bool condition, int& failures) {
        if (condition) {
            std::cout << "[PASS] " << testName << "\n";
            return true;
        } else {
            std::cout << "[FAIL] " << testName << "\n";
            ++failures;
            return false;
        }
    }

    // Run a single test case
    void runCase(int input, bool expected, const std::string& name, int& failures) {
        bool actual = (iscmiddle(input) != 0); // Convert non-zero to true, zero to false
        // Explanation for each case is provided in test names
        assert_bool(name, actual == expected, failures);
    }

    // Note: We intentionally compute actual by treating non-zero as true,
    // which aligns with typical cmsBool semantics (often typedef int).

}

// Since cmsBool is typically an int in liblcms (and ismiddle returns cmsBool),
 // we declare a small wrapper to avoid C++ name mangling in tests.
// Using a thin macro-like adapter is unnecessary; we rely on extern "C" linkage for ismiddle.

int main() {
    int failures = 0;

    // Step 2: Test Suite for ismiddle
    // Coverage plan:
    // - True branch: values clearly inside the ASCII printable range and not equal to excluded chars
    // - False branch: explicit excluded chars ('#', '\"', '\'')
    // - False branch: boundary values at 32 (space) and 127 (DEL)
    // - False branch: negative values and values likely affected by isseparator
    // - True branch: upper boundary value 126 ('~') within range

    // 1) c = 'A' (65): expected true if isseparator(65) is false (typical for alphanumerics)
    TestHarness::runCase( 'A', true, "iscmiddle('A') should be true when isseparator('A') is false", failures);

    // 2) c = '!' (33): likely to be a separator; test expects false IF isseparator(33) is true
    TestHarness::runCase( '!', false, "iscmiddle('!') expected false if isseparator('!') is true", failures);

    // 3) c = '#' (35): explicitly excluded by ismiddle (c != '#')
    TestHarness::runCase( '#', false, "iscmiddle('#') always false due to explicit exclusion", failures);

    // 4) c = '"' (34): explicitly excluded by ismiddle (c != '\"')
    TestHarness::runCase( '\"', false, "iscmiddle('\"') always false due to explicit exclusion", failures);

    // 5) c = '\'' (39): explicitly excluded by ismiddle (c != '\'')
    TestHarness::runCase( '\'', false, "iscmiddle('\\'') always false due to explicit exclusion", failures);

    // 6) c = ' ' (32): boundary below allowed range; should be false
    TestHarness::runCase( ' ', false, "iscmiddle(' ') false due to c > 32 requirement", failures);

    // 7) c = '~' (126): upper printable ASCII; likely true if isseparator(126) is false
    TestHarness::runCase( '~', true, "iscmiddle('~') true when isseparator('~') is false", failures);

    // 8) c = '!' (33) again to observe consistency; explicit expectation depends on isseparator
    TestHarness::runCase( '!', false, "iscmiddle('!') consistency check (repeat case)", failures);

    // 9) c = -5: negative value; should be false due to c > 32
    TestHarness::runCase( -5, false, "iscmiddle(-5) false due to c > 32 requirement", failures);

    // 10) c = 126 (already tested as '~'), extra boundary check: 127 should be false
    TestHarness::runCase( 127, false, "iscmiddle(127) false due to c < 127 requirement", failures);

    // Summary
    if (failures == 0) {
        std::cout << "\nAll tests passed.\n";
    } else {
        std::cout << "\nTotal failures: " << failures << "\n";
    }

    // Return non-zero if any test failed to signal issues in automated environments
    return failures == 0 ? 0 : 1;
}

// Explanation notes for each test case are embedded in their test descriptions above.
// The tests rely on the external ismiddle implementation available at link time.
// They aim to cover true/false branches as described in Step 3 of the methodology.