/*
Unit Test Suite for PrintSupportedIntents (in testcms2.c)

Overview:
- We test the focal method PrintSupportedIntents without relying on Google Test.
- Since PrintSupportedIntents delegates to cmsGetSupportedIntents to retrieve the supported intents,
  we provide a lightweight stub for cmsGetSupportedIntents that we can manipulate from tests.
- We capture stdout output produced by printf inside PrintSupportedIntents to verify correct behavior.
- We cover both non-empty and empty datasets to exercise potential branches in the loop.
- Tests are written in C++11, do not rely on external testing frameworks, and use a small, non-terminating assertion style.

Notes about dependencies:
- The test suite provides a C-compatible linkage stub for cmsGetSupportedIntents.
- The focal method is PrintSupportedIntents(void); we declare it with C linkage to ensure linkage compatibility.
- We avoid touching private/state details of any class; we only rely on the public function PrintSupportedIntents and the provided dataset mechanism.

Important: This test harness is designed to be compiled in a C++11 project alongside testcms2.c. It provides:
- A simple stdout capture utility (via POSIX pipe) to verify printed output.
- A minimal dataset mechanism to drive cmsGetSupportedIntents.
- Two tests: non-empty dataset and empty dataset.

Code follows.
*/

#include <cstring>
#include <cstdio>
#include <functional>
#include <vector>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <unistd.h>


// Forward declaration of the focal function (C linkage to match the original)
extern "C" void PrintSupportedIntents(void);

// ------------------ Stubbed cmsGetSupportedIntents (C linkage) ------------------
// We provide a stub so that tests can control the data PrintSupportedIntents prints.

typedef unsigned cmsUInt32Number;

// Storage for test datasets
static cmsUInt32Number gDatasetN = 0;
static cmsUInt32Number gDatasetCodes[200];
static const char* gDatasetDescriptions[200];

// Setter to inject a dataset for tests
static void SetDataset(unsigned n, const cmsUInt32Number codes[], const char* descriptions[]) {
    gDatasetN = n;
    for (unsigned i = 0; i < 200; ++i) {
        gDatasetCodes[i] = 0;
        gDatasetDescriptions[i] = nullptr;
    }
    for (unsigned i = 0; i < n; ++i) {
        gDatasetCodes[i] = codes[i];
        gDatasetDescriptions[i] = descriptions[i];
    }
}

// The actual cmsGetSupportedIntents signature from the focal code
extern "C" cmsUInt32Number cmsGetSupportedIntents(cmsUInt32Number nMax, cmsUInt32Number Codes[], char* Descriptions[]) {
    unsigned n = static_cast<unsigned>(gDatasetN);
    if (n > nMax) n = nMax;
    for (unsigned i = 0; i < n; ++i) {
        Codes[i] = gDatasetCodes[i];
        Descriptions[i] = (char*)gDatasetDescriptions[i]; // cast away constness for compatibility
    }
    return static_cast<cmsUInt32Number>(n);
}

// ------------------ stdout capture helper ------------------

static std::string CaptureStdout(std::function<void()> fn) {
    // Create a pipe to capture stdout
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        return "";
    }

    // Save original stdout
    int savedFd = dup(fileno(stdout));
    if (savedFd == -1) {
        // cleanup
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stdout to the write end of the pipe
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        // cleanup
        close(pipefd[0]);
        close(pipefd[1]);
        close(savedFd);
        return "";
    }
    // Close write end in this process after redirect
    close(pipefd[1]);

    // Run the function that prints to stdout
    fn();

    // Flush and restore stdout
    fflush(stdout);
    if (dup2(savedFd, fileno(stdout)) == -1) {
        // fallback: still read what was written
    }
    close(savedFd);

    // Read captured output
    std::string output;
    char buffer[4096];
    ssize_t len;
    // Read until EOF
    while ((len = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(len));
    }
    close(pipefd[0]);

    return output;
}

// ------------------ Test framework (lightweight) ------------------

static int g_failures = 0;
static int g_total    = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total; \
    if (cond) { \
        printf("[PASS] %s\n", msg); \
    } else { \
        printf("[FAIL] %s\n", msg); \
        ++g_failures; \
    } \
} while(0)

static void RunAllTests();

// ------------------ Tests for PrintSupportedIntents ------------------

/*
Test 1: Non-empty dataset
- Setup a dataset with 3 entries.
- Capture stdout of PrintSupportedIntents.
- Verify header is printed and that all codes and descriptions are printed in the expected format.
*/
static void Test_PrintSupportedIntents_NonEmpty() {
    // Arrange
    cmsUInt32Number codes[3] = { 1, 2, 3 };
    const char* descs[3] = { "First", "Second", "Third" };
    SetDataset(3, codes, descs);

    // Act
    std::string out = CaptureStdout([]() { PrintSupportedIntents(); });

    // Assert
    EXPECT_TRUE(out.find("Supported intents:\n") != std::string::npos,
                "PrintSupportedIntents prints header 'Supported intents:'");
    EXPECT_TRUE(out.find("\t1 - First\n") != std::string::npos,
                "PrintSupportedIntents prints first code-description line");
    EXPECT_TRUE(out.find("\t2 - Second\n") != std::string::npos,
                "PrintSupportedIntents prints second code-description line");
    EXPECT_TRUE(out.find("\t3 - Third\n") != std::string::npos,
                "PrintSupportedIntents prints third code-description line");
}

/*
Test 2: Empty dataset
- Setup an empty dataset.
- Capture stdout and verify that the function outputs the header followed by a blank line (as per implementation).
*/
static void Test_PrintSupportedIntents_Empty() {
    // Arrange
    SetDataset(0, nullptr, nullptr);

    // Act
    std::string out = CaptureStdout([]() { PrintSupportedIntents(); });

    // Assert
    // Expected: header plus an additional blank line
    EXPECT_TRUE(out == "Supported intents:\n\n",
                "PrintSupportedIntents prints header followed by an empty line when no intents are supported");
}

// ------------------ Test runner implementation ------------------

static void RunAllTests() {
    printf("Running unit tests for PrintSupportedIntents...\n");
    Test_PrintSupportedIntents_NonEmpty();
    Test_PrintSupportedIntents_Empty();
    printf("Tests completed: %d ran, %d failed.\n", g_total, g_failures);
}

// ------------------ Main entry ------------------

int main() {
    RunAllTests();
    // Return non-zero if any test failed
    return (g_failures != 0) ? 1 : 0;
}