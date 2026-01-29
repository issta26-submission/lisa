// Unit test suite for the focal method: StringPtr in cmscgats.c
// This test targets the function with signature: char* StringPtr(string* s)
// It uses a minimal, self-contained test harness (no GTest) with non-terminating assertions.
// Candidate Keywords guiding test generation: StringPtr, string, begin, cmscgats.c, pointer return

#include <cstring>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>


// Step 1: Provide compatibility typedefs for the "string" type used by StringPtr.
// We declare a minimal replica of the expected type to allow linker-to-function testing.
// It mirrors the essential member used by StringPtr: a pointer named 'begin'.
typedef struct string string;
struct string {
    char* begin;
};

// Step 2: Declare the focal function with C linkage to ensure proper linkage with cmscgats.c
extern "C" char* StringPtr(string* s);

// --------------------------------------------------------------------------------
// Simple, non-terminating test harness
// Keeps track of total tests and failures, prints detailed messages on failure,
// but does not abort execution to maximize code coverage.
// --------------------------------------------------------------------------------

static int g_total_tests = 0;
static int g_failed_tests = 0;

// Helper for pointer equality assertions (for char* pointers)
static void expect_ptr_eq(char* actual, char* expected, const char* msg) {
    ++g_total_tests;
    if (actual != expected) {
        ++g_failed_tests;
        std::cerr << "TEST FAIL: " << msg << "\n"
                  << "  Actual  pointer: " << static_cast<void*>(actual) << "\n"
                  << "  Expected pointer: " << static_cast<void*>(expected) << "\n";
    } else {
        // Optional success message (kept quiet to reduce noisy output)
        // std::cout << "TEST PASS: " << msg << "\n";
    }
}

// Test 1: Basic sanity - StringPtr should return the exact s->begin pointer
// This ensures the function does not modify the pointer and dereferences correctly.
static void test_StringPtr_basic_pointer_preserved() {
    // Arrange
    string s;
    char buffer[16] = "hello";
    s.begin = buffer;

    // Act
    char* res = StringPtr(&s);

    // Assert
    expect_ptr_eq(res, s.begin,
                  "StringPtr should return the same pointer as s->begin (basic case)");
}

// Test 2: Null begin member - ensure NULL is preserved as a valid pointer value
// Verifies function returns the NULL pointer when s->begin is NULL.
static void test_StringPtr_null_begin() {
    // Arrange
    string s;
    s.begin = NULL;

    // Act
    char* res = StringPtr(&s);

    // Assert
    expect_ptr_eq(res, NULL,
                  "StringPtr should return NULL when s->begin is NULL");
}

// Test 3: Begin points to a string literal - verify pointer identity for constant data
static void test_StringPtr_begin_points_to_literal() {
    // Arrange
    string s;
    const char* lit = "literal";
    s.begin = const_cast<char*>(lit);

    // Act
    char* res = StringPtr(&s);

    // Assert
    expect_ptr_eq(res, (char*)lit,
                  "StringPtr should return the address of the literal that s.begin points to");
}

// Test 4: Begin points to dynamically allocated memory - ensure pointer equality remains intact
static void test_StringPtr_begin_points_to_heap() {
    // Arrange
    string s;
    char* mem = (char*)std::malloc(32);
    if (mem == NULL) {
        std::cerr << "TEST SKIP: malloc failed in test_StringPtr_begin_points_to_heap\n";
        return;
    }
    std::strcpy(mem, "heap memory");
    s.begin = mem;

    // Act
    char* res = StringPtr(&s);

    // Assert
    expect_ptr_eq(res, mem, "StringPtr should return the pointer to heap-allocated memory stored in s.begin");

    // Cleanup
    std::free(mem);
}

// Helper to run all tests
static void run_all_tests() {
    test_StringPtr_basic_pointer_preserved();
    test_StringPtr_null_begin();
    test_StringPtr_begin_points_to_literal();
    test_StringPtr_begin_points_to_heap();
}

// --------------------------------------------------------------------------------
// Main entry
// Runs tests and reports summary. Exit code 0 on success, 1 if any test failed.
// --------------------------------------------------------------------------------
int main() {
    run_all_tests();

    std::cout << "Total tests run: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;

    return (g_failed_tests == 0) ? 0 : 1;
}