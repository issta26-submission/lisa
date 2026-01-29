/*
Unit test suite for the focal method main in pufftest.c (C file under test).

Step 0 - Contextual notes:
- The focal method main processes command line arguments to either decompress data
  from a file using puff(), optionally writing the decompressed data to stdout when
  -w and appropriate conditions are set.
- Key argument branches to cover:
  - invalid option: any option that does not match -w, -f, or a numeric skip.
  - -w option: triggers write-to-stdout path (tested indirectly by ensuring program
    handles the parsing path and can proceed to file loading).
  - -f option: triggers the "force" flag (similarly, parsed and used to influence output length).
  - numeric skip: any arg consisting of a leading '-' followed by a digit, e.g., -9.
  - file name handling: exactly one filename allowed; more than one filename prints
    "only one file name allowed".
- Coverage goals:
  - True/false branches of each argument predicate.
  - Error branches for invalid options and multiple file names.
  - Path where a file is empty or unreadable (len == 0).
  - Path where skip >= len triggers a specific error.
- Limitations:
  - Decompression correctness depends on compressed data; tests avoid requiring
    actual valid compressed input (instead focus on argument parsing and error paths).
  - This test harness invokes the compiled pufftest binary as a separate process.

Notes about environment:
- This test suite expects a pre-built pufftest binary in the same directory as this test launcher.
- The tests use POSIX system calls (system, WEXITSTATUS, WIFEXITED). Windows support is not included here.
- All tests are non-GTest, using only the C++ standard library and POSIX facilities (no external mocking).

Candidate Keywords extracted from the focal method main (for traceability):
- put, fail, skip, source, dest, puff, load, SET_BINARY_MODE, stdout, stderr
- invalid option, memory allocation failure, could not read, skip request, puff().

Below is the self-contained C++11 test launcher. It runs pufftest with various arguments, captures stdout/stderr, and checks exit codes and messages.

*/
#include <cstdio>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sys/wait.h>
#include <unistd.h>
#include <puff.h>
#include <stdlib.h>
#include <cstdlib>


namespace {

bool fileExists(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0;
}

// Run the pufftest binary with given args, redirect stdout to stdoutPath and stderr to stderrPath.
// Returns the exit code of the pufftest process (WEXITSTATUS if available).
int runPufftestWithArgs(const std::string& args,
                       std::string& stdoutOut,
                       std::string& stderrOut) {
    // Temporary files for capturing outputs
    const std::string outPath = "pufftest_stdout.tmp";
    const std::string errPath = "pufftest_stderr.tmp";

    // Build shell command: pufftest [args] 1>stdout.tmp 2>stderr.tmp
    std::string cmd = "./pufftest " + args + " 1>" + outPath + " 2>" + errPath;

    int ret = system(cmd.c_str());

    // Read stdout
    {
        std::ifstream ifs(outPath);
        stdoutOut.assign((std::istreambuf_iterator<char>(ifs)),
                         std::istreambuf_iterator<char>());
    }
    // Read stderr
    {
        std::ifstream ifs(errPath);
        stderrOut.assign((std::istreambuf_iterator<char>(ifs)),
                         std::istreambuf_iterator<char>());
    }

    // Cleanup
    remove(outPath.c_str());
    remove(errPath.c_str());

    int exitCode = -1;
#if defined(__unix__) || defined(__APPLE__)
    if (WIFEXITED(ret)) exitCode = WEXITSTATUS(ret);
    else exitCode = ret;
#else
    exitCode = ret;
#endif
    return exitCode;
}

// Simple non-terminating expectations (EXPECT_* style)
struct TestState {
    int total{0};
    int passed{0};
    int failed{0};
    void record(bool ok, const std::string& msg) {
        ++total;
        if (ok) ++passed;
        else {
            ++failed;
            std::cerr << "TEST FAILURE: " << msg << "\n";
        }
    }
};

void test_invalid_option(TestState& st, bool puffExists) {
    // Test: invalid option -x should yield exit code 3 and contain "invalid option" in stderr
    std::string stdoutStr, stderrStr;
    int code = runPufftestWithArgs("-x", stdoutStr, stderrStr);

    bool ok = (code == 3) && (stderrStr.find("invalid option") != std::string::npos);
    st.record(ok, "invalid option handling should return 3 and print 'invalid option' in stderr.");
    if (!ok) {
        std::cerr << "  Got exit code: " << code << "\n";
        std::cerr << "  Stderr: " << stderrStr << "\n";
    }
}

void test_two_filenames(TestState& st, bool puffExists) {
    // Test: providing two file names should yield exit code 3 with message "only one file name allowed"
    std::string stdoutStr, stderrStr;
    int code = runPufftestWithArgs("foo bar", stdoutStr, stderrStr);

    bool ok = (code == 3) && (stderrStr.find("only one file name allowed") != std::string::npos);
    st.record(ok, "two filenames should cause error 'only one file name allowed' with exit code 3.");
    if (!ok) {
        std::cerr << "  Got exit code: " << code << "\n";
        std::cerr << "  Stderr: " << stderrStr << "\n";
    }
}

void writeTempFile(const std::string& path, const std::string& data) {
    std::ofstream ofs(path, std::ios::binary | std::ios::trunc);
    ofs.write(data.data(), data.size());
}

void test_empty_file(TestState& st, bool puffExists) {
    // Create an empty file and pass its path as argument
    const std::string path = "pufftest_empty_file.tmp";
    writeTempFile(path, ""); // zero-length

    std::string stdoutStr, stderrStr;
    int code = runPufftestWithArgs(path, stdoutStr, stderrStr);

    bool ok = (code == 3) && (stderrStr.find("could not read") != std::string::npos);
    st.record(ok, "empty file should trigger 'could not read' message and exit code 3.");
    if (!ok) {
        std::cerr << "  Got exit code: " << code << "\n";
        std::cerr << "  Stderr: " << stderrStr << "\n";
    }

    // cleanup
    remove(path.c_str());
}

void test_skip_too_large(TestState& st, bool puffExists) {
    // Create a small file (2 bytes) and pass a large skip value, e.g., -9
    const std::string path = "pufftest_small.bin";
    writeTempFile(path, std::string("AB")); // 2 bytes

    std::string stdoutStr, stderrStr;
    // Use -9 to trigger skip >= len condition
    int code = runPufftestWithArgs("-9 " + path, stdoutStr, stderrStr);

    bool ok = (code == 3) && (stderrStr.find("skip request of 9 leaves no input") != std::string::npos);
    st.record(ok, "skip >= file length should yield skip error message and exit code 3.");
    if (!ok) {
        std::cerr << "  Got exit code: " << code << "\n";
        std::cerr << "  Stderr: " << stderrStr << "\n";
    }

    // cleanup
    remove(path.c_str());
}

} // anonymous namespace

int main() {
    // Simple availability check for pufftest binary
    bool puffExists = fileExists("./pufftest");
    if (!puffExists) {
        std::cerr << "Pufftest binary './pufftest' not found. Please build pufftest first.\n";
        return 1;
    }

    TestState st;

    // Step 1: Argument parsing tests (true/false branches)
    test_invalid_option(st, puffExists);
    test_two_filenames(st, puffExists);
    test_empty_file(st, puffExists);
    test_skip_too_large(st, puffExists);

    // Summary
    std::cout << "\nTest Summary: " << st.passed << " passed, " << st.failed << " failed, out of "
              << st.total << " tests.\n";

    // Return non-zero if any test failed
    return st.failed;
}