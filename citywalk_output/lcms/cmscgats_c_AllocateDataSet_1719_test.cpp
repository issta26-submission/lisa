// Test suite for the focal method AllocateDataSet in cmscgats.c
// This test harness follows the constraints described in the prompt:
// - No GTest/GMock; a lightweight in-process test runner is used.
// - Uses the library's native APIs to exercise AllocateDataSet.
// - Provides 3 test cases covering important branches of AllocateDataSet.
// - Uses non-terminating assertions style (collects failures and reports at end).

#include <lcms2_internal.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>


// Domain knowledge hints: use the library's internal types and functions.
// We assume the environment provides cmsBool, TRUE/FALSE, cmsIT8, TABLE, GetTable,
// cmsIT8Alloc, cmsIT8Free, cmsIT8GetProperty, AllocChunk, and AllocateDataSet
// as visible symbols (as in the provided code excerpt).

extern "C" {
    // Forward declarations (these are expected to be provided by the CMS/Little CMS headers
    // in the actual project). If your include path provides real declarations, you can
    // remove these stubs; otherwise rely on the real ones from the project.
    typedef int cmsBool;
    const int TRUE = 1;
    const int FALSE = 0;

    typedef void* cmsHANDLE;
    typedef struct TABLE_TABLE* TABLE; // Placeholder for actual TABLE type
    typedef struct cmsIT8_s cmsIT8;

    // Real project provides:
    // cmsBool AllocateDataSet(cmsIT8* it8);
    cmsBool AllocateDataSet(cmsIT8* it8);

    // Real project provides helpers:
    cmsIT8* cmsIT8Alloc(void* ContextID);
    void cmsIT8Free(cmsHANDLE h);

    TABLE* GetTable(cmsIT8* it8);
    const char* cmsIT8GetProperty(cmsIT8* it8, const char* property);

    // The below are used in the existing code for memory allocation helper.
    // If your build uses different signatures, adjust accordingly.
    void* AllocChunk(cmsIT8* it8, size_t size);
}

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { \
            ++g_failed_tests; \
            std::cerr << "[FAILED] " msg << std::endl; \
        } else { \
            std::cout << "[PASSED] " msg << std::endl; \
        } \
    } while (0)

#define EXPECT_FALSE(cond, msg) \
    do { \
        ++g_total_tests; \
        if (cond) { \
            ++g_failed_tests; \
            std::cerr << "[FAILED] " msg << std::endl; \
        } else { \
            std::cout << "[PASSED] " msg << std::endl; \
        } \
    } while (0)

#define EXPECT_NOT_NULL(ptr, msg) \
    do { \
        ++g_total_tests; \
        if ((ptr) == nullptr) { \
            ++g_failed_tests; \
            std::cerr << "[FAILED] " msg << std::endl; \
        } else { \
            std::cout << "[PASSED] " msg << std::endl; \
        } \
    } while (0)


// Helper to convert to needed types if necessary (no-op on most platforms)
static inline void expect_null_or_valid_ptr(void* p) { (void)p; }

// Test 1: Data already allocated should short-circuit and return TRUE without allocating again
// This exercises the early-return path: if (t->Data) return TRUE;
void test_allocate_already_allocated() {
    std::cout << "Running test_allocate_already_allocated..." << std::endl;

    cmsIT8* it8 = cmsIT8Alloc(NULL);
    EXPECT_NOT_NULL(it8, "AllocateDataSet test: cmsIT8Alloc failed to create it8.");
    if (it8 == nullptr) return;

    TABLE* t = GetTable(it8);
    if (t == nullptr) {
        std::cerr << "[WARNING] TABLE pointer is NULL. The test environment may differ from expectations." << std::endl;
        cmsIT8Free((cmsHANDLE)it8);
        return;
    }

    // Simulate the "already allocated" state
    t->Data = (char**)1; // Any non-NULL value signals "allocated" in the production code
    int beforeDataPtr = (int)(t->Data);

    cmsBool res = AllocateDataSet(it8);

    EXPECT_TRUE(res == TRUE, "AllocateDataSet should return TRUE when Data is already allocated.");
    // Ensure no crash and the Data pointer remains in a non-NULL state
    EXPECT_TRUE(t->Data != NULL, "Data pointer should remain non-NULL after early return.");

    // Cleanup
    cmsIT8Free((cmsHANDLE)it8);
    // Note: In a real test, ensure t->Data memory integrity is preserved or freed if allocated.
}

// Test 2: Normal allocation path with valid numbers of fields/sets
// Expect t->nSamples and t->nPatches to be used to allocate Data and for t->Data to be non-NULL afterwards
void test_allocate_normal_path() {
    std::cout << "Running test_allocate_normal_path..." << std::endl;

    cmsIT8* it8 = cmsIT8Alloc(NULL);
    EXPECT_NOT_NULL(it8, "AllocateDataSet test: cmsIT8Alloc failed to create it8.");
    if (it8 == nullptr) return;

    TABLE* t = GetTable(it8);
    if (t == nullptr) {
        std::cerr << "[WARNING] TABLE pointer is NULL. The test environment may differ from expectations." << std::endl;
        cmsIT8Free((cmsHANDLE)it8);
        return;
    }

    // Ensure Data is not pre-allocated
    t->Data = NULL;
    // Choose safe, valid values within the allowed ranges
    t->nSamples = 2;
    t->nPatches = 3;
    // Sanity: ensure product is within the 200000 limit (4*5=20 <= 200000)
    int maxEntries = t->nSamples * t->nPatches;
    (void)maxEntries; // suppress unused warning if not used elsewhere

    cmsBool res = AllocateDataSet(it8);
    EXPECT_TRUE(res == TRUE, "AllocateDataSet should return TRUE for valid dataset sizes.");
    EXPECT_NOT_NULL(t->Data, "Data array should be allocated when sizes are valid.");

    // Cleanup
    cmsIT8Free((cmsHANDLE)it8);
}

// Test 3: Data size too large should trigger SynError and return FALSE
void test_allocate_too_large() {
    std::cout << "Running test_allocate_too_large..." << std::endl;

    cmsIT8* it8 = cmsIT8Alloc(NULL);
    EXPECT_NOT_NULL(it8, "AllocateDataSet test: cmsIT8Alloc failed to create it8.");
    if (it8 == nullptr) return;

    TABLE* t = GetTable(it8);
    if (t == nullptr) {
        std::cerr << "[WARNING] TABLE pointer is NULL. The test environment may differ from expectations." << std::endl;
        cmsIT8Free((cmsHANDLE)it8);
        return;
    }

    // Ensure Data is not pre-allocated
    t->Data = NULL;
    // Trigger the "too much data" predicate
    t->nSamples = 0x7fff;  // 32767, which is > 0x7ffe (32766)
    t->nPatches = 1;

    cmsBool res = AllocateDataSet(it8);
    EXPECT_FALSE(res == TRUE, "AllocateDataSet should return FALSE when data size exceeds limits.");

    // Cleanup
    cmsIT8Free((cmsHANDLE)it8);
}

// Runner
int main() {
    std::cout << "Starting AllocateDataSet test suite (no GTest)..." << std::endl;

    test_allocate_already_allocated();
    test_allocate_normal_path();
    test_allocate_too_large();

    std::cout << "Test suite finished. "
              << "Total: " << g_total_tests
              << ", Passed: " << (g_total_tests - g_failed_tests)
              << ", Failed: " << g_failed_tests
              << std::endl;

    return g_failed_tests == 0 ? 0 : 1;
}