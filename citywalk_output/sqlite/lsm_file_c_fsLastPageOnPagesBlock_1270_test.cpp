/*
Unit test suite for fsLastPageOnPagesBlock(FileSystem*, LsmPgno) in lsm_file.c

Context and approach:
- The focal method is a thin wrapper:
    LsmPgno fsLastPageOnPagesBlock(FileSystem *pFS, LsmPgno iPg){
        return fsLastPageOnBlock(pFS, fsPageToBlock(pFS, iPg));
      }
- fsPageToBlock and fsLastPageOnBlock are static functions inside lsm_file.c, so we cannot call them directly from tests.
- We derive a test strategy that uses public configuration hooks (as exposed in the provided class dependencies) to establish a predictable Pages-Per-Block mapping, then validates that:
  fsLastPageOnPagesBlock(pFS, iPg) returns the last page index of the corresponding block.
- We rely on the public API to configure page/block sizes:
    void lsmFsSetPageSize(FileSystem *pFS, int nPgsz);
    void lsmFsSetBlockSize(FileSystem *pFS, int nBlocksize);

- We implement tests in C++11 (no Google Test) and drive them from main().
- Test strategy uses a deterministic mapping:
  pagesPerBlock = blockSizeInPages (as configured by lsmFsSetBlockSize).
  For a given iPg, the expected last page of its block is:
    expected = ( (iPg / pagesPerBlock) + 1 ) * pagesPerBlock - 1
  where pagesPerBlock > 0.
- This test is self-contained: it allocates a FileSystem object, configures page/block sizes, invokes the target function, and asserts the result matches the expected value for a range of iPg.

Notes for maintainers:
- The test uses the library’s public API to drive internal behavior (no access to static helpers is required).
- We allocate a FileSystem object with malloc and initialize via library setters to avoid depending on internal initialization details.
- Assertions are non-terminating: we collect and print failures but continue executing remaining tests to maximize coverage.

Caveats:
- This test assumes that:
  - lsmFsSetPageSize(pFS, nPgsz) sets the page size in bytes per page.
  - lsmFsSetBlockSize(pFS, nBlocksize) sets the number of pages per block.
  - fsLastPageOnPagesBlock uses that pages-per-block to compute the last page index of the block containing iPg.
- If the real code uses a different unit (e.g., block size in bytes), you may need to adjust the test’s calculation accordingly.

Usage:
- Compile this test together with the library that provides lsm_file.c and lsmInt.h.
- Run the resulting executable to see PASS/FAIL outputs and a short summary.

Code begins here:
*/

#include <sys/types.h>
#include <lsmInt.h>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstdlib>
#include <cstdint>


// Import C headers for types and function declarations.
// We use extern "C" to prevent C++ name mangling when linking against C code.
extern "C" {
}

// Public API declarations (assumed from provided dependencies)
// These allow configuring a FileSystem instance for the test.
extern "C" void lsmFsSetPageSize(FileSystem *pFS, int nPgsz);
extern "C" void lsmFsSetBlockSize(FileSystem *pFS, int nBlocksize);

// Focal function under test (non-static, external linkage in lsm_file.c)
extern "C" LsmPgno fsLastPageOnPagesBlock(FileSystem *pFS, LsmPgno iPg);

// Simple non-terminating test assertion helpers
static int g_total = 0;
static int g_pass  = 0;

static void log_result(const char* test_name, bool passed, const char* detail = "") {
    ++g_total;
    if (passed) {
        ++g_pass;
        printf("PASS: %s%s\n", test_name, detail[0] ? (": " + std::string(detail)) .c_str() : "");
    } else {
        printf("FAIL: %s%s\n", test_name, detail[0] ? (": " + std::string(detail)) .c_str() : "");
    }
}

// Utility to safely allocate a FileSystem instance
static FileSystem* allocate_fs_instance() {
    // Allocate raw memory for FileSystem and zero it.
    // Real initialization is performed by library setters.
    FileSystem* pFS = (FileSystem*)malloc(sizeof(FileSystem));
    if (pFS == nullptr) {
        fprintf(stderr, "Failed to allocate FileSystem instance.\n");
        return nullptr;
    }
    std::memset(pFS, 0, sizeof(FileSystem));
    return pFS;
}

// Compute expected last page for a given iPg, given pagesPerBlock
static LsmPgno expected_last_page_in_block(int pagesPerBlock, LsmPgno iPg) {
    if (pagesPerBlock <= 0) return 0; // guard
    LsmPgno iBlk = iPg / pagesPerBlock;
    return (iBlk + 1) * pagesPerBlock - 1;
}

// Test 1: Basic mapping with small, explicit page/block sizes
static void test_fsLastPageOnPagesBlock_basic_mapping() {
    // Create and configure a FileSystem
    FileSystem* pFS = allocate_fs_instance();
    if (!pFS) return;

    // Configure: small, deterministic sizes
    // Assume pagesPerBlock = 4
    lsmFsSetPageSize(pFS, 4);   // 4 bytes per page (example)
    lsmFsSetBlockSize(pFS, 4);  // 4 pages per block
    const int pagesPerBlock = 4;

    // Test a range of iPg values across multiple blocks
    const int testRange = 20;
    for (int iPg = 0; iPg < testRange; ++iPg) {
        LsmPgno expected = expected_last_page_in_block(pagesPerBlock, (LsmPgno)iPg);
        LsmPgno got = fsLastPageOnPagesBlock(pFS, (LsmPgno)iPg);
        char detail[128];
        std::snprintf(detail, sizeof(detail), "iPg=%d, expected=%llu, got=%llu",
                      iPg, (unsigned long long)expected, (unsigned long long)got);
        log_result("fsLastPageOnPagesBlock_basic_mapping", (got == expected), detail);
    }

    free(pFS);
}

// Test 2: Different configuration to exercise another pagesPerBlock
static void test_fsLastPageOnPagesBlock_else_config() {
    FileSystem* pFS = allocate_fs_instance();
    if (!pFS) return;

    // Configure: larger pagesPerBlock
    // Assume pagesPerBlock = 8
    lsmFsSetPageSize(pFS, 8);   // 8 bytes per page (example)
    lsmFsSetBlockSize(pFS, 8);  // 8 pages per block
    const int pagesPerBlock = 8;

    // Test a representative range
    const int testRange = 40;
    for (int iPg = 0; iPg < testRange; ++iPg) {
        LsmPgno expected = expected_last_page_in_block(pagesPerBlock, (LsmPgno)iPg);
        LsmPgno got = fsLastPageOnPagesBlock(pFS, (LsmPgno)iPg);
        char detail[128];
        std::snprintf(detail, sizeof(detail), "iPg=%d, expected=%llu, got=%llu",
                      iPg, (unsigned long long)expected, (unsigned long long)got);
        log_result("fsLastPageOnPagesBlock_else_config", (got == expected), detail);
    }

    free(pFS);
}

// Run all tests and print a summary
static void run_all_tests() {
    printf("Starting tests for fsLastPageOnPagesBlock...\n");
    test_fsLastPageOnPagesBlock_basic_mapping();
    test_fsLastPageOnPagesBlock_else_config();

    printf("Test summary: %d ran, %d passed, %d failed.\n",
           g_total, g_pass, g_total - g_pass);
}

int main() {
    // Step 1: Program Understanding
    // This test suite targets fsLastPageOnPagesBlock behavior via public page/block size configuration.
    // Step 2 & 3: Unit Test Generation and Refinement
    // We generate tests that cover two configurations and multiple iPg values to exercise normal behavior.

    run_all_tests();
    return (g_total == g_pass) ? 0 : 1;
}