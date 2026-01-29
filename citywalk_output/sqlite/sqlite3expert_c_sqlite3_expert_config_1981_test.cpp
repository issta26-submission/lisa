// Test suite for the focal method: sqlite3_expert_config
// Focus: verify EXPERT_CONFIG_SAMPLE handling (clamping 0..100) and default (not found) path.
// This test is designed for C++11, without GoogleTest/GMock, using a lightweight assertion approach.
// It assumes the project provides sqlite3expert.h including the sqlite3expert struct with at least
// a member: int iSample; and the constants EXPERT_CONFIG_SAMPLE, SQLITE_OK, SQLITE_NOTFOUND.

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <cstdint>
#include <sqlite3expert.h>


// Include the C header under C linkage to ensure proper name mangling for the C function.
// The header (sqlite3expert.h) must declare sqlite3expert and the relevant symbols used here.
extern "C" {
}

// Lightweight non-terminating test assertions (only report failures, continue execution)
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << msg << " (Line " << __LINE__ << ")\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if(!((a) == (b))) { \
        std::cerr << "[FAIL] " << msg \
                  << " | Expected: " << (static_cast<long long>(a)) \
                  << "  Got: " << (static_cast<long long>(b)) \
                  << " (Line " << __LINE__ << ")\n"; \
        ++g_failures; \
    } \
} while(0)

// Test 1: In-range input should set iSample and return SQLITE_OK
// Domain note: TRUE branch of the iVal range check (0 <= iVal <= 100).
static void test_config_sample_in_range() {
    // Setup: a sqlite3expert instance with an initial value
    sqlite3expert p;
    p.iSample = -1; // initial junk value

    // Exercise: call with a valid value within range
    int rc = sqlite3_expert_config(&p, EXPERT_CONFIG_SAMPLE, 42);

    // Verify outcomes
    EXPECT_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK for in-range input");
    EXPECT_EQ(p.iSample, 42, "p.iSample should be updated to 42 for in-range input");
}

// Test 2: Negative input should clamp to 0
// Domain note: lower bound clamping path (iVal < 0) should set iSample to 0.
static void test_config_sample_below_range() {
    sqlite3expert p;
    p.iSample = 50;

    int rc = sqlite3_expert_config(&p, EXPERT_CONFIG_SAMPLE, -5);

    EXPECT_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK for negative input");
    EXPECT_EQ(p.iSample, 0, "p.iSample should be clamped to 0 when input is negative");
}

// Test 3: Input above 100 should clamp to 100
// Domain note: upper bound clamping path (iVal > 100) should set iSample to 100.
static void test_config_sample_above_range() {
    sqlite3expert p;
    p.iSample = 20;

    int rc = sqlite3_expert_config(&p, EXPERT_CONFIG_SAMPLE, 150);

    EXPECT_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK for input above range");
    EXPECT_EQ(p.iSample, 100, "p.iSample should be clamped to 100 when input exceeds 100");
}

// Test 4: Unsupported operation should return SQLITE_NOTFOUND
// Domain note: default branch of the switch should be exercised.
static void test_config_not_found_op() {
    sqlite3expert p;
    p.iSample = 0;

    // Use an arbitrary op code that is not EXPERT_CONFIG_SAMPLE
    int invalid_op = 9999;
    int rc = sqlite3_expert_config(&p, invalid_op);

    EXPECT_EQ(rc, SQLITE_NOTFOUND, "rc should be SQLITE_NOTFOUND for unsupported op");
}

// Helper: print a short summary
static void print_summary() {
    std::cout << "=== sqlite3_expert_config test summary ===\n";
    if(g_failures == 0) {
        std::cout << "All tests PASSED.\n";
    } else {
        std::cout << g_failures << " test(s) FAILED.\n";
    }
    std::cout << "=========================================\n";
}

// Main: run all tests
int main() {
    // Run tests
    test_config_sample_in_range();
    test_config_sample_below_range();
    test_config_sample_above_range();
    test_config_not_found_op();

    // Summary
    print_summary();

    // Return code 0 if all tests passed, non-zero if any failed
    return (g_failures == 0) ? 0 : 1;
}