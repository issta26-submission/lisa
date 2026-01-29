// Test suite for sqlite3_create_disposable_module (vtshim.c) using a lightweight
// C++11 harness with manual assertions (no Google Test).
// This test suite mocks minimal SQLite dependencies to exercise key code paths
// in sqlite3_create_disposable_module, focusing on:
 // - Allocation failure (pAux==0 branch)
 // - rc == SQLITE_OK path
 // - rc != SQLITE_OK path
// IMPORTANT: This test is designed to be compiled together with vtshim.c and
// a minimal subset of sqlite3 headers available in the environment.
// The test provides lightweight stubs for sqlite3_malloc, sqlite3_free, sqlite3_mprintf
// and sqlite3_create_module_v2. It uses real vtshim.c behavior for other branches.

#include <string.h>
#include <vector>
#include <assert.h>
#include <cstdio>
#include <sqlite3ext.h>
#include <cstdarg>
#include <cstring>
#include <cstdlib>


// Forward declare SQLite-like types to keep test standalone.
// Real deployment uses actual sqlite3.h, but we keep the interface minimal here.
struct sqlite3;
struct sqlite3_module;
struct sqlite3_vtab;

typedef struct sqlite3 sqlite3;
typedef struct sqlite3_module sqlite3_module;
typedef struct sqlite3_vtab sqlite3_vtab;

// Prototype of the focal function under test (C linkage)
extern "C" void* sqlite3_create_disposable_module(
    sqlite3 *db,
    const char *zName,
    const sqlite3_module *p,
    void *pClientData,
    void (*xDestroy)(void*)
);

// Prototypes for dependencies we will mock (C linkage)
extern "C" void* sqlite3_malloc(size_t);
extern "C" void  sqlite3_free(void*);
extern "C" char* sqlite3_mprintf(const char*, ...);
extern "C" int   sqlite3_create_module_v2(sqlite3*, const char*, const sqlite3_module*, void*, void(*)(void*));

// Lightweight test harness state
namespace {
    // Test scenario management
    enum TestCase {
        TEST_CASE_OOM_ON_FIRST_ALLOC = 1,
        TEST_CASE_RC_OK = 2,
        TEST_CASE_RC_NOT_OK = 3
    };

    // Global test controls (manipulated by individual tests)
    static TestCase g_current_case = TEST_CASE_OOM_ON_FIRST_ALLOC;
    static int g_alloc_calls = 0;
    static void* g_last_alloc = nullptr;
    static std::vector<void*> g_freed_ptrs;

    // Destruction callback to verify it is invoked in OOM path
    static int g_destroy_called = 0;
    static void test_destroy(void*){
        g_destroy_called = 1;
    }

    // RC to return from sqlite3_create_module_v2 stub
    static int g_rc_to_return = 0; // 0 for SQLITE_OK, non-zero for error path

    // OOM mode flag
    static int g_oom_on_first_alloc = 0;

    // Dummy module function pointer for xCreate to satisfy signature
    // The real signature is defined in sqlite3.h, but we only need a placeholder here.
    // We provide a compatible function signature based on sqlite3_module::xCreate.
    static int dummy_xCreate(sqlite3* db, void* pAux, int argc, const char* const* argv, sqlite3_vtab** ppVtab, char** pzErr){
        (void)db; (void)pAux; (void)argc; (void)argv; (void)ppVtab; (void)pzErr;
        return 0;
    }

    // SD: Real code in vtshim.c uses the actual sqlite3_module layout; we only need to set
    // iVersion and at least one non-NULL xCreate to move through the code path that
    // prepares the sSelf module. The rest are left zeroed.

    // Mocks for SQLite-like API used by vtshim.c
    extern "C" {

    // Forward declare to satisfy the linker for test-driven compilation
    // Implementations below override actual SQLite functions when linked together.

    void* sqlite3_malloc(size_t n){
        // Simulate OOM on first allocation for specific test case
        if (g_current_case == TEST_CASE_OOM_ON_FIRST_ALLOC) {
            if (g_alloc_calls == 0) {
                ++g_alloc_calls;
                g_last_alloc = nullptr;
                return nullptr;
            }
        }
        void* p = std::malloc(n);
        g_alloc_calls++;
        g_last_alloc = p;
        return p;
    }

    void sqlite3_free(void* p){
        if(p) g_freed_ptrs.push_back(p);
        std::free(p);
    }

    char* sqlite3_mprintf(const char* fmt, ...){
        va_list ap;
        va_start(ap, fmt);
        int needed = vsnprintf(nullptr, 0, fmt, ap);
        va_end(ap);
        if(needed < 0){
            return nullptr;
        }
        char* buf = (char*)sqlite3_malloc((size_t)needed + 1);
        if(!buf) {
            va_list ap2;
            va_start(ap2, fmt);
            va_end(ap2);
            return nullptr;
        }
        va_start(ap, fmt);
        vsnprintf(buf, (size_t)needed + 1, fmt, ap);
        va_end(ap);
        return buf;
    }

    int sqlite3_create_module_v2(sqlite3* db, const char* zName, const sqlite3_module* p, void* pClientData, void(*destructor)(void*)){
        // Record call for test visibility (we can't access internals, but we can observe behavior via rc)
        (void)db; (void)zName; (void)p; (void)pClientData; (void)destructor;
        return g_rc_to_return;
    }

    } // extern "C"

} // anonymous namespace

// Test helpers
#define EXPECT(cond, msg) do { if(!(cond)) { printf("  [EXPECT FAIL] %s\n", (msg)); tests_failed++; } else { /* OK */ } } while(0)
static int tests_failed = 0;
static int tests_run = 0;
static void reset_test_state() {
    g_alloc_calls = 0;
    g_last_alloc = nullptr;
    g_freed_ptrs.clear();
    g_destroy_called = 0;
    g_rc_to_return = 0;
    g_oom_on_first_alloc = 0;
}

// Test 1: Allocation failure triggers pAux==0 path and calls xDestroy when provided
bool test_oom_allocation_path(){
    reset_test_state();
    g_current_case = TEST_CASE_OOM_ON_FIRST_ALLOC;
    g_oom_on_first_alloc = 1;
    g_rc_to_return = 0;
    g_destroy_called = 0;
    // Prepare a minimal module descriptor
    sqlite3_module mod;
    std::memset(&mod, 0, sizeof(mod));
    // Provide a non-null xCreate to exercise more fields (not used due to early exit)
    mod.iVersion = 1;
    mod.xCreate = dummy_xCreate;

    void* result = sqlite3_create_disposable_module((sqlite3*)0x1234, "oom_mod", &mod, (void*)0xAB, test_destroy);
    bool ok = (result == nullptr) && (g_destroy_called == 1);
    EXPECT(ok, "OOM path should return NULL and call xDestroy when provided");
    return ok;
}

// Test 2: rc == SQLITE_OK path returns non-null pointer
bool test_rc_ok_path(){
    reset_test_state();
    g_current_case = TEST_CASE_RC_OK;
    g_rc_to_return = 0; // SQLITE_OK
    // Prepare module with iVersion and non-null xCreate to exercise assignments
    sqlite3_module mod;
    std::memset(&mod, 0, sizeof(mod));
    mod.iVersion = 2;
    mod.xCreate = dummy_xCreate;
    void* result = sqlite3_create_disposable_module((sqlite3*)0x1234, "ok_mod", &mod, NULL, NULL);
    bool ok = (result != nullptr);
    EXPECT(ok, "RC_OK path should return non-null pointer");
    return ok;
}

// Test 3: rc != SQLITE_OK path returns NULL and frees allocated pAux
bool test_rc_not_ok_path(){
    reset_test_state();
    g_current_case = TEST_CASE_RC_NOT_OK;
    g_rc_to_return = 1; // non-zero => error
    // Prepare module
    sqlite3_module mod;
    std::memset(&mod, 0, sizeof(mod));
    mod.iVersion = 2;
    mod.xCreate = dummy_xCreate;
    // First allocate path occurs; ensure allocation captured
    void* res = sqlite3_create_disposable_module((sqlite3*)0xDEAD, "not_ok_mod", &mod, NULL, NULL);
    bool ok = (res == nullptr);
    EXPECT(ok, "RC_NOT_OK path should return NULL");

    // Ensure that a pAux allocation occurred and was freed
    bool freed = !g_freed_ptrs.empty();
    bool frees_match_alloc = false;
    if (freed) {
        void* freed_ptr = g_freed_ptrs.back();
        frees_match_alloc = (freed_ptr == g_last_alloc);
    }
    EXPECT(freed && frees_match_alloc, "On RC_NOT_OK, pAux should be freed (free called with allocated pointer)");
    return ok && freed && frees_match_alloc;
}

// Main test runner
int main(){
    printf("Starting vtshim tests for sqlite3_create_disposable_module...\n");

    bool t1 = test_oom_allocation_path();
    tests_run++;

    bool t2 = test_rc_ok_path();
    tests_run++;

    bool t3 = test_rc_not_ok_path();
    tests_run++;

    printf("Tests run: %d, Failures: %d\n", tests_run, tests_failed);

    if(t1 && t2 && t3 && tests_failed==0){
        printf("All tests PASSED.\n");
        return 0;
    }else{
        printf("Some tests FAILED.\n");
        return 1;
    }
}

/*
Notes for building and running:

1) Source organization:
   - vtshim.c should be compiled together with this test file.
   - Provide a minimal sqlite3.h equivalent or use the real sqlite3.h from SQLite distribution.
   - The test uses lightweight, hand-written mocks for sqlite3_malloc, sqlite3_free, sqlite3_mprintf,
     and sqlite3_create_module_v2 to exercise the code paths inside sqlite3_create_disposable_module.

2) How to compile (example, adjust include paths as needed):
   g++ -std=c++11 -O2 -Wall -Wextra \
     test_vtshim.cpp vtshim.c -Isrc/sqlite -o test_vtshim

3) Test behavior notes:
   - Test 1 simulates an out-of-memory on allocation, ensuring destructor is invoked and
     the function returns NULL.
   - Test 2 simulates a successful path (rc == SQLITE_OK) and expects a non-null result.
   - Test 3 forces rc to a non-zero value, verifying the function returns NULL and that the allocated
     pAux is freed.

4) Static/private behavior:
   - This test does not touch private members directly but exercises public entry and critical branches
     by simulating environmental conditions (allocation failure, rc values).

5) Domain knowledge alignment:
   - We use a real sqlite3_module-like structure to trigger the vtshim wiring in sqlite3_create_disposable_module.
   - We respect the expected function signature for the xCreate helper and the module constructor path.

6) Limitations:
   - This harness relies on linking vtshim.c with the mocked sqlite3_* functions. If vtshim.c uses any
     additional hidden dependencies, adapt mocks accordingly.
*/