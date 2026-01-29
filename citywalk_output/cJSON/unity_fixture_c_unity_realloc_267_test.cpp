/*
 * unity_realloc_tests.cpp
 *
 * A lightweight C++11 test suite for the focal method:
 *     void* unity_realloc(void* oldMem, size_t size)
 *
 * The tests exercise the behavior of unity_realloc by allocating and
 * reusing memory through Unity's standard allocation path (unity_malloc/unity_free),
 * without relying on a GTest framework.
 *
 * Notes:
 * - The tests use extern "C" declarations to call the C-based Unity memory API.
 * - The tests rely on Unity's malloc/frees and its internal behavior for guards
 *   (the Guard header is created internally by unity_malloc).
 * - We intentionally cover:
 *   1) NULL oldMem path (allocates via unity_malloc),
 *   2) shrinking (guard->size >= requested size) returns same pointer,
 *   3) expansion (data is preserved on copy to new memory),
 *   4) size == 0 (frees old block, returns NULL),
 *   5) malloc failure during realloc (old block remains intact).
 *
 * This file is intended to be compiled with a C++11 capable compiler and linked
 * against the Unity framework (unity_fixture.c/.h etc) in a C project layout.
 *
 * Explanatory comments accompany each unit test.
 */


#include <unity_internals.h>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <unity_fixture.h>


// Import Unity C API (memory management) with C linkage
extern "C" {
    // Focal method under test
    void* unity_realloc(void* oldMem, size_t size);

    // Basic memory allocation helpers used by Unity
    void* unity_malloc(size_t size);
    void  unity_free(void* mem);

    // Utility to simulate malloc failure for testing realloc failure path
    void UnityMalloc_MakeMallocFailAfterCount(int countdown);
}

/*
 * Test 1: realloc with NULL oldMem should allocate new memory of requested size.
 * Expected: returns a non-NULL pointer allocated via unity_malloc.
 */
static void test_unity_realloc_null_oldmem_allocates(void)
{
    printf("Test 1: unity_realloc(NULL, 64) should allocate new memory...\n");

    void* p = unity_realloc(nullptr, 64);
    assert(p != nullptr);    // realloc should allocate new memory when oldMem is NULL

    // Basic sanity: write/read to ensure allocated block is usable
    unsigned char* data = (unsigned char*)p;
    data[0] = 0xAA;
    data[63] = 0xBB;

    // Clean up
    unity_free(p);

    printf("Test 1 passed.\n");
}

/*
 * Test 2: Shrinking an existing allocation should return the same pointer
 * and not move memory. This exercises the path: if (guard->size >= size) return oldMem;
 */
static void test_unity_realloc_shrink_returns_same_ptr(void)
{
    printf("Test 2: Realloc shrinking should return same pointer...\n");

    size_t oldSize = 64;
    void* p = unity_malloc(oldSize);
    assert(p != nullptr);

    // Initialize data
    unsigned char* payload = (unsigned char*)p;
    for (size_t i = 0; i < oldSize; ++i) payload[i] = (unsigned char)(i & 0xFF);

    void* r = unity_realloc(p, 32); // shrink to 32
    assert(r == p);                 // same pointer expected

    // Validate that existing data remains (at least for the first 32 bytes)
    for (size_t i = 0; i < 32; ++i)
        assert(payload[i] == (unsigned char)(i & 0xFF));

    // Clean up the memory (r and p are the same)
    unity_free(r);

    printf("Test 2 passed.\n");
}

/*
 * Test 3: Expanding an allocation should allocate new memory, copy old data,
 * and release the old block. The new memory should contain the old bytes at start.
 */
static void test_unity_realloc_expand_copies_and_releases_old(void)
{
    printf("Test 3: Realloc expanding should copy old data and release old block...\n");

    size_t oldSize = 64;
    void* p = unity_malloc(oldSize);
    assert(p != nullptr);

    unsigned char* oldData = (unsigned char*)p;
    for (size_t i = 0; i < oldSize; ++i) oldData[i] = (unsigned char)(i & 0xFF);

    // Save a copy of the original data for verification after realloc
    unsigned char savedOldData[64];
    memcpy(savedOldData, oldData, oldSize);

    size_t newSize = 128;
    void* newMem = unity_realloc(p, newSize);
    assert(newMem != nullptr);
    assert(newMem != p); // should be a new block

    unsigned char* newData = (unsigned char*)newMem;
    // First oldSize bytes should match the original data
    for (size_t i = 0; i < oldSize; ++i)
        assert(newData[i] == savedOldData[i]);

    // Clean up the new memory
    unity_free(newMem);

    printf("Test 3 passed.\n");
}

/*
 * Test 4: Realloc with size 0 should free old memory and return NULL.
 */
static void test_unity_realloc_zero_frees_and_returns_null(void)
{
    printf("Test 4: Realloc with size 0 should free old block and return NULL...\n");

    size_t oldSize = 32;
    void* p = unity_malloc(oldSize);
    assert(p != nullptr);

    unsigned char* data = (unsigned char*)p;
    data[0] = 0x11;
    data[31] = 0x22;

    void* r = unity_realloc(p, 0);
    assert(r == nullptr); // per contract, should return NULL

    // Ensure we can still allocate new memory afterwards (sanity check)
    void* q = unity_malloc(16);
    assert(q != nullptr);
    unity_free(q);

    // Note: old block has been freed; no further asserts on its content are possible

    printf("Test 4 passed.\n");
}

/*
 * Test 5: If malloc fails during realloc, the old memory must NOT be freed.
 * We force a failure via UnityMalloc_MakeMallocFailAfterCount.
 */
static void test_unity_realloc_malloc_fail_does_not_free_old(void)
{
    printf("Test 5: Realloc should not free old block if allocation fails...\n");

    size_t oldSize = 128;
    void* p = unity_malloc(oldSize);
    assert(p != nullptr);

    unsigned char* data = (unsigned char*)p;
    for (size_t i = 0; i < oldSize; ++i) data[i] = (unsigned char)(i & 0xFF);

    // Force the next malloc to fail
    UnityMalloc_MakeMallocFailAfterCount(1);

    void* r = unity_realloc(p, oldSize * 2);
    assert(r == nullptr); // allocation failed, realloc should return NULL

    // Old data should remain intact in the original buffer
    for (size_t i = 0; i < oldSize; ++i)
        assert(data[i] == (unsigned char)(i & 0xFF));

    // Clean up the original memory
    unity_free(p);

    printf("Test 5 passed.\n");
}

/*
 * Entry point: run all tests in a simple sequence.
 * This avoids relying on GTest or Unity's fixture macros, per requirements.
 */
int main(void)
{
    printf("Starting unity_realloc_test_suite (C++11) ...\n");

    test_unity_realloc_null_oldmem_allocates();
    test_unity_realloc_shrink_returns_same_ptr();
    test_unity_realloc_expand_copies_and_releases_old();
    test_unity_realloc_zero_frees_and_returns_null();
    test_unity_realloc_malloc_fail_does_not_free_old();

    printf("All tests completed successfully.\n");
    return 0;
}