// This is a C++11 test suite for the focal method:
// int sqlite3_create_collation(sqlite3* db, const char *zName, int enc, void* pCtx,
//                            int(*xCompare)(void*,int,const void*,int,const void*))
// The implementation delegates to sqlite3_create_collation_v2 with xDel = 0.
// The tests exercise the wrapper by creating a custom collation and
// performing simple comparisons to verify behavior.
// Note: This test uses the public SQLite C API (sqlite3.h) and links against sqlite3.
// It does not require GoogleTest and uses a lightweight, main()-driven test harness.

#include <sqlite3.h>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


// Candidate Keywords extracted from Step 1 (for reference in tests):
// - sqlite3_create_collation, sqlite3_create_collation_v2, sqlite3* db, zName, enc
// - pCtx, xCompare, xDel (passed as 0 in wrapper)
// - Collation behavior: string comparison via provided xCompare function
// - SQLITE_OK, error handling, in-memory database usage ":memory:"

// Global callback to capture a single integer result from a SQL statement.
static int intCallback(void* p, int argc, char** argv, char** colName) {
    if (p != nullptr && argc > 0 && argv != nullptr && argv[0] != nullptr) {
        int val = std::atoi(argv[0]);
        *(static_cast<int*>(p)) = val;
    }
    return 0;
}

// A simple, deterministic comparison function for test purposes.
// It compares two binary keys by their byte content, respecting lengths.
static int testCmp(void* pCtx, int nKey1, const void* pKey1, int nKey2, const void* pKey2) {
    // Cast to unsigned char to avoid sign extension issues with negative bytes
    const unsigned char* a = static_cast<const unsigned char*>(pKey1);
    const unsigned char* b = static_cast<const unsigned char*>(pKey2);
    int minLen = (nKey1 < nKey2) ? nKey1 : nKey2;
    int cmp = std::memcmp(a, b, static_cast<size_t>(minLen));
    if (cmp != 0) return cmp;
    // If common prefix is equal, shorter key sorts first
    if (nKey1 == nKey2) return 0;
    return (nKey1 < nKey2) ? -1 : 1;
}

// Test 1: Verify that the wrapper creates a functional collation and that
// string comparison using the new collation yields the expected true value.
bool test_collation_basic_true() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Test 1: Failed to open in-memory DB. rc=" << rc << "\n";
        return false;
    }

    // Create a collating sequence named TEST_CPP_A
    rc = sqlite3_create_collation(db, "TEST_CPP_A", SQLITE_UTF8, nullptr, testCmp);
    if (rc != SQLITE_OK) {
        std::cerr << "Test 1: Failed to create collation. rc=" << rc << "\n";
        sqlite3_close(db);
        return false;
    }

    // Use the new collation to compare 'abc' and 'abd'. Expect true ('abc' < 'abd')
    int result = -1;
    rc = sqlite3_exec(db,
                      "SELECT CASE WHEN 'abc' < 'abd' COLLATE TEST_CPP_A THEN 1 ELSE 0 END;",
                      intCallback, &result, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Test 1: SQL execution failed. rc=" << rc << "\n";
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    if (result == 1) {
        // PASS
        return true;
    } else {
        std::cerr << "Test 1: Expected 1 (true), got " << result << "\n";
        return false;
    }
}

// Test 2: Verify that the wrapper creates a functional collation and that
// string comparison using the new collation yields the expected false value.
bool test_collation_basic_false() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Test 2: Failed to open in-memory DB. rc=" << rc << "\n";
        return false;
    }

    rc = sqlite3_create_collation(db, "TEST_CPP_A", SQLITE_UTF8, nullptr, testCmp);
    // If test 1 ran previously, the name TEST_CPP_A may exist; since this is a fresh DB,
    // it's safe to reuse or fail gracefully. If it fails due to existing, try a unique name.
    if (rc != SQLITE_OK) {
        // Try with a fresh name in case the previous test left state in the environment
        sqlite3_close(db);
        return false;
    }

    // Use the new collation to compare 'z' and 'a'. Expect false ('z' < 'a' is false)
    int result = -1;
    rc = sqlite3_exec(db,
                      "SELECT CASE WHEN 'z' < 'a' COLLATE TEST_CPP_A THEN 1 ELSE 0 END;",
                      intCallback, &result, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Test 2: SQL execution failed. rc=" << rc << "\n";
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    if (result == 0) {
        // PASS
        return true;
    } else {
        std::cerr << "Test 2: Expected 0 (false), got " << result << "\n";
        return false;
    }
}

// Test 3: Duplicate collation name should fail. Ensure error is returned when re-declaring.
bool test_duplicate_collation_error() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Test 3: Failed to open in-memory DB. rc=" << rc << "\n";
        return false;
    }

    // First creation should succeed
    rc = sqlite3_create_collation(db, "TEST_CPP_DUP", SQLITE_UTF8, nullptr, testCmp);
    if (rc != SQLITE_OK) {
        std::cerr << "Test 3: First collation creation failed. rc=" << rc << "\n";
        sqlite3_close(db);
        return false;
    }

    // Second creation with the same name should fail
    rc = sqlite3_create_collation(db, "TEST_CPP_DUP", SQLITE_UTF8, nullptr, testCmp);
    sqlite3_close(db);

    if (rc != SQLITE_OK) {
        // PASS: Re-declaration correctly failed
        return true;
    } else {
        std::cerr << "Test 3: Duplicate collation creation unexpectedly succeeded.\n";
        return false;
    }
}

// Test 4: Supplying a NULL xCompare function should result in an error (invalid API usage).
bool test_null_xCompare_error() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Test 4: Failed to open in-memory DB. rc=" << rc << "\n";
        return false;
    }

    // xCompare is NULL; this should fail as per API contract
    rc = sqlite3_create_collation(db, "TEST_CPP_NULL", SQLITE_UTF8, nullptr, nullptr);
    sqlite3_close(db);

    if (rc != SQLITE_OK) {
        // PASS: NULL xCompare correctly reported as error
        return true;
    } else {
        std::cerr << "Test 4: Creation with NULL xCompare unexpectedly succeeded.\n";
        return false;
    }
}

int main() {
    int total = 0;
    int passed = 0;

    // Step 2: Unit Test Generation
    // Test suite covers:
    // - True branch of string comparison using a custom collation
    // - False branch of string comparison using a custom collation
    // - Behavior when attempting to redefine an existing collation (error path)
    // - Behavior when xCompare is NULL (error path)
    std::cout << "Starting tests for sqlite3_create_collation wrapper...\n";

    if (test_collation_basic_true()) {
        std::cout << "[PASS] test_collation_basic_true\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_collation_basic_true\n";
    }
    ++total;

    if (test_collation_basic_false()) {
        std::cout << "[PASS] test_collation_basic_false\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_collation_basic_false\n";
    }
    ++total;

    if (test_duplicate_collation_error()) {
        std::cout << "[PASS] test_duplicate_collation_error\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_duplicate_collation_error\n";
    }
    ++total;

    if (test_null_xCompare_error()) {
        std::cout << "[PASS] test_null_xCompare_error\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_null_xCompare_error\n";
    }
    ++total;

    std::cout << "Test results: " << passed << " / " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}