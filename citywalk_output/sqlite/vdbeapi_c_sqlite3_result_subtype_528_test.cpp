// A self-contained C++11 test harness for the focal function sqlite3_result_subtype.
// This test suite mocks minimal SQLite internal structures to exercise the core logic
// of sqlite3_result_subtype without requiring a full SQLite build.
//
// The focal method code copied here for testing purposes (as per the prompt).
#include <cstdio>
#include <opcodes.h>
#include <cstdarg>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <string>
#include <iostream>


// -----------------------------------------------------------------------------
// Lightweight test framework (non-terminating, suitable for C++11)
// -----------------------------------------------------------------------------

static int g_failures = 0;

#define LOG(...) do { printf(__VA_ARGS__); } while(0)
#define TEST_FAIL(msg) do { LOG("TEST FAILURE: %s\n", (msg)); g_failures++; } while(0)
#define EXPECT_TRUE(cond) do { if(!(cond)) { TEST_FAIL("EXPECT_TRUE(" #cond ") failed"); } } while(0)
#define EXPECT_EQ(a,b) do { if((a)!=(b)) { LOG("EXPECT_EQ failed: %lld != %lld\n", (long long)(a), (long long)(b)); g_failures++; } } while(0)
#define EXPECT_STR_EQ(a,b) do { if(((a)==0 && (b)!=0) || ((a)!=0 && (b)==0) || strcmp((a),(b))!=0) { LOG("EXPECT_STR_EQ failed: \"%s\" != \"%s\"\n", (a)?(a):"(null)", (b)?(b):"(null)"); g_failures++; } } while(0)

static void test_summary() {
    if(g_failures==0){
        LOG("ALL TESTS PASSED\n");
    } else {
        LOG("TOTAL FAILURES: %d\n", g_failures);
    }
}

// -----------------------------------------------------------------------------
// Minimal, test-only simulated SQLite internal types and helpers
// -----------------------------------------------------------------------------

// Macros to drive the focal code paths (compile-time switches)
#define SQLITE_ENABLE_API_ARMOR 1
#define SQLITE_STRICT_SUBTYPE 1
#define SQLITE_RESULT_SUBTYPE 0x01
#define MEM_Subtype 0x01

// Forward declarations/definitions to mirror the focal function's expectations
struct Mem;
struct Db;
struct Func;
struct sqlite3_context;

// Minimal dummy mutex check function
static int sqlite3_mutex_held(void *mutex){
    // For testing, any non-null mutex pointer indicates the mutex is held
    return mutex != nullptr;
}

// Simple snprintf wrapper compatible with printf family
static int sqlite3_snprintf(size_t n, char *z, const char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    int r = vsnprintf(z, n, fmt, ap);
    va_end(ap);
    if( r < 0 ) r = 0;
    return r;
}

// Global storage to inspect error messages from sqlite3_result_error
static std::string g_last_error;

// Simulated sqlite3_result_error: captures the error text for assertions
extern "C" void sqlite3_result_error(struct sqlite3_context *pCtx, const char *z, int n){
    (void)pCtx;
    (void)n;
    if(z){
        g_last_error.assign(z);
    }else{
        g_last_error.clear();
    }
}

// Simulated sqlite3_context structure (as used by the focal function)
struct sqlite3_context {
    Mem *pOut;
    Func *pFunc;
};

// Minimal helper structs to satisfy the focal code
struct Db {
    void *mutex;
};

struct Mem {
    unsigned int flags;
    unsigned int eSubtype;
    Db *db;
};

// Simple function descriptor to satisfy pCtx->pFunc->zName and flags
struct Func {
    unsigned int funcFlags;
    const char *zName;
};

// The focal function under test (copied/adapted for a self-contained test)
extern "C" void sqlite3_result_subtype(sqlite3_context *pCtx, unsigned int eSubtype){
  Mem *pOut;
#ifdef SQLITE_ENABLE_API_ARMOR
  if( pCtx==0 ) return;
#endif
#if defined(SQLITE_STRICT_SUBTYPE) && SQLITE_STRICT_SUBTYPE+0!=0
  if( pCtx->pFunc!=0
   && (pCtx->pFunc->funcFlags & SQLITE_RESULT_SUBTYPE)==0
  ){
    char zErr[200];
    sqlite3_snprintf(sizeof(zErr), zErr,
                     "misuse of sqlite3_result_subtype() by %s()", 
                     pCtx->pFunc->zName);
    sqlite3_result_error(pCtx, zErr, -1);
    return;
  }
#endif /* SQLITE_STRICT_SUBTYPE */
  pOut = pCtx->pOut;
  // In this test harness, we ensure the mutex is considered held if non-null
  assert( sqlite3_mutex_held(pOut->db->mutex) );
  pOut->eSubtype = eSubtype & 0xff;
  pOut->flags |= MEM_Subtype;
}

// -----------------------------------------------------------------------------
// Test harness setup: instances and helpers
// -----------------------------------------------------------------------------

// Global test objects to simulate a running environment
static Db g_test_db;
static Mem g_test_mem;
static Func g_test_func;
static sqlite3_context g_test_ctx;

// Utility to reset the test context/state before each test
static void reset_test_state() {
    g_test_db.mutex = (void*)0x1; // non-null to indicate "held"
    g_test_mem.flags = 0;
    g_test_mem.eSubtype = 0;
    g_test_mem.db = &g_test_db;
    g_test_func.funcFlags = 0;
    g_test_func.zName = "fname"; // default name used in error message
    g_test_ctx.pOut = &g_test_mem;
    g_test_ctx.pFunc = &g_test_func;
    g_last_error.clear();
}

// -----------------------------------------------------------------------------
// Individual test cases
// -----------------------------------------------------------------------------

// Test 1: When pCtx is null, the function should return without performing any action.
// This validates the API armor path (no crash on null context).
static void test_case_null_context(){
    // No setup required; ensure no crash and no error text set
    g_last_error.clear();
    sqlite3_result_subtype(nullptr, 0x55);
    EXPECT_TRUE(g_last_error.empty());
    LOG("Test 1: null context path executed (no crash)\\n");
}

// Test 2: Misuse path under SQLITE_STRICT_SUBTYPE: pCtx->pFunc != 0 and the
// functionFlags do not allow SUBTYPE. Should produce a descriptive error message.
static void test_case_strict_subtype_misuse(){
    reset_test_state();

    // Configure function to simulate misuse (flags WITHOUT SQLITE_RESULT_SUBTYPE)
    g_test_func.funcFlags = 0; // not allowing SUBTYPE
    g_test_func.zName = "misuseFunc";

    // Call the function
    sqlite3_result_subtype(&g_test_ctx, 0x12);

    // Verify the error message contains the correct function name
    const char *expected = "misuse of sqlite3_result_subtype() by misuseFunc()";
    EXPECT_STR_EQ(g_last_error.c_str(), expected);
    LOG("Test 2: strict subtype misuse path produced expected error: %s\\n", g_last_error.c_str());
}

// Test 3: Normal path: when the subtype flag is allowed, ensure
// eSubtype is set and MEM_Subtype flag is raised.
static void test_case_normal_path_sets_subtype(){
    reset_test_state();

    // Configure function to allow subtype
    g_test_func.funcFlags = SQLITE_RESULT_SUBTYPE;
    g_test_func.zName = "normalFunc";

    // Call the function with a sample subtype
    unsigned int testSubtype = 0xAB;
    sqlite3_result_subtype(&g_test_ctx, testSubtype);

    // Validate that the subtype was stored and the flag was set
    EXPECT_EQ(g_test_mem.eSubtype, testSubtype & 0xff);
    EXPECT_TRUE((g_test_mem.flags & MEM_Subtype) != 0);
    LOG("Test 3: normal path set subtype to 0x%02x and MEM_Subtype flag.\n", testSubtype & 0xff);
}

// -----------------------------------------------------------------------------
// Main: run all tests
// -----------------------------------------------------------------------------

int main(){
    // Run tests in sequence
    test_case_null_context();
    test_case_strict_subtype_misuse();
    test_case_normal_path_sets_subtype();

    test_summary();
    return g_failures;
}