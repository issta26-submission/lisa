// C++11 test harness for the focal method main() in the enough.c-like binary.
// This harness assumes the compiled binary is available as ./enough in the
// current working directory. It executes the binary with various argument
// combinations to exercise key control-flow paths of the program (parsing,
// abort path, invalid arguments path, and typical successful path).
// No GTest is used; this is a lightweight, self-contained test runner.

#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <string>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <sys/wait.h>
#include <stdlib.h>
#include <cstdlib>


// Run the external program with the given command string and return the exit code.
// Returns -1 on system error or if the process did not terminate normally.
int runProgram(const std::string& cmd) {
    int status = std::system(cmd.c_str());
    if (status == -1) {
        return -1;
    }
#ifdef _WIN32
    // On Windows, system() return value semantics differ; return as-is.
    return status;
#else
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    } else {
        // Abnormal termination
        return -1;
    }
#endif
}

int main() {
    // Path to the target binary compiled from enough.c
    const std::string binaryPath = "./enough";

    struct TestCase {
        std::string description;
        std::string cmd;
        int expected;
    };

    std::vector<TestCase> tests;

    // Test 1: Default run (no arguments). Expect successful exit (0).
    // This exercises the normal code path with default values.
    tests.push_back({
        "Default run (no args): exercise typical successful path with defaults.",
        binaryPath, // no extra args
        0
    });

    // Test 2: Invalid argument count (argc > 4). Expect failure exit (1) due to invalid usage message.
    // Provide four extra arguments to trigger argc > 4 (since argv[0] is program name).
    tests.push_back({
        "Invalid argument count (argc > 4): verify argument-validation branch.",
        binaryPath + " 286 9 15 0", // exactly 4 additional args -> argc == 5
        1
    });

    // Test 3: Abort path (code length too long for internal types).
    // Provide max > syms-1 to force abort condition. Expect exit 1.
    tests.push_back({
        "Abort path (too-long code length): max forced high to trigger abort message.",
        binaryPath + " 286 9 1000", // syms=286, root=9, max=1000 -> max capped to 285 -> abort
        1
    });

    // Test 4: Cannot handle minimum code lengths > root path.
    // Choose syms small enough and max small enough to not abort, but root too small for syms.
    // Expect exit 1 due to "symbols cannot be coded in X bits" message.
    tests.push_back({
        "Cannot handle minimum code lengths > root: syms=5, root=1, max=2.",
        binaryPath + " 5 1 2",
        1
    });

    int passed = 0;
    int failed = 0;

    for (size_t i = 0; i < tests.size(); ++i) {
        const auto& t = tests[i];
        int rc = runProgram(t.cmd);
        bool ok = (rc == t.expected);
        std::cout << "Test " << (i + 1) << ": " << t.description << "\n";
        std::cout << "  Command: " << t.cmd << "\n";
        std::cout << "  Expected exit: " << t.expected << ", Got: " << rc
                  << (ok ? " [PASS]" : " [FAIL]") << "\n";
        if (ok) ++passed; else ++failed;
    }

    std::cout << "\nSummary: " << passed << " passed, " << failed << " failed.\n";

    // If any test failed, return non-zero to indicate overall failure.
    return (failed == 0) ? 0 : 2;
}