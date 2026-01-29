/*
  Test suite for the focal method main() in gznorm.c
  Notes:
  - This test suite uses non-GTest style assertions and a small harness
    to exercise the main() function as an integration point.
  - The tests rely on running the gznorm binary as a separate process to
    exercise its behavior with controlled stdin/stdout/stderr.
  - Two tests are implemented:
      1) Basic path: when gzip_normalize reports success (ret == 0)
      2) Error path override via LD_PRELOAD (ret == 1) using a custom shared library
  - This harness is intended for Linux-like environments where LD_PRELOAD is supported.
  - It avoids private/private-implementation access; it uses external process behavior.
  - The test code is self-contained and does not require GTest; it uses a small internal test runner.
  - If the dynamic override is not possible (static linkage), the second test will be skipped gracefully.
*/

/* Candidate Keywords extracted from the focal method and its dependencies:
   - gzip_normalize
   - SET_BINARY_MODE
   - stdin, stdout
   - err handling, free(err)
   - ret return code
   - gznorm error: format
   - standard IO redirection
   - LD_PRELOAD dynamic override
   - C I/O: FILE*, fprintf, printf
*/

/* Domain knowledge considerations implemented:
   - Use only standard library facilities and basic system calls (no GTest).
   - Tests are executable from main() and print clear results.
   - Avoid private/internal details; test behavior via integration points.
   - Provide explanations for each test in comments.
*/

#include <cstdio>
#include <cstring>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <vector>
#include <zlib.h>
#include <fstream>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>


using std::string;
using std::cout;
using std::endl;

/* Global counters for minimal test reporting */
static int g_total_tests = 0;
static int g_passed_tests = 0;

/* Helper: read entire file into a string */
static std::string readFile(const std::string& path) {
    std::ifstream ifs(path.c_str(), std::ios::in | std::ios::binary);
    if (!ifs) return "";
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        std::istreambuf_iterator<char>());
    return content;
}

/* Helper: write data to a temporary file and return its path */
static std::string writeTempFile(const std::string& data) {
    char tmpl[] = "/tmp/gznorm_test_XXXXXX";
    int fd = mkstemp(tmpl);
    if (fd == -1) return "";
    // Ensure we have a valid C string path to return
    std::string path(tmpl);
    if (!data.empty()) {
        write(fd, data.data(), data.size());
    }
    close(fd);
    return path;
}

/* Helper: remove a file if it exists */
static void removeFile(const std::string& path) {
    if (!path.empty()) {
        unlink(path.c_str());
    }
}

/* Core helper: Run the gznorm binary with given input content.
   It creates temp files for input/out/err, runs the binary,
   and captures stdout, stderr, and exit code. Optional LD_PRELOAD
   (preload) can be used to override gzip_normalize for testing error path.
*/
static bool runGznormWithInput(const std::string& input,
                              std::string& stdout_out,
                              std::string& stderr_out,
                              int& exit_code,
                              const std::string& preload = "")
{
    // Prepare temp files
    std::string in_path  = writeTempFile(input);  // stores input
    std::string out_path = writeTempFile("");     // will capture stdout
    std::string err_path = writeTempFile("");     // will capture stderr

    if (in_path.empty() || out_path.empty() || err_path.empty()) {
        // Cleanup any created files (best effort)
        removeFile(in_path);
        removeFile(out_path);
        removeFile(err_path);
        return false;
    }

    // Build command line
    std::string cmd;
    if (!preload.empty()) {
        cmd = "LD_PRELOAD=" + preload + " ";
    }
    // Redirect stdin from in_path, stdout to out_path, stderr to err_path
    cmd += "./gznorm < " + in_path + " > " + out_path + " 2> " + err_path;

    int status = system(cmd.c_str());
    if (status == -1) {
        removeFile(in_path);
        removeFile(out_path);
        removeFile(err_path);
        return false;
    }

    // Get exit code
    if (WIFEXITED(status)) {
        exit_code = WEXITSTATUS(status);
    } else {
        exit_code = -1;
    }

    // Read outputs
    stdout_out = readFile(out_path);
    stderr_out = readFile(err_path);

    // Cleanup
    removeFile(in_path);
    removeFile(out_path);
    removeFile(err_path);

    return true;
}

/* Test 1: Basic path - gzip_normalize returns 0 (success)
   Expect: main() exits with code 0, and nothing is written to stdout/stderr.
   Rationale: Validates the non-error path in main when gzip_normalize signals success.
*/
static bool test_basic_success_path() {
    std::string stdout_out, stderr_out;
    int exit_code = -1;

    // Empty input is used to exercise stdin path
    if (!runGznormWithInput("", stdout_out, stderr_out, exit_code)) {
        cout << "[Test Basic Success] FAILED: could not run gznorm.\n";
        return false;
    }

    bool ok = true;
    if (exit_code != 0) {
        cout << "[Test Basic Success] FAILED: expected exit code 0, got " << exit_code << "\n";
        ok = false;
    }
    if (!stdout_out.empty()) {
        cout << "[Test Basic Success] FAILED: expected empty stdout, got length "
             << stdout_out.size() << "\n";
        ok = false;
    }
    if (!stderr_out.empty()) {
        cout << "[Test Basic Success] FAILED: expected empty stderr, got length "
             << stderr_out.size() << "\n";
        ok = false;
    }

    if (ok) {
        cout << "[Test Basic Success] PASSED: main() returned 0 with no IO on success path.\n";
    }
    return ok;
}

/* Test 2: Error path via LD_PRELOAD override
   Intention: Force gzip_normalize to return an error by providing a mock
   implementation through LD_PRELOAD that returns ret = 1 and sets
   err string to "mock error".
   Behavior: main() should print "gznorm error: mock error" to stderr and return 1.
   Note: This test depends on gzip_normalize not being static in the binary.
         If static linkage is used, this test will gracefully be skipped.
*/
static bool test_error_path_via_preload() {
#if defined(__linux__)
    // 1) Create a small C source that overrides gzip_normalize
    const char* src_path = "/tmp/libgzip_override.c";
    const char* so_path  = "/tmp/libgzip_override.so";
    const char* code =
        "#include <stdio.h>\n"
        "#include <stdlib.h>\n"
        "#include <string.h>\n"
        "\n"
        "int gzip_normalize(FILE *in, FILE *out, char **err) {\n"
        "  (void)in; (void)out;\n"
        "  if (err) *err = (char*)malloc(64);\n"
        "  if (err && *err) strcpy(*err, \"mock error\");\n"
        "  return 1; // force error path\n"
        "}\n";

    // Write source file
    FILE* f = fopen("/tmp/libgzip_override.c", "w");
    if (!f) {
        cout << "[Test Error Via Preload] SKIPPED: cannot write override source.\n";
        return false;
    }
    fwrite(code, 1, strlen(code), f);
    fclose(f);

    // Compile as a shared library
    std::string compileCmd = "gcc -shared -fPIC -o /tmp/libgzip_override.so /tmp/libgzip_override.c";
    int retCompile = system(compileCmd.c_str());
    if (retCompile != 0) {
        cout << "[Test Error Via Preload] SKIPPED: failed to compile override library.\n";
        return false;
    }

    // Now run gznorm with LD_PRELOAD to override gzip_normalize
    std::string stdout_out, stderr_out;
    int exit_code = -1;
    if (!runGznormWithInput("", stdout_out, stderr_out, exit_code, "/tmp/libgzip_override.so")) {
        cout << "[Test Error Via Preload] FAILED: could not run gznorm with preload.\n";
        return false;
    }

    // Validate outcomes
    bool ok = true;
    if (exit_code != 1) {
        cout << "[Test Error Via Preload] FAILED: expected exit code 1, got " << exit_code << "\n";
        ok = false;
    }
    // The error message should be printed to stderr according to gznorm's main
    if (stderr_out.find("gznorm error: mock error") == std::string::npos) {
        cout << "[Test Error Via Preload] FAILED: expected error message on stderr.\n";
        ok = false;
    }

    if (ok) {
        cout << "[Test Error Via Preload] PASSED: error path exercised via LD_PRELOAD override.\n";
    }

    return ok;
#else
    cout << "[Test Error Via Preload] SKIPPED: LD_PRELOAD test not supported on this platform.\n";
    return false;
#endif
}

/* Entry point: Run all tests from main() as per the instructions.
   We print a short summary and return non-zero if any test fails.
*/
int main(void) {
    std::cout << "Starting gznorm main() test suite (non-GTest, integration-style).\n";

    // Run individual tests
    g_total_tests = 0; // compile-time trick not used; we re-use globals later
    g_total_tests = 0;
    g_passed_tests = 0;

    // Test 1: basic success path
    g_total_tests++;
    bool t1 = test_basic_success_path();
    if (t1) g_passed_tests++;

    // Test 2: error path via preload (best-effort)
    g_total_tests++;
    bool t2 = test_error_path_via_preload();
    if (t2) g_passed_tests++;

    // Summary
    std::cout << "Test results: " << g_passed_tests << "/" << g_total_tests << " tests passed.\n";
    if (g_passed_tests == g_total_tests) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED\n";
        return 1;
    }
}