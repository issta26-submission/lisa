// Test suite for the focal method main(int argc, char **argv) in checksum-icc.c
// This test harness uses a lightweight, self-contained approach (no GTest).
// It runs the target program as a subprocess via POSIX pipes to capture stdout/stderr
// and exit codes, exercising the argc > 1 branch (file-based processing) as well as
// the basic header/output logic. It does not modify the original source.
// Explanations are provided for each unit test.

// Candidate Keywords derived from the focal method and dependencies:
// adler32, crc32, MD5[16], read_one_file, fopen, stdin, perror, fprintf, printf, file-name,
// read error, NULL, - (stdin), the fallback/else branch, header output, zlib, FILE, main.

#include <vector>
#include <memory>
#include <string>
#include <zlib.h>
#include <fcntl.h>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdlib>
#include <stdio.h>
#include <cstring>




using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;

/*
 Helper: Run the checksum-icc binary with given arguments and optional stdin content.
 Returns stdout output, and the exit code of the process.
 Note: The path "./checksum-icc" is assumed; adapt if your build places the binary elsewhere.
*/
bool runProgram(const vector<string>& args, const string& stdinData,
                string& stdoutOut, int& exitCode)
{
    int in_pipe[2];  // parent -> child (stdin)
    int out_pipe[2]; // child -> parent (stdout)

    if (pipe(in_pipe) != 0) return false;
    if (pipe(out_pipe) != 0) return false;

    pid_t pid = fork();
    if (pid < 0) {
        // Fork failed
        return false;
    }

    if (pid == 0) {
        // Child
        // Redirect stdin
        dup2(in_pipe[0], STDIN_FILENO);
        // Redirect stdout
        dup2(out_pipe[1], STDOUT_FILENO);

        // Close unused ends
        close(in_pipe[0]); close(in_pipe[1]);
        close(out_pipe[0]); close(out_pipe[1]);

        // Build argv array
        // Program path
        const char* progPath = "./checksum-icc"; // adjust if needed
        std::vector<char*> argvC;
        argvC.push_back(const_cast<char*>(progPath));
        for (const auto& s : args) {
            // allocate a copy
            char* c = new char[s.size() + 1];
            std::strcpy(c, s.c_str());
            argvC.push_back(c);
        }
        argvC.push_back(nullptr);

        // Execute
        execv(progPath, argvC.data());

        // If exec fails
        _exit(127);
    } else {
        // Parent
        // Close unused ends
        close(in_pipe[0]);
        close(out_pipe[1]);

        // Write stdin data if provided
        if (!stdinData.empty()) {
            size_t written = 0;
            while (written < stdinData.size()) {
                ssize_t w = write(in_pipe[1], stdinData.data() + written, stdinData.size() - written);
                if (w <= 0) break;
                written += static_cast<size_t>(w);
            }
        }
        // Close stdin pipe to signal EOF
        close(in_pipe[1]);

        // Read stdout
        char buffer[4096];
        ssize_t n;
        stdoutOut.clear();
        while ((n = read(out_pipe[0], buffer, sizeof(buffer))) > 0) {
            stdoutOut.append(buffer, buffer + n);
        }
        // Close stdout pipe
        close(out_pipe[0]);

        // Wait for child
        int status = 0;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            exitCode = WEXITSTATUS(status);
        } else {
            exitCode = -1;
        }

        // Free allocated argv
        // Note: In child process only; parent doesn't own those allocations.
        // We can't access them here; nevertheless, child cleans up in its own process.
        return true;
    }
}

// Simple helper to create a temporary file with given content.
// Returns the file path. Caller is responsible for deleting the file.
string createTempFile(const string& content)
{
    char tmpl[] = "/tmp/checksum_test_file_XXXXXX";
    int fd = mkstemp(tmpl);
    if (fd == -1) return "";
    // Write content
    if (!content.empty()) {
        write(fd, content.c_str(), content.size());
    }
    close(fd);
    // Return path
    return string(tmpl);
}

// Basic contains finder
bool contains(const string& hay, const string& needle)
{
    return hay.find(needle) != string::npos;
}

// Lightweight test harness
int main(int argc, char** argv)
{
    (void)argc; (void)argv; // silence unused in some build configurations

    int totalTests = 0;
    int failed = 0;

    auto fail = [&](int n, const string& name, const string& reason)
    {
        ++totalTests;
        cerr << "[TEST " << n << " FAILED] " << name << ": " << reason << endl;
        ++failed;
    };
    auto pass = [&](int n, const string& name)
    {
        ++totalTests;
        cout << "[TEST " << n << " PASSED] " << name << endl;
    };

    // Test 1: Single valid file should return 0 and output header present
    {
        const string testName = "Single valid file produces 0 exit and header output";
        int exitCode = -1;
        string stdoutOut;
        // Create a small valid test file (contents arbitrary)
        string tempFile = createTempFile("testdata1234");
        if (tempFile.empty()) {
            fail(1, testName, "failed to create temp file");
        } else {
            vector<string> args;
            // program path is "./checksum-icc" and then the temp file path
            args.push_back(tempFile); // Note: main expects argv[1] to be a file name
            bool ok = runProgram(args, "", stdoutOut, exitCode);
            if (!ok) {
                fail(1, testName, "failed to execute program");
            } else {
                // Expect exit code 0
                if (exitCode != 0) {
                    fail(1, testName, "unexpected non-zero exit code: " + std::to_string(exitCode));
                } else {
                    // Expect header output present in stdout
                    const string header = "/* adler32, crc32, MD5[16], intent, date, length, file-name */";
                    if (!contains(stdoutOut, header)) {
                        fail(1, testName, "stdout missing expected header string");
                    } else {
                        pass(1, testName);
                    }
                }
            }
            // cleanup file
            unlink(tempFile.c_str());
        }
    }

    // Test 2: Missing file should return non-zero and print an error message
    {
        const string testName = "Missing file yields non-zero and prints read error";
        int exitCode = -1;
        string stdoutOut;
        vector<string> args;
        args.push_back("/path/does/not/exist.file");
        bool ok = runProgram(args, "", stdoutOut, exitCode);
        if (!ok) {
            fail(2, testName, "failed to execute program");
        } else {
            if (exitCode != 1) {
                fail(2, testName, "expected exit code 1, got " + std::to_string(exitCode));
            } else {
                // Expect an error message line similar to "/* ERROR: <name> */"
                string expected = "/* ERROR: /path/does/not/exist.file */";
                if (!contains(stdoutOut, "/* ERROR:")) {
                    fail(2, testName, "stdout does not contain expected error marker");
                } else {
                    pass(2, testName);
                }
            }
        }
    }

    // Test 3: Multiple files with one missing should exit with non-zero and report error
    {
        const string testName = "Multiple files with one missing yields non-zero and prints error";
        int exitCode = -1;
        string stdoutOut;
        // Create one valid file
        string goodFile = createTempFile("data");
        if (goodFile.empty()) {
            fail(3, testName, "failed to create temp good file");
        } else {
            vector<string> args;
            args.push_back(goodFile);
            args.push_back("/does/not/exist.file2");
            bool ok = runProgram(args, "", stdoutOut, exitCode);
            if (!ok) {
                fail(3, testName, "failed to execute program");
            } else {
                if (exitCode != 1) {
                    fail(3, testName, "expected exit code 1, got " + std::to_string(exitCode));
                } else {
                    if (!contains(stdoutOut, "/* ERROR:")) {
                        fail(3, testName, "stdout does not contain expected error marker for missing file");
                    } else {
                        pass(3, testName);
                    }
                }
            }
            unlink(goodFile.c_str());
        }
    }

    // Summary
    cout << "Total tests: " << totalTests << ", Failures: " << failed << endl;
    // Exit status: 0 if all tests pass, 1 otherwise
    return failed ? 1 : 0;
}