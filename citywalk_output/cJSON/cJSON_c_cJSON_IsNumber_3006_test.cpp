#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <limits.h>


extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

#define TEST_EXPECT_TRUE(cond, desc) do { \
    ++g_total; \
    if(!(cond)) { \
        ++g_failed; \
        std::cerr << "FAIL: " << desc << " (in " << __FILE__ << ":" << __LINE__ << ")\n"; \
    } \
} while(0)

#define TEST_EXPECT_FALSE(cond, desc) do { \
    ++g_total; \
    if((cond)) { \
        ++g_failed; \
        std::cerr << "FAIL: " << desc << " (in " << __FILE__ << ":" << __LINE__ << ")\n"; \
    } \
} while(0)

#define TEST_EXPECT_EQ(a, b, desc) do { \
    ++g_total; \
    if(!((a) == (b))) { \
        ++g_failed; \
        std::cerr << "FAIL: " << desc << " (expected: " << (b) << ", actual: " << (a) << ") (in " << __FILE__ << ":" << __LINE__ << ")\n"; \
    } \
} while(0)

static void test_IsNumber_WithNullPointer_ReturnsFalse() {
    // When item is NULL, cJSON_IsNumber should return false
    TEST_EXPECT_FALSE(cJSON_IsNumber(nullptr), "cJSON_IsNumber(NULL) must be false");
}

static void test_IsNumber_NumberType_ReturnsTrue() {
    // Create a number item and verify IsNumber returns true
    cJSON *num = cJSON_CreateNumber(3.14159);
    TEST_EXPECT_TRUE(cJSON_IsNumber(num), "cJSON_IsNumber should return true for a number item");
    cJSON_Delete(num);
}

static void test_IsNumber_StringType_ReturnsFalse() {
    // Create a string item and verify IsNumber returns false
    cJSON *str = cJSON_CreateString("hello");
    TEST_EXPECT_FALSE(cJSON_IsNumber(str), "cJSON_IsNumber should return false for a string item");
    cJSON_Delete(str);
}

static void test_IsNumber_Number_With_HighBits_ReturnsTrue() {
    // Even if high bits are set in type, as long as low byte is Number, it should be true
    cJSON *num = cJSON_CreateNumber(1.23);
    // Simulate extra flags in higher bits
    num->type |= 0x100;
    TEST_EXPECT_TRUE(cJSON_IsNumber(num), "cJSON_IsNumber must be true when high bits are set but low byte equals Number");
    cJSON_Delete(num);
}

static void test_IsNumber_String_With_HighBits_ReturnsFalse() {
    // String item with extra high bits should still be non-number
    cJSON *str = cJSON_CreateString("world");
    str->type |= 0x100;
    TEST_EXPECT_FALSE(cJSON_IsNumber(str), "cJSON_IsNumber must be false when low byte is String even if high bits are set");
    cJSON_Delete(str);
}

int main() {
    // Run tests
    test_IsNumber_WithNullPointer_ReturnsFalse();
    test_IsNumber_NumberType_ReturnsTrue();
    test_IsNumber_StringType_ReturnsFalse();
    test_IsNumber_Number_With_HighBits_ReturnsTrue();
    test_IsNumber_String_With_HighBits_ReturnsFalse();

    // Summary
    std::cout << "Tests run: " << g_total << ", Failures: " << g_failed << std::endl;
    // Non-terminating: return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}