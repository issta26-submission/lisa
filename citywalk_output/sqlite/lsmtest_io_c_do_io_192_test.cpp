// Test suite for the focal method do_io in lsmtest_io.c
// This test harness is designed for C++11, without GTest.
// It provides minimal C-lean wrappers for the production-time dependencies
// (testPrintUsage, testPrintError, testTimeInit, testTimeGet) so that
// do_io can be exercised from C++ tests.
// The tests aim to cover primary branches of do_io such as argument validation
// and file-open failure scenarios. Due to the static/internal nature of some
// helper functions in the focal file, the tests focus on observable behavior
// of do_io (return codes) given typical input permutations.

#include <lsmtest.h>
#include <vector>
#include <cstdio>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <cstdlib>


// Declare the focal function with C linkage to allow linking with the C file.
// do_io signature is: int do_io(int nArg, char **azArg);
extern "C" int do_io(int nArg, char **azArg);

// Forward declare test-time and messaging stubs used by the focal module.
// These functions are expected by the focal code (lsmtest_io.c) and must be
// provided for linking in the test environment.

extern "C" {

// Print usage message (called when too few arguments are supplied)
void testPrintUsage(const char* zFormat) {
    // Minimal stub: print to stdout for visibility and keep tests non-fatal.
    if (zFormat) {
        std::printf("TEST_PRINT_USAGE: %s\n", zFormat);
    } else {
        std::printf("TEST_PRINT_USAGE: usage requested\n");
    }
}

// Print error message (variadic in production, simplified here)
void testPrintError(const char* zFormat, ...) {
    // Forward variadic arguments to vprintf for visibility
    va_list args;
    va_start(args, zFormat);
    std::vfprintf(stderr, zFormat, args);
    std::fprintf(stderr, "\n");
    va_end(args);
}

// Initialize test timer (no-op or minimal simulated timer)
void testTimeInit() {
    // No-op: in this test scaffold we don't rely on real timing.
}

// Return a pseudo elapsed time in milliseconds
int testTimeGet() {
    // Simple monotonically increasing counter (static for consistency across calls)
    static int t = 0;
    t += 1;
    return t;
}

// Read all stdin for the test scenario (not used in these specific tests,
// but provided to satisfy potential linker/usage expectations in the TU)
void readStdin(char** pzOut) {
    // Not used by our test scenarios; provide a safe empty string.
    if (pzOut) {
        *pzOut = nullptr;
    }
}
} // extern "C"

// Helper: Convert vector<string> to argv array for do_io
static int callDoIoWithArgs(const std::vector<std::string>& args) {
    int argc = (int)args.size();
    std::vector<char*> argv(argc);
    for (int i = 0; i < argc; ++i) {
        argv[i] = strdup(args[i].c_str());
    }

    int rc = do_io(argc, argc ? argv.data() : nullptr);

    for (int i = 0; i < argc; ++i) {
        free(argv[i]);
    }
    return rc;
}

// Helper: Check file existence
static bool fileExists(const std::string& path) {
    struct stat st;
    return (stat(path.c_str(), &st) == 0);
}

// Test 1: Insufficient arguments should return -1
// This exercises the early branch of do_io that prints usage and exits.
static bool test_insufficient_arguments() {
    std::cout << "Running Test 1: Insufficient arguments -> expect -1\n";
    // argc = 1 (only program name) to trigger the usage path
    std::vector<std::string> args = { "lsmtest_io" };
    int rc = callDoIoWithArgs(args);
    bool passed = (rc == -1);
    if (!passed) {
        std::cout << "Test 1 FAILED: expected -1, got " << rc << "\n";
    } else {
        std::cout << "Test 1 PASSED\n";
    }
    return passed;
}

// Test 2: Open a valid file path and exercise the else-branch (nArg > 2)
// We provide a temporary file path in /tmp and a harmless command string.
// The actual behavior of doOneCmd is implementation-dependent, but a successful
// open path followed by a loop exit (via doOneCmd behavior) should yield rc==0.
static bool test_open_success_nontrivial_args() {
    std::cout << "Running Test 2: Open valid path with extra command arg -> expect non-negative rc\n";
    // Use a unique temporary file path
    pid_t pid = getpid();
    std::string tmpPath = "/tmp/lsmtest_io_testfile_" + std::to_string(pid) + ".bin";

    // Ensure previous file is removed
    unlink(tmpPath.c_str());

    // Args: program, path, pgsz-string, a dummy command
    std::vector<std::string> args = { "lsmtest_io", tmpPath, "1024", "CMD_DUMMY" };
    int rc = callDoIoWithArgs(args);

    // If the file was created/opened, rc may still be 0 depending on doOneCmd,
    // but the primary expectation here is that we did not crash and the call
    // returns a reasonable code (0 or positive). We conservatively check non-negative.
    bool passed = (rc >= 0);
    if (!passed) {
        std::cout << "Test 2 FAILED: expected rc >= 0, got " << rc << "\n";
    } else {
        std::cout << "Test 2 PASSED (rc=" << rc << ")\n";
    }

    // Cleanup: remove the file if created
    unlink(tmpPath.c_str());
    return passed;
}

// Test 3: Open should fail for an invalid path (simulate permission/dir error) -> expect -1
static bool test_open_failure_invalid_path() {
    std::cout << "Running Test 3: Open invalid path -> expect -1\n";
    // An obviously invalid path (likely to be inaccessible)
    std::string badPath = "/path/that/does/not/exist/lsmtest_io.bin";

    std::vector<std::string> args = { "lsmtest_io", badPath, "1024" };
    int rc = callDoIoWithArgs(args);

    bool passed = (rc == -1);
    if (!passed) {
        std::cout << "Test 3 FAILED: expected -1, got " << rc << "\n";
    } else {
        std::cout << "Test 3 PASSED\n";
    }
    return passed;
}

// Simple test harness
int main() {
    int failures = 0;
    failures += test_insufficient_arguments() ? 0 : 1;
    failures += test_open_success_nontrivial_args() ? 0 : 1;
    failures += test_open_failure_invalid_path() ? 0 : 1;

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << failures << " TEST(S) FAILED\n";
        return 1;
    }
}