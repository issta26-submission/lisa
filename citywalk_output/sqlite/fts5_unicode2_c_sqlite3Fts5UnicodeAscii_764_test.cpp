#include <assert.h>
#include <cstring>
#include <cstdint>
#include <iostream>


// Forward declaration of the focal C function under test.
// It is defined in fts5_unicode2.c and uses u8 (unsigned char) in the original code.
// We map it to standard C++ types for the test harness.
extern "C" void sqlite3Fts5UnicodeAscii(uint8_t *aArray, uint8_t *aAscii);

// Simple non-terminating test assertion macro.
// It reports failures but does not abort execution, allowing multiple tests to run.
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) \
                  << " [" << __FILE__ << ":" << __LINE__ << "]\n"; \
        ++g_failures; \
    } \
} while(0)

// Test 1: Zero input array.
// Rationale:
// - When aArray is all zeros, the token value bToken computed from aArray[...] will be zero.
// - The function should set aAscii[0] to 0 explicitly at the end.
// - This test validates the basic invariant aAscii[0] == 0 regardless of internal writes.
static bool test_ascii_zero_input() {
    uint8_t aArray[128];
    std::memset(aArray, 0, sizeof(aArray));

    uint8_t aAscii[128];
    std::memset(aAscii, 0, sizeof(aAscii)); // initialize to known state

    sqlite3Fts5UnicodeAscii(aArray, aAscii);

    // Invariant: aAscii[0] must be 0 according to the implementation.
    EXPECT_TRUE(aAscii[0] == 0, "aAscii[0] should be 0 after zero input");

    // Do not rely on other indices to be initialized; only verify safety of 0th element.
    return g_failures == 0;
}

// Test 2: Non-zero, uniform token value.
// Rationale:
// - Set the first 32 entries of aArray to a non-zero value (0x01).
// - bToken will be 0x01 for any accessed index (due to aFts5UnicodeData[...] & 0x1F mapping).
// - The test asserts that any non-zero entries in aAscii are exactly 0x01 and that at least one
//   index is written (i.e., some aAscii[i] != 0), ensuring writes occurred and produced the
//   expected token value.
static bool test_ascii_uniform_token_value_one() {
    const int AARRAY_SIZE = 32;
    uint8_t aArray[AARRAY_SIZE];
    std::memset(aArray, 0x01, sizeof(aArray)); // all 0x01

    uint8_t aAscii[128];
    std::memset(aAscii, 0, sizeof(aAscii));

    sqlite3Fts5UnicodeAscii(aArray, aAscii);

    // aAscii[0] must be 0
    EXPECT_TRUE(aAscii[0] == 0, "aAscii[0] should be 0 with uniform 0x01 token input");

    // Check that any non-zero entries are exactly 0x01
    bool hasNonzero = false;
    for(int i = 0; i < 128; ++i){
        if(aAscii[i] != 0){
            hasNonzero = true;
            EXPECT_TRUE(aAscii[i] == 0x01, "Non-zero aAscii entry should be exactly 0x01 for uniform 0x01 input");
        }
    }

    // At least one index should have been written to (non-zero)
    EXPECT_TRUE(hasNonzero, "Expected at least one aAscii entry to be non-zero for uniform 0x01 input");

    return g_failures == 0;
}

// Test 3: Non-zero, uniform token value (0xAA).
// Rationale:
// - Similar to Test 2 but with a different non-zero token to ensure the function preserves the token value.
// - All non-zero outputs should be exactly 0xAA.
// - This confirms that the function does not alter the token value across writes when the input token is fixed.
static bool test_ascii_uniform_token_value_aa() {
    const int AARRAY_SIZE = 32;
    uint8_t aArray[AARRAY_SIZE];
    std::memset(aArray, 0xAA, sizeof(aArray)); // all 0xAA

    uint8_t aAscii[128];
    std::memset(aAscii, 0, sizeof(aAscii));

    sqlite3Fts5UnicodeAscii(aArray, aAscii);

    // aAscii[0] must be 0
    EXPECT_TRUE(aAscii[0] == 0, "aAscii[0] should be 0 with uniform 0xAA token input");

    // Check that any non-zero entries are exactly 0xAA
    bool hasNonzero = false;
    for(int i = 0; i < 128; ++i){
        if(aAscii[i] != 0){
            hasNonzero = true;
            EXPECT_TRUE(aAscii[i] == 0xAA, "Non-zero aAscii entry should be exactly 0xAA for uniform 0xAA input");
        }
    }

    // At least one index should have been written to (non-zero)
    EXPECT_TRUE(hasNonzero, "Expected at least one aAscii entry to be non-zero for uniform 0xAA input");

    return g_failures == 0;
}

int main() {
    // Run the tests in a lightweight, non-GTest manner.
    std::cout << "Running sqlite3Fts5UnicodeAscii unit tests (C++11 harness)..." << std::endl;

    // Test 1: Zero input
    test_ascii_zero_input();

    // Test 2: Uniform token 0x01
    test_ascii_uniform_token_value_one();

    // Test 3: Uniform token 0xAA
    test_ascii_uniform_token_value_aa();

    // Summary
    if(g_failures == 0){
        std::cout << "All tests passed." << std::endl;
        return 0;
    }else{
        std::cerr << "Total failures: " << g_failures << std::endl;
        return 1;
    }
}