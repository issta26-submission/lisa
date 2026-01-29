// This test suite targets the focal method PrintPerformance(Bytes, SizeOfPixel, diff)
// located in testcms2.c. We implement a small, POSIX-based harness (no GTest) to
// capture stdout output and verify the numeric result printed by PrintPerformance.
// The tests follow the step-by-step plan described in the prompt and include
// explanations as comments for each unit test.

// Step 1 (Overview):
//   - PrintPerformance computes seconds = diff / CLOCKS_PER_SEC
//   - Computes mpix_sec = Bytes / (1024^2 * seconds * SizeOfPixel)
//   - Outputs: "%#4.3g MPixel/sec.\n" via printf and flushes stdout
//
// Candidate keywords / dependencies to consider when testing:
//   - CLOCKS_PER_SEC, stdout (printf), fflush
//   - 1024.0*1024.0 (MB in bytes), SizeOfPixel, Bytes
//   - cmsUInt32Number, cmsFloat64Number (types used by the focal method)

#include <cstring>
#include <cstdio>
#include <fcntl.h>
#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <algorithm>
#include <unistd.h>
#include <cmath>
#include <cerrno>
#include <ctime>



// Forward declaration of the focal method (C linkage)
extern "C" void PrintPerformance(cmsUInt32Number Bytes, cmsUInt32Number SizeOfPixel, cmsFloat64Number diff);

// Utility: Capture stdout output of PrintPerformance using a POSIX pipe
// Preconditions: running on POSIX (Linux/macOS). On Windows this needs adaptation.
static std::string capturePrintPerformanceOutput(cmsUInt32Number Bytes,
                                                cmsUInt32Number SizeOfPixel,
                                                cmsFloat64Number diff)
{
#ifndef _WIN32
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        perror("pipe");
        return "";
    }

    // Save current stdout
    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1) {
        perror("dup stdout");
        // Clean up
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stdout to pipe
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        perror("dup2 stdout");
        // Clean up
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        return "";
    }

    // We no longer need the write end in the parent
    close(pipefd[1]);

    // Call the focal function
    PrintPerformance(Bytes, SizeOfPixel, diff);

    // Flush and restore stdout
    fflush(stdout);
    if (dup2(saved_stdout, fileno(stdout)) == -1) {
        perror("restore stdout");
        close(saved_stdout);
        close(pipefd[0]);
        return "";
    }
    close(saved_stdout);

    // Read the captured output
    std::string output;
    char buf[256];
    ssize_t n;
    // Read until EOF
    // Set the read end non-blocking with a small loop to avoid deadlock
    int flags = fcntl(pipefd[0], F_GETFL, 0);
    fcntl(pipefd[0], F_SETFL, flags | O_NONBLOCK);

    // Try to read, loop until no more data is available
    while (true) {
        n = read(pipefd[0], buf, sizeof(buf));
        if (n > 0) {
            output.append(buf, buf + n);
        } else if (n == 0) {
            break;
        } else {
            // EAGAIN/EWOULDBLOCK means no more data right now
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // Small sleep to allow the writer to finish (though in this case, printing is synchronous)
                usleep(1000);
                continue;
            } else {
                // Real read error
                break;
            }
        }
    }

    // Clean up
    close(pipefd[0]);
    return output;
#else
    // Windows alternative would go here; for the scope of this test, skip.
    (void)Bytes; (void)SizeOfPixel; (void)diff;
    return "";
#endif
}

// Utility: Extract the first floating-point number from a string.
// Handles integers, decimals, and exponential notation.
static bool extractFirstDouble(const std::string& s, double& value)
{
    // Find first character that could start a number
    size_t i = 0;
    while (i < s.size() && !( (s[i] >= '0' && s[i] <= '9') || s[i] == '+' || s[i] == '-' || s[i] == '.' ))
        ++i;
    if (i >= s.size()) return false;

    // Extract token
    size_t j = i;
    bool hasDot = false;
    bool hasExp = false;
    // optional sign
    if (s[j] == '+' || s[j] == '-') ++j;
    while (j < s.size()) {
        char c = s[j];
        if (c >= '0' && c <= '9') { ++j; continue; }
        if (c == '.' && !hasDot && !hasExp) { hasDot = true; ++j; continue; }
        if ((c == 'e' || c == 'E') && !hasExp) { hasExp = true; ++j;
            if (j < s.size() && (s[j] == '+' || s[j] == '-')) ++j;
            continue;
        }
        break;
    }
    std::string token = s.substr(i, j - i);
    try {
        value = std::stod(token);
        return true;
    } catch (...) {
        return false;
    }
}

// Simple test framework scaffolding (no GTest)
// Tracks and reports failures; prints pass messages per test
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define RUN_TEST(name) do { \
    bool ok = name(); \
    ++g_total_tests; \
    if (!ok) { ++g_failed_tests; fprintf(stderr, "TEST FAILED: %s\n", #name); } \
    else { printf("TEST PASSED: %s\n", #name); } \
} while(0)


// Step 2 (Unit Test Generation):
// Test 1: Basic scenario with seconds = 1 (diff == CLOCKS_PER_SEC), SizeOfPixel = 4, Bytes configured to produce mpix_sec = 1.0
static bool test_PrintPerformance_Basic_OneSecond()
{
    // Candidate keywords for this scenario:
    //   - Bytes = 4 * 1024 * 1024, SizeOfPixel = 4, diff = CLOCKS_PER_SEC
    cmsUInt32Number Bytes = 4 * 1024 * 1024;   // 4 MB
    cmsUInt32Number SizeOfPixel = 4;            // 4 bytes per pixel
    cmsFloat64Number diff = (cmsFloat64Number)CLOCKS_PER_SEC; // 1 second

    std::string out = capturePrintPerformanceOutput(Bytes, SizeOfPixel, diff);
    if (out.empty()) {
        // Could not capture output
        fprintf(stderr, "Failed to capture output for Basic_OneSecond.\n");
        return false;
    }

    double observed = 0.0;
    if (!extractFirstDouble(out, observed)) {
        fprintf(stderr, "Could not parse numeric MPixel/sec from output: %s\n", out.c_str());
        return false;
    }

    // Expected: mpix_sec = Bytes / (1024^2 * 1 * SizeOfPixel)
    double seconds = diff / (double)CLOCKS_PER_SEC; // should be 1.0
    double expected = (double)Bytes / (1024.0*1024.0*seconds*SizeOfPixel);

    double tol = 1e-6;
    if (std::fabs(observed - expected) > tol) {
        fprintf(stderr, "Mismatch in Basic_OneSecond: observed=%f, expected=%f, out='%s'\n",
                observed, expected, out.c_str());
        return false;
    }

    return true;
}

// Test 2: Fractional seconds (diff = 0.5 * CLOCKS_PER_SEC) to exercise non-integer seconds.
// Expect mpix_sec = 2 * Bytes / (1024^2 * SizeOfPixel)
static bool test_PrintPerformance_Basic_HalfSecond()
{
    cmsUInt32Number Bytes = 4 * 1024 * 1024;   // 4 MB
    cmsUInt32Number SizeOfPixel = 4;            // 4 bytes per pixel
    cmsFloat64Number diff = 0.5 * (cmsFloat64Number)CLOCKS_PER_SEC; // 0.5 seconds

    std::string out = capturePrintPerformanceOutput(Bytes, SizeOfPixel, diff);
    if (out.empty()) {
        fprintf(stderr, "Failed to capture output for Basic_HalfSecond.\n");
        return false;
    }

    double observed = 0.0;
    if (!extractFirstDouble(out, observed)) {
        fprintf(stderr, "Could not parse numeric MPixel/sec from output: %s\n", out.c_str());
        return false;
    }

    double seconds = diff / (double)CLOCKS_PER_SEC; // should be 0.5
    double expected = (double)Bytes / (1024.0*1024.0*seconds*SizeOfPixel);

    double tol = 1e-6;
    if (std::fabs(observed - expected) > tol) {
        fprintf(stderr, "Mismatch in Basic_HalfSecond: observed=%f, expected=%f, out='%s'\n",
                observed, expected, out.c_str());
        return false;
    }

    return true;
}

// Test 3: Zero Bytes to ensure output is produced and equals 0 MPixel/sec.
// Note: diff should be non-zero to avoid division-by-zero; we use diff = CLOCKS_PER_SEC
static bool test_PrintPerformance_ZeroBytes()
{
    cmsUInt32Number Bytes = 0;
    cmsUInt32Number SizeOfPixel = 4;
    cmsFloat64Number diff = (cmsFloat64Number)CLOCKS_PER_SEC; // 1 second

    std::string out = capturePrintPerformanceOutput(Bytes, SizeOfPixel, diff);
    if (out.empty()) {
        fprintf(stderr, "Failed to capture output for ZeroBytes.\n");
        return false;
    }

    double observed = 0.0;
    if (!extractFirstDouble(out, observed)) {
        fprintf(stderr, "Could not parse numeric MPixel/sec from output: %s\n", out.c_str());
        return false;
    }

    double seconds = diff / (double)CLOCKS_PER_SEC; // 1.0
    double expected = (double)Bytes / (1024.0*1024.0*seconds*SizeOfPixel);

    double tol = 1e-6;
    if (std::fabs(observed - expected) > tol) {
        fprintf(stderr, "Mismatch in ZeroBytes: observed=%f, expected=%f, out='%s'\n",
                observed, expected, out.c_str());
        return false;
    }

    return true;
}

// Step 3 (Test Refinement): Aggregated test launcher
int main()
{
    // Step 2 tests
    RUN_TEST(test_PrintPerformance_Basic_OneSecond);
    RUN_TEST(test_PrintPerformance_Basic_HalfSecond);
    RUN_TEST(test_PrintPerformance_ZeroBytes);

    // Step 3: Summary
    printf("=====================================\n");
    printf("Total tests run: %d\n", g_total_tests);
    printf("Total failures: %d\n", g_failed_tests);
    printf("Final result: %s\n", (g_failed_tests == 0) ? "ALL TESTS PASSED" : "SOME TESTS FAILED");

    return g_failed_tests ? 1 : 0;
}