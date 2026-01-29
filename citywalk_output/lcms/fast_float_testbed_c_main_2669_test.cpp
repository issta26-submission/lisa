/*
  Lightweight C++11 test harness for the focal method: main() in fast_float_testbed.c

  What this test suite does:
  - Treats the focal method as a black-box executable by running the compiled
    testbed binary as a separate process and validating its runtime behavior.
  - Verifies that the testbed completes successfully (exit code 0) and that
    its stdout contains expected success messages indicating all tests passed.
  - Provides basic, minimal test cases without relying on GTest or any heavy
    testing framework (as requested).
  - Uses a simple cross-platform process launcher (POSIX and Windows) to
    execute the target binary and capture its output for assertions.

  Step mapping (high-level):
  Step 1: Program Understanding
    - Candidate Keywords reflect core components invoked by the focal main:
      { CheckComputeIncrements, CheckPremultiplied, CheckFormatters15,
        Check15bitsConversions, CheckAccuracy16Bits, CheckLab2RGB, CheckChangeFormat,
        CheckSoftProofing, CheckConversionFloat, sRGB_XYZ_roundtrip,
        SpeedTest8, SpeedTest16, SpeedTest15, SpeedTestFloat,
        ComparativeFloatVs16bits, ComparativeLineStride8bits,
        TestGrayTransformPerformance, TestGrayTransformPerformance1 }

  Step 2: Unit Test Generation
    - The test suite executes the fast_float_testbed binary (the focal main) as a
      separate process and asserts on its exit status and on key success messages
      found in stdout.

  Step 3: Test Case Refinement
    - Additional tests consider absence of the binary (failure to start) and
      partial output checks for progress lines, to improve robustness.

  Notes:
  - This approach avoids modifying or instrumenting the existing C source
    (fast_float_testbed.c) and does not require GTest.
  - The test harness expects the target binary to be buildable as a separate
    executable named something like fast_float_testbed (or fast_float_testbed.exe
    on Windows). If a different path is used, pass it as a command-line argument.

  Usage:
    1) Build the testbed binary separately (e.g., produce fast_float_testbed).
    2) Compile this harness (e.g., g++ -std=c++11 -O2 -o test_fast_float_testbed test_fast_float_testbed.cpp).
    3) Run: ./test_fast_float_testbed [path_to_fast_float_testbed_binary]
*/

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <functional>
#include <vector>
#include <io.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdlib>
#include <windows.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <direct.h>


#if defined(_WIN32) || defined(_WIN64)
#define OS_WINDOWS 1
#else
#define OS_WINDOWS 0
#endif

// Candidate Keywords representing core dependent components extracted from the focal main.
// These are used here purely as documentation for coverage reasoning.
static const char* CandidateKeywords[] = {
    "CheckComputeIncrements",
    "CheckPremultiplied",
    "CheckFormatters15",
    "Check15bitsConversions",
    "CheckAccuracy16Bits",
    "CheckLab2RGB",
    "CheckChangeFormat",
    "CheckSoftProofing",
    "CheckConversionFloat",
    "sRGB_XYZ_roundtrip",
    "SpeedTest8",
    "SpeedTest16",
    "SpeedTest15",
    "SpeedTestFloat",
    "ComparativeFloatVs16bits",
    "ComparativeLineStride8bits",
    "TestGrayTransformPerformance",
    "TestGrayTransformPerformance1"
};

// A tiny cross-platform helper to run an external binary and capture its stdout.
// We intentionally avoid external libraries; this keeps the harness lightweight.
class ProcessRunner
{
public:
    // Runs the executable at 'path' and captures stdout into 'output'.
    // Returns true on successful launch; 'exitCode' receives the process exit code
    // (0 usually means success for this testbed).
    static bool run(const std::string& path, std::string& output, int& exitCode)
    {
#if OS_WINDOWS
        // On Windows, use _popen to capture stdout
        std::string cmd = path;
        cmd += " 2>&1"; // redirect stderr to stdout
        FILE* pipe = _popen(cmd.c_str(), "r");
        if (!pipe) return false;

        constexpr size_t bufSize = 4096;
        char buffer[bufSize];
        output.clear();
        while (fgets(buffer, (int)bufSize, pipe) != nullptr) {
            output += buffer;
        }

        int rc = _pclose(pipe);
        exitCode = rc;
        return true;
#else
        // POSIX path: use popen
        std::string cmd = path;
        cmd += " 2>&1"; // redirect stderr to stdout
        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe) return false;

        constexpr size_t bufSize = 4096;
        char buffer[bufSize];
        output.clear();
        while (fgets(buffer, (int)bufSize, pipe) != nullptr) {
            output += buffer;
        }

        int rc = pclose(pipe);
        // In POSIX, WIFEXITED/ WEXITSTATUS interpret the status.
        // For simplicity, attempt to extract the exit code if available.
        exitCode = -1;
#if defined(WIFEXITED)
        if (WIFEXITED(rc)) exitCode = WEXITSTATUS(rc);
#endif
        // If WIFEXITED isn't available, fall back to the raw status.
        if (exitCode == -1) exitCode = rc;
        return true;
#endif
    }
};

// Small assertion helper for test readability
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        resultStream() << "ASSERT FAILED: " << msg << "\n"; \
        return false; \
    } \
} while (0)


// Lightweight test harness state
static std::ostream& resultStream()
{
    static std::ostringstream oss;
    return oss;
}

// Convenience to print final summary
static void printSummary(size_t total, size_t passed)
{
    std::cout << "\nTest Summary: " << passed << " / " << total << " tests passed.\n";
}

static bool contains(const std::string& hay, const std::string& needle)
{
    return hay.find(needle) != std::string::npos;
}

/*
  Test 1: Normal run of the focal testbed
  - Executes the binary and asserts a clean exit (0) and presence of success strings.
  - Expects substrings:
      "All floating point tests passed OK"
      "All tests passed OK"
*/
static bool Test_NormalRun(const std::string& binaryPath)
{
    std::string stdoutText;
    int exitCode = -1;
    bool started = ProcessRunner::run(binaryPath, stdoutText, exitCode);
    if (!started) {
        std::cout << "[Test_NormalRun] Failed to start binary: " << binaryPath << "\n";
        return false;
    }

    bool okExit = (exitCode == 0);
    bool hasFloatingOK = contains(stdoutText, "All floating point tests passed OK");
    bool hasAllOK = contains(stdoutText, "All tests passed OK");

    std::cout << "[Test_NormalRun] Exit code: " << exitCode << "\n";
    // Optional: log stdout for debugging if needed
    // std::cout << "OUTPUT:\n" << stdoutText << "\n";

    ASSERT_TRUE(okExit, "Binary did not exit with code 0.");
    ASSERT_TRUE(hasFloatingOK, "\"All floating point tests passed OK\" not found in stdout.");
    ASSERT_TRUE(hasAllOK, "\"All tests passed OK\" not found in stdout.");

    return true;
}

/*
  Test 2: Failure to start the testbed binary (non-existent path)
  - Verifies the harness properly handles inability to launch the target.
*/
static bool Test_MissingBinary(const std::string& binaryPath)
{
    std::string stdoutText;
    int exitCode = -1;
    bool started = ProcessRunner::run(binaryPath, stdoutText, exitCode);
    // We expect failure to start; 'started' should be false.
    ASSERT_TRUE(!started, "Expected failure to start missing binary, but it started.");
    // No exit code to check; return true if we reached here.
    return true;
}

/*
  Test 3: Basic stdout content sanity (progress lines)
  - Ensure that the testbed outputs progressive installation and test steps.
  - Looks for a known early-progress fragment, e.g., "Installing plug-in" and "done."
*/
static bool Test_ProgressOutput(const std::string& binaryPath)
{
    std::string stdoutText;
    int exitCode = -1;
    bool started = ProcessRunner::run(binaryPath, stdoutText, exitCode);
    if (!started) {
        std::cout << "[Test_ProgressOutput] Failed to start binary: " << binaryPath << "\n";
        return false;
    }

    bool okExit = (exitCode == 0);
    bool hasInstall = contains(stdoutText, "Installing plug-in");
    bool hasDone = contains(stdoutText, "done.");

    // If the testbed completed successfully, both messages should appear in stdout.
    ASSERT_TRUE(okExit, "Binary did not exit with code 0.");
    ASSERT_TRUE(hasInstall, "\"Installing plug-in\" not found in stdout.");
    ASSERT_TRUE(hasDone, "\"done.\" (progress message) not found in stdout.");

    return true;
}

// Entry point for the test harness
int main(int argc, char* argv[])
{
    // Determine path to the target testbed binary
    std::string testbedPath;
    if (argc > 1) {
        testbedPath = argv[1];
    } else {
#if defined(_WIN32)
        testbedPath = "fast_float_testbed.exe";
#else
        testbedPath = "./fast_float_testbed";
#endif
    }

    std::vector<std::function<bool(void)>> tests;

    // Populate tests with descriptive lambdas (each returns true on pass)
    // Note: We wrap each test in a lambda to maintain a simple, linear runner.
    tests.push_back([&]() { std::cout << "Running Test_NormalRun against: " << testbedPath << "\n"; return Test_NormalRun(testbedPath); });
    tests.push_back([&]() { std::cout << "Running Test_ProgressOutput against: " << testbedPath << "\n"; return Test_ProgressOutput(testbedPath); });
    // If you want to also validate missing binary behavior, uncomment the following line after providing a non-existent path or run with a crafted binary path.
    // tests.push_back([&]() { std::cout << "Running Test_MissingBinary against invalid path\n"; return Test_MissingBinary("/path/does/not/exist"); });

    size_t total = tests.size();
    size_t passed = 0;

    // Run tests
    for (size_t i = 0; i < tests.size(); ++i) {
        bool result = tests[i]();
        if (result) {
            std::cout << "[Test " << i+1 << "] PASSED\n";
            ++passed;
        } else {
            std::cout << "[Test " << i+1 << "] FAILED\n";
        }
        // Brief separation in log
        std::cout << "----------------------------------------\n";
    }

    printSummary(total, passed);

    // Return 0 if all tests pass, non-zero otherwise
    return (passed == total) ? 0 : 1;
}