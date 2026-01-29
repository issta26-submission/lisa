// C++11 test suite for sqlite3_value_numeric_type
// Focus: validate behavior when the input value is TEXT (affinity is applied)
// and when the input is non-TEXT (should pass through), plus NULL case.
// This test avoids GTest and uses a lightweight harness with non-terminating
// assertions to maximize coverage.

#include <vector>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Domain knowledge hints: this test uses internal SQLite structures.
// Include internal headers to get Mem and related constants/types.
extern "C" {
}

// Expose the focal function from the C codebase
extern "C" int sqlite3_value_numeric_type(sqlite3_value *pVal);

// Helper: a minimal non-terminating assertion/logging mechanism
static int g_tests_passed = 0;
static int g_tests_total  = 0;

#define TX_FAIL(msg) do { \
    std::cerr << "TEST FAILED: " << msg << std::endl; \
} while(0)

#define EXPECT_EQ(actual, expected, msg) do { \
    g_tests_total++; \
    if ((actual) != (expected)) { TX_FAIL(std::string("EXPECT_EQ failed: ") + msg + \
        " | actual=" + std::to_string((long long)(actual)) + \
        " expected=" + std::to_string((long long)(expected))); \
    } else { g_tests_passed++; } \
} while(0)

#define ASSERT_TRUE(cond, msg) do { \
    g_tests_total++; \
    if (!(cond)) { TX_FAIL(std::string("ASSERT_TRUE failed: ") + msg); } \
    else { g_tests_passed++; } \
} while(0)


// Utility: create a Mem object representing a TEXT value.
// We allocate memory for the string and set Mem fields accordingly.
// This relies on internal Mem layout; we assume Mem layout matches the
// sqlite3_value internal type used by sqlite3_value_type/value_numeric_type.
static Mem buildTextMem(const char* text) {
    Mem m;
    memset(&m, 0, sizeof(Mem));
    size_t len = text ? std::strlen(text) : 0;
    if (len) {
        m.z = (char*)std::malloc(len + 1);
        std::memcpy(m.z, text, len);
        m.z[len] = '\0';
        m.n = (u32)len;
    } else {
        m.z = nullptr;
        m.n = 0;
    }
    // mark as text
    m.flags = MEM_Str;
    m.enc = SQLITE_UTF8;
    // Other fields are zeroed; applyNumericAffinity will operate on the text
    return m;
}

// Utility: free a Mem object created by buildTextMem
static void freeTextMem(Mem& m) {
    if (m.z) {
        std::free(m.z);
        m.z = nullptr;
    }
    m.n = 0;
    m.flags = 0;
    m.enc = SQLITE_UTF8;
}


// Test 1: TEXT "123" should be interpreted as SQLITE_INTEGER after numeric affinity
static void test_text_numeric_integer() {
    Mem m = buildTextMem("123");
    int res = sqlite3_value_numeric_type((sqlite3_value*)&m);
    EXPECT_EQ(res, SQLITE_INTEGER, "Text '123' should be numeric INTEGER");
    freeTextMem(m);
}

// Test 2: TEXT "123.45" should be interpreted as SQLITE_FLOAT after numeric affinity
static void test_text_numeric_float() {
    Mem m = buildTextMem("123.45");
    int res = sqlite3_value_numeric_type((sqlite3_value*)&m);
    EXPECT_EQ(res, SQLITE_FLOAT, "Text '123.45' should be numeric FLOAT/REAL");
    freeTextMem(m);
}

// Test 3: TEXT "abc" should remain SQLITE_TEXT (non-numeric)
static void test_text_non_numeric() {
    Mem m = buildTextMem("abc");
    int res = sqlite3_value_numeric_type((sqlite3_value*)&m);
    EXPECT_EQ(res, SQLITE_TEXT, "Text 'abc' should remain TEXT");
    freeTextMem(m);
}

// Test 4: Non-TEXT input (INTEGER) should pass through unchanged
static void test_non_text_integer() {
    Mem m;
    memset(&m, 0, sizeof(Mem));
    m.flags = MEM_Int;
    m.u.i = 42;
    m.enc = SQLITE_UTF8;
    int res = sqlite3_value_numeric_type((sqlite3_value*)&m);
    // Expect INTEGER
    EXPECT_EQ(res, SQLITE_INTEGER, "Pre-set MEM_Int should yield SQLITE_INTEGER");
    // Clean up (no dynamic allocation in this Mem)
}

// Test 5: NULL input should yield SQLITE_NULL
static void test_null_value() {
    Mem m;
    memset(&m, 0, sizeof(Mem));
    m.flags = MEM_Null;
    int res = sqlite3_value_numeric_type((sqlite3_value*)&m);
    EXPECT_EQ(res, SQLITE_NULL, "MEM_Null should yield SQLITE_NULL");
}


// Entry point
int main() {
    std::cout << "Running sqlite3_value_numeric_type tests (C core integration)" << std::endl;

    test_text_numeric_integer();
    test_text_numeric_float();
    test_text_non_numeric();
    test_non_text_integer();
    test_null_value();

    std::cout << "Tests completed. "
              << "Passed: " << g_tests_passed << "/" << g_tests_total
              << std::endl;

    // Return non-zero if test failures exist
    if (g_tests_passed != g_tests_total) {
        std::cerr << "Some tests failed." << std::endl;
        return 1;
    }
    return 0;
}