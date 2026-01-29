// Unit test suite for the focal method: decodeUTF8 in cmsnamed.c
// This test suite is implemented in C++11 without GoogleTest.
// It uses C linkage to call the focal C function and covers various UTF-8 scenarios,
// including cross-platform wchar_t width differences.

#include <cstring>
#include <iostream>
#include <iomanip>
#include <cwchar>
#include <lcms2_internal.h>


// Prototype matching the focal C function. We assume extern "C" linkage.
extern "C" unsigned int decodeUTF8(wchar_t* out, const char* in);

static int tests_run = 0;
static int tests_passed = 0;

// Simple assertion helpers that continue on failure (non-terminating).
#define ASSERT_EQ(expected, actual, msg) do { \
    if ((static_cast<unsigned long long>(actual)) != (static_cast<unsigned long long>(expected))) { \
        std::cerr << "ASSERT FAILED: " << msg << " | expected: " << std::dec << (expected) \
                  << ", actual: " << (actual) << std::endl; \
        return false; \
    } \
} while(0)

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "ASSERT FAILED: " << msg << std::endl; \
        return false; \
    } \
} while(0)

#define RUN_TEST(name) do { \
    ++tests_run; \
    if (name()) { \
        ++tests_passed; \
        std::cout << "[PASS] " << #name << std::endl; \
    } else { \
        std::cout << "[FAIL] " << #name << std::endl; \
    } \
} while(0)

// Test 1: ASCII single character with output buffer
bool test_ascii_single_with_output()
{
    wchar_t out[4];
    std::memset(out, 0, sizeof(out));

    const char* in = "A"; // 0x41
    unsigned int size = decodeUTF8(out, in);

    // ASCII should produce a single code unit equal to 'A'
    ASSERT_EQ(1u, size, "ASCII input should produce size 1");
    ASSERT_TRUE(out[0] == L'A', "ASCII output should be character 'A'");

    return true;
}

// Test 1b: ASCII with null output buffer (out = nullptr) should still return size
bool test_ascii_with_null_output()
{
    const char* in = "A";
    unsigned int size = decodeUTF8(nullptr, in);

    ASSERT_EQ(1u, size, "ASCII input with null output should still return size 1");
    return true;
}

// Test 2: Two-byte UTF-8 sequence for U+00A9 (Copyright Sign)
bool test_two_byte_sequence()
{
    wchar_t out[4];
    std::memset(out, 0, sizeof(out));

    // UTF-8 for U+00A9 is C2 A9
    const char in[] = "\xC2\xA9";

    unsigned int size = decodeUTF8(out, in);

    ASSERT_EQ(1u, size, "Two-byte sequence should produce size 1");
    ASSERT_TRUE(out[0] == L'\u00A9', "Decoded codepoint should be U+00A9");
    return true;
}

// Test 3: Three-byte UTF-8 sequence for U+20AC (Euro Sign)
bool test_three_byte_sequence()
{
    wchar_t out[4];
    std::memset(out, 0, sizeof(out));

    // UTF-8 for U+20AC is E2 82 AC
    const char in[] = "\xE2\x82\xAC";

    unsigned int size = decodeUTF8(out, in);

    ASSERT_EQ(1u, size, "Three-byte sequence should produce size 1");
    ASSERT_TRUE(out[0] == L'\u20AC', "Decoded codepoint should be U+20AC (Euro Sign)");
    return true;
}

// Test 4: Four-byte UTF-8 sequence for U+1F600 (Grinning Face)
// Behavior depends on wchar_t width.
// - If wchar_t is wider than 2 bytes (common on Linux), should emit single code unit 0x1F600.
// - If wchar_t is 2 bytes (Windows), should emit a surrogate pair (size 2).
bool test_four_byte_sequence()
{
    wchar_t out[4];
    std::memset(out, 0, sizeof(out));

    // UTF-8 for U+1F600 is F0 9F 98 80
    const char in[] = "\xF0\x9F\x98\x80";

    unsigned int size = decodeUTF8(out, in);

    if (sizeof(wchar_t) > 2) {
        ASSERT_EQ(1u, size, "Four-byte sequence with wide wchar_t should produce size 1");
        ASSERT_TRUE(out[0] == 0x1F600, "Decoded codepoint should be U+1F600 (on wide wchar_t)");
    } else {
        // Expect a surrogate pair
        ASSERT_EQ(2u, size, "Four-byte sequence with 16-bit wchar_t should produce surrogate pair (size 2)");
        // Basic sanity: first surrogate in high range, second in low surrogate range
        bool high_ok = (out[0] >= 0xD800) && (out[0] <= 0xDBFF);
        bool low_ok  = (out[1] >= 0xDC00) && (out[1] <= 0xDFFF);
        ASSERT_TRUE(high_ok, "High surrogate should be in 0xD800..0xDBFF");
        ASSERT_TRUE(low_ok,  "Low surrogate should be in 0xDC00..0xDFFF");
    }

    return true;
}

// Test 5: Multiple codepoints in a single string: 'A' followed by Euro Sign
bool test_multiple_codepoints()
{
    wchar_t out[4];
    std::memset(out, 0, sizeof(out));

    // "A" (0x41) + "€" (U+20AC)
    const char in[] = "\x41\xE2\x82\xAC";

    unsigned int size = decodeUTF8(out, in);

    if (sizeof(wchar_t) > 2) {
        ASSERT_EQ(2u, size, "Two codepoints should yield size 2 on wide wchar_t");
        ASSERT_TRUE(out[0] == L'A', "First codepoint should be 'A'");
        ASSERT_TRUE(out[1] == L'\u20AC', "Second codepoint should be U+20AC");
    } else {
        // 16-bit wchar_t: still two codepoints, but euro fits in one wchar_t
        ASSERT_EQ(2u, size, "Two codepoints should yield size 2 on narrow wchar_t (euros)");
        ASSERT_TRUE(out[0] == L'A', "First codepoint should be 'A'");
        ASSERT_TRUE(out[1] == L'\u20AC', "Second codepoint should be U+20AC");
    }

    return true;
}

// Test 6: Ensure that providing a non-null input with null output and multiple codepoints works as expected
bool test_multiple_with_null_output()
{
    // "A" + "€" in sequence
    const char in[] = "\x41\xE2\x82\xAC";

    unsigned int size = decodeUTF8(nullptr, in);

    // Should return the number of resulting wide characters without writing them
    if (sizeof(wchar_t) > 2) {
        ASSERT_EQ(2u, size, "Expected size 2 for two codepoints with null output on wide wchar_t");
    } else {
        ASSERT_EQ(2u, size, "Expected size 2 for two codepoints with null output on narrow wchar_t");
    }

    return true;
}

int main()
{
    std::cout << "Starting decodeUTF8 test suite (C++11, no GTest)" << std::endl;

    RUN_TEST(test_ascii_single_with_output);
    RUN_TEST(test_ascii_with_null_output);
    RUN_TEST(test_two_byte_sequence);
    RUN_TEST(test_three_byte_sequence);
    RUN_TEST(test_four_byte_sequence);
    RUN_TEST(test_multiple_codepoints);
    RUN_TEST(test_multiple_with_null_output);

    std::cout << "\nTests run: " << tests_run << ", Passed: " << tests_passed
              << ", Failed: " << (tests_run - tests_passed) << std::endl;

    return (tests_run == tests_passed) ? 0 : 1;
}