// C++11 unit test suite for UnityIgnoreTest in unity_fixture.c
// This test harness exercises the UnityIgnoreTest function by validating
// the printing behavior (verbose vs non-verbose) and the selection predicates
// (name/group filters) via the UnityFixture state.
// No external test framework is used (as requested).

#include <unity_internals.h>
#include <unistd.h>
#include <iostream>
#include <functional>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unity_fixture.h>
#include <fcntl.h>


// Provide C linkage for the focal Unity API when included from C++
extern "C" {

// Function under test (focal method)
void UnityIgnoreTest(const char* printableName, const char* group, const char* name);

// Access to the Unity internal state (to control selection predicates)
extern struct UNITY_FIXTURE_T UnityFixture;
}

// Helper to capture stdout output produced by the focal function.
// Redirects stdout to a pipe, executes the provided function, then reads
// all output from the pipe and restores stdout.
static std::string capture_stdout(void (*func)()) {
    // Create a pipe for capturing stdout
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // If pipe creation fails, return empty output
        return "";
    }

    // Save current stdout
    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stdout to pipe's write end
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        // On failure, clean up
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        return "";
    }

    // Close the pipe's write end in this process (it's duplicated to stdout)
    close(pipefd[1]);

    // Run the test function that produces output
    func();

    // Flush and restore stdout
    fflush(stdout);
    if (dup2(saved_stdout, fileno(stdout)) == -1) {
        // Restore failed; fall back to reading from pipe
    }
    close(saved_stdout);

    // Read all data from the pipe's read end
    std::string output;
    char buffer[1024];
    ssize_t bytes_read;
    // Set the read end to the current position at the start
    lseek(pipefd[0], 0, SEEK_SET);
    while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(bytes_read));
    }
    close(pipefd[0]);
    return output;
}

// Wrappers to invoke UnityIgnoreTest with fixed parameters.
// These wrappers are used by the capture_stdout helper.

static void call_UnityIgnoreTest_printable1(void) {
    // Basic test: selected path (default UnityFixture state)
    UnityIgnoreTest("Printable1", "GroupA", "TestName1");
}
static void call_UnityIgnoreTest_printable2(void) {
    UnityIgnoreTest("Printable2", "GroupB", "TestName2");
}
static void call_UnityIgnoreTest_printable3(void) {
    UnityIgnoreTest("P3", "G3", "N3");
}
static void call_UnityIgnoreTest_printable4(void) {
    UnityIgnoreTest("P4", "G4", "N4");
}

// Reset Unity internal fixture state to a known baseline for tests.
// This avoids cross-test contamination.
static void reset_Unity_state_baseline() {
    // The actual fields are declared in unity_fixture.h as used by the focal method.
    // We reset only the knobs we control in tests.
    UnityFixture.NameFilter = NULL;
    UnityFixture.GroupFilter = NULL;
    UnityFixture.Verbose = false;
    // Other fields remain in their default (initialized) state.
}

// --- Individual test cases (return true on pass, false on fail) ---

// Test 1: Non-verbose mode prints a single '!' when the test is selected.
static bool test_non_verbose_prints_exclamation_when_selected() {
    reset_Unity_state_baseline();
    UnityFixture.Verbose = false;          // non-verbose path
    UnityFixture.NameFilter  = NULL;        // ensure selection by name
    UnityFixture.GroupFilter = NULL;

    std::string out = capture_stdout(call_UnityIgnoreTest_printable1);
    if (out == "!") {
        return true;
    } else {
        std::cerr << "Test 1 failed: expected '!' but got [" << out << "]\n";
        return false;
    }
}

// Test 2: Verbose mode prints the printableName followed by newline when the test is selected.
static bool test_verbose_prints_printable_name_with_newline() {
    reset_Unity_state_baseline();
    UnityFixture.Verbose = true;           // verbose path
    UnityFixture.NameFilter  = NULL;
    UnityFixture.GroupFilter = NULL;

    std::string out = capture_stdout(call_UnityIgnoreTest_printable1);
    // Expect the printed name followed by newline
    if (out == "Printable1\n") {
        return true;
    } else {
        std::cerr << "Test 2 failed: expected 'Printable1\\n' but got [" << out << "]\n";
        return false;
    }
}

// Test 3: When NameFilter is set to a non-matching string, the test is not selected.
static bool test_no_selection_due_to_name_filter() {
    reset_Unity_state_baseline();
    UnityFixture.Verbose = false;
    UnityFixture.NameFilter  = "nomatch";
    UnityFixture.GroupFilter = NULL;

    std::string out = capture_stdout(call_UnityIgnoreTest_printable1);
    if (out.empty()) {
        return true;
    } else {
        std::cerr << "Test 3 failed: expected no output, got [" << out << "]\n";
        return false;
    }
}

// Test 4: When multiple selected invocations occur in verbose mode, multiple printable names are output.
static bool test_multiple_verbose_prints_for_multiple_calls() {
    reset_Unity_state_baseline();
    UnityFixture.Verbose = true;
    UnityFixture.NameFilter  = NULL;
    UnityFixture.GroupFilter = NULL;

    std::string out1 = capture_stdout(call_UnityIgnoreTest_printable3);
    // Adjusture to ensure first wrapper prints P3
    bool firstOk = (out1 == "P3\n");

    std::string out2 = capture_stdout(call_UnityIgnoreTest_printable4);
    bool secondOk = (out2 == "P4\n");

    if (firstOk && secondOk) {
        return true;
    } else {
        std::cerr << "Test 4 failed: expected outputs 'P3\\n' and 'P4\\n', got ["
                  << out1 << "] and [" << out2 << "]\n";
        return false;
    }
}

// Test 5: Non-selected due to group filter yields no output despite verbose mode.
static bool test_no_selection_due_to_group_filter() {
    reset_Unity_state_baseline();
    UnityFixture.Verbose = true;
    UnityFixture.NameFilter  = NULL;
    UnityFixture.GroupFilter = "nomatch";

    std::string out = capture_stdout(call_UnityIgnoreTest_printable2);
    if (out.empty()) {
        return true;
    } else {
        std::cerr << "Test 5 failed: expected no output due to group filter, got [" << out << "]\n";
        return false;
    }
}

// --- Test runner and main ---

int main() {
    // Run all tests and report results.
    int total = 0;
    int passes = 0;

    auto run = [&](bool (*testFunc)(), const char* name) -> void {
        ++total;
        bool ok = testFunc();
        if (ok) {
            ++passes;
            std::cout << "[PASS] " << name << "\n";
        } else {
            std::cout << "[FAIL] " << name << "\n";
        }
    };

    run(test_non_verbose_prints_exclamation_when_selected, "test_non_verbose_prints_exclamation_when_selected");
    run(test_verbose_prints_printable_name_with_newline, "test_verbose_prints_printable_name_with_newline");
    run(test_no_selection_due_to_name_filter, "test_no_selection_due_to_name_filter");
    run(test_multiple_verbose_prints_for_multiple_calls, "test_multiple_verbose_prints_for_multiple_calls");
    run(test_no_selection_due_to_group_filter, "test_no_selection_due_to_group_filter");

    std::cout << "\nSummary: " << passes << " / " << total << " tests passed.\n";
    return (passes == total) ? 0 : 1;
}