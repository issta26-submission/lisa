// Test suite for the focal method: skip_line(FILE *f) from etherent.c
// This suite is designed for C++11 compilation, without GoogleTest.
// It uses a non-terminating, per-test report style to maximize code coverage.
// Each test is self-contained and uses tmpfile() to avoid filesystem cleanup concerns.

#include <string.h>
#include <pcap/namedb.h>
#include <nametoaddr.h>
#include <memory.h>
#include <os-proto.h>
#include <thread-local.h>
#include <stdio.h>
#include <config.h>
#include <pcap-int.h>
#include <iostream>
#include <pcap-types.h>


// Declaration of the focal function from the C source.
// Using extern "C" to avoid C++ name mangling issues when linking.
extern "C" int skip_line(FILE *f);

// Helper: run a single test case with given contents and expectations.
// contents: data to be written into a temporary file (as binary).
// expectedReturn: the value returned by skip_line(f) (typically '\\n' or EOF).
// expectedNextChar: the character expected to be readable after the line skipped;
//                   use EOF (-1) if no next character is expected.
// testName: descriptive name for the test case.
static bool run_test_case(const char* testName, const char* contents,
                          int expectedReturn, int expectedNextChar)
{
    // Create a temporary file for the test data.
    FILE* f = tmpfile();
    if (!f) {
        std::cerr << "[ERROR] tmpfile() failed for test: " << testName << std::endl;
        return false;
    }

    // Write contents to the temporary file.
    if (contents && contents[0] != '\0') {
        size_t len = strlen(contents);
        if (fwrite(contents, 1, len, f) != len) {
            std::cerr << "[ERROR] fwrite failed for test: " << testName << std::endl;
            fclose(f);
            return false;
        }
    }

    // Ensure data is written and the stream is at the beginning.
    fflush(f);
    fseek(f, 0, SEEK_SET);

    // Call the focal function.
    int ret = skip_line(f);

    // Read the next character after the line that skip_line consumed.
    int nextChar = fgetc(f);

    // Cleanup.
    fclose(f);

    // Validate results.
    bool ok = (ret == expectedReturn) && (nextChar == expectedNextChar);
    if (!ok) {
        std::cerr << "[FAIL] " << testName
                  << " | Expected ret=" << expectedReturn
                  << ", next=" << expectedNextChar
                  << " | Got ret=" << ret
                  << ", next=" << nextChar << std::endl;
    } else {
        std::cout << "[OK]   " << testName << std::endl;
    }
    return ok;
}

int main() {
    int total = 0;
    int passed = 0;

    // Test 1: Basic line with newline; ensure return is newline and the next char is the following line's first char.
    // Contents: "abc\nDEF"
    // Expectation: skip_line returns '\n', next character after newline is 'D'.
    total++;
    bool t1 = run_test_case("Basic line with newline followed by text",
                            "abc\nDEF",
                            '\n', 'D');
    if (t1) passed++;

    // Test 2: Line without a trailing newline; expect EOF.
    // Contents: "abcdef"
    // Expectation: skip_line returns EOF, no next character.
    total++;
    bool t2 = run_test_case("Line without trailing newline (EOF at end)",
                            "abcdef",
                            EOF, EOF);
    if (t2) passed++;

    // Test 3: Immediate newline at start; first read is '\n', no characters consumed before newline.
    // Contents: "\nabc"
    // Expectation: skip_line returns '\n', next character after newline is 'a'.
    total++;
    bool t3 = run_test_case("Immediate newline at start",
                            "\nabc",
                            '\n', 'a');
    if (t3) passed++;

    // Test 4: Empty file; expect EOF immediately.
    // Contents: ""
    // Expectation: skip_line returns EOF, no next character.
    total++;
    bool t4 = run_test_case("Empty file (no content)",
                            "",
                            EOF, EOF);
    if (t4) passed++;

    // Test 5: Multiple lines; ensure only the first line is skipped and the next line starts with 'l'.
    // Contents: "line1\nline2\n"
    // Expectation: skip_line returns '\n' after the first line; next character is 'l' (start of "line2").
    total++;
    bool t5 = run_test_case("Multiple lines; skip first line only",
                            "line1\nline2\n",
                            '\n', 'l');
    if (t5) passed++;

    // Summary
    std::cout << "Summary: " << passed << "/" << total << " tests passed." << std::endl;
    // Return non-zero if any test failed.
    return (passed == total) ? 0 : 1;
}