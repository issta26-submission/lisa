// Lightweight, self-contained unit tests for the ConsoleErrorHandler function.
// Targets C++11 and avoids GTest by providing a small test harness.
// The tests redirect/redirected stderr content to a temporary file to verify exact output.

#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <functional>
#include <vector>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <cstdarg>
#include <unistd.h>
#include <utils.h>
#include <tiffio.h>


// Forward declaration of the focal C function (C linkage)
extern "C" void ConsoleErrorHandler(const char* module, const char* fmt, va_list ap);

// Variadic wrapper to call the focal function with a proper va_list
void CallConsoleErrorHandler(const char* module, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    ConsoleErrorHandler(module, fmt, ap);
    va_end(ap);
}

// Lightweight stderr capture using a temporary file (POSIX)
class StderrCapture {
public:
    StderrCapture() : savedStderrFd(-1), tmpFile(nullptr) {}
    ~StderrCapture() { if (tmpFile) fclose(tmpFile); }

    // Begin redirecting stderr to a temporary file
    void start() {
        // Save current stderr
        savedStderrFd = dup(STDERR_FILENO);
        // Create a temporary file to capture output
        tmpFile = tmpfile();
        if (!tmpFile) {
            std::cerr << "Failed to create temporary file for stderr capture.\n";
            return;
        }
        int tmpFd = fileno(tmpFile);
        // Redirect stderr to the temporary file
        dup2(tmpFd, STDERR_FILENO);
        // Close the duplicate fd; the redirected descriptor remains active (STDERR_FILENO)
        close(tmpFd);
    }

    // Stop redirection, restore stderr, and return captured content
    std::string stop() {
        std::string output;

        // Ensure all data is flushed to the file
        fflush(stderr);

        // Restore original stderr
        if (savedStderrFd != -1) {
            dup2(savedStderrFd, STDERR_FILENO);
            close(savedStderrFd);
            savedStderrFd = -1;
        }

        // Read captured content from the temporary file
        if (tmpFile) {
            // Rewind to the beginning to read everything written
            if (fseek(tmpFile, 0, SEEK_SET) == 0) {
                char buf[4096];
                while (fgets(buf, sizeof(buf), tmpFile)) {
                    output.append(buf);
                }
            }
            fclose(tmpFile);
            tmpFile = nullptr;
        }

        return output;
    }

private:
    int savedStderrFd;
    FILE* tmpFile;
};

// Simple non-terminating test assertion helpers
static int gTestsRun = 0;
static int gTestsFailed = 0;

inline void assert_eq(const std::string& got, const std::string& expected, const std::string& description) {
    ++gTestsRun;
    if (got != expected) {
        ++gTestsFailed;
        std::cerr << "TEST FAILED: " << description << "\n";
        std::cerr << "  Expected: [" << expected << "]\n";
        std::cerr << "  Got:      [" << got << "]\n";
    } else {
        // Optional: print per-test success (comment out if noisy)
        // std::cout << "TEST PASSED: " << description << "\n";
    }
}

// Test 1: Ensure the output includes the module prefix when module is non-NULL
// Behavior tested:
// - module != NULL triggers "module: " prefix
// - fmt is formatted with va_list (Value=42)
// - Output ends with a period and newline as per implementation
void test_ConsoleErrorHandler_ModuleNotNull_OutputCorrect() {
    StderrCapture cap;
    cap.start();

    // Call the focal function with a known value
    CallConsoleErrorHandler("mod", "Value=%d", 42);

    // Stop capture and obtain the string written to stderr
    std::string out = cap.stop();

    // Expected exact capture: "\nError mod: Value=42.\n"
    const std::string expected = "\nError mod: Value=42.\n";
    assert_eq(out, expected, "ConsoleErrorHandler should print with module prefix when module != NULL");
}

// Test 2: Ensure the output does not include a module prefix when module is NULL
// Behavior tested:
// - module == NULL results in no "module: " prefix
// - fmt is formatted with va_list (Value=123)
// - Output ends with a period and newline
void test_ConsoleErrorHandler_ModuleNull_OutputCorrect() {
    StderrCapture cap;
    cap.start();

    CallConsoleErrorHandler(nullptr, "Value=%d", 123);

    std::string out = cap.stop();

    // Expected exact capture: "\nError Value=123.\n"
    const std::string expected = "\nError Value=123.\n";
    assert_eq(out, expected, "ConsoleErrorHandler should print without module prefix when module == NULL");
}

// The test runner
int main() {
    std::cout << "Starting ConsoleErrorHandler unit tests...\n";

    // Run tests
    test_ConsoleErrorHandler_ModuleNotNull_OutputCorrect();
    test_ConsoleErrorHandler_ModuleNull_OutputCorrect();

    // Summary
    std::cout << "Tests run: " << gTestsRun << "\n";
    if (gTestsFailed == 0) {
        std::cout << "All tests PASSED.\n";
    } else {
        std::cout << "Tests FAILED: " << gTestsFailed << "\n";
    }

    // Return non-zero if any test failed (helps integration with basic build systems)
    return (gTestsFailed == 0) ? 0 : 1;
}