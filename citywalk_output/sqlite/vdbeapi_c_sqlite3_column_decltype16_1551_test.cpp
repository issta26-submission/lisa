// Test suite for sqlite3_column_decltype16
// This test harness is a lightweight, non-GTest C++11 test suite.
// It exercises the focal function via the public SQLite API by using
// an in-memory database and prepared statements to derive the declared types
// of result columns. The tests are designed to be executed from main().
// Notes:
// - This relies on the libsqlite3 available on the system.
// - We interpret the UTF-16LE result from sqlite3_column_decltype16 as a narrow
//   ASCII string for assertion (works for TYPE names like TEXT, INTEGER, REAL, etc.).
// - Each test includes explanatory comments describing the intent and expectations.

#include <functional>
#include <vector>
#include <opcodes.h>
#include <sqlite3.h>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// Helper: convert a UTF-16LE string pointed to by p to a std::string (ASCII subset)
static std::string utf16le_to_string(const void* p) {
    if (p == nullptr) return "";
    const uint16_t* w = reinterpret_cast<const uint16_t*>(p);
    std::string s;
    // Read until a 0 terminator (UTF-16LE)
    for (size_t i = 0; w[i] != 0; ++i) {
        // Assume ASCII subset for the type names (TEXT, INTEGER, REAL, BLOB, NUMERIC, etc.)
        s.push_back(static_cast<char>(w[i] & 0xFF));
        // Since we assume ASCII, upper byte is ignored.
    }
    return s;
}

// Simple test framework (no GTest)
struct TestCase {
    std::string name;
    std::function<bool()> func;
};

static void run_tests(const std::vector<TestCase>& tests) {
    int passed = 0;
    int failed = 0;
    for (const auto& t : tests) {
        bool result = false;
        try {
            result = t.func();
        } catch (...) {
            std::cerr << "[ERROR] Exception in test: " << t.name << "\n";
            result = false;
        }
        if (result) {
            ++passed;
            std::cout << "[PASS] " << t.name << "\n";
        } else {
            ++failed;
            std::cout << "[FAIL] " << t.name << "\n";
        }
    }
    std::cout << "\nTest results: " << passed << " passed, " << failed << " failed, "
              << tests.size() << " total.\n";
}

// Test 1: Verify that CAST(... AS TEXT) yields the declared type "TEXT".
// This exercises the path where a specific declared type is provided for the
// result column.
static bool test_decltype_cast_text() {
    sqlite3* db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        std::cerr << "Failed to open in-memory DB\n";
        return false;
    }

    sqlite3_stmt* stmt = nullptr;
    const char* zSql = "SELECT CAST(1 AS TEXT) AS t";
    int rc = sqlite3_prepare_v2(db, zSql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed for CAST TEXT statement: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    // sqlite3_column_decltype16 should return the declared type (TEXT) for the first column.
    const void* p = sqlite3_column_decltype16(stmt, 0);
    std::string s = utf16le_to_string(p);

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Expect "TEXT" (case-sensitive expected by SQLite's identity)
    if (s == "TEXT") {
        return true;
    } else {
        std::cerr << "Expected TYPE 'TEXT', got '" << s << "'\n";
        return false;
    }
}

// Test 2: Verify that a simple SELECT returns a non-empty declared type (likely INTEGER or similar).
// This covers the non-CAST case and exercises the branch where a type may be inferred.
static bool test_decltype_simple_integer() {
    sqlite3* db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        std::cerr << "Failed to open in-memory DB\n";
        return false;
    }

    sqlite3_stmt* stmt = nullptr;
    const char* zSql = "SELECT 1 AS t";
    int rc = sqlite3_prepare_v2(db, zSql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed for simple INTEGER statement: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    const void* p = sqlite3_column_decltype16(stmt, 0);
    std::string s = utf16le_to_string(p);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    if (s.empty()) {
        std::cerr << "Expected non-empty declared type for simple integer column, got empty string\n";
        return false;
    }
    // Acceptable values include INTEGER, NUMERIC, REAL, TEXT, BLOB, etc. For a simple
    // SELECT 1, we typically get "INTEGER" or an affinity-derived type.
    // We only require non-empty content to indicate some declared type was returned.
    return true;
}

// Test 3: Verify that CAST(1 AS REAL) yields the declared type "REAL".
// This tests another explicit type path.
static bool test_decltype_cast_real() {
    sqlite3* db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        std::cerr << "Failed to open in-memory DB\n";
        return false;
    }

    sqlite3_stmt* stmt = nullptr;
    const char* zSql = "SELECT CAST(1 AS REAL) AS r";
    int rc = sqlite3_prepare_v2(db, zSql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed for CAST REAL statement: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    const void* p = sqlite3_column_decltype16(stmt, 0);
    std::string s = utf16le_to_string(p);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    if (s == "REAL") {
        return true;
    } else {
        std::cerr << "Expected TYPE 'REAL', got '" << s << "'\n";
        return false;
    }
}

// Test 4: Invalid column index should yield NULL (or empty) pointer.
// This tests the boundary condition where N is out of range.
static bool test_decltype_invalid_index() {
    sqlite3* db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        std::cerr << "Failed to open in-memory DB\n";
        return false;
    }

    sqlite3_stmt* stmt = nullptr;
    const char* zSql = "SELECT 1 AS t";
    int rc = sqlite3_prepare_v2(db, zSql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed for invalid index test: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    // Intentionally use an out-of-bounds column index
    const void* p = sqlite3_column_decltype16(stmt, 1);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Expect NULL pointer for invalid index
    if (p == nullptr) {
        return true;
    } else {
        // If non-null, print diagnostic information
        std::string s = utf16le_to_string(p);
        std::cerr << "Expected NULL for invalid index, got non-null string: '" << s << "'\n";
        return false;
    }
}

int main() {
    // Gather tests
    std::vector<TestCase> tests = {
        {"sqlite3_column_decltype16_cast_text", test_decltype_cast_text},
        {"sqlite3_column_decltype16_simple_integer", test_decltype_simple_integer},
        {"sqlite3_column_decltype16_cast_real", test_decltype_cast_real},
        {"sqlite3_column_decltype16_invalid_index", test_decltype_invalid_index}
    };

    // Run tests
    run_tests(tests);

    // Final result based on test outcomes is printed by run_tests()
    return 0;
}