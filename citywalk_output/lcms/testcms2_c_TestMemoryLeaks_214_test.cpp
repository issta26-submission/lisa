// C++11 test suite for the focal method: TestMemoryLeaks in testcms2.c
// The tests run in-process, capture stdout produced by the C code (printf) and
// compare it against expected outputs. We use a minimal, non-terminating test
// harness (no GTest) as requested.

#include <cstring>
#include <cstdio>
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <sys/types.h>
#include <unistd.h>


// Include the C headers. Ensure C linkage for functions defined in testcms2.c.
extern "C" {
}

// Ensure stdout is unbuffered for deterministic captures
static void makeStdoutUnbuffered() {
    setvbuf(stdout, NULL, _IONBF, 0);
}

// Capture stdout produced during the execution of func.
// Uses a POSIX pipe to redirect stdout to a buffer.
static std::string captureOutput(const std::function<void()>& func) {
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // If pipe creation fails, fallback to empty output to avoid crash.
        return std::string();
    }

    // Save current stdout
    int stdoutSave = dup(fileno(stdout));
    // Redirect stdout to pipe write end
    fflush(stdout);
    dup2(pipefd[1], fileno(stdout));
    close(pipefd[1]);

    // Run the function that will write to stdout
    func();

    // Flush and restore stdout
    fflush(stdout);
    dup2(stdoutSave, fileno(stdout));
    close(stdoutSave);

    // Read captured data from pipe read end
    std::string result;
    char buffer[4096];
    ssize_t n;
    // It's safe to read in a loop until EOF
    while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        result.append(buffer, static_cast<size_t>(n));
    }
    close(pipefd[0]);
    return result;
}

// Simple test harness state
static int g_totalTests = 0;
static int g_failedTests = 0;

// Helper: Run a test case and compare output with expected string.
// Prints PASSED/FAILED per test.
static void runAndCheck(const char* testName,
                        const std::string& expectedOutput,
                        const std::function<void()>& testFunc) {
    std::string actual = captureOutput(testFunc);
    // Normalize line endings if necessary (not strictly required here)
    if (actual == expectedOutput) {
        std::printf("[PASSED] %s\n", testName);
    } else {
        std::printf("[FAILED] %s: expected \"%s\" but got \"%s\"\n",
                    testName, expectedOutput.c_str(), actual.c_str());
        ++g_failedTests;
    }
    ++g_totalTests;
}

// Entry point
int main(int argc, char* argv[]) {
    makeStdoutUnbuffered();

    // Safety: Ensure MemStr is accessible for building expected strings
    // and TotalMemory is writable (defined in the C test header as extern/global).
    // We rely on the actual header definitions for types like cmsBool, cmsUInt32Number.

    // Test 1: TotalMemory > 0; ensure the "Ok, but <MemStr(TotalMemory)> are left!" path is taken.
    // We explicitly set TotalMemory to a known value and call TestMemoryLeaks.
    runAndCheck("TestMemoryLeaks_PositiveTotal",
        // Expected: "Ok, but <MemStr(total)> are left!\n"
        // Build expected string using the provided MemStr function to stay consistent.
        std::string("Ok, but ") + MemStr((cmsUInt32Number)123) + std::string(" are left!\n"),
        []() {
            // Setup: simulate a leak of 123 bytes
            TotalMemory = 123;
            TestMemoryLeaks(/*ok=*/1);  // ok value doesn't affect this branch
        });

    // Test 2: TotalMemory == 0 and ok == true; expect "Ok.\n"
    runAndCheck("TestMemoryLeaks_ZeroTotal_OkTrue",
        std::string("Ok.\n"),
        []() {
            TotalMemory = 0;
            TestMemoryLeaks(/*ok=*/1);
        });

    // Test 3: TotalMemory == 0 and ok == false; expect no output
    runAndCheck("TestMemoryLeaks_ZeroTotal_OkFalse",
        std::string(""),
        []() {
            TotalMemory = 0;
            TestMemoryLeaks(/*ok=*/0);
        });

    // Summary
    std::printf("Tests run: %d, Failures: %d\n", g_totalTests, g_failedTests);

    // Return non-zero if any test failed to signal failure to buildsystem/scripts
    return (g_failedTests == 0) ? 0 : 1;
}

/*
Notes for maintainers:
- The tests rely on the global variables and helper functions exposed by testcms2.h:
  - TotalMemory: cmsUInt32Number, controls the first branch.
  - MemStr(cmsUInt32Number): returns a C-string describing the memory amount.
  - TestMemoryLeaks(cmsBool ok): focal function under test.
- We capture stdout to verify exact output, including newline characters.
- We use a minimal, non-terminating test harness (no GTest) as requested.
- If MemStr uses internal static buffers, we construct expected outputs using MemStr(...) at test time
  to ensure consistency with actual runtime formatting.
*/