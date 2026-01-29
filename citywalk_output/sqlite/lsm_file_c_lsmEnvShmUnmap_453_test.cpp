/*
Unit tests for the focal method:
  void lsmEnvShmUnmap(lsm_env *pEnv, lsm_file *pFile, int bDel)

What we test:
- lsmEnvShmUnmap should forward the call to pEnv->xShmUnmap(pFile, bDel).
- We verify that the exact pFile pointer and the exact bDel value are passed through.

Notes:
- lsm_env.xShmUnmap is a function pointer; we provide a fake callback to record the arguments.
- lsm_file is an opaque type in this test; we allocate a tiny dummy object and cast to lsm_file* to avoid requiring a full definition.

This test is written for a C++11 environment without Google Test. It uses a small, non-terminating assertion style (EXPECT-like helpers) and a minimal test harness.
*/

#include <sys/types.h>
#include <lsmInt.h>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>



// Global state to capture callback invocations
static lsm_file *g_last_pFile = nullptr;
static int g_last_bDel = -1;
static bool g_called = false;

// Callback used by the fake lsm_env.xShmUnmap to record parameters
static void test_shmUnmap_cb(lsm_file *pFile, int bDel){
  g_last_pFile = pFile;
  g_last_bDel = bDel;
  g_called = true;
}

// Helpers for test assertions (non-terminating)
static void reset_cb_state() {
  g_last_pFile = nullptr;
  g_last_bDel = -1;
  g_called = false;
}

static void expect_true(bool cond, const char* msg) {
  if(!cond) {
    std::cerr << "[EXPECT_FALSE] " << msg << std::endl;
  }
}

static void expect_ptr_eq(const void* a, const void* b, const char* msg) {
  if(a != b) {
    std::cerr << "[EXPECT_PTR_EQ] " << msg
              << " (expected " << b << ", got " << a << ")" << std::endl;
  }
}

static void expect_int_eq(int a, int b, const char* msg) {
  if(a != b) {
    std::cerr << "[EXPECT_INT_EQ] " << msg
              << " (expected " << b << ", got " << a << ")" << std::endl;
  }
}

// Test 1: Forwarding the callback and parameters (basic forwarding correctness)
static void test_lsmEnvShmUnmap_ForwardsCallback() {
  reset_cb_state();

  // Prepare environment with the fake callback
  lsm_env env;
  env.xShmUnmap = &test_shmUnmap_cb;

  // Create a dummy lsm_file instance (opaque type; a tiny allocated region suffices)
  lsm_file *pFile = reinterpret_cast<lsm_file*>(new char[1]);

  int delValue = 7;
  lsmEnvShmUnmap(&env, pFile, delValue);

  // Assertions: callback should have been invoked with exact parameters
  expect_true(g_called, "Callback should be invoked by lsmEnvShmUnmap");
  expect_ptr_eq((const void*)g_last_pFile, (const void*)pFile, "Callback should receive the same pFile pointer");
  expect_int_eq(g_last_bDel, delValue, "Callback should receive the same bDel value");

  delete[] reinterpret_cast<char*>(pFile);
}

// Test 2: Forwarding with a different parameter set (ensures multiple calls work)
static void test_lsmEnvShmUnmap_WithDifferentParams() {
  reset_cb_state();

  lsm_env env;
  env.xShmUnmap = &test_shmUnmap_cb;

  lsm_file *pFile = reinterpret_cast<lsm_file*>(new char[1]);
  int delValue = 0;
  lsmEnvShmUnmap(&env, pFile, delValue);

  // Assertions: callback should have been invoked again with new parameters
  expect_true(g_called, "Callback should be invoked for the second test");
  expect_ptr_eq((const void*)g_last_pFile, (const void*)pFile, "Second test: pFile pointer mismatch");
  expect_int_eq(g_last_bDel, delValue, "Second test: bDel value mismatch");

  delete[] reinterpret_cast<char*>(pFile);
}

// Simple test runner and summary
int main() {
  std::cout << "Running unit tests for lsmEnvShmUnmap...\n";

  test_lsmEnvShmUnmap_ForwardsCallback();
  test_lsmEnvShmUnmap_WithDifferentParams();

  std::cout << "Unit tests completed." << std::endl;
  return 0;
}