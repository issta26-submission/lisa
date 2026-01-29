/*
  Unit test suite for the focal method Say(const char* str) found in testcms2.h/testcms2.c context.

  - Strategy:
    1) Redirect stdout to a temporary file for each test case, invoke Say, then read the file content to verify the exact bytes printed.
    2) Cover multiple input scenarios: simple string, empty string, string with spaces, and a long string.
    3) Use a lightweight, non-terminating test harness (no gtest) that reports results via std::cerr.
    4) Do not rely on private data or private methods. Use provided Say() from the codebase.
    5) Ensure C++11 compatibility.

  - Notes:
    The header/test harness provided (testcms2.h) contains the Say implementation inside the header scope. We include it to exercise Say directly. The tests here redirect stdout to files to capture output without terminating the test run, and then compare the captured content to the expected strings.
*/

#include <cstring>
#include <cstdio>
#include <iterator>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <cstdlib>


// Include the focal class dependencies and the Say function to be tested.
// The Say implementation is provided in testcms2.h as per the problem statement.

// Utility to capture the output of Say by redirecting stdout to a file.
// Returns the exact bytes written to stdout (no extra newline or spaces added by us).
static std::string captureSayOutput(const std::string& input) {
    // Use a per-call output file to avoid cross-test contamination
    static int s_case = 0;
    ++s_case;
    std::string filename = "say_output_" + std::to_string(s_case) + ".txt";

    // Ensure any previous file with the same name is removed
    std::remove(filename.c_str());

    // Redirect stdout to the file
    if (freopen(filename.c_str(), "w", stdout) == nullptr) {
        // If redirection fails, return empty content to indicate failure
        return "";
    }

    // Invoke the focal method
    Say(input.c_str());

    // Ensure content is flushed to the file
    fflush(stdout);

    // Close stdout so the file is finalized and can be read reliably
    fclose(stdout);

    // Read the file content back for verification
    std::ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary);
    if (!ifs) {
        return "";
    }
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        std::istreambuf_iterator<char>());
    return content;
}

/*
  Test 1: Verify that Say prints a simple ASCII string exactly as provided.
  Purpose: Basic, canonical path coverage of the printing behavior.
*/
static bool test_Say_SimpleString() {
    std::string input = "Hello";
    std::string output = captureSayOutput(input);
    return output == input;
}

/*
  Test 2: Verify that Say handles an empty string correctly.
  Purpose: Ensure no extraneous characters are emitted for zero-length input.
*/
static bool test_Say_EmptyString() {
    std::string input = "";
    std::string output = captureSayOutput(input);
    return output == input;
}

/*
  Test 3: Verify that Say preserves spaces and prints the string exactly as provided.
  Purpose: Validate that internal spaces are not collapsed or altered.
*/
static bool test_Say_Spaces() {
    std::string input = "Hello World with spaces";
    std::string output = captureSayOutput(input);
    return output == input;
}

/*
  Test 4: Verify that Say can handle a long string without truncation.
  Purpose: Ensure robustness for larger inputs.
*/
static bool test_Say_LongString() {
    std::string input(1024, 'A'); // 1024 'A' characters
    std::string output = captureSayOutput(input);
    return output == input;
}

/*
  Lightweight test harness:
  - Runs a fixed set of Say tests.
  - Reports results to std::cerr (not stdout) to avoid clobbering test output when stdout is redirected.
  - Returns non-zero if any test fails; otherwise 0.
*/
int main() {
    using std::cerr;
    int total = 0;
    int passed = 0;

    struct TestCase {
        const char* name;
        bool (*fn)();
    };

    const TestCase tests[] = {
        {"Say_SimpleString", test_Say_SimpleString},
        {"Say_EmptyString",  test_Say_EmptyString},
        {"Say_Spaces",       test_Say_Spaces},
        {"Say_LongString",     test_Say_LongString},
    };

    const int kTestCount = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < kTestCount; ++i) {
        bool ok = tests[i].fn();
        ++total;
        if (ok) ++passed;
        cerr << (ok ? "[PASS] " : "[FAIL] ") << tests[i].name << "\n";
    }

    cerr << "Total=" << total << " Passed=" << passed << "\n";

    return (passed == total) ? 0 : 1;
}