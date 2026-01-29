// High-quality C++11 unit test suite for sqlite3SetString (malloc.c) without using GTest.
// This test harness provides lightweight expectations that do not terminate on failure.
// It includes minimal in-file implementations of the dependent sqlite3 internals to allow compiling
// and exercising the focal function in isolation.

#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <unordered_set>
#include <iostream>


// Step 1: Candidate Keywords (extracted from the focal method and dependencies)
// - sqlite3SetString
// - sqlite3DbStrDup
// - sqlite3DbFree
// - sqlite3 *db
// - char **pz
// - const char *zNew
// - Memory management and ownership transfer

// Step 2/3: Lightweight test harness with in-file stubs for dependencies
// - We implement a minimal sqlite3 struct
// - We provide in-file implementations for sqlite3SetString, sqlite3DbStrDup and sqlite3DbFree
// - We track allocations and frees to verify correct memory management

// Minimal sqlite3 structure for test purposes
struct sqlite3 {
    int dummy;
};

// Memory tracking for allocations performed by sqlite3DbStrDup
static std::unordered_set<void*> g_allocated;
static std::unordered_set<void*> g_freed;

// Helper to free all currently allocated blocks (to avoid leaks between tests)
static void freeAllAllocated() {
    for (auto p : g_allocated) {
        delete[] reinterpret_cast<char*>(p);
    }
    g_allocated.clear();
}

// sqlite3DbStrDup: duplicates a C-string into a newly allocated buffer
char* sqlite3DbStrDup(sqlite3 *db, const char *z) {
    (void)db; // unused in test stub
    if (!z) return nullptr;
    size_t n = std::strlen(z);
    char* copy = new char[n + 1];
    std::memcpy(copy, z, n + 1);
    g_allocated.insert(static_cast<void*>(copy));
    return copy;
}

// sqlite3DbFree: frees a previously allocated string
void sqlite3DbFree(sqlite3 *db, void *p) {
    (void)db; // unused in test stub
    if (p == nullptr) return;
    // Mark as freed if it was allocated by sqlite3DbStrDup
    auto it = g_allocated.find(p);
    if (it != g_allocated.end()) {
        g_freed.insert(p);
        g_allocated.erase(it);
    }
    delete[] reinterpret_cast<char*>(p);
}

// The focal method under test (copied exactly as provided)
void sqlite3SetString(char **pz, sqlite3 *db, const char *zNew){
  char *z = sqlite3DbStrDup(db, zNew);
  sqlite3DbFree(db, *pz);
  *pz = z;
}

// Test harness helpers

static int g_failures = 0;

// Simple non-terminating expectation
#define EXPECT_TRUE(cond, msg)                                                      \
    do {                                                                            \
        if (!(cond)) {                                                              \
            std::cerr << "[FAIL] " << (msg) << " (line " << __LINE__ << ")\n";     \
            ++g_failures;                                                           \
        }                                                                           \
    } while (0)

#define EXPECT_EQ_STR(a, b, msg)                                                   \
    do {                                                                           \
        const char* sa = (a);                                                      \
        const char* sb = (b);                                                      \
        if (sa == nullptr && sb == nullptr) { /* both null -> equal */               \
        } else if (!(sa && sb && std::strcmp(sa, sb) == 0)) {                      \
            std::cerr << "[FAIL] " << (msg) << " (line " << __LINE__ << "): "      \
                      << "expected \"" << (sb ? sb : "nullptr")                     \
                      << "\", got \"" << (sa ? sa : "nullptr") << "\"\n";          \
            ++g_failures;                                                          \
        }                                                                          \
    } while (0)

static void memoryStateReset() {
    // Free all allocated blocks and reset trackers
    freeAllAllocated();
    g_freed.clear();
}

// Test 1: Basic allocation and assignment when old pointer is NULL
bool test_SetString_BasicAllocatesNewString() {
    memoryStateReset();

    sqlite3 db;
    char *pz = nullptr;

    sqlite3SetString(&pz, &db, "hello");

    // Expectation: pz points to a newly allocated string "hello"
    EXPECT_TRUE(pz != nullptr, "pz should be non-null after SetString");
    EXPECT_EQ_STR(pz, "hello", "pz content should be 'hello' after SetString");

    // No old pointer to free, so allocated count should be 1 (the new allocation)
    EXPECT_TRUE(g_allocated.size() == 1, "one allocation should be tracked");
    EXPECT_TRUE(g_freed.find(pz) == g_freed.end(), "pz should not be marked as freed");

    // Cleanup
    memoryStateReset();
    return g_failures == 0;
}

// Test 2: Replacing an existing string should free the old one and allocate a new one
bool test_SetString_ReplaceOldString_freesOld() {
    memoryStateReset();

    sqlite3 db;

    // Create an initial string "old"
    char *oldP = sqlite3DbStrDup(&db, "old");
    // Now p points to oldP
    char *p = oldP;

    // Replace with "new"
    sqlite3SetString(&p, &db, "new");

    // Expectations:
    // - p points to "new"
    // - oldP has been freed
    EXPECT_TRUE(p != nullptr, "pz after replacement should be non-null");
    EXPECT_EQ_STR(p, "new", "pz content should be 'new' after SetString");
    EXPECT_TRUE(g_freed.find(oldP) != g_freed.end(), "old pointer should be freed");

    // Ensure oldP and p are distinct
    EXPECT_TRUE(p != oldP, "new allocation should not reuse old pointer");

    // Cleanup
    memoryStateReset();
    return g_failures == 0;
}

// Test 3: Replacing when there is no existing allocation (NULL old pointer)
bool test_SetString_WithNullOldPointer() {
    memoryStateReset();

    sqlite3 db;
    char *p = nullptr;

    sqlite3SetString(&p, &db, "abc");

    EXPECT_TRUE(p != nullptr, "pz should be non-null after SetString");
    EXPECT_EQ_STR(p, "abc", "pz content should be 'abc'");

    memoryStateReset();
    return g_failures == 0;
}

// Test 4: Empty string handling
bool test_SetString_EmptyString() {
    memoryStateReset();

    sqlite3 db;
    char *p = nullptr;

    sqlite3SetString(&p, &db, "");

    EXPECT_TRUE(p != nullptr, "pz should be non-null after SetString with empty string");
    EXPECT_EQ_STR(p, "", "pz content should be empty string");

    memoryStateReset();
    return g_failures == 0;
}

// Runner
int main() {
    std::cout << "Starting sqlite3SetString unit tests (C++11, no GTest)" << std::endl;

    bool ok = true;
    ok &= test_SetString_BasicAllocatesNewString();
    ok &= (test_SetString_ReplaceOldString_freesOld() == true);
    ok &= test_SetString_WithNullOldPointer();
    ok &= test_SetString_EmptyString();

    if (g_failures == 0 && ok) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << "Tests finished with " << g_failures << " failure(s)." << std::endl;
        return 1;
    }
}