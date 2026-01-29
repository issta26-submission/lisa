// Unit test suite for the focal method lsmFsMetaPageData (C function) using a lightweight
// C++11 test harness (no Google Test). The tests are designed to be compiled together
// with the project sources (lsm_file.c, etc.) in a typical C/C++ build environment.
//
// Focus: verify that lsmFsMetaPageData returns the aData pointer and, when pnData is non-NULL,
// writes the FileSystem.nMetaRwSize value into *pnData.
//
// Key concepts exercised (Candidate Keywords):
// - MetaPage, FileSystem, aData, nMetaRwSize
// - lsmFsMetaPageData behavior with non-NULL and NULL pnData
// - static-like dependencies are mocked via lightweight stubs in test harness
// - basic data-type correctness and pointer equality checks

#include <sys/types.h>
#include <lsmInt.h>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstdint>


// Step 1: Provide C declarations identical to the production API as used by lsmFsMetaPageData.
// We declare minimal structural representations (pFS and aData) that the focal function accesses.
// These declarations are intended to be compatible with the actual production types for binary
// compatibility of member access used by lsmFsMetaPageData.

extern "C" {
  typedef uint8_t u8;

  // Lightweight stub of FileSystem used by MetaPage
  struct FileSystem {
    int nMetaRwSize; // The value that should be returned via *pnData when requested
  };

  // Lightweight stub of MetaPage containing only the fields accessed by lsmFsMetaPageData
  struct MetaPage {
    FileSystem *pFS; // Pointer to associated file system
    u8 *aData;       // Pointer to the data buffer contained in the meta page
  };

  // Focused function under test (produced by the production code in lsm_file.c)
  // The actual implementation uses the real types, but the test provides compatible types
  // for invocation and verification.
  u8 *lsmFsMetaPageData(MetaPage *pPg, int *pnData);
}

// Simple non-terminating test harness
static int g_total = 0;
static int g_failed = 0;

#define TEST_INIT() do { g_total = 0; g_failed = 0; } while(0)
#define ASSERT_TRUE(cond, msg) do { g_total++; if(!(cond)){ fprintf(stderr, "ASSERT_TRUE FAILED: %s\n", msg); g_failed++; } } while(0)
#define ASSERT_EQ(a, b, msg) do { g_total++; if((a) != (b)){ fprintf(stderr, "ASSERT_EQ FAILED: %s. Expected %ld, got %ld\n", msg, (long)(b), (long)(a)); g_failed++; } } while(0)
#define ASSERT_PTR_EQ(a, b, msg) do { g_total++; if((void*)(a) != (void*)(b)){ fprintf(stderr, "ASSERT_PTR_EQ FAILED: %s. Expected %p, got %p\n", msg, (void*)(b), (void*)(a)); g_failed++; } } while(0)

// Namespace to group tests and explain intent
namespace LsmFsMetaPageDataTests {

  // Helper to print a header for each test
  static void print_test_header(const char* name) {
    printf("[TEST] %s\n", name);
  }

  // Test 1: Basic behavior - when pnData is non-NULL, returns aData and writes nMetaRwSize into *pnData
  // This validates both the return value and the side-effect on the output parameter.
  void test_basic_sets_pnData_and_returns_aData() {
    print_test_header("test_basic_sets_pnData_and_returns_aData");
    // Arrange
    FileSystem fs;
    fs.nMetaRwSize = 1234; // arbitrary non-zero value to verify transfer
    u8 data_buf[16] = {0xAA, 0xBB, 0xCC, 0xDD, 0}; // distinct, non-zero initial pattern
    MetaPage page;
    page.pFS = &fs;
    page.aData = data_buf;

    int outVal = -1;

    // Act
    u8 *ret = lsmFsMetaPageData(&page, &outVal);

    // Assert
    ASSERT_PTR_EQ(ret, data_buf, "lsmFsMetaPageData should return pointer to aData buffer");
    ASSERT_EQ(outVal, 1234, "pnData should be assigned from pFS->nMetaRwSize");
  }

  // Test 2: pnData is NULL - the function should not attempt to write and should still return aData
  void test_null_pnData_does_not_write() {
    print_test_header("test_null_pnData_does_not_write");
    // Arrange
    FileSystem fs;
    fs.nMetaRwSize = 999;
    u8 data_buf[8] = {0,1,2,3,4,5,6,7};
    MetaPage page;
    page.pFS = &fs;
    page.aData = data_buf;

    // Act
    u8 *ret = lsmFsMetaPageData(&page, NULL);

    // Assert
    ASSERT_PTR_EQ(ret, data_buf, "When pnData is NULL, should still return aData buffer pointer");
    // Note: Since pnData is NULL, there is no value to compare. We rely on the return pointer.
  }

  // Test 3: Boundary case - nMetaRwSize is zero
  void test_zero_nMetaRwSize_value() {
    print_test_header("test_zero_nMetaRwSize_value");
    // Arrange
    FileSystem fs;
    fs.nMetaRwSize = 0;
    u8 data_buf[4] = {9, 9, 9, 9};
    MetaPage page;
    page.pFS = &fs;
    page.aData = data_buf;

    int outVal = -7;

    // Act
    u8 *ret = lsmFsMetaPageData(&page, &outVal);

    // Assert
    ASSERT_PTR_EQ(ret, data_buf, "Return should be aData buffer even if nMetaRwSize is zero");
    ASSERT_EQ(outVal, 0, "pnData should reflect zero value when nMetaRwSize == 0");
  }

  // Run all tests
  int run_all() {
    test_basic_sets_pnData_and_returns_aData();
    test_null_pnData_does_not_write();
    test_zero_nMetaRwSize_value();
    return g_failed;
  }
} // namespace

// Entrypoint
int main() {
  TEST_INIT();
  int failures = LsmFsMetaPageDataTests::run_all();
  int total = g_total;
  printf("Summary: Total=%d, Failures=%d\n", total, failures);
  return failures;
}