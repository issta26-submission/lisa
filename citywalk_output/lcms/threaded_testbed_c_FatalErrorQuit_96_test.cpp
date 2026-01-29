// This test suite targets the focal method FatalErrorQuit as found in threaded_testbed.c.
// The method is a thin wrapper that logs a fatal error and then calls exit(1).
// Due to the presence of exit(1), tests are executed in a separate process (fork) to
// prevent the test runner from being terminated by the call.
// The tests capture output from the trace function (assumed to write to stdout/stderr) and
// verify that the process exits with code 1 and that a log line containing "Fatal" appears.
//
// Step 1: Candidate Keywords extracted from the focal method and its dependencies
// - FatalErrorQuit: the function under test
// - trace: logging helper used by FatalErrorQuit
// - exit: process termination call
// - UNUSED_PARAMETER: macro used to suppress unused parameter warnings
// - cmsContext, cmsUInt32Number: parameter types
// - Text: input C-string argument carried into trace
// - ContextID, ErrorCode: parameters ignored by FatalErrorQuit
// - threading/internal dependencies included via threaded_internal.h
//
// Step 2/3: Test generation and refinement are implemented below in a POSIX-compatible manner.

#include <cstring>
#include <cstdio>
#include <vector>
#include <threaded_internal.h>
#include <string>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>


#if defined(__cplusplus)
extern "C" {
#endif
// Forward declaration of types used by FatalErrorQuit.
// We include the header that defines these types, while ensuring C linkage.
void FatalErrorQuit(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *Text);
#if defined(__cplusplus)
}
#endif

// Helper: run a single test case that calls FatalErrorQuit in a child process.
// The test verifies:
// - The child exits with status 1
// - Some trace output containing the word "Fatal" was produced (captured via pipe)
static bool RunFatalErrorQuitTest(const char* testText) {
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        perror("pipe");
        return false;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return false;
    }

    if (pid == 0) {
        // Child process
        // Redirect stdout and stderr to the pipe so we can capture trace output.
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) _exit(1);
        if (dup2(pipefd[1], STDERR_FILENO) == -1) _exit(1);
        // Close unused ends
        close(pipefd[0]);
        close(pipefd[1]);

        // Call the focal function. We intentionally pass non-null testText.
        FatalErrorQuit(0, 0, testText);

        // If FatalErrorQuit unexpectedly returns, exit with a distinct code.
        _exit(2);
    } else {
        // Parent process
        // Close write end; read from the read end
        close(pipefd[1]);

        // Read all data produced by the child (trace output)
        std::string output;
        char buffer[4096];
        ssize_t n;
        // Non-blocking read loop with a simple wait for child termination
        while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            output.append(buffer, buffer + n);
        }

        int status = 0;
        waitpid(pid, &status, 0);

        close(pipefd[0]);

        bool exitedWithCode1 = WIFEXITED(status) && (WEXITSTATUS(status) == 1);

        // Heuristic: ensure the trace output contains the word "Fatal" (case-sensitive)
        bool logContainsFatal = (output.find("Fatal") != std::string::npos);

        return exitedWithCode1 && logContainsFatal;
    }
}

// Convenience wrappers for two representative test cases:
// 1) Non-empty Text
static bool test_FatalErrorQuit_LogsAndExits_WithNonEmptyText() {
    // This tests that with a normal string, FatalErrorQuit exits with code 1
    // and logs a message containing "Fatal".
    const char* text = "NonEmptyTestMessage";
    bool result = RunFatalErrorQuitTest(text);
    if (!result) {
        fprintf(stderr, "Test FAILED: FatalErrorQuit did not exit with code 1 or log did not contain 'Fatal' for non-empty text.\n");
    } else {
        printf("Test PASSED: FatalErrorQuit with non-empty text exits with code 1 and logs a fatal message.\n");
    }
    return result;
}

// 2) Empty Text
static bool test_FatalErrorQuit_LogsAndExits_WithEmptyText() {
    // This tests that with an empty string, FatalErrorQuit still exits with code 1
    // and logs a message containing "Fatal".
    const char* text = "";
    bool result = RunFatalErrorQuitTest(text);
    if (!result) {
        fprintf(stderr, "Test FAILED: FatalErrorQuit did not exit with code 1 or log did not contain 'Fatal' for empty text.\n");
    } else {
        printf("Test PASSED: FatalErrorQuit with empty text exits with code 1 and logs a fatal message.\n");
    }
    return result;
}

// Minimal test harness
int main() {
    std::vector<bool> results;

    // Run tests (ordered)
    results.push_back(test_FatalErrorQuit_LogsAndExits_WithNonEmptyText());
    results.push_back(test_FatalErrorQuit_LogsAndExits_WithEmptyText());

    // Summary
    int failures = 0;
    for (size_t i = 0; i < results.size(); ++i) {
        if (!results[i]) ++failures;
    }

    if (failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("%zu TEST(S) FAILED\n", static_cast<unsigned long>(failures));
        return 1;
    }
}