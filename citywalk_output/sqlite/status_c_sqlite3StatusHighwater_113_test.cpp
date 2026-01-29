// Test suite for sqlite3StatusHighwater (mocked environment)
// This test harness provides a self-contained emulation of the minimal
// sqlite3StatusHighwater dependencies so we can execute and verify the
// core logic without requiring the full SQLite source tree.
// Notes:
// - We implement a local replica of the function body from the focal method.
// - We exercise both paths for mutex selection (Pcache1Mutex vs MallocMutex).
// - We verify the "highwater" update behavior: only when newValue > mxValue.
// - All tests are non-Google-Test, using simple return flags and console output.

#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// Domain constants (mirror the ones used in the focal method)
#define SQLITE_STATUS_MALLOC_SIZE    0
#define SQLITE_STATUS_PAGECACHE_SIZE 1
#define SQLITE_STATUS_PARSER_STACK   2

// Helper macro to compute array size (as used in the focal code)
#define ArraySize(x) (sizeof(x)/sizeof((x)[0]))

// Typedef matching the focal environment
typedef uint32_t sqlite3StatValueType;

// Forward declaration of global state used by sqlite3StatusHighwater
// (emulated environment)
struct WsdStat {
    sqlite3StatValueType nowValue[3];
    sqlite3StatValueType mxValue[3];
} wsdStat;

// Emulated mutex objects (placeholders)
void* sqlite3Pcache1Mutex() { return (void*)0xA1; }
void* sqlite3MallocMutex()  { return (void*)0xB2; }

// Emulated mutex-ownership predicate
bool sqlite3_mutex_held(void* mutex) {
    // In tests, we consider the mutex "held" if it matches one of our sentinels
    return (mutex == (void*)0xA1) || (mutex == (void*)0xB2);
}

// Array of per-operation mutex pointers (NULL means use MallocMutex)
void* statMutex[3];

// Lightweight initialization macro (mimics wsdStatInit in the focal code)
#define wsdStatInit do {                                       \
    for (int _i = 0; _i < 3; _i++) {                           \
        wsdStat.nowValue[_i] = 0;                              \
        wsdStat.mxValue[_i] = 0;                               \
    }                                                          \
} while(0)

// The focal method under test, re-implemented to run in this test harness
void sqlite3StatusHighwater(int op, int X){
    sqlite3StatValueType newValue;
    wsdStatInit;                        // Initialize working/statistics data
    assert( X>=0 );
    newValue = (sqlite3StatValueType)X;
    assert( op>=0 && op<ArraySize(wsdStat.nowValue) );
    assert( op>=0 && op<ArraySize(statMutex) );
    assert( sqlite3_mutex_held(statMutex[op] ? sqlite3Pcache1Mutex()
                                             : sqlite3MallocMutex()) );
    assert( op==SQLITE_STATUS_MALLOC_SIZE
            || op==SQLITE_STATUS_PAGECACHE_SIZE
            || op==SQLITE_STATUS_PARSER_STACK );
    if( newValue>wsdStat.mxValue[op] ){
        wsdStat.mxValue[op] = newValue;
    }
}

// Utility: reset global state between tests
void resetGlobals() {
    // Clear arrays
    for (int i = 0; i < 3; ++i) {
        wsdStat.nowValue[i] = 0;
        wsdStat.mxValue[i]  = 0;
        statMutex[i] = nullptr;
    }
}

// Simple assertion helper (non-terminating for test suite)
bool expect_eq_uint(const char* desc, uint32_t a, uint32_t b) {
    if (a == b) {
        std::cout << "[PASS] " << desc << " (" << a << " == " << b << ")" << std::endl;
        return true;
    } else {
        std::cout << "[FAIL] " << desc << " (expected " << b << ", got " << a << ")" << std::endl;
        return false;
    }
}

// Tests
bool test_update_with_pcache_mutex_path() {
    // Arrange
    resetGlobals();
    int op = SQLITE_STATUS_PARSER_STACK; // 2
    uint32_t initial = 5;
    uint32_t newVal  = 10;
    wsdStat.mxValue[op] = initial;
    statMutex[op] = (void*)1; // non-NULL => Pcache1Mutex path

    // Act
    sqlite3StatusHighwater(op, (int)newVal);

    // Assert
    bool ok = expect_eq_uint("update mxValue when newValue > mxValue (pcache path)",
                            wsdStat.mxValue[op], newVal);

    return ok;
}

bool test_update_with_malloc_mutex_path() {
    // Arrange
    resetGlobals();
    int op = SQLITE_STATUS_MALLOC_SIZE; // 0
    uint32_t initial = 0;
    uint32_t newVal  = 3;
    wsdStat.mxValue[op] = initial;
    statMutex[op] = nullptr; // NULL => MallocMutex path

    // Act
    sqlite3StatusHighwater(op, (int)newVal);

    // Assert
    bool ok = expect_eq_uint("update mxValue when newValue > mxValue (malloc path)",
                            wsdStat.mxValue[op], newVal);

    return ok;
}

bool test_no_update_when_lower_value() {
    // Arrange
    resetGlobals();
    int op = SQLITE_STATUS_PAGECACHE_SIZE; // 1
    uint32_t initial = 7;
    uint32_t newVal  = 5; // less than current
    wsdStat.mxValue[op] = initial;
    statMutex[op] = nullptr; // Malloc path; value should not change

    // Act
    sqlite3StatusHighwater(op, (int)newVal);

    // Assert
    bool ok = expect_eq_uint("no update when newValue <= mxValue",
                            wsdStat.mxValue[op], initial);

    return ok;
}

bool test_independent_ops() {
    // Arrange
    resetGlobals();
    // First operation
    int opA = SQLITE_STATUS_MALLOC_SIZE; // 0
    uint32_t valA = 4;
    wsdStat.mxValue[opA] = 0;
    statMutex[opA] = (void*)1; // Pcache path
    // Second operation
    int opB = SQLITE_STATUS_PARSER_STACK; // 2
    uint32_t valB = 9;
    wsdStat.mxValue[opB] = 2;
    statMutex[opB] = nullptr; // Malloc path

    // Act
    sqlite3StatusHighwater(opA, (int)valA);
    sqlite3StatusHighwater(opB, (int)valB);

    // Assert
    bool ok1 = expect_eq_uint("independent ops: mxValue for opA updated", wsdStat.mxValue[opA], valA);
    bool ok2 = expect_eq_uint("independent ops: mxValue for opB updated", wsdStat.mxValue[opB], valB);
    return ok1 && ok2;
}

int main() {
    std::cout << "Starting sqlite3StatusHighwater unit tests (mocked env)" << std::endl;

    bool all = true;
    all &= test_update_with_pcache_mutex_path();
    all &= test_update_with_malloc_mutex_path();
    all &= test_no_update_when_lower_value();
    all &= test_independent_ops();

    if (all) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << "Some tests FAILED." << std::endl;
        return 1;
    }
}