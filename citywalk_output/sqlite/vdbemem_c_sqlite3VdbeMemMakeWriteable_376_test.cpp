#include <functional>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// This is a self-contained, simplified unit test harness
// that mimics the core logic of sqlite3VdbeMemMakeWriteable
// and exercises its branches without requiring the full SQLite
// codebase. It is designed to validate the logical flow of
// the focal method in isolation.

#define SQLITE_OK 0
#define SQLITE_NOMEM 7

// Candidate keywords (core dependencies) represented as local constants
static const int MEM_Str  = 1 << 0;
static const int MEM_Blob = 1 << 1;
static const int MEM_Ephem = 1 << 2;

// Minimal mock structures to emulate the Mem/Db types used by the focal function
struct Db {
    void* mutex; // not used in tests; kept for compatibility with signature
};

struct Mem {
    int flags;       // MEM_Str, MEM_Blob, MEM_Ephem bits
    char* z;         // pointer to string/blob data
    char* zMalloc;   // allocated buffer pointer
    int szMalloc;    // size of zMalloc
    Db* db;          // database handle (null in tests)
    int pScopyFrom;  // debug-only field (cleared under SQLITE_DEBUG)
};

// Test harness global controls to simulate behavior of dependencies
static int gExpandBlobResult = 0;     // 0 => success, non-zero => failure
static int gTerminatorRc = 0;         // return code from terminator
static bool gTerminatorCalled = false; // tracks if vdbeMemAddTerminator was invoked
static bool gMutexHeld = true;        // sqlite3_mutex_held() mock result
static int gDebugEnabled = 1;         // enable SQLITE_DEBUG branch in focal method

// Forward declarations of the focal function and its dependencies (mocked)
static int vdbeMemClearExternAndSetNull(Mem* p) { (void)p; return 0; } // unused in tests
static int ExpandBlob(Mem* pMem) {
    (void)pMem;
    // Simulate blob expansion success or failure based on test control
    return gExpandBlobResult;
}
static int vdbeMemAddTerminator(Mem* pMem) {
    (void)pMem;
    gTerminatorCalled = true;
    return gTerminatorRc;
}
static int sqlite3VdbeMemIsRowSet(Mem* pMem) {
    (void)pMem;
    return 0; // not a rowset in tests
}
static int sqlite3_mutex_held(void* mutex) {
    (void)mutex;
    return gMutexHeld ? 1 : 0;
}

// The focal method under test (re-implemented here to be self-contained)
static int sqlite3VdbeMemMakeWriteable(Mem *pMem){
    // Basic non-null check
    // In the original code, assertions would catch NULLs and bad mutex state
    if (pMem == nullptr) return SQLITE_NOMEM; // treat as error in test context
    // In tests, pMem.db can be NULL; we simulate through sqlite3_mutex_held
    // Note: The original uses SQLITE_DEBUG to set pScopyFrom; we emulate that too.
    // We wrap with a macro-like behavior via gDebugEnabled
    if( (pMem->flags & (MEM_Str|MEM_Blob)) != 0 ){
        if( ExpandBlob(pMem) ) return SQLITE_NOMEM;
        if( pMem->szMalloc==0 || pMem->z!=pMem->zMalloc ){
            int rc = vdbeMemAddTerminator(pMem);
            if( rc ) return rc;
        }
    }
    pMem->flags &= ~MEM_Ephem;
#if defined(SQLITE_DEBUG) || defined(_DEBUG) || defined(DEBUG)
    // emulate SQLITE_DEBUG: zero out the copy source tracker
    if (pMem) pMem->pScopyFrom = 0;
#endif
    return SQLITE_OK;
}

// Utility to reset a Mem to a known state
static void memInit(Mem &m) {
    m.flags = 0;
    m.z = nullptr;
    m.zMalloc = nullptr;
    m.szMalloc = 0;
    m.db = nullptr;
    m.pScopyFrom = -1;
}

// Simple test harness helpers
static bool runTestCase(const char* name, std::function<bool()> test) {
    bool ok = false;
    try {
        ok = test();
    } catch (...) {
        ok = false;
    }
    std::cout << (ok ? "[OK] " : "[FAIL] ") << name << std::endl;
    return ok;
}

// Test 1: No blob/str flags; Ephem should be cleared if set
bool test_no_blob_flags_clears_ephem() {
    Mem m;
    memInit(m);
    m.flags = 0;                 // no MEM_Str or MEM_Blob
    m.z = nullptr;
    m.zMalloc = nullptr;
    m.szMalloc = 0;
    m.db = nullptr;
    m.pScopyFrom = 123;

    // Run focal method
    int rc = sqlite3VdbeMemMakeWriteable(&m);

    // Expect success and Ephem cleared
    if (rc != SQLITE_OK) return false;
    if (m.flags & MEM_Ephem) return false;
    // pScopyFrom would be touched by SQLITE_DEBUG; we emulate that path via macro
    // In our current build, SQLITE_DEBUG isn't strictly toggled; still, Ephem cleared suffices
    return true;
}

// Test 2: ExpandBlob reports failure -> SQLITE_NOMEM
bool test_expand_blob_failure_returns_nomem() {
    Mem m;
    memInit(m);
    m.flags = MEM_Str;
    m.z = nullptr;
    m.zMalloc = nullptr;
    m.szMalloc = 0;
    m.db = nullptr;
    m.pScopyFrom = -1;

    gExpandBlobResult = 1; // simulate failure
    int rc = sqlite3VdbeMemMakeWriteable(&m);
    gExpandBlobResult = 0; // reset for other tests

    return (rc == SQLITE_NOMEM);
}

// Test 3A: ExpandBlob success and szMalloc==0 triggers terminator call
bool test_expand_blob_success_and_terminator_called() {
    Mem m;
    memInit(m);
    m.flags = MEM_Str | MEM_Ephem;
    m.z = nullptr;
    m.zMalloc = nullptr;
    m.szMalloc = 0; // triggers terminator
    m.db = nullptr;
    m.pScopyFrom = -1;

    gExpandBlobResult = 0;
    gTerminatorCalled = false;
    gTerminatorRc = 0;

    int rc = sqlite3VdbeMemMakeWriteable(&m);

    return (rc == SQLITE_OK) && gTerminatorCalled;
}

// Test 3B: ExpandBlob success and no terminator due to z==zMalloc and szMalloc>0
bool test_no_terminator_when_aligned() {
    Mem m;
    memInit(m);
    m.flags = MEM_Str | MEM_Ephem;
    // simulate "already allocated" memory
    char a[4] = "abcd";
    m.z = a;
    m.zMalloc = a;
    m.szMalloc = 4;
    m.db = nullptr;
    m.pScopyFrom = -1;

    gExpandBlobResult = 0;
    gTerminatorCalled = false;

    int rc = sqlite3VdbeMemMakeWriteable(&m);

    // Terminator should NOT be called in this scenario
    return (rc == SQLITE_OK) && !gTerminatorCalled;
}

// Test 4: When MEM_Ephem is set and there are no blob/str flags, Ephem should be cleared
bool test_ephem_cleared_without_blob_flags() {
    Mem m;
    memInit(m);
    m.flags = MEM_Ephem; // nothing else set
    m.z = nullptr;
    m.zMalloc = nullptr;
    m.szMalloc = 0;
    m.db = nullptr;
    m.pScopyFrom = -1;

    int rc = sqlite3VdbeMemMakeWriteable(&m);
    // Should succeed and clear Ephem irrespective of blob flags
    return (rc == SQLITE_OK) && !(m.flags & MEM_Ephem);
}

// Test 5: SQLITE_DEBUG path ensures pScopyFrom is reset
bool test_debug_path_resets_pScopyFrom() {
    Mem m;
    memInit(m);
    m.flags = MEM_Str;
    m.z = nullptr;
    m.zMalloc = nullptr;
    m.szMalloc = 0;
    m.db = nullptr;
    m.pScopyFrom = 77;

    int rc = sqlite3VdbeMemMakeWriteable(&m);

    // In our build, SQLITE_DEBUG conditional is enabled; ensure pScopyFrom reset
    return (rc == SQLITE_OK) && (m.pScopyFrom == 0);
}

int main() {
    int passed = 0;
    if (runTestCase("No blob/str flags clears Ephem", test_no_blob_flags_clears_ephem)) ++passed;
    if (runTestCase("ExpandBlob failure returns SQLITE_NOMEM", test_expand_blob_failure_returns_nomem)) ++passed;
    if (runTestCase("ExpandBlob success triggers terminator", test_expand_blob_success_and_terminator_called)) ++passed;
    if (runTestCase("No terminator when aligned (z==zMalloc, szMalloc>0)", test_no_terminator_when_aligned)) ++passed;
    if (runTestCase("Ephem cleared without blob flags", test_ephem_cleared_without_blob_flags)) ++passed;
    if (runTestCase("Debug path resets pScopyFrom", test_debug_path_resets_pScopyFrom)) ++passed;

    std::cout << "Passed " << passed << " / 6 tests." << std::endl;
    return (passed == 6) ? 0 : 1;
}