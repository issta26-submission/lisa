// Test suite for sqlite3_nextchar_init from nextchar.c
// This suite provides lightweight unit tests without a testing framework.
// It uses a small mock of sqlite3_create_function to verify the control flow
// of sqlite3_nextchar_init (three potential registrations: arity 3, 4, 5).

#include <string.h>
#include <vector>
#include <sqlite3ext.h>
#include <cstddef>
#include <iostream>


// Forward declare minimal SQLite-like types to allow compiling this test
typedef struct sqlite3 sqlite3;
typedef struct sqlite3_api_routines sqlite3_api_routines;
typedef struct sqlite3_context sqlite3_context;
typedef struct sqlite3_value sqlite3_value;

// Forward declare the focal function under test
extern "C" int sqlite3_nextchar_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);

// We will provide a tiny mock for sqlite3_create_function with C linkage.
// The mock's behavior is driven by a configurable return sequence so we can
// exercise all branches of sqlite3_nextchar_init.
static std::vector<int> g_ReturnSequence; // per-call return values (0 = success)
static std::size_t g_CallIndex = 0;        // how many times sqlite3_create_function was invoked

// Dummy types to satisfy the function signature of sqlite3_create_function
extern "C" {

 // The mock function replaces the real sqlite3_create_function in the test build.
 int sqlite3_create_function(sqlite3*,
                             const char*,
                             int,
                             int,
                             void*,
                             void (*pxFunc)(sqlite3_context*, int, sqlite3_value**),
                             void (*pxStep)(sqlite3_context*, int, sqlite3_value**),
                             void (*pxFinal)(sqlite3_context*))
 {
     // Determine return based on configured sequence; default to OK (0) if exhausted
     int ret = 0;
     if (g_CallIndex < g_ReturnSequence.size())
         ret = g_ReturnSequence[g_CallIndex];
     else
         ret = 0; // SQLITE_OK equivalent for our tests

     ++g_CallIndex;
     // For testing purposes we do not need to actually register the function.
     // This mock simply records how many times it was called and returns the
     // configured code to drive sqlite3_nextchar_init's flow.
     (void)db;       // suppress unused parameter warnings when building
     (void)pxFunc;   // function pointer provided by the focal code (not used here)
     (void)pxStep;
     (void)pxFinal;
     (void)pxFunc; // silence unused parameter warnings if compiled differently
     return ret;
 }

} // extern "C"

// Helper to reset mock state between tests
static void reset_mock() {
    g_ReturnSequence.clear();
    g_CallIndex = 0;
}

// Helper to set the desired sequence of return values for each sqlite3_create_function call
static void set_return_sequence(const std::vector<int>& seq) {
    g_ReturnSequence = seq;
    g_CallIndex = 0;
}

// Minimal dummy pApi object to pass to sqlite3_nextchar_init
static sqlite3_api_routines dummyApiInstance;
static sqlite3_api_routines* const dummyApiPtr = &dummyApiInstance;

// Minimal dummy database object
static sqlite3 dummyDbInstance;
static sqlite3* const dummyDbPtr = &dummyDbInstance;

// Test 1: All three registrations succeed (arity 3, 4, and 5)
 // Expect sqlite3_nextchar_init to return 0 (OK) and perform exactly 3 registrations.
static bool test_three_registrations_ok() {
    reset_mock();
    // Configure mock to return SQLITE_OK (0) for the first three calls
    set_return_sequence({0, 0, 0});

    int rc = sqlite3_nextchar_init(dummyDbPtr, nullptr, dummyApiPtr);

    bool passed = (rc == 0) && (g_CallIndex == 3);
    std::cout << "Test 1 - three registrations succeed: " << (passed ? "PASS" : "FAIL")
              << " (rc=" << rc << ", calls=" << g_CallIndex << ")\n";
    return passed;
}

// Test 2: First registration fails
 // Expect sqlite3_nextchar_init to abort early and return non-zero code after first call.
static bool test_first_registration_fails() {
    reset_mock();
    // First call fails; subsequent calls should not be attempted
    set_return_sequence({1});

    int rc = sqlite3_nextchar_init(dummyDbPtr, nullptr, dummyApiPtr);

    bool passed = (rc == 1) && (g_CallIndex == 1);
    std::cout << "Test 2 - first registration fails: " << (passed ? "PASS" : "FAIL")
              << " (rc=" << rc << ", calls=" << g_CallIndex << ")\n";
    return passed;
}

// Test 3: First succeeds, second fails
 // Expect sqlite3_nextchar_init to perform two registrations and return the failing code
 static bool test_second_registration_fails() {
    reset_mock();
    // First OK, second fails; third not attempted
    set_return_sequence({0, 1});

    int rc = sqlite3_nextchar_init(dummyDbPtr, nullptr, dummyApiPtr);

    bool passed = (rc == 1) && (g_CallIndex == 2);
    std::cout << "Test 3 - second registration fails: " << (passed ? "PASS" : "FAIL")
              << " (rc=" << rc << ", calls=" << g_CallIndex << ")\n";
    return passed;
}

// Entrypoint for the test suite
int main() {
    bool a = test_three_registrations_ok();
    bool b = test_first_registration_fails();
    bool c = test_second_registration_fails();

    int total = a + b + c;
    std::cout << "Summary: " << total << "/3 tests passed.\n";

    return (total == 3) ? 0 : 1;
}