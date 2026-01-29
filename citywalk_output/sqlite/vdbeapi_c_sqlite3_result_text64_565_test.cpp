#include <opcodes.h>
#include <sqlite3.h>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>


// This test suite exercises the focal function sqlite3_result_text64
// by validating its interaction through a public SQLite API usage pattern.
// We register user-defined functions (UDFs) that invoke sqlite3_result_text64
// and verify the results via standard SQL queries.
// The tests are written in C++11 and rely only on the public SQLite API.

/*
Domain considerations implemented:
- Use public API: sqlite3_open, sqlite3_create_function, sqlite3_prepare_v2, sqlite3_step, sqlite3_column_text, sqlite3_column_type, sqlite3_finalize, sqlite3_close.
- Test multiple encodings via UDFs that call sqlite3_result_text64 with different enc values.
- Test edge case: large n (> 0x7fffffff) to trigger the destructor path without producing a result.
- Test edge case: zero-length n to ensure empty string handling.
- Use in-memory DB to keep tests fast and isolated.
- Provide explanatory comments for each unit test.
*/

// -------------------------
// UDF Implementations
// -------------------------

// UDF 1: Return a normal UTF-8 encoded string with explicit length
extern "C" void udf_text_utf8(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    (void)argc;
    (void)argv;
    const char* z = "hello";
    // n = 5, enc = SQLITE_UTF8
    sqlite3_result_text64(ctx, z, 5, SQLITE_TRANSIENT, SQLITE_UTF8);
}

// UDF 2: Return the same text but encoded as UTF-16
extern "C" void udf_text_utf16(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    (void)argc;
    (void)argv;
    const char* z = "hello";
    // n = 5, enc = SQLITE_UTF16
    sqlite3_result_text64(ctx, z, 5, SQLITE_TRANSIENT, SQLITE_UTF16);
}

// UDF 3: Large n to exercise the destructor path (n > 0x7fffffff)
extern "C" void udf_text_large_n(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    (void)argc;
    (void)argv;
    // n is intentionally large to trigger the destructor path in sqlite3_result_text64
    sqlite3_result_text64(ctx, "hello", (sqlite3_uint64)0x80000000, SQLITE_TRANSIENT, SQLITE_UTF8);
}

// UDF 4: Zero-length n to exercise zero-length string handling
extern "C" void udf_text_zero_length(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    (void)argc;
    (void)argv;
    // n = 0 should yield an empty string according to sqlite3_result_text64 semantics
    sqlite3_result_text64(ctx, "ignored", 0, SQLITE_TRANSIENT, SQLITE_UTF8);
}

// -------------------------
// Helper Utilities
// -------------------------

// Executes a SQL statement that should return a single TEXT result.
// Returns true on success and fills outText with the result content.
static bool execSingleText(sqlite3* db, const char* sql, std::string& outText) {
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        std::cerr << "Step did not return a row: " << rc << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    const unsigned char* text = sqlite3_column_text(stmt, 0);
    // If NULL, treat as failure for this helper (caller can decide NULL vs non-NULL)
    if (text == nullptr) {
        sqlite3_finalize(stmt);
        return false;
    }
    outText = reinterpret_cast<const char*>(text);
    sqlite3_finalize(stmt);
    return true;
}

// Executes a SQL statement that should return NULL (i.e., no text result).
static bool execSingleNull(sqlite3* db, const char* sql) {
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        // If no row, it's an error for our test
        std::cerr << "Step did not return a row (expected NULL): " << rc << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    int colType = sqlite3_column_type(stmt, 0);
    sqlite3_finalize(stmt);
    return (colType == SQLITE_NULL);
}

// -------------------------
// Test Routines
// -------------------------

// Test 1: UTF-8 encoding path should return the expected string
static bool test_utf8_result(sqlite3* db) {
    // Register a 0-argument UDF returning "hello" with UTF-8 encoding
    int rc = sqlite3_create_function(db, "utf8_text", 0, SQLITE_UTF8, nullptr, udf_text_utf8, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to register utf8_text: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    std::string result;
    bool ok = execSingleText(db, "SELECT utf8_text();", result);
    if (!ok) {
        std::cerr << "UTF-8 test: failed to get result text." << std::endl;
        return false;
    }
    if (result != "hello") {
        std::cerr << "UTF-8 test: expected 'hello', got '" << result << "'" << std::endl;
        return false;
    }
    return true;
}

// Test 2: UTF-16 encoding path should return the same textual content
static bool test_utf16_result(sqlite3* db) {
    // Register a 0-argument UDF returning "hello" with UTF-16 encoding
    int rc = sqlite3_create_function(db, "utf16_text", 0, SQLITE_UTF16, nullptr, udf_text_utf16, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to register utf16_text: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    std::string result;
    bool ok = execSingleText(db, "SELECT utf16_text();", result);
    if (!ok) {
        std::cerr << "UTF-16 test: failed to get result text." << std::endl;
        return false;
    }
    if (result != "hello") {
        std::cerr << "UTF-16 test: expected 'hello', got '" << result << "'" << std::endl;
        return false;
    }
    return true;
}

// Test 3: Zero-length n should yield an empty string (not NULL)
static bool test_zero_length_result(sqlite3* db) {
    int rc = sqlite3_create_function(db, "zero_text", 0, SQLITE_UTF8, nullptr, udf_text_zero_length, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to register zero_text: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    std::string result;
    bool ok = execSingleText(db, "SELECT zero_text();", result);
    if (!ok) {
        std::cerr << "Zero-length test: failed to get result." << std::endl;
        return false;
    }
    // We expect an empty string due to n == 0
    if (result != "") {
        std::cerr << "Zero-length test: expected empty string, got '" << result << "'" << std::endl;
        return false;
    }
    return true;
}

// Test 4: Large n should trigger destructor path and yield NULL result
static bool test_large_n_result_null(sqlite3* db) {
    int rc = sqlite3_create_function(db, "large_n_text", 0, SQLITE_UTF8, nullptr, udf_text_large_n, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to register large_n_text: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    bool ok = execSingleNull(db, "SELECT large_n_text();");
    if (!ok) {
        std::cerr << "Large-n test: expected NULL result but did not observe NULL." << std::endl;
        return false;
    }
    return true;
}

// -------------------------
// Main Test Runner
// -------------------------

int main() {
    // Initialize in-memory SQLite database
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open in-memory database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    int total = 0;
    int passed = 0;

    auto runTest = [&](const char* name, bool (*fn)(sqlite3*)){
        std::cout << "Running test: " << name << " ... ";
        total++;
        bool ok = fn(db);
        if (ok) {
            std::cout << "PASSED" << std::endl;
            passed++;
        } else {
            std::cout << "FAILED" << std::endl;
        }
        return ok;
    };

    // Execute tests
    runTest("sqlite3_result_text64_utf8_path_returns_expected_text", test_utf8_result);
    runTest("sqlite3_result_text64_utf16_path_returns_expected_text", test_utf16_result);
    runTest("sqlite3_result_text64_zero_length_path_returns_empty_string", test_zero_length_result);
    runTest("sqlite3_result_text64_large_n_path_returns_null", test_large_n_result_null);

    // Summary
    std::cout << "\nTest Summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Cleanup
    sqlite3_close(db);
    return (passed == total) ? 0 : 1;
}