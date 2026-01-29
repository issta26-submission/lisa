/*
  Unit test suite for sqlite3_uri_int64 (Step 2 and 3)
  - Uses C++11, no Google Test
  - Provides lightweight mocks for sqlite3_uri_parameter and sqlite3DecOrHexToI64
  - Exercises true/false branches of parameter extraction and parsing
  - Tests are executed from main()
  - Explanatory comments accompany each test
*/

#include <vector>
#include <cerrno>
#include <string>
#include <cinttypes>
#include <map>
#include <sqliteInt.h>
#include <limits>
#include <iostream>
#include <cstdlib>


typedef long long sqlite3_int64;

// Forward declarations of the function under test and its dependencies
extern "C" sqlite3_int64 sqlite3_uri_int64(
  const char *zFilename,
  const char *zParam,
  sqlite3_int64 bDflt
);

extern "C" const char *sqlite3_uri_parameter(const char *zFilename, const char *zParam);
extern "C" int sqlite3DecOrHexToI64(const char *z, sqlite3_int64 *out);

// ---------------- Mock infrastructure for dependent C functions ----------------

// Global storage for mapping (filename, param) -> string value
static std::map<std::string, std::string> g_uri_param_map;

// Storage to keep lifetime of returned C strings
static std::vector<std::string> g_param_strings;

// Helpers to manage mock state
static void clear_mocks() {
    g_uri_param_map.clear();
    g_param_strings.clear();
}

static void set_uri_param(const std::string& filename, const std::string& param, const std::string& value) {
    g_uri_param_map[filename + "|" + param] = value;
}

// Mock implementation of sqlite3_uri_parameter
extern "C" const char* sqlite3_uri_parameter(const char* zFilename, const char* zParam) {
    std::string key = std::string(zFilename) + "|" + std::string(zParam);
    auto it = g_uri_param_map.find(key);
    if (it == g_uri_param_map.end()) {
        return nullptr;
    }
    // Persist the value so that the pointer remains valid after the function returns
    g_param_strings.push_back(it->second);
    return g_param_strings.back().c_str();
}

// Mock implementation of sqlite3DecOrHexToI64
// Returns 0 on success, non-zero on failure, consistent with sqlite3's convention
extern "C" int sqlite3DecOrHexToI64(const char* z, sqlite3_int64* out) {
    if (z == nullptr || out == nullptr) {
        return 1;
    }
    try {
        std::string s(z);
        std::size_t idx = 0;
        long long val = 0;

        // Detect hex (prefix 0x or 0X) or decimal
        if (s.size() >= 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
            val = std::stoll(s, &idx, 16);
        } else {
            val = std::stoll(s, &idx, 10);
        }

        // Ensure the entire string was parsed
        if (idx != s.size()) {
            return 1;
        }

        *out = static_cast<sqlite3_int64>(val);
        return 0;
    } catch (...) {
        return 1;
    }
}

// ---------------- Test harness helpers ----------------

static int g_tests_failed = 0;

#define EXPECT_EQ(actual, expected, msg) do { \
    if ((actual) != (expected)) { \
        std::cerr << "[FAIL] " << (msg) << " | Expected: " << (expected) \
                  << "  Got: " << (actual) << std::endl; \
        ++g_tests_failed; \
    } else { \
        std::cout << "[OK] " << (msg) << std::endl; \
    } \
} while(0)

static void test_decimal_updates_bDflt() {
    // True branch: z has a valid decimal value, and parsing succeeds
    clear_mocks();
    set_uri_param("uri_decimal", "param", "123");

    sqlite3_int64 initial = 42;
    sqlite3_int64 result = sqlite3_uri_int64("uri_decimal", "param", initial);

    EXPECT_EQ(result, 123, "Decimal value should update bDflt to 123 when parsed successfully");
}

static void test_hex_updates_bDflt() {
    // True branch: z has a valid hex value, parsing succeeds
    clear_mocks();
    set_uri_param("uri_hex", "param", "0x2A"); // 42 in decimal

    sqlite3_int64 initial = 7;
    sqlite3_int64 result = sqlite3_uri_int64("uri_hex", "param", initial);

    EXPECT_EQ(result, 42, "Hex value 0x2A should update bDflt to 42 when parsed successfully");
}

static void test_invalid_value_does_not_update() {
    // False branch: parsing fails, should not update bDflt
    clear_mocks();
    set_uri_param("uri-invalid", "param", "notanumber");

    sqlite3_int64 initial = 55;
    sqlite3_int64 result = sqlite3_uri_int64("uri-invalid", "param", initial);

    EXPECT_EQ(result, 55, "Invalid numeric string should not update bDflt");
}

static void test_missing_param_does_not_update() {
    // False branch: parameter missing, sqlite3_uri_parameter returns nullptr
    clear_mocks();
    // Do not set any parameter for this filename/param

    sqlite3_int64 initial = 77;
    sqlite3_int64 result = sqlite3_uri_int64("uri_missing", "param", initial);

    EXPECT_EQ(result, 77, "Missing parameter should not update bDflt");
}

static void test_overflow_value_does_not_update() {
    // False branch: value too large to fit in sqlite3_int64
    clear_mocks();
    set_uri_param("uri_overflow", "param", "99999999999999999999999999"); // obviously too large

    sqlite3_int64 initial = 10;
    sqlite3_int64 result = sqlite3_uri_int64("uri_overflow", "param", initial);

    EXPECT_EQ(result, 10, "Overflow value should not update bDflt");
}

static void test_boundary_hex_maxupdates() {
    // True branch: maximum signed 64-bit hex value
    clear_mocks();
    set_uri_param("uri_hex_max", "param", "0x7FFFFFFFFFFFFFFF"); // max positive int64

    sqlite3_int64 initial = 0;
    sqlite3_int64 result = sqlite3_uri_int64("uri_hex_max", "param", initial);

    EXPECT_EQ(result, 9223372036854775807LL, "Hex max value should update to 9223372036854775807");
}

// ---------------- Main to run tests ----------------

int main() {
    std::cout << "Starting sqlite3_uri_int64 unit tests (non-GTest, C++11)\n";

    test_decimal_updates_bDflt();
    test_hex_updates_bDflt();
    test_invalid_value_does_not_update();
    test_missing_param_does_not_update();
    test_overflow_value_does_not_update();
    test_boundary_hex_maxupdates();

    if (g_tests_failed != 0) {
        std::cerr << g_tests_failed << " test(s) failed.\n";
        return 1;
    } else {
        std::cout << "All tests passed.\n";
        return 0;
    }
}