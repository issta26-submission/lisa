// Test suite for the focal method: encodeUTF8
// This test suite is written in C++11 (no GTest) and exercises the
// encodeUTF8 function assumed to be provided by the project (extern "C").
// The tests rely on the function signature:
//   unsigned int encodeUTF8(char* out, const wchar_t* in, unsigned int max_wchars, unsigned int max_chars);
//
// Notes:
// - We treat cmsUInt32Number as unsigned int for test purposes.
// - We purposefully avoid terminating asserts; instead we log failures and continue.
// - The tests cover ASCII, 2-byte, 3-byte, and 4-byte UTF-8 encodings, as well as
//   behavior when the output buffer is NULL or too small to write.

#include <lcms2_internal.h>
#include <iostream>
#include <cwchar>
#include <cstring>


extern "C" unsigned int encodeUTF8(char* out, const wchar_t* in, unsigned int max_wchars, unsigned int max_chars);

// Simple non-terminating test expectation helper
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void expect(bool condition, const char* message) {
    ++g_total_tests;
    if (!condition) {
        ++g_failed_tests;
        std::cerr << "TEST FAIL: " << message << std::endl;
    }
}

// Test 1: ASCII character should encode to a single byte when there is enough space.
// Input: L"A"; max_wchars = 1; max_chars = 3
// Expected: size = 1, out[0] == 'A'
static void test_ascii_single_byte() {
    wchar_t in[] = L"A";
    char out[4] = {0};
    unsigned int sz = encodeUTF8(out, in, 1, 3);
    expect(sz == 1, "ASCII single char should return size 1");
    expect(out[0] == 'A', "ASCII 'A' should be encoded as single byte 'A'");
}

// Test 2: When out == NULL, function should still return the number of bytes that would be written.
static void test_ascii_out_null() {
    wchar_t in[] = L"B";
    unsigned int sz = encodeUTF8(NULL, in, 1, 2);
    expect(sz == 1, "ASCII size should be 1 even if out is NULL");
}

// Test 3: 2-byte UTF-8 encoding for U+00E9 (é).
// Input: L"\u00E9"; max_wchars = 1; max_chars = 3
// Expected bytes: 0xC3 0xA9
static void test_two_byte_utf8() {
    wchar_t in[] = L"\u00E9"; // é
    char out[4] = {0};
    unsigned int sz = encodeUTF8(out, in, 1, 3);
    expect(sz == 2, "2-byte UTF-8 encoding should return size 2");
    expect((unsigned char)out[0] == 0xC3 && (unsigned char)out[1] == 0xA9,
           "2-byte UTF-8 for é should be C3 A9");
}

// Test 4: 3-byte UTF-8 encoding for U+20AC (€).
// Input: L"\u20AC"; max_wchars = 1; max_chars = 4
// Expected bytes: 0xE2 0x82 0xAC
static void test_three_byte_utf8() {
    wchar_t in[] = L"\u20AC"; // €
    char out[4] = {0};
    unsigned int sz = encodeUTF8(out, in, 1, 4);
    expect(sz == 3, "3-byte UTF-8 encoding should return size 3");
    expect((unsigned char)out[0] == 0xE2 &&
           (unsigned char)out[1] == 0x82 &&
           (unsigned char)out[2] == 0xAC,
           "3-byte UTF-8 for € should be E2 82 AC");
}

// Test 5: 4-byte UTF-8 encoding for U+1F600 (grinning face emoji).
// Input: wide 0x1F600; max_wchars = 1; max_chars = 5
// Expected bytes: 0xF0 0x9F 0x98 0x80
static void test_four_byte_utf8() {
    wchar_t in[] = {static_cast<wchar_t>(0x1F600), 0}; // 😀
    char out[6] = {0};
    unsigned int sz = encodeUTF8(out, in, 1, 5);
    expect(sz == 4, "4-byte UTF-8 encoding should return size 4");
    expect((unsigned char)out[0] == 0xF0 &&
           (unsigned char)out[1] == 0x9F &&
           (unsigned char)out[2] == 0x98 &&
           (unsigned char)out[3] == 0x80,
           "4-byte UTF-8 for U+1F600 should be F0 9F 98 80");
}

// Test 6: Multiple characters with a limit on max_wchars (only first N chars are processed).
// Input: L"ABCD"; max_wchars = 2; max_chars large enough
// Expected: size == 2 and first two bytes correspond to 'A' and 'B'
static void test_multiple_chars_limit() {
    wchar_t in[] = L"ABCD";
    char out[6] = {0};
    unsigned int sz = encodeUTF8(out, in, 2, 6);
    expect(sz == 2, "Only first two wide chars should be processed");
    expect(out[0] == 'A' && out[1] == 'B', "First two bytes should be 'A' and 'B'");
}

// Test 7: Boundary case where max_chars prevents any writing (size remains but nothing is written).
// Input: L"A"; max_wchars = 1; max_chars = 1
// Expected: size == 1, and out is not modified
static void test_no_write_due_to_buffer_limit() {
    wchar_t in[] = L"A";
    char out[4] = {'X', 'X', 'X', 'X'};
    unsigned int sz = encodeUTF8(out, in, 1, 1);
    expect(sz == 1, "Size should be 1 even if no byte is written");
    expect(out[0] == 'X' && out[1] == 'X', "Buffer should remain unchanged due to limit");
}

// Run all tests and print a summary
static void runAllTests() {
    test_ascii_single_byte();
    test_ascii_out_null();
    test_two_byte_utf8();
    test_three_byte_utf8();
    test_four_byte_utf8();
    test_multiple_chars_limit();
    test_no_write_due_to_buffer_limit();

    std::cout << "Total tests: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;

    // Exit status: 0 if all tests passed, 1 otherwise
    if (g_failed_tests > 0) {
        std::exit(1);
    }
}

// The main entry point that executes the test suite.
int main() {
    runAllTests();
    return 0;
}