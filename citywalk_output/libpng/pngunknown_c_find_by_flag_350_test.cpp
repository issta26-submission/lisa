// Unit test suite for the focal method: find_by_flag
// This test reproduces the minimal environment required by the function:
// - A chunk_info array with a flag member
// - A constant NINFO representing the number of info entries
// - A test-friendly override of exit() to capture non-local termination
// - A C linkage for find_by_flag to mimic the original C function usage

#include <cstdint>
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>


// Domain-specific: define the minimal types used by the focal function
typedef uint32_t png_uint_32;

// Dependency: a tiny chunk_info structure used by the function
typedef struct {
    png_uint_32 flag;
} chunk_info_t;

// Minimal test environment mirroring the original code's globals
static const int NINFO = 4;            // number of info entries
static chunk_info_t chunk_info[NINFO];  // array of info entries

// Jump buffer used to intercept the exit(4) path
static jmp_buf test_jmpbuf;

// Override of the standard exit() function to enable testable non-local exits.
// This captures exit(4) calls from the focal function and returns control to tests
extern "C" void exit(int /*status*/)
{
    // Transplant control back to the point where setjmp(test_jmpbuf) was called
    longjmp(test_jmpbuf, 1);
}

// The focal method under test, implemented with C linkage to resemble the original.
// It searches for the last entry with a matching flag value, returning its index.
// If no match is found, it reports an internal error and exits with status 4.
extern "C" int find_by_flag(png_uint_32 flag)
{
{
   int i = NINFO;
   while (--i >= 0)
      if (chunk_info[i].flag == flag)
         return i;
   fprintf(stderr, "pngunknown: internal error\n");
   exit(4);
}
}

// Simple test harness (without GTest) to validate the focal method.
// It covers:
 // - A positive path where the flag matches an entry (testing the last match behavior)
 // - Another positive path with duplicates to ensure last occurrence is chosen
 // - A negative path where the flag is absent, intercepting exit(4) via setjmp/longjmp
int main(void)
{
    int total_tests = 0;
    int failed_tests = 0;

    // Helper lambda-like blocks are emulated by sequential blocks with explanatory comments.

    // Test 1: Flag matches the last entry (duplicate presence)
    // Expected: return the index of the last entry whose flag equals the given value.
    {
        // Arrange: set up flags such that 20 appears at indices 1 and 3; last is index 3
        chunk_info[0].flag = 10;
        chunk_info[1].flag = 20;
        chunk_info[2].flag = 30;
        chunk_info[3].flag = 20;

        // Act
        int idx = find_by_flag(20);

        // Assert
        ++total_tests;
        if (idx != 3) {
            ++failed_tests;
            std::printf("Test 1 FAILED: expected index 3 for flag=20, got %d\n", idx);
        } else {
            std::printf("Test 1 PASSED: found last occurrence at index 3 for flag=20\n");
        }
    }

    // Test 2: Flag matches a non-tail entry (duplicate elsewhere)
    // Expected: return the index of the last occurrence of the flag value (here it should be 1)
    {
        // Arrange: create duplicates of 99 at indices 0 and 1
        chunk_info[0].flag = 99;
        chunk_info[1].flag = 99;
        chunk_info[2].flag = 55;
        chunk_info[3].flag = 20;

        // Act
        int idx = find_by_flag(99);

        // Assert
        ++total_tests;
        if (idx != 1) {
            ++failed_tests;
            std::printf("Test 2 FAILED: expected index 1 for flag=99, got %d\n", idx);
        } else {
            std::printf("Test 2 PASSED: found last occurrence at index 1 for flag=99\n");
        }
    }

    // Test 3: Flag not found should trigger exit(4); intercept with setjmp/longjmp
    {
        // Arrange: ensure the flag is not present in the array
        chunk_info[0].flag = 11;
        chunk_info[1].flag = 22;
        chunk_info[2].flag = 33;
        chunk_info[3].flag = 44;

        // Assert: intercept exit via longjmp
        ++total_tests;
        if (setjmp(test_jmpbuf) == 0) {
            // Act
            int ret = find_by_flag(999); // non-existent flag
            // If control returns here, the exit interception failed
            (void)ret; // silence unused warning
            std::printf("Test 3 FAILED: expected exit to be intercepted, but function returned %d\n", ret);
            ++failed_tests;
        } else {
            // We landed here via longjmp from exit(), meaning exit(4) path was exercised
            std::printf("Test 3 PASSED: exit(4) intercepted by test harness\n");
        }
    }

    // Summary
    std::printf("Total tests: %d, Failures: %d\n", total_tests, failed_tests);
    return failed_tests == 0 ? 0 : 1;
}

// Notes:
// - This test suite uses a minimal environment to exercise the focal method.
// - Exit interception is achieved by providing an override of exit(int) that performs a longjmp.
// - Static members and class dependencies from the actual project are not required here; the test
//   focuses solely on the behavior of find_by_flag and its immediate dependencies.
// - The tests are self-contained and do not rely on the GTest framework, per the constraints.