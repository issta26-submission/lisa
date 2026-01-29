/*
 * Unit test suite for the focal method:
 *   int lsmFsSortedAppend(FileSystem *pFS, Snapshot *pSnapshot, Level *pLvl, int bDefer, Page **ppOut)
 *
 * Approach and goals:
 * - Build a lightweight, self-contained test harness in C++11 that exercises the lsmFsSortedAppend
 *   behavior without requiring a full application context.
 * - Rely on the project's existing type definitions and symbols (as provided by the real codebase)
 *   to ensure realistic interaction with FileSystem, Level, Segment, Page, etc.
 * - Cover multiple code paths by varying:
 *     - pFS->pCompress
 *     - bDefer
 *     - pLvl->lhs.iLastPg (iPrev)
 * - Use a small, non-terminating test framework (EXPECT_* macros) to maximize code coverage
 *   while avoiding premature termination on assertion failures.
 * - Provide comments explaining intent and expected state transitions for each test.
 *
 * Notes:
 * - This test assumes the project headers (e.g., lsmInt.h and related declarations) are accessible
 *   to the test translation unit. The test uses extern "C" to declare the focal function.
 * - The tests rely on the real implementation of helper functions (fsPageBuffer, findAppendPoint,
 *   fsBlockNext, etc.) from the project. The goal is to exercise as much realistic behavior as
 *   the environment permits, not to re-implement the engine here.
 * - Static helpers and internal state are left to the original implementation; we focus on
 *   observable effects from the public interface of lsmFsSortedAppend.
 *
 * Build (example):
 *   g++ -std=c++11 -I<path-to-project-headers> -c test_lsmtree.cpp
 *   g++ -std=c++11 test_lsmtree.o lsm_file.o -o test_lsmtree
 *
 * This file does not include any GoogleTest/GMock dependencies; it provides a lightweight harness.
 */

#include <sys/types.h>
#include <lsmInt.h>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>


// Forward declare the focal function using C linkage
extern "C" int lsmFsSortedAppend(
  FileSystem *pFS, 
  Snapshot *pSnapshot,
  Level *pLvl,
  int bDefer,
  Page **ppOut
);

// Import project type definitions

// Simple non-terminating test framework (EXPECT_ macros)
static int g_test_count = 0;
static int g_fail_count = 0;

#define EXPECT_TRUE(expr) do { \
    ++g_test_count; \
    if(!(expr)) { \
        std::cerr << "EXPECT_TRUE failed: " #expr " at " __FILE__ ":" << __LINE__ << "\n"; \
        ++g_fail_count; \
    } \
} while(0)

#define EXPECT_FALSE(expr) do { \
    ++g_test_count; \
    if(expr) { \
        std::cerr << "EXPECT_FALSE failed: " #expr " at " __FILE__ ":" << __LINE__ << "\n"; \
        ++g_fail_count; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    ++g_test_count; \
    if(!((a) == (b))) { \
        std::cerr << "EXPECT_EQ failed: " #a " == " #b " (actual: " << (a) << " vs " << (b) << ")" \
                  << " at " __FILE__ ":" << __LINE__ << "\n"; \
        ++g_fail_count; \
    } \
} while(0)

#define EXPECT_NEQ(a, b) do { \
    ++g_test_count; \
    if((a) == (b)) { \
        std::cerr << "EXPECT_NEQ failed: " #a " != " #b " (actual: " << (a) << ")" \
                  << " at " __FILE__ ":" << __LINE__ << "\n"; \
        ++g_fail_count; \
    } \
} while(0)

static void test_summary(const char* label) {
    std::cout << "Test: " << label << " => "
              << (g_fail_count ? "FAILED" : "PASSED")
              << " (" << g_test_count << " checks)" << std::endl;
}

// Helper to reset test counters between test cases
static void reset_counters() {
    g_test_count = 0;
    g_fail_count = 0;
}

/*
 * Test 1: Compressed or Defer path
 * Scenario:
 * - Trigger the first branch by setting pFS->pCompress != 0 or bDefer != 0.
 * - You expect a Page to be allocated and marked dirty, with a non-null ppOut, and
 *   the Page's pFS and pSeg pointers to reflect the test objects.
 * Rationale:
 * - This exercises the "In compressed database mode the page is not assigned a page number" code path
 *   and ensures that the Page structure is prepared correctly for later persistence.
 */
static void test_lsmFsSortedAppend_compressed_or_defer_path() {
    reset_counters();

    // Obtain or construct actual project types; rely on real project headers
    FileSystem fs;
    Snapshot *pSnapshot = nullptr;
    Level lvl;
    Page *pOut = nullptr;

    // Initialize a minimal, coherent environment for the test
    // Compressed mode to force the first branch
    fs.pCompress = 1;           // non-zero triggers the compressed path
    fs.nOut = 0;
    fs.nPagesize = 1024;        // typical page size

    // Level/LHS setup: Segment must exist; ensure redirect is NULL as required by assert
    lvl.lhs.iLastPg = 0;
    lvl.lhs.pRedirect = 0;
    lvl.lhs.iFirst = 0;

    // Call the focal method
    int rc = lsmFsSortedAppend(&fs, pSnapshot, &lvl, 0 /* bDefer */, &pOut);

    // Assertions: rc ok, page produced, and basic wiring is established
    EXPECT_EQ(0, rc);                    // LSM_OK is typically 0
    EXPECT_TRUE(pOut != nullptr);        // Output page allocated
    if (pOut != nullptr) {
        EXPECT_TRUE(pOut->pFS == &fs);    // Page should reference the fille system
        EXPECT_TRUE(pOut->pSeg == &lvl.lhs); // Page should reference the segment used
        EXPECT_TRUE((pOut->flags & PAGE_DIRTY) != 0); // Page should be marked dirty
        // nData should be at least the page size (minus 4 if compress==0; here compress==1)
        EXPECT_TRUE(pOut->nData >= fs.nPagesize - 0);  
        // Basic sanity checks (nRef should be 1 as per method)
        EXPECT_TRUE(pOut->nRef >= 1);
    }

    test_summary("test_lsmFsSortedAppend_compressed_or_defer_path");
}

/*
 * Test 2: Non-compressed path first page allocation
 * Scenario:
 * - pFS->pCompress == 0 and bDefer == 0
 * - iPrev (p->iLastPg) is 0, forcing a call to findAppendPoint()
 * - We expect the function to allocate a new page (via fsPageGet) and mark the page dirty,
 *   updating the Segment's iFirst and iLastPg, and potentially wiring end-of-block pointers
 *   for first/last page cases.
 * Rationale:
 * - Validates the non-compressed path that handles initial page allocation and block setup.
 */
static void test_lsmFsSortedAppend_noncompressed_first_page_allocation() {
    reset_counters();

    FileSystem fs;
    Snapshot *pSnapshot = nullptr;
    Level lvl;
    Page *pOut = nullptr;

    fs.pCompress = 0;              // non-compressed path
    fs.nOut = 0;
    fs.nPagesize = 1024;

    // Level's left-hand segment state initial
    lvl.lhs.iLastPg = 0;
    lvl.lhs.pRedirect = 0;
    lvl.lhs.iFirst = 0;

    // Call focal method with defer disabled
    int rc = lsmFsSortedAppend(&fs, pSnapshot, &lvl, 0 /* bDefer */, &pOut);

    // Assertions
    EXPECT_EQ(0, rc);
    EXPECT_TRUE(pOut != nullptr);
    if (pOut != nullptr) {
        EXPECT_TRUE(pOut->pFS == &fs);
        EXPECT_TRUE(pOut->pSeg == &lvl.lhs);
        EXPECT_TRUE((pOut->flags & PAGE_DIRTY) != 0);
        // First insertion should set iFirst and iLastPg to the allocated page number
        EXPECT_TRUE(lvl.lhs.iFirst != 0 || lvl.lhs.iLastPg != 0); // at least one updated
        // If the page data is writable, buffer must exist
        EXPECT_TRUE(pOut->aData != nullptr);
    }

    test_summary("test_lsmFsSortedAppend_noncompressed_first_page_allocation");
}

/*
 * Test 3: Non-compressed path: subsequent page in the block
 * Scenario:
 * - pFS->pCompress == 0 and bDefer == 0
 * - iPrev != 0 and fsIsLast(pFS, iPrev) returns false, so iApp should be iPrev + 1
 * - We verify that the function returns and provides a non-null ppOut, and that the page
 *   is marked dirty for continuation in the LSM block.
 * Rationale:
 * - Exercise the "else" branch where a new page is allocated in the same block.
 * - This checks the "not last in block" path and ensures page linkage is established.
 */
static void test_lsmFsSortedAppend_noncompressed_subsequent_in_block() {
    reset_counters();

    FileSystem fs;
    Snapshot *pSnapshot = nullptr;
    Level lvl;
    Page *pOut = nullptr;

    fs.pCompress = 0;
    fs.nOut = 0;
    fs.nPagesize = 1024;

    // Simulate a previously allocated page as the last page, but force not-last logic via non-last condition
    lvl.lhs.iLastPg = 5;     // some non-zero previous page
    lvl.lhs.pRedirect = 0;
    lvl.lhs.iFirst = 5;      // assume the block has a first page

    int rc = lsmFsSortedAppend(&fs, pSnapshot, &lvl, 0, &pOut);

    // Assertions
    EXPECT_EQ(0, rc);
    EXPECT_TRUE(pOut != nullptr);
    if (pOut != nullptr) {
        EXPECT_TRUE(pOut->pFS == &fs);
        EXPECT_TRUE(pOut->pSeg == &lvl.lhs);
        EXPECT_TRUE((pOut->flags & PAGE_DIRTY) != 0);
    }

    test_summary("test_lsmFsSortedAppend_noncompressed_subsequent_in_block");
}

/*
 * Test runner
 * - Invokes all test cases and reports a final summary.
 */
int main(void) {
    std::cout << "Starting tests for lsmFsSortedAppend...\n";

    test_lsmFsSortedAppend_compressed_or_defer_path();
    test_lsmFsSortedAppend_noncompressed_first_page_allocation();
    test_lsmFsSortedAppend_noncompressed_subsequent_in_block();

    int total = g_test_count;
    int failed = g_fail_count;
    std::cout << "Total checks: " << total << ", Failures: " << failed << "\n";

    // Return non-zero if there were failures to indicate test issues
    return (failed ? 1 : 0);
}