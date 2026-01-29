// Test suite for sqlite3VdbeParameterIndex
// This test is self-contained and does not rely on GTest.
// It mocks the necessary parts of the SQLite internal structures to exercise the focal method.
//
// FOCAL METHOD being tested (conceptual):
// int sqlite3VdbeParameterIndex(Vdbe *p, const char *zName, int nName){
//   if( p==0 || zName==0 ) return 0;
//   return sqlite3VListNameToNum(p->pVList, zName, nName);
// }
//
// The tests verify:
// - returns 0 when p is null
// - returns 0 when zName is null
// - forwards to sqlite3VListNameToNum for non-null inputs
// - mapping behavior via a mock sqlite3VListNameToNum
//
// Notes:
// - The test provides minimal mock implementations of Vdbe and VList, and a simple
//   mapping mechanism to simulate parameter name to index translation.
// - Tests are written in C++11 and use simple, non-terminating assertions.

#include <opcodes.h>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <map>
#include <unordered_map>
#include <cstring>
#include <iostream>


// Lightweight mock definitions to simulate SQLite's internal types
typedef struct VList {
  // Intentionally minimal; real internals are not required for this test
  int dummy;
} VList;

typedef struct Vdbe {
  VList *pVList;
} Vdbe;

// Global mapping storage to simulate name-to-index resolution for VList.
// We key on the address of the VList instance to allow multiple VList instances
// with independent mappings.
static std::unordered_map<void*, std::map<std::string,int>> g_vlist_mapping;

// Helper to register a mapping for a given VList instance.
static void registerVListMapping(VList *pList, const char* zName, int index) {
  if(pList==nullptr || zName==nullptr) return;
  std::string key(zName);
  g_vlist_mapping[(void*)pList][key] = index;
}

// Mock of the original SQLite helper:
// int sqlite3VListNameToNum(VList *pList, const char *zName, int nName);
extern "C" int sqlite3VListNameToNum(VList *pList, const char *zName, int nName) {
  if(pList==nullptr || zName==nullptr) {
    // Align with the focal function's expectation when inputs are null
    return 0;
  }
  auto itList = g_vlist_mapping.find((void*)pList);
  if(itList == g_vlist_mapping.end()) {
    // No mapping defined for this VList instance
    return -1;
  }
  const std::map<std::string,int>& mp = itList->second;
  // Respect the provided length nName (truncate if necessary)
  size_t len = (nName > 0) ? static_cast<size_t>(nName) : std::strlen(zName);
  std::string key(zName, len);
  auto it = mp.find(key);
  if(it == mp.end()) {
    return -1;
  }
  return it->second;
}

// Focal method under test (adapted for unit test within this translation unit)
extern "C" int sqlite3VdbeParameterIndex(Vdbe *p, const char *zName, int nName) {
  if( p==nullptr || zName==nullptr ) return 0;
  return sqlite3VListNameToNum(p->pVList, zName, nName);
}

// Simple assertion helper that does not terminate on failure.
// It reports failures and continues execution to maximize code-path coverage.
static int g_failures = 0;
#define ASSERT_EQ(actual, expected, msg) do { \
  if((actual) != (expected)) { \
    std::cerr << "[FAIL] " << msg << " | Expected " << (expected) \
              << ", got " << (actual) << std::endl; \
    ++g_failures; \
  } else { \
    std::cout << "[OK] " << msg << std::endl; \
  } \
} while(0)

static void run_null_pointer_tests() {
  // Test: p == nullptr -> should return 0
  Vdbe dummy; dummy.pVList = nullptr;
  int res = sqlite3VdbeParameterIndex(nullptr, "param", -1);
  ASSERT_EQ(res, 0, "sqlite3VdbeParameterIndex(nullptr, 'param', -1) returns 0");

  // Test: zName == nullptr -> should return 0
  res = sqlite3VdbeParameterIndex(&dummy, nullptr, 0);
  ASSERT_EQ(res, 0, "sqlite3VdbeParameterIndex(&dummy, nullptr, 0) returns 0");
}

static void run_mapping_forwarding_test() {
  // Prepare a VList with a known mapping and a Vdbe referencing it
  VList list;
  Vdbe vdbe;
  vdbe.pVList = &list;

  // No mapping yet; expect -1 from the mock when name not found
  int res = sqlite3VdbeParameterIndex(&vdbe, "paramA", 6);
  ASSERT_EQ(res, -1, "Unmapped VList returns -1 for unknown name");

  // Register mappings and verify forward to sqlite3VListNameToNum with exact name
  registerVListMapping(&list, "paramA", 3);
  res = sqlite3VdbeParameterIndex(&vdbe, "paramA", 6);
  ASSERT_EQ(res, 3, "Mapped name 'paramA' resolves to index 3 (full-length name)");

  // Test with truncation (nName shorter than actual name)
  // Add mapping for the truncated key "param" and verify truncation behavior
  registerVListMapping(&list, "param", 42);
  res = sqlite3VdbeParameterIndex(&vdbe, "paramA", 4); // length 4 -> "param"
  // Should return 42 due to truncation to "param"
  ASSERT_EQ(res, 42, "Truncated name 'paramA' with nName=4 resolves to 42 via truncation");

  // Also verify that exact longer name still resolves
  res = sqlite3VdbeParameterIndex(&vdbe, "paramA", 6); // full "paramA" length used
  ASSERT_EQ(res, 3, "Exact name 'paramA' resolves to index 3 when nName=6");
}

static void run_multiple_vlist_instances_test() {
  // Ensure multiple VList instances maintain independent mappings
  VList a, b;
  Vdbe va, vb;
  va.pVList = &a;
  vb.pVList = &b;

  registerVListMapping(&a, "alpha", 1);
  registerVListMapping(&b, "beta", 2);

  int resA = sqlite3VdbeParameterIndex(&va, "alpha", 5);
  int resB = sqlite3VdbeParameterIndex(&vb, "beta", 4);

  ASSERT_EQ(resA, 1, "VList instance A maps 'alpha' to 1");
  ASSERT_EQ(resB, 2, "VList instance B maps 'beta' to 2");

  // Ensure cross-instance name not found
  int resCross = sqlite3VdbeParameterIndex(&va, "beta", 4);
  ASSERT_EQ(resCross, -1, "Cross-instance lookup for 'beta' on A returns -1");
}

int main() {
  std::cout << "Starting sqlite3VdbeParameterIndex unit tests (C++11, self-contained mocks)" << std::endl;

  run_null_pointer_tests();
  run_mapping_forwarding_test();
  run_multiple_vlist_instances_test();

  if(g_failures == 0) {
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  } else {
    std::cerr << g_failures << " test(s) FAILED" << std::endl;
    return 1;
  }
}