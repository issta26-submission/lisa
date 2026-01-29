// Test suite for the focal method sqlite3WhereRealloc
// This test suite is implemented in plain C++11 (no GTest) and uses simple
// non-terminating assertions via test result reporting to maximize code coverage.
// The tests are designed to run with a self-contained minimal replica of the
// dependent environment (WhereInfo, WhereMemBlock) and a local stub of
// sqlite3WhereMalloc used by sqlite3WhereRealloc.
//
// The goal is to verify:
// - True branch: when both pOld and pNew are non-null, a memory block is copied
// - False branch: when pOld is null, no copy occurs and a new block is allocated
// - False branch (allocation failure): when sqlite3WhereMalloc fails (returns null),
//   sqlite3WhereRealloc returns null and does not dereference pOld
//
// Important: This is a minimal reproducer focusing on the focal method's logic.

#include <whereInt.h>
#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <cstdlib>
#include <cassert>


// Domain-specific types (minimal stubs for testing)
using u64 = unsigned long long;

struct WhereInfo {};        // Minimal placeholder for test purposes

// Block header structure used by sqlite3WhereRealloc to determine the previous block size.
// The test constructs memory so that the header sits immediately before the user data.
struct WhereMemBlock {
    size_t sz;                 // Size of the previous allocation (old data size)
    // Real SQLite has additional fields; for test purposes only sz is needed.
};

// Global flag to simulate sqlite3WhereMalloc failure in tests
static bool gNextMallocShouldFail = false;

// Stubbed memory allocator used by sqlite3WhereRealloc
extern "C" void *sqlite3WhereMalloc(WhereInfo *pWInfo, u64 nByte) {
    if (gNextMallocShouldFail) {
        return nullptr;
    }
    return std::malloc(static_cast<size_t>(nByte));
}

// Focal method under test (replicated here for a self-contained test harness)
extern "C" void *sqlite3WhereRealloc(WhereInfo *pWInfo, void *pOld, u64 nByte) {
    void *pNew = sqlite3WhereMalloc(pWInfo, nByte);
    if (pNew && pOld) {
        WhereMemBlock *pOldBlk = (WhereMemBlock*)pOld;
        pOldBlk--;                            // Move back to the header preceding the data
        assert(pOldBlk->sz < nByte);          // Ensure old size fits into new allocation
        std::memcpy(pNew, pOld, pOldBlk->sz);
    }
    return pNew;
}

// Simple test harness utilities
static int gFailures = 0;

#define TEST_PASS(name) \
    printf("PASS: %s\n", name)

#define TEST_FAIL(name, msg) \
    do { \
        printf("FAIL: %s - %s\n", name, msg); \
        ++gFailures; \
    } while(0)

static void test_copy_when_both_non_null() {
    // Test that when both pNew and pOld are non-null, the previous block's data
    // is copied to the new allocation.
    const char testName[] = "test_copy_when_both_non_null";

    // Old allocation layout:
    // [WhereMemBlock header][old data bytes]
    const size_t oldSize = 8;
    const size_t newSize = 32;

    // Prepare header with oldSize
    WhereMemBlock header{oldSize};

    // Allocate contiguous memory: header + old data
    void *buffer = std::malloc(sizeof(WhereMemBlock) + oldSize);
    if (!buffer) {
        TEST_FAIL(testName, "memory allocation for old block failed");
        return;
    }
    // Place header at start
    std::memcpy(buffer, &header, sizeof(WhereMemBlock));
    // Place old data immediately after header
    unsigned char *oldData = (unsigned char*)buffer + sizeof(WhereMemBlock);
    for (size_t i = 0; i < oldSize; ++i) {
        oldData[i] = static_cast<unsigned char>(i + 1); // 1,2,3,...,8
    }

    void *pOld = (void*)oldData; // Pointer to the data portion (as expected by function)

    // Ensure malloc does not fail
    gNextMallocShouldFail = false;
    void *pNew = sqlite3WhereRealloc(nullptr, pOld, newSize);

    // Validation
    bool pass = true;
    if (!pNew) {
        pass = false;
        TEST_FAIL(testName, "pNew is null although pOld is non-null and malloc succeeds");
    } else {
        // The first oldSize bytes should have been copied to pNew
        if (std::memcmp(pNew, pOld, oldSize) != 0) {
            pass = false;
            TEST_FAIL(testName, "data was not copied correctly from pOld to pNew");
        }
    }

    // Cleanup
    std::free(buffer);
    if (pNew) {
        std::free(pNew);
    }

    if (pass) TEST_PASS(testName);
}

static void test_no_copy_when_old_null() {
    // Test that when pOld is null, sqlite3WhereRealloc returns a non-null pNew
    // but does not attempt to copy data (the if(pNew && pOld) block is skipped).
    const char testName[] = "test_no_copy_when_old_null";

    const size_t newSize = 16;

    // No old buffer needed since pOld will be null
    void *pOld = nullptr;

    gNextMallocShouldFail = false;
    void *pNew = sqlite3WhereRealloc(nullptr, pOld, newSize);

    if (!pNew) {
        TEST_FAIL(testName, "pNew is null when pOld is null but malloc succeeds");
    } else {
        TEST_PASS(testName);
        std::free(pNew);
    }
}

static void test_allocation_failure_results_in_null() {
    // Test that when sqlite3WhereMalloc fails (returns null), sqlite3WhereRealloc
    // returns null and does not dereference pOld.
    const char testName[] = "test_allocation_failure_results_in_null";

    // Prepare a valid old block to ensure the branch where pOld is non-null is exercised
    const size_t oldSize = 8;
    const size_t newSize = 32;

    WhereMemBlock header{oldSize};
    void *buffer = std::malloc(sizeof(WhereMemBlock) + oldSize);
    if (!buffer) {
        TEST_FAIL(testName, "memory allocation for old block failed");
        return;
    }
    std::memcpy(buffer, &header, sizeof(WhereMemBlock));
    unsigned char *oldData = (unsigned char*)buffer + sizeof(WhereMemBlock);
    for (size_t i = 0; i < oldSize; ++i) {
        oldData[i] = static_cast<unsigned char>(i + 10);
    }

    void *pOld = (void*)oldData;

    // Simulate allocation failure
    gNextMallocShouldFail = true;
    void *pNew = sqlite3WhereRealloc(nullptr, pOld, newSize);

    // Validation
    if (pNew != nullptr) {
        TEST_FAIL(testName, "expected malloc failure to yield null, but got non-null pNew");
        std::free(pNew);
    } else {
        TEST_PASS(testName);
    }

    // Cleanup
    gNextMallocShouldFail = false;
    std::free(buffer);
}

int main() {
    // Run all tests
    test_copy_when_both_non_null();
    test_no_copy_when_old_null();
    test_allocation_failure_results_in_null();

    // Summary
    if (gFailures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("TOTAL FAILURES: %d\n", gFailures);
        return 1;
    }
}