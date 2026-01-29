// UnityNumToPtr unit test suite (C++11, no GTest)
// File: unity_numtoPtr_tests.cpp

/*
Step 1 - Program Understanding (candidate keywords)
- UNITY_INTERNAL_PTR: return type of UnityNumToPtr
- UnityNumToPtr(num, size): converts an integer to a pointer to a sized storage
  - size 1 -> int8
  - size 2 -> int16
  - size 8 (if UNITY_SUPPORT_64) -> int64
  - default (4 bytes) -> int32
- UnityQuickCompare.i8, i16, i32, i64: internal storage fields written by UnityNumToPtr
- UNITY_INT, UNITY_UINT8: input and size types
- The function is implemented in unity.c and relies on UnityQuickCompare as a global storage
- Tests should validate that:
  - Correct underlying type is used for each size
  - Correct value is stored for each type
  - 8-byte path is included only when UNITY_SUPPORT_64 is defined
- We will use a lightweight, non-terminating assertion style to maximize coverage
- We will run tests from main() without GTest framework
*/

#include <stddef.h>
#include <iostream>
#include <cstdint>
#include <unity.h>


// Bring in Unity types/prototypes with C linkage
extern "C" {
}

// Declare the focal function (ensure C linkage)
extern "C" UNITY_INTERNAL_PTR UnityNumToPtr(const UNITY_INT num, const UNITY_UINT8 size);

// Simple non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Basic assertion helper: records failure but continues
#define TEST_CHECK(cond, msg) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "[FAIL] " << msg << " (File: " << __FILE__ << ", Line: " << __LINE__ << ")" << std::endl; \
    } else { \
        std::cout << "[PASS] " << msg << std::endl; \
    } \
} while (0)

// Test: size = 1 should store int8 value
void test_UnityNumToPtr_Size1_Int8()
{
    // Arrange
    const UNITY_INT test_num = 123; // fits into int8
    // Act
    UNITY_INTERNAL_PTR p = UnityNumToPtr(test_num, 1);
    // Assert
    int8_t value = *reinterpret_cast<int8_t*>(p);
    TEST_CHECK(value == static_cast<int8_t>(test_num),
        "UnityNumToPtr(123,1) -> int8 value equals 123");
}

// Test: size = 2 should store int16 value
void test_UnityNumToPtr_Size2_Int16()
{
    // Arrange
    const UNITY_INT test_num = 0x1234; // 4660
    // Act
    UNITY_INTERNAL_PTR p = UnityNumToPtr(test_num, 2);
    // Assert
    int16_t value = *reinterpret_cast<int16_t*>(p);
    TEST_CHECK(value == static_cast<int16_t>(test_num),
        "UnityNumToPtr(0x1234,2) -> int16 value equals 0x1234");
}

// Test: default size (4) should store int32 value
void test_UnityNumToPtr_Default_Int32()
{
    // Arrange
    const UNITY_INT test_num = 0x7FFFFFFF; // INT32_MAX
    // Act
    UNITY_INTERNAL_PTR p = UnityNumToPtr(test_num, 3); // any non-1/2/8 should default to 4 bytes
    // Assert
    int32_t value = *reinterpret_cast<int32_t*>(p);
    TEST_CHECK(value == static_cast<int32_t>(test_num),
        "UnityNumToPtr(default,3) -> int32 value equals 0x7FFFFFFF");
}

// Test: size = 8 should store int64 (only if UNITY_SUPPORT_64 is defined)
void test_UnityNumToPtr_Size8_Int64()
{
#ifdef UNITY_SUPPORT_64
    // Arrange
    const UNITY_INT test_num = 0x7FFFFFFF; // positive value, will be sign-extended to 64-bit as 0x000000007FFFFFFF
    // Act
    UNITY_INTERNAL_PTR p = UnityNumToPtr(test_num, 8);
    // Assert
    int64_t value = *reinterpret_cast<int64_t*>(p);
    TEST_CHECK(value == static_cast<int64_t>(test_num),
        "UnityNumToPtr(0x7FFFFFFF,8) -> int64 value equals 0x000000007FFFFFFF");
#else
    // If 64-bit support is not available, skip with a PASS message
    ++g_total_tests;
    std::cout << "[SKIP] UnityNumToPtr(8) not compiled with UNITY_SUPPORT_64" << std::endl;
#endif
}

// Test: ensure 3 (non-supported) size path still uses 4-byte int32
void test_UnityNumToPtr_Size3_Default_Int32()
{
    // Arrange
    const UNITY_INT test_num = -123456; // negative value, check sign handling
    // Act
    UNITY_INTERNAL_PTR p = UnityNumToPtr(test_num, 3); // not 1,2,8 so default to 4 bytes
    // Assert
    int32_t value = *reinterpret_cast<int32_t*>(p);
    TEST_CHECK(value == static_cast<int32_t>(test_num),
        "UnityNumToPtr(negative,3) -> int32 value equals input cast");
}

// Test: negative and edge cases for size 1 (int8)
void test_UnityNumToPtr_Size1_NegativeInt8()
{
    // Arrange
    const UNITY_INT test_num = -1; // 0xFF in int8
    // Act
    UNITY_INTERNAL_PTR p = UnityNumToPtr(test_num, 1);
    // Assert
    int8_t value = *reinterpret_cast<int8_t*>(p);
    TEST_CHECK(value == static_cast<int8_t>(test_num),
        "UnityNumToPtr(-1,1) -> int8 value equals -1 (0xFF)");
}

// Run all tests
void run_all_unity_num_to_ptr_tests()
{
    test_UnityNumToPtr_Size1_Int8();
    test_UnityNumToPtr_Size2_Int16();
    test_UnityNumToPtr_Default_Int32();
    test_UnityNumToPtr_Size8_Int64();
    test_UnityNumToPtr_Size3_Default_Int32();
    test_UnityNumToPtr_Size1_NegativeInt8();
}

// Main harness
int main()
{
    std::cout << "Running UnityNumToPtr unit tests (UnityNumToPtr(const UNITY_INT,int))" << std::endl;
    run_all_unity_num_to_ptr_tests();

    std::cout << "Tests completed. Total: " << g_total_tests << ", Failures: " << g_failed_tests << std::endl;
    // Return non-zero if any test failed
    return (g_failed_tests > 0) ? 1 : 0;
}