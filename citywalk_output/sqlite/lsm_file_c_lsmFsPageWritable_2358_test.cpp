/*
  Unit test suite for the focal method:
  int lsmFsPageWritable(Page *pPg)

  Step 1 (Program Understanding):
  The function lsmFsPageWritable checks whether a Page structure is dirty by testing the PAGE_DIRTY flag.
  Core Candidate Keywords: Page, flags, PAGE_DIRTY, lsmFsPageWritable.

  Step 2 (Unit Test Generation):
  Tests verify true/false branches:
  - When PAGE_DIRTY bit is set -> returns 1
  - When PAGE_DIRTY bit is not set -> returns 0
  - When other bits are set alongside PAGE_DIRTY -> still returns 1

  Step 3 (Test Case Refinement):
  - Use a lightweight C++11 test harness (no GTest) running under main()
  - Use non-terminating assertions (log failures and continue)
  - Access the function via C linkage to ensure compatibility
  - Rely on the project-provided Page type and PAGE_DIRTY macro

  Notes:
  - This test assumes the project provides lsmInt.h with the Page struct and the PAGE_DIRTY macro.
  - The test links against the project's lsm_file.c (or equivalent compilation unit) containing lsmFsPageWritable.

  The tests are designed to be compiled with C++11 and do not rely on any third-party testing framework.
*/

#include <sys/types.h>
#include <lsmInt.h>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>


// Bring in the project definitions. The Page type and PAGE_DIRTY macro are defined in the project.
#ifdef __cplusplus
extern "C" {
#endif
// Prototype for the focal function. It is defined in a C source (lsm_file.c).
int lsmFsPageWritable(Page *pPg);
#ifdef __cplusplus
}
#endif

// Simple non-terminating test harness
namespace testutil {
    static int g_total = 0;
    static int g_failed = 0;

    // Non-terminating assertion: records failure but continues execution
    inline void expect_eq_int(int actual, int expected, const char* msg) {
        ++g_total;
        if (actual != expected) {
            ++g_failed;
            std::cerr << "[TEST_FAIL] " << (msg ? msg : "") 
                      << " | got: " << actual << " , expected: " << expected << "\n";
        } else {
            std::cout << "[TEST_PASS] " << (msg ? msg : "") 
                      << " | value: " << actual << "\n";
        }
    }

    // Report summary
    inline void summarize() {
        std::cerr << "Tests run: " << g_total << ", Failures: " << g_failed << "\n";
    }
}

// Unit Test 1: Dirty flag set -> lsmFsPageWritable should return 1
void test_lsmFsPageWritable_dirty_flag_set() {
    // Prepare a Page with only the PAGE_DIRTY bit set
    Page p;
    std::memset(&p, 0, sizeof(p));        // Zero-initialize to avoid undefined fields
    p.flags = PAGE_DIRTY;                 // Set only the dirty flag

    int res = lsmFsPageWritable(&p);
    testutil::expect_eq_int(res, 1, "Dirty flag set should return 1");
}

// Unit Test 2: Dirty flag not set -> lsmFsPageWritable should return 0
void test_lsmFsPageWritable_dirty_flag_clear() {
    // Prepare a Page with no dirty flag
    Page p;
    std::memset(&p, 0, sizeof(p));        // Zero-initialize
    p.flags = 0;                          // Ensure dirty flag is not set

    int res = lsmFsPageWritable(&p);
    testutil::expect_eq_int(res, 0, "No dirty flag should return 0");
}

// Unit Test 3: Dirty flag with other bits set -> still returns 1
void test_lsmFsPageWritable_dirty_with_other_bits() {
    // Prepare a Page with PAGE_DIRTY plus some other bits
    Page p;
    std::memset(&p, 0, sizeof(p));        // Zero-initialize
    // Combine PAGE_DIRTY with arbitrary additional bits
    p.flags = PAGE_DIRTY | 0x00000002 | 0x00000004;

    int res = lsmFsPageWritable(&p);
    testutil::expect_eq_int(res, 1, "Dirty flag + other bits should return 1");
}

// Entry point to run all tests
int main() {
    std::cout << "Running tests for lsmFsPageWritable(Page *pPg)..." << std::endl;

    test_lsmFsPageWritable_dirty_flag_set();
    test_lsmFsPageWritable_dirty_flag_clear();
    test_lsmFsPageWritable_dirty_with_other_bits();

    testutil::summarize();

    // Return non-zero if any test failed
    return (testutil::g_failed > 0) ? 1 : 0;
}