// This is a self-contained C++11 test harness for the focal method sqlite3_status64.
// Since the original SQLite codebase (status.c) and internal dependencies are not
// linked here, we re-create a minimal, test-focused subset of the required
// infrastructure to exercise the core logic of the method as described in the
// provided FOCAL_METHOD snippet. The tests cover key branches and behaviors.

#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstdint>


// Domain-specific utilities for test assertions
static int gFailures = 0;

// Simple test assertion that continues execution on failure (non-terminating)
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test failure: " << (msg) << " [Line " << __LINE__ << "]\n"; \
        ++gFailures; \
    } \
} while(0)

// Basic SQLite-like types and macros (simplified)
typedef long long sqlite3_int64;
#define ArraySize(x) (sizeof(x)/sizeof((x)[0]))
#define SQLITE_OK 0
#define SQLITE_MISUSE_BKPT 1

// Minimal mutex scaffold to satisfy sqlite3_status64 dependencies
struct sqlite3_mutex { int dummy; };
static sqlite3_mutex mPcache;  // used by sqlite3Pcache1Mutex
static sqlite3_mutex mMalloc;   // used by sqlite3MallocMutex

static sqlite3_mutex* sqlite3Pcache1Mutex() { return &mPcache; }
static sqlite3_mutex* sqlite3MallocMutex()  { return &mMalloc; }
static void sqlite3_mutex_enter(sqlite3_mutex*) { /* no-op for test harness */ }
static void sqlite3_mutex_leave(sqlite3_mutex*) { /* no-op for test harness */ }

// Internal dependencies mimicked for the focal function
static const int WSD_SIZE = 4;  // Size of wsdStat.nowValue/mxValue arrays

// Per-op mutex decision flag: non-zero uses Pcache mutex, zero uses malloc mutex
static int statMutex[WSD_SIZE] = {1, 0, 1, 0};

// Structure that holds current and highwater values per operation
struct WsdStatStruct {
    sqlite3_int64 nowValue[WSD_SIZE];
    sqlite3_int64 mxValue[WSD_SIZE];
};
// Global statistic state used by sqlite3_status64
static WsdStatStruct wsdStat;

// Macro to initialize wsdStat (exactly as in the focal code's style)
#define wsdStatInit \
    do { \
        for(int i = 0; i < WSD_SIZE; ++i) { \
            wsdStat.nowValue[i] = 0; \
            wsdStat.mxValue[i]  = 0; \
        } \
    } while(0)

// Minimal stubs to mimic the real environment
static void reset_stat_values_for_tests() {
    // Provide deterministic initial values for tests
    for(int i = 0; i < WSD_SIZE; ++i) {
        wsdStat.nowValue[i] = i + 1;       // e.g., 1,2,3,4
        wsdStat.mxValue[i]  = (i + 1) * 10; // e.g., 10,20,30,40
    }
}

// Core: replica of the focal method behavior (without API_ARMOR macro)
static int sqlite3_status64_impl(int op,
                                 sqlite3_int64 *pCurrent,
                                 sqlite3_int64 *pHighwater,
                                 int resetFlag)
{
    sqlite3_mutex *pMutex;
    wsdStatInit;
    // Boundary check
    if( op < 0 || op >= ArraySize(wsdStat.nowValue) ){
        return SQLITE_MISUSE_BKPT;
    }
    // Armor branch would be here in the real code when SQLITE_ENABLE_API_ARMOR is defined
    pMutex = statMutex[op] ? sqlite3Pcache1Mutex() : sqlite3MallocMutex();
    sqlite3_mutex_enter(pMutex);
    *pCurrent  = wsdStat.nowValue[op];
    *pHighwater = wsdStat.mxValue[op];
    if( resetFlag ){
        wsdStat.mxValue[op] = wsdStat.nowValue[op];
    }
    sqlite3_mutex_leave(pMutex);
    (void)pMutex;  /* Suppress unused warning when threading is disabled in tests */
    return SQLITE_OK;
}

// Core: replica of the focal method behavior with API_ARMOR-like NULL-pointer check
// This variant simulates the code path when API armor is enabled and NULL pointers are rejected.
static int sqlite3_status64_witharmor(int op,
                                      sqlite3_int64 *pCurrent,
                                      sqlite3_int64 *pHighwater,
                                      int resetFlag)
{
    sqlite3_mutex *pMutex;
    wsdStatInit;
    if( op < 0 || op >= ArraySize(wsdStat.nowValue) ){
        return SQLITE_MISUSE_BKPT;
    }
    // Simulated API armor branch: reject NULL pointers
    if( pCurrent == 0 || pHighwater == 0 ) return SQLITE_MISUSE_BKPT;
    pMutex = statMutex[op] ? sqlite3Pcache1Mutex() : sqlite3MallocMutex();
    sqlite3_mutex_enter(pMutex);
    *pCurrent  = wsdStat.nowValue[op];
    *pHighwater = wsdStat.mxValue[op];
    if( resetFlag ){
        wsdStat.mxValue[op] = wsdStat.nowValue[op];
    }
    sqlite3_mutex_leave(pMutex);
    (void)pMutex;
    return SQLITE_OK;
}

// Simple test harness helpers
static void run_all_tests();

// Individual test implementations with explanatory comments

// Test 1: Op out of bounds (negative) should return MISUSE_BKPT and not modify outputs
static void test_op_out_of_bounds_negative() {
    // Arrange
    reset_stat_values_for_tests();
    sqlite3_int64 cur = -777, high = -888;
    // Act
    int rc = sqlite3_status64_impl(-1, &cur, &high, 0);
    // Assert
    TEST_ASSERT(rc == SQLITE_MISUSE_BKPT, "Negative op should return MISUSE_BKPT");
    TEST_ASSERT(cur == -777 && high == -888, "Output pointers should remain unchanged on misuse");
}

// Test 2: Op out of bounds (upper bound) should return MISUSE_BKPT
static void test_op_out_of_bounds_upper() {
    // Arrange
    reset_stat_values_for_tests();
    sqlite3_int64 cur = 0, high = 0;
    // Act
    int rc = sqlite3_status64_impl(WSD_SIZE, &cur, &high, 0);
    // Assert
    TEST_ASSERT(rc == SQLITE_MISUSE_BKPT, "Upper bound op should return MISUSE_BKPT");
    // No reliable guarantees about cur/high values in misuse path; just ensure no crash
}

// Test 3: In-range with resetFlag = 0 should return current and highwater without modifying highwater
static void test_in_range_no_reset() {
    // Arrange
    reset_stat_values_for_tests();
    // Set known values for a specific op
    int op = 2;
    wsdStat.nowValue[op] = 123;
    wsdStat.mxValue[op]  = 999;
    sqlite3_int64 cur = 0, high = 0;
    // Act
    int rc = sqlite3_status64_impl(op, &cur, &high, 0);
    // Assert
    TEST_ASSERT(rc == SQLITE_OK, "In-range op should return SQLITE_OK");
    TEST_ASSERT(cur == 123, "Current value should match nowValue[op]");
    TEST_ASSERT(high == 999, "Highwater should match mxValue[op]");
    // Ensure highwater not modified when resetFlag == 0
    TEST_ASSERT(wsdStat.mxValue[op] == 999, "mxValue should remain unchanged when resetFlag is 0");
}

// Test 4: In-range with resetFlag = 1 should update mxValue to current after call
static void test_in_range_with_reset() {
    // Arrange
    reset_stat_values_for_tests();
    int op = 1;
    wsdStat.nowValue[op] = 42;
    wsdStat.mxValue[op]  = 7;
    sqlite3_int64 cur = 0, high = 0;
    // Act
    int rc = sqlite3_status64_impl(op, &cur, &high, 1);
    // Assert
    TEST_ASSERT(rc == SQLITE_OK, "In-range op with reset should return SQLITE_OK");
    TEST_ASSERT(cur == 42, "Current value should reflect nowValue[op]");
    TEST_ASSERT(high == 7, "Highwater should reflect pre-reset mxValue[op]");
    TEST_ASSERT(wsdStat.mxValue[op] == 42, "mxValue should be updated to current after reset");
}

// Test 5: API armor behavior - NULL pointers should trigger MISUSE_BKPT
static void test_api_armor_rejects_null_pointers() {
    // Arrange
    reset_stat_values_for_tests();
    int op = 2;
    sqlite3_int64 cur, high;
    // Act
    int rc = sqlite3_status64_witharmor(op, NULL, &high, 0);
    // Assert
    TEST_ASSERT(rc == SQLITE_MISUSE_BKPT, "With API armor, NULL pCurrent should trigger MISUSE_BKPT");
}

// Run all tests and report summary
static void run_all_tests() {
    test_op_out_of_bounds_negative();
    test_op_out_of_bounds_upper();
    test_in_range_no_reset();
    test_in_range_with_reset();
    test_api_armor_rejects_null_pointers();

    if(gFailures > 0) {
        std::cerr << "Total failures: " << gFailures << "\n";
    } else {
        std::cout << "All tests passed.\n";
    }
}

int main() {
    // Initialize internal state and run tests
    reset_stat_values_for_tests();
    run_all_tests();
    return gFailures ? 1 : 0;
}