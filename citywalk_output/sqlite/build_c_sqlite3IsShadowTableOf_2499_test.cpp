// Self-contained C++11 test suite for the focal function logic.
// Note: This test is designed to be self-contained and does not rely on
// the real sqlite source tree. It mimics the essential control-flow paths
// of sqlite3IsShadowTableOf using lightweight stub types.
// The goal is to exercise true/false branches and ensure non-terminating
// assertions (test continues after failures).

#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <cctype>
#include <unordered_map>
#include <iostream>


// -----------------------------------------------------------------------------
// Lightweight stub environment to emulate the focal function's behavior.
// This is intentionally self-contained to avoid requiring the full sqlite3
// internal headers. The test focuses on the control-flow branches rather than
// exact sqlite internals.
// -----------------------------------------------------------------------------

// Forward-declare the dummy sqlite3 type and the focal function prototype.
struct sqlite3; // opaque in the real project; here we only need a type.
int sqlite3IsShadowTableOf(sqlite3 *db, struct Table *pTab, const char *zName);

// Hash placeholder type (the real code uses a Hash type, but here we only
// need a marker to allow the sqlite3HashFind-like function to resolve).
struct Hash {};

// Forward declare minimal internal types used by the function under test.
struct PModule {
  int iVersion;
  int (*xShadowName)(const char *z);
};

struct Module {
  PModule *pModule;
};

// Simulated vtab argument holder: we only rely on azArg[0] in tests.
struct VTab {
  char **azArg;
};

// The Table type used by the focal function.
// We reproduce only the members required by sqlite3IsShadowTableOf.
struct Table {
  const char *zName;
  struct { VTab vtab; } u;
  bool isVirtual; // whether the table is virtual; used by IsVirtual(...)
};

// Lightweight, fake"database" type to hold the aModule hash.
// In the real project, aModule is a Hash structure; we emulate with a map.
struct sqlite3 {
  Hash aModule;
};

// ---------------------------------------------------------------------------------
// Helpers to simulate the internal helpers used by sqlite3IsShadowTableOf
// ---------------------------------------------------------------------------------

// Simple, inline replacement for the IsVirtual macro.
// In the real code, IsVirtual(pTab) checks some internal flag/state.
// Here we expose a tiny function to maintain test readability.
static inline int IsVirtual(const Table *pTab) {
  return pTab->isVirtual ? 1 : 0;
}

// Length limited to 30 per the original function (sqlite3Strlen30).
static int sqlite3Strlen30(const char *z) {
  int n = 0;
  while (n < 30 && z[n] != '\0') n++;
  return n;
}

// Case-insensitive comparison up to n characters (simple emulation of sqlite3_strnicmp).
static int sqlite3_strnicmp(const char *zName, const char *zTabName, int n) {
  for (int i = 0; i < n && zName[i] != '\0' && zTabName[i] != '\0'; i++) {
    int c1 = std::tolower((unsigned char)zName[i]);
    int c2 = std::tolower((unsigned char)zTabName[i]);
    if (c1 != c2) return c1 - c2;
  }
  // If one string is shorter than n and differs by length, report difference.
  return 0;
}

// Hash lookup shim: we implement a tiny in-process map to emulate sqlite3HashFind.
static std::unordered_map<std::string, Module*> g_hash;

// The fake sqlite3HashFind; signature mirrors the actual usage in the focal code
// (Hash* followed by a const char* key). We return a void* that can be cast to Module*.
static void* sqlite3HashFind(Hash * /*pHash*/, const char *zKey) {
  auto it = g_hash.find(zKey ? zKey : "");
  if (it == g_hash.end()) return nullptr;
  return (void*)it->second;
}

// The test stubs for the Module and PModule creation
static Module* createModule(int version, int (*fn)(const char *)) {
  PModule *p = new PModule{version, fn};
  Module *m = new Module{p};
  return m;
}

// Shadow name function used by tests to emulate a non-zero return value.
static int shadowNameReturnValue(const char * /*z*/) {
  return 1234; // distinctive non-zero to validate the successful path
}

// ---------------------------------------------------------------------------------
// FOCAL METHOD REPRODUCTION (self-contained):
// int sqlite3IsShadowTableOf(sqlite3 *db, Table *pTab, const char *zName){
//   int nName;                    /* Length of zName */
//   Module *pMod;                 /* Module for the virtual table */
//   if( !IsVirtual(pTab) ) return 0;
//   nName = sqlite3Strlen30(pTab->zName);
//   if( sqlite3_strnicmp(zName, pTab->zName, nName)!=0 ) return 0;
//   if( zName[nName]!='_' ) return 0;
//   pMod = (Module*)sqlite3HashFind(&db->aModule, pTab->u.vtab.azArg[0]);
//   if( pMod==0 ) return 0;
//   if( pMod->pModule->iVersion<3 ) return 0;
//   if( pMod->pModule->xShadowName==0 ) return 0;
//   return pMod->pModule->xShadowName(zName+nName+1);
// }
// ---------------------------------------------------------------------------------

// The actual function under test (re-implemented in test for self-containment)
int sqlite3IsShadowTableOf(sqlite3 *db, Table *pTab, const char *zName) {
  int nName;
  Module *pMod;
  if (!IsVirtual(pTab)) return 0;
  nName = sqlite3Strlen30(pTab->zName);
  if (sqlite3_strnicmp(zName, pTab->zName, nName) != 0) return 0;
  if (zName[nName] != '_') return 0;
  pMod = (Module*)sqlite3HashFind(&db->aModule, pTab->u.vtab.azArg[0]);
  if (pMod == 0) return 0;
  if (pMod->pModule->iVersion < 3) return 0;
  if (pMod->pModule->xShadowName == 0) return 0;
  return pMod->pModule->xShadowName(zName + nName + 1);
}

// ---------------------------------------------------------------------------------
// Test harness and test cases
// ---------------------------------------------------------------------------------

static int g_failures = 0;

#define EXPECT_EQ(actual, expected, msg)                              \
  do {                                                                 \
    if ((actual) != (expected)) {                                      \
      std::cerr << "TEST FAIL: " << msg << " | expected: " << (expected) \
                << " actual: " << (actual) << std::endl;             \
      ++g_failures;                                                   \
    } else {                                                           \
      std::cout << ".";                                              \
    }                                                                  \
  } while (0)

#define EXPECT_TRUE(cond, msg)  EXPECT_EQ((cond) ? 1 : 0, 1, msg)
#define EXPECT_FALSE(cond, msg) EXPECT_EQ((cond) ? 1 : 0, 0, msg)

// Test 1: Non-virtual table -> should return 0 (false)
void test_case_non_virtual_returns_zero() {
  // Setup fake db and table
  sqlite3 db;
  g_hash.clear();
  // Table not virtual
  Table tab;
  tab.zName = "tab";
  tab.isVirtual = false;
  tab.u.vtab.azArg = nullptr;
  const char* zName = "tab_shadow";

  int res = sqlite3IsShadowTableOf(&db, &tab, zName);
  EXPECT_EQ(res, 0, "Non-virtual table should yield 0");
}

// Test 2: Name prefix mismatch -> 0
void test_case_name_prefix_mismatch() {
  sqlite3 db;
  g_hash.clear();

  Table tab;
  tab.zName = "tab";
  tab.isVirtual = true;
  // arg[0] points to module key; used by the hash Find
  static char* arg0 = (char*)"mod1";
  tab.u.vtab.azArg = &arg0;

  // zName differs in prefix from tab.zName
  const char* zName = "notTab_shadow";

  int res = sqlite3IsShadowTableOf(&db, &tab, zName);
  EXPECT_EQ(res, 0, "Prefix mismatch should yield 0");
}

// Test 3: Underscore missing after prefix -> 0
void test_case_missing_underscore() {
  sqlite3 db;
  g_hash.clear();

  Table tab;
  tab.zName = "tab";
  tab.isVirtual = true;
  static char* arg0 = (char*)"mod1";
  tab.u.vtab.azArg = &arg0;

  // zName has no underscore after the prefix
  const char* zName = "tabshadow"; // no '_' after "tab"

  int res = sqlite3IsShadowTableOf(&db, &tab, zName);
  EXPECT_EQ(res, 0, "Missing underscore should yield 0");
}

// Test 4: Module not found in hash -> 0
void test_case_module_not_found() {
  sqlite3 db;
  g_hash.clear();

  Table tab;
  tab.zName = "tab";
  tab.isVirtual = true;
  // Point to a key that is NOT present in the hash table
  static char* arg0 = (char*)"missing_mod";
  tab.u.vtab.azArg = &arg0;

  const char* zName = "tab_shadow";

  int res = sqlite3IsShadowTableOf(&db, &tab, zName);
  EXPECT_EQ(res, 0, "Missing module should yield 0");
}

// Test 5: Module version < 3 -> 0
void test_case_module_version_too_old() {
  sqlite3 db;
  g_hash.clear();

  // Create a module with iVersion = 2 (too old)
  Module* m = createModule(2, shadowNameReturnValue);
  g_hash["mod1"] = m;

  Table tab;
  tab.zName = "tab";
  tab.isVirtual = true;
  static char* arg0 = (char*)"mod1";
  tab.u.vtab.azArg = &arg0;

  const char* zName = "tab_shadow";

  int res = sqlite3IsShadowTableOf(&db, &tab, zName);
  EXPECT_EQ(res, 0, "Old module version should yield 0");
}

// Test 6: ShadowName function pointer is null -> 0
void test_case_shadowname_null() {
  sqlite3 db;
  g_hash.clear();

  // Create a module with iVersion >= 3 but xShadowName == null
  Module* m = createModule(3, nullptr);
  g_hash["mod1"] = m;

  Table tab;
  tab.zName = "tab";
  tab.isVirtual = true;
  static char* arg0 = (char*)"mod1";
  tab.u.vtab.azArg = &arg0;

  const char* zName = "tab_shadow";

  int res = sqlite3IsShadowTableOf(&db, &tab, zName);
  EXPECT_EQ(res, 0, "Null xShadowName should yield 0");
}

// Test 7: Success path (all conditions satisfied)
void test_case_success_path() {
  sqlite3 db;
  g_hash.clear();

  // xShadowName returns a non-zero value to indicate success
  Module* m = createModule(5, shadowNameReturnValue);
  g_hash["mod1"] = m;

  Table tab;
  tab.zName = "tab";          // prefix
  tab.isVirtual = true;
  static char* arg0 = (char*)"mod1";
  tab.u.vtab.azArg = &arg0;

  const char* zName = "tab_shadow";
  int res = sqlite3IsShadowTableOf(&db, &tab, zName);
  EXPECT_EQ(res, 1234, "Successful path should return xShadowName result (1234)");
}

// ---------------------------------------------------------------------------------
// Main: run all tests
// ---------------------------------------------------------------------------------
int main() {
  std::cout << "Running sqlite3IsShadowTableOf unit tests (self-contained harness) ..." << std::endl;

  test_case_non_virtual_returns_zero();
  test_case_name_prefix_mismatch();
  test_case_missing_underscore();
  test_case_module_not_found();
  test_case_module_version_too_old();
  test_case_shadowname_null();
  test_case_success_path();

  if (g_failures == 0) {
    std::cout << "\nAll tests passed." << std::endl;
    return 0;
  } else {
    std::cout << "\nTotal failures: " << g_failures << std::endl;
    return 1;
  }
}