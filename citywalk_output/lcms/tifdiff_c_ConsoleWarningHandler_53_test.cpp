// Unit tests for ConsoleWarningHandler in tifdiff.c
// Target: C++11, no Google Test. Tests rely on the actual ConsoleWarningHandler
// implementation and the global Verbose flag as defined in the project's sources.
// The tests capture stderr output using a temporary file (tmpfile) to verify
// exact formatting and branching behavior.

#include <cstdio>
#include <memory>
#include <iostream>
#include <string>
#include <cstdarg>
#include <unistd.h>
#include <utils.h>
#include <tiffio.h>


// Forward declarations to interface with the focal C code.
// We assume the project defines Verbose and ConsoleWarningHandler with C linkage.
extern "C" {
    // In the real project, Verbose is a global flag controlling verbose output.
    // It is declared as an int in the C code.
    extern int Verbose;

    // Focal function under test. Signature matches the C definition.
    void ConsoleWarningHandler(const char* module, const char* fmt, va_list ap);
}

// Helper: invoke ConsoleWarningHandler with a va_list and capture stderr output
// by redirecting it to a temporary file (tmpfile). This allows us to verify
// the exact output produced when Verbose is enabled.
static std::string InvokeConsoleWarning(const char* module, const char* fmt, va_list ap) {
    // Create a temporary file to capture stderr output
    FILE* tmp = tmpfile();
    if (!tmp) {
        return "";
    }

    // Save current stderr
    int saved_stderr = dup(STDERR_FILENO);
    if (saved_stderr == -1) {
        fclose(tmp);
        return "";
    }

    // Redirect stderr to the temporary file
    int tmp_fd = fileno(tmp);
    if (dup2(tmp_fd, STDERR_FILENO) == -1) {
        // Restore and cleanup on failure
        dup2(saved_stderr, STDERR_FILENO);
        close(saved_stderr);
        fclose(tmp);
        return "";
    }

    // Ensure any buffered data is written to the temp file by flushing
    fflush(stderr);

    // Call the focal function with the provided va_list
    ConsoleWarningHandler(module, fmt, ap);

    // Flush and restore stderr
    fflush(stderr);
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stderr);

    // Read captured data from the temporary file
    fseek(tmp, 0, SEEK_SET);
    std::string output;
    const size_t BUF_SIZE = 1024;
    char buffer[BUF_SIZE];
    while (true) {
        size_t n = fread(buffer, 1, BUF_SIZE, tmp);
        if (n > 0) {
            output.append(buffer, n);
        }
        if (n < BUF_SIZE) break;
    }

    fclose(tmp);
    return output;
}

// Convenience: capture ConsoleWarningHandler output by supplying variadic args
static std::string CaptureWarning(const char* module, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    // Copy va_list so we can pass a stable list to the function
    va_list ap_copy;
    va_copy(ap_copy, ap);
    std::string result = InvokeConsoleWarning(module, fmt, ap_copy);
    va_end(ap_copy);
    va_end(ap);
    return result;
}

// Helper: test wrapper to run a test function and report its result
static bool RunTest(const std::string& name, bool (*testFunc)()) {
    bool ok = testFunc();
    if (ok) {
        std::cout << "[PASS] " << name << std::endl;
    } else {
        std::cout << "[FAIL] " << name << std::endl;
    }
    return ok;
}

// Test 1: Module prefix and formatting when module is provided
// - Verifies that output contains a leading newline, the word "Warning",
//   the module prefix followed by ": ", and the final formatted message ending with a dot.
// - Branch: module != NULL (true branch).
static bool Test_ModulePrefixAndFormat_WithModule() {
    int oldVerbose = Verbose;
    Verbose = 1;
    const char* module = "mod";
    const char* fmt = "Hello %s";
    std::string out = CaptureWarning(module, fmt, "World");
    Verbose = oldVerbose;

    const std::string expected = "\nWarning mod: Hello World.\n";
    return out == expected;
}

// Test 2: No module prefix when module is NULL
// - Verifies that output does not include a module prefix.
// - Branch: module != NULL (false branch).
static bool Test_ModulePrefix_WithNullModule() {
    int oldVerbose = Verbose;
    Verbose = 1;
    const char* module = nullptr;
    const char* fmt = "A=%d";
    std::string out = CaptureWarning(module, fmt, 5);
    Verbose = oldVerbose;

    const std::string expected = "\nWarning A=5.\n";
    return out == expected;
}

// Test 3: Verbose off emits no output
// - Ensures no writes to stderr when Verbose is false.
static bool Test_VerboseOffProducesNoOutput() {
    int oldVerbose = Verbose;
    Verbose = 0;
    const char* module = "mod";
    std::string out = CaptureWarning(module, "Should %s", "not print");
    Verbose = oldVerbose;

    return out.empty();
}

// Test 4: Another representative case with different formatting
// - Confirms correct handling of different content and module prefix.
static bool Test_AlternateFormatting_WithModule() {
    int oldVerbose = Verbose;
    Verbose = 1;
    const char* module = "API";
    const char* fmt = "Value=%d, Status=%s";
    std::string out = CaptureWarning(module, fmt, 42, "OK");
    Verbose = oldVerbose;

    const std::string expected = "\nWarning API: Value=42, Status=OK.\n";
    return out == expected;
}

// Main function: run all tests and summarize results
int main() {
    // Run tests and collect results
    int passed = 0;
    int total = 0;

    if (RunTest("ModulePrefixAndFormat_WithModule", Test_ModulePrefixAndFormat_WithModule)) ++passed;
    ++total;

    if (RunTest("ModulePrefix_WithNullModule", Test_ModulePrefix_WithNullModule)) ++passed;
    ++total;

    if (RunTest("VerboseOffProducesNoOutput", Test_VerboseOffProducesNoOutput)) ++passed;
    ++total;

    if (RunTest("AlternateFormatting_WithModule", Test_AlternateFormatting_WithModule)) ++passed;
    ++total;

    // Summary
    std::cout << "Test summary: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}

// Notes for maintainers:
// - The tests rely on the project's global Verbose flag and the ConsoleWarningHandler
//   function defined in tifdiff.c. They simulate realistic usage and verify both
//   true and false branches of the module != NULL conditional.
// - Output is captured via a temporary file (tmpfile) to avoid interfering with
//   the process's standard streams and to enable exact string comparison.
// - All tests are designed to run under C++11 without GTest; they report PASS/FAIL
//   and do not terminate prematurely on failure, allowing full coverage of the suite.