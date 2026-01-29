// Test suite for sqlite3_sha_init using a lightweight, self-contained approach.
// This test uses a mock sqlite3_create_function to verify control flow
// (i.e., whether the second function is registered only after the first succeeds).
// It also demonstrates handling of the pApi parameter and verifies the two
// function registration attempts ("sha1" and "sha1_query").
//
// Notes:
// - This test assumes the environment provides the SQLite headers (sqlite3ext.h, etc.)
// - The sha1.c file is included in this translation unit to obtain sqlite3_sha_init.
// - A custom C symbol sqlite3_create_function is provided to mock SQLite's function
//   registration behavior without requiring a full SQLite runtime.
// - The test uses plain C/C++ assertions (no GTest) and a main function to run
//   the tests, adhering to the provided constraints.

#include <string.h>
#include <sha1.c>
#include <assert.h>
#include <sqlite3ext.h>
#include <cstring>
#include <stdarg.h>
#include <iostream>
#include <cassert>


// Forward declare sqlite3 types to ease integration with sha1.c which uses sqlite3ext.h.
struct sqlite3;
typedef struct sqlite3 sqlite3;

// Include the SQLite extension header before pulling in the focal method.
// This provides the necessary sqlite3 types/macros used by sqlite3_sha_init.
extern "C" {
}

// Include the focal method's source so we can test sqlite3_sha_init in this TU.
// We wrap in extern "C" to ensure C linkage for the focal function and any helpers.
extern "C" {
}

// Mock infrastructure for sqlite3_create_function to intercept and validate
// how sqlite3_sha_init calls the function creation API.
//
// The mock records:
// - The number of times sqlite3_create_function is invoked (call_count).
// - The name of the function being registered on each invocation (last_func_name).
// - The return codes to simulate for the first and second registrations (first_rc, second_rc).
// We can control the simulated outcomes per test to exercise true/false branches.

static int g_first_rc = 0;
static int g_second_rc = 0;
static int g_call_count = 0;
static const char* g_last_func_name = nullptr;

// Provide the mock implementation with C linkage so it matches the expected symbol
// resolved by the focal method when it calls sqlite3_create_function.
extern "C" int sqlite3_create_function(
    sqlite3* db,
    const char* zFunc,
    int argc,
    int textEncoding,
    void* pApp,
    void(*xFunc)(sqlite3_context*, int, sqlite3_value**),
    void(*xStep)(sqlite3_context*, int, sqlite3_value**),
    void(*xFinal)(sqlite3_context*)
) {
    // Record the function name for verification
    g_last_func_name = zFunc;
    // We only care about the sha1* registrations in this test
    if (zFunc && std::strcmp(zFunc, "sha1") == 0) {
        if (g_call_count == 0) {
            ++g_call_count;
            return g_first_rc;  // Simulate result of first registration
        } else if (g_call_count == 1) {
            ++g_call_count;
            return g_second_rc; // Simulate result of second registration
        }
    }
    // For any other function names (not used by sqlite3_sha_init), default to OK
    ++g_call_count;
    return 0;
}

// Helper to reset mock state before each test
static void reset_mock_state(int first_rc, int second_rc) {
    g_first_rc = first_rc;
    g_second_rc = second_rc;
    g_call_count = 0;
    g_last_func_name = nullptr;
}

// Test 1: Both registrations succeed (first_rc == 0, second_rc == 0)
// Expectation: sqlite3_sha_init returns SQLITE_OK (0) and two registration attempts occurred.
// The second function name should be "sha1_query".
static void test_sha_init_both_registrations_succeed() {
    reset_mock_state(0, 0);

    // pApi is a required non-null pointer by the macro SQLITE_EXTENSION_INIT2.
    // We use a dummy non-null pointer value.
    const sqlite3_api_routines* pApi = reinterpret_cast<const sqlite3_api_routines*>(reinterpret_cast<uintptr_t>(0x1));

    // Call the focal method with a null db (we don't exercise real DB behavior in this mock)
    sqlite3 *db = nullptr;
    char *pzErrMsg = nullptr;

    int rc = sqlite3_sha_init(db, &pzErrMsg, pApi);

    // Assertions to verify expected behavior
    assert(rc == (0)); // SQLITE_OK
    // Two registrations should have been attempted: sha1 then sha1_query
    assert(g_call_count == 2);
    // The second call should be for sha1_query
    assert(g_last_func_name != nullptr && std::strcmp(g_last_func_name, "sha1_query") == 0);

    std::cout << "test_sha_init_both_registrations_succeed: PASSED\n";
}

// Test 2: First registration fails (first_rc != 0)
// Expectation: sqlite3_sha_init returns the error code from the first registration
// and the second registration is not attempted.
static void test_sha_init_first_registration_fails() {
    reset_mock_state(-1, 0);

    const sqlite3_api_routines* pApi = reinterpret_cast<const sqlite3_api_routines*>(reinterpret_cast<uintptr_t>(0x1));
    sqlite3 *db = nullptr;
    char *pzErrMsg = nullptr;

    int rc = sqlite3_sha_init(db, &pzErrMsg, pApi);

    // Assertions to verify expected behavior
    assert(rc == -1);
    // Only one registration attempt should have occurred
    assert(g_call_count == 1);
    // The first call should be for sha1
    // If the first call was made, g_last_func_name should be "sha1"
    assert(g_last_func_name != nullptr && std::strcmp(g_last_func_name, "sha1") == 0);

    std::cout << "test_sha_init_first_registration_fails: PASSED\n";
}

// Main function to execute the tests in a straightforward, non-Gmock manner.
// This adheres to the requirement of "call test methods from the main function" when GTest is not used.
int main() {
    std::cout << "Running sqlite3_sha_init unit tests (inline, no GTest)..." << std::endl;

    test_sha_init_both_registrations_succeed();
    test_sha_init_first_registration_fails();

    std::cout << "All tests completed." << std::endl;
    return 0;
}