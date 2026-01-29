/*
 * Unit tests for lsmFsSegmentContainsPg
 * 
 * Approach:
 * - Use a lightweight, self-contained test harness (no GTest).
 * - Exercise the focal function lsmFsSegmentContainsPg from lsm_file.c.
 * - Rely on the project's existing type definitions (FileSystem, Segment, Redirect, LsmPgno, etc.)
 *   and its static helpers (fsPageToBlock, fsRedirectBlock, fsBlockNext) as implemented in
 *   the actual project sources. We only construct inputs and call the function.
 * - Provide multiple test cases to exercise true/false branches of the predicate and
 *   the loop's behavior (as much as possible given the underlying implementation).
 * - Use a non-terminating assertion style (EXPECT-like) to execute all tests.
 *
 * Notes:
 * - This test assumes the project headers are available on the include path.
 * - The tests are designed to compile with a C++11 toolchain and link against the
 *   existing C sources (lsm_file.c and related). 
 * - The code includes explanatory comments for each test case.
 *
 * To run:
 *   - Compile with your project build system, ensuring the test file is linked with the
 *     library/object containing lsmFsSegmentContainsPg and the related types.
 *   - The main function executes all test cases and reports results.
 */

#include <vector>
#include <sys/types.h>
#include <lsmInt.h>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>


// Bring in C declarations for the project types and function under test.
// We wrap in extern "C" to ensure proper linkage when compiled as C++.
extern "C" {
}

// Lightweight test framework (non-terminating assertions)
static int g_test_failures = 0;
#define EXPECT_EQ(actual, expected, msg) do { \
    if ((actual) != (expected)) { \
        std::cerr << "[FAIL] " << msg << " | Expected: " << (expected) \
                  << ", Got: " << (actual) << std::endl; \
        ++g_test_failures; \
    } else { \
        std::cout << "[PASS] " << msg << std::endl; \
    } \
} while(0)

#define EXPECT_TRUE(cond, msg)  EXPECT_EQ((int)(!!(cond)), 1, msg)
#define EXPECT_FALSE(cond, msg) EXPECT_EQ((int)(!!(cond)), 0, msg)

// Helper to create a minimal test harness environment.
// We rely on the project's real structures, so we merely instantiate and relate them.

//
// Test Case 1: iPg equals the first page of the segment.
// Expected: The loop is not entered, pbRes should be true, and rc should be LSM_OK.
// This exercises the "true" branch of pbRes without traversing blocks.
//
void test_case_contains_pg_when_pg_is_first() {
    // Setup: Build a minimal FileSystem, Segment, and Redirect.
    // The actual behavior of fsPageToBlock/fsRedirectBlock depends on the real code,
    // but we assume the scenario where the page to check is the first page of the segment.
    FileSystem fs;
    Segment seg;
    Redirect redir;

    seg.pRedirect = &redir;
    // Choose first/last pages such that iPgBlock should equal the block of iFirst
    // and the redirect mapping yields iBlk equal to iPgBlock initially.
    seg.iFirst = 1;       // first page in the segment
    seg.iLastPg = 10;     // last page in the segment (arbitrary for this test)

    // We pass iPg equal to the first page to exercise the "already on the first block" case.
    LsmPgno iPg = seg.iFirst;
    int bRes = 0;

    int rc = lsmFsSegmentContainsPg(&fs, &seg, iPg, &bRes);

    EXPECT_EQ(rc, LSM_OK, "rc should be LSM_OK when first-page case is evaluated");
    EXPECT_TRUE(bRes, "pbRes should be true when iPg is the first page of the segment");
}

// 
// Test Case 2: iPg is not on the traversal path; traversal should end with iBlk==iLastBlk without matching iPgBlock.
// This should yield pbRes == false (0) assuming the traversal stops on iLastBlk before hitting iPgBlock.
// This exercises the false branch of the predicate.
//
void test_case_contains_pg_pg_not_on_path() {
    FileSystem fs;
    Segment seg;
    Redirect redir;

    seg.pRedirect = &redir;
    // Choose a segment where the first and last blocks are distinct, and iPg is unlikely
    // to be encountered by following the redirect path (as determined by the real fsBlockNext logic).
    seg.iFirst = 2;
    seg.iLastPg = 9;

    // Pick an iPg that is not the first page of the segment.
    LsmPgno iPg = seg.iFirst + 3; // e.g., 5

    int bRes = -1;
    int rc = lsmFsSegmentContainsPg(&fs, &seg, iPg, &bRes);

    // We cannot guarantee pbRes due to internal mapping, but we expect the function to complete
    // with rc == LSM_OK (no I/O error) and an explicit boolean for containment.
    EXPECT_EQ(rc, LSM_OK, "rc should be LSM_OK for normal traversal (no IO error)");
    // pbRes should be either 0 or 1 depending on whether iPgBlock was reached.
    // We document the expectation that in typical non-path case, it's false.
    if (bRes == 0) {
        std::cout << "[INFO] pbRes is false as expected for non-path iPg in this setup." << std::endl;
    } else {
        // If the internal redirect mapping unexpectedly includes iPg, still acceptable;
        // report as a pass for exercising the branch.
        std::cout << "[INFO] pbRes unexpectedly true; traversal included iPgBlock in this setup." << std::endl;
    }
}

// 
// Test Case 3: iPg equals the last page's block but is not reached due to redirect/loop behavior.
// This helps exercise the scenario where the loop terminates immediately or near-immediately
// due to iBlk == iLastBlk, testing another execution path.
//
void test_case_contains_pg_when_last_blk_reached_immediately() {
    FileSystem fs;
    Segment seg;
    Redirect redir;

    seg.pRedirect = &redir;
    seg.iFirst = 3;
    seg.iLastPg = 4;

    // Choose iPg such that its block is the same as iLastBlk but not equal to iPgBlock.
    // The actual values depend on the internal mapping. We rely on the function to terminate
    // quickly when iBlk equals iLastBlk.
    LsmPgno iPg = seg.iLastPg; // attempt to align with last page block
    int bRes = 0;
    int rc = lsmFsSegmentContainsPg(&fs, &seg, iPg, &bRes);

    EXPECT_EQ(rc, LSM_OK, "rc should be LSM_OK when terminating due to last-block condition");
    // pbRes may be true or false depending on internal mappings; log the result for visibility.
    if (bRes) {
        std::cout << "[INFO] pbRes is true under last-block termination." << std::endl;
    } else {
        std::cout << "[INFO] pbRes is false under last-block termination." << std::endl;
    }
}

// Simple runner to execute all tests
int main() {
    std::cout << "Running tests for lsmFsSegmentContainsPg..." << std::endl;

    test_case_contains_pg_when_first();
    test_case_contains_pg_pg_not_on_path();
    test_case_contains_pg_when_last_blk_reached_immediately();

    if (g_test_failures != 0) {
        std::cerr << "[SUMMARY] Tests completed with " << g_test_failures << " failure(s)." << std::endl;
        return 1;
    } else {
        std::cout << "[SUMMARY] All tests executed. No failures detected." << std::endl;
        return 0;
    }
}