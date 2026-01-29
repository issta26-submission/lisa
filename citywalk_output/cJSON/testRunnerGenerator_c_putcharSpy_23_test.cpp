/*
Test suite for the focal method:
- putcharSpy(int c) which is a passthrough to putchar.
- Goal: verify that putcharSpy writes the expected character(s) to stdout.
- Approach: capture stdout via a POSIX pipe by redirecting STDOUT_FILENO, invoke putcharSpy, then restore stdout and verify the captured bytes.
- Platform caveat: POSIX pipe-based capture is used. On non-POSIX platforms, tests will be skipped with a note.
- This test harness is a lightweight C11-based runner (no GTest/GMock required) focused on correctness and coverage for the passthrough behavior.
*/

#include <unistd.h>
#include <unity.h>
#include <string.h>
#include <Defs.h>
#include <stdlib.h>
#include <CException.h>
#include <errno.h>
#include <stdio.h>


// The focal function is expected to be linked in from the production code.
// It is the passthrough wrapper around putchar(int c).
extern void putcharSpy(int c);

#if defined(_WIN32) || defined(_WIN64)
#define CAPTURE_SUPPORTED 0
#else
#define CAPTURE_SUPPORTED 1
#endif

// Test 1: Verify that putcharSpy writes the character 'A' to stdout.
// This uses a POSIX pipe to capture a single character written to stdout.
static int test_putcharSpy_WritesSingleCharacter_A(void) {
#if CAPTURE_SUPPORTED
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        perror("pipe");
        return 1;
    }

    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1) {
        perror("dup");
        close(pipefd[0]);
        close(pipefd[1]);
        return 1;
    }

    // Redirect stdout to the write end of the pipe
    if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
        perror("dup2");
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        return 1;
    }
    close(pipefd[1]); // Write end is now stdout

    // Call the function under test
    putcharSpy('A');
    fflush(stdout);

    // Restore original stdout
    if (dup2(saved_stdout, STDOUT_FILENO) == -1) {
        perror("dup2 restore");
        close(pipefd[0]);
        close(saved_stdout);
        return 1;
    }
    close(saved_stdout);

    // Read captured output
    char ch;
    ssize_t r = read(pipefd[0], &ch, 1);
    close(pipefd[0]);
    if (r != 1) {
        fprintf(stderr, "Expected 1 byte, got %zd\n", r);
        return 1;
    }
    if ((unsigned char)ch != (unsigned char)'A') {
        fprintf(stderr, "Expected 'A', got 0x%02x\n", (unsigned char)ch);
        return 1;
    }
    return 0;
#else
    printf("Skipping test_putcharSpy_WritesSingleCharacter_A on non-POSIX platform.\n");
    return 0;
#endif
}

// Test 2: Verify that consecutive calls to putcharSpy write two characters in order, e.g., 'X' then 'Y'.
// We capture both bytes in a single read after redirecting stdout.
static int test_putcharSpy_WritesMultipleCharacters_XY(void) {
#if CAPTURE_SUPPORTED
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        perror("pipe");
        return 1;
    }

    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1) {
        perror("dup");
        close(pipefd[0]);
        close(pipefd[1]);
        return 1;
    }

    if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
        perror("dup2");
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        return 1;
    }
    close(pipefd[1]);

    // Two consecutive characters
    putcharSpy('X');
    putcharSpy('Y');
    fflush(stdout);

    // Restore stdout
    if (dup2(saved_stdout, STDOUT_FILENO) == -1) {
        perror("dup2 restore");
        close(pipefd[0]);
        close(saved_stdout);
        return 1;
    }
    close(saved_stdout);

    // Read two bytes
    char buf[3] = {0};
    ssize_t total = 0;
    while (total < 2) {
        ssize_t r = read(pipefd[0], buf + total, 2 - total);
        if (r <= 0) break;
        total += r;
    }
    close(pipefd[0]);
    if (total != 2) {
        fprintf(stderr, "Expected 2 bytes, got %zd\n", total);
        return 1;
    }
    if ((unsigned char)buf[0] != (unsigned char)'X' || (unsigned char)buf[1] != (unsigned char)'Y') {
        fprintf(stderr, "Expected \"XY\", got \"%c%c\" (0x%02x 0x%02x)\n",
                buf[0], buf[1], (unsigned char)buf[0], (unsigned char)buf[1]);
        return 1;
    }
    return 0;
#else
    printf("Skipping test_putcharSpy_WritesMultipleCharacters_XY on non-POSIX platform.\n");
    return 0;
#endif
}

// Test 3: Verify that putcharSpy can handle a null character (0x00) without error.
// The captured byte should be 0x00.
static int test_putcharSpy_WritesNullByte(void) {
#if CAPTURE_SUPPORTED
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        perror("pipe");
        return 1;
    }

    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1) {
        perror("dup");
        close(pipefd[0]);
        close(pipefd[1]);
        return 1;
    }

    if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
        perror("dup2");
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        return 1;
    }
    close(pipefd[1]);

    putcharSpy(0x00);
    fflush(stdout);

    if (dup2(saved_stdout, STDOUT_FILENO) == -1) {
        perror("dup2 restore");
        close(pipefd[0]);
        close(saved_stdout);
        return 1;
    }
    close(saved_stdout);

    char ch;
    ssize_t r = read(pipefd[0], &ch, 1);
    close(pipefd[0]);
    if (r != 1) {
        fprintf(stderr, "Expected 1 byte, got %zd\n", r);
        return 1;
    }
    if ((unsigned char)ch != 0x00) {
        fprintf(stderr, "Expected 0x00, got 0x%02x\n", (unsigned char)ch);
        return 1;
    }
    return 0;
#else
    printf("Skipping test_putcharSpy_WritesNullByte on non-POSIX platform.\n");
    return 0;
#endif
}

// Test 4: Verify that putcharSpy handles extended byte values (e.g., 0xFF).
static int test_putcharSpy_WritesExtendedByte(void) {
#if CAPTURE_SUPPORTED
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        perror("pipe");
        return 1;
    }

    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1) {
        perror("dup");
        close(pipefd[0]);
        close(pipefd[1]);
        return 1;
    }

    if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
        perror("dup2");
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        return 1;
    }
    close(pipefd[1]);

    putcharSpy(0xFF);
    fflush(stdout);

    if (dup2(saved_stdout, STDOUT_FILENO) == -1) {
        perror("dup2 restore");
        close(pipefd[0]);
        close(saved_stdout);
        return 1;
    }
    close(saved_stdout);

    char ch;
    ssize_t r = read(pipefd[0], &ch, 1);
    close(pipefd[0]);
    if (r != 1) {
        fprintf(stderr, "Expected 1 byte, got %zd\n", r);
        return 1;
    }
    if ((unsigned char)ch != 0xFF) {
        fprintf(stderr, "Expected 0xFF, got 0x%02x\n", (unsigned char)ch);
        return 1;
    }
    return 0;
#else
    printf("Skipping test_putcharSpy_WritesExtendedByte on non-POSIX platform.\n");
    return 0;
#endif
}

int main(void) {
    const int totalTests = 4;
    int failures = 0;

    printf("Starting test suite for putcharSpy...\n\n");

    // Test 1
    printf("Running test_putcharSpy_WritesSingleCharacter_A...\n");
    if (test_putcharSpy_WritesSingleCharacter_A() != 0) {
        printf("FAIL: test_putcharSpy_WritesSingleCharacter_A\n");
        failures++;
    } else {
        printf("PASS: test_putcharSpy_WritesSingleCharacter_A\n");
    }
    printf("\n");

    // Test 2
    printf("Running test_putcharSpy_WritesMultipleCharacters_XY...\n");
    if (test_putcharSpy_WritesMultipleCharacters_XY() != 0) {
        printf("FAIL: test_putcharSpy_WritesMultipleCharacters_XY\n");
        failures++;
    } else {
        printf("PASS: test_putcharSpy_WritesMultipleCharacters_XY\n");
    }
    printf("\n");

    // Test 3
    printf("Running test_putcharSpy_WritesNullByte...\n");
    if (test_putcharSpy_WritesNullByte() != 0) {
        printf("FAIL: test_putcharSpy_WritesNullByte\n");
        failures++;
    } else {
        printf("PASS: test_putcharSpy_WritesNullByte\n");
    }
    printf("\n");

    // Test 4
    printf("Running test_putcharSpy_WritesExtendedByte...\n");
    if (test_putcharSpy_WritesExtendedByte() != 0) {
        printf("FAIL: test_putcharSpy_WritesExtendedByte\n");
        failures++;
    } else {
        printf("PASS: test_putcharSpy_WritesExtendedByte\n");
    }
    printf("\n");

    printf("Test suite completed: %d/%d tests failed.\n", failures, totalTests);
    return failures ? 1 : 0;
}