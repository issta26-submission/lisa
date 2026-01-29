// test_lsm_fsopen.cpp
// A lightweight, non-GTest style unit test suite for the focal method lsmFsOpen.
// This harness uses a minimal in-process test runner with non-fatal (EXPECT_*) assertions
// so that multiple tests can execute within a single run.
//
// Note: This test assumes the presence of the project under test (the LSM library) and its
// public API headers (e.g., lsmFsOpen, lsm_db, lsm_env, FileSystem, etc.).
// It should be compiled and linked against the real library sources in the target environment.

#include <sys/types.h>
#include <lsmInt.h>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstdlib>


extern "C" {
  // Include the headers that declare lsmFsOpen and related data structures.
  // The actual paths depend on the project layout.
  #include "lsmInt.h"    // Internal structures/types (if available in public headers)
  #include "lsm_file.h"  // Declarations for lsmFsOpen and FileSystem, lsm_db, etc.
}

// Lightweight non-fatal assertion helpers (non-terminating)
static int g_test_total = 0;
static int g_test_failed = 0;

#define EXPECT_TRUE(expr) do { \
  ++g_test_total; \
  if(!(expr)) { \
    printf("EXPECT_TRUE FAILED: %s:%d: %s\n", __FILE__, __LINE__, #expr); \
    ++g_test_failed; \
  } \
} while(0)

#define EXPECT_EQ(a, b) do { \
  ++g_test_total; \
  if((a) != (b)) { \
    printf("EXPECT_EQ FAILED: %s:%d: %s == %lld (expected) != %lld (actual)\n", __FILE__, __LINE__, #a, (long long)(a), (long long)(b)); \
    ++g_test_failed; \
  } \
} while(0)

#define EXPECT_STREQ(a, b) do { \
  ++g_test_total; \
  if(((a) == NULL) || ((b) == NULL) || strcmp((a), (b)) != 0) { \
    printf("EXPECT_STREQ FAILED: %s:%d: \"%s\" != \"%s\"\n", __FILE__, __LINE__, (a ? (a) : "NULL"), (b ? (b) : "NULL")); \
    ++g_test_failed; \
  } \
} while(0)

#define EXPECT_STRNE(a, b) do { \
  ++g_test_total; \
  if(((a) != NULL) && ((b) != NULL) && strcmp((a), (b)) == 0) { \
    printf("EXPECT_STRNE FAILED: %s:%d: strings are equal: \"%s\" == \"%s\"\n", __FILE__, __LINE__, (a), (b)); \
    ++g_test_failed; \
  } \
} while(0)

// Forward declarations of test cases
static void test_lsmFsOpen_basic_opens_db();
static void test_lsmFsOpen_log_name_construction();

// Entry point
int main() {
  printf("Running tests for lsmFsOpen...\n");

  test_lsmFsOpen_basic_opens_db();
  test_lsmFsOpen_log_name_construction();

  printf("Tests completed: %d total, %d failed\n", g_test_total, g_test_failed);
  // Return non-zero if any test failed
  return (g_test_failed != 0) ? 1 : 0;
}

// Test 1: Basic open path should initialize a FileSystem, copy db name, and set fields.
// Rationale: Validates that with minimal environment, lsmFsOpen allocates FileSystem,
// stores the database path, derives the log path correctly, and associates the DB.
static void test_lsmFsOpen_basic_opens_db() {
  printf("[Test] test_lsmFsOpen_basic_opens_db: Basic open path and initialization\n");

  // Prepare a minimal fake database context
  lsm_db *pDb = (lsm_db*)calloc(1, sizeof(lsm_db));
  if(!pDb){
    printf("Memory allocation failed for lsm_db in test\n");
    g_test_failed++;
    g_test_total++;
    return;
  }

  // Provide a dummy environment. We rely on the library's own allocation mechanics (via pEnv).
  lsm_env *pEnv = (lsm_env*)calloc(1, sizeof(lsm_env));
  if(!pEnv){
    printf("Memory allocation failed for lsm_env in test\n");
    free(pDb);
    g_test_failed++;
    g_test_total++;
    return;
  }

  pDb->pEnv = pEnv;
  pDb->pFS = 0;
  pDb->pWorker = 0;
  pDb->pClient = 0;

  const char *zDb = "unit_test_db.db";

  int rc = lsmFsOpen(pDb, zDb, 0);

  // Expect success and a non-null FileSystem
  EXPECT_EQ(rc, LSM_OK);
  EXPECT_TRUE(pDb->pFS != 0);

  if(pDb->pFS != 0){
    // The FileSystem should reference back to the same pDb and pEnv
    EXPECT_TRUE(pDb->pFS->pDb == pDb);
    EXPECT_TRUE(pDb->pFS->pEnv == pEnv);

    // zDb should be copied including the terminating null character
    int nDb = strlen(zDb);
    EXPECT_STREQ(pDb->pFS->zDb, zDb);

    // zLog should be a concatenation of zDb and "-log" (no space, no extra terminator until end)
    // Check prefix equals zDb
    EXPECT_TRUE(strncmp(pDb->pFS->zLog, zDb, nDb) == 0);
    // Check the hyphen and "log" suffix
    EXPECT_TRUE(pDb->pFS->zLog[nDb] == '-');
    // The four characters after the '-' should spell "log" and terminate
    EXPECT_TRUE(strncmp(pDb->pFS->zLog + nDb, "-log", 4) == 0);
  }

  // Cleanup
  if(pDb->pFS) {
    lsmFsClose(pDb->pFS);
    pDb->pFS = 0;
  }
  free(pEnv);
  free(pDb);

  printf("[Test] test_lsmFsOpen_basic_opens_db: completed\n");
}

// Test 2: Ensure that the log path construction is correct for various db names.
// Rationale: Validates the exact string composition for zLog using the "-log" suffix.
static void test_lsmFsOpen_log_name_construction() {
  printf("[Test] test_lsmFsOpen_log_name_construction: Verifies zLog composition with -log suffix\n");

  lsm_db *pDb = (lsm_db*)calloc(1, sizeof(lsm_db));
  if(!pDb){
    printf("Memory allocation failed for lsm_db in test\n");
    g_test_failed++;
    g_test_total++;
    return;
  }

  lsm_env *pEnv = (lsm_env*)calloc(1, sizeof(lsm_env));
  if(!pEnv){
    printf("Memory allocation failed for lsm_env in test\n");
    free(pDb);
    g_test_failed++;
    g_test_total++;
    return;
  }

  pDb->pEnv = pEnv;
  pDb->pFS = 0;
  pDb->pWorker = 0;
  pDb->pClient = 0;

  // Use a different database name to verify string composition
  const char *zDb = "path/to/another_db.sqlite";
  int rc = lsmFsOpen(pDb, zDb, 0);
  EXPECT_EQ(rc, LSM_OK);
  EXPECT_TRUE(pDb->pFS != 0);

  if(pDb->pFS){
    int nDb = strlen(zDb);
    // zDb should be exactly the input string
    EXPECT_STREQ(pDb->pFS->zDb, zDb);

    // zLog should copy zDb prefix and append "-log"
    // Validate prefix
    EXPECT_TRUE(strncmp(pDb->pFS->zLog, zDb, nDb) == 0);
    // Validate suffix
    EXPECT_TRUE(pDb->pFS->zLog[nDb] == '-');
    EXPECT_TRUE(strncmp(pDb->pFS->zLog + nDb, "-log", 4) == 0);
  }

  if(pDb->pFS) {
    lsmFsClose(pDb->pFS);
    pDb->pFS = 0;
  }
  free(pEnv);
  free(pDb);

  printf("[Test] test_lsmFsOpen_log_name_construction: completed\n");
}