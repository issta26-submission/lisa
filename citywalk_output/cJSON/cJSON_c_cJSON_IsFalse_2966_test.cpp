/*
  Candidate Keywords (from Step 1): 
  - cJSON_IsFalse, cJSON_False, item->type, NULL, cJSON_CreateFalse, cJSON_CreateTrue, cJSON_Delete
  - (item->type & 0xFF) comparison, return value (cJSON_bool)
  - NULL pointer handling, boolean truthiness, memory management for test scaffolding
  - Minimal test harness: non-terminating assertions, test runner, simple output
  - C++11 compatibility, include/call patterns for C library functions from C++
*/

/* Unit tests for cJSON_IsFalse in cJSON.c
   - No GTest: implement a lightweight, non-terminating assertion framework
   - Tests cover: NULL input, explicit false item, explicit true item, and synthetic items
     with type field manipulated to exercise the bit-masking logic (lower 8 bits)
*/

#include <locale.h>
#include <math.h>
#include <cstring>
#include <cJSON.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <limits.h>



// Lightweight non-terminating assertion helpers within an anonymous namespace
namespace {

int g_failures = 0;

// Log a non-fatal test failure with location info
inline void log_failure(const char* file, int line, const char* expr) {
    fprintf(stderr, "Test failure at %s:%d: %s\n", file, line, expr);
    ++g_failures;
}

// EXPECT_TRUE / EXPECT_FALSE macros emulate non-terminating assertions
#define EXPECT_TRUE(cond) do { if(!(cond)) log_failure(__FILE__, __LINE__, #cond); } while(0)
#define EXPECT_FALSE(cond) do { if((cond)) log_failure(__FILE__, __LINE__, "!(" #cond ")"); } while(0)
#define EXPECT_NOT_NULL(ptr) do { if((ptr) == NULL) log_failure(__FILE__, __LINE__, #ptr " != NULL"); } while(0)
#define EXPECT_NULL(ptr)     do { if((ptr) != NULL) log_failure(__FILE__, __LINE__, #ptr " == NULL"); } while(0)

} // namespace

// Test 1: NULL pointer input should yield false
// This ensures the NULL guard path is exercised.
static void test_IsFalse_NullPointer() {
    // Act
    cJSON_bool res = cJSON_IsFalse(NULL);
    // Assert: false when input is NULL
    EXPECT_FALSE(res);
}

// Test 2: Explicit cJSON_CreateFalse() should yield true
// Verifies the true negative condition for the IsFalse predicate.
static void test_IsFalse_ExplicitFalse() {
    cJSON *item = cJSON_CreateFalse();
    EXPECT_NOT_NULL(item);
    int res = cJSON_IsFalse(item);
    // The function returns non-zero (true) for false-type items
    EXPECT_TRUE(res != 0);
    cJSON_Delete(item);
}

// Test 3: Explicit cJSON_CreateTrue() should yield false
// Ensures the predicate does not falsely identify a true item as false.
static void test_IsFalse_ExplicitTrue() {
    cJSON *item = cJSON_CreateTrue();
    EXPECT_NOT_NULL(item);
    int res = cJSON_IsFalse(item);
    // Should be false for a true-type item
    EXPECT_FALSE(res != 0);
    cJSON_Delete(item);
}

// Test 4: Synthetic item with lower 8 bits of type == 0 (like cJSON_False)
// This tests the masking behavior without relying on public creators.
static void test_IsFalse_SyntheticZeroLower8Bits() {
    cJSON *synthetic = (cJSON*)malloc(sizeof(cJSON));
    if (synthetic == NULL) {
        log_failure(__FILE__, __LINE__, "malloc failed for synthetic item");
        return;
    }
    synthetic->type = 0; // lower 8 bits == 0, which equals cJSON_False
    int res = cJSON_IsFalse(synthetic);
    EXPECT_TRUE(res != 0);
    free(synthetic);
}

// Test 5: Synthetic item with lower 8 bits == 0 via 0x100 mask (0x100 & 0xFF == 0)
// Verifies that higher bits do not affect the masking logic.
static void test_IsFalse_SyntheticZeroLower8Bits_ViaMask() {
    cJSON *synthetic = (cJSON*)malloc(sizeof(cJSON));
    if (synthetic == NULL) {
        log_failure(__FILE__, __LINE__, "malloc failed for synthetic item (mask case)");
        return;
    }
    synthetic->type = 0x100; // lower 8 bits == 0
    int res = cJSON_IsFalse(synthetic);
    EXPECT_TRUE(res != 0);
    free(synthetic);
}

// Runner to execute all tests and report summary
static void RunAll_IsFalse_Tests() {
    test_IsFalse_NullPointer();
    test_IsFalse_ExplicitFalse();
    test_IsFalse_ExplicitTrue();
    test_IsFalse_SyntheticZeroLower8Bits();
    test_IsFalse_SyntheticZeroLower8Bits_ViaMask();

    int total = 5;
    printf("cJSON_IsFalse tests completed. Total: %d, Failures: %d\n", total, g_failures);
}

// Main entry point for the test executable
int main() {
    RunAll_IsFalse_Tests();
    // Return non-zero if any test failed
    return (g_failures == 0) ? 0 : 1;
}