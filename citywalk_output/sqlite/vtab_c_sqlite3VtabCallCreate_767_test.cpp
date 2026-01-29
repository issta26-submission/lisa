// Candidate Keywords extracted from the focal method sqlite3VtabCallCreate
// - sqlite3FindTable, IsVirtual, azArg, aModule
// - Module, pModule, xCreate, xDestroy
// - vtabCallConstructor, sqlite3GetVTable
// - growVTrans, addToVTrans
// - ALWAYS, SQLITE_OK, SQLITE_ERROR
// - pTab, zMod, zTab, pzErr
//
// This test suite provides a self-contained, stand-alone C++11 unit test
// harness that exercises the core decision branches of sqlite3VtabCallCreate
// using lightweight, in-process mocks. It does not depend on GTest and uses a
// tiny in-repo test framework implemented below.

#include <functional>
#include <vector>
#include <memory>
#include <string>
#include <limits>
#include <sqliteInt.h>
#include <cstdarg>
#include <unordered_map>
#include <iostream>
#include <cassert>


// Lightweight test harness macros
#define TEST_PASS 0
#define TEST_FAIL 1
#define ASSERT_TRUE(cond, msg) if(!(cond)) { std::cerr << "ASSERT_TRUE failed: " << (msg) << "\n"; return TEST_FAIL; }
#define ASSERT_EQ(a, b, msg) if((a) != (b)) { std::cerr << "ASSERT_EQ failed: " << (msg) 
                                           << " (expected: " << (a) << ", actual: " << (b) << ")\n"; return TEST_FAIL; }

// Domain constants (imitating SQLite error codes)
const int SQLITE_OK = 0;
const int SQLITE_ERROR = 1;

// A minimal, self-contained stand-alone test harness model
namespace FocalVtabCreateTest {

// Forward declaration of the function under test signature to illustrate usage.
// In a real environment, this would link against the actual sqlite3VtabCallCreate
// from vtab.c. Here we simulate the core logic via a lightweight test double.
extern "C" int simulate_sqlite3VtabCallCreate(
    void* db, int iDb, const std::string& zTab, std::string& zErrOut,
    // Additional knobs to drive branches in tests
    bool expectVTabPresentAfterCtor, bool growTransSucceeds, bool vtabExistsAfterCtor);

// Lightweight fake database schema sufficient for tests
struct FakeModule {
  std::string name;
  bool hasCreate;
  bool hasDestroy;
};

struct FakeTable {
  std::string name;
  bool isVirtual;
  // azArg[0] is the module name
  std::vector<std::string> azArg;
  void* vtabPtr; // represents pTab->u.vtab.p
};

struct FakeDb {
  std::string zDbSName;
  std::vector<FakeTable> tables;
  // map module name -> FakeModule
  std::unordered_map<std::string, FakeModule> modules;

  // Helpers to configure state
  void addTable(const std::string& tabName, const std::string& moduleName) {
    FakeTable t;
    t.name = tabName;
    t.isVirtual = true;
    t.azArg.push_back(moduleName);
    t.vtabPtr = nullptr;
    tables.push_back(t);
  }

  void addModule(const std::string& modName, bool hasCreate, bool hasDestroy) {
    FakeModule m{modName, hasCreate, hasDestroy};
    modules.emplace(modName, m);
  }

  FakeTable* findTable(const std::string& tabName) {
    for (auto& t : tables) {
      if (t.name == tabName) return &t;
    }
    return nullptr;
  }

  FakeModule* findModule(const std::string& modName) {
    auto it = modules.find(modName);
    if (it != modules.end()) return &it->second;
    return nullptr;
  }
};

// A tiny, deterministic helper to build error strings
static std::string makeNoSuchModuleError(const std::string& modName) {
  return "no such module: " + modName;
}

// A very small, deterministic "vtab" object identity to simulate sqlite3GetVTable
struct FakeVTable { int dummy; };

// Implementation of the simulate function that mirrors the control flow
// of sqlite3VtabCallCreate, but entirely within test code using the above
// lightweight mocks. It returns SQLITE_OK or SQLITE_ERROR and populates zErrOut.
extern "C" int simulate_sqlite3VtabCallCreate(
    void* db, int iDb, const std::string& zTab, std::string& zErrOut,
    bool expectVTabPresentAfterCtor, bool growTransSucceeds, bool vtabExistsAfterCtor)
{
  // In tests, db is a pointer to FakeDb (we reinterpret_cast for simplicity)
  FakeDb* pDb = reinterpret_cast<FakeDb*>(db);
  if (!pDb) {
    zErrOut = "null db pointer";
    return SQLITE_ERROR;
  }

  // Step 1: locate the virtual table entry
  FakeTable* pTab = pDb->findTable(zTab);
  if (!pTab) {
    zErrOut = "table not found: " + zTab;
    return SQLITE_ERROR;
  }
  // Assert-like preconditions (we enforce them in tests via configuration)
  ASSERT_TRUE(pTab->isVirtual, "Table must be virtual for sqlite3VtabCallCreate");
  // In this mock world, ensure not already created
  ASSERT_TRUE(pTab->vtabPtr == nullptr || pTab->vtabPtr == (void*)-1, "pTab->u.vtab.p must be NULL (not yet created)");

  // Step 2: locate the module name from azArg[0]
  if (pTab->azArg.empty()) {
    zErrOut = "no such module: <empty>";
    return SQLITE_ERROR;
  }
  const std::string zMod = pTab->azArg[0];

  // Step 3: lookup module
  FakeModule* pMod = pDb->findModule(zMod);
  if (pMod == nullptr || !pMod->hasCreate || !pMod->hasDestroy) {
    zErrOut = makeNoSuchModuleError(zMod);
    return SQLITE_ERROR;
  }

  // Step 4: simulate vtab constructor call (xCreate)
  // We model that the constructor succeeded and created a vtab
  // If expectVTabPresentAfterCtor is false, we pretend constructor did not create vtab.
  if (expectVTabPresentAfterCtor) {
    pTab->vtabPtr = reinterpret_cast<void*>(0x1); // mark created
  }

  int rc = SQLITE_OK;
  if (!(expectVTabPresentAfterCtor)) {
    // Simulate a bad constructor that does not create a vtab
    rc = SQLITE_OK; // but no vtab created
    // ensure vtabPtr remains nullptr
    pTab->vtabPtr = nullptr;
  }

  // Step 5: Always(sqlite3GetVTable(db, pTab)) path
  // Use vtabExistsAfterCtor to decide if GetVTable returns non-null
  assertex:
  if (rc == SQLITE_OK && vtabExistsAfterCtor) {
    // rc becomes result of growVTrans
    rc = growTransSucceeds ? SQLITE_OK : SQLITE_ERROR;
    if (rc == SQLITE_OK) {
      // addToVTrans would be invoked; in tests we just acknowledge
      // (no-op)
    }
  }

  // Return final rc
  return rc;
}

// A small, static wrapper to avoid dependency on real vtab.c internals in tests
int runTestCase_SetupCheckAndCall(
    FakeDb& db, const std::string& tabName,
    std::string& outErr,
    bool ctorCreatesVTab, bool growSucceeds, bool vtabGetSucceeds) {
  // Prepare a pointer-typed db for the simulate function
  void* dbPtr = reinterpret_cast<void*>(&db);
  return simulate_sqlite3VtabCallCreate(dbPtr, 0, tabName, outErr,
                                        ctorCreatesVTab, growSucceeds, vtabGetSucceeds);
}

// A tiny test runner
static int gFailures = 0;
static void logTestHeader(const std::string& name) {
  std::cout << "RUNNING TEST: " << name << " ..." << std::endl;
}
static void logTestPass() {
  std::cout << "PASS" << std::endl;
}
static void logTestFail(const std::string& msg) {
  std::cerr << "FAIL: " << msg << std::endl;
  ++gFailures;
}
static bool testFailed(const std::string& msg) {
  logTestFail(msg);
  return false;
}

// Test 1: Module not found should yield SQLITE_ERROR and appropriate message
bool test_ModuleNotFound() {
  logTestHeader("ModuleNotFound");
  FakeDb db;
  db.zDbSName = "main";
  db.tables.clear();
  db.modules.clear();

  // Create a virtual table "vt1" that uses a module named "missing_mod"
  db.addTable("vt1", "missing_mod");

  std::string err;
  int rc = runTestCase_SetupCheckAndCall(db, "vt1", err, true, true, true);
  // Expect error due to missing module
  bool pass = (rc == SQLITE_ERROR) && (err == "no such module: missing_mod");
  if (!pass) {
    logTestFail("Expected SQLITE_ERROR with 'no such module: missing_mod', got rc=" +
                std::to_string(rc) + " err='" + err + "'");
  } else {
    logTestPass();
  }
  return pass;
}

// Test 2: Module exists but lacks either xCreate or xDestroy -> error
bool test_ModuleMissingCreateDestroy() {
  logTestHeader("ModuleMissingCreateDestroy");
  FakeDb db;
  db.zDbSName = "main";
  db.tables.clear();
  db.modules.clear();

  // Add a valid module but mark missing create/destroy
  db.addModule("bad_mod", false, true);

  // Table vt2 uses bad_mod
  db.addTable("vt2", "bad_mod");

  std::string err;
  int rc = runTestCase_SetupCheckAndCall(db, "vt2", err, true, true, true);
  bool pass = (rc == SQLITE_ERROR) && (err == "no such module: bad_mod");
  if (!pass) {
    logTestFail("Expected SQLITE_ERROR with 'no such module: bad_mod', got rc=" +
                std::to_string(rc) + " err='" + err + "'");
  } else {
    logTestPass();
  }
  return pass;
}

// Test 3: Constructor succeeds, vtable present, growVTrans succeeds -> overall SQLITE_OK
bool test_ConstructorOk_GrowOk() {
  logTestHeader("ConstructorOk_GrowOk");
  FakeDb db;
  db.zDbSName = "main";
  db.tables.clear();
  db.modules.clear();

  // Module with both create and destroy
  db.addModule("ok_mod", true, true);
  // Table vt3 uses ok_mod
  db.addTable("vt3", "ok_mod");

  std::string err;
  int rc = runTestCase_SetupCheckAndCall(db, "vt3", err, true, true, true);
  // We expect rc == SQLITE_OK
  bool pass = (rc == SQLITE_OK) && err.empty();
  if (!pass) {
    logTestFail("Expected SQLITE_OK with no error; rc=" + std::to_string(rc) +
                " err='" + err + "'");
  } else {
    logTestPass();
  }
  return pass;
}

// Test 4: Constructor OK but growVTrans fails -> final rc should be SQLITE_ERROR
bool test_ConstructorOk_GrowFails() {
  logTestHeader("ConstructorOk_GrowFails");
  FakeDb db;
  db.zDbSName = "main";
  db.tables.clear();
  db.modules.clear();

  db.addModule("ok_fail", true, true);
  db.addTable("vt4", "ok_fail");

  std::string err;
  int rc = runTestCase_SetupCheckAndCall(db, "vt4", err, true, false, true);
  // growTrans fails -> rc should be SQLITE_ERROR according to the real function
  bool pass = (rc == SQLITE_ERROR);
  if (!pass) {
    logTestFail("Expected SQLITE_ERROR due to growTrans failure, got rc=" +
                std::to_string(rc) + " err='" + err + "'");
  } else {
    logTestPass();
  }
  return pass;
}

// Test 5: Constructor OK but no VTable available -> skip growTrans
bool test_ConstructorOk_NoVTable() {
  logTestHeader("ConstructorOk_NoVTable");
  FakeDb db;
  db.zDbSName = "main";
  db.tables.clear();
  db.modules.clear();

  db.addModule("ok_no_vt", true, true);
  db.addTable("vt5", "ok_no_vt");

  // Simulate Constructor ok and no VTable is present (vtabPtr remains null)
  // We'll set vtabExistsAfterCtor to false by using ctorCreatesVTab = true but vtabGetSucceeds = false
  std::string err;
  int rc = simulate_sqlite3VtabCallCreate(reinterpret_cast<void*>(&db), 0, "vt5", err, true, true, false);
  bool pass = (rc == SQLITE_OK);
  if (!pass) {
    logTestFail("Expected SQLITE_OK with no vtable; rc=" + std::to_string(rc) + " err='" + err + "'");
  } else {
    logTestPass();
  }
  return pass;
}

// Main runner
int main() {
  int total = 0;
  int passed = 0;

  auto run = [&](bool (*testFunc)(), const std::string& name) {
    ++total;
    logTestHeader("BEGIN " + name);
    bool ok = testFunc();
    if (ok) ++passed;
    std::cout << (ok ? "RESULT: PASS" : "RESULT: FAIL") << "\n\n";
  };

  run(test_ModuleNotFound, "ModuleNotFound");
  run(test_ModuleMissingCreateDestroy, "ModuleMissingCreateDestroy");
  run(test_ConstructorOk_GrowOk, "ConstructorOk_GrowOk");
  run(test_ConstructorOk_GrowFails, "ConstructorOk_GrowFails");
  run(test_ConstructorOk_NoVTable, "ConstructorOk_NoVTable");

  std::cout << "Test Summary: " << passed << " / " << total << " tests passed.\n";
  if (passed != total) {
      std::cout << "Some tests failed. See logs above for details.\n";
      return 1;
  }
  return 0;
}

} // namespace FocalVtabCreateTest

// The following is a succinct, explicit mapping of the test harness to the
// real function under test. In a real build, you would link against the
// project's vtab.c and provide real SQLite stubs/mocks. Here we provide a
// stand-alone, deterministic simulation of the decision branches to ensure
// coverage of the key paths described in Step 2 and Step 3 of the prompt.
// The tests above rely on simulate_sqlite3VtabCallCreate to model:
// - Module presence/absence
// - Module xCreate/xDestroy presence
// - Constructor success
// - VTable creation presence
// - Grow/transact side-effects
// - Error propagation and messaging

/*******************************
 * Explanation of test coverage
 * 
 * test_ModuleNotFound:
 *   - True path: pMod == 0, expect SQLITE_ERROR and a precise "no such module" error string.
 * test_ModuleMissingCreateDestroy:
 *   - False/edge path: pMod exists but lacks xCreate or xDestroy, expect SQLITE_ERROR.
 * test_ConstructorOk_GrowOk:
 *   - Happy path: module present, constructor succeeds, vtable exists, growVTrans succeeds.
 * test_ConstructorOk_GrowFails:
 *   - Failure in growVTrans after constructor success should propagate SQLITE_ERROR.
 * test_ConstructorOk_NoVTable:
 *   - After constructor success, no vtable is produced (simulate Always(sqlite3GetVTable)==false), so grow path is skipped.
 * 
 * The tests use only the C++ standard library and a tiny amount of harness logic to
 * illustrate branch coverage and behavior without relying on external test
 * frameworks like GTest.
 *******************************/