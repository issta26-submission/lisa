/*
  C++11 unit test suite for the focal method:
  int lsmFsMetaPageGet(FileSystem *pFS, int bWrite, int iPg, MetaPage **ppPg)

  Notes:
  - Tests are written as a lightweight, non-terminating assertion framework (no external test libraries).
  - The tests rely on the project-provided C interface (lsmFsMetaPageGet, lsmFsMetaPageRelease and the
    types FileSystem, MetaPage, etc.) exposed by the project headers.
  - We test key branches and behavior:
    1) Mapping path when nMapLimit > 0 (iPg == 1 and iPg == 2)
    2) Non-mapping path when nMapLimit == 0 (iPg == 1)
  - Static linkage details of the original project are preserved; the tests exercise the actual focal method
    through its public entry point and rely on the project’s internal implementations for correctness.

  Assumptions:
  - The project provides lsmInt.h (or equivalent) exposing FileSystem, MetaPage, lsmFsMetaPageGet,
    lsmFsMetaPageRelease, and the constant LSM_OK, with C linkage.
  - The test environment links against the original lsm_file.c (or a build that includes it) so that
    static helper functions used inside lsmFsMetaPageGet behave as in production.

  How to run (example):
  - Compile with C++11, linking against the project library:
      g++ -std=c++11 -Ipath_to_headers test_lsmmeta.cpp -o test_lsmmeta -Lpath_to_lib -llsm
  - Run: ./test_lsmmeta

  The test code below includes explanatory comments for each unit test.
*/

#include <sys/types.h>
#include <lsmInt.h>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <cstdlib>


// Import project dependencies (adjust include path as necessary)
extern "C" {
  // The project is expected to provide the following types and functions:
  // - typedef struct FileSystem FileSystem;
  // - typedef struct MetaPage MetaPage;
  // - int lsmFsMetaPageGet(FileSystem *pFS, int bWrite, int iPg, MetaPage **ppPg);
  // - int lsmFsMetaPageRelease(MetaPage *pPg);
  // - LSM_OK macro or constant indicating success
  // - lsm_env type and related fields used by FileSystem
  #include "lsmInt.h"  // Adjust path if necessary

  // In case the project uses naming that requires explicit prototypes:
  int lsmFsMetaPageGet(FileSystem *pFS, int bWrite, int iPg, MetaPage **ppPg);
  int lsmFsMetaPageRelease(MetaPage *pPg);
}

// Lightweight test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define CHECK(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "[FAIL] " << __FUNCTION__ << ":" << __LINE__ << " - " << (msg) << "\n"; \
    ++g_failed_tests; \
  } \
} while(0)

#define RUN_TEST(fn) do { ++g_total_tests; fn(); } while(0)

// Test 1: Mapping path (nMapLimit > 0), iPg = 1
void test_lsmFsMetaPageGet_MapPath_Ipg1_Read() {
  // Setup a FileSystem with mapping enabled
  FileSystem fs;
  std::memset(&fs, 0, sizeof(FileSystem));

  // Create a dummy environment object (lsm_env). The exact internal layout is provided by the
  // project's headers; we just allocate a placeholder to satisfy pointer initialization.
  // Note: The real lsmMallocZeroRc used inside may also allocate based on pEnv, but for this test
  // we primarily verify the aData mapping behavior.
  lsm_env *env = reinterpret_cast<lsm_env*>(std::malloc(sizeof(lsm_env)));
  fs.pEnv = env;

  // Configure metasize and mapping
  fs.nMetasize = 128;        // arbitrary metasize
  fs.nMapLimit = 1;           // enable mapping path
  // Allocate a map big enough for 2 metasizes
  fs.pMap = reinterpret_cast<u8*>(std::malloc(2 * fs.nMetasize));
  fs.nMetaRwSize = 8;         // read size used when reading un-mapped
  fs.fdDb = 0;                // not used in mapping path

  ppPg = nullptr; // ensure compile context
  MetaPage *pPg = nullptr;

  // Call focal method for iPg=1, bWrite=false
  int rc = lsmFsMetaPageGet(&fs, 0, 1, &pPg);

  // Assertions
  CHECK(rc == LSM_OK, "rc must be LSM_OK for mapping path iPg=1");
  CHECK(pPg != nullptr, "pPg must be non-null after mapping path allocation");
  CHECK(pPg->iPg == 1, "pPg should reflect iPg=1");
  CHECK(pPg->bWrite == 0, "pPg should reflect bWrite=0");
  CHECK(pPg->pFS == &fs, "pPg->pFS must point to the original FileSystem");
  CHECK(pPg->aData == fs.pMap, "pPg->aData should point to pMap+0 for iPg=1");

  // Cleanup
  if (pPg) {
    lsmFsMetaPageRelease(pPg);
  }
  // free mapping and environment
  if (fs.pMap) std::free(fs.pMap);
  if (fs.pEnv) std::free(fs.pEnv);
}

// Test 2: Mapping path (nMapLimit > 0), iPg = 2
void test_lsmFsMetaPageGet_MapPath_Ipg2_Read() {
  // Setup a FileSystem with mapping enabled
  FileSystem fs;
  std::memset(&fs, 0, sizeof(FileSystem));

  lsm_env *env = reinterpret_cast<lsm_env*>(std::malloc(sizeof(lsm_env)));
  fs.pEnv = env;

  fs.nMetasize = 128;
  fs.nMapLimit = 1;
  fs.pMap = reinterpret_cast<u8*>(std::malloc(2 * fs.nMetasize));
  fs.nMetaRwSize = 8;
  fs.fdDb = 0;

  MetaPage *pPg = nullptr;
  int rc = lsmFsMetaPageGet(&fs, 0, 2, &pPg);

  CHECK(rc == LSM_OK, "rc must be LSM_OK for mapping path iPg=2");
  CHECK(pPg != nullptr, "pPg must be non-null after mapping path allocation");
  CHECK(pPg->iPg == 2, "pPg should reflect iPg=2");
  CHECK(pPg->bWrite == 0, "pPg should reflect bWrite=0");
  CHECK(pPg->pFS == &fs, "pPg->pFS must point to the original FileSystem");
  CHECK(pPg->aData == fs.pMap + fs.nMetasize, "pPg->aData should point to pMap + nMetasize for iPg=2");

  if (pPg) lsmFsMetaPageRelease(pPg);
  if (fs.pMap) std::free(fs.pMap);
  if (fs.pEnv) std::free(fs.pEnv);
}

// Test 3: No-mapping path (nMapLimit == 0) with iPg = 1
void test_lsmFsMetaPageGet_NoMap_Ipg1_NoMapPath() {
  // Setup a FileSystem without mapping
  FileSystem fs;
  std::memset(&fs, 0, sizeof(FileSystem));

  lsm_env *env = reinterpret_cast<lsm_env*>(std::malloc(sizeof(lsm_env)));
  fs.pEnv = env;

  fs.nMetasize = 256;
  fs.nMapLimit = 0;  // no mapping
  fs.pMap = nullptr;
  fs.nMetaRwSize = 8;
  fs.fdDb = 0; // no actual disk io in mapping path

  MetaPage *pPg = nullptr;
  int rc = lsmFsMetaPageGet(&fs, 1, 1, &pPg);

  // rc should still be OK in ideal path, and aData should be allocated by lsmMallocRc
  CHECK(rc == LSM_OK, "rc must be LSM_OK for no-map path iPg=1");
  CHECK(pPg != nullptr, "pPg must be non-null for no-map path");
  CHECK(pPg->iPg == 1, "pPg should reflect iPg=1");
  CHECK(pPg->bWrite == 1, "pPg should reflect bWrite=1");
  CHECK(pPg->pFS == &fs, "pPg->pFS must point to the original FileSystem");
  CHECK(pPg->aData != nullptr, "pPg->aData must be non-null for no-map path");

  if (pPg) lsmFsMetaPageRelease(pPg);
  // Free dummy environment
  if (fs.pEnv) std::free(fs.pEnv);
}

// Test runner
int main() {
  // Run tests
  RUN_TEST(test_lsmFsMetaPageGet_MapPath_Ipg1_Read);
  RUN_TEST(test_lsmFsMetaPageGet_MapPath_Ipg2_Read);
  RUN_TEST(test_lsmFsMetaPageGet_NoMap_Ipg1_NoMapPath);

  // Summary
  std::cout << "Tests run: " << g_total_tests << "  Failures: " << g_failed_tests << "\n";
  return g_failed_tests != 0 ? 1 : 0;
}