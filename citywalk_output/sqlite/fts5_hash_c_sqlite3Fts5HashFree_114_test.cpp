/*
  Comprehensive C++11 unit tests for sqlite3Fts5HashFree focusing on its
  interaction with dependencies (sqlite3Fts5HashClear and sqlite3_free)
  using a lightweight, non-GTest approach.

  Test plan:
  - Test 1: Passing a nullptr does not invoke clear or frees.
  - Test 2: Non-null hash frees aSlot then the hash object, in proper order.
  - Test 3: Non-null hash with aSlot == nullptr frees only the hash after clear.

  Implementation notes:
  - We mock sqlite3Fts5HashClear and sqlite3_free to observe call counts
    and argument values without requiring the real SQLite runtime.
  - We allocate Fts5Hash and its aSlot with malloc to ensure matching
    free semantics in the mock (free is used for both allocations).
  - We access pHash->aSlot under the assumption that the Fts5Hash type exposes
    aSlot (as seen in the focal method). This mirrors the production behavior.
  - All tests use non-terminating assertions to allow full coverage in a single run.
*/

#include <iostream>
#include <cstdlib>
#include <vector>
#include <fts5Int.h>


// Attempt to include the production header to obtain Fts5Hash type.
// The actual path may differ in the real project; adjust as needed.
// Here we assume the file provides at least: typedef struct Fts5Hash { void *aSlot; } Fts5Hash;

// Forward declaration of the function under test with C linkage.
extern "C" void sqlite3Fts5HashFree(Fts5Hash *pHash);

// Globals to capture mock behavior
static int g_clear_calls = 0;
static Fts5Hash *g_last_clear_arg = nullptr;
static std::vector<void*> g_free_calls;

// Mock: sqlite3Fts5HashClear - records that it was invoked and with which hash
extern "C" void sqlite3Fts5HashClear(Fts5Hash *pHash) {
    ++g_clear_calls;
    g_last_clear_arg = pHash;
}

// Mock: sqlite3_free - records all freed pointers
extern "C" void sqlite3_free(void* ptr) {
    g_free_calls.push_back(ptr);
    // Do not actually free memory in order to keep test simple and deterministic
    // (we cannot safely know allocation method of all pointers in a real test fixture
    //  without deeper integration).
    // If desired for a more realistic test, one could track pointer types and call free(ptr)
    // for those known to be malloc'ed. We avoid that here to prevent UB with "new" allocations.
    // Note: We still push the pointer to verify correct frees in tests.
}

// Simple non-terminating assertion macro suitable for C++ without a test framework
static int g_tests_failed = 0;
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test assertion failed: " << (msg) \
                  << " (line " << __LINE__ << ")" << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)

static void reset_mocks() {
    g_clear_calls = 0;
    g_last_clear_arg = nullptr;
    g_free_calls.clear();
}

// Test 1: Freeing a nullptr should be a no-op (no calls to clear or free)
static void test_sqlite3Fts5HashFree_null() {
    reset_mocks();

    // Call the function under test with nullptr
    sqlite3Fts5HashFree(nullptr);

    // Assertions: no calls should have happened
    TEST_ASSERT(g_clear_calls == 0, "sqlite3Fts5HashClear should not be called for null input");
    TEST_ASSERT(g_free_calls.empty(), "sqlite3_free should not be called for null input");
}

// Test 2: Freeing a valid hash should invoke clear, then free aSlot and the hash itself
static void test_sqlite3Fts5HashFree_nonNull() {
    reset_mocks();

    // Allocate a hash object and its aSlot
    Fts5Hash *pHash = (Fts5Hash*)std::malloc(sizeof(Fts5Hash));
    TEST_ASSERT(pHash != nullptr, "Failed to allocate Fts5Hash");
    // Allocate aSlot memory
    void *slotMem = std::malloc(128);
    TEST_ASSERT(slotMem != nullptr, "Failed to allocate aSlot memory");
    pHash->aSlot = slotMem;

    // Call the function under test
    sqlite3Fts5HashFree(pHash);

    // Assertions: one clear call with pHash, two frees (slotMem and pHash)
    TEST_ASSERT(g_clear_calls == 1, "sqlite3Fts5HashClear should be called exactly once");
    TEST_ASSERT(g_last_clear_arg == pHash, "sqlite3Fts5HashClear should be called with the original hash pointer");
    TEST_ASSERT(g_free_calls.size() == 2, "sqlite3_free should be called exactly twice");

    TEST_ASSERT(g_free_calls[0] == slotMem, "First sqlite3_free should free aSlot memory");
    TEST_ASSERT(g_free_calls[1] == pHash, "Second sqlite3_free should free the hash object");
}

// Test 3: Freeing a non-null hash with aSlot == nullptr should still free the hash after clear
static void test_sqlite3Fts5HashFree_nonNull_slotNull() {
    reset_mocks();

    Fts5Hash *pHash = (Fts5Hash*)std::malloc(sizeof(Fts5Hash));
    TEST_ASSERT(pHash != nullptr, "Failed to allocate Fts5Hash");
    pHash->aSlot = nullptr;  // Explicitly test null aSlot

    sqlite3Fts5HashFree(pHash);

    TEST_ASSERT(g_clear_calls == 1, "sqlite3Fts5HashClear should be called exactly once");
    TEST_ASSERT(g_last_clear_arg == pHash, "sqlite3Fts5HashClear should be called with the original hash pointer");
    TEST_ASSERT(g_free_calls.size() == 2, "sqlite3_free should be called exactly twice even if aSlot is null");

    TEST_ASSERT(g_free_calls[0] == nullptr, "First sqlite3_free should be called with nullptr (empty aSlot)");
    TEST_ASSERT(g_free_calls[1] == pHash, "Second sqlite3_free should free the hash object");
}

// Entry point: run all tests
int main() {
    // Ensure C linkage expectations are clear (no remaining tests rely on global state)
    test_sqlite3Fts5HashFree_null();
    test_sqlite3Fts5HashFree_nonNull();
    test_sqlite3Fts5HashFree_nonNull_slotNull();

    if(g_tests_failed == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << g_tests_failed << " test(s) failed." << std::endl;
        return 1;
    }
}