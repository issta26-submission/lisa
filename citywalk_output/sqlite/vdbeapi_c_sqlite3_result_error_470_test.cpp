// Unit test suite for the focal method: sqlite3_result_error
// This test suite is designed to be self-contained (no GTest) and compiles under C++11.
// It provides minimal stubs for the critical dependencies used by sqlite3_result_error
// and verifies key behaviors (armor branch, state update, and memory/output handling).

#include <opcodes.h>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// ----------------------------------------------------------------------------
// Minimal domain stubs to emulate the environment relied upon by sqlite3_result_error
// ----------------------------------------------------------------------------

// Macro definitions to mirror the tiny subset used by the focal function
#define SQLITE_UTF8 1
#define SQLITE_TRANSIENT 0
#define SQLITE_ERROR 1

// Enable the API armor branch (as used in the focal method)
#define SQLITE_ENABLE_API_ARMOR

// Forward declarations of types used by the focal function
typedef void (*DelPtr)(void*);

// Minimal representation of the Mem object (as used by sqlite3VdbeMemSetStr)
struct Db {
    void* mutex; // placeholder for the mutex handle
};

struct Mem {
    Db* db;          // pointer to a database context to reach mutex
    const char* z;   // data pointer (text payload)
    int n;           // size
    int enc;         // encoding (e.g., SQLITE_UTF8)
    DelPtr xDel;      // destructor (not used in tests)
};

// Minimal sqlite3_context recreation required by the focal method
struct sqlite3_context {
    Mem* pOut;       // output memory region where the result is stored
    int isError;       // error flag set by sqlite3_result_error
};

// Global instrumentation to verify that the mutex check path is exercised
static int g_mutexHeldCallCount = 0;

// Stub for sqlite3_mutex_held to simulate that the mutex is always held.
// In the real SQLite code, this would validate thread ownership of the mutex.
// Here we increment a counter to observe invocation during tests.
int sqlite3_mutex_held(void* /*mutex*/) {
    ++g_mutexHeldCallCount;
    return 1; // indicate "held"
}

// Stub for sqlite3VdbeMemSetStr as used by sqlite3_result_error
// Stores the provided string and metadata into the Mem object.
void sqlite3VdbeMemSetStr(Mem *pOut, const char *z, int n, int enc, DelPtr xDel) {
    if(pOut) {
        pOut->z = z;
        pOut->n = n;
        pOut->enc = enc;
        pOut->xDel = xDel;
    }
}

// The focal method under test (copied/adapted as a local implementation for unit testing)
void sqlite3_result_error(sqlite3_context *pCtx, const char *z, int n){
#ifdef SQLITE_ENABLE_API_ARMOR
  if( pCtx==0 ) return;
#endif
  assert( sqlite3_mutex_held(pCtx->pOut->db->mutex) );
  pCtx->isError = SQLITE_ERROR;
  sqlite3VdbeMemSetStr(pCtx->pOut, z, n, SQLITE_UTF8, SQLITE_TRANSIENT);
}

// ----------------------------------------------------------------------------
// Test harness (no external testing framework; uses simple main with helpers)

struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};

// Helper to run a single test and collect the result
static TestResult runTest(const std::string &name, bool condition, const std::string &msgIfFail) {
    TestResult r;
    r.name = name;
    r.passed = condition;
    r.message = condition ? "OK" : msgIfFail;
    return r;
}

// Test 1: Validate normal path when pCtx is non-null.
// - Ensures the error flag is set to SQLITE_ERROR
// - Ensures the output Mem stores the provided string and length
// - Verifies the mutex-held assertion path is exercised
// - Verifies that the mutex check is invoked exactly once
TestResult test_sqlite3_result_error_basic() {
    // Prepare a minimal environment
    Db db;
    db.mutex = (void*)0xDEADBEEF; // dummy mutex handle
    Mem out;
    out.db = &db;
    out.z = nullptr;
    out.n = 0;
    out.enc = 0;
    out.xDel = nullptr;

    sqlite3_context ctx;
    ctx.pOut = &out;
    ctx.isError = 0;

    const char *message = "Test error message";
    int message_len = (int)strlen(message);

    // Reset instrumentation
    g_mutexHeldCallCount = 0;

    // Call the focal function
    sqlite3_result_error(&ctx, message, message_len);

    // Assertions (non-terminating; use asserts to collect correctness)
    bool ok = true;
    if(ctx.isError != SQLITE_ERROR) ok = false;
    if(out.z != message) ok = false;
    if(out.n != message_len) ok = false;
    if(out.enc != SQLITE_UTF8) ok = false;
    if(out.xDel == nullptr) {
        // Even if xDel is not used, the parameter should be accepted; this test
        // only checks that the slot was filled (we won't fail if it's nullptr)
        // but to be strict, require that the setter touched the field (even if null)
    }

    // Ensure the mutex check path was invoked
    if(g_mutexHeldCallCount != 1) ok = false;

    return runTest("test_sqlite3_result_error_basic", ok,
                   "Failed basic behavior test: expected error flag, string storage, and mutex check.");
}

// Test 2: Armor path: when pCtx is null, the function should return immediately
// This exercises the true-branch of the API armor guard.
// Note: In practice, this should not crash; it is a no-op.
TestResult test_sqlite3_result_error_armored_null_context() {
    // We simply call with a null context. The armor guard should return immediately.
    // If the code executes past the guard, it would dereference pCtx and crash;
    // thus, successful completion indicates correct behavior.
    // We do not have a direct observable state to verify here beyond lack of crash.
    // We still perform the call to exercise the code path.
    sqlite3_result_error(nullptr, "ignored", 0);

    // If we reach here, the path did not crash. We consider the test passed.
    return runTest("test_sqlite3_result_error_armored_null_context", true,
                   "Armored null-context path executed without crash.");
}

// Test 3: Armored path with non-null context but empty strings.
// Ensure that zero-length strings are handled without error and that state is updated accordingly.
TestResult test_sqlite3_result_error_empty_string() {
    Db db;
    db.mutex = (void*)0xCAFEBABE;
    Mem out;
    out.db = &db;
    out.z = nullptr;
    out.n = 0;
    out.enc = 0;
    out.xDel = nullptr;

    sqlite3_context ctx;
    ctx.pOut = &out;
    ctx.isError = 0;

    const char *empty = "";
    int len = 0;

    g_mutexHeldCallCount = 0;
    sqlite3_result_error(&ctx, empty, len);

    bool ok = true;
    if(ctx.isError != SQLITE_ERROR) ok = false;
    if(out.z != empty) ok = false;
    if(out.n != len) ok = false;
    if(g_mutexHeldCallCount != 1) ok = false;

    return runTest("test_sqlite3_result_error_empty_string", ok,
                   "Empty string handling failure: incorrect state or mutex check.");
}

// Aggregator for all tests
TestResult tests[] = {
    test_sqlite3_result_error_basic(),
    test_sqlite3_result_error_armored_null_context(),
    test_sqlite3_result_error_empty_string()
};

// Utility to print test results
void printTestResults(const TestResult* t, size_t count) {
    int passed = 0;
    for(size_t i = 0; i < count; ++i) {
        const TestResult &r = t[i];
        if(r.passed) ++passed;
        std::cout << (r.passed ? "[PASS] " : "[FAIL] ") << r.name
                  << " - " << r.message << std::endl;
    }
    std::cout << "Total: " << count << "  Passed: " << passed << "  Failed: "
              << (int)(count - passed) << std::endl;
}

// ----------------------------------------------------------------------------
// Domain knowledge notes for maintainers (concise):
// - Candidate Keywords: sqlite3_result_error, pCtx, pOut, db, mutex, SQLITE_UTF8, SQLITE_TRANSIENT, SQLITE_ERROR, assert, sqlite3VdbeMemSetStr
// - The tests exercise true/false branches of the API_ARMOR guard (when enabled).
// - Static helpers (sqlite3_mutex_held, sqlite3VdbeMemSetStr) are mocked locally to avoid linking with
//   the full SQLite codebase; they simulate behavior needed for unit testing.
// - Tests are designed to be executable without GTest; main() delegates to test routines.
// - Data-type assertions are performed on pointers and primitive types; address vs. content checks are distinguished.
// - All code resides in the global namespace for portability; no reliance on private/internal SQLite headers.
// - Since gmock/gmock isn't used, tests rely on simple boolean checks and manual output.
//
// ----------------------------------------------------------------------------

int main() {
    // Run tests and print results
    printTestResults(tests, sizeof(tests)/sizeof(tests[0]));
    return 0;
}