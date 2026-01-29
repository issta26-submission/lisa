/*
Candidate Keywords and Dependencies (derived from the focal method main):
- argc, argv
- FILE *f
- buf, siz_buf
- fopen, fseek, ftell, rewind
- SEEK_END
- malloc, free
- fread
- LLVMFuzzerTestOneInput
- goto err
- err label
- fprintf(stderr, ...)
- return value (0)
This test suite targets the fuzz_main executable behavior by driving its public I/O via the
command line interface and inspecting standard error/output to exercise the different
branches of the main function as described in Step 1 and Step 2.
Note: This test suite does not use GTest. It spawns the fuzz_main process and validates
expected behavior by inspecting stdout/stderr files.
*/

#include <fstream>
#include <limits.h>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <iostream>
#include <vector>
#include <cstdio>
#include <stdlib.h>
#include <sys/stat.h>
#include <cstdlib>
#include <stdint.h>
#include <string>
#include <stdio.h>


// Simple test harness for fuzz_main behavior without relying on a specific test framework.
// It executes the fuzz_main binary with various arguments and captures stdout/stderr for assertions.

struct RunResult {
    int exit_code;
    std::string stdout_str;
    std::string stderr_str;
};

// Utility: check if a file exists and is executable
static bool file_exists(const std::string& path) {
    return access(path.c_str(), X_OK) == 0;
}

// Utility: read contents of a text file
static std::string read_file(const std::string& path) {
    std::ifstream ifs(path, std::ios::in | std::ios::binary);
    if (!ifs) return "";
    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    return content;
}

// Utility: create a unique temp file path in /tmp
static std::string make_temp_path(const std::string& suffix = "") {
    static int counter = 0;
    pid_t pid = getpid();
    char path[256];
    snprintf(path, sizeof(path), "/tmp/fuzz_test_%d_%d%s", (int)pid, counter++, suffix.c_str());
    return std::string(path);
}

// Utility: execute fuzz_main with given arguments and capture stdout/stderr
static RunResult run_fuzz_main(const std::vector<std::string>& args) {
    // Resolve fuzz_main binary path
    const char* env_path = std::getenv("FUZZ_MAIN_BIN");
    std::string fuzz_main_path = (env_path && env_path[0] != '\0') ? std::string(env_path) : "./fuzz_main";

    RunResult res;
    res.exit_code = -1;
    res.stdout_str.clear();
    res.stderr_str.clear();

    if (!file_exists(fuzz_main_path)) {
        // If fuzz_main isn't found, we cannot run tests; return a clear error state.
        res.exit_code = -1;
        res.stderr_str = "fuzz_main binary not found: " + fuzz_main_path;
        return res;
    }

    // Build command: fuzz_main with args, capture stdout and stderr separately
    std::string stdout_file = make_temp_path("stdout.txt");
    std::string stderr_file = make_temp_path("stderr.txt");

    std::string cmd = "\"" + fuzz_main_path + "\"";
    for (const auto& a : args) {
        cmd += " \"" + a + "\"";
    }
    // Redirect stdout and stderr to files
    cmd += " > \"" + stdout_file + "\" 2> \"" + stderr_file + "\"";

    int status = system(cmd.c_str());
    // Determine exit code (POSIX: WEXITSTATUS on wait(2) return)
    int exit_code = -1;
#if defined(_POSIX_VERSION)
    if (WIFEXITED(status))
        exit_code = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        exit_code = 128 + WTERMSIG(status);
#else
    exit_code = status;
#endif

    res.exit_code = exit_code;
    res.stdout_str = read_file(stdout_file);
    res.stderr_str = read_file(stderr_file);

    // Cleanup
    remove(stdout_file.c_str());
    remove(stderr_file.c_str());

    return res;
}

// Utility: case-insensitive substring search (simple)
static bool contains(const std::string& hay, const std::string& needle) {
    return hay.find(needle) != std::string::npos;
}

// Test 1: No input file argument should produce "no input file" message on stderr
static bool test_no_input_file(const std::string& fuzz_main_path) {
    (void)fuzz_main_path; // not used directly here; path resolution is inside run_fuzz_main
    RunResult r = run_fuzz_main({}); // no args
    bool ok = (r.exit_code == 0) && contains(r.stderr_str, "no input file");
    if (!ok) {
        std::cerr << "[Test 1] Expected stderr to contain \"no input file\" and exit 0. "
                  << "Got exit_code=" << r.exit_code
                  << ", stderr=\"" << r.stderr_str << "\"\n";
    } else {
        std::cout << "[Test 1] Passed: no input file branch exercised.\n";
    }
    return ok;
}

// Test 2: Non-existent input file should produce "error opening input file" on stderr
static bool test_open_failure(const std::string& fuzz_main_path) {
    // Use a path that almost certainly does not exist
    std::string fake_path = "/tmp/non_existent_input_file_should_not_exist_XXXXXX";
    RunResult r = run_fuzz_main({"", fake_path}); // ensure argument handling includes a file path
    bool ok = (r.exit_code == 0) && contains(r.stderr_str, "error opening input file");
    if (!ok) {
        std::cerr << "[Test 2] Expected stderr to contain \"error opening input file\" and exit 0. "
                  << "Got exit_code=" << r.exit_code
                  << ", stderr=\"" << r.stderr_str << "\"\n";
    } else {
        std::cout << "[Test 2] Passed: open failure branch exercised.\n";
    }
    return ok;
}

// Test 3: Empty input file should exit gracefully (no additional error messages)
static bool test_empty_input_file(const std::string& fuzz_main_path) {
    std::string path = make_temp_path("empty.txt");
    // Create empty file
    {
        FILE* f = fopen(path.c_str(), "wb");
        if (f) fclose(f);
        else {
            std::cerr << "[Test 3] Failed to create empty temp file: " << path << "\n";
            return false;
        }
    }
    RunResult r = run_fuzz_main({ path });
    bool ok = (r.exit_code == 0);
    // The code path for siz_buf < 1 doesn't print an error message, so stderr should be empty
    if (!ok) {
        std::cerr << "[Test 3] Expected exit code 0 for empty file. Got exit_code=" << r.exit_code
                  << ", stderr=\"" << r.stderr_str << "\"\n";
    } else if (!r.stderr_str.empty()) {
        std::cerr << "[Test 3] Expected empty stderr for empty file. Got: " << r.stderr_str << "\n";
        ok = false;
    } else {
        std::cout << "[Test 3] Passed: empty input file branch exercised.\n";
    }
    std::remove(path.c_str());
    return ok;
}

// Test 4: Normal input path should attempt to process data via LLVMFuzzerTestOneInput
// This test assumes the fuzz_main binary is linked with a valid LLVMFuzzerTestOneInput
// implementation. We simply verify that the program runs to completion and returns 0.
static bool test_normal_path(const std::string& fuzz_main_path) {
    std::string path = make_temp_path("data.bin");
    // Create a small non-empty file
    {
        FILE* f = fopen(path.c_str(), "wb");
        if (f) {
            const char payload[] = { 0x00, 0x01, 0x02, 0x03, 0x04 };
            fwrite(payload, sizeof(payload), 1, f);
            fclose(f);
        } else {
            std::cerr << "[Test 4] Failed to create temp input file: " << path << "\n";
            return false;
        }
    }
    RunResult r = run_fuzz_main({ path });
    bool ok = (r.exit_code == 0);
    if (!ok) {
        std::cerr << "[Test 4] Expected exit code 0 for normal path. Got exit_code=" << r.exit_code
                  << ", stderr=\"" << r.stderr_str << "\"\n";
    } else {
        std::cout << "[Test 4] Passed: normal path exercised (may invoke LLVMFuzzerTestOneInput).\n";
    }
    std::remove(path.c_str());
    return ok;
}

// Entry point: execute all tests and report summary
int main() {
    // Resolve fuzz_main binary path for tests
    const char* env_path = std::getenv("FUZZ_MAIN_BIN");
    std::string fuzz_main_path = (env_path && env_path[0] != '\0') ? std::string(env_path) : "./fuzz_main";

    int total = 0;
    int passed = 0;

    std::cout << "Starting fuzz_main unit tests (without GTest). This harness uses the actual fuzz_main binary.\n";

    // Test 1: No input file argument
    total++;
    if (test_no_input_file(fuzz_main_path)) ++passed;

    // Test 2: Non-existent input file
    total++;
    if (test_open_failure(fuzz_main_path)) ++passed;

    // Test 3: Empty input file
    total++;
    if (test_empty_input_file(fuzz_main_path)) ++passed;

    // Test 4: Normal input path
    total++;
    if (test_normal_path(fuzz_main_path)) ++passed;

    std::cout << "Tests completed: " << passed << "/" << total << " passed.\n";
    return (total == 0) ? 0 : (passed == total ? 0 : 1);
}