// Test suite for the focal method: sqlite3PragmaVtabRegister
// This test suite is implemented in C++11 without Google Test.
// It provides a minimal mock environment to exercise the function
// under three scenarios: pName==0, pName with no required flags, and pName
// with required flags. The goal is to achieve meaningful coverage without
// terminating the test run on non-critical assertions from the focal method.

#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <pragma.h>


// Domain-specific test scaffolding (mocked dependencies)

/*
   Simulated domain types and constants used by
   sqlite3PragmaVtabRegister in the focal method.
*/

// Pragma flag bits (as in the production code)
static const unsigned int PragFlg_Result0 = 1u << 0;
static const unsigned int PragFlg_Result1 = 1u << 1;

// Lightweight placeholder for the sqlite3 Module type
struct Module { int dummy; };

// Global pragma module placeholder used by sqlite3VtabCreateModule call
Module pragmaVtabModule;

// Forward declaration of the function used by sqlite3PragmaVtabRegister
Module* sqlite3VtabCreateModule(struct sqlite3*, const char*, Module*, void*, int);

// Lightweight placeholder for the sqlite3 database structure
struct sqlite3 { int aModule; };

// Hash lookup stub: production code uses sqlite3HashFind(&db->aModule, zName)
int sqlite3HashFind(int* /*hash*/, const char* /*zName*/) {
    // In tests, the hash lookup is controlled by test harness if needed.
    // Default: pretend the entry does not exist to satisfy the production path
    // where the code asserts the result is 0 (not found).
    return 0;
}

// Case-insensitive string compare for the focal assertion
int sqlite3_strnicmp(const char* a, const char* b, int n) {
    if (a == nullptr || b == nullptr) return (a == b) ? 0 : 1;
    for (int i = 0; i < n; ++i) {
        unsigned char ca = static_cast<unsigned char>(a[i]);
        unsigned char cb = static_cast<unsigned char>(b[i]);
        // Convert to lowercase for comparison
        if (ca >= 'A' && ca <= 'Z') ca = ca - 'A' + 'a';
        if (cb >= 'A' && cb <= 'Z') cb = cb - 'A' + 'a';
        if (ca < cb) return -1;
        if (ca > cb) return 1;
        if (a[i] == '\0' || i == n - 1) break;
    }
    return 0;
}

// PragmaName structure used by the focal method
struct PragmaName { unsigned int mPragFlg; };

// Forward declaration for pragmaLocate used by the focal method.
// We implement pragmaLocate in test code (external linkage) so test harness can
// control its behavior per-test.
static const PragmaName* pragmaLocate(const char* zName);

// Test harness control: map what pragmaLocate returns for a given zName
static const char* g_lookupName = nullptr;
static const PragmaName* g_lookupPtr = nullptr;

// pragmaLocate implementation controlled by the test harness
const PragmaName* pragmaLocate(const char* zName) {
    if (zName == nullptr) return nullptr;
    if (g_lookupName != nullptr && std::strcmp(zName, g_lookupName) == 0) {
        return g_lookupPtr;
    }
    return nullptr;
}

// Production-like function used by the focal method
Module* sqlite3VtabCreateModule(sqlite3* /*db*/, const char* /*zName*/, Module* /*pModule*/, void* /*pAux*/, int /*iAuth*/) {
    // Return a new Module object to simulate successful module creation
    return new Module();
}

// The focal method under test (copied as-is from the prompt)
Module *sqlite3PragmaVtabRegister(sqlite3 *db, const char *zName){
  const PragmaName *pName;
  // The real code asserts that zName starts with "pragma_"
  // We ensure the test always uses a valid prefix.
  assert( sqlite3_strnicmp(zName, "pragma_", 7)==0 );
  pName = pragmaLocate(zName+7);
  if( pName==0 ) return 0;
  if( (pName->mPragFlg & (PragFlg_Result0|PragFlg_Result1))==0 ) return 0;
  assert( sqlite3HashFind(&db->aModule, zName)==0 );
  return sqlite3VtabCreateModule(db, zName, &pragmaVtabModule, (void*)pName, 0);
}

// Simple test harness infrastructure

struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};

static std::vector<TestResult> g_results;

// Helper to record a test result
static void recordResult(const std::string& name, bool passed, const std::string& message) {
    g_results.push_back({name, passed, message});
}

// Test Case A: pName == 0 path should return 0
static void test_caseA_pNameNull() {
    // Configure test harness: when zName="pragma_bar", pragmaLocate("bar") -> nullptr
    g_lookupName = "bar";
    g_lookupPtr = nullptr;

    // Ensure hash lookup would return 0 (not used in this path, but to keep consistency)
    // Note: sqlite3HashFind is a stub that returns 0 by default.

    sqlite3 db;
    db.aModule = 0;

    Module* res = sqlite3PragmaVtabRegister(&db, "pragma_bar");

    if (res == nullptr) {
        recordResult("test_caseA_pNameNull", true, "Expected null when pName is null, got null as expected.");
    } else {
        recordResult("test_caseA_pNameNull", false, "Expected null, but got non-null Module*.");
        delete res;
    }

    // Cleanup
    g_lookupName = nullptr;
    g_lookupPtr = nullptr;
}

// Test Case B: pName non-null but flags do not include Result0/Result1 => return 0
static void test_caseB_pNameNoFlags() {
    // Configure test harness: pragmaLocate("bar") -> pBad with no flags
    static PragmaName pBad = { 0 }; // no PragFlg_Result0/Result1
    g_lookupName = "bar";
    g_lookupPtr = &pBad;

    sqlite3 db;
    db.aModule = 0;

    Module* res = sqlite3PragmaVtabRegister(&db, "pragma_bar");

    if (res == nullptr) {
        recordResult("test_caseB_pNameNoFlags", true, "Expected null when pName lacks required flags, got null as expected.");
    } else {
        recordResult("test_caseB_pNameNoFlags", false, "Expected null due to missing flags, but got non-null Module*.");
        delete res;
    }

    // Cleanup
    g_lookupName = nullptr;
    g_lookupPtr = nullptr;
}

// Test Case C: pName valid and flags include required bits => should create module (non-null)
static void test_caseC_pNameWithFlags() {
    // Configure test harness: pragmaLocate("bar") -> pOk with PragFlg_Result0
    static PragmaName pOk = { PragFlg_Result0 };
    g_lookupName = "bar";
    g_lookupPtr = &pOk;

    sqlite3 db;
    db.aModule = 0;

    Module* res = sqlite3PragmaVtabRegister(&db, "pragma_bar");

    if (res != nullptr) {
        recordResult("test_caseC_pNameWithFlags", true, "Expected non-null Module* when all conditions pass.");
        delete res;
    } else {
        recordResult("test_caseC_pNameWithFlags", false, "Expected non-null Module* but got null.");
    }

    // Cleanup
    g_lookupName = nullptr;
    g_lookupPtr = nullptr;
}

// Entry point for running all tests
int main() {
    // Note: We intentionally do not use Google Test or GMock.
    // The helper functions above run isolated scenarios and record results.

    test_caseA_pNameNull();
    test_caseB_pNameNoFlags();
    test_caseC_pNameWithFlags();

    // Report test results
    int failed = 0;
    for (const auto& r : g_results) {
        std::cout << (r.passed ? "[PASS] " : "[FAIL] ") << r.name;
        if (!r.message.empty()) {
            std::cout << " - " << r.message;
        }
        std::cout << std::endl;
        if (!r.passed) ++failed;
    }

    if (failed > 0) {
        std::cout << "Total failed tests: " << failed << "/" << g_results.size() << std::endl;
    } else {
        std::cout << "All tests passed: " << g_results.size() << " tests." << std::endl;
    }

    // Clean up any potential allocated modules
    // (In the tests, modules are allocated only when a case passes)
    // No global leaks expected for this small suite.

    return (failed == 0) ? 0 : 1;
}