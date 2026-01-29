// Comprehensive unit tests for cJSON_IsRaw using a lightweight, non-GTest test harness.
// The tests are written in C++11 and rely on the public cJSON API.
// Each test includes explanatory comments describing its purpose and coverage.
// Note: The tests assume the presence of cJSON.h/cJSON.c in the build environment.

#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "[TEST FAILURE] " << __FILE__ << ":" << __LINE__ \
                  << " - Expected true for: " #cond << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_FALSE(cond) do { \
    if((cond)) { \
        std::cerr << "[TEST FAILURE] " << __FILE__ << ":" << __LINE__ \
                  << " - Expected false for: " #cond << std::endl; \
        ++g_failures; \
    } \
} while(0)

// Step 1 and Domain Knowledge notes (embedded as comments for clarity)
// - Candidate Keywords: item, cJSON_IsRaw, cJSON_Raw, item->type, NULL check
// - Behavior: return false if item is NULL; otherwise check (item->type & 0xFF) == cJSON_Raw
// - Coverage targets: NULL path (false), Raw path (true), non-Raw path (false), and bit-mask edge case where higher bits are set but lower bits indicate Raw

// Test 1: Null input should return false (verify the NULL branch)
void test_IsRaw_null_input_returns_false()
{
    // Purpose: Ensure that passing a NULL pointer does not dereference and returns false.
    // This directly exercises the early return path in cJSON_IsRaw.
    EXPECT_FALSE(cJSON_IsRaw(nullptr));
}

// Test 2: Raw object should return true (lower 8 bits indicate Raw)
void test_IsRaw_with_raw_object_returns_true()
{
    // Create a Raw JSON item using the public API and verify cJSON_IsRaw recognizes it.
    cJSON *raw = cJSON_CreateRaw("rawdata");
    EXPECT_TRUE(raw != nullptr); // ensure creation succeeded
    EXPECT_TRUE(cJSON_IsRaw(raw));
    cJSON_Delete(raw);
}

// Test 3: Non-Raw object (e.g., string) should return false
void test_IsRaw_with_string_object_returns_false()
{
    // Create a string item and verify it is not Raw.
    cJSON *str = cJSON_CreateString("hello");
    EXPECT_TRUE(str != nullptr);
    EXPECT_FALSE(cJSON_IsRaw(str));
    cJSON_Delete(str);
}

// Test 4: Manipulate the type field to simulate higher bits while lower bits indicate Raw
void test_IsRaw_with_upper_bits_preserving_raw_lower_bits()
{
    // Create a Null item and then artificially set its lower 8 bits to Raw.
    // This tests that the function only looks at the lower 8 bits (mask 0xFF).
    cJSON *item = cJSON_CreateNull();
    EXPECT_TRUE(item != nullptr);

    // Preserve any existing high bits, force lower 8 bits to Raw
    item->type = (item->type & ~0xFF) | cJSON_Raw;

    // Now the lower 8 bits indicate Raw despite being a Null-type object originally.
    EXPECT_TRUE(cJSON_IsRaw(item));

    cJSON_Delete(item);
}

// Step 3: Test case orchestration (main entry point for the test suite)
int main()
{
    std::cout << "Running tests for cJSON_IsRaw...\n";

    test_IsRaw_null_input_returns_false();
    test_IsRaw_with_raw_object_returns_true();
    test_IsRaw_with_string_object_returns_false();
    test_IsRaw_with_upper_bits_preserving_raw_lower_bits();

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed.\n";
        return 1;
    }
}