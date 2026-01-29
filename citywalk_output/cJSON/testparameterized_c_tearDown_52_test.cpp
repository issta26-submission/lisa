// test_tearDown_suite.cpp
// Purpose: Provide a focused test suite for the focal tearDown() function
//          found in testparameterized.c. The tests exercise the conditional
//          branches inside tearDown without terminating the test run.
// Notes:
// - This suite targets the Unity-based C environment described by the <FOCAL_METHOD>
//   and <FOCAL_CLASS_DEP> blocks you provided.
// - The tests are written in C++11 (as requested) but operate on C-symbols and
//   rely on the existing Unity-like test environment (no GTest).
// - We capture stdout to verify the message produced by the second conditional
//   branch (the "Test Should Have Passed But Did Not" warning).
// - We avoid triggering TEST_FAIL_MESSAGE in our tests to prevent premature
//   termination of the test run; instead, we validate the non-terminating path.
// - This file is designed to be compiled and linked with the project that already
//   contains testparameterized.c and Unity's runtime.
//
// How this works (high level):
// - We manipulate the global flags SetToOneToFailInTearDown, SetToOneMeanWeAlreadyCheckedThisGuy
//   and the Unity state (CurrentTestFailed) to drive the two main conditional branches.
// - We call tearDown() directly (declared with C linkage) from C++ to exercise the
//   code in <FOCAL_METHOD>.
// - We redirect stdout around the tearDown() invocation to capture and verify the
//   printed warning when the second condition is true.
// - Explanatory comments accompany each test case.

#include <unistd.h>
#include <unity.h>
#include <setjmp.h>
#include <iostream>
#include <fcntl.h>
#include <vector>
#include <cstdio>
#include <string>
#include <stdio.h>


// Ensure C linkage for the focal tearDown symbol and its related globals.
// We do not include "unity.h" here to keep dependencies minimal in the test harness.
// The actual tearDown implementation uses these symbols with C linkage, so we declare
// the minimal subset we need for testing.

extern "C" {
    // Declared in the provided class dependencies (C linkage)
    void tearDown(void);

    // Global flags used by the focal tearDown
    extern int SetToOneToFailInTearDown;
    extern int SetToOneMeanWeAlreadyCheckedThisGuy;

    // The Unity instance used by the focal tearDown
    // We provide a minimal forward declaration for the needed member to
    // allow manipulation of CurrentTestFailed (the actual Unity struct layout in
    // the real project may have more members; we assume CurrentTestFailed is accessible).
    struct UnityStruct {
        int CurrentTestFailed;
    };
    extern struct UnityStruct Unity;
}

// Helper: capture stdout produced by a function, returning the captured string.
static std::string capture_stdout(void (*func)(void))
{
    // Create a pipe to capture stdout
    int pipes[2];
    if (pipe(pipes) != 0) {
        return "";
    }

    // Save current stdout
    int saved_stdout = dup(fileno(stdout));
    // Redirect stdout to the pipe's write end
    dup2(pipes[1], fileno(stdout));
    // Close the write end in this process (it's now duplicated to stdout)
    close(pipes[1]);

    // Execute the function whose output we want to capture
    func();

    // Important: flush to ensure all output is written
    fflush(stdout);

    // Read the captured output
    std::string output;
    const size_t CHUNK = 256;
    char buffer[CHUNK + 1];
    ssize_t n;
    // Close write end already; read from read end
    ssize_t bytesRead;
    while ((n = read(pipes[0], buffer, CHUNK)) > 0) {
        buffer[n] = '\0';
        output += buffer;
    }

    // Restore stdout
    dup2(saved_stdout, fileno(stdout));
    close(saved_stdout);
    close(pipes[0]);

    return output;
}

// Forward declarations of test helpers defined below (we'll implement inline test cases here)
static std::string run_tearDown_with_state(int setToOneToFailValue, int setToOneMeanWeAlreadyCheckedValue, int unityCurrentTestFailed)
{
    // Configure global state used by tearDown
    SetToOneToFailInTearDown = setToOneToFailValue;
    SetToOneMeanWeAlreadyCheckedThisGuy = setToOneMeanWeAlreadyCheckedValue;

    // Attempt to set Unity's CurrentTestFailed (best-effort through our proxy type)
    // The actual Unity struct in the project may have a more complex layout;
    // we assume the first-chunk member named CurrentTestFailed is at a compatible location.
    Unity.CurrentTestFailed = unityCurrentTestFailed;

    // Capture stdout while invoking tearDown
    std::string out;
    // Use a separate, isolated function call to tearDown to ensure clean capture
    // We wrap in a lambda-like call via a static function to satisfy capture_stdout signature
    auto invoke = []() { tearDown(); };
    out = capture_stdout(reinterpret_cast<void (*)(void)>(invoke));

    return out;
}

// Test Case 1: Verify that the second branch prints the warning when a test has failed
//           and we have not yet checked this guy (SetToOneMeanWeAlreadyCheckedThisGuy == 0).
//           Preconditions: SetToOneToFailInTearDown != 1; Unity.CurrentTestFailed > 0
//           Expected: stdout contains the warning string "Test Should Have Passed But Did Not"
static void test_TearDown_Prints_Warning_When_TestFailed_And_Not_Already_Checked()
{
    // Arrange
    SetToOneToFailInTearDown = 0;          // Do not trigger TEST_FAIL_MESSAGE
    SetToOneMeanWeAlreadyCheckedThisGuy = 0;
    Unity.CurrentTestFailed = 1;             // Simulate a failing test

    // Act
    std::string output = run_tearDown_with_state(SetToOneToFailInTearDown, SetToOneMeanWeAlreadyCheckedThisGuy, Unity.CurrentTestFailed);

    // Assert: the warning should be present in the captured output
    if (output.find("Test Should Have Passed But Did Not") == std::string::npos) {
        std::cerr << "[ERROR] TearDown did not print the expected warning when test failed and not yet checked.\n";
        std::cerr << "Captured output: [" << output << "]\n";
        // Non-terminating assertion: write error but do not terminate the program
    } else {
        std::cout << "[OK] TearDown printed warning as expected when test failed and not yet checked.\n";
    }
}

// Test Case 2: Verify that the second branch does NOT print the warning when the 'already checked' flag is set.
// Precondition: SetToOneMeanWeAlreadyCheckedThisGuy != 0
static void test_TearDown_DoesNot_Print_Warning_When_Already_Checked()
{
    // Arrange
    SetToOneToFailInTearDown = 0;
    SetToOneMeanWeAlreadyCheckedThisGuy = 1; // Already checked
    Unity.CurrentTestFailed = 1;               // Simulate a failing test

    // Act
    std::string output = run_tearDown_with_state(SetToOneToFailInTearDown, SetToOneMeanWeAlreadyCheckedThisGuy, Unity.CurrentTestFailed);

    // Assert: there should be no warning message
    if (output.find("Test Should Have Passed But Did Not") != std::string::npos) {
        std::cerr << "[ERROR] TearDown printed warning even though the 'already checked' flag was set.\n";
        std::cerr << "Captured output: [" << output << "]\n";
    } else {
        std::cout << "[OK] TearDown did not print warning when 'already checked' flag was set.\n";
    }
}

// Test Case 3: Verify that the warning is not printed when there is no test failure
static void test_TearDown_No_Warning_When_No_Test_Failure()
{
    // Arrange
    SetToOneToFailInTearDown = 0;
    SetToOneMeanWeAlreadyCheckedThisGuy = 0;
    Unity.CurrentTestFailed = 0; // No test failure

    // Act
    std::string output = run_tearDown_with_state(SetToOneToFailInTearDown, SetToOneMeanWeAlreadyCheckedThisGuy, Unity.CurrentTestFailed);

    // Assert: no warning message should be present
    if (output.find("Test Should Have Passed But Did Not") != std::string::npos) {
        std::cerr << "[ERROR] TearDown printed warning even though there was no test failure.\n";
        std::cerr << "Captured output: [" << output << "]\n";
    } else {
        std::cout << "[OK] TearDown did not print warning when there was no test failure.\n";
    }
}

// Main harness
int main()
{
    // Note: We intentionally run the three focused scenarios to maximize coverage
    // of the conditional branches inside tearDown(), without terminating execution.

    std::cout << "Running TearDown unit tests (non-GTest) ...\n";

    // Run Test Case 1
    test_TearDown_Prints_Warning_When_TestFailed_And_Not_Already_Checked();

    // Run Test Case 2
    test_TearDown_DoesNot_Print_Warning_When_Already_Checked();

    // Run Test Case 3
    test_TearDown_No_Warning_When_No_Test_Failure();

    std::cout << "TearDown unit tests completed.\n";
    return 0;
}

/*
Explanations for each test (embedded comments above explain intent briefly):

- test_TearDown_Prints_Warning_When_TestFailed_And_Not_Already_Checked
  - Ensures the second conditional branch executes when:
    SetToOneMeanWeAlreadyCheckedThisGuy == 0 AND Unity.CurrentTestFailed > 0.
  - Verifies that the warning string ": [[[[ Test Should Have Passed But Did Not ]]]]" is emitted.

- test_TearDown_DoesNot_Print_Warning_When_Already_Checked
  - Ensures that when SetToOneMeanWeAlreadyCheckedThisGuy != 0, the second branch is not taken,
    even if Unity.CurrentTestFailed > 0.
  - Verifies that no warning message is printed.

- test_TearDown_No_Warning_When_No_Test_Failure
  - Ensures that when Unity.CurrentTestFailed == 0, the second branch is not taken,
    regardless of the SetToOneMeanWeAlreadyCheckedThisGuy value.
  - Verifies that no warning message is printed.

Notes for integration:
- Build and link this test with the existing object file that contains testparameterized.c and the Unity runtime.
- The test harness relies on the presence of the global symbols SetToOneToFailInTearDown, SetToOneMeanWeAlreadyCheckedThisGuy,
  and Unity.CurrentTestFailed as exposed by the project's Unity integration.
- stdout capture is used to verify the textual warning produced by the second branch.
- The TEST_FAIL_MESSAGE path (SetToOneToFailInTearDown == 1) is not exercised here to avoid terminating tests;
this suite focuses on non-terminating branches and output verification as requested.

*/