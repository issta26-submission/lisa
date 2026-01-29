// Unit test suite for the focal method main in mkgrayer.c
// This test harness uses a lightweight, self-contained test framework (no GTest).
// It relies on the presence of the mkgrayer binary (built from the provided focal method).
// The tests execute the binary, verify exit codes, and inspect side effects (e.g., created files).
// Explanatory comments are provided for each test to clarify intent and validation strategy.

#include <cstring>
#include <sys/stat.h>
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include <cerrno>
#include <lcms.h>


// Lightweight test framework (non-terminating assertions)
namespace TestFramework {

static int g_total = 0;
static int g_failed = 0;

// Non-terminating assertion: the test continues after a failure
#define EXPECT_TRUE(cond, msg) do { \
    ++g_total; \
    if (!(cond)) { \
        ++g_failed; \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " - " << (msg) << "\n"; \
    } else { \
        std::cerr << "[PASSED] " << __FILE__ << ":" << __LINE__ \
                  << " - " << (msg) << "\n"; \
    } \
} while(0)

static void print_summary() {
    std::cerr << "\nTest Summary: " << g_total << " run, " << g_failed
              << " failed, " << (g_total - g_failed) << " passed.\n";
}

} // namespace TestFramework

// Helper: run the mkgrayer binary and return its exit code.
// If the environment variable MKGRAYER_BIN is set, use that path; otherwise use "./mkgrayer".
static int runMkGrayerBinary() {
    const char* envPath = std::getenv("MKGRAYER_BIN");
    const char* path = envPath ? envPath : "./mkgrayer";

    // Use system() to invoke the binary. This avoids requiring a separate testing runner.
    // We translate the system() exit code to a meaningful integer exit status.
    int status = std::system(path);
    if (status == -1) {
        // system() failed to launch the process
        return -1;
    }

    // Normalize to a typical exit code (works on POSIX systems)
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
        // Abnormal termination by signal; encode as a negative value for testing clarity
        return -WTERMSIG(status);
    }

    // Fallback
    return -2;
}

// Helper: check if a file exists (POSIX)
static bool file_exists(const char* path) {
    struct stat st;
    return (stat(path, &st) == 0);
}

// Test 1: Verify that the focal program's main exits with code 0 under normal conditions.
// Rationale: The provided focal method performs a sequence of operations and should complete successfully.
// This test exercises the true execution path of the program.
static void test_main_exits_zero() {
    // Execute the binary
    int code = runMkGrayerBinary();

    // Assertion: Expect exit code 0
    EXPECT_TRUE(code == 0, "mkgrayer main should exit with code 0 on normal run");
}

// Test 2: Verify that the focal program creates the expected side-effect file interpol2.icc.
// Rationale: The program attempts to open and write to interpol2.icc as part of its workflow.
// This test checks for the presence of that file after execution.
// Note: This relies on the library path behaving in a standard way (as per the provided code).
static void test_interpol2_icc_created() {
    const char* iccPath = "./interpol2.icc";

    // Ensure the file does not exist before the run
    if (file_exists(iccPath)) {
        // Best-effort cleanup before test
        if (unlink(iccPath) != 0) {
            // If cleanup fails, still proceed; the test will observe the result afterward
            std::cerr << "[WARN] Could not remove pre-existing interpol2.icc before test.\n";
        }
    }

    // Run the focal program
    int code = runMkGrayerBinary();
    EXPECT_TRUE(code == 0, "mkgrayer main should exit with code 0 when checking file creation");

    // Check for the existence of interpol2.icc after execution
    bool exists = file_exists(iccPath);
    EXPECT_TRUE(exists, "interpol2.icc should be created by the focal program during run");

    // Optional cleanup after test
    if (exists) {
        if (unlink(iccPath) != 0) {
            std::cerr << "[WARN] Could not clean interpol2.icc after test.\n";
        }
    }
}

// Main test runner
int main() {
    using namespace TestFramework;

    std::cerr << "Starting unit tests for mkgrayer.c focal main\n";

    // Test 1: Basic exit code verification
    test_main_exits_zero();

    // Test 2: Side-effect verification (file creation)
    test_interpol2_icc_created();

    // Summary
    print_summary();

    // Return non-zero if tests failed, to be compatible with CI expectations
    return (TestFramework::g_failed == 0) ? 0 : 1;
}