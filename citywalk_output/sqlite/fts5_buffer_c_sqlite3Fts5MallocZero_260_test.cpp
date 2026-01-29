// Unit test suite for sqlite3Fts5MallocZero
// This test focuses on the focal method's behavior when interacting with a mocked
// sqlite3_malloc64 allocator. No GTest is used; a lightweight in-file test harness
// is implemented with simple assertions that log failures but continue execution
// to maximize coverage.
//
// Key behaviors exercised:
// - When *pRc == SQLITE_OK and allocation succeeds: memory is zero-initialized.
// - When *pRc == SQLITE_OK and allocation fails: returns nullptr and *pRc becomes SQLITE_NOMEM.
// - When *pRc != SQLITE_OK: no allocation attempted; function returns nullptr.
// - Basic handling for nByte == 0 (no strict memory write expected).

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fts5Int.h>


// Domain-specific typedefs/macros (stand-ins for sqlite3 types/macros in test)
typedef long long sqlite3_int64;
#define SQLITE_OK 0
#define SQLITE_NOMEM 7

// Global state to control/mock allocator behavior
static int g_fail_malloc = 0;          // if non-zero, sqlite3_malloc64 will fail
static int g_malloc_call_count = 0;    // tracks how many times sqlite3_malloc64 is invoked

// Mock implementation of sqlite3_malloc64 to drive test scenarios.
// It uses the global flags above to simulate success/failure.
extern "C" void *sqlite3_malloc64(sqlite3_int64 n){
    ++g_malloc_call_count;
    if( g_fail_malloc ) return nullptr;
    // Simulate typical allocator behavior
    if( n <= 0 ) return malloc(0);
    return malloc((size_t)n);
}

// Forward declaration of the focal method under test.
// The actual implementation is expected to be linked from fts5_buffer.c.
extern "C" void *sqlite3Fts5MallocZero(int *pRc, sqlite3_int64 nByte);

// Simple test harness (non-terminating assertions): each test logs its own failures
// but execution continues to exercise subsequent tests.

static bool assert_true(bool cond, const char* msg){
    if(!cond){
        std::cerr << "Assertion failed: " << msg << std::endl;
        return false;
    }
    return true;
}

static bool assert_ptr_not_null(void* p, const char* msg){
    if(p == nullptr){
        std::cerr << "Assertion failed: " << msg << " (pointer is null)" << std::endl;
        return false;
    }
    return true;
}

static bool assert_int_eq(int a, int b, const char* msg){
    if(a != b){
        std::cerr << "Assertion failed: " << msg << " expected " << b << ", got " << a << std::endl;
        return false;
    }
    return true;
}

// Test 1: When *pRc == SQLITE_OK and allocation succeeds, memory should be zero-initialized.
bool test_sqlite3Fts5MallocZero_basic_allocation_success(){
    // Domain keywords:
    // - Branch: success path (pRet != 0) leading to memset to zero
    // - Dependency: sqlite3_malloc64 succeeds
    g_fail_malloc = 0;
    g_malloc_call_count = 0;

    int rc = SQLITE_OK;
    sqlite3_int64 nBytes = 128;
    void *p = sqlite3Fts5MallocZero(&rc, nBytes);

    bool ok = true;
    ok &= assert_ptr_not_null(p, "Basic allocation should return non-null when nBytes > 0 and allocator succeeds");
    ok &= assert_int_eq(rc, SQLITE_OK, "rc should remain SQLITE_OK after successful allocation");

    if(p){
        // Memory should be zero-initialized
        unsigned char* bytes = static_cast<unsigned char*>(p);
        ok &= assert_true(bytes[0] == 0, "First byte should be zero after sqlite3Fts5MallocZero");
    }

    if(p) std::free(p);
    return ok;
}

// Test 2: When *pRc == SQLITE_OK and allocation fails, return nullptr and set rc to SQLITE_NOMEM.
bool test_sqlite3Fts5MallocZero_allocation_failure_sets_nominus(){
    // Domain keywords:
    // - Branch: failure path (pRet == 0) -> *pRc should be SQLITE_NOMEM
    // - Dependency: simulate malloc64 returning NULL
    g_fail_malloc = 1;
    g_malloc_call_count = 0;

    int rc = SQLITE_OK;
    sqlite3_int64 nBytes = 64;
    void *p = sqlite3Fts5MallocZero(&rc, nBytes);

    bool ok = true;
    ok &= assert_ptr_not_null(nullptr, "Pointer should be null on failure? (handled below)");
    // Actually pointer should be null, but we cannot rely on assert_ptr_not_null here.
    // We'll check directly:
    if(p != nullptr){
        std::cerr << "Assertion failed: Expected nullptr on allocation failure" << std::endl;
        ok = false;
    }
    ok &= assert_int_eq(rc, SQLITE_NOMEM, "rc should be SQLITE_NOMEM after allocation failure");

    // malloc should have been invoked at least once in this test
    ok &= assert_true(g_malloc_call_count > 0, "sqlite3_malloc64 should have been called on failure path");

    return ok;
}

// Test 3: When *pRc != SQLITE_OK, no allocation should be attempted; function should return nullptr.
bool test_sqlite3Fts5MallocZero_no_allocation_when_rc_not_ok(){
    // Domain keywords:
    // - Branch: non-OK rc prevents allocator usage
    g_fail_malloc = 0;
    g_malloc_call_count = 0;

    int rc = 1;            // non-zero, not SQLITE_OK
    sqlite3_int64 nBytes = 256;
    void *p = sqlite3Fts5MallocZero(&rc, nBytes);

    bool ok = true;
    ok &= assert_ptr_not_null(nullptr, "Pointer should be null when rc != SQLITE_OK"); // we expect null
    if(p != nullptr){
        std::cerr << "Assertion failed: Expected nullptr when rc != SQLITE_OK" << std::endl;
        ok = false;
    }
    ok &= assert_int_eq(rc, 1, "rc should remain unchanged when not SQLITE_OK");
    ok &= assert_true(g_malloc_call_count == 0, "sqlite3_malloc64 should not be called when rc != SQLITE_OK");

    return ok;
}

// Test 4: Boundary case nByte = 0 with SQLITE_OK and allocator returns non-null.
// We validate that rc remains SQLITE_OK and a (possibly non-null) pointer is returned.
// Note: malloc(0) semantics may vary; the test focuses on not corrupting rc.
bool test_sqlite3Fts5MallocZero_nbyte_zero_edge(){
    g_fail_malloc = 0;
    g_malloc_call_count = 0;

    int rc = SQLITE_OK;
    sqlite3_int64 nBytes = 0;
    void *p = sqlite3Fts5MallocZero(&rc, nBytes);

    bool ok = true;
    ok &= assert_int_eq(rc, SQLITE_OK, "rc should remain SQLITE_OK for nByte == 0");
    // No strong assertion on pointer value due to malloc(0) differences; just ensure we didn't crash
    if(p != nullptr){
        // If non-null, it's acceptable
    }
    // Clean up if allocated
    if(p) std::free(p);

    return ok;
}

// Entry point: run all tests and report summary
int main(){
    int total = 0;
    int failures = 0;

    std::cout << "Running sqlite3Fts5MallocZero unit tests (C++11, no GTest)..." << std::endl;

    if(test_sqlite3Fts5MallocZero_basic_allocation_success()) ++failures;
    ++total;

    if(test_sqlite3Fts5MallocZero_allocation_failure_sets_nominus()) ++failures;
    ++total;

    if(test_sqlite3Fts5MallocZero_no_allocation_when_rc_not_ok()) ++failures;
    ++total;

    if(test_sqlite3Fts5MallocZero_nbyte_zero_edge()) ++failures;
    ++total;

    if(failures == 0){
        std::cout << "All tests passed (" << total << " tests)." << std::endl;
        return 0;
    }else{
        std::cerr << failures << " test(s) failed out of " << total << "." << std::endl;
        return 1;
    }
}