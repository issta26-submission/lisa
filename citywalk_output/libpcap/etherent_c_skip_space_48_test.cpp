// test_skip_space.cpp
// Unit tests for skip_space(FILE *f) in the provided C source.
// Compile with: g++ -std=c++11 test_skip_space.cpp etherent.c -o test_skip_space
#include <string.h>
#include <pcap/namedb.h>
#include <nametoaddr.h>
#include <memory.h>
#include <os-proto.h>
#include <thread-local.h>
#include <stdio.h>
#include <stdlib.h>
#include <config.h>
#include <pcap-int.h>
#include <pcap-types.h>


extern "C" int skip_space(FILE *f); // C function under test

// Simple non-terminating test assertion macro (does not terminate on failure)
static int g_failures = 0;
#define EXPECT_EQ_INT(expected, actual) do { \
    int _exp = (int)(expected); \
    int _act = (int)(actual); \
    if (_exp != _act) { \
        fprintf(stderr, "EXPECT_EQ_INT failed: expected %d, got %d (at %s:%d)\n", _exp, _act, __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while (0)

// Helper to create an in-memory FILE* with given content and rewind to start
static FILE* make_file_with(const char* content) {
    // tmpfile creates a temporary file object (read/write)
    FILE* f = tmpfile();
    if (!f) {
        perror("tmpfile");
        return NULL;
    }
    if (content && content[0] != '\0') {
        // write content to the file
        fwrite(content, 1, strlen(content), f);
        fflush(f);
    }
    // rewind to start for reading
    fseek(f, 0, SEEK_SET);
    return f;
}

// Test 1: Leading spaces, then letters: expect first non-space character 'A'
static void test_skip_space_leading_spaces_returns_first_char() {
    // "   ABC" -> should skip 3 spaces and return 'A'
    const char* content = "   ABC";
    FILE* f = make_file_with(content);
    if (!f) {
        fprintf(stderr, "Test 1: failed to create input file\n");
        g_failures++;
        return;
    }

    int result = skip_space(f);
    fclose(f);
    EXPECT_EQ_INT((int) 'A', result);
}

// Test 2: Mixed whitespace including tab, carriage return, and newline:
// content starts with spaces/tabs/CR, then a newline; since '\n' is not in skip-space set,
// the function should return '\n'
static void test_skip_space_handles_tab_cr_and_newline() {
    // "\t\r\nX" would skip tab and CR and stop at '\n' (which is returned)
    const char* content = "\t\r\nX";
    FILE* f = make_file_with(content);
    if (!f) {
        fprintf(stderr, "Test 2: failed to create input file\n");
        g_failures++;
        return;
    }

    int result = skip_space(f);
    fclose(f);
    EXPECT_EQ_INT((int) '\n', result);
}

// Test 3: No leading whitespace: first character should be returned immediately
static void test_skip_space_no_leading_space_returns_first_char() {
    const char* content = "ABC";
    FILE* f = make_file_with(content);
    if (!f) {
        fprintf(stderr, "Test 3: failed to create input file\n");
        g_failures++;
        return;
    }

    int result = skip_space(f);
    fclose(f);
    EXPECT_EQ_INT((int) 'A', result);
}

// Test 4: Only spaces: should reach EOF and return EOF
static void test_skip_space_only_spaces_returns_eof() {
    const char* content = "    "; // four spaces
    FILE* f = make_file_with(content);
    if (!f) {
        fprintf(stderr, "Test 4: failed to create input file\n");
        g_failures++;
        return;
    }

    int result = skip_space(f);
    fclose(f);
    EXPECT_EQ_INT((int) EOF, result);
}

// Test 5: Empty file: should return EOF
static void test_skip_space_empty_file_returns_eof() {
    const char* content = "";
    FILE* f = make_file_with(content);
    if (!f) {
        fprintf(stderr, "Test 5: failed to create input file\n");
        g_failures++;
        return;
    }

    int result = skip_space(f);
    fclose(f);
    EXPECT_EQ_INT((int) EOF, result);
}

// Test 6: Leading tab and CR before a letter: should skip to the letter 'X'
static void test_skip_space_tabs_cr_before_char() {
    const char* content = "\t\rX";
    FILE* f = make_file_with(content);
    if (!f) {
        fprintf(stderr, "Test 6: failed to create input file\n");
        g_failures++;
        return;
    }

    int result = skip_space(f);
    fclose(f);
    EXPECT_EQ_INT((int) 'X', result);
}

// Helper to run all tests and report summary
static void run_all_tests() {
    // Ensure clean state
    g_failures = 0;

    test_skip_space_leading_spaces_returns_first_char();
    test_skip_space_handles_tab_cr_and_newline();
    test_skip_space_no_leading_space_returns_first_char();
    test_skip_space_only_spaces_returns_eof();
    test_skip_space_empty_file_returns_eof();
    test_skip_space_tabs_cr_before_char();

    if (g_failures == 0) {
        printf("All tests PASSED.\n");
    } else {
        printf("Total failures: %d\n", g_failures);
    }
}

// Main entry: Run tests
int main() {
    run_all_tests();
    // Exit code indicates number of failures (0 means success)
    return g_failures;
}