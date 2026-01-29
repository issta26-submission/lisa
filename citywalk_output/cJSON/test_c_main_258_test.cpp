// Lightweight C++11 test suite (no Google Test) for the focal C function:
// The focal method is the non-C++ main() function that prints a version string
// via cJSON_Version() and then calls create_objects() before returning 0.
// This test harness uses a dynamic-load approach to avoid symbol conflicts with
// any potential mains in the test harness or in the focal library.
// The test assumes the focal main() is compiled into a shared library and its path
// is supplied via the FOCAL_LIB_PATH environment variable.
// See comments for rationale and usage notes.

#include <dlfcn.h>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <vector>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <string>
#include <cJSON.h>


// Ensure CDECL macro is present if the library uses it.
// If not defined, we treat it as empty.
#ifndef CJSON_CDECL
#define CJSON_CDECL
#endif

// Prototype of the focal main() function inside the dynamic library.
// We declare the type here; the actual symbol will be resolved via dlsym.
typedef int (*focal_main_t)(void);

// Global handle to keep the dynamic library loaded during tests.
static void* g_focal_lib_handle = nullptr;

// Utility: load the focal library and retrieve the symbol "main".
// Returns a function pointer to int main(void) or nullptr on failure.
static focal_main_t load_focal_main()
{
    // The path to the focal library is provided by environment variable.
    const char* lib_path = std::getenv("FOCAL_LIB_PATH");
    if (!lib_path || lib_path[0] == '\0')
    {
        std::cerr << "[TEST] FOCAL_LIB_PATH environment variable not set or empty.\n";
        return nullptr;
    }

    // Attempt to open the shared library.
    g_focal_lib_handle = dlopen(lib_path, RTLD_NOW);
    if (!g_focal_lib_handle)
    {
        std::cerr << "[TEST] dlopen failed: " << dlerror() << "\n";
        return nullptr;
    }

    // Clear any prior error.
    dlerror();

    // Resolve the symbol: main
    focal_main_t f = (focal_main_t)dlsym(g_focal_lib_handle, "main");
    const char* dlsym_err = dlerror();
    if (dlsym_err != nullptr)
    {
        std::cerr << "[TEST] dlsym failed to locate 'main': " << dlsym_err << "\n";
        dlclose(g_focal_lib_handle);
        g_focal_lib_handle = nullptr;
        return nullptr;
    }

    if (!f)
    {
        std::cerr << "[TEST] Symbol 'main' not found in focal library.\n";
        dlclose(g_focal_lib_handle);
        g_focal_lib_handle = nullptr;
        return nullptr;
    }

    return f;
}

// Utility: capture stdout produced by invoking the focal main function.
// Returns the captured string. Retaining the library handle ensures the library
// remains loaded during capture.
static std::string capture_output_of_main(focal_main_t focal_main)
{
    // Pipe to capture stdout
    int pipefd[2];
    if (pipe(pipefd) != 0)
    {
        perror("pipe");
        return "";
    }

    // Save current stdout
    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1)
    {
        perror("dup stdout");
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stdout to the pipe's write end
    if (dup2(pipefd[1], fileno(stdout)) == -1)
    {
        perror("dup2 stdout");
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        return "";
    }
    // Close the pipe's write end as it's now duplicated to stdout
    close(pipefd[1]);

    // Invoke focal main
    int ret = focal_main();

    // Ensure all output is flushed
    fflush(stdout);

    // Restore stdout
    if (dup2(saved_stdout, fileno(stdout)) == -1)
    {
        perror("restore stdout");
        // fall through
    }
    close(saved_stdout);

    // Read captured data
    std::string output;
    char buffer[4096];
    ssize_t n;
    while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0)
    {
        output.append(buffer, buffer + n);
    }
    close(pipefd[0]);

    // Ignore the return value in this capture
    (void)ret;
    return output;
}

// Simple non-terminating assertion mechanism.
// Counts successes/failures and logs descriptive messages without exiting.
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT(cond, desc) \
    do { \
        ++g_tests_run; \
        if (cond) { \
            ++g_tests_passed; \
        } else { \
            ++g_tests_failed; \
            std::cerr << "[TEST] FAIL: " << (desc) << "\n"; \
        } \
    } while (0)


// Test 1: Ensure the focal main prints a Version line starting with "Version: "
static bool test_main_prints_version_prefix()
{
    focal_main_t focal = load_focal_main();
    if (!focal)
    {
        std::cerr << "[TEST] Failed to load focal main for Test 1.\n";
        return false;
    }

    std::string output = capture_output_of_main(focal);

    // Expected: Output begins with "Version: "
    bool ok = (output.size() >= 9) && (output.rfind("Version: ", 0) == 0);
    if (!ok)
    {
        std::cerr << "[TEST] Test 1 output did not start with 'Version: '. Output was: '"
                  << output << "'\n";
    }
    return ok;
}

// Test 2: Ensure the focal main returns 0 (success) when executed.
static bool test_main_returns_zero()
{
    focal_main_t focal = load_focal_main();
    if (!focal)
    {
        std::cerr << "[TEST] Failed to load focal main for Test 2.\n";
        return false;
    }

    // We will discard stdout to keep test output clean
    int pipefd[2];
    if (pipe(pipefd) != 0)
    {
        perror("pipe (Test 2)");
        return false;
    }

    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1)
    {
        perror("dup stdout (Test 2)");
        close(pipefd[0]);
        close(pipefd[1]);
        return false;
    }

    // Redirect stdout to /dev/null
    int devnull = open("/dev/null", O_WRONLY);
    if (devnull == -1)
    {
        perror("open /dev/null (Test 2)");
        dup2(saved_stdout, fileno(stdout));
        close(saved_stdout);
        close(pipefd[0]);
        close(pipefd[1]);
        return false;
    }

    if (dup2(devnull, fileno(stdout)) == -1)
    {
        perror("redirect stdout (Test 2)");
        // restore
        dup2(saved_stdout, fileno(stdout));
        close(saved_stdout);
        close(pipefd[0]);
        close(pipefd[1]);
        close(devnull);
        return false;
    }

    // Close the write end of the pipe since we won't capture in this test
    close(devnull);
    close(pipefd[1]); // not used for capture

    // Call focal main
    int ret = focal( );

    // Restore stdout
    if (dup2(saved_stdout, fileno(stdout)) == -1)
    {
        perror("restore stdout (Test 2)");
        // fall through
    }
    close(saved_stdout);

    // The test asserts return value == 0
    bool ok = (ret == 0);
    if (!ok)
    {
        std::cerr << "[TEST] Test 2: focal main returned " << ret << " (expected 0).\n";
    }
    return ok;
}

// Main test runner
int main()
{
    // Step through each test, using non-terminating assertions
    // (we continue running all tests even if one fails)
    // The tests are designed to exercise the focal main's behavior, given
    // the environment provides the focal library as a shared object.

    // Test 1: Version line is printed
    bool t1 = test_main_prints_version_prefix();
    TEST_ASSERT(t1, "Main should print a Version line starting with 'Version: '");

    // Test 2: main returns 0
    bool t2 = test_main_returns_zero();
    TEST_ASSERT(t2, "Main should return 0 on successful execution");

    // Summary
    std::cout << "Test summary: "
              << g_tests_passed << " passed, "
              << g_tests_failed << " failed, "
              << g_tests_run << " total tests.\n";

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}

// Notes and usage:
// - Build this test wrapper as a separate executable.
// - Compile the focal library code as a shared library (e.g., libfocal.so) that
//   exports a symbol named 'main' with the signature int main(void).
// - Set the environment variable FOCAL_LIB_PATH to the path of the shared library
//   prior to running the test executable, e.g.:
//     export FOCAL_LIB_PATH=./libfocal.so
// - The test harness will dynamically load the library, invoke its main(), capture
//   its stdout, and verify behavior without causing symbol conflicts with the test
//   executable's own entry point.
// - This approach adheres to the constraint about static/dynamic linkage and avoids
//   private/private method exposure issues, while focusing on the focal method's
//   observable behavior (stdout content and return value).

// End of test suite code.