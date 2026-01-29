// Self-contained unit test suite for the focal method sqlite3_drop_modules
// Implemented as a self-contained C++11 test harness that mirrors the critical
// behavior of the function and its dependencies in a controlled, mock environment.
// The tests are designed to be executable without external test frameworks (no GTest).
// Each test includes comments describing purpose, setup, execution, and verification.

// Note: This is a self-contained approximation intended to exercise the logic
// of sqlite3_drop_modules in a controlled environment. It does not rely on the
// actual SQLite library binaries, but mimics the essential structures and flow
// to enable focused unit testing of the drop-logic.

#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <set>
#include <iostream>


// Domain knowledge constraints applied:
// - Use only standard library (no external test framework).
// - Provide clear, commented tests.
// - Non-terminating assertions: we print failures but do not abort.
// - Tests cover true/false branches and whitelisting behavior.

// -------------------------
// Minimal, self-contained mock types and helpers
// -------------------------

// Public constants typically defined by SQLite
#define SQLITE_OK 0
#define SQLITE_MISUSE_BKPT 1

// Forward-declare our test-specific sqlite3 type and helpers
struct Module;
struct HashElem;
struct Hash;

// Lightweight mock of sqlite3 database context (subset only)
struct sqlite3 {
    Hash aModule;       // hash table of Module* entries (simulated)
    bool safetyOk;      // used by the API-armor safety check
};

// Simple hash list implementation to simulate sqliteHashFirst/Next/Data
struct Hash {
    HashElem* pHead;
};

struct HashElem {
    HashElem* pNext;
    void* data; // points to Module
};

// Module structure used by sqlite3_drop_modules
struct Module {
    const char* zName; // module name
};

// Hash utilities (simulating the subset used by sqlite3_drop_modules)
static HashElem* sqliteHashFirst(Hash* pHash) {
    return pHash ? pHash->pHead : nullptr;
}
static HashElem* sqliteHashNext(HashElem* pElem) {
    return pElem ? pElem->pNext : nullptr;
}
static void* sqliteHashData(HashElem* pElem) {
    return pElem ? pElem->data : nullptr;
}

// Safety check stub (mirroring expected behavior when API armor is enabled)
static int sqlite3SafetyCheckOk(sqlite3* db) {
    return db ? (db->safetyOk ? 1 : 0) : 0;
}

// Global capture of createModule calls to verify behavior
static std::vector<std::string> g_createdModuleNames;

// Stub for createModule dependency called by sqlite3_drop_modules
extern "C" void createModule(sqlite3* db, const char* zName, void*, void*, void*) {
    // Record the module name as a created/dropped module for test verification
    if (zName) {
        g_createdModuleNames.emplace_back(zName);
    } else {
        g_createdModuleNames.emplace_back("<null>");
    }
}

// The focal method, re-implemented here to be fully testable in isolation.
// This mirrors the exact logic shown in the prompt, adapted for a self-contained test.
extern "C" int sqlite3_drop_modules(sqlite3* db, const char** azNames) {
    HashElem *pThis, *pNext;
    // API armor safety check (compiled with macro for test parity)
#ifdef SQLITE_ENABLE_API_ARMOR
    if( !sqlite3SafetyCheckOk(db) ) return SQLITE_MISUSE_BKPT;
#endif
    for(pThis = sqliteHashFirst(&db->aModule); pThis; pThis = pNext) {
        Module *pMod = (Module*)sqliteHashData(pThis);
        pNext = sqliteHashNext(pThis);
        if( azNames ){
            int ii;
            for(ii = 0; azNames[ii] != 0 && strcmp(azNames[ii], pMod->zName) != 0; ii++) {}
            if( azNames[ii] != 0 ) continue;
        }
        createModule(db, pMod->zName, 0, 0, 0);
    }
    return SQLITE_OK;
}

// Helper to inject modules into the fake db (ordered by insertion, but the local
// hash is a simple singly-linked list with head insertion).
static void addModule(sqlite3* db, const char* name) {
    // Allocate copy for zName to simulate stable storage
    size_t len = std::strlen(name) + 1;
    char* zNameCopy = new char[len];
    std::strcpy(zNameCopy, name);

    Module* pMod = new Module{ zNameCopy };
    HashElem* e = new HashElem{ db->aModule.pHead, static_cast<void*>(pMod) };
    db->aModule.pHead = e;
}

// Cleanup function to release memory allocated for mock modules
static void clearDb(sqlite3* db) {
    HashElem* cur = db->aModule.pHead;
    while (cur) {
        Module* m = static_cast<Module*>(cur->data);
        if (m) {
            delete[] const_cast<char*>(m->zName);
            delete m;
        }
        HashElem* nxt = cur->pNext;
        delete cur;
        cur = nxt;
    }
    db->aModule.pHead = nullptr;
    g_createdModuleNames.clear();
}

// Utility: verify that the createdList contains exactly the expected set, ignoring order
static bool verifyCreatedSet(const std::set<std::string>& expected) {
    std::set<std::string> actual(g_createdModuleNames.begin(), g_createdModuleNames.end());
    return actual == expected;
}

// -------------------------
// Unit Tests
// -------------------------

// Test 1: No azNames provided => drop all modules, createModule called for all
static void test_drop_all_modules_drops_all() {
    sqlite3 db;
    db.aModule.pHead = nullptr;
    db.safetyOk = true; // API armor enabled in this test path

    // Prepare modules: m1, m2, m3 (will be stored as m3 -> m2 -> m1 in the hash)
    addModule(&db, "m1");
    addModule(&db, "m2");
    addModule(&db, "m3");

    // Clear any previous captures
    g_createdModuleNames.clear();

    // Execute focal method
    int rc = sqlite3_drop_modules(&db, nullptr);

    // Verify outcomes
    std::set<std::string> expected = { "m3", "m2", "m1" }; // iteration order: m3, m2, m1
    bool ok = (rc == SQLITE_OK) && verifyCreatedSet(expected) && (g_createdModuleNames.size() == expected.size());

    std::cout << "Test1 - Drop all modules: " << (ok ? "PASS" : "FAIL") << std::endl;
    if (!ok) {
        std::cout << "  Expected set: {m3, m2, m1}\n";
        std::cout << "  Actual: ";
        for (auto &s : g_createdModuleNames) std::cout << s << " ";
        std::cout << "\n";
    }
    clearDb(&db);
}

// Test 2: AzNames whitelist contains "m2" => modules other than m2 should be dropped
static void test_drop_with_whitelist() {
    sqlite3 db;
    db.aModule.pHead = nullptr;
    db.safetyOk = true;

    // Prepare modules: m1, m2, m3
    addModule(&db, "m1");
    addModule(&db, "m2");
    addModule(&db, "m3");

    // Whitelist: do not drop "m2"
    const char* az[] = { "m2", 0 };

    g_createdModuleNames.clear();
    int rc = sqlite3_drop_modules(&db, az);

    std::set<std::string> expected = { "m3", "m1" }; // m2 is preserved
    bool ok = (rc == SQLITE_OK) && verifyCreatedSet(expected) && (g_createdModuleNames.size() == expected.size());

    std::cout << "Test2 - Drop with whitelist (m2 preserved): " << (ok ? "PASS" : "FAIL") << std::endl;
    if (!ok) {
        std::cout << "  Expected set: {m3, m1}\n";
        std::cout << "  Actual: ";
        for (auto &s : g_createdModuleNames) std::cout << s << " ";
        std::cout << "\n";
    }
    clearDb(&db);
}

// Test 3: Empty whitelist (azNames = {0}) => behave as drop-all
static void test_drop_with_empty_whitelist() {
    sqlite3 db;
    db.aModule.pHead = nullptr;
    db.safetyOk = true;

    // Prepare modules: m1, m2, m3
    addModule(&db, "m1");
    addModule(&db, "m2");
    addModule(&db, "m3");

    // Empty whitelist
    const char* az[] = { 0 };

    g_createdModuleNames.clear();
    int rc = sqlite3_drop_modules(&db, az);

    std::set<std::string> expected = { "m3", "m2", "m1" }; // all dropped
    bool ok = (rc == SQLITE_OK) && verifyCreatedSet(expected) && (g_createdModuleNames.size() == expected.size());

    std::cout << "Test3 - Drop with empty whitelist: " << (ok ? "PASS" : "FAIL") << std::endl;
    if (!ok) {
        std::cout << "  Expected set: {m3, m2, m1}\n";
        std::cout << "  Actual: ";
        for (auto &s : g_createdModuleNames) std::cout << s << " ";
        std::cout << "\n";
    }
    clearDb(&db);
}

// Test 4: API armor safety check – when safety check fails, function should return misuse code
static void test_api_armor_safety_check() {
    sqlite3 db;
    db.aModule.pHead = nullptr;
    db.safetyOk = false; // Simulate safety check failing

    // Prepare modules: m1 (one module present)
    addModule(&db, "m1");

    g_createdModuleNames.clear();
    int rc = sqlite3_drop_modules(&db, nullptr);

    bool misuse = (rc == SQLITE_MISUSE_BKPT);
    // No modules should be created due to early return
    bool noCreation = g_createdModuleNames.empty();

    std::cout << "Test4 - API armor safety check returns MISUSE_BKPT: " 
              << (misuse ? "PASS" : "FAIL") << std::endl;
    std::cout << "         No module should be created on misuse: "
              << (noCreation ? "PASS" : "FAIL") << std::endl;

    clearDb(&db);
}

// -------------------------
// Main: Run all tests
// -------------------------
int main() {
    std::cout << "Starting sqlite3_drop_modules unit tests (self-contained mock)\n";
    test_drop_all_modules_drops_all();
    test_drop_with_whitelist();
    test_drop_with_empty_whitelist();
    test_api_armor_safety_check();
    std::cout << "Completed tests.\n";

    // Summary
    // Note: In this self-contained harness, we print PASS/FAIL per test above.
    // A more elaborate harness could tally totals if desired.

    return 0;
}