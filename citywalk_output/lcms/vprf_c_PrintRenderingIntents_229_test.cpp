// Test suite for PrintRenderingIntents in vprf.c (C++11, no GTest)
// This test captures the stderr output produced by PrintRenderingIntents and
// performs lightweight, non-terminating assertions to verify formatting and content.
// Note: We rely on the actual behavior of cmsGetSupportedIntents from LittleCMS.
// If the environment does not have LittleCMS, this test may need adjustment or stubbing.

#include <cstring>
#include <cstdio>
#include <vector>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <utils.h>


// Declaration of the focal function.
// The function is defined in vprf.c; we assume the linker will provide it.
// Use C linkage to ensure correct linkage if included in a C++ file.
extern "C" void PrintRenderingIntents(void);

// Wrapper to call the focal function (used by the capture helper).
static void call_PrintRenderingIntents() {
    PrintRenderingIntents();
}

// Utility: capture stderr output produced by a function.
// It redirects file descriptor 2 (stderr) to a pipe, runs the function, then restores
// the original stderr and returns the captured output as a std::string.
static std::string captureStderr(void (*func)()) {
    // Create a pipe for capturing stderr.
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // If pipe creation fails, fall back to empty output to avoid crashing tests.
        return "";
    }

    // Save current stderr.
    int savedStderr = dup(2);
    if (savedStderr == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stderr to the write end of the pipe.
    if (dup2(pipefd[1], 2) == -1) {
        // Cleanup on failure.
        close(pipefd[0]);
        close(pipefd[1]);
        close(savedStderr);
        return "";
    }

    // Close the write end in the parent; the function will write to fd 2.
    close(pipefd[1]);

    // Invoke the function whose stdout/stderr we want to capture.
    func();

    // Flush and restore stderr.
    fflush(stderr);
    dup2(savedStderr, 2);
    close(savedStderr);

    // Read captured data from the read end of the pipe.
    std::string output;
    char buffer[4096];
    ssize_t bytesRead;
    // The read end is pipefd[0].
    // It may still be open; read until exhausted.
    while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(bytesRead));
    }

    // Close the read end.
    close(pipefd[0]);

    return output;
}

// Lightweight assertion helpers (no GTest). They return true/false and print diagnostics.
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "Assertion failed: " << (msg) << std::endl; \
        return false; \
    } \
} while (0)

#define ASSERT_STR_PREFIX(actual, prefix, msg) do { \
    if ((actual).rfind((prefix), 0) != 0) { \
        std::cerr << "Assertion failed: " << (msg) \
                  << " - expected prefix \"" << (prefix) \
                  << "\", got \"" << (actual).substr(0, (prefix).size()) \
                  << "\"..." << std::endl; \
        return false; \
    } \
} while (0)

#define ASSERT_CONTAINS(haystack, needle, msg) do { \
    if ((haystack).find((needle)) == std::string::npos) { \
        std::cerr << "Assertion failed: " << (msg) \
                  << " - missing substring \"" << (needle) << "\"." << std::endl; \
        return false; \
    } \
} while (0)

// Test 1: Header line formatting is exactly printed before any rendering intents.
static bool Test_PrintRenderingIntents_HeaderFormat() {
    // Capture output of the focal function.
    std::string output = captureStderr(call_PrintRenderingIntents);

    // The header line is a literal string in the code: "-t<n> rendering intent:\n\n"
    const std::string expectedHeader = "-t<n> rendering intent:\n\n";

    // Check that the output starts with the exact header.
    ASSERT_TRUE(output.size() >= expectedHeader.size(), "Header size is smaller than expected");
    ASSERT_STR_PREFIX(output, expectedHeader, "Header does not match exact expected string");

    // If we reached here, header formatting is correct.
    return true;
}

// Test 2: Each rendering intent entry line uses the expected formatting:
// a tab, a numeric code, a dash, and a description.
static bool Test_PrintRenderingIntents_EntriesFormat() {
    std::string output = captureStderr(call_PrintRenderingIntents);

    // There should be at least one rendered line after the header.
    // Look for a line starting with a tab, followed by a number and " - ".
    // We conservatively search for at least one occurrence of "\n\t<digits> - ".
    bool foundEntry = false;
    size_t pos = 0;
    while (true) {
        // Find next newline.
        size_t nl = output.find('\n', pos);
        if (nl == std::string::npos) break;

        // Check if next line starts with a tab.
        size_t afterNewline = nl + 1;
        if (afterNewline < output.size() && output[afterNewline] == '\t') {
            // Look for " - " after the tab, indicating "<Code> - <Desc>"
            size_t dashPos = output.find(" - ", afterNewline);
            if (dashPos != std::string::npos) {
                foundEntry = true;
                break;
            }
        }
        pos = nl + 1;
    }

    ASSERT_TRUE(foundEntry, "No rendering intent entry line in expected format found");

    // Additionally, verify there is at least one trailing newline after the last line.
    ASSERT_TRUE(!output.empty() && output.back() == '\n', "Output should end with a newline");

    return true;
}

// Test 3: The function prints content beyond the header (i.e., at least one rendering intent line),
// ensuring the function does more than just emit the header.
static bool Test_PrintRenderingIntents_ContentPresence() {
    std::string output = captureStderr(call_PrintRenderingIntents);

    // The header may be followed by multiple lines. Ensure there is at least one newline after header.
    const std::string header = "-t<n> rendering intent:\n\n";
    size_t headerPos = output.find(header);
    ASSERT_TRUE(headerPos != std::string::npos, "Header not found in output");

    // After header, there should be at least one additional line (even if empty) due to the subsequent lines.
    // We'll simply check that there exists at least one tab-intro line as in Test 2.
    bool hasEntryLine = (output.find("\n\t") != std::string::npos) || (output.find("\t") != std::string::npos);
    ASSERT_TRUE(hasEntryLine, "Output does not contain any rendering intent entry lines");
    return true;
}

// Minimal test runner to execute all tests and report results.
int main() {
    std::vector<std::string> testNames;
    std::vector<bool>  testResults;

    // Register tests
    testNames.push_back("Test_PrintRenderingIntents_HeaderFormat");
    testResults.push_back(Test_PrintRenderingIntents_HeaderFormat());

    testNames.push_back("Test_PrintRenderingIntents_EntriesFormat");
    testResults.push_back(Test_PrintRenderingIntents_EntriesFormat());

    testNames.push_back("Test_PrintRenderingIntents_ContentPresence");
    testResults.push_back(Test_PrintRenderingIntents_ContentPresence());

    // Summarize results
    int passed = 0;
    int total  = static_cast<int>(testNames.size());

    for (size_t i = 0; i < testNames.size(); ++i) {
        if (testResults[i]) {
            std::cout << "[PASS] " << testNames[i] << "\n";
            ++passed;
        } else {
            std::cout << "[FAIL] " << testNames[i] << "\n";
        }
    }

    std::cout << "Test results: " << passed << " / " << total << " passed.\n";

    return (passed == total) ? 0 : 1;
}