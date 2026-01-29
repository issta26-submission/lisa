// Automated unit tests for testRestoreDb in lsmtest6.c
// This test suite uses a lightweight, homegrown harness (no GTest).
// It intercepts memory allocation, deallocation, and file-copy operations
// by providing wrappers for testMallocPrintf, testFree, and copy_file.
// The focal method under test: testRestoreDb(const char *zFile, const char *zAux)

#include <lsmtest6.c>
#include <lsmtest.h>
#include <vector>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <string>
#include <set>
#include <iostream>
#include <cstdlib>


// ----------------------------------------------------------------------------
// Test harness scaffolding for interception
// ----------------------------------------------------------------------------

// Track each copy_file call made by testRestoreDb
struct CopyCall {
  std::string zFrom;
  std::string zTo;
  int isDatabase;
};

// Global trackers to verify behavior
static std::vector<CopyCall> g_copy_calls;
static std::vector<void*>    g_alloc_ptrs;    // pointers returned by testMallocPrintf
static std::set<void*>         g_freed_ptrs;    // pointers passed to testFree
static std::vector<std::string> g_alloc_strings;   // semantic log of allocated strings

// Forward declare wrappers with C linkage to match expectations inside lsmtest6.c
extern "C" {

  // Intercepted allocation function used by testRestoreDb
  char* myTestMallocPrintf(const char* fmt, ...);

  // Intercepted deallocation function used by testRestoreDb
  void myTestFree(void* p);

  // Intercepted file copy function used by testRestoreDb
  void myCopyFile(const char* zFrom, const char* zTo, int isDatabase);
}

// Macro redirections so the focal code uses our wrappers instead of the real ones
#define testMallocPrintf myTestMallocPrintf
#define testFree         myTestFree
#define copy_file        myCopyFile

// Implementation of wrappers

char* myTestMallocPrintf(const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  // Small fixed buffer for formatting; inputs are small in testRestoreDb
  char buf[4096];
  int len = vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);
  if(len < 0) len = 0;
  // Create a C-string to return (must be freed by testFree)
  char* p = (char*)malloc((size_t)len + 1);
  if(p) {
    memcpy(p, buf, (size_t)len);
    p[len] = '\0';
  }

  // Track allocations for verification
  g_alloc_ptrs.push_back(p);
  g_alloc_strings.emplace_back(buf, (size_t)len);
  return p;
}

void myTestFree(void* p)
{
  if(p) {
    g_freed_ptrs.insert(p);
  }
  free(p); // Always free to prevent real leaks in the test environment
}

void myCopyFile(const char* zFrom, const char* zTo, int isDatabase)
{
  CopyCall c;
  c.zFrom = zFrom ? zFrom : "";
  c.zTo = zTo ? zTo : "";
  c.isDatabase = isDatabase;
  g_copy_calls.push_back(c);
}

// ----------------------------------------------------------------------------
// Include the focal C file under test, while macro-wrapping the calls
// so that we intercept its interactions.
// ----------------------------------------------------------------------------


#undef testMallocPrintf
#undef testFree
#undef copy_file

// ----------------------------------------------------------------------------
// Minimal, file-local test helpers
// ----------------------------------------------------------------------------

static void reset_tracking() {
  g_copy_calls.clear();
  g_alloc_ptrs.clear();
  g_freed_ptrs.clear();
  g_alloc_strings.clear();
}

// Helper to determine if all allocated pointers were freed
static bool all_allocations_freed() {
  if(g_alloc_ptrs.empty()) return true;
  for(void* p : g_alloc_ptrs) {
    if(g_freed_ptrs.find(p) == g_freed_ptrs.end()) return false;
  }
  return true;
}

// Helper: access to the focal function
extern "C" void testRestoreDb(const char *zFile, const char *zAux); // declared for linkage

// ----------------------------------------------------------------------------
// Unit Tests
// Each test is self-contained and prints its own result log.
// ----------------------------------------------------------------------------

// Test 1: Basic behavior with non-empty zFile and zAux
static bool test_case_basic() {
  reset_tracking();
  testRestoreDb("db1", "aux1");

  // Expected copy_file calls:
  // 1) from "db1-save" to "db1" (isDatabase = 1)
  // 2) from "db1-aux1-save" to "db1-aux1" (isDatabase = 0)
  bool ok = true;

  if(g_copy_calls.size() != 2) ok = false;
  else {
    if(!(g_copy_calls[0].zFrom == "db1-save" &&
           g_copy_calls[0].zTo == "db1" &&
           g_copy_calls[0].isDatabase == 1)) ok = false;
    if(!(g_copy_calls[1].zFrom == "db1-aux1-save" &&
           g_copy_calls[1].zTo == "db1-aux1" &&
           g_copy_calls[1].isDatabase == 0)) ok = false;
  }

  // Check allocations and frees
  if(g_alloc_ptrs.size() != 3) ok = false;
  if(!all_allocations_freed()) ok = false;

  // Log diagnostic if failed
  if(!ok) {
    std::cerr << "[test_case_basic] FAILED\n";
    std::cerr << "  copy_file calls: " << g_copy_calls.size() << "\n";
    for(size_t i=0; i<g_copy_calls.size(); ++i) {
      std::cerr << "    ["<<i<<"] from="<<g_copy_calls[i].zFrom
                << " to="<<g_copy_calls[i].zTo
                << " isDatabase="<<g_copy_calls[i].isDatabase << "\n";
    }
  }

  return ok;
}

// Test 2: Edge-case with empty zAux (zAux = "")
static bool test_case_empty_aux() {
  reset_tracking();
  testRestoreDb("db2", "");

  bool ok = true;
  if(g_copy_calls.size() != 2) ok = false;
  else {
    if(!(g_copy_calls[0].zFrom == "db2-save" &&
           g_copy_calls[0].zTo == "db2" &&
           g_copy_calls[0].isDatabase == 1)) ok = false;
    if(!(g_copy_calls[1].zFrom == "db2--save" &&
           g_copy_calls[1].zTo == "db2-" &&
           g_copy_calls[1].isDatabase == 0)) ok = false;
  }

  if(g_alloc_ptrs.size() != 3) ok = false;
  if(!all_allocations_freed()) ok = false;

  if(!ok) {
    std::cerr << "[test_case_empty_aux] FAILED\n";
    std::cerr << "  copy_file calls: " << g_copy_calls.size() << "\n";
  }

  return ok;
}

// Test 3: Complex paths with spaces and special chars
static bool test_case_complex_paths() {
  reset_tracking();
  testRestoreDb("/tmp/db with spaces/complex-db", "ver-2.0");

  bool ok = true;
  if(g_copy_calls.size() != 2) ok = false;
  else {
    if(!(g_copy_calls[0].zFrom == "/tmp/db with spaces/complex-db-save" &&
           g_copy_calls[0].zTo == "/tmp/db with spaces/complex-db" &&
           g_copy_calls[0].isDatabase == 1)) ok = false;
    if(!(g_copy_calls[1].zFrom == "/tmp/db with spaces/complex-db-ver-2.0-save" &&
           g_copy_calls[1].zTo == "/tmp/db with spaces/complex-db-ver-2.0" &&
           g_copy_calls[1].isDatabase == 0)) ok = false;
  }

  if(g_alloc_ptrs.size() != 3) ok = false;
  if(!all_allocations_freed()) ok = false;

  if(!ok) {
    std::cerr << "[test_case_complex_paths] FAILED\n";
  }

  return ok;
}

// ----------------------------------------------------------------------------
// Test Driver
// ----------------------------------------------------------------------------

int main() {
  bool t1 = test_case_basic();
  bool t2 = test_case_empty_aux();
  bool t3 = test_case_complex_paths();

  std::cout << "Test results: "
            << (t1 ? "PASS" : "FAIL") << " / "
            << (t2 ? "PASS" : "FAIL") << " / "
            << (t3 ? "PASS" : "FAIL") << std::endl;

  if(t1 && t2 && t3) {
    std::cout << "All tests passed.\n";
    return 0;
  } else {
    std::cerr << "Some tests failed. See logs above for details.\n";
    return 1;
  }
}