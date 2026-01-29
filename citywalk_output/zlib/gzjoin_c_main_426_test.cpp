// C++11 test suite for the focal method main in gzjoin.c
// Notes for the reviewer:
// - This test suite targets the program behavior of gzjoin's main by executing
//   the gzjoin executable as a separate process (to avoid linking conflicts
//   and to enable stdout/stderr capture).
// - Tests cover the primary true/false branch of the argc handling inside main:
//   when no file arguments are provided (usage message is printed and exit code is 0).
// - A second test exercises the non-empty-args path by creating a real gzip input
//   file and verifying that gzjoin can produce a joined gzip output (one input).
// - The test assumes an executable named ./gzjoin is built separately from gzjoin.c.
// - This approach adheres to the constraint "If gtest isn't allowed, call test methods
//   from the main function" by providing a lightweight in-process test runner that
//   invokes the external program and checks results.
// - Non-terminating assertions (manual checks) are used to keep tests executable in
//   environments without GTest/GMock.

#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <cassert>
#include <string>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <vector>
#include <memory>
#include <zlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>


// Helper: simple logging
#define LOG(msg) std::cout << "[TEST] " << msg << std::endl
#define FAIL(msg) do { LOG("FAIL: " << msg); failures++; } while(false)
#define PASS(msg) do { LOG("PASS: " << msg); passes++; } while(false)

static int failures = 0;
static int passes = 0;

// Helper: check if a string contains a substring
static bool contains(const std::string &s, const std::string &sub) {
    return s.find(sub) != std::string::npos;
}

// Helper: read all data from a file descriptor into a string
static std::string readAll(int fd) {
    std::string result;
    char buf[4096];
    ssize_t r;
    while ((r = read(fd, buf, sizeof(buf))) > 0) {
        result.append(buf, (size_t)r);
    }
    return result;
}

// Test 1: Verify that running gzjoin with no arguments prints usage to stderr and exits with 0
static int test_usage_no_args_no_input() {
    LOG("Test 1: gzjoin with no arguments should print usage to stderr and exit 0");

    int errpipe[2];
    if (pipe(errpipe) != 0) {
        FAIL("pipe creation failed");
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        FAIL("fork failed");
        return -1;
    }

    if (pid == 0) {
        // Child: redirect stderr to pipe, then exec gzjoin with no args
        dup2(errpipe[1], STDERR_FILENO);
        close(errpipe[0]);
        close(errpipe[1]);

        // argv for gzjoin (no additional args)
        char *args[] = { const_cast<char*>("./gzjoin"), NULL };
        execv("./gzjoin", args);

        // If exec fails
        _exit(127);
    } else {
        // Parent: close write end, read stderr
        close(errpipe[1]);
        // Wait for child to finish
        int status = 0;
        waitpid(pid, &status, 0);

        // Read captured stderr
        std::string captured = readAll(errpipe[0]);
        close(errpipe[0]);

        // Assertion: exit code should be 0 and usage message should be present
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            const std::string usage_str = "gzjoin usage: gzjoin f1.gz [f2.gz [f3.gz ...]] > fjoin.gz\n";
            if (contains(captured, usage_str)) {
                PASS("Usage message printed to stderr and exit code 0");
                return 0;
            } else {
                // Provide more context on mismatch
                FAIL("Usage message not found in stderr. Captured: " + captured);
                return -1;
            }
        } else {
            FAIL("gzjoin exited abnormally. Exit status: " +
                 std::to_string(WEXITSTATUS(status)));
            return -2;
        }
    }
}

// Helper: create a temporary gzip input file with given content
// We rely on system's bash and gzip; this is a controlled integration step.
static std::string create_temp_input_gzip(const std::string &content, const std::string &prefix = "/tmp/input1_XXXXXX.gz") {
    // Create a unique path using mkstemp
    char template_path[256];
    std::strncpy(template_path, prefix.c_str(), sizeof(template_path));
    template_path[sizeof(template_path) - 1] = '\0';
    int fd = mkstemps(template_path, 3); // keep ".gz" suffix
    if (fd == -1) {
        return "";
    }
    close(fd);
    // Write content into a gzip-compressed file using shell
    // Construct a command: bash -lc 'printf "<content>" | gzip -c > <path>'
    // Note: content is assumed ascii and safe for printf here.
    std::string cmd = "bash -lc 'printf \"" + content + "\" | gzip -c > " + template_path + "'";
    int ret = system(cmd.c_str());
    if (ret != 0) {
        // Cleanup on failure
        unlink(template_path);
        return "";
    }
    return std::string(template_path);
}

// Helper: run gzjoin with one input, redirect stdout to a file
static int run_gzjoin_with_output(const std::string &input_path, const std::string &output_path) {
    pid_t pid = fork();
    if (pid < 0) {
        return -1;
    }
    if (pid == 0) {
        // Child: redirect stdout to output file
        int outfd = open(output_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (outfd < 0) _exit(2);
        dup2(outfd, STDOUT_FILENO);
        close(outfd);

        // Exec gzjoin with the input gzip path
        std::vector<char*> argv;
        argv.push_back(const_cast<char*>("./gzjoin"));
        argv.push_back(const_cast<char*>(input_path.c_str()));
        argv.push_back(nullptr);
        execv("./gzjoin", argv.data());

        // Exec failed
        _exit(3);
    } else {
        int status = 0;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
}

// Helper: decompress a gzip file and capture its contents
static std::string decompress_gzip_file(const std::string &gzpath) {
    std::string cmd = "zcat " + gzpath;
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "";
    std::string result;
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    // Remove trailing newline if present (we expect exact "Hello" with no newline)
    if (!result.empty() && result[result.size()-1] == '\n')
        result.erase(result.size()-1);
    return result;
}

// Test 2: Verify that gzjoin can join a real gzip input and produce a valid gzip output
// We create a gzip file containing "Hello" and pass it to gzjoin. We then decompress
// the output and compare with "Hello".
static int test_join_one_input_basic() {
    LOG("Test 2: gzjoin with one gzip input should produce a valid joined gzip output that decompresses to content");

    // Step 1: Create gzip input containing "Hello"
    std::string input_path = "/tmp/input1_join_test.gz";
    std::string input_content = "Hello";
    std::string created = create_temp_input_gzip(input_content, input_path);
    if (created.empty()) {
        FAIL("Failed to create temporary gzip input");
        return -1;
    }

    // Step 2: Run gzjoin with the input, redirect stdout to a temporary output gzip
    std::string output_path = "/tmp/output_join_test.gz";
    int exitcode = run_gzjoin_with_output(input_path, output_path);
    if (exitcode != 0) {
        FAIL("gzjoin did not exit cleanly for single input. Exit: " + std::to_string(exitcode));
        // Cleanup
        unlink(input_path.c_str());
        unlink(output_path.c_str());
        return -1;
    }

    // Step 3: Decompress and verify content
    std::string decompressed = decompress_gzip_file(output_path);
    if (decompressed == input_content) {
        PASS("gzjoin output decompresses to original content for single input");
    } else {
        FAIL("Unexpected decompressed content. Expected: 'Hello', Got: '" + decompressed + "'");
        unlink(input_path.c_str());
        unlink(output_path.c_str());
        return -1;
    }

    // Cleanup
    unlink(input_path.c_str());
    unlink(output_path.c_str());
    return 0;
}

// Entry point for the lightweight test harness
int main() {
    LOG("Starting gzjoin focal-method tests (C++ test harness, no GTest)");

    // Run Test 1: usage message when no args
    int res1 = test_usage_no_args_no_input();
    if (res1 == 0) {
        PASS("Test 1 completed successfully");
    } else {
        FAIL("Test 1 failed with non-zero result");
    }

    // Run Test 2: join with one input
    int res2 = test_join_one_input_basic();
    if (res2 == 0) {
        PASS("Test 2 completed successfully");
    } else {
        FAIL("Test 2 failed with non-zero result");
    }

    // Summary
    std::cout << std::endl << "Test Summary: " 
              << passes << " passed, " << failures << " failed." << std::endl;

    // Return 0 if all tests pass, non-zero otherwise
    return failures == 0 ? 0 : 1;
}