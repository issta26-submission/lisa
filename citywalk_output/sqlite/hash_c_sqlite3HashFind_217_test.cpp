// Unit test suite for sqlite3HashFind (C linkage) using C++11 without Google Test.
// The tests rely on the real sqlite3 hash implementation available in the project,
// compiling and linking hash.c with sqliteInt.h.
// The tests are designed to be non-terminating on failure and report results manually.
//
// Notes on test design mapping to the task requirements:
// - Step 1 (Program Understanding): Candidate Keywords extracted from the focal method and
//   its dependencies: pH, pKey, findElementWithHash, data, Hash, HashElem, _ht, strHash,
//   insertElement, rehash, removeElementGivenHash.
// - Step 2 (Unit Test Generation): Tests focus on sqlite3HashFind behavior when a key is present
//   in the hash (positive path). Since sqlite3HashFind asserts non-null parameters, tests avoid
//   triggering assertion failures (no-Assert mode). We exercise insertion and lookup for correct data
//   pointer return value, and update behavior via repeated inserts for the same key.
// - Step 3 (Test Case Refinement): Simple, robust tests with clear PASS/FAIL reporting and
//   explanations for each test. We rely on the project's actual implementations of sqlite3HashInit,
//   sqlite3HashInsert, sqlite3HashFind and sqlite3HashClear. Static helpers in hash.c are not mocked;
//   we interact with public APIs only.

#include <assert.h>
#include <sqliteInt.h>
#include <string>
#include <iostream>


// Import C APIs from the SQLite-like hash implementation.
// The project provides C headers; ensure C linkage for C++ test code.
extern "C" {
}

// Candidate Keywords (Step 1 mapping):
// - pH: Hash* parameter to sqlite3HashFind
// - pKey: const char* parameter to sqlite3HashFind
// - findElementWithHash: internal static helper used by sqlite3HashFind (not directly accessible here)
// - data: member of HashElem returned via findElementWithHash (sqlite3HashFind returns ->data)
// - Hash: the hash table structure used by sqlite3HashFind/Insert
// - HashElem: element type used within the hash to store {key, data}
// - _ht: internal helper struct used by the hash (not used directly in tests)
// - strHash: hash function (static, not directly used in tests)
// - insertElement: internal helper to insert into hash (public API is sqlite3HashInsert)
// - rehash: internal helper for resizing (not used in tests)
// - removeElementGivenHash: internal helper to remove by hash (not used in tests)

static bool test_hashFind_basic_lookup() {
    // Purpose: verify that inserting a key-data pair and then finding it yields the original data pointer.
    // This exercises the positive path of sqlite3HashFind when the key is present.
    Hash h;
    sqlite3HashInit(&h);

    int value = 12345;
    void *expected = &value;

    const char *key = "alpha";

    // Insert a key-value pair
    sqlite3HashInsert(&h, key, expected);

    // Lookup the key and verify the returned data pointer matches the inserted value
    void *result = sqlite3HashFind(&h, key);
    if (result != expected) {
        std::cerr << "test_hashFind_basic_lookup: expected data pointer " << expected
                  << " but got " << result << std::endl;
        sqlite3HashClear(&h);
        return false;
    }

    sqlite3HashClear(&h);
    return true;
}

static bool test_hashFind_multiple_entries() {
    // Purpose: verify that multiple entries can be stored and retrieved independently.
    // This ensures that the internal structure doesn't mix up different keys/data.
    Hash h;
    sqlite3HashInit(&h);

    int a = 11;
    int b = 22;
    void *pa = &a;
    void *pb = &b;

    sqlite3HashInsert(&h, "one", pa);
    sqlite3HashInsert(&h, "two", pb);

    void *r1 = sqlite3HashFind(&h, "one");
    void *r2 = sqlite3HashFind(&h, "two");

    if (r1 != pa) {
        std::cerr << "test_hashFind_multiple_entries: mismatch for key 'one'. Expected "
                  << pa << ", got " << r1 << std::endl;
        sqlite3HashClear(&h);
        return false;
    }
    if (r2 != pb) {
        std::cerr << "test_hashFind_multiple_entries: mismatch for key 'two'. Expected "
                  << pb << ", got " << r2 << std::endl;
        sqlite3HashClear(&h);
        return false;
    }

    sqlite3HashClear(&h);
    return true;
}

static bool test_hashFind_update_value_for_existing_key() {
    // Purpose: verify that updating the value for an existing key results in the new value
    // being returned by sqlite3HashFind for subsequent lookups.
    Hash h;
    sqlite3HashInit(&h);

    int x = 7;
    int y = 77;
    void *px = &x;
    void *py = &y;

    sqlite3HashInsert(&h, "A", px);
    void *r1 = sqlite3HashFind(&h, "A");
    if (r1 != px) {
        std::cerr << "test_hashFind_update_value_for_existing_key: initial insert/get failed. "
                  << "Expected " << px << ", got " << r1 << std::endl;
        sqlite3HashClear(&h);
        return false;
    }

    // Update the value for the same key
    sqlite3HashInsert(&h, "A", py);
    void *r2 = sqlite3HashFind(&h, "A");
    if (r2 != py) {
        std::cerr << "test_hashFind_update_value_for_existing_key: update failed. "
                  << "Expected " << py << ", got " << r2 << std::endl;
        sqlite3HashClear(&h);
        return false;
    }

    sqlite3HashClear(&h);
    return true;
}

// Lightweight test runner (non-terminating, prints results)
int main() {
    int tests_run = 0;
    int tests_failed = 0;

    std::cout << "Starting sqlite3HashFind unit tests (C interface) - C++ harness\n";

    // Test 1: basic lookup after insertion
    ++tests_run;
    if (test_hashFind_basic_lookup()) {
        std::cout << "[PASSED] test_hashFind_basic_lookup\n";
    } else {
        ++tests_failed;
        std::cout << "[FAILED] test_hashFind_basic_lookup\n";
    }

    // Test 2: multiple entries retrieval
    ++tests_run;
    if (test_hashFind_multiple_entries()) {
        std::cout << "[PASSED] test_hashFind_multiple_entries\n";
    } else {
        ++tests_failed;
        std::cout << "[FAILED] test_hashFind_multiple_entries\n";
    }

    // Test 3: update existing key value
    ++tests_run;
    if (test_hashFind_update_value_for_existing_key()) {
        std::cout << "[PASSED] test_hashFind_update_value_for_existing_key\n";
    } else {
        ++tests_failed;
        std::cout << "[FAILED] test_hashFind_update_value_for_existing_key\n";
    }

    std::cout << "sqlite3HashFind tests completed. Ran: " << tests_run
              << ", Passed: " << (tests_run - tests_failed)
              << ", Failed: " << tests_failed << "\n";

    // Return non-zero if any test failed
    return (tests_failed == 0) ? 0 : 1;
}