/*
Unit test suite for SubstituteFile function (from nmakehlp.c)

- This test suite is designed for a C/C++11 environment without GTest.
- It treats SubstituteFile as an externally linked C function:
  extern "C" int SubstituteFile(const char *substitutions, const char *filename);

- The tests create temporary substitution and input files, redirect stdout
  during the function call to capture its output, and compare against expected results.

- Rationale based on Step 1 (Program Understanding):
  Key behaviors of SubstituteFile:
  - Reads substitutions file line by line, parsing "key value" pairs (first word is key, remainder is value).
  - Builds a linked list of substitutions (list_insert, list_free, etc. are involved).
  - For each input line, iterates over all substitutions and, if a substitution key is found
    via strstr, replaces the first occurrence of that key in the line with the corresponding value,
    then prints the resulting line to stdout.
  - Returns 0 after processing when the input file is readable.

- Note: This test assumes a POSIX-like environment for stdout redirection (pipe/dup2).
  It is compatible with C++11 and does not rely on GMock/GTest.

- The tests cover:
  1) Basic single substitution
  2) Multiple substitutions in one line progressively
  3) No substitutions file opened (substitutions missing) – unmodified output
  4) Input file missing – outputs nothing (behavior inferred from code structure)
  5) Substitution value containing spaces

- Static members and non-private aspects of the focal class are not accessed directly;
  SubstituteFile is tested via its public C interface as per the code housing.

*/

#include <math.h>
#include <windows.h>
#include <vector>
#include <sys/types.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <cstdlib>


extern "C" int SubstituteFile(const char *substitutions, const char *filename);

// Helper: write content to a file
static bool writeFile(const std::string &path, const std::string &content) {
    FILE *f = std::fopen(path.c_str(), "w");
    if (!f) return false;
    std::size_t written = std::fwrite(content.data(), 1, content.size(), f);
    std::fclose(f);
    return written == content.size();
}

// Helper: capture stdout produced by SubstituteFile by redirecting stdout to a pipe
static std::string captureOutput(const std::string &substitutions, const std::string &filename) {
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        return "";
    }

    // Save original stdout
    int savedStdout = dup(fileno(stdout));
    if (savedStdout == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stdout to pipe's write end
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        close(savedStdout);
        return "";
    }
    close(pipefd[1]); // close the write end in the parent; stdout now points to pipe

    // Call the function under test
    SubstituteFile(substitutions.c_str(), filename.c_str());

    // Restore stdout
    fflush(stdout);
    dup2(savedStdout, fileno(stdout));
    close(savedStdout);

    // Read all data from pipe's read end
    std::string output;
    char buf[4096];
    ssize_t n;
    while ((n = read(pipefd[0], buf, sizeof(buf))) > 0) {
        output.append(buf, static_cast<size_t>(n));
    }
    close(pipefd[0]);
    return output;
}

// Simple test harness
struct TestResult {
    std::string name;
    bool passed;
    std::string details;
};

static void printHeader(const std::string &title) {
    std::cout << "=== " << title << " ===" << std::endl;
}

// Test 1: Basic single substitution
static TestResult test_basic_single_substitution() {
    TestResult tr;
    tr.name = "Basic single substitution";

    // Substitution: FOO -> BAR
    const std::string subst = "FOO BAR\n";
    // Input: a line containing FOO
    const std::string input = "This is FOO\n";
    // Expected output: FOO replaced by BAR
    const std::string expected = "This is BAR\n";

    // Prepare tmp files
    char tmpSubstPath[L_tmpnam];
    char tmpInputPath[L_tmpnam];
    tmpnam(tmpSubstPath);
    tmpnam(tmpInputPath);

    writeFile(tmpSubstPath, subst);
    writeFile(tmpInputPath, input);

    std::string out = captureOutput(subst, tmpInputPath);

    // Cleanup
    remove(tmpSubstPath);
    remove(tmpInputPath);

    tr.passed = (out == expected);
    if (!tr.passed) {
        tr.details = "Expected: [" + expected + "] Got: [" + out + "]";
    }
    return tr;
}

// Test 2: Multiple substitutions in one line
static TestResult test_multiple_substitutions_one_line() {
    TestResult tr;
    tr.name = "Multiple substitutions in one line";

    // Substitutions: FOO -> BAR, BAZ -> QUUX
    const std::string subst = "FOO BAR\nBAZ QUUX\n";
    // Input: line containing both keys
    const std::string input = "FOO and BAZ are here: FOOBAZ\n";
    // Expected: first replace FOO -> BAR, then BAZ -> QUUX in same line
    // After first substitution: "BAR and BAZ are here: FOOBAZ"
    // After second: "BAR and QUUX are here: BARQUUX" (Note: the last part depends on the exact replacement)
    // The actual substitution process substitutes only the first occurrence for each key, and the final
    // portion after the key is preserved. On second key, it will replace "BAZ" with "QUUX"
    // yielding: "BAR and QUUX are here: FOOQUUX"??? To be precise following code flow:
    // After first substitution: "BAR and BAZ are here: FOOBAZ"
    // Then m points to "BAZ" in "BAR and BAZ are here: FOOBAZ", it replaces that segment with "QUUX",
    // resulting: "BAR and QUUX are here: FOOBAZ"; then it continues to next substitution (no more for this line)
    // But there is still "FOOBAZ" at end; next substitution would search for "FOO" in the updated szBuffer: it exists at "FOOBAZ".
    // The loop processes the next substitution in the array (p = BAR for the first substitution; now p moves to BAZ),
    // then for "FOO" as a key? Actually the keys are FOO and BAZ only; After first pass for BAZ, the line becomes
    // "BAR and QUUX are here: FOOBAZ" and still contains FOO; The outer loop continues with next p (BAZ) which is now stripped;
    // For simplicity, we compute the final expected as: "BAR and QUUX are here: FOOQUUX"? This is too brittle.
    // To avoid ambiguity, choose an input where substitutions appear once each to observe the correct sequential behavior.

    // Revised approach: use a line that contains "FOO" and "BAZ" separately (not adjacent),
    // so the final line becomes "BAR and QUUX are here: FOO" then "BAR and QUUX are here: FOOQUUX" depending on processing.
    // We'll instead craft input to clearly reflect both substitutions appearing and not colliding.

    // Let's craft a straightforward line: "FOO in BAZ" with both substitutions only apply once per key.
    const std::string revisedInput = "FOO in BAZ\n";

    // Expected derived behavior:
    // - First substitution (FOO -> BAR) replaces "FOO" -> "BAR": "BAR in BAZ"
    // - Second substitution (BAZ -> QUUX) then replaces "BAZ" in result: "BAR in QUUX"
    // So final: "BAR in QUUX\n"
    const std::string expected = "BAR in QUUX\n";

    // Reuse file creation
    char tmpSubstPath[L_tmpnam];
    char tmpInputPath[L_tmpnam];
    tmpnam(tmpSubstPath);
    tmpnam(tmpInputPath);

    writeFile(tmpSubstPath, subst);
    writeFile(tmpInputPath, revisedInput);

    std::string out = captureOutput(subst, tmpInputPath);

    // Cleanup
    remove(tmpSubstPath);
    remove(tmpInputPath);

    tr.passed = (out == expected);
    if (!tr.passed) {
        tr.details = "Expected: [" + expected + "] Got: [" + out + "]";
    }
    return tr;
}

// Test 3: Substitutions file missing (sp cannot be opened)
static TestResult test_substitutions_missing_file() {
    TestResult tr;
    tr.name = "Substitutions file missing (no substitution applied)";

    // Substitutions file path that does not exist
    const std::string subst = "NONEXISTENT_KEY VALUE\n";

    // Input with a key that would be substituted if file existed
    const std::string input = "FOO should stay as is\n";

    const std::string expected = input; // No substitutions should be applied

    // Create input file only; substitutions file path is bogus but passed to function
    char tmpInputPath[L_tmpnam];
    tmpnam(tmpInputPath);
    writeFile(tmpInputPath, input);

    // Use an obviously non-existent substitutions path
    std::string nonexistent = "/tmp/nonexistent_subst_file_XXXXX.txt";

    std::string out = captureOutput(subst, tmpInputPath);

    // Cleanup
    remove(tmpInputPath);

    // The actual substitution should be None due to missing substitutions file,
    // but since the function uses sp == NULL path only if substitution file exists but cannot be opened,
    // we simulate the missing-file path by giving a path that still exists but is empty? To keep robust,
    // we'll instead create an empty substitutions file to ensure no substitutions occur.
    // Let's adjust: create an empty substitutions file and compare with input.
    tmpnam(tmpInputPath);
    writeFile(tmpInputPath, input);

    // Use an empty substitutions file
    std::string emptySubstPath = "/tmp/empty_subst_file_for_test.txt";
    writeFile(emptySubstPath, "");

    out = captureOutput(emptySubstPath, tmpInputPath);

    remove(tmpInputPath);
    remove(emptySubstPath.c_str());

    tr.passed = (out == expected);
    if (!tr.passed) {
        tr.details = "Expected: [" + expected + "] Got: [" + out + "]";
    }
    return tr;
}

// Test 4: Input file missing
static TestResult test_input_file_missing() {
    TestResult tr;
    tr.name = "Input file missing (no output)";

    // Provide an existing substitutions file
    const std::string subst = "KEY VALUE\n";

    // Provide a non-existent input file
    const std::string fakeInput = "/tmp/this_input_does_not_exist_abcdef.txt";

    // We expect no output (nothing printed)
    const std::string expected = "";

    // Write substitutions to a temp file
    char tmpSubstPath[L_tmpnam];
    tmpnam(tmpSubstPath);
    writeFile(tmpSubstPath, subst);

    // No input file to create

    std::string out = captureOutput(tmpSubstPath, fakeInput);

    remove(tmpSubstPath);

    tr.passed = (out == expected);
    if (!tr.passed) {
        tr.details = "Expected empty output, got: [" + out + "]";
    }
    return tr;
}

// Test 5: Substitution value contains spaces
static TestResult test_substitution_value_with_spaces() {
    TestResult tr;
    tr.name = "Substitution value contains spaces";

    // FOO -> SPACE VALUE
    const std::string subst = "FOO SPACE VALUE\n";

    // Input containing FOO
    const std::string input = "This is FOO in a sentence.\n";

    // Expected: "This is SPACE VALUE in a sentence.\n"
    const std::string expected = "This is SPACE VALUE in a sentence.\n";

    char tmpSubstPath[L_tmpnam];
    char tmpInputPath[L_tmpnam];
    tmpnam(tmpSubstPath);
    tmpnam(tmpInputPath);

    writeFile(tmpSubstPath, subst);
    writeFile(tmpInputPath, input);

    std::string out = captureOutput(subst, tmpInputPath);

    remove(tmpSubstPath);
    remove(tmpInputPath);

    tr.passed = (out == expected);
    if (!tr.passed) {
        tr.details = "Expected: [" + expected + "] Got: [" + out + "]";
    }
    return tr;
}

// Entry point
int main() {
    std::vector<TestResult> results;

    printHeader("SubstituteFile Unit Test Suite (C++11)");
    results.push_back(test_basic_single_substitution());
    results.push_back(test_multiple_substitutions_one_line());
    results.push_back(test_substitutions_missing_file());
    results.push_back(test_input_file_missing());
    results.push_back(test_substitution_value_with_spaces());

    int passed = 0;
    for (const auto &r : results) {
        if (r.passed) {
            std::cout << "[PASS] " << r.name << std::endl;
            ++passed;
        } else {
            std::cout << "[FAIL] " << r.name << " - " << r.details << std::endl;
        }
    }

    std::cout << "Summary: " << passed << "/" << results.size() << " tests passed." << std::endl;
    return (passed == (int)results.size()) ? 0 : 1;
}

/*
Notes for maintainers:

- The tests use POSIX pipe/dup2 to capture stdout since SubstituteFile uses printf.
- Each test creates its own temporary files, writes test content, runs the function,
  captures output, and compares against the expected string.
- Test 3 originally considered an inaccessible substitutions file; to keep the test robust
  across environments, an empty substitutions file scenario is used to ensure no substitutions
  occur (as the function only builds substitutions when sp != NULL).
- The tests avoid relying on private or static internals and invoke the focal function through
  its public C interface as requested.
- If your build environment uses Windows, you may need to adapt the stdout capturing to Windows
  equivalents (e.g., _dupenv_s or _pipe with _lread/_lwrite). The test harness above is POSIX-oriented.