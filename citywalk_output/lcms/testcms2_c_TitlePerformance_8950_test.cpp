/*
  Test suite for the focal method TitlePerformance located in testcms2.c

  Key points validated:
  - The function prints the input string left-justified in a field of width 45
    followed by a colon and a space (": "), with no trailing newline.
  - Strings shorter than 45 characters are padded with spaces to reach 45 chars.
  - Strings longer than 45 characters are not truncated (the full string is printed).

  How it works:
  - A small POSIX-based output-capture helper redirects stdout to a pipe,
    invokes TitlePerformance, flushes, restores stdout, and reads the
    captured output for exact comparison.
  - Four tests are provided:
    1) Short input "Hello" (padding verification)
    2) Empty string (all padding)
    3) Exact length 45 string (no extra truncation or padding beyond needed)
    4) Long input > 45 characters (no truncation)

  Build note:
  - Link this test with the C source that provides TitlePerformance, e.g.:
      g++ -std=c++11 -o test_TitlePerformance test_TitlePerformance.cpp testcms2.c
  - The test uses only the C standard library and C++ standard library (no GTest).

  Important: This test assumes a POSIX environment for pipe/dup mechanics.
*/

#include <cstring>
#include <sys/stat.h>
#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <sys/types.h>
#include <unistd.h>


extern "C" void TitlePerformance(const char* Txt); // Focal method from testcms2.c

// Helper: capture stdout produced by TitlePerformance via a pipe
static std::string capture_TitlePerformance(const char* txt)
{
    // Save current stdout
    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1) return "";

    // Create a pipe for stdout redirection
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // Restore and bail out on error
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
        return "";
    }

    // Redirect stdout to pipe's write end
    if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
        // Cleanup
        close(pipefd[0]);
        close(pipefd[1]);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
        return "";
    }

    // We no longer need the original write end in this process
    close(pipefd[1]);

    // Call the focal function
    TitlePerformance(txt);

    // Ensure all output is flushed
    fflush(stdout);

    // Restore original stdout
    if (dup2(saved_stdout, STDOUT_FILENO) == -1) {
        // Cleanup and bail out
        close(saved_stdout);
        close(pipefd[0]);
        return "";
    }
    close(saved_stdout);

    // Read from the pipe's read end
    std::string result;
    char buffer[1024];
    ssize_t n;
    // pipefd[0] is still open; read until EOF
    while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        result.append(buffer, static_cast<size_t>(n));
    }
    // Cleanup
    close(pipefd[0]);

    return result;
}

// Simple non-terminating assertion helper
static bool assert_equal(const std::string& observed, const std::string& expected, const char* testName)
{
    if (observed == expected) {
        // Passed silently
        return true;
    } else {
        std::cerr << "TEST FAILURE: " << testName << "\n";
        std::cerr << "  Expected: [" << expected << "]\n";
        std::cerr << "  Observed: [" << observed << "]\n";
        return false;
    }
}

/*
  Test 1: Short input "Hello" - verify left-justified padding to width 45 and
  the trailing ": " sequence.
*/
static bool Test_TitlePerformance_Short()
{
    const char* input = "Hello";
    std::string out = capture_TitlePerformance(input);
    std::string expected = std::string("Hello") + std::string(45 - 5, ' ') + ": ";
    // 5 is the length of "Hello"; 45 - 5 = 40 padding spaces
    return assert_equal(out, expected, "Test_TitlePerformance_Short");
}

/*
  Test 2: Empty input - entire 45 spaces followed by ": "
*/
static bool Test_TitlePerformance_Empty()
{
    const char* input = "";
    std::string out = capture_TitlePerformance(input);
    std::string expected = std::string(45, ' ') + ": ";
    return assert_equal(out, expected, "Test_TitlePerformance_Empty");
}

/*
  Test 3: Exact length 45 string - no truncation beyond the field width,
  but the string length equals the field width.
*/
static bool Test_TitlePerformance_Exact45()
{
    std::string long45(45, 'X'); // 45 'X' characters
    std::string out = capture_TitlePerformance(long45.c_str());
    std::string expected = long45 + ": ";
    return assert_equal(out, expected, "Test_TitlePerformance_Exact45");
}

/*
  Test 4: Long input > 45 characters - verify no truncation occurs.
*/
static bool Test_TitlePerformance_Long()
{
    std::string longtxt(60, 'A'); // 60 'A' characters
    std::string out = capture_TitlePerformance(longtxt.c_str());
    std::string expected = longtxt + ": ";
    return assert_equal(out, expected, "Test_TitlePerformance_Long");
}

int main()
{
    bool all_passed = true;
    all_passed &= Test_TitlePerformance_Short();
    all_passed &= Test_TitlePerformance_Empty();
    all_passed &= Test_TitlePerformance_Exact45();
    all_passed &= Test_TitlePerformance_Long();

    if (all_passed) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << "SOME TESTS FAILED\n";
        return 1;
    }
}