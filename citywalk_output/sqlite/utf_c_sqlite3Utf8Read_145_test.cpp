/*
  Self-contained unit test suite for sqlite3Utf8Read (C++11, no GTest).

  Notes:
  - We implement a minimal, self-contained version of sqlite3Utf8Read that mirrors
    the focal method exactly as provided, including its use of sqlite3Utf8Trans1.
  - We provide a minimal global table sqlite3Utf8Trans1 and an initialization routine.
  - We create multiple test cases to cover:
      * ASCII path (c < 0xC0) -> first branch not taken
      * 2-byte sequence with a valid continuation -> yields a valid code point
      * 2-byte sequence that becomes invalid (forces 0xFFFD)
      * 3-byte sequence path through continuation bytes
  - Tests are executed from main() with simple, non-terminating assertions:
    they report PASS/FAIL messages and continue execution to maximize coverage.
  - This file is standalone and does not rely on GTest; it uses standard C++11.
*/

#include <assert.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Type alias to match the focal API
using u32 = unsigned int;

// Global translation table used by sqlite3Utf8Read
static unsigned int sqlite3Utf8Trans1[0x40];

// Initialize the translation table with safe default values
//  - By default, index 0 -> 0x80 (to allow a simple 2-byte sequence in tests)
//  - index 0x20 -> 0x40 (to enable a 3-byte sequence test)
static void init_utf8_trans1() {
    for(int i = 0; i < 0x40; ++i) sqlite3Utf8Trans1[i] = 0;
    sqlite3Utf8Trans1[0] = 0x80;
    sqlite3Utf8Trans1[0x20] = 0x40;
}

// The focal function under test, implemented exactly as described
u32 sqlite3Utf8Read(const unsigned char **pz) {
    unsigned int c;
    /* Same as READ_UTF8() above but without the zTerm parameter.
       For this routine, we assume the UTF8 string is always zero-terminated. */
    c = *((*pz)++);
    if( c>=0xc0 ){
        c = sqlite3Utf8Trans1[c-0xc0];
        while( (*(*pz) & 0xc0)==0x80 ){
            c = (c<<6) + (0x3f & *((*pz)++));
        }
        if( c<0x80
            || (c&0xFFFFF800)==0xD800
            || (c&0xFFFFFFFE)==0xFFFE ){  c = 0xFFFD; }
    }
    return c;
}

// Simple test harness (non-terminating assertions)
static int g_failures = 0;
#define EXPECT_EQ(expected, actual, msg) \
    do { \
        if((expected) != (actual)) { \
            std::cerr << "FAIL: " << msg << " | expected: " << (expected) \
                      << " actual: " << (actual) << "\n"; \
            ++g_failures; \
        } else { \
            std::cout << "PASS: " << msg << "\n"; \
        } \
    } while(0)

#define EXPECT_LT(a,b,msg) \
    do { if (!((a) < (b))) { std::cerr << "FAIL: " << msg << " | " \
        << (a) << " !< " << (b) << "\n"; ++g_failures; } else { \
        std::cout << "PASS: " << msg << "\n"; }} while(0)

// Test 1: ASCII path (first byte < 0xC0) should return the ASCII value and advance one byte
static void test_ascii_single_byte() {
    // Prepare input: 'A' (0x41)
    unsigned char data[] = { 'A', 0x00 }; // zero terminator present but not used by function
    const unsigned char *p = data;
    u32 val = sqlite3Utf8Read(&p);
    EXPECT_EQ((u32)'A', val, "ASCII single-byte read returns codepoint for 'A'");

    // Pointer should advance by exactly 1
    EXPECT_EQ(1u, (unsigned int)(p - data), "ASCII single-byte read advances pointer by 1");
}

// Test 2a: 2-byte sequence with a valid continuation -> uses mapping and yields a valid code point
static void test_two_byte_valid_continuation() {
    // Arrange a 2-byte UTF-8 sequence starting with 0xC2 and continuation 0x80
    // We configure sqlite3Utf8Trans1[0] = 0x80 so result becomes 0x80 << 6 = 0x200 (512)
    sqlite3Utf8Trans1[0] = 0x80; // ensure 2-byte path yields 0x200
    unsigned char data[] = { 0xC2, 0x80, 0x00 };
    const unsigned char *p = data;
    u32 val = sqlite3Utf8Read(&p);

    EXPECT_EQ((u32)0x200, val, "Two-byte sequence with 0xC2,0x80 yields 0x200");
    EXPECT_EQ(2u, (unsigned int)(p - data), "Pointer advanced by 2 after two-byte read");

    // Restore to default for other tests
    sqlite3Utf8Trans1[0] = 0x80;
}

// Test 2b: 2-byte sequence with no additional continuation (no loop) -> yields 0x80
static void test_two_byte_no_continuation() {
    // 0xC2 followed by 0x41 (ASCII 'A'), which is not a continuation byte (0x80..0xBF)
    // We keep sqlite3Utf8Trans1[0] = 0x80 so initial c becomes 0x80 and no loop occurs.
    unsigned char data[] = { 0xC2, 0x41, 0x00 };
    const unsigned char *p = data;
    u32 val = sqlite3Utf8Read(&p);

    EXPECT_EQ((u32)0x80, val, "Two-byte sequence with no continuation yields 0x80");
    EXPECT_EQ(2u, (unsigned int)(p - data), "Pointer advanced by 2 after two-byte read with no continuation");
}

// Test 3: 2-byte sequence that should become invalid and yield 0xFFFD
static void test_two_byte_invalid_surrogate_or_zero() {
    // Force the first mapping to 0x00 so that after one continuation, c becomes 0x00
    // which triggers the invalid sequence clamp to 0xFFFD.
    unsigned int old0 = sqlite3Utf8Trans1[0];
    sqlite3Utf8Trans1[0] = 0x00;

    unsigned char data[] = { 0xC0, 0x80, 0x00 }; // start with 0xC0, next 0x80 is continuation
    const unsigned char *p = data;
    u32 val = sqlite3Utf8Read(&p);

    EXPECT_EQ((u32)0xFFFD, val, "Invalid 2-byte sequence yields 0xFFFD");

    // Pointer advanced by 2
    EXPECT_EQ(2u, (unsigned int)(p - data), "Pointer advanced by 2 after invalid 2-byte read");

    // Restore original mapping
    sqlite3Utf8Trans1[0] = old0;
}

// Test 4: 3-byte sequence path (requires multiple continuations) -> test via 0xE0 start
static void test_three_byte_sequence() {
    // Ensure 0x20 maps to 0x40 for 3-byte sequence path
    unsigned int old20 = sqlite3Utf8Trans1[0x20];
    sqlite3Utf8Trans1[0x20] = 0x40;

    // Data: 0xE0, 0xA0, 0x80, 0x00
    // Continuations: 0xA0 (0xA0 & 0xC0 == 0x80) and 0x80 (continuation)
    // After first continuation: c = (0x40<<6) + (0x3f & 0xA0) = 0x1000 + 0x20 = 0x1020
    // After second: c = (0x1020<<6) + (0x3f & 0x80) = 0x40800 + 0 = 0x40800
    unsigned char data[] = { 0xE0, 0xA0, 0x80, 0x00 };
    const unsigned char *p = data;
    u32 val = sqlite3Utf8Read(&p);

    EXPECT_EQ((u32)0x40800, val, "3-byte sequence yields 0x40800");

    // Pointer advanced by 3
    EXPECT_EQ(3u, (unsigned int)(p - data), "Pointer advanced by 3 after 3-byte read");

    // Restore original mapping
    sqlite3Utf8Trans1[0x20] = old20;
}

int main() {
    init_utf8_trans1();

    test_ascii_single_byte();
    test_two_byte_valid_continuation();
    test_two_byte_no_continuation();
    test_two_byte_invalid_surrogate_or_zero();
    test_three_byte_sequence();

    if(g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED\n";
        return 1;
    }
}