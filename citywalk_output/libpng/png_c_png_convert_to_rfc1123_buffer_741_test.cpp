// Minimal C++11 test suite for the focal C function:
// png_convert_to_rfc1123_buffer(char out[29], const void* ptime)
// The tests are designed to be self-contained, do not rely on GTest, and run from main().
// The tests use a light-weight, non-terminating assertion system to maximize code coverage.

#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Declaration of the focal function imported from C code.
// We declare the signature with a generic pointer type (const void*) to avoid
// dependency on the exact libpng internal type layout, while still calling into
// the real function provided by the C source under test.
extern "C" int png_convert_to_rfc1123_buffer(char out[29], const void* ptime);

// Lightweight test harness (non-terminating assertions).
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "[TEST FAILED] " << msg \
                      << " (in " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
            ++g_failures; \
        } \
    } while (0)

#define EXPECT_EQ(actual, expected, msg) \
    do { \
        if ((actual) != (expected)) { \
            std::cerr << "[TEST FAILED] " << msg \
                      << " | Expected: " << (expected) << "  Got: " << (actual) \
                      << " (in " << __FILE__ << ":" << __LINE__ << ")" \
                      << std::endl; \
            ++g_failures; \
        } \
    } while (0)

#define EXPECT_MEMEQ(buf1, buf2, len, msg) \
    do { \
        if (std::memcmp((buf1), (buf2), (len)) != 0) { \
            std::cerr << "[TEST FAILED] " << msg \
                      << " | Buffers differ (in " << __FILE__ << ":" << __LINE__ << ")" \
                      << std::endl; \
            ++g_failures; \
        } \
    } while (0)

// Simple time structure used to feed the focal function.
// We pass the address as const void* per the function's signature (via a generic pointer).
struct TestTime {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
};

// Test 1: Null out parameter should return 0 (false path)
void test_null_out() {
    TestTime t{2020, 11, 23, 7, 5, 9};
    int ret = png_convert_to_rfc1123_buffer(nullptr, (const void*)&t);
    EXPECT_EQ(ret, 0, "png_convert_to_rfc1123_buffer should return 0 when out is NULL");
}

// Test 2: Invalid year (> 9999) should return 0
void test_invalid_year() {
    TestTime t{10000, 11, 23, 7, 5, 9};
    char out[29] = {0};
    int ret = png_convert_to_rfc1123_buffer(out, (const void*)&t);
    EXPECT_EQ(ret, 0, "Invalid year > 9999 should cause return 0");
}

// Test 3: Invalid month (0) should return 0
void test_invalid_month_zero() {
    TestTime t{2020, 0, 23, 7, 5, 9};
    char out[29] = {0};
    int ret = png_convert_to_rfc1123_buffer(out, (const void*)&t);
    EXPECT_EQ(ret, 0, "Month 0 should cause return 0");
}

// Test 4: Invalid day (0) should return 0
void test_invalid_day_zero() {
    TestTime t{2020, 11, 0, 7, 5, 9};
    char out[29] = {0};
    int ret = png_convert_to_rfc1123_buffer(out, (const void*)&t);
    EXPECT_EQ(ret, 0, "Day 0 should cause return 0");
}

// Test 5: Invalid hour (24) should return 0
void test_invalid_hour() {
    TestTime t{2020, 11, 23, 24, 5, 9};
    char out[29] = {0};
    int ret = png_convert_to_rfc1123_buffer(out, (const void*)&t);
    EXPECT_EQ(ret, 0, "Hour >= 24 should cause return 0");
}

// Test 6: Invalid minute (60) should return 0
void test_invalid_minute() {
    TestTime t{2020, 11, 23, 7, 60, 9};
    char out[29] = {0};
    int ret = png_convert_to_rfc1123_buffer(out, (const void*)&t);
    EXPECT_EQ(ret, 0, "Minute > 59 should cause return 0");
}

// Test 7: Invalid second (61) should return 0
void test_invalid_second() {
    TestTime t{2020, 11, 23, 7, 5, 61};
    char out[29] = {0};
    int ret = png_convert_to_rfc1123_buffer(out, (const void*)&t);
    EXPECT_EQ(ret, 0, "Second > 60 should cause return 0");
}

// Test 8: Valid time formatting. Expect exact first 26 bytes corresponding to:
// "23 Nov 2020 07:05:09 +0000"
// Note: The function writes exactly 26 bytes for this input; the remaining 3 bytes in out[29]
// are not guaranteed to be initialized and should not be relied upon.
void test_valid_time_format() {
    TestTime t{2020, 11, 23, 7, 5, 9};
    char out[29] = {0};
    int ret = png_convert_to_rfc1123_buffer(out, (const void*)&t);
    EXPECT_EQ(ret, 1, "Valid time should return 1");

    const std::string expected = "23 Nov 2020 07:05:09 +0000"; // 26 chars
    // Compare only the first 26 bytes that constitute the formatted timestamp.
    EXPECT_MEMEQ(out, expected.data(), static_cast<size_t>(26),
                 "Formatted RFC1123 buffer does not match expected value");
}

// Run all tests and report results
int main() {
    std::cout << "Starting png_convert_to_rfc1123_buffer tests (C++11)..." << std::endl;

    test_null_out();
    test_invalid_year();
    test_invalid_month_zero();
    test_invalid_day_zero();
    test_invalid_hour();
    test_invalid_minute();
    test_invalid_second();
    test_valid_time_format();

    if (g_failures != 0) {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}