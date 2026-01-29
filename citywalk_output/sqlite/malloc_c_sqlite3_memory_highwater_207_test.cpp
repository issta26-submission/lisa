// Unit test suite for sqlite3_memory_highwater using a lightweight, non-GTest harness.
// This test mocks the sqlite3_status64 behavior to simulate memory usage and highwater tracking.
// The goal is to verify the highwater semantics for resetFlag = 0 and resetFlag = 1 across scenarios.
// Notes:
// - No external test frameworks are used (per instructions).
// - We provide a minimal mock of sqlite3_status64 to drive sqlite3_memory_highwater without a real SQLite runtime.
// - Each test step includes explanatory comments and uses non-terminating EXPECT_EQ-style checks.

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>
#include <stdarg.h>


// Lightweight typedef compatibility for the focal code.
// The real project uses sqlite3_int64; we align with a 64-bit signed integer.
typedef long long sqlite3_int64;

// Forward declaration of the function under test from the target file (malloc.c).
extern "C" sqlite3_int64 sqlite3_memory_highwater(int resetFlag);

// Expose a mock of sqlite3_status64 to drive the behavior of sqlite3_memory_highwater.
// The real code calls sqlite3_status64(SQLITE_STATUS_MEMORY_USED, &res, &mx, resetFlag);
extern "C" void sqlite3_status64(int op, sqlite3_int64 *pCurrent, sqlite3_int64 *pHighwater, int resetFlag);

// A simple, global memory model to drive the mock.
// g_current_memory mimics "current memory used".
// g_highwater_memory mimics "highwater memory usage" tracked by the system.
static sqlite3_int64 g_current_memory = 0;
static sqlite3_int64 g_highwater_memory = 0;

// A trivial opcode constant to identify the memory-used operation in the mock.
// This mirrors the usage in sqlite3_memory_highwater.
static const int SQLITE_STATUS_MEMORY_USED = 0x01; // Value is not important for the test harness beyond equality checks.

// Lightweight test assertion mechanism (non-terminating).
static int g_failures = 0;
#define EXPECT_EQ(actual, expected) do { \
    if ((actual) != (expected)) { \
        std::cerr << "EXPECT_EQ failed at line " << __LINE__ << ": " \
                  << (actual) << " != " << (expected) << "\n"; \
        ++g_failures; \
    } \
} while (0)

// Mock implementation of sqlite3_status64.
// It should emulate:
// - pCurrent = current memory used.
// - pHighwater = highwater (updated as max of current, previous highwater).
// - If resetFlag is non-zero, highwater is reset to current before returning highwater.
// - The function is invoked by sqlite3_memory_highwater and must be visible for the linker.
extern "C" void sqlite3_status64(int op, sqlite3_int64 *pCurrent, sqlite3_int64 *pHighwater, int resetFlag) {
    if (op != SQLITE_STATUS_MEMORY_USED) {
        // Only the SQLITE_STATUS_MEMORY_USED branch is relevant for this test.
        return;
    }

    // Expose current memory used to the caller.
    if (pCurrent) {
        *pCurrent = g_current_memory;
    }

    // Highwater logic:
    // - If resetFlag is non-zero: highwater becomes current.
    // - Regardless, highwater should reflect the maximum observed current unless reset.
    if (pHighwater) {
        if (resetFlag) {
            g_highwater_memory = g_current_memory;
        }
        // Emulate typical sqlite3 highwater maintenance: highwater is at least the max(current, prior highwater).
        if (g_current_memory > g_highwater_memory) {
            g_highwater_memory = g_current_memory;
        }
        *pHighwater = g_highwater_memory;
    }
}

// For completeness, declare the memory_highwater function prototype expected by the test harness.
extern "C" sqlite3_int64 sqlite3_memory_highwater(int resetFlag);

// Entry point for the test harness.
int main() {
    std::cout << "sqlite3_memory_highwater unit tests (mocked sqlite3_status64)\n";

    // Test 1: Initial state - both current and highwater are zero.
    g_current_memory = 0;
    g_highwater_memory = 0;
    {
        sqlite3_int64 v = sqlite3_memory_highwater(0);
        EXPECT_EQ(v, 0);
    }

    // Test 2: Increase current memory to 10; highwater should reflect 10 when resetFlag is 0.
    g_current_memory = 10;
    {
        sqlite3_int64 v = sqlite3_memory_highwater(0);
        // After first observation, highwater becomes 10.
        EXPECT_EQ(v, 10);
    }

    // Test 3: Reduce current memory to 5; highwater should remain at 10 (no reset).
    g_current_memory = 5;
    {
        sqlite3_int64 v = sqlite3_memory_highwater(0);
        EXPECT_EQ(v, 10);
    }

    // Test 4: Increase current memory to 20; highwater should rise to 20.
    g_current_memory = 20;
    {
        sqlite3_int64 v = sqlite3_memory_highwater(0);
        EXPECT_EQ(v, 20);
    }

    // Test 5: Reset highwater to current with resetFlag = 1 (should set highwater to 25 and return 25).
    g_current_memory = 25;
    {
        sqlite3_int64 v = sqlite3_memory_highwater(1);
        EXPECT_EQ(v, 25);
    }

    // Test 6: With resetFlag = 0 and current memory 30, highwater should update to max(25,30)=30.
    g_current_memory = 30;
    {
        sqlite3_int64 v = sqlite3_memory_highwater(0);
        EXPECT_EQ(v, 30);
    }

    // Test 7: Reset again to a lower current value (28) with resetFlag = 1, highwater becomes 28.
    g_current_memory = 28;
    {
        sqlite3_int64 v = sqlite3_memory_highwater(1);
        EXPECT_EQ(v, 28);
    }

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed\n";
    } else {
        std::cerr << g_failures << " test(s) failed\n";
    }

    // Return non-zero if any test failed to signal abnormal termination in environments that check return codes.
    return (g_failures == 0) ? 0 : 1;
}