// Test suite for the focal method: sqlite3BenignMallocHooks
// This file provides a self-contained, GTest-free unit test harness
// compatible with C++11. It exercises the behavior of sqlite3BenignMallocHooks
// and its related hooks/declarations as depicted in the provided focal dependencies.

#include <iostream>
#include <sqliteInt.h>


// Domain: Minimal reproduction of the environment required by sqlite3BenignMallocHooks

// Simulated global hook structure as expected by the focal method
struct WsdHooks {
    void (*xBenignBegin)(void);
    void (*xBenignEnd)(void);
};

// Static instance representing the global hooks in the original code
static WsdHooks wsdHooks;

// Macro to mimic the initialization macro seen in the focal source
// It resets the hook pointers before a new assignment
#define wsdHooksInit do { \
    wsdHooks.xBenignBegin = nullptr; \
    wsdHooks.xBenignEnd = nullptr; \
} while(false)

// Forward declarations of related functions (as in the dependency block)
void sqlite3BeginBenignMalloc(void);
void sqlite3EndBenignMalloc(void);

// Focal method under test (reproduced in this test harness for self-containment)
void sqlite3BenignMallocHooks(
    void (*xBenignBegin)(void),
    void (*xBenignEnd)(void)
){
    wsdHooksInit;
    wsdHooks.xBenignBegin = xBenignBegin;
    wsdHooks.xBenignEnd = xBenignEnd;
}

// Helper non-hook functions to serve as distinct function pointers for testing
static void BenignBeginFnA(void) { /* intentionally empty */ }
static void BenignEndFnA(void) { /* intentionally empty */ }

static void BenignBeginFnB(void) { /* intentionally empty */ }
static void BenignEndFnB(void) { /* intentionally empty */ }

// Globals to verify that non-hook functions can be invoked (for completeness)
static int g_begin_calls = 0;
static int g_end_calls = 0;

// Redefine the static-like "dependencies" to track invocation of file-scope helpers.
// These definitions mirror what the original code might expose as static functions
void sqlite3BeginBenignMalloc(void){
    ++g_begin_calls;
}
void sqlite3EndBenignMalloc(void){
    ++g_end_calls;
}

// Lightweight test harness (non-terminating assertions)
static int gFailures = 0;

static void reportFailure(const char* msg) {
    ++gFailures;
    std::cerr << "Failure: " << msg << std::endl;
}

// ASSERT-like macro that does not terminate tests on failure
#define ASSERT_TRUE(cond, msg) do { if(!(cond)) reportFailure(msg); } while(0)


// Test Case 1: Basic assignment with non-null pointers
// Verify that sqlite3BenignMallocHooks assigns the given function pointers
static void test_case_basic_assignment() {
    gFailures = 0;

    // Arrange: two distinct function pointers
    void (*beginPtr)(void) = BenignBeginFnA;
    void (*endPtr)(void)   = BenignEndFnA;

    // Act: install hooks
    sqlite3BenignMallocHooks(beginPtr, endPtr);

    // Assert: the internal wsdHooks reflect the provided pointers
    ASSERT_TRUE(wsdHooks.xBenignBegin == beginPtr,
                "Begin hook should be set to the provided function pointer (A).");
    ASSERT_TRUE(wsdHooks.xBenignEnd == endPtr,
                "End hook should be set to the provided function pointer (A).");

    // Report
    if(gFailures == 0) {
        std::cout << "test_case_basic_assignment: passed" << std::endl;
    } else {
        std::cout << "test_case_basic_assignment: " << gFailures << " failure(s)" << std::endl;
    }
}

// Test Case 2: Null pointers are assigned after initialization (reset behavior)
// The macro wsdHooksInit resets values before assignment
static void test_case_null_assignment() {
    gFailures = 0;

    // Act: set both hooks to nullptr
    sqlite3BenignMallocHooks(nullptr, nullptr);

    // Assert: internal state reflects null pointers after reset
    ASSERT_TRUE(wsdHooks.xBenignBegin == nullptr,
                "Begin hook should be nullptr after assigning null pointer.");
    ASSERT_TRUE(wsdHooks.xBenignEnd == nullptr,
                "End hook should be nullptr after assigning null pointer.");

    if(gFailures == 0) {
        std::cout << "test_case_null_assignment: passed" << std::endl;
    } else {
        std::cout << "test_case_null_assignment: " << gFailures << " failure(s)" << std::endl;
    }
}

// Test Case 3: Reassignment with different pointers updates internal state
static void test_case_reassignment() {
    gFailures = 0;

    // Arrange: new distinct function pointers
    void (*beginPtr2)(void) = BenignBeginFnB;
    void (*endPtr2)(void)   = BenignEndFnB;

    // Act: re-install hooks with different pointers
    sqlite3BenignMallocHooks(beginPtr2, endPtr2);

    // Assert: internal state reflects new pointers
    ASSERT_TRUE(wsdHooks.xBenignBegin == beginPtr2,
                "Begin hook should be updated to the new function pointer (B).");
    ASSERT_TRUE(wsdHooks.xBenignEnd == endPtr2,
                "End hook should be updated to the new function pointer (B).");

    if(gFailures == 0) {
        std::cout << "test_case_reassignment: passed" << std::endl;
    } else {
        std::cout << "test_case_reassignment: " << gFailures << " failure(s)" << std::endl;
    }
}

// Test Case 4: Basic invocation tracking of sqlite3BeginBenignMalloc and sqlite3EndBenignMalloc
static void test_case_begin_end_invocation() {
    gFailures = 0;
    g_begin_calls = 0;
    g_end_calls = 0;

    // Act: call the helper functions that are part of the dependency surface
    sqlite3BeginBenignMalloc();
    sqlite3EndBenignMalloc();

    // Assert: the counters reflect calls
    ASSERT_TRUE(g_begin_calls == 1,
                "sqlite3BeginBenignMalloc should increment begin counter exactly once.");
    ASSERT_TRUE(g_end_calls == 1,
                "sqlite3EndBenignMalloc should increment end counter exactly once.");

    if(gFailures == 0) {
        std::cout << "test_case_begin_end_invocation: passed" << std::endl;
    } else {
        std::cout << "test_case_begin_end_invocation: " << gFailures << " failure(s)" << std::endl;
    }
}


// Entry point to run all tests
int main() {
    std::cout << "Starting sqlite3BenignMallocHooks unit tests (non-GTest harness, C++11)" << std::endl;

    test_case_basic_assignment();
    test_case_null_assignment();
    test_case_reassignment();
    test_case_begin_end_invocation();

    // Summary
    int totalFailures = gFailures;
    if(totalFailures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "Total failures: " << totalFailures << std::endl;
        return totalFailures;
    }
}