/*
  Unit test suite for lsmFsPurgeCache(FileSystem *pFS)
  - Targeted to compile under C++11 without GoogleTest
  - Uses a lightweight custom test harness with non-terminating assertions
  - Tests basic behavior: empty cache purge and purge with two pages
  - Relies on real lsm_file.c implementation being linked (static helpers are used internally)
  - Accesses public fields of FileSystem and Page as defined in lsmInt.h
*/

#include <sys/types.h>
#include <lsmInt.h>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstdlib>


// Bring in the C declarations for the core data structures used by lsmFsPurgeCache
extern "C" {
}

// Declare lsmFsPurgeCache from the focal C file (C function)
extern "C" void lsmFsPurgeCache(FileSystem *pFS);

/* Lightweight test harness
   - Non-terminating assertions: on failure, increment counter and print message, but continue
*/
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_CHECK(cond, msg) do { \
    ++g_tests_run; \
    if(!(cond)) { \
        ++g_tests_failed; \
        std::fprintf(stderr, "TEST_FAIL: %s (line %d)\n", (msg), __LINE__); \
    } \
} while(0)

#define TEST_EQ(actual, expected, msg) do { \
    ++g_tests_run; \
    if((actual) != (expected)) { \
        ++g_tests_failed; \
        std::fprintf(stderr, "TEST_FAIL: %s - expected %ld, got %ld (line %d)\n", (msg), (long)(expected), (long)(actual), __LINE__); \
    } \
} while(0)

/* Utility to safely create/destroy Page objects consistent with the focal code expectations.
   We only initialize fields that lsmFsPurgeCache reads directly:
   - Page *pLruNext
   - unsigned int flags (to satisfy PAGE_FREE assertion)
   Other fields are left as_default by the C implementation;
   we avoid touching fields we cannot guarantee exist in the current header.
*/
static Page* make_page_with_next(Page* next, bool page_free)
{
    Page *p = new Page();
    // Initialize minimal fields observed by lsmFsPurgeCache:
    // - pLruNext is used by the purge loop
    p->pLruNext = next;
    // - flags must include PAGE_FREE for the assertion inside purge
    p->flags = page_free ? PAGE_FREE : 0;
    return p;
}

/* Test 1: Purge with an empty cache (pLruFirst == nullptr)
   - Expect no crashes
   - pLruFirst and pLruLast should remain null after purge
   - nCacheAlloc must satisfy invariant: 0 <= nCacheAlloc <= nOut
*/
static void test_purge_cache_empty_list()
{
    // Arrange
    FileSystem fs;
    std::memset(&fs, 0, sizeof(fs)); // zero all fields to start clean

    fs.pLruFirst = nullptr;
    fs.pLruLast  = nullptr;
    fs.nCacheAlloc = 0;
    fs.nOut = 0;

    // Act
    lsmFsPurgeCache(&fs);

    // Assert
    TEST_CHECK(fs.pLruFirst == nullptr, "pLruFirst should remain null after purge (empty list)");
    TEST_CHECK(fs.pLruLast  == nullptr, "pLruLast should remain null after purge (empty list)");
    TEST_CHECK(fs.nCacheAlloc <= fs.nOut && fs.nCacheAlloc >= 0, "Cache allocation invariant must hold after purge (empty list)");
}

/* Test 2: Purge with a small LRU chain of two pages
   - Both pages must have PAGE_FREE flag to satisfy the assert in purge
   - After purge, pLruFirst and pLruLast must be reset to null
   - We do not rely on internal static helpers; we only verify public effects
*/
static void test_purge_cache_with_pages()
{
    // Arrange: create two Page objects linked via pLruNext
    Page *pA = make_page_with_next(nullptr, true); // placeholder, pA will point to pB below
    Page *pB = make_page_with_next(nullptr, true);

    // Link them: pA -> pB
    pA->pLruNext = pB;
    // Note: The test uses only pLruNext and flags; avoid touching extra fields

    // Prepare FileSystem state
    FileSystem fs;
    std::memset(&fs, 0, sizeof(fs));
    fs.pLruFirst = pA;
    fs.pLruLast  = pB;
    fs.nCacheAlloc = 2;
    fs.nOut = 2;

    // Act
    lsmFsPurgeCache(&fs);

    // Assert
    TEST_CHECK(fs.pLruFirst == nullptr, "After purge, pLruFirst must be null");
    TEST_CHECK(fs.pLruLast  == nullptr, "After purge, pLruLast must be null");
    TEST_CHECK(fs.nCacheAlloc <= fs.nOut && fs.nCacheAlloc >= 0, "Cache allocation invariant must hold after purge with pages");

    // Cleanup: Pages themselves are not freed by lsmFsPurgeCache; release memory to avoid leaks
    delete pA;
    delete pB;
}

/* Test runner */
static void run_all_tests()
{
    test_purge_cache_empty_list();
    test_purge_cache_with_pages();

    std::fprintf(stdout, "Tests run: %d, Failures: %d\n", g_tests_run, g_tests_failed);
}

int main()
{
    // Run the test suite
    run_all_tests();

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}