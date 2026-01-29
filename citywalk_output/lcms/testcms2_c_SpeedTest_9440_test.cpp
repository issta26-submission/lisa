// SpeedTestCppTests.cpp
// A lightweight C++11 test harness for the focal method SpeedTest() from testcms2.c.
// This test avoids external test frameworks (no GTest) and uses a small custom
// assertion mechanism. It focuses on ensuring SpeedTest executes safely and
// prints the expected header text. The actual heavy-lifting ICC operations are
// exercised via the real implementation in testcms2.c, but are validated here
// through observable side-effects (stdout output) and non-crashing behavior.

// Notes:
// - The tests rely on the existence of a C function with C linkage:
//     void SpeedTest(void);
// - We capture stdout around SpeedTest() to verify that the performance header is printed.
// - We keep assertions non-terminating: we log failures and continue, to improve
//   code coverage across possible execution paths.

#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <functional>
#include <vector>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <sys/types.h>
#include <algorithm>
#include <unistd.h>


// Forward declaration of the focal function from testcms2.c (C linkage)
extern "C" void SpeedTest(void);

// Simple non-terminating assertion framework
struct TestLogger {
    std::vector<std::string> failures;

    void log_failure(const char* msg, const char* file, int line) {
        char buf[1024];
        std::snprintf(buf, sizeof(buf), "Assertion failed: %s (at %s:%d)", msg, file, line);
        failures.push_back(std::string(buf));
    }

    bool has_failures() const { return !failures.empty(); }

    void dump() const {
        if (failures.empty()) {
            std::cout << "[PASS] All tests succeeded.\n";
        } else {
            std::cout << "[FAIL] " << failures.size() << " failure(s) detected:\n";
            for (const auto& f : failures) {
                std::cout << "  - " << f << "\n";
            }
        }
    }
};

#define EXPECT_TRUE(cond) \
    do { if(!(cond)) { logger.log_failure(#cond, __FILE__, __LINE__); } } while(0)

#define EXPECT_NOT_NULL(ptr) \
    do { if((ptr)==nullptr) { logger.log_failure(#ptr " is NULL", __FILE__, __LINE__); } } while(0)


// Utility: capture stdout produced by a callable (POSIX)
std::string capture_stdout(const std::function<void()> &fn) {
    // Create a pipe to capture stdout
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // If pipe creation fails, fall back to direct call
        // and return empty string to avoid masking issues.
        fn();
        return std::string();
    }

    // Save original stdout
    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1) {
        // Fallback: just run the function
        close(pipefd[0]);
        close(pipefd[1]);
        fn();
        return std::string();
    }

    // Redirect stdout to the pipe's write end
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        // Fallback
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        fn();
        return std::string();
    }

    // Close the write end in the parent; stdout now writes to the pipe
    close(pipefd[1]);

    // Execute function
    fn();

    // Ensure all data is flushed
    fflush(stdout);

    // Restore original stdout
    dup2(saved_stdout, fileno(stdout));
    close(saved_stdout);

    // Read from the pipe
    std::string output;
    char buffer[4096];
    ssize_t n;
    // Re-open the read end
    int read_fd = pipefd[0];
    // Read until EOF
    while ((n = read(read_fd, buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(n));
    }
    close(read_fd);
    return output;
}


// Test 1: Verify that SpeedTest prints the performance header without crashing
void test_SpeedTest_PrintsHeader(TestLogger &logger) {
    // Capture stdout while SpeedTest executes
    std::string out = capture_stdout([](){ SpeedTest(); });

    // The focal function prints a header string at the start
    const std::string header = "P E R F O R M A N C E   T E S T S";

    bool found = (out.find(header) != std::string::npos);
    EXPECT_TRUE(found);
    // Optional: ensure there is a separator line following header
    const std::string sep = "=================================";
    bool sep_found = (out.find(sep) != std::string::npos);
    EXPECT_TRUE(sep_found);
}

// Test 2: Basic non-crashing execution check for SpeedTest
// We only verify that the function returns normally (no crashes or exceptions).
void test_SpeedTest_DoesNotCrash(TestLogger &logger) {
    // We call SpeedTest() directly and rely on the absence of exceptions.
    // If it crashes (e.g., segmentation fault), the test process will abort.
    // To provide a non-terminating assertion, we perform a simple no-op check.
    // We also run it with stdout redirected to avoid spurious test noise.
    std::string out = capture_stdout([](){ SpeedTest(); });

    // Basic guard: ensure some output was produced (not guaranteed, but a sanity check)
    // If library prints nothing, it's still possible, but we log a warning as a failure.
    EXPECT_TRUE(!out.empty());
}


// Minimal test runner
int main() {
    TestLogger logger;

    // Run tests
    test_SpeedTest_PrintsHeader(logger);
    test_SpeedTest_DoesNotCrash(logger);

    // Report results
    logger.dump();

    // Return non-zero if any test failed
    if (logger.has_failures()) {
        return 1;
    }
    return 0;
}