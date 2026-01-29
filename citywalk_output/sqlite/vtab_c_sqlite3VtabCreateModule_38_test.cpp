// Self-contained C++11 unit tests for sqlite3VtabCreateModule
// Note: This test suite reimplements a minimal subset of the relevant SQLite
// types and helpers (Module, sqlite3, sqlite3_malloc, etc.) in order to test
// the focal function behavior in isolation without GTest.
// The focal function is re-implemented here to ensure executable tests in this
// standalone environment, mirroring the logic shown in <FOCAL_METHOD>.

// Includes
#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <unordered_map>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Lightweight test harness (non-terminating assertions)
static int g_testCount = 0;
static int g_failCount = 0;

static void logFailure(const char* testName, const char* cond, int line) {
    std::cerr << "Failure in " << testName << " at line " << line << ": "
              << cond << std::endl;
    ++g_failCount;
}
#define EXPECT_TRUE(cond) do { \
    ++g_testCount; \
    if(!(cond)) logFailure(currentTestName, #cond, __LINE__); \
} while(0)
#define EXPECT_EQ(a,b) do { \
    ++g_testCount; \
    if(!((a) == (b))) { \
        std::cerr << "Failure in " << currentTestName << " at line " << __LINE__ \
                  << ": expected " << #a " == " << #b << " (" << (a) << " vs " << (b) << ")" << std::endl; \
        ++g_failCount; \
    } \
} while(0)
#define EXPECT_NEQ(a,b) do { \
    ++g_testCount; \
    if((a) == (b)) { \
        std::cerr << "Failure in " << currentTestName << " at line " << __LINE__ \
                  << ": expected " << #a << " != " << #b << std::endl; \
        ++g_failCount; \
    } \
} while(0)

// Forward declarations for the test environment
struct sqlite3;
struct sqlite3_module;
struct Module;

// Minimal Hash structure to emulate sqlite3HashInsert behavior
struct Hash {
    std::unordered_map<std::string, void*> map;
};

// Minimal Module structure as used by sqlite3VtabCreateModule
struct Module {
    char *zName;
    const sqlite3_module *pModule;
    void *pAux;
    void (*xDestroy)(void*);
    void *pEpoTab;
    int nRefModule;
};

// Minimal sqlite3 database structure
struct sqlite3 {
    Hash aModule;               // hash table for modules
    bool oomCalled;               // tracks OOM fault invocation
    Module* lastCleared;          // last pDel passed to sqlite3VtabEponymousTableClear
    Module* lastUnref;             // last pDel passed to sqlite3VtabModuleUnref
};

// Dummy sqlite3_module type (contents not used in tests)
struct sqlite3_module { };

// Global test harness helpers
static bool allocateShouldFail = false; // Simulate OOM on next allocation

// Allocate memory with OOM simulation
static void* sqlite3Malloc(size_t n){
    if(allocateShouldFail){
        allocateShouldFail = false;
        return nullptr;
    }
    return std::malloc(n);
}
static void sqlite3DbFree(sqlite3* db, void* p){
    // In tests, ignore freeing of null ptr
    (void)db;
    std::free(p);
}
static void sqlite3OomFault(sqlite3* db){
    if(db){
        db->oomCalled = true;
    }
}
static int sqlite3Strlen30(const char* zName){
    size_t len = std::strlen(zName);
    if(len > 30) len = 30;
    return (int)len;
}
static void* sqlite3HashInsert(Hash* pHash, const char* zKey, void* pNew){
    auto it = pHash->map.find(std::string(zKey));
    void* old = nullptr;
    if(it != pHash->map.end()){
        old = it->second;
        it->second = pNew;
    }else{
        pHash->map.emplace(std::string(zKey), pNew);
    }
    return old;
}

// Tracking hooks to verify calls
static void sqlite3VtabEponymousTableClear(sqlite3* db, Module* pMod){
    if(db) db->lastCleared = pMod;
}
static void sqlite3VtabModuleUnref(sqlite3* db, Module* pMod){
    if(db) db->lastUnref = pMod;
}

// The focal function under test (re-implemented here for standalone testing)
static Module* sqlite3VtabCreateModule(
    sqlite3 *db,                    /* Database in which module is registered */
    const char *zName,              /* Name assigned to this module */
    const sqlite3_module *pModule,  /* The definition of the module */
    void *pAux,                     /* Context pointer for xCreate/xConnect */
    void (*xDestroy)(void *)        /* Module destructor function */
){
    Module *pMod;
    Module *pDel;
    char *zCopy;
    if( pModule==0 ){
        zCopy = (char*)zName;
        pMod = 0;
    }else{
        int nName = sqlite3Strlen30(zName);
        pMod = (Module *)sqlite3Malloc(sizeof(Module) + nName + 1);
        if( pMod==0 ){
            sqlite3OomFault(db);
            return 0;
        }
        zCopy = (char *)(&pMod[1]);
        memcpy(zCopy, zName, nName+1);
        pMod->zName = zCopy;
        pMod->pModule = pModule;
        pMod->pAux = pAux;
        pMod->xDestroy = xDestroy;
        pMod->pEpoTab = 0;
        pMod->nRefModule = 1;
    }
    pDel = (Module *)sqlite3HashInsert(&db->aModule,zCopy,(void*)pMod);
    if( pDel ){
        if( pDel==pMod ){
            sqlite3OomFault(db);
            sqlite3DbFree(db, pDel);
            pMod = 0;
        }else{
            sqlite3VtabEponymousTableClear(db, pDel);
            sqlite3VtabModuleUnref(db, pDel);
        }
    }
    return pMod;
}

// Small helper to create a new mock database
static sqlite3* createMockDb(){
    sqlite3* db = static_cast<sqlite3*>(std::malloc(sizeof(sqlite3)));
    if(!db) return nullptr;
    db->aModule = Hash();
    db->oomCalled = false;
    db->lastCleared = nullptr;
    db->lastUnref = nullptr;
    return db;
}
static void destroyMockDb(sqlite3* db){
    if(db){
        // Note: In real SQLite, modules would be freed here as needed.
        std::free(db);
    }
}

// A dummy module implementation (empty)
static void dummyDestroy(void*){}

// Test 1: Basic path with pModule == 0
static void test_basic_null_module_path(){
    const char* name = "basic_null";
    sqlite3* db = createMockDb();
    currentTestName = "test_basic_null_module_path";

    Module* res = sqlite3VtabCreateModule(db, name, nullptr, nullptr, nullptr);
    // Expect NULL return when pModule == 0
    EXPECT_TRUE(res == nullptr);
    // Ensure the hash stores the key with a NULL value
    auto it = db->aModule.map.find(std::string(name));
    EXPECT_TRUE(it != db->aModule.map.end());
    EXPECT_TRUE(it->second == nullptr);

    destroyMockDb(db);
}

// Test 2: Non-null module creation populates Module and hash
static void test_module_creation_populates_fields(){
    const char* name = "mod_basic";
    sqlite3* db = createMockDb();
    currentTestName = "test_module_creation_populates_fields";

    static sqlite3_module dummyMod = {}; // empty structure
    void* pAux = (void*)0x1234;
    void (*dtor)(void*) = &dummyDestroy;
    Module* res = sqlite3VtabCreateModule(db, name, &dummyMod, pAux, dtor);
    EXPECT_TRUE(res != nullptr);
    // zName should be present in allocated space after res
    std::string storedName = res->zName ? std::string(res->zName) : "";
    EXPECT_EQ(storedName, std::string(name));
    EXPECT_TRUE(res->pModule == &dummyMod);
    EXPECT_TRUE(res->pAux == pAux);
    EXPECT_TRUE(res->xDestroy == dtor);
    EXPECT_TRUE(res->pEpoTab == 0);
    EXPECT_EQ(res->nRefModule, 1);

    // Hash should map name to res
    auto it = db->aModule.map.find(std::string(name));
    EXPECT_TRUE(it != db->aModule.map.end());
    EXPECT_EQ(it->second, static_cast<void*>(res));

    destroyMockDb(db);
}

// Test 3: Allocation failure triggers OOM fault and returns NULL
static void test_allocation_oom_path(){
    const char* name = "mod_oom";
    sqlite3* db = createMockDb();
    currentTestName = "test_allocation_oom_path";

    allocateShouldFail = true; // cause next sqlite3Malloc() to fail
    static sqlite3_module dummyMod = {};
    void* pAux = nullptr;
    void (*dtor)(void*) = &dummyDestroy;

    Module* res = sqlite3VtabCreateModule(db, name, &dummyMod, pAux, dtor);
    EXPECT_TRUE(res == nullptr);
    EXPECT_TRUE(db->oomCalled);

    destroyMockDb(db);
}

// Test 4: Inserting with an existing key replaces old and calls clear/unref
static void test_duplicate_key_replaces_and_unrefs(){
    const char* name = "dup_mod";
    sqlite3* db = createMockDb();
    currentTestName = "test_duplicate_key_replaces_and_unrefs";

    // First insertion
    static sqlite3_module modA = {};
    void* pAuxA = nullptr;
    void (*dtorA)(void*) = &dummyDestroy;
    Module* first = sqlite3VtabCreateModule(db, name, &modA, pAuxA, dtorA);
    EXPECT_TRUE(first != nullptr);

    // Clear tracking vars
    db->lastCleared = nullptr;
    db->lastUnref = nullptr;

    // Second insertion with same name but different Module
    static sqlite3_module modB = {};
    void* pAuxB = (void*)0xDEADBEEF;
    void (*dtorB)(void*) = &dummyDestroy;
    Module* second = sqlite3VtabCreateModule(db, name, &modB, pAuxB, dtorB);
    EXPECT_TRUE(second != nullptr);
    // Old module should have been cleared and unref'd
    EXPECT_TRUE(db->lastCleared == first);
    EXPECT_TRUE(db->lastUnref == first);

    // Hash should now map to the new module
    auto it = db->aModule.map.find(std::string(name));
    EXPECT_TRUE(it != db->aModule.map.end());
    EXPECT_EQ(it->second, static_cast<void*>(second));

    destroyMockDb(db);
}

// Helper: current test name tracking
static const char* currentTestName = "";

// Test runner
static void runAllTests(){
    test_basic_null_module_path();
    test_module_creation_populates_fields();
    test_allocation_oom_path();
    test_duplicate_key_replaces_and_unrefs();
}

// Main: run tests and report
int main(){
    std::cout << "Starting sqlite3VtabCreateModule unit tests (C++11, non-GTest)..." << std::endl;
    runAllTests();
    std::cout << "Tests run: " << g_testCount << ", Failures: " << g_failCount << std::endl;
    if(g_failCount > 0){
        std::cerr << "Some tests failed." << std::endl;
        return 1;
    }
    std::cout << "All tests passed." << std::endl;
    return 0;
}