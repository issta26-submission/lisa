// High-quality C++11 unit test suite for the focal method testMallocCheck
// This test suite is designed to work with the existing C code under test
// (lsm.h and related implementation). It uses a lightweight, non-terminating
// assertion approach since GTest is not allowed per instructions.
// Each test is explained via comments and uses extern "C" to call the focal function.

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <assert.h>
#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <lsm.h>


// Forward declarations to integrate with the existing C code under test.
// The focal method testMallocCheck is provided in the project (lsmtest_mem.c).
extern "C" {
// Opaque C structs/types assumed to exist in the project headers.
// The tests rely on lsm_env and TmGlobal/TmBlockHdr as defined by the
// project's lsm.h and related headers.
typedef struct lsm_env lsm_env;
typedef struct TmGlobal TmGlobal;
typedef struct TmBlockHdr TmBlockHdr;

// The focal function under test (non-static in the project).
void testMallocCheck(lsm_env *pEnv, int *pnLeakAlloc, int *pnLeakByte, FILE *pFile);
}

// Simple non-terminating assertion mechanism.
// Tracks total checks and failures, but does not abort test execution.
static int g_total_checks = 0;
static int g_failures = 0;

#define TEST_ASSERT(cond, msg) \
  do { \
    ++g_total_checks; \
    if(!(cond)) { \
      std::cerr << "TEST FAILURE: " << msg << std::endl; \
      ++g_failures; \
    } \
  } while(0)

//
// Test 1: Null memory context should yield zero leaks
// - pEnv.pMemCtx == 0 -> testMallocCheck should set pnLeakAlloc/pnLeakByte to 0
//
void test_null_pMemCtx()
{
  lsm_env env;
  // Ensure memory context is null
  env.pMemCtx = 0;

  int leaks = -1;
  int bytes = -1;

  // Call the focal function with null mem context
  testMallocCheck(&env, &leaks, &bytes, nullptr);

  // Validate: both leak counts should be zero
  TEST_ASSERT(leaks == 0, "test_null_pMemCtx: leaks should be 0 when pMemCtx is null");
  TEST_ASSERT(bytes == 0, "test_null_pMemCtx: leak bytes should be 0 when pMemCtx is null");
}

//
// Test 2: Single block allocation reports correct leak stats
// - pEnv.pMemCtx non-null with a single TmBlockHdr in the list
// - Expect pnLeakAlloc == 1 and pnLeakByte == nByte of the single block
//
void test_single_block()
{
  // Set up a single block header
  TmBlockHdr block1;
  block1.pNext = nullptr;
  block1.nByte = 10; // 10 bytes leaked

  // Global context pointing to the single block
  TmGlobal g;
  g.pFirst = &block1;

  // Environment pointing to the above global context
  lsm_env env;
  env.pMemCtx = &g;

  int leaks = -1;
  int bytes = -1;

  testMallocCheck(&env, &leaks, &bytes, nullptr);

  TEST_ASSERT(leaks == 1, "test_single_block: expected 1 leak (one block in list)");
  TEST_ASSERT(bytes == 10, "test_single_block: expected 10 leaked bytes");
}

//
// Test 3: Multiple block allocations report correct cumulative leak stats
// - pEnv.pMemCtx non-null with a chain of two blocks
// - Expect pnLeakAlloc == 2 and pnLeakByte == sum of both blocks' nByte
//
void test_multiple_blocks()
{
  // Set up two block headers forming a list: block1 -> block2
  TmBlockHdr block2;
  block2.pNext = nullptr;
  block2.nByte = 5; // second block

  TmBlockHdr block1;
  block1.pNext = &block2;
  block1.nByte = 10; // first block

  // Global context pointing to the head of the list
  TmGlobal g;
  g.pFirst = &block1;

  // Environment pointing to the above global context
  lsm_env env;
  env.pMemCtx = &g;

  int leaks = -1;
  int bytes = -1;

  testMallocCheck(&env, &leaks, &bytes, nullptr);

  TEST_ASSERT(leaks == 2, "test_multiple_blocks: expected 2 leaks (two blocks in list)");
  TEST_ASSERT(bytes == 15, "test_multiple_blocks: expected 15 leaked bytes (10 + 5)");
}

//
// Test harness: run all tests and report summary
//
int main()
{
  std::cout << "Starting testMallocCheck unit tests (no GTest):\n";

  g_total_checks = 0;
  g_failures = 0;

  test_null_pMemCtx();
  test_single_block();
  test_multiple_blocks();

  std::cout << "Test summary: " << g_total_checks << " checks, "
            << g_failures << " failures." << std::endl;

  if (g_failures > 0) {
    std::cerr << "Some tests failed. See details above." << std::endl;
    return 1;
  }

  std::cout << "All tests passed." << std::endl;
  return 0;
}