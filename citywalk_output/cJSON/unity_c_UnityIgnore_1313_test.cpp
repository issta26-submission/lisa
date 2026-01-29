// Test suite for the UnityIgnore function located in unity.c
// This test suite is written in C++11 (no GTest) and relies on the
// Unity C framework being linkable with a C API.
// The goal is to exercise the two branches of the UnityIgnore implementation:
//
// 1) When msg is NULL (no additional message printed)
// 2) When msg is non-NULL (prints the message after a colon and a space)
//
// Given the provided scaffolding (Unity library stubs), the test focuses on
// ensuring the function can be invoked without crashing and that both input
// forms can be exercised.

// Note: This test does not rely on actual console output. It invokes UnityIgnore
// with different parameters and ensures no runtime crash occurs. This aligns
// with the stubs in the provided Unity dependencies where output functions may
// be implemented as no-ops in this test environment.

#include <stddef.h>
#include <iostream>
#include <string>
#include <unity.h>


// Include the Unity header to get the declaration of UnityIgnore and the
// related types. The Unity project in use should provide a functioning unity.h.
extern "C" {
}

// Simple non-terminating assertion macro for test checkpoints.
// It does not abort on failure, but records a failure count to allow a summary.
static int g_failed = 0;
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " #cond " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failed; \
    } \
} while(0)

#define EXPECT_NO_CRASH(call) do { \
    /* We call the syntax either as a statement or expression; ignore return */ \
    (void) (call); \
} while(0)


// Test 1: Call UnityIgnore with NULL message.
// This exercises the branch where the message is not printed.
void test_UnityIgnore_WithNullMessage() {
    // Call with NULL message and a sample line number.
    // Ensure no crash and that the function returns (void).
    EXPECT_NO_CRASH(UnityIgnore(nullptr, 10));
}

// Test 2: Call UnityIgnore with a non-NULL message.
// This exercises the branch where the message is printed after a colon and a space.
void test_UnityIgnore_WithNonNullMessage() {
    // Use a sample message to exercise the code path that prints a custom message.
    EXPECT_NO_CRASH(UnityIgnore("Something went wrong", 20));
}


// Minimal test runner to satisfy the "main" entry point requirement.
// The tests are purposely simple to ensure compatibility with various
// Unity harness configurations.
int main() {
    // Run tests
    test_UnityIgnore_WithNullMessage();
    test_UnityIgnore_WithNonNullMessage();

    // Summary
    if (g_failed == 0) {
        std::cout << "UnityIgnore tests passed (no observable crashes) across branches." << std::endl;
        return 0;
    } else {
        std::cerr << "UnityIgnore tests had " << g_failed << " non-fatal failures." << std::endl;
        return 1;
    }
}