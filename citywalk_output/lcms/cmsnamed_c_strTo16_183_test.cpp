// C++11 test suite for cmsUInt16Number strTo16(const char str[3])
// This test harness is a lightweight, non-GTest framework with non-terminating assertions.
// It targets the focal method inside cmsnamed.c and uses external linkage to CMS types.
// The tests are designed to be compiled and linked with the lcsm2_internal.h definitions.

#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <cstdint>
#include <sstream>


// Import necessary dependencies to obtain CMS type definitions.
// Adjust the include path as per your build setup.

// Declare the focal function with C linkage to ensure correct symbol resolution.
extern "C" cmsUInt16Number strTo16(const char str[3]);

// Lightweight test harness in a dedicated namespace
namespace CMSUnitTest {

static int totalTests = 0;
static int failedTests = 0;
static std::vector<std::string> logs;

// Helper to stringify values of generic types for assertion messages
template <typename T>
static std::string to_string_val(const T& v) {
    std::ostringstream oss;
    oss << v;
    return oss.str();
}

// Convenience: print summary of test results
static void printSummary() {
    std::cout << "Total tests: " << totalTests << ", Failures: " << failedTests << std::endl;
    for (const auto& m : logs) {
        std::cout << m << std::endl;
    }
}

// EXPECT_EQ: non-terminating equality assertion
#define EXPECT_EQ(a, b) do {                           \
    ++totalTests;                                       \
    auto _a = (a);                                      \
    auto _b = (b);                                      \
    if (!(_a == _b)) {                                   \
        ++failedTests;                                   \
        std::ostringstream oss;                          \
        oss << "EXPECT_EQ failed: " << #a                \
            << " != " << #b                               \
            << " (actual: " << to_string_val(_a)         \
            << " vs " << to_string_val(_b)                \
            << ") in " << __FILE__ << ":" << __LINE__;     \
        logs.push_back(oss.str());                       \
    }                                                    \
} while (0)

// Additional convenience assertions
#define EXPECT_TRUE(cond) do {                             \
    ++totalTests;                                             \
    if (!(cond)) {                                              \
        ++failedTests;                                         \
        std::ostringstream oss;                                \
        oss << "EXPECT_TRUE failed: " << #cond                \
            << " in " << __FILE__ << ":" << __LINE__;           \
        logs.push_back(oss.str());                              \
    }                                                           \
} while (0)

#define EXPECT_FALSE(cond) do {                            \
    ++totalTests;                                             \
    if (cond) {                                               \
        ++failedTests;                                         \
        std::ostringstream oss;                                \
        oss << "EXPECT_FALSE failed: " << #cond               \
            << " in " << __FILE__ << ":" << __LINE__;           \
        logs.push_back(oss.str());                              \
    }                                                           \
} while (0)

// Test 1: Null input should return 0 (non-existent strings branch)
static void test_strTo16_null_input() {
    // When str is NULL, function should return 0
    cmsUInt16Number v = strTo16(nullptr);
    EXPECT_EQ(v, (cmsUInt16Number)0);
}

// Test 2: Two-byte big-endian value with explicit bytes
// Input: {0x01, 0x02, 0x00} -> 0x0102
static void test_strTo16_two_bytes_basic() {
    unsigned char bytes[2] = {0x01, 0x02};
    cmsUInt16Number v = strTo16((const char*)bytes);
    EXPECT_EQ(v, (cmsUInt16Number)0x0102);
}

// Test 3: ASCII-based two-byte value to ensure correct high-byte/low-byte packing
// Input: {'A','B','\0'} -> 0x4142
static void test_strTo16_ascii_basic() {
    char bytes[3] = {'A', 'B', '\0'};
    cmsUInt16Number v = strTo16(bytes);
    EXPECT_EQ(v, (cmsUInt16Number)0x4142);
}

// Test 4: Third byte should be ignored; only first two bytes used
// Input: {0x01, 0x02, 0xFF} -> 0x0102
static void test_strTo16_ignores_third_byte() {
    unsigned char bytes[3] = {0x01, 0x02, 0xFF};
    cmsUInt16Number v = strTo16((const char*)bytes);
    EXPECT_EQ(v, (cmsUInt16Number)0x0102);
}

// Test 5: Maximum value using 0xFF, 0xFF -> 0xFFFF
static void test_strTo16_max_value() {
    unsigned char bytes[2] = {0xFF, 0xFF};
    cmsUInt16Number v = strTo16((const char*)bytes);
    EXPECT_EQ(v, (cmsUInt16Number)0xFFFF);
}

// Test 6: Alternate high-byte and low-byte to ensure endianness handling
// Input: {0x80, 0x01} -> 0x8001
static void test_strTo16_endianness_variant() {
    unsigned char bytes[2] = {0x80, 0x01};
    cmsUInt16Number v = strTo16((const char*)bytes);
    EXPECT_EQ(v, (cmsUInt16Number)0x8001);
}

// Test 7: Short input array (size 2) should still work correctly
// Input: {0x12, 0x34} -> 0x1234
static void test_strTo16_two_byte_minimal() {
    unsigned char bytes[2] = {0x12, 0x34};
    cmsUInt16Number v = strTo16((const char*)bytes);
    EXPECT_EQ(v, (cmsUInt16Number)0x1234);
}

// Run all tests
static void runAllTests() {
    test_strTo16_null_input();
    test_strTo16_two_bytes_basic();
    test_strTo16_ascii_basic();
    test_strTo16_ignores_third_byte();
    test_strTo16_max_value();
    test_strTo16_endianness_variant();
    test_strTo16_two_byte_minimal();

    printSummary();
}

} // namespace CMSUnitTest

// Entry point
int main() {
    // Execute the test suite
    CMSUnitTest::runAllTests();
    // Return non-zero if any test failed to indicate test suite failure
    return (CMSUnitTest::failedTests == 0) ? 0 : 1;
}