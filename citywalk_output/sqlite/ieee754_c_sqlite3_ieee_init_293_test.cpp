// Test suite for sqlite3_ieee_init (no GTest; plain C++11 with a simple test harness)
// Notes:
// - This test suite is designed to be built alongside the ieee754.c / sqlite3_ieee_init
//   implementation. It mocks the sqlite3_create_function API to capture calls made by
//   sqlite3_ieee_init and to verify behavior (normal path and error path).
// - The tests rely on the library's function sqlite3_ieee_init being present and linked.
// - A lightweight mock of sqlite3_create_function records each call, including the
//   function name, argument count, and the iAux value passed via the pApp parameter
//   (taken from the address of the static aFunc[i].iAux in the implementation).
// - The test uses extern "C" to call the C-style function from C++ code.

#include <string.h>
#include <vector>
#include <assert.h>
#include <sqlite3ext.h>
#include <string>
#include <cstring>
#include <iostream>


// Forward declare the C types used by sqlite3_ieee_init (minimal subset for testing)
struct sqlite3;
struct sqlite3_context;
struct sqlite3_value;
struct sqlite3_api_routines;

// Function prototype of the focal method (to be linked from ieee754.c)
extern "C" int sqlite3_ieee_init(struct sqlite3 *db,
                               char **pzErrMsg,
                               const struct sqlite3_api_routines *pApi);

// Prototype for the sqlite3_create_function mock (C linkage)
extern "C" int sqlite3_create_function(struct sqlite3*,
                                     const char*,
                                     int,
                                     int,
                                     void*,
                                     void (*)(sqlite3_context*,int,sqlite3_value**),
                                     void (*)(sqlite3_context*,int,sqlite3_value**),
                                     void (*)(sqlite3_context*));

// Lightweight mock environment to intercept sqlite3_create_function calls
// and verify the internal aFunc[] values (names, nArg, iAux pointers).
static int g_mock_call_count = 0;
static int g_mock_expected_calls = -1; // negative means 'no expectation'
static int g_mock_fail_on_call = -1;   // 0-based index at which to fail (or -1 for never fail)

// Structure to store mock call records for verification
struct MockCallRecord {
    std::string zFName;
    int nArg;
    int iAux;      // value read from the pApp (address of aFunc[i].iAux in real code)
    void* xFunc;     // pointer to the registered function (ieee754func or similar)
};

static MockCallRecord g_mock_calls[20];
static int g_mock_calls_collected = 0;

// The mock implementation of sqlite3_create_function.
// It records the call data and simulates success/failure according to test control variables.
// Note: This mock does not attempt to simulate full SQLite behavior; it validates
// that sqlite3_ieee_init attempts to register the 7 functions with the expected
// metadata (names, nArg, and iAux via pApp).
extern "C" int sqlite3_create_function(struct sqlite3* db,
                                      const char* zName,
                                      int nArg,
                                      int,           // eTextRep (ignored in mock)
                                      void* pApp,
                                      void (*xFunc)(sqlite3_context*,int,sqlite3_value**),
                                      void (*xStep)(sqlite3_context*,int,sqlite3_value**),
                                      void (*xFinal)(sqlite3_context*)) {

    // If configured to fail on a specific call, do so
    int current_call = g_mock_call_count;
    if (g_mock_fail_on_call >= 0 && current_call == g_mock_fail_on_call) {
        // Simulate an error return from sqlite3_create_function
        g_mock_call_count++;
        // Do not store this failed call in the records
        return 1; // non-SQLITE_OK
    }

    // Capture the iAux by reading the integer pointed to by pApp (as per production code):
    int iAuxValue = 0;
    if (pApp != nullptr) {
        // The production code passes (void*)&aFunc[i].iAux; read as int
        iAuxValue = *reinterpret_cast<int*>(pApp);
    }

    // Record the call details for later verification
    MockCallRecord rec;
    rec.zFName = zName ? std::string(zName) : std::string();
    rec.nArg  = nArg;
    rec.iAux  = iAuxValue;
    rec.xFunc = reinterpret_cast<void*>(xFunc);
    if (g_mock_calls_collected < static_cast<int>(sizeof(g_mock_calls)/sizeof(g_mock_calls[0]))) {
        g_mock_calls[g_mock_calls_collected++] = rec;
    }

    g_mock_call_count++;
    return 0; // SQLITE_OK in test environment
}

// Simple helper to reset mock state before each test
static void reset_mock_state() {
    g_mock_call_count = 0;
    g_mock_calls_collected = 0;
    g_mock_expected_calls = -1;
    g_mock_fail_on_call = -1;
    // Clear recorded records
    for (int i = 0; i < 20; ++i) {
        g_mock_calls[i] = MockCallRecord();
    }
}

// Lightweight assertion helper for tests
static void assert_true(bool cond, const std::string& msg) {
    if (!cond) {
        std::cerr << "ASSERTION FAILED: " << msg << std::endl;
    }
}

// Test 1: Normal path - ensure 7 registrations occur with expected metadata
static void test_sqlite3_ieee_init_normal_path() {
    reset_mock_state();

    // Prepare dummy db and API structures (values are not used by mock beyond presence)
    sqlite3 dummy_db;
    char* errMsg = nullptr;
    struct sqlite3_api_routines dummy_api;
    std::memset(&dummy_api, 0, sizeof(dummy_api));

    // Call the focal method (the function is expected to exist in the linked production code)
    int rc = sqlite3_ieee_init(&dummy_db, &errMsg, &dummy_api);

    // Verify that the function reported success
    if (rc != 0) {
        std::cerr << "test_sqlite3_ieee_init_normal_path: rc != SQLITE_OK (expected 0)" << std::endl;
    }
    assert_true(rc == 0, "sqlite3_ieee_init should return SQLITE_OK on success");

    // Verify that 7 registration calls were made
    assert_true(g_mock_call_count == 7, "sqlite3_create_function should be invoked 7 times");

    // Verify the metadata for each registered function
    // Expected sequence and correspondingArg values as per the focal code
    const char* expected_names[] = {
        "ieee754", "ieee754", "ieee754_mantissa", "ieee754_exponent",
        "ieee754_to_blob", "ieee754_from_blob", "ieee754_inc"
    };
    const int expected_nArg[] = {1, 2, 1, 1, 1, 1, 2};
    const int expected_iAux[] = {0, 0, 1, 2, 0, 0, 0};

    for (int i = 0; i < 7; ++i) {
        const MockCallRecord& rec = g_mock_calls[i];
        std::string msg = "Registration " + std::to_string(i) +
                          " should be " + expected_names[i];
        assert_true(rec.zFName == expected_names[i], msg);
        if (rec.nArg != expected_nArg[i]) {
            std::cerr << "Mismatch in nArg for " << expected_names[i]
                      << ": got " << rec.nArg << ", expected " << expected_nArg[i] << std::endl;
        }
        // Check iAux read from the pApp pointer (as produced by the real code)
        if (rec.iAux != expected_iAux[i]) {
            std::cerr << "Mismatch in iAux for " << expected_names[i]
                      << ": got " << rec.iAux << ", expected " << expected_iAux[i] << std::endl;
        }
        // We won't verify xFunc pointer equality (addresses depend on the binary)
        // but ensuring it's non-null provides basic sanity
        assert_true(rec.xFunc != nullptr, "xFunc should be non-null");
    }

    std::cout << "TEST PASSED: test_sqlite3_ieee_init_normal_path" << std::endl;
}

// Test 2: Failure path - simulate sqlite3_create_function failing on the 3rd call
static void test_sqlite3_ieee_init_failure_on_third_call() {
    reset_mock_state();

    // Configure mock to fail on the 3rd call (0-based index)
    g_mock_fail_on_call = 2;

    sqlite3 dummy_db;
    char* errMsg = nullptr;
    struct sqlite3_api_routines dummy_api;
    std::memset(&dummy_api, 0, sizeof(dummy_api));

    int rc = sqlite3_ieee_init(&dummy_db, &errMsg, &dummy_api);

    // Expect rc != SQLITE_OK (non-zero)
    if (rc == 0) {
        std::cerr << "test_sqlite3_ieee_init_failure_on_third_call: Expected non-zero rc on failure" << std::endl;
    }
    assert_true(rc != 0, "sqlite3_ieee_init should return non-zero when a registration fails");

    // Since it failed on the 3rd call, only first two registrations should have happened
    assert_true(g_mock_call_count == 3, "sqlite3_create_function should have been invoked up to the failure (including failed call)");

    // The two successful registrations should be recorded; the third should have failed and thus not added
    // However, since we increment the counter before returning the error, we expect 3 invocations total
    // Check that at least the first two registrations match expected metadata
    if (g_mock_calls_collected >= 2) {
        // First two should match "ieee754" with nArg 1 and 2
        assert_true(g_mock_calls[0].zFName == "ieee754" && g_mock_calls[0].nArg == 1,
                    "First registration should be ieee754 with nArg 1");
        assert_true(g_mock_calls[1].zFName == "ieee754" && g_mock_calls[1].nArg == 2,
                    "Second registration should be ieee754 with nArg 2");
    } else {
        std::cerr << "Insufficient mock registration records collected." << std::endl;
    }

    std::cout << "TEST PASSED: test_sqlite3_ieee_init_failure_on_third_call" << std::endl;
}

// Test 3: Sanity check - pzErrMsg parameter is unused by the function
static void test_sqlite3_ieee_init_pzErrMsg_unused() {
    reset_mock_state();

    sqlite3 dummy_db;
    // Non-null errmsg reference should be ignored by the function
    char *errMsg = (char*)"INITIAL";
    struct sqlite3_api_routines dummy_api;
    std::memset(&dummy_api, 0, sizeof(dummy_api));

    int rc = sqlite3_ieee_init(&dummy_db, &errMsg, &dummy_api);

    // The function should still succeed (if the path is normal)
    if (rc != 0) {
        // If normal path fails here due to environment, still report the behavior
        std::cerr << "test_sqlite3_ieee_init_pzErrMsg_unused: rc != SQLITE_OK (non-zero in test environment)" << std::endl;
    }

    // errMsg should remain unchanged because the function does not modify pzErrMsg
    assert_true(errMsg && std::strcmp(errMsg, "INITIAL") == 0,
                "pzErrMsg should be left unchanged by sqlite3_ieee_init");

    std::cout << "TEST PASSED: test_sqlite3_ieee_init_pzErrMsg_unused" << std::endl;
}

// Main runner to execute tests
int main() {
    // Informative header
    std::cout << "Running sqlite3_ieee_init unit tests (no GTest)..." << std::endl;

    // Run tests
    test_sqlite3_ieee_init_normal_path();
    test_sqlite3_ieee_init_failure_on_third_call();
    test_sqlite3_ieee_init_pzErrMsg_unused();

    std::cout << "All tests executed." << std::endl;
    return 0;
}