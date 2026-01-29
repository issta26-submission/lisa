// Test suite for sqlite3Fts3HashClear (C function) using a C++11 test harness (no GTest).
// The goal is to exercise key branches of sqlite3Fts3HashClear by constructing
// Fts3Hash and its elements, invoking the function, and asserting post-conditions
// without terminating the test on assertion failures (non-terminating checks).

#include <fts3Int.h>
#include <stdlib.h>
#include <string.h>
#include <fts3_hash.h>
#include <assert.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


#ifdef __cplusplus
extern "C" {
#endif

// Prototypes and basic types are assumed to be provided by the project headers.
// Include the actual header to access Fts3Hash, Fts3HashElem and the function.

#ifdef __cplusplus
}
#endif

// Lightweight test harness (non-terminating assertions).
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if(!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "EXPECT_TRUE FAILED: " << (msg) \
                  << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
    } \
} while(0)

#define EXPECT_EQ_INT(a, b, msg) do { \
    ++g_tests_run; \
    if((a) != (b)) { \
        ++g_tests_failed; \
        std::cerr << "EXPECT_EQ_INT FAILED: " << (msg) \
                  << " - expected: " << (b) << ", actual: " << (a) \
                  << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
    } \
} while(0)

// Helper to create a single hash element
static Fts3HashElem* make_elem(void* key, Fts3HashElem* next) {
    Fts3HashElem* e = (Fts3HashElem*)std::malloc(sizeof(Fts3HashElem));
    if(!e) return nullptr;
    e->pKey = key;
    e->next = next;
    return e;
}

// Helpers to allocate keys with simple payload
static void* alloc_key(size_t n) {
    void* p = std::malloc(n);
    if(p && n>0) std::memset(p, 0xA5, n); // fill with visible pattern
    return p;
}

// Helper to cleanly release hash object memory (if function freed it, this is a no-op)
static void free_memory(void* p) {
    if(p) std::free(p);
}

// Test 1: CopyKey = true, two elements with non-null keys.
// This exercises the path: if(pH->copyKey && elem->pKey) { fts3HashFree(elem->pKey); }
static void test_Clear_WithElements_CopyKeyTrue_TwoElements() {
    // Allocate and initialize hash struct
    Fts3Hash* pH = (Fts3Hash*)std::malloc(sizeof(Fts3Hash));
    if(!pH) {
        std::cerr << "Memory allocation failed for pH in test 1\n";
        return;
    }
    // Create two elements with non-null keys
    Fts3HashElem* e2 = make_elem(alloc_key(16), nullptr);
    Fts3HashElem* e1 = make_elem(alloc_key(16), e2);

    // Initialize Fts3Hash fields
    pH->first = e1;
    pH->ht = NULL;       // Avoid extra frees for internal hash table
    pH->htsize = 0;
    pH->copyKey = 1;     // true
    pH->count = 2;         // non-zero before clear (will be reset to 0)

    // Invoke focal method
    sqlite3Fts3HashClear(pH);

    // Post-conditions
    EXPECT_TRUE(pH->first == nullptr, "First element list should be NULL after clear");
    EXPECT_TRUE(pH->ht == nullptr, "Hash table should be NULL after clear");
    EXPECT_EQ_INT(pH->htsize, 0, "htsize should be reset to 0");
    EXPECT_EQ_INT(pH->count, 0, "count should be reset to 0");

    // Clean up the Fts3Hash struct itself
    std::free(pH);
    // Note: The elements and their keys should have been freed by sqlite3Fts3HashClear.
    // We do not attempt to access them after the call.
}

// Test 2: CopyKey = false, two elements with non-null keys.
// This exercises the false branch: if(pH->copyKey && elem->pKey) is skipped.
static void test_Clear_WithElements_CopyKeyFalse_TwoElements() {
    Fts3Hash* pH = (Fts3Hash*)std::malloc(sizeof(Fts3Hash));
    if(!pH) {
        std::cerr << "Memory allocation failed for pH in test 2\n";
        return;
    }

    Fts3HashElem* e2 = make_elem(alloc_key(8), nullptr);
    Fts3HashElem* e1 = make_elem(alloc_key(8), e2);

    pH->first = e1;
    pH->ht = NULL;
    pH->htsize = 0;
    pH->copyKey = 0;     // false
    pH->count = 2;

    sqlite3Fts3HashClear(pH);

    // Post-conditions
    EXPECT_TRUE(pH->first == nullptr, "First element list should be NULL after clear (copyKey=false)");
    EXPECT_TRUE(pH->ht == nullptr, "Hash table should be NULL after clear");
    EXPECT_EQ_INT(pH->htsize, 0, "htsize should be reset to 0");
    EXPECT_EQ_INT(pH->count, 0, "count should be reset to 0");

    std::free(pH);
    // e1/e2 and their key blocks are freed by the clear call when copyKey is false
    // (no explicit verification possible here without instrumentation of free).
}

// Test 3: One element with NULL pKey, and another with non-null key, CopyKey = true.
// This ensures we skip freeing NULL keys but still free existing ones.
static void test_Clear_WithElements_PKeyNullAndNonNull_CopyKeyTrue() {
    Fts3Hash* pH = (Fts3Hash*)std::malloc(sizeof(Fts3Hash));
    if(!pH) {
        std::cerr << "Memory allocation failed for pH in test 3\n";
        return;
    }

    Fts3HashElem* e2 = make_elem(alloc_key(12), nullptr);
    Fts3HashElem* e1 = make_elem(nullptr, e2); // e1 has NULL pKey

    pH->first = e1;
    pH->ht = NULL;
    pH->htsize = 0;
    pH->copyKey = 1;     // true
    pH->count = 2;

    sqlite3Fts3HashClear(pH);

    // Post-conditions
    EXPECT_TRUE(pH->first == nullptr, "First element list should be NULL after clear (pKey NULL case)");
    EXPECT_TRUE(pH->ht == nullptr, "Hash table should be NULL after clear");
    EXPECT_EQ_INT(pH->htsize, 0, "htsize should be reset to 0");
    EXPECT_EQ_INT(pH->count, 0, "count should be reset to 0");

    std::free(pH);
    // e1 (with NULL pKey) and e2 (with non-null pKey) are freed by clear; e1->pKey is NULL, so nothing to free there.
}

// Test 4: Empty hash (no elements). Verifies that function handles empty chains gracefully.
static void test_Clear_EmptyHash() {
    Fts3Hash* pH = (Fts3Hash*)std::malloc(sizeof(Fts3Hash));
    if(!pH) {
        std::cerr << "Memory allocation failed for pH in test 4\n";
        return;
    }

    pH->first = nullptr;
    pH->ht = NULL;
    pH->htsize = 0;
    pH->copyKey = 1;
    pH->count = 0;

    sqlite3Fts3HashClear(pH);

    // Post-conditions
    EXPECT_TRUE(pH->first == nullptr, "First should remain NULL after clear on empty hash");
    EXPECT_TRUE(pH->ht == nullptr, "HT should remain NULL after clear on empty hash");
    EXPECT_EQ_INT(pH->htsize, 0, "htsize should remain 0");
    EXPECT_EQ_INT(pH->count, 0, "count should remain 0");

    std::free(pH);
}

// Runner
int main() {
    std::cout << "Running sqlite3Fts3HashClear test suite...\n";

    test_Clear_WithElements_CopyKeyTrue_TwoElements();
    test_Clear_WithElements_CopyKeyFalse_TwoElements();
    test_Clear_WithElements_PKeyNullAndNonNull_CopyKeyTrue();
    test_Clear_EmptyHash();

    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << "\n";

    if(g_tests_failed != 0) {
        std::cerr << "Some tests failed. Please examine the error messages above.\n";
    } else {
        std::cout << "All tests passed (non-terminating assertions used for checks).\n";
    }

    // Return non-zero if any test failed
    return g_tests_failed;
}