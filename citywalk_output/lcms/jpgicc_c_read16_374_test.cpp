// Lightweight unit test suite for the FOCAL_METHOD: read16 in jpgicc.c
// Notes according to the provided domain knowledge:
// - This test suite uses only the C++ standard library (no GTest).
// - Tests exercise true/false branches of the conditional and both swap paths.
// - Tests call the C function read16 via extern "C" to avoid name mangling.
// - craftedFile() is part of the same translation unit in the project; here we rely on its side effects being none (returns 0 when bounds are violated).
// - Static handling: there are no static members in this focal function; tests simply call the function directly.

#include <cstring>
#include <iccjpeg.h>
#include <cstdio>
#include <jpeglib.h>
#include <cstddef>
#include <utils.h>
#include <cstdint>


// Declare the focal C function for linking with C++ tests.
extern "C" uint16_t read16(uint8_t* arr, size_t pos, int swapBytes, size_t max);

// Simple non-terminating assertion utility.
// If an assertion fails, it prints a message but does not abort tests to maximize coverage.
#define EXPECT_EQ(expected, actual) do { \
    if (static_cast<uint64_t>(static_cast<uint16_t>(expected)) != static_cast<uint64_t>(static_cast<uint16_t>(actual))) { \
        printf("TEST_FAIL: Expected %u, got %u at %s:%d\n", static_cast<unsigned>(static_cast<uint16_t>(expected)), \
               static_cast<unsigned>(static_cast<uint16_t>(actual)), __FILE__, __LINE__); \
        failures++; \
    } \
    else { \
        printf("TEST_PASS: %s == %s\n", #expected, #actual); \
    } \
} while(0)

// Helper macro for test naming (prints the test name).
#define TEST_NAME(name) printf("Running " #name "...\n");

// Run all tests; returns number of failed checks.
static int run_all_read16_tests() {
    int failures = 0;

    // Test 1: No swap, simple two-byte sequence from start of array.
    // Expect: (b1 << 8) | b2
    TEST_NAME(Test_Read16_NoSwap_Beginning);
    {
        uint8_t arr[] = {0x12, 0x34, 0x56, 0x78};
        size_t pos = 0;
        int swapBytes = 0;
        size_t max = sizeof(arr);
        uint16_t result = read16(arr, pos, swapBytes, max);
        EXPECT_EQ(0x1234, result);
    }

    // Test 2: With swapBytes = 1, two-byte sequence from start of array.
    // Expect: (b2 << 8) | b1
    TEST_NAME(Test_Read16_WithSwapAtBeginning);
    {
        uint8_t arr[] = {0x12, 0x34, 0x56, 0x78};
        size_t pos = 0;
        int swapBytes = 1;
        size_t max = sizeof(arr);
        uint16_t result = read16(arr, pos, swapBytes, max);
        EXPECT_EQ(0x3412, result);
    }

    // Test 3: Non-zero position, no swap; ensure proper byte order from mid-array.
    // Expect: (b1 << 8) | b2 where b1=arr[1], b2=arr[2]
    TEST_NAME(Test_Read16_NoSwap_Pos1);
    {
        uint8_t arr[] = {0xAA, 0x01, 0x02, 0x03};
        size_t pos = 1;
        int swapBytes = 0;
        size_t max = sizeof(arr);
        uint16_t result = read16(arr, pos, swapBytes, max);
        EXPECT_EQ(0x0102, result);
    }

    // Test 4: Non-zero position with swap; ensure swapped result from mid-array.
    // Expect: (b2 << 8) | b1 where b1=arr[1], b2=arr[2]
    TEST_NAME(Test_Read16_WithSwap_Pos1);
    {
        uint8_t arr[] = {0xAA, 0x01, 0x02, 0x03};
        size_t pos = 1;
        int swapBytes = 1;
        size_t max = sizeof(arr);
        uint16_t result = read16(arr, pos, swapBytes, max);
        EXPECT_EQ(0x0201, result);
    }

    // Test 5: Crafted path (bounds check): pos + 2 >= max triggers craftedFile() and returns 0.
    // Here pos=1, max=3 => 1+2 >= 3 is true.
    TEST_NAME(Test_Read16_CraftedPath);
    {
        uint8_t arr[] = {0xDE, 0xAD, 0xBE};
        size_t pos = 1;
        int swapBytes = 0;
        size_t max = 3; // 1 + 2 >= 3 -> craftedFile() executed, expect 0
        uint16_t result = read16(arr, pos, swapBytes, max);
        EXPECT_EQ(0, result);
    }

    // Test 6: Edge case where max bounds out the read (pos=0, max=2)
    // Expect: craftedFile() path, return 0.
    TEST_NAME(Test_Read16_EdgeBounds);
    {
        uint8_t arr[] = {0x12, 0x34};
        size_t pos = 0;
        int swapBytes = 0;
        size_t max = 2; // 0 + 2 >= 2 -> craftedFile() executed, expect 0
        uint16_t result = read16(arr, pos, swapBytes, max);
        EXPECT_EQ(0, result);
    }

    return failures;
}

int main() {
    printf("Starting read16 unit test suite (C/C++11 compatible, GTest-free)...\n");
    int failures = run_all_read16_tests();
    if (failures == 0) {
        printf("ALL TESTS PASSED\n");
    } else {
        printf("TOTAL FAILURES: %d\n", failures);
    }
    return failures;
}