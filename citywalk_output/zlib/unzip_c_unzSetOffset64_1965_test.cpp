// Unit test suite for the focal method: unzSetOffset64
// Target: verify behavior of unzSetOffset64 as defined in unzip.c
// Approach: use lightweight, self-contained C++ test harness (no GTest)
// Notes:
// - Assumes the unzip.h header provides the required types and constants
//   (e.g., unzFile, unz64_s, UNZ_PARAMERROR, UNZ_OK, ZPOS64_T, etc.)
// - Tests focus on public observable effects: parameter validation, side-effects
//   on the unz64_s instance fields, and the relation between return value and state.
// - The test-suite compiles with a project that links against the unzip.c implementation.

#include <cstdio>
#include <cstring>
#include <string.h>
#include <cstdint>
#include <stdio.h>
#include <crypt.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <unzip.h>


// Include the unzip API/types used by the focal method

// Lightweight test framework (no external libraries)
static int g_failures = 0;

#define ASSERT_TRUE(cond) \
    do { \
        if(!(cond)) { \
            std::cerr << "Assertion failed: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
            ++g_failures; \
        } \
    } while(0)

#define ASSERT_EQ(a, b) \
    do { \
        if(!((a) == (b))) { \
            std::cerr << "Assertion failed: " #a " == " #b " (actual: " << (a) \
                      << ", expected: " << (b) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_failures; \
        } \
    } while(0)

#define ASSERT_NEQ(a, b) \
    do { \
        if((a) == (b)) { \
            std::cerr << "Assertion failed: " #a " != " #b " (actual: " << (a) \
                      << ", not expected: " << (b) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_failures; \
        } \
    } while(0)

// Test 1: Ensure that passing NULL as the file returns UNZ_PARAMERROR
void test_unzSetOffset64_paramerror_on_null_file() {
    // Exercise the param validation branch
    int err = unzSetOffset64(nullptr, (ZPOS64_T)0);
    ASSERT_EQ(err, UNZ_PARAMERROR);
}

// Test 2: Verify side-effects and return behavior for a non-NULL file
//  - pos_in_central_dir should be updated to the provided pos
//  - num_file should be updated to gi.number_entry (hack behavior)
//  - current_file_ok should reflect whether the internal call succeeded (err == UNZ_OK)
void test_unzSetOffset64_updates_fields_for_non_null_file() {
    unz64_s s;

    // Initialize expected dependent fields
    s.gi.number_entry = 7;           // number of entries in central directory
    s.pos_in_central_dir = 0;        // will be overwritten
    s.num_file = 0;                  // will be overwritten to gi.number_entry
    s.current_file_ok = 0;           // will be updated based on internal err

    // Call with a representative 64-bit offset
    ZPOS64_T testPos = (ZPOS64_T)0x123456789ABCDEF0ULL;
    int err = unzSetOffset64((unzFile)&s, testPos);

    // Validate the observable state after the call
    ASSERT_EQ((uint64_t)s.pos_in_central_dir, (uint64_t)testPos);
    ASSERT_EQ((uint64_t)s.num_file, (uint64_t)s.gi.number_entry);
    bool expectedCurrentOk = (err == UNZ_OK);
    ASSERT_EQ((bool)s.current_file_ok, expectedCurrentOk);

    // Note: The exact value of err depends on unz64local_GetCurrentFileInfoInternal,
    // but the relation current_file_ok == (err == UNZ_OK) must hold.
}

// Test 3: Validate behavior with another large offset to ensure 64-bit handling
void test_unzSetOffset64_handles_another_large_offset() {
    unz64_s s;

    s.gi.number_entry = 3;
    s.pos_in_central_dir = 0;
    s.num_file = 0;
    s.current_file_ok = 0;

    ZPOS64_T testPos = (ZPOS64_T)0xFFFFFFFFFFFFFFFFULL; // max 64-bit value
    int err = unzSetOffset64((unzFile)&s, testPos);

    ASSERT_EQ((uint64_t)s.pos_in_central_dir, (uint64_t)testPos);
    ASSERT_EQ((uint64_t)s.num_file, (uint64_t)s.gi.number_entry);
    bool expectedCurrentOk = (err == UNZ_OK);
    ASSERT_EQ((bool)s.current_file_ok, expectedCurrentOk);
}

// Entry point for running tests
int main() {
    std::cout << "Running unit tests for unzSetOffset64...\n";

    test_unzSetOffset64_paramerror_on_null_file();
    test_unzSetOffset64_updates_fields_for_non_null_file();
    test_unzSetOffset64_handles_another_large_offset();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED\n";
        return g_failures;
    }
}