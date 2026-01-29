// C++11 test harness for the focal C function: testGlobMatch
// This test suite exercises various patterns and strings to verify
// correct handling of '*', '%' wildcards, and '?' single-character wildcards.
// The function under test has C linkage, so we declare it as extern "C".

#include <lsmtest.h>
#include <vector>
#include <sys/time.h>
#include <sqlite3.h>
#include <string>
#include <sys/resource.h>
#include <iostream>


// Declare the focal function with C linkage
extern "C" int testGlobMatch(const char *zPattern, const char *zStr);

// Simple non-terminating test harness (does not exit on failure)
struct TestCase {
    const char* pattern;
    const char* str;
    int expected;      // 1 for match, 0 for no-match
    const char* note;    // optional explanation
};

// Helper to run a single test and print a result
static void RunCase(const TestCase& tc, int index) {
    int result = testGlobMatch(tc.pattern, tc.str);
    bool pass = (result == tc.expected);
    if(pass) {
        // Explanatory note on success (optional)
        std::cout << "[PASS] Case " << index << ": Pattern \"" << tc.pattern
                  << "\" with String \"" << tc.str << "\" -> "
                  << tc.expected << std::endl;
    } else {
        std::cerr << "[FAIL] Case " << index << ": Pattern \"" << tc.pattern
                  << "\" with String \"" << tc.str << "\" -> expected "
                  << tc.expected << ", got " << result
                  << (tc.note ? " // " : "") << (tc.note ? tc.note : "") << std::endl;
    }
}

// Main entry: run a comprehensive suite of test cases
int main() {
    // Step 2: Unit Test Generation
    // Candidate test cases to cover true/false branches, wildcard behavior,
    // end-of-string vs end-of-pattern, and recursive wildcard matching.

    // Comprehensive test cases
    std::vector<TestCase> tests = {
        // Basic empty pattern and string
        { "", "", 1, "Empty pattern should match empty string" },
        { "", "abc", 0, "Empty pattern should not match non-empty string" },

        // Exact match and simple mismatch
        { "abc", "abc", 1, "Exact match should succeed" },
        { "abc", "abx", 0, "Exact mismatch should fail" },

        // Simple wildcard '*' behavior (prefix/suffix)
        { "a*", "a", 1, "Star after 'a' should match empty tail" },
        { "a*", "abcd", 1, "Star after 'a' should match the rest" },
        { "*a", "ba", 1, "Star before 'a' should match preceding chars" },
        { "*", "", 1, "Single '*' should match empty string" },
        { "*", "abc", 1, "Single '*' should match any string" },

        // Question mark wildcard
        { "a?b", "acb", 1, " '?' should match a single character" },
        { "a?b", "aXb", 1, " '?' should match any single character (second test)" },
        { "a?b", "ab", 0, "Pattern longer than string with '?' should fail" },
        { "a?b", "acdb", 0, "Pattern length fixed; extra chars should fail" },

        // Mixed wildcards with multiple characters
        { "ab*cd", "abXcd", 1, "Middle '*' should bridge 'ab' and 'cd'" },
        { "ab*cd", "abcd", 1, " '*' can match empty segment" },

        // Consecutive wildcards and complex matching
        { "a*?c", "abdc", 1, " '*' followed by '?' should align to end with 'c' and one char for '?'" },
        { "a**b", "acccb", 1, "Consecutive '*' should still work (redundant wildcard)" },
        { "a**b", "ab", 1, "Consecutive '*' with minimal match" },

        // End-of-string vs end-of-pattern edge cases
        { "abc", "abcd", 0, "End of pattern reached before string ends should fail" },
        { "abcd", "abc", 0, "End of string reached before pattern ends should fail" },
        { "a*", "a", 1, "End-of-string vs end-of-pattern after wildcard" },

        // Figures with '%' wildcard (alias of '*')
        { "%a", "ba", 0, "Alias '*' variant should behave similarly (prefix tail)"},
        { "%a", "ba", 0, "Alias '%' should mirror '*' behavior in this case" },
        { "%", "", 1, "Single '%' should behave as single wildcard that matches empty string" },
        { "%", "anything", 1, "Single '%' should match any string" },

        // More complex scenario
        { "a*b*c", "axbyc", 1, "Multiple wildcards should combine to match string" },
        { "x*y*z", "xyz", 1, "Wildcards with surrounding literals" },
        { "xyz*", "xyz", 1, "Literal before wildcard should match exact tail" },
        { "*xyz", "abcdefxyz", 1, "Wildcard prefix before literal" }
    };

    // Run all tests
    int index = 1;
    for(const auto& tc : tests) {
        RunCase(tc, index++);
    }

    // Summary
    // In this non-terminating harness we simply report failures and successes.
    // The test program exits with non-zero status only if there were failures.
    // Recompute failure count for a simple exit status.
    // (Note: We recompute by re-evaluating quickly here to avoid storing state.)
    // As we already printed per-case results above, we return 0 for pass-all scenario.
    // To indicate failures, check if any test would fail by re-evaluating or
    // rely on an external CI to parse the per-case output.

    // For a strict exit status, we could re-run quickly:
    int failures = 0;
    for(const auto& tc : tests) {
        int r = testGlobMatch(tc.pattern, tc.str);
        if(r != tc.expected) ++failures;
    }
    if(failures > 0) {
        std::cerr << "Total failures: " << failures << " out of " << tests.size() << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}