/*
  Unit Test Suite for sqlite3Fts3HashFind (C function)
  - Implemented in C source fts3_hash.c, with public API declared in fts3_hash.h
  - This test harness uses C++11, no Google Test, and runs in a single main()
  - It exercises the Fts3HashFind path by creating a hash, inserting elements, and verifying
    the behavior when an element exists and when it doesn't.
  - Notes:
    - We interact with C APIs via extern "C" to ensure proper linkage.
    - We keep tests non-terminating on failure to maximize coverage; failures are logged and reported at the end.
    - We avoid touching private/static internals; we only use the public API (init, insert, find, clear).
*/

#include <fts3Int.h>
#include <stdlib.h>
#include <string.h>
#include <fts3_hash.h>
#include <assert.h>
#include <cstring>
#include <iostream>
#include <cstdint>


// Bring in the C headers with C linkage
extern "C" {
}

// Global test failure counter
static int g_failures = 0;

// Lightweight non-terminating assertion macro for tests
#define TEST_ASSERT(cond, message) do { \
    if(!(cond)) { \
        std::cerr << "TEST FAILURE: " << (message) \
                  << "  [File: " << __FILE__ << ", Line: " << __LINE__ << "]" << std::endl; \
        ++g_failures; \
    } \
} while(0)

// Helper to print test summary
static void print_summary() {
    if(g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::cerr << "Total test failures: " << g_failures << std::endl;
    }
}

// Test 1: When an element exists for the given key, sqlite3Fts3HashFind should return its data pointer.
static void test_find_returns_data_when_element_present() {
    Fts3Hash h;
    // Initialize the hash with default key management (no copy, raw key)
    sqlite3Fts3HashInit(&h, 0 /* keyClass */, 0 /* copyKey */);

    // Prepare a stable data payload
    static const char value1[] = "VALUE_ONE";

    // Create a non-trivial key (binary data to exercise length-aware lookup)
    static const unsigned char key1[] = { 0xA1, 0xB2, 0xC3, 0xD4 };
    int nKey1 = 4;

    // Insert the element
    sqlite3Fts3HashInsert(&h, key1, nKey1, (void*)value1);

    // Find the same key
    void *found = sqlite3Fts3HashFind(&h, key1, nKey1);

    // Expect the exact data pointer
    TEST_ASSERT(found == (void*)value1, "sqlite3Fts3HashFind should return the data pointer for an existing key");

    // Cleanup
    sqlite3Fts3HashClear(&h);
}

// Test 2: When no element exists for the given key, sqlite3Fts3HashFind should return NULL.
static void test_find_returns_null_when_not_present() {
    Fts3Hash h;
    sqlite3Fts3HashInit(&h, 0, 0);

    // Key to look up (different from any inserted key)
    static const unsigned char key2[] = { 0x01, 0x02, 0x03 };
    int nKey2 = 3;

    // Ensure nothing is inserted
    void *found = sqlite3Fts3HashFind(&h, key2, nKey2);
    TEST_ASSERT(found == nullptr, "sqlite3Fts3HashFind should return NULL when key is not present");

    sqlite3Fts3HashClear(&h);
}

// Test 3: Multiple elements should be retrievable independently by their keys.
static void test_find_multiple_elements_independent() {
    Fts3Hash h;
    sqlite3Fts3HashInit(&h, 0, 0);

    static const char valueA[] = "ALPHA";
    static const char valueB[] = "BETA";

    static const unsigned char keyA[] = { 0x11, 0x22, 0x33 };
    static const unsigned char keyB[] = { 0x44, 0x55, 0x66, 0x77 };
    int nKeyA = 3;
    int nKeyB = 4;

    sqlite3Fts3HashInsert(&h, keyA, nKeyA, (void*)valueA);
    sqlite3Fts3HashInsert(&h, keyB, nKeyB, (void*)valueB);

    void *foundA = sqlite3Fts3HashFind(&h, keyA, nKeyA);
    void *foundB = sqlite3Fts3HashFind(&h, keyB, nKeyB);

    TEST_ASSERT(foundA == (void*)valueA, "Find for keyA should return valueA pointer");
    TEST_ASSERT(foundB == (void*)valueB, "Find for keyB should return valueB pointer");

    sqlite3Fts3HashClear(&h);
}

// Test 4: Inserting an element with NULL data should yield NULL on Find.
static void test_insert_null_data_returns_null_on_find() {
    Fts3Hash h;
    sqlite3Fts3HashInit(&h, 0, 0);

    static const unsigned char key[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE };
    int nKey = 5;

    sqlite3Fts3HashInsert(&h, key, nKey, NULL);

    void *found = sqlite3Fts3HashFind(&h, key, nKey);
    TEST_ASSERT(found == nullptr, "Find should return NULL when inserted data is NULL");

    sqlite3Fts3HashClear(&h);
}

// Test 5: Behavior with different key lengths to ensure nKey is respected.
static void test_find_respects_key_length() {
    Fts3Hash h;
    sqlite3Fts3HashInit(&h, 0, 0);

    static const char value = 'V';
    // Key data that are similar but have different lengths
    const unsigned char key_short[] = { 0x01, 0x02 };
    const unsigned char key_long[]  = { 0x01, 0x02, 0x03 };
    int nKeyShort = 2;
    int nKeyLong  = 3;

    sqlite3Fts3HashInsert(&h, key_short, nKeyShort, (void*)(&value));
    sqlite3Fts3HashInsert(&h, key_long,  nKeyLong,  (void*)(&value));

    void *foundShort = sqlite3Fts3HashFind(&h, key_short, nKeyShort);
    void *foundLong  = sqlite3Fts3HashFind(&h, key_long,  nKeyLong);

    TEST_ASSERT(foundShort != nullptr, "Find should succeed for short key");
    TEST_ASSERT(foundLong  != nullptr, "Find should succeed for long key");

    // Ensure that keys do not clash due to length ambiguity
    TEST_ASSERT(foundShort != foundLong, "Different key lengths should map to potentially different entries when data pointers are compared");

    sqlite3Fts3HashClear(&h);
}

// Entry point
int main() {
    // Run tests
    test_find_returns_data_when_element_present();
    test_find_returns_null_when_not_present();
    test_find_multiple_elements_independent();
    test_insert_null_data_returns_null_on_find();
    test_find_respects_key_length();

    // Print summary and return appropriate exit code
    print_summary();
    return g_failures ? 1 : 0;
}