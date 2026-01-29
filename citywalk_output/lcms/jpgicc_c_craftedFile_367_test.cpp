/*
Unit Test Suite for the focal method craftedFile() in the provided C source.

Notes on approach (embedded as comments for traceability):
- The focal function craftedFile() unconditionally calls FatalError("Corrupted EXIF data");
- Directly testing this in-process would terminate the test harness depending on FatalError's implementation.
- To observe the call without terminating the test harness, we use a dynamic interposition approach:
  - A small preload library (libfatal_override.so) defines FatalError and records the invocation, then exits the child process gracefully.
  - The test harness runs a child process (same executable) with LD_PRELOAD pointing to the preload library and RUN_CHILD=1 to trigger craftedFile().
  - The parent checks that the preload side-effect occurred (via a flag file) to confirm the function invoked FatalError.
- Additional test verifies the presence of the symbol craftedFile in the binary via dlsym, ensuring the symbol exists and is linkable.
- This test suite does not rely on Google Test (GTest) and uses a lightweight self-contained harness with simple, non-terminating assertions.
- Build notes (to be applied by the build system):
  - Compile the focal C file(s) (including jpgicc.c content) into the test binary as a regular translation unit (e.g., g++ test_main.cpp jpgicc.c -o test_exec -ldl).
  - Build the preload library:
      g++ -fPIC -shared preload_fatal.cpp -o libfatal_override.so
  - Run tests with LD_PRELOAD pointing to the library:
      PRELOAD_LIB=/path/to/libfatal_override.so ./test_exec
  - The test harness will internally spawn a child process and set RUN_CHILD=1 to execute craftedFile() under the preload.

The following are two source files to implement the described test suite:
1) test_main.cpp - test harness and test cases
2) preload_fatal.cpp - preload library that overrides FatalError for the test

*/

#include <iccjpeg.h>
#include <cstdio>
#include <fstream>
#include <jpeglib.h>
#include <iostream>
#include <dlfcn.h>
#include <cstdlib>
#include <sys/wait.h>
#include <sys/types.h>
#include <limits.h>
#include <unistd.h>
#include <utils.h>


// Forward declaration of the focal method implemented in the production C source.
// It is declared with C linkage as it is defined in a C translation unit.
extern "C" void craftedFile(void);

// Test harness utilities and tests
namespace {

// Helper: Simple non-terminating assertion/trace
void log_result(const char* test_name, bool passed, int &passed_count, int &total_count) {
    ++total_count;
    if (passed) {
        ++passed_count;
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        std::cout << "[FAIL] " << test_name << "\n";
    }
}

// Test 1: Verify that the symbol craftedFile exists in the current binary.
// We avoid calling the function here to prevent in-process termination.
// Rationale: symbol presence ensures the function is linked and exposed as expected.
bool test_symbol_present_in_binary() {
    // Use RTLD_DEFAULT to refer to the main program
    void* handle = dlopen(nullptr, RTLD_LAZY);
    if (!handle) {
        std::cerr << "dlopen failed to obtain handle for the current binary.\n";
        return false;
    }
    // Look up the symbol; expect it to be non-null
    void* sym = dlsym(handle, "craftedFile");
    dlclose(handle);
    return (sym != nullptr);
}

// Test 2: Verify that craftedFile triggers FatalError via a preloaded override.
// Strategy: Run a child process of this binary with LD_PRELOAD pointing to a preload library
// which defines FatalError. The preload writes a flag file and exits, allowing
// the parent to confirm invocation without terminating the test harness.
bool test_craftedFile_triggersFatalError_via_preload(const char* preloadLibPath) {
    if (!preloadLibPath) {
        std::cerr << "Preload library path not provided. Skipping test.\n";
        return false;
    }

    // Determine path to the current executable
    char exe_path[PATH_MAX] = {0};
    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (len <= 0) {
        std::cerr << "Failed to resolve executable path for child invocation.\n";
        return false;
    }
    exe_path[len] = '\0';

    // Paths used by the preload to signal the test
    const char* flagPath = "/tmp/craftedfile_fatal_called.flag";

    // Ensure previous flag is removed
    remove(flagPath);

    // Fork a child process that will execute the same binary with RUN_CHILD=1
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed.\n";
        return false;
    }

    if (pid == 0) {
        // Child process
        // Indicate to the binary to execute craftedFile()
        setenv("RUN_CHILD", "1", 1);
        // Provide path to the preload library via environment variable
        setenv("PRELOAD_LIB", preloadLibPath, 1);
        // Trigger the child to run the same executable
        execl(exe_path, exe_path, (char*)nullptr);
        // If exec fails, exit with non-zero
        _exit(2);
    }

    // Parent process: wait for child to finish
    int status = 0;
    if (waitpid(pid, &status, 0) == -1) {
        std::cerr << "Failed to wait for child process.\n";
        return false;
    }

    // We expect the child to exit successfully (0) due to FatalError override
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        // Validate that the preload wrote the flag file
        std::ifstream f(flagPath);
        bool flagExists = f.good();
        if (f.good()) f.close();
        // Cleanup
        remove(flagPath);
        return flagExists;
    }

    // Child did not exit cleanly with code 0
    return false;
}

} // end anonymous namespace

int main(int argc, char* argv[]) {
    // Early execution path for the child to run craftedFile directly.
    // The child is launched with environment variable RUN_CHILD=1 by the test harness.
    if (getenv("RUN_CHILD") != nullptr) {
        // Call the focal function. In the test harness environment, this call
        // will be intercepted by the preload library (FatalError override).
        craftedFile();
        // If FatalError does not terminate, exit gracefully to avoid hanging tests.
        return 0;
    }

    // Test harness execution
    int total = 0;
    int passed = 0;

    // Step: Candidate knowledge notes (embedded as comments for traceability)
    // Candidate Keywords extracted from the focal class dependencies:
    // - craftedFile, FatalError, utils.h, jpeglib.h, iccjpeg.h
    // - Class dependencies and helpers: my_error_exit, IsITUFax, SetITUFax, Lab conversion
    // - Domain hints: static members, proper usage of <namespace>, no GTest, use simple asserts

    // -------------------
    // Test 1: Symbol presence
    // -------------------
    bool t1 = test_symbol_present_in_binary();
    log_result("Symbol presence: craftedFile is exportable from the binary", t1, passed, total);

    // -------------------
    // Test 2: Invocation via LD_PRELOAD override
    // -------------------
    // Obtain path to preload library from environment variable set by the build/run script.
    const char* preloadLibPath = std::getenv("PRELOAD_LIB");
    bool t2 = test_craftedFile_triggersFatalError_via_preload(preloadLibPath);
    log_result("Invocation via LD_PRELOAD causes FatalError (intercepted by preload)", t2, passed, total);

    // Summary
    std::cout << "Test suite finished. Passed " << passed << " of " << total << " tests.\n";

    return (passed == total) ? 0 : 1;
}

/*
Notes for maintainers:
- This test suite requires a preload-capable runtime (Linux/POSIX) and a compatible compiler.
- Build steps (example):
  1) Compile the focal code (jpgicc.c) as part of the test binary:
     g++ test_main.cpp jpgicc.c -o test_exec -ldl
  2) Compile the preload library:
     g++ -fPIC -shared preload_fatal.cpp -o libfatal_override.so
  3) Run tests with the preload:
     PRELOAD_LIB=/path/to/libfatal_override.so ./test_exec
- The preload library writes a flag file to /tmp to signal that FatalError was invoked.
- A separate test can be run without LD_PRELOAD to verify symbol presence or baseline behavior (not attempting to call craftedFile in-process to avoid termination).
*/

/* 
Content for preload_fatal.cpp (preload library):
This file should be compiled into a shared library that overrides the FatalError(char*) symbol.
It records that FatalError was invoked and exits the child process gracefully to allow test verification
without killing the test harness process.

Compile:
g++ -fPIC -shared preload_fatal.cpp -o libfatal_override.so

Place the file in a path accessible to the test harness and set PRELOAD_LIB to that path when running the tests.

Code (preload_fatal.cpp):


extern "C" void FatalError(const char* msg)
{
    // Log the call for verification
    FILE* f = std::fopen("/tmp/craftedfile_fatal_called.log", "w");
    if (f) {
        std::fprintf(f, "FatalError invoked with message: %s\n", msg ? msg : "");
        std::fclose(f);
    }

    // Create a simple flag to indicate the invocation happened
    FILE* flag = std::fopen("/tmp/craftedfile_fatal_called.flag", "w");
    if (flag) {
        std::fprintf(flag, "1");
        std::fclose(flag);
    }

    // Terminate the child process gracefully so the test harness can continue
    std::exit(0);
}
*/