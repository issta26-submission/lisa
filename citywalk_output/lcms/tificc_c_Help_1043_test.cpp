// Test suite for the focal method Help(int level) in tificc.c using a lightweight C++11 harness.
// Notes:
// - We override certain external dependencies by providing C-stub definitions with C linkage:
//   cmsGetEncodedCMMversion, PrintBuiltins, PrintRenderingIntents, and exit(...).
// - Help() ends with exit(0). We intercept exit via a longjmp-based mechanism to prevent test process termination.
// - We capture stderr output via a pipe to validate the printed help text without requiring a full Little CMS setup.
// - This test suite does not rely on GTest; it uses a small, self-contained testing approach with simple EXPECT-style checks.

#include <cstring>
#include <cstdio>
#include <lcms2_plugin.h>
#include <csetjmp>
#include <vector>
#include <iostream>
#include <string>
#include <stdint.h>
#include <unistd.h>
#include <utils.h>
#include <tiffio.h>


// Forward declaration of the focal function (C linkage from tificc.c)
extern "C" void Help(int level);

// External dependencies provided by the code under test (stubbed for the test)
extern "C" unsigned int cmsGetEncodedCMMversion(void);
extern "C" void PrintBuiltins(void);
extern "C" void PrintRenderingIntents(void);

// We override exit to catch Help's termination and control test flow.
// Implemented with setjmp/longjmp to return to the test harness without terminating the process.
static jmp_buf g_test_jmp_buf;
static volatile int g_test_exit_status;      // Captured exit status from Help(...)
static bool g_printBuiltins_called;          // Stubbed indicator: PrintBuiltins() was invoked
static bool g_printRenderingIntents_called;    // Stubbed indicator: PrintRenderingIntents() was invoked

// Pipe for capturing stderr output produced by Help(...)
static int g_stderr_pipe[2];

// Simple test failure counter
static int g_total_failures = 0;

// Lightweight EXPECT macro to accumulate failures without terminating tests
#define EXPECT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST FAILURE: " << (msg) << std::endl; \
        ++g_total_failures; \
    } \
} while(0)

// Stub implementations (C linkage) for dependencies expected by Help
// Provide deterministic behavior suitable for unit testing.

extern "C" unsigned int cmsGetEncodedCMMversion(void)
{
    // Return a version that formats as "8.10" when divided by 1000.0
    return 8100;
}

extern "C" void PrintBuiltins(void)
{
    // Mark that the builtins printing path was invoked
    g_printBuiltins_called = true;
}

extern "C" void PrintRenderingIntents(void)
{
    // Mark that the rendering intents printing path was invoked
    g_printRenderingIntents_called = true;
}

// Override exit() to trap calls from Help() and jump back to the test harness
extern "C" void exit(int status)
{
    g_test_exit_status = status;
    longjmp(g_test_jmp_buf, 1);
}

// Helper: reset test state
static void reset_test_state()
{
    g_test_exit_status = -1;
    g_printBuiltins_called = false;
    g_printRenderingIntents_called = false;
}

// Test 1: Help should end by calling exit(0) (we intercept with longjmp)
static bool test_help_exits_with_zero()
{
    reset_test_state();
    int setjmp_rc = setjmp(g_test_jmp_buf);
    if (setjmp_rc == 0) {
        // This call should not return normally; exit(0) will be hijacked by our exit(...)
        Help(0);
        // If we reach here, Help() did not terminate via exit() as expected
        std::cerr << "Help() unexpectedly returned normally in Test 1." << std::endl;
        return false;
    } else {
        // Returned via our longjmp from exit(0)
        EXPECT(g_test_exit_status == 0, "Help should exit with status 0");
        return (g_test_exit_status == 0);
    }
}

// Test 2: Help should print usage and header messages to stderr and invoke stubbed helpers
// We capture stderr via a pipe and validate that:
// - The output contains a recognizable usage line.
// - The output contains the header line "Little CMS ICC profile applier for TIFF".
// - The stubbed functions were invoked (PrintBuiltins and PrintRenderingIntents).
static bool test_help_output_and_stub_calls()
{
    reset_test_state();

    // Prepare a pipe to capture stderr
    if (pipe(g_stderr_pipe) != 0) {
        perror("pipe");
        return false;
    }

    // Redirect stderr to the pipe
    int saved_stderr = dup(STDERR_FILENO);
    if (dup2(g_stderr_pipe[1], STDERR_FILENO) == -1) {
        perror("dup2");
        return false;
    }
    // Close the unused end of the pipe in the writing side
    close(g_stderr_pipe[1]);

    // Run Help inside a setjmp/longjmp boundary to intercept exit()
    int setjmp_rc = setjmp(g_test_jmp_buf);
    if (setjmp_rc == 0) {
        Help(0);
    } else {
        // Returned via exit interception
        // Continue to analyze captured output
    }

    // Important: Ensure all buffered output is flushed before reading
    fflush(stderr);

    // Restore original stderr
    fflush(stderr);
    if (dup2(saved_stderr, STDERR_FILENO) == -1) {
        perror("dup2 restore");
        return false;
    }
    close(saved_stderr);

    // Read captured stdout
    // Note: There might be more data than buffer; accumulate until EOF
    std::string captured;
    char buf[4096];
    ssize_t n;
    // Close the write end if not already
    // Read until no more data
    while ((n = read(g_stderr_pipe[0], buf, sizeof(buf) - 1)) > 0) {
        buf[n] = '\0';
        captured += buf;
    }
    close(g_stderr_pipe[0]);

    // Validate captured content and that the hooks were invoked
    bool hasUsageLine = captured.find("usage: tificc [flags] input.tif output.tif") != std::string::npos ||
                       captured.find("usage: tificc") != std::string::npos;
    bool hasHeaderLine = captured.find("Little CMS ICC profile applier for TIFF") != std::string::npos;

    EXPECT(g_test_exit_status == 0, "Help should exit with status 0 after printing");
    EXPECT(g_printBuiltins_called, "PrintBuiltins() should be invoked by Help()");
    EXPECT(g_printRenderingIntents_called, "PrintRenderingIntents() should be invoked by Help()");
    EXPECT(hasUsageLine, "Help output should contain a usage line");
    EXPECT(hasHeaderLine, "Help output should mention the header line for Little CMS");

    return (g_test_exit_status == 0) && g_printBuiltins_called && g_printRenderingIntents_called && hasUsageLine && hasHeaderLine;
}

int main()
{
    std::vector<std::string> test_names = {
        "Help exits with status 0",
        "Help prints header/usage and invokes stubs"
    };

    int passed = 0;
    // Run Test 1
    std::cout << "Running test: " << test_names[0] << std::endl;
    if (test_help_exits_with_zero()) {
        std::cout << "[PASSED] " << test_names[0] << std::endl;
        ++passed;
    } else {
        std::cout << "[FAILED] " << test_names[0] << std::endl;
    }

    // Run Test 2
    std::cout << "Running test: " << test_names[1] << std::endl;
    if (test_help_output_and_stub_calls()) {
        std::cout << "[PASSED] " << test_names[1] << std::endl;
        ++passed;
    } else {
        std::cout << "[FAILED] " << test_names[1] << std::endl;
    }

    // Summary
    std::cout << "Tests completed: " << passed << " / " << test_names.size() << " passed." << std::endl;
    return (g_total_failures == 0) ? (passed == (int)test_names.size() ? 0 : 1) : 1;
}