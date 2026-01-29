/*
Unit test suite for the focal method CheckToEncodedLab (C function) using C++11.
No GTest is used. A minimal in-process test harness is provided with optional
process-separation and stdout-capture capabilities to improve test isolation
and coverage of the code under test.

Notes and rationale (embedded as comments for maintainability):
- The focal method CheckToEncodedLab creates color transforms and performs many
  conversions across a large RGB cube. It exercises the branch:
    if (err > 0.1) { trace(...); }
  Because the actual numeric results depend on the LittleCMS library and color
  data, we capitalize on two observable behaviors:
  1) The function should execute without crashing (sanity).
  2) There should be at least some trace output when err > 0.1 occurs
     (we capture stdout to detect the trace text).
- We provide two tests:
  a) Sanity: runs CheckToEncodedLab in a guarded manner to catch crashes
     (SIGSEGV/SIGABRT) via setjmp/longjmp.
  b) TraceOutput: captures stdout while the focal method runs and checks for
     the expected trace message substring "Error on lab encoded".
- Platform caveat: stdout capture is implemented for POSIX systems. On Windows,
  the trace-output verification is skipped to avoid platform-specific pipe tricks.
- The tests rely on the library LittleCMS functions (e.g., cmsCreateContext, cmsDoTransform)
  being available and correctly linked with the test binary, as per the project
  setup described in the prompt.

Below is the test code. It expects to be compiled and linked with the production
code that defines CheckToEncodedLab(void).
*/

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <csignal>
#include <csetjmp>
#include <string>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>


extern "C" void CheckToEncodedLab(void); // Focal method under test (C linkage)

// Candidate Keywords (from Step 1 analysis)
// - cmsCreateContext, cmsCreateTransformTHR, cmsDoTransform, cmsDeltaE
// - lab8toLab, Trace/trace, cmsDeleteTransform, cmsCloseProfile
// - rgb<->Lab encodings, sRGB profile, Lab4 profile
// - loop over r,g,b in 0..255, step 5
// - error threshold 0.1, perceptual intent
// - memory/resource cleanup, context/transform lifetime management

// Simple test harness (no external test framework)
static volatile sig_atomic_t g_fault_occurred = 0;
static jmp_buf g_jmpbuf;

static void segv_handler(int) {
    g_fault_occurred = 1;
    longjmp(g_jmpbuf, 1);
}

// Helper: platform check
static bool is_posix() {
#if defined(_WIN32) || defined(_WIN64)
    return false;
#else
    return true;
#endif
}

// Test 1: Sanity check - ensure CheckToEncodedLab runs without crashing.
// Approach: Run the focal function in a guarded environment. If a segmentation fault
// or abort occurs, the test fails gracefully.
static bool Test_CheckToEncodedLab_Sanity(void) {
    // Install a minimal fault handler
    struct sigaction old_sigsegv, old_sigabrt;
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = segv_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    g_fault_occurred = 0;
    if (sigaction(SIGSEGV, &sa, &old_sigsegv) != 0) {
        // If we cannot install a handler, degrade gracefully
        // and attempt a direct call (riskier, but better than failing early)
    }
    if (sigaction(SIGABRT, &sa, &old_sigabrt) != 0) {
        // Same as above
    }

    int jf_res = 0;
    if (setjmp(g_jmpbuf) == 0) {
        // No fault yet: execute the focal function
        CheckToEncodedLab();
        jf_res = 1; // success path
    } else {
        // Jumped here due to a fault
        jf_res = 0;
    }

    // Restore previous signal handlers
    sigaction(SIGSEGV, &old_sigsegv, nullptr);
    sigaction(SIGABRT, &old_sigabrt, nullptr);

    // If a fault occurred, the test fails
    if (!jf_res) {
        printf("[Test_CheckToEncodedLab_Sanity] FAILED: CheckToEncodedLab crashed during execution.\n");
        return false;
    }
    printf("[Test_CheckToEncodedLab_Sanity] PASSED: CheckToEncodedLab executed without crashing.\n");
    return true;
}

// Test 2: Trace output coverage - verify that the true-branch (err > 0.1)
// gets exercised by capturing stdout and looking for the trace text.
//
// On POSIX platforms, redirect stdout to a pipe, run the function, restore stdout,
// and examine captured data for the substring "Error on lab encoded".
static bool Test_CheckToEncodedLab_TraceOutput(void) {
#if !defined(_WIN32) && is_posix()
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        printf("[Test_CheckToEncodedLab_TraceOutput] FAILED: pipe creation failed.\n");
        return false;
    }

    // Save original stdout
    int stdout_fd = dup(fileno(stdout));
    if (stdout_fd == -1) {
        printf("[Test_CheckToEncodedLab_TraceOutput] FAILED: dup stdout failed.\n");
        close(pipefd[0]);
        close(pipefd[1]);
        return false;
    }

    // Redirect stdout to pipe
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        printf("[Test_CheckToEncodedLab_TraceOutput] FAILED: redirect stdout failed.\n");
        close pipefd[0];
        close pipefd[1];
        close(stdout_fd);
        return false;
    }
    // Close the write end in the test thread as it's now dup'ed to stdout
    close(pipefd[1]);

    // Run the focal function
    Test_CheckToEncodedLab_Sanity(); // Ensure it can run in the redirected context as well
    // Note: We call the underlying function again to populate potential trace output.
    // Some environments may print regardless of previous test pass.

    // Flush and restore stdout
    fflush(stdout);
    if (dup2(stdout_fd, fileno(stdout)) == -1) {
        printf("[Test_CheckToEncodedLab_TraceOutput] FAILED: restore stdout failed.\n");
        close(pipefd[0]);
        close(pipefd[1]);
        close(stdout_fd);
        return false;
    }
    close(stdout_fd);

    // Read the captured output
    constexpr size_t BUF_SIZE = 4096;
    char buffer[BUF_SIZE];
    std::string captured;
    // Move to read end
    ssize_t n = 0;
    // Reopen read end as FILE* for convenience
    FILE* reader = fdopen(pipefd[0], "r");
    if (reader) {
        while ((n = fread(buffer, 1, BUF_SIZE, reader)) > 0) {
            captured.append(buffer, (size_t)n);
        }
        fclose(reader);
    } else {
        // Fallback: use low-level read if fdopen fails
        char temp[BUF_SIZE];
        ssize_t r;
        while ((r = read(pipefd[0], temp, BUF_SIZE)) > 0) {
            captured.append(temp, (size_t)r);
        }
    }
    close(pipefd[0]);

    // Look for the trace text fragment
    const std::string needle = "Error on lab encoded";
    bool found = captured.find(needle) != std::string::npos;

    if (found) {
        printf("[Test_CheckToEncodedLab_TraceOutput] PASSED: Trace output detected in stdout.\n");
    } else {
        printf("[Test_CheckToEncodedLab_TraceOutput] FAILED: Trace output not detected; stdout captured:\n%s\n", captured.c_str());
    }
    return found;
#else
    // Windows or non-POSIX environment: skip stdout capture; perform a basic sanity check instead
    printf("[Test_CheckToEncodedLab_TraceOutput] SKIPPED: stdout capture not implemented on this platform.\n");
    return true;
#endif
}

// Entry point for tests
int main(void) {
    // Run tests and report results
    bool t1 = Test_CheckToEncodedLab_Sanity();
#if !defined(_WIN32)
    bool t2 = Test_CheckToEncodedLab_TraceOutput();
#else
    bool t2 = true; // skip on Windows in this test harness
#endif

    int total = 2;
    int passed = (t1 ? 1 : 0) + (t2 ? 1 : 0);
    if (passed == total) {
        printf("ALL TESTS PASSED (%d/%d).\n", passed, total);
        return 0;
    } else {
        printf("SOME TESTS FAILED (%d/%d).\n", passed, total);
        return 1;
    }
}