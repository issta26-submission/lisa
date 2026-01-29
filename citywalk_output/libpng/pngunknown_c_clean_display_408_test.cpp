// Test suite for the focal function clean_display(display *d)
// This test suite is written in C++11 and avoids Google Test.
// It uses a minimal, self-contained approach with a small test harness.
// The production function is C code; we provide a small interposition
// for png_destroy_read_struct to avoid depending on a real libpng during tests.
// The tests cover:
//  - False branch: when all pointers are NULL, clean_display should return without exiting.
//  - True branch: when at least one pointer is non-NULL, clean_display should call exit(1).
//
// Notes:
// - The tests assume the following struct layout for display used by clean_display:
//   struct display { void* png_ptr; void* info_ptr; void* end_ptr; const char* file; const char* test; };
// - We declare clean_display and provide a matching struct in this test file to ensure linkage.
// - A tiny interposition for png_destroy_read_struct is provided to avoid libpng dependency.
// - On non-POSIX platforms, the true-branch test is skipped gracefully.

#include <setjmp.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <cstring>


// Forward declaration of the focal structure layout as used by clean_display.
// We align the fields and order to match the expected usage in the focal method.
struct display {
    void *png_ptr;
    void *info_ptr;
    void *end_ptr;
    const char *file;
    const char *test;
};

// Declaration of the focal function under test (as it would be seen by callers)
extern "C" void clean_display(struct display *d);

// Interposition: provide a dummy png_destroy_read_struct with C linkage.
// We intentionally do not modify the passed pointers here to trigger the
// true-branch behavior in clean_display when non-NULL pointers are present.
extern "C" void png_destroy_read_struct(void **png_ptr_ptr,
                                      void **info_ptr_ptr,
                                      void **end_ptr_ptr)
{
    // Do nothing: do not alter the input pointers.
    (void)png_ptr_ptr; (void)info_ptr_ptr; (void)end_ptr_ptr;
}

// Simple non-terminating assertion macro: records failures but continues.
#define EXPECT_TRUE(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "EXPECT_TRUE FAILED: " << (msg) \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            ++g_failures; \
        } \
    } while(0)

static int g_failures = 0;

// Test 1: False branch - all pointers NULL, clean_display should return without exiting.
bool test_clean_display_false_branch()
{
    // Arrange: all pointers NULL, with sample file/test names.
    display d;
    d.png_ptr = nullptr;
    d.info_ptr = nullptr;
    d.end_ptr = nullptr;
    d.file = "test_file_false";
    d.test = "test_false_case";

    // Act: call the focal function in the same process.
    // The interposed png_destroy_read_struct is a no-op, so no crash occurs.
    clean_display(&d);

    // Assert: since all pointers are NULL, the conditional should be false and
    // function should return normally (no exit called).
    // We check by re-validating that pointers remain NULL (they should).
    EXPECT_TRUE(d.png_ptr == nullptr, "png_ptr should remain NULL");
    EXPECT_TRUE(d.info_ptr == nullptr, "info_ptr should remain NULL");
    EXPECT_TRUE(d.end_ptr == nullptr, "end_ptr should remain NULL");

    return g_failures == 0;
}

// Test 2: True branch - at least one non-NULL pointer. We run the test in a child process
// to safely observe exit(1) without terminating the whole test harness.
// The child redirects stderr to a pipe so we can capture the error message.
bool test_clean_display_true_branch()
{
#if defined(__unix__) || defined(__APPLE__)
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        std::cerr << "Pipe creation failed" << std::endl;
        ++g_failures;
        return false;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed" << std::endl;
        ++g_failures;
        return false;
    }
    if (pid == 0) {
        // Child process
        // Redirect stderr to the pipe
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        // Prepare display with a non-NULL pointer to trigger the true-branch
        display d;
        d.png_ptr = (void*)0x1; // non-NULL
        d.info_ptr = nullptr;
        d.end_ptr = nullptr;
        d.file = "child_file_true";
        d.test = "child_true_case";

        // Call the function under test; we expect it to print an error and exit(1)
        clean_display(&d);

        // If we ever return here, exit with success (this should be rare)
        _exit(0);
    } else {
        // Parent process
        close(pipefd[1]); // Close write end; read from pipe
        // Wait for child to finish
        int status = 0;
        waitpid(pid, &status, 0);

        // Read what the child printed to stderr
        char buf[512];
        ssize_t r = read(pipefd[0], buf, sizeof(buf) - 1);
        if (r < 0) r = 0;
        buf[r] = '\0';

        close(pipefd[0]);

        // We expect the child to have exited with status 1
        if (WIFEXITED(status)) {
            int es = WEXITSTATUS(status);
            EXPECT_TRUE(es == 1, "Child did not exit with status 1 as expected");
        } else {
            EXPECT_TRUE(false, "Child did not terminate normally");
        }

        // Check that the error message mentions png_destroy_read_struct
        const char *expected_substr = "png_destroy_read_struct";
        bool found = (strstr(buf, expected_substr) != nullptr);
        EXPECT_TRUE(found, "Expected error message to mention png_destroy_read_struct");

        return g_failures == 0;
    }
#else
    // On non-POSIX platforms, we skip the true-branch test gracefully.
    std::cout << "Test skipped: true-branch requires POSIX fork/pipe support." << std::endl;
    return true;
#endif
}

int main()
{
    std::cout << "Running tests for clean_display...\n";

    // Run Test 1
    bool t1 = test_clean_display_false_branch();
    std::cout << "Test 1 (false branch) " << (t1 ? "PASSED" : "FAILED") << "\n";

    // Run Test 2
    bool t2 = test_clean_display_true_branch();
    std::cout << "Test 2 (true branch) " << (t2 ? "PASSED" : "FAILED") << "\n";

    if (t1 && t2 && g_failures == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << "Some tests FAILED. Failures: " << g_failures << "\n";
        return 1;
    }
}