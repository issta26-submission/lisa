/*
Unit test suite for the focal method PrintInfo in vprf.c

Assumptions and approach:
- We provide a minimal, self-contained test harness in C++11 that exercises
  the focal function PrintInfo via a mock for cmsGetProfileInfoASCII.
- The mock cmsGetProfileInfoASCII is implemented in this test harness to avoid
  depending on the actual LittleCMS implementation, ensuring deterministic behavior
  and full control over the test scenarios.
- We test:
  1) The branch where the first call returns len == 0, leading to an early return.
  2) The branch where len > 0 and the subsequent text is non-empty, which should
     print the text followed by a newline.
  3) The branch where len > 0 but the text is empty, which should not print anything.
- Output is captured by redirecting stdout to a pipe, to verify the printed content.

Notes:
- The test uses C linkage for cmsGetProfileInfoASCII and PrintInfo to override the
  behavior of the "external" dependency for the duration of the tests.
- The tests avoid terminating on assertion failures by using a non-terminating
  assertion macro (TEST_ASSERT) and report a summary at the end.
- This file should be compiled and linked together with vprf.c in a C++11 capable
  build environment. If your build system uses separate translation units, ensure
  that this test file is linked with vprf.o and that the symbol cmsGetProfileInfoASCII
  resolves to the test harness implementation provided below.

Usage:
- Build with a C++11 capable compiler, linking vprf.c and this test source.
- Run the resulting executable; it will report test outcomes on stderr.

Code below:
*/

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
#include <cassert>
#include <unistd.h>
#include <utils.h>


// Forward declarations of the function under test (as seen by the test harness)
extern "C" void PrintInfo(void* h, int Info);

// Mockable API declaration (signature must match the project's usage)
extern "C" int cmsGetProfileInfoASCII(void* h, int Info, const char* Lang, const char* Country, char* Text, int maxlen);

// Helper function to configure the mock behavior for cmsGetProfileInfoASCII
extern "C" void cmsSetMockProfileInfo(void* h, int Info, const char* text);

// Global state used by the mock CMS function to simulate profile text data
static std::string g_mockText;

// Implementation of the mock setter.
// This function is used by tests to specify what text cmsGetProfileInfoASCII should "return".
extern "C" void cmsSetMockProfileInfo(void* /*h*/, int /*Info*/, const char* text)
{
    g_mockText = text ? text : "";
}

// Mock implementation of cmsGetProfileInfoASCII.
// It simulates a two-call contract used by PrintInfo:
//  - First call with Text == NULL and maxlen == 0 returns the required buffer length (including null terminator).
//  - Second call with a real buffer copies the text into Text and returns number of bytes copied.
extern "C" int cmsGetProfileInfoASCII(void* /*h*/, int /*Info*/, const char* /*Lang*/, const char* /*Country*/, char* Text, int maxlen)
{
    // Access the mock text prepared by cmsSetMockProfileInfo
    if (Text == NULL || maxlen == 0) {
        // Special marker to request length
        // If the test wants to simulate a zero-length result, set g_mockText to "<ZERO_LEN>"
        if (g_mockText == "<ZERO_LEN>") {
            return 0;
        }
        return (int)g_mockText.size() + 1; // includes null terminator
    }

    // Copy up to maxlen-1 characters and terminate
    int copy = std::min((size_t)maxlen - 1, g_mockText.size());
    if (copy > 0) {
        memcpy(Text, g_mockText.c_str(), copy);
    }
    Text[copy] = '\0';
    return copy;
}

// Test harness helpers

// Non-terminating assertion macro
static int g_testFailures = 0;
#define TEST_ASSERT(cond, msg) \
    do { if(!(cond)) { std::cerr << "TEST FAILURE: " << (msg) << " (in " << __func__ << ")\n"; ++g_testFailures; } } while(0)

// Capture stdout output of a given PrintInfo invocation
static std::string captureOutput(void (*fn)(void*, int), void* h, int Info)
{
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        perror("pipe");
        return "";
    }

    // Redirect stdout to pipe
    int savedOut = dup(fileno(stdout));
    if (savedOut == -1) {
        perror("dup");
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        perror("dup2");
        close(pipefd[0]);
        close(pipefd[1]);
        close(savedOut);
        return "";
    }
    close(pipefd[1]); // stdout now goes to pipe

    // Call the function under test
    fn(h, Info);

    // Flush and restore stdout
    fflush(stdout);
    if (dup2(savedOut, fileno(stdout)) == -1) {
        perror("dup2 restore");
    }
    close(savedOut);

    // Read all data from the pipe
    std::string output;
    char buf[4096];
    ssize_t n;
    while ((n = read(pipefd[0], buf, sizeof(buf))) > 0) {
        output.append(buf, buf + n);
    }
    close(pipefd[0]);
    return output;
}

// Individual unit tests

// Test 1: len == 0 should cause an early return with no output
static void test_len_zero()
{
    // Arrange: configure mock to simulate zero-length information
    cmsSetMockProfileInfo((void*)0x3, 1, "<ZERO_LEN>");

    // Act: capture output of PrintInfo
    std::string out = captureOutput(PrintInfo, (void*)0x3, 1);

    // Assert: no output should be produced
    TEST_ASSERT(out.empty(), "PrintInfo should not print anything when len == 0");
}

// Test 2: len > 0 and text is non-empty should print the text followed by a newline
static void test_print_nonempty_text()
{
    // Arrange: configure mock to provide a non-empty string
    cmsSetMockProfileInfo((void*)0x1, 1, "Hello from test");

    // Act
    std::string out = captureOutput(PrintInfo, (void*)0x1, 1);

    // Assert
    TEST_ASSERT(out == "Hello from test\n", "PrintInfo should print the provided non-empty text followed by newline");
}

// Test 3: len > 0 but text is empty should not print anything
static void test_len_positive_empty_text()
{
    // Arrange: mock returns zero-length text
    cmsSetMockProfileInfo((void*)0x2, 1, "");

    // Act
    std::string out = captureOutput(PrintInfo, (void*)0x2, 1);

    // Assert
    TEST_ASSERT(out.empty(), "PrintInfo should not print when the provided text length is zero (but len > 0)");
}

// Test runner
static void runAllTests()
{
    test_len_zero();
    test_print_nonempty_text();
    test_len_positive_empty_text();

    if (g_testFailures == 0) {
        std::cerr << "ALL TESTS PASSED\n";
    } else {
        std::cerr << g_testFailures << " TEST(S) FAILED\n";
    }
}

// Main entry
int main()
{
    runAllTests();
    return g_testFailures == 0 ? 0 : 1;
}

/*
Explanatory notes:
- Test 1 validates the path where cmsGetProfileInfoASCII returns 0 for the requested info,
  causing PrintInfo to exit early without printing anything.
- Test 2 validates the path where the length is positive and the retrieved ASCII text is non-empty.
  PrintInfo should print the text followed by a newline.
- Test 3 validates the path where length is positive but the retrieved text is empty.
  PrintInfo should not print anything in this case.

Implementation details:
- cmsSetMockProfileInfo is provided to configure the mock for cmsGetProfileInfoASCII.
- cmsGetProfileInfoASCII is implemented to honor the two-call contract used by the focal method:
  first call returns the required buffer length; second call fills the buffer.
- The test harness uses a pipe to capture stdout so that we can verify exact printed content
  (including newlines) without modifying the focal method's implementation.
- All assertions are non-terminating; the test suite reports the number of failures at the end.
*/