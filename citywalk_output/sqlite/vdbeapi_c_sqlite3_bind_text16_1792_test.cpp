// Minimal C++11 test suite for sqlite3_bind_text16
// This test re-creates a small, self-contained environment to verify
// the behavior of sqlite3_bind_text16 with respect to length masking
// (n & ~(u64)1) and the proper passing of encoding (SQLITE_UTF16NATIVE)
// and the xDel callback through to bindText.
//
// Note: This is a lightweight, isolated unit test harness and does not
// depend on external GTest. It uses simple in-process checks and prints
// failures to std::cerr while continuing execution to maximize coverage.

#include <opcodes.h>
#include <cstddef>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Domain-specific typedefs and constants (simplified for test harness)
using i64 = long long;
using u64 = unsigned long long;

// Mocked encoding constant used by the real code
static const unsigned char SQLITE_UTF16NATIVE = 1;

// Forward-declare the test-friendly sqlite3_stmt type (opaque in real SQLite)
struct sqlite3_stmt { };

// Global state to observe parameters passed to the internal bindText function
static int g_last_i = -1;
static const void* g_last_zData = nullptr;
static i64 g_last_nData = 0;
static void (*g_last_xDel)(void*) = nullptr;
static unsigned char g_last_encoding = 0;

// Return value propagation simulator
static int g_last_ret = 0;

// "Internal" bindText mock to capture input parameters.
// In the real code, bindText is a static function; here we mock its observable
// behavior to verify what sqlite3_bind_text16 forwards to it.
static int bindText(sqlite3_stmt *pStmt,
                    int i,
                    const void *zData,
                    i64 nData,
                    void (*xDel)(void*),
                    unsigned char encoding)
{
    // Record all inputs BSO that tests can assert on them
    g_last_i = i;
    g_last_zData = zData;
    g_last_nData = nData;
    g_last_xDel = xDel;
    g_last_encoding = encoding;

    // Return a sentinel to verify propagation of return value
    g_last_ret = 0xBADC0DE; // arbitrary distinctive value
    return g_last_ret;
}

// The focal method under test (reconstructed for unit testing in this harness)
int sqlite3_bind_text16(
  sqlite3_stmt *pStmt,
  int i,
  const void *zData,
  int n,
  void (*xDel)(void*)
){
    // Mask length to even number and pass through to bindText with SQLITE_UTF16NATIVE
    return bindText(pStmt, i, zData, n & ~(u64)1, xDel, SQLITE_UTF16NATIVE);
}

// ---------------------------------------------------------------------------
// Lightweight test framework (non-terminating assertions)
// ---------------------------------------------------------------------------

static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_START() ++g_total_tests
#define TEST_FAIL(msg) do { std::cerr << "Test failure: " << (msg) << "\n"; ++g_failed_tests; } while(0)
#define EXPECT_EQ_INT(a,b, msg) do { int _a = (a); int _b = (b); if(_a != _b){ std::cerr << "EXPECT_EQ_INT failed: " << (msg) 
                                                << " | expected " << _b << ", got " << _a << "\n"; ++g_failed_tests; } } while(0)
#define EXPECT_EQ_PTR(a,b, msg) do { const void* _a = (a); const void* _b = (b); if(_a != _b){ std::cerr << "EXPECT_EQ_PTR failed: " << (msg) 
                                                << " | expected " << _b << ", got " << _a << "\n"; ++g_failed_tests; } } while(0)
#define EXPECT_TRUE(cond, msg) do { bool _c = (cond); if(!_c){ std::cerr << "EXPECT_TRUE failed: " << (msg) << "\n"; ++g_failed_tests; } } while(0)


// Simple test data payload
static const char test_data[] = "test-data";

// ---------------------------------------------------------------------------
// Unit tests
// ---------------------------------------------------------------------------

// Test that sqlite3_bind_text16 forwards the original data pointer, i, and
// xDel callback to bindText, and encodes length n as an even number.
bool test_bind_text16_even_length_forwards_correctly() {
    TEST_START();
    sqlite3_stmt stmt;
    const void* zData = (const void*)test_data;
    void (*del)(void*) = [](void*){ /* no-op deleter for test */ };

    // Reset observed state
    g_last_i = -1;
    g_last_zData = nullptr;
    g_last_nData = 0;
    g_last_xDel = nullptr;
    g_last_encoding = 0;

    int ret = sqlite3_bind_text16(&stmt, 2, zData, 4, del);

    // Verify that all parameters are forwarded correctly
    EXPECT_EQ_INT(g_last_i, 2, "i should be forwarded to bindText");
    EXPECT_EQ_PTR(g_last_zData, zData, "zData pointer should be passed through");
    EXPECT_EQ_INT((int)g_last_nData, 4, "n should be masked to even (already even)");
    EXPECT_EQ_PTR((void*)g_last_xDel, (void*)del, "xDel should be passed through");
    EXPECT_TRUE(g_last_encoding == SQLITE_UTF16NATIVE, "encoding should be SQLITE_UTF16NATIVE");
    EXPECT_EQ_INT(ret, 0xBADC0DE, "sqlite3_bind_text16 should return the value from bindText");

    return g_failed_tests == 0;
}

// Test that sqlite3_bind_text16 masks odd lengths down to the nearest even value
bool test_bind_text16_odd_length_masks_to_even() {
    TEST_START();
    sqlite3_stmt stmt;
    const void* zData = (const void*)test_data;
    void (*del)(void*) = nullptr;

    int ret = sqlite3_bind_text16(&stmt, 1, zData, 5, del);

    // Expect n passed to bindText to be 4 (5 & ~1 == 4)
    EXPECT_EQ_INT(g_last_i, 1, "i should be forwarded to bindText for odd length");
    EXPECT_EQ_PTR(g_last_zData, zData, "zData pointer should be passed through");
    EXPECT_EQ_INT((int)g_last_nData, 4, "n should be masked to nearest even value (5 -> 4)");
    EXPECT_EQ_PTR((void*)g_last_xDel, (void*)del, "xDel should be passed through (nullptr in this test)");
    EXPECT_TRUE(g_last_encoding == SQLITE_UTF16NATIVE, "encoding should still be SQLITE_UTF16NATIVE");
    EXPECT_EQ_INT(ret, 0xBADC0DE, "sqlite3_bind_text16 should return the value from bindText");

    return g_failed_tests == 0;
}

// Ensure zero length still passes through correctly (n = 0)
bool test_bind_text16_zero_length() {
    TEST_START();
    sqlite3_stmt stmt;
    const void* zData = (const void*)test_data;
    void (*del)(void*) = nullptr;

    int ret = sqlite3_bind_text16(&stmt, 0, zData, 0, del);

    EXPECT_EQ_INT(g_last_i, 0, "i should be forwarded to bindText (zero length case)");
    EXPECT_EQ_PTR(g_last_zData, zData, "zData pointer should be passed through");
    EXPECT_EQ_INT((int)g_last_nData, 0, "n should be masked to 0 (already even)");
    EXPECT_EQ_PTR((void*)g_last_xDel, (void*)del, "xDel should be passed through (nullptr)");
    EXPECT_TRUE(g_last_encoding == SQLITE_UTF16NATIVE, "encoding should be SQLITE_UTF16NATIVE");
    EXPECT_EQ_INT(ret, 0xBADC0DE, "sqlite3_bind_text16 should return the value from bindText");

    return g_failed_tests == 0;
}


// ---------------------------------------------------------------------------
// Main: run tests and report summary
// ---------------------------------------------------------------------------

int main() {
    // Run individual tests
    bool ok = true;
    ok &= test_bind_text16_even_length_forwards_correctly();
    ok &= test_bind_text16_odd_length_masks_to_even();
    ok &= test_bind_text16_zero_length();

    if (g_failed_tests == 0) {
        std::cout << "ALL TESTS PASSED (" << g_total_tests << " tests)." << std::endl;
        return 0;
    } else {
        std::cerr << g_failed_tests << " TEST(S) FAILED out of " << g_total_tests << "." << std::endl;
        return 1;
    }
}