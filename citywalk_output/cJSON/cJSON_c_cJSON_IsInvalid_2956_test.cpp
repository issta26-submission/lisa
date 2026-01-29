// C++11 unit test suite for cJSON_IsInvalid (no GoogleTest, simple test harness)
// Purpose: verify behavior of cJSON_IsInvalid for NULL input, valid items, and manipulated low-byte type field.
// Notes:
// - Tests are designed to be non-terminating (they run all tests even if some fail).
// - Uses only C++ standard library and the provided cJSON API.
// - Test functions are invoked from main() as there is no external test framework.
// - Includes cJSON.h with extern "C" to ensure correct linkage from C++.

#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <limits.h>


// Bring in CJSON public API (C header with C linkage)
extern "C" {
}

//
// Lightweight test harness
//
static int g_total = 0;
static int g_failed = 0;

#define RUN_TEST(name) do { \
    bool ok = (name)(); \
    ++g_total; \
    if (!ok) { ++g_failed; } \
} while (0)

#define TEST_OK(cond, msg) do { \
    if (!(cond)) { \
        fprintf(stderr, "Test failure: %s\n", (msg)); \
        return false; \
    } \
} while (0)

//
// Test 1: NULL input should return false
//
bool test_IsInvalid_nullptr() {
    // when item is NULL, cJSON_IsInvalid should return false (per implementation)
    bool res = cJSON_IsInvalid(nullptr);
    TEST_OK(res == false, "cJSON_IsInvalid(NULL) must be false");
    return true;
}

//
// Test 2: Valid item (created via cJSON_CreateNull) should NOT be invalid
//
bool test_IsInvalid_on_valid_item() {
    cJSON *item = cJSON_CreateNull();
    TEST_OK(item != nullptr, "Failed to create valid cJSON item (null)");
    bool res = cJSON_IsInvalid((const cJSON *)item);
    TEST_OK(res == false, "cJSON_IsInvalid() must be false for a valid item");
    cJSON_Delete(item);
    return true;
}

//
// Test 3: Explicitly mark an item as invalid by setting its type to cJSON_Invalid
//
bool test_IsInvalid_explicit_invalid() {
    cJSON *item = cJSON_CreateNull();
    TEST_OK(item != nullptr, "Failed to create valid cJSON item for invalid test");
    // Force invalid by assigning the low byte to cJSON_Invalid
    item->type = (int)(unsigned int) cJSON_Invalid;
    bool res = cJSON_IsInvalid((const cJSON *)item);
    TEST_OK(res == true, "cJSON_IsInvalid() must be true when type low byte == cJSON_Invalid");
    cJSON_Delete(item);
    return true;
}

//
// Test 4: Low byte equals cJSON_Invalid due to additional high bits (e.g., 0x100 added)
// This ensures that masking with 0xFF works as intended.
//
bool test_IsInvalid_lowbyte_with_highbits() {
    cJSON *item = cJSON_CreateNull();
    TEST_OK(item != nullptr, "Failed to create valid cJSON item for high-bit test");
    unsigned int invalid_low = (unsigned int) cJSON_Invalid;
    item->type = (int)(invalid_low | 0x100); // high bit set, low byte remains invalid
    bool res = cJSON_IsInvalid((const cJSON *)item);
    TEST_OK(res == true, "cJSON_IsInvalid() must be true when low byte == cJSON_Invalid even with high bits");
    cJSON_Delete(item);
    return true;
}

//
// Test 5: Low byte not equal to cJSON_Invalid should be false
//
bool test_IsInvalid_lowbyte_not_invalid() {
    cJSON *item = cJSON_CreateNull();
    TEST_OK(item != nullptr, "Failed to create valid cJSON item for non-invalid low-byte test");
    // Choose a low byte value that is unlikely to be cJSON_Invalid. Use a non-zero, non-invalid value.
    // We do not know the exact value of cJSON_Invalid, so pick a value that is unlikely to equal it.
    item->type = 0xAB; // 171 decimal
    bool res = cJSON_IsInvalid((const cJSON *)item);
    TEST_OK(res == false, "cJSON_IsInvalid() must be false when low byte != cJSON_Invalid");
    cJSON_Delete(item);
    return true;
}

//
// Main: run all tests
//
int main() {
    // Run all tests; non-terminating (continues after failures)
    RUN_TEST(test_IsInvalid_nullptr);
    RUN_TEST(test_IsInvalid_on_valid_item);
    RUN_TEST(test_IsInvalid_explicit_invalid);
    RUN_TEST(test_IsInvalid_lowbyte_with_highbits);
    RUN_TEST(test_IsInvalid_lowbyte_not_invalid);

    // Summary
    printf("cJSON_IsInvalid tests: total=%d, failed=%d\n", g_total, g_failed);
    // Return non-zero if any test failed to aid integration with build systems
    return (g_failed == 0) ? 0 : 1;
}