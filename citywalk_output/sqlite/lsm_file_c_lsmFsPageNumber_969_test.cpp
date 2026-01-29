/*
Unit test suite for the focal method:
  LsmPgno lsmFsPageNumber(Page *pPage)

Context and approach:
- The function returns pPage ? pPage->iPg : 0.
- True branch: when pPage is non-null, returns the iPg member.
- False branch: when pPage is null, returns 0.
- We test both branches using the actual Page type defined in the project headers.
- We avoid terminating assertions; failures are logged but tests continue.
- We rely on the project-provided lsmInt.h header to obtain Page, LsmPgno, and the function prototype.
- The test uses a minimal harness and prints PASS/FAIL per assertion.

Notes:
- This test is written for C++11 and assumes the project exposes Page, LsmPgno, and the function lsmFsPageNumber via lsmInt.h.
- The tests call the C function through an extern "C" declaration to avoid name mangling when compiled as C++.
*/

#include <sys/types.h>
#include <lsmInt.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <cstdint>


// Include project headers that define Page, LsmPgno, etc.
// Adjust the include path as necessary for the test environment.

extern "C" {
  // Ensure C linkage for the focal function
  LsmPgno lsmFsPageNumber(Page *pPage);
}

// Lightweight, non-terminating test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Non-terminating assertion macro: logs failure but continues execution
#define EXPECT_EQ(expected, actual) do {                                    \
    ++g_tests_run;                                                             \
    if (static_cast<int64_t>(static_cast<int64_t>(expected)) !=                   \
        static_cast<int64_t>(static_cast<int64_t>(actual))) {                 \
      std::cerr << "FAIL: " << __func__ << "@" << __LINE__                    \
                << " - Expected: " << (expected)                                \
                << ", Actual: " << (actual) << std::endl;                    \
      ++g_tests_failed;                                                       \
    } else {                                                                 \
      std::cout << "PASS: " << __func__ << "@" << __LINE__ << std::endl;       \
    }                                                                        \
  } while(0)


// Test 1: Non-null Page should return the page number stored in iPg
void test_lsmFsPageNumber_nonNull() {
  // Prepare a Page instance with a known iPg value.
  Page p;
  p.iPg = 7;  // Known value to verify correct propagation
  LsmPgno result = lsmFsPageNumber(&p);

  // Expect the function to return exactly the iPg value.
  EXPECT_EQ((LsmPgno)7, result);

  // Additional check with another value is optional but helps catch regressions.
  p.iPg = 12345;
  result = lsmFsPageNumber(&p);
  EXPECT_EQ((LsmPgno)12345, result);
}

// Test 2: Null Page pointer should return 0
void test_lsmFsPageNumber_null() {
  // Pass a null pointer to exercise the false branch.
  LsmPgno result = lsmFsPageNumber(nullptr);

  // Expect 0 when pPage is null.
  EXPECT_EQ((LsmPgno)0, result);
}

int main() {
  // Run tests
  test_lsmFsPageNumber_nonNull();
  test_lsmFsPageNumber_null();

  // Summary
  if (g_tests_failed == 0) {
    std::cout << "ALL TESTS PASSED (" << g_tests_run << " assertions)." << std::endl;
  } else {
    std::cerr << g_tests_failed << " test(s) FAILED out of " << g_tests_run
              << " assertions." << std::endl;
  }

  // Return non-zero if any test failed
  return g_tests_failed;
}