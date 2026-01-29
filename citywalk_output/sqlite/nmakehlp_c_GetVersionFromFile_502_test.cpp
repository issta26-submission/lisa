// This is a C++11 test harness for the C-style function GetVersionFromFile
// under test. The tests are designed to be portable and do not rely on GTest.
// All tests are executed from main() as requested in the domain knowledge.

/*
Notes on the focal method (GetVersionFromFile):
- It opens a file and scans line by line for the given match string.
- Once found, it skips non-digit characters to the first digit after the match.
- It then consumes characters that are digits, '.', 'a', or 'b' under a
  constraint controlled by numdots (to limit the number of '.' or 'a'/'b').
- It terminates the extracted version string in-place in a static buffer and
  returns a pointer to that buffer.
- If the file cannot be opened or the match is not found, NULL is returned.
- The returned string points to a static buffer and may be overwritten by
  a subsequent call; tests copy the content immediately if needed.
*/

#include <math.h>
#include <windows.h>
#include <vector>
#include <cstdio>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>


// Declaration for the focal function (assumed to be linked from nmakehlp.c)
extern "C" char* GetVersionFromFile(const char* filename, const char* match, int numdots);

// Simple non-terminating test harness (no asserts that abort execution)
static int g_total_tests = 0;
static int g_failures = 0;
static std::vector<std::string> g_logs;

static void log_pass(const std::string& msg) {
    g_logs.push_back("PASS: " + msg);
}
static void log_fail(const std::string& msg) {
    g_logs.push_back("FAIL: " + msg);
    ++g_failures;
}
static void check(bool cond, const std::string& message) {
    if (cond) log_pass(message);
    else log_fail(message);
    // Do not abort; continue with next checks to maximize coverage
}

// Helper: Write test content to a file and return its path
static std::string writeTestFile(int idx, const std::string& content) {
    std::string path = "test_getver_" + std::to_string(idx) + ".txt";
    std::ofstream ofs(path.c_str(), std::ios::binary);
    ofs << content;
    ofs.close();
    return path;
}

// Helper: Call the focal function and return a copied std::string (safe to reuse)
static std::string GetVersionFromFileStr(const std::string& filename,
                                       const std::string& match,
                                       int numdots) {
    char* res = GetVersionFromFile(filename.c_str(), match.c_str(), numdots);
    if (res == NULL) return "";
    return std::string(res);
}

// Candidates (Step 1): Key components observed in GetVersionFromFile
// - File I/O (fopen, fgets, fclose)
// - String search (strstr) and digit parsing (isdigit)
// - Pointer arithmetic to locate the version substring
// - History/side-effects via a static buffer (szBuffer) and return pointer
// - Handling of numdots to limit the count of '.' or 'a'/'b' characters

// Step 2: Unit tests for GetVersionFromFile (non-GTest, inline in main)

/*
Test 1: Non-existent file
- Expect NULL result since fopen fails.
*/
static void test_no_file_exists() {
    std::string path = "nonexistent_file_for_test_1.txt";
    char* res = GetVersionFromFile(path.c_str(), "MATCH:", 2);
    check(res == NULL, "GetVersionFromFile returns NULL when the file does not exist");
}

/*
Test 2: No match string present in file
- Create a file with content that does not contain the match substring.
- Expect NULL result.
*/
static void test_no_match_in_file() {
    std::string content = "Line one\nLine two without the needed token\nEnd.";
    std::string path = writeTestFile(2, content);

    char* res = GetVersionFromFile(path.c_str(), "MATCH:", 2);
    check(res == NULL, "GetVersionFromFile returns NULL when match string is absent");
}

/*
Test 3: Match found with digits only after the match
- Content: "MATCH: 42"
- Expect "42"
*/
static void test_match_digits_only() {
    std::string content = "Intro\nMATCH: 42\nEnd";
    std::string path = writeTestFile(3, content);

    std::string ver = GetVersionFromFileStr(path, "MATCH:", 2);
    check(ver == "42", "GetVersionFromFile extracts digits after match: expected '42'");
}

/*
Test 4: Match with digits and dots (no ab), respecting numdots limit
- Content: "MATCH: 7.8.9"
- Expect "7.8.9" when numdots >= 2 (two dots allowed)
*/
static void test_match_with_dots_allowed() {
    std::string content = "Header\nMATCH: 7.8.9\nFooter";
    std::string path = writeTestFile(4, content);

    std::string ver = GetVersionFromFileStr(path, "MATCH:", 2);
    // Should include up to the second dot; third dot would stop if more present
    // Here we have exactly two dots; expect full "7.8.9"
    check(ver == "7.8.9", "GetVersionFromFile extracts version with allowed dots: '7.8.9'");
}

/*
Test 5: Match with letters a/b (e.g., 'a'/'b' allowed)
- Content: "MATCH: 1.2ab3x"
- Expect "1.2ab3" (stops before non-allowed 'x')
*/
static void test_match_with_ab_letters() {
    std::string content = "Info\nMATCH: 1.2ab3x\nMore";
    std::string path = writeTestFile(5, content);

    std::string ver = GetVersionFromFileStr(path, "MATCH:", 2);
    check(ver == "1.2ab3", "GetVersionFromFile handles 'a'/'b' characters within version: '1.2ab3'");
}

/*
Test 6: Match on the first line (verification of early exit)
- Content: line1 contains MATCH, line2 contains other data
- Expect the first-line match to be returned
*/
static void test_match_on_first_line() {
    std::string content = "MATCH: 123\nNext line data";
    std::string path = writeTestFile(6, content);

    std::string ver = GetVersionFromFileStr(path, "MATCH:", 2);
    check(ver == "123", "GetVersionFromFile finds match on first line: '123'");
}

/*
Test 7: Multi-line scanning (match not on the very first line)
- Ensure scanning continues across lines until a match is found
- Content: line1 without match, line2 contains the match
*/
static void test_multiline_scan() {
    std::string content = "First line\nSecond line with MATCH: 9.99\nEnd";
    std::string path = writeTestFile(7, content);

    std::string ver = GetVersionFromFileStr(path, "MATCH:", 2);
    check(ver == "9.99", "GetVersionFromFile scans lines until match is found in a later line");
}

// Main entry: execute all tests and print a summary (Step 3)
int main() {
    // Reset counters for a clean run
    g_total_tests = 0;
    g_failures = 0;
    g_logs.clear();

    // Run tests (each function performs its own checks and logs results)
    test_no_file_exists();
    test_no_match_in_file();
    test_match_digits_only();
    test_match_with_dots_allowed();
    test_match_with_ab_letters();
    test_match_on_first_line();
    test_multiline_scan();

    // Print detailed logs
    std::cout << "Test execution complete. Details:\n";
    for (const auto& s : g_logs) {
        std::cout << s << "\n";
    }
    std::cout << "Summary: total=" << g_total_tests
              << ", failures=" << g_failures << "\n";

    // Return non-zero if any test failed
    return g_failures > 0 ? 1 : 0;
}