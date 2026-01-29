// Test suite for the focal method: lsmBlockRefree in lsm_shared.c
// This test is written in C++11 without using Google Test.
// It relies on providing a C-compatible override of freelistAppend
// so that the behavior of lsmBlockRefree can be precisely verified.
//
// How this test is structured:
// - A minimal C-compatible stub for freelistAppend is provided.
// - The stub records the parameters it was called with and returns a controlled value.
// - Two unit tests are implemented:
//     1) When freelistAppend returns a success code (0), lsmBlockRefree must return 0
//        and must pass the same iBlk and iId (0) to freelistAppend.
//     2) When freelistAppend returns a non-zero error code, lsmBlockRefree must
//        propagate that error code unchanged and still pass the correct iBlk and iId.
// - A lightweight, non-terminating assertion strategy is used (via simple checks).
// - The test does not rely on private members; it exercises the public interface of lsmBlockRefree
//   and the globally visible freelistAppend stub.

#include <lsmInt.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Forward declaration of the C type used by the focal function.
// The actual definition lives in the C source (lsm_shared.c); for test purposes
// we only need an opaque pointer to satisfy the interface.
struct lsm_db;

// Declaration of the focal function (C linkage for compatibility)
extern "C" int lsmBlockRefree(struct lsm_db *pDb, int iBlk);

// Test harness state to control and observe the behavior of freelistAppend.
static int s_next_return_code = 0;       // Value to return from freelistAppend
static uint32_t s_last_iBlk = 0;           // Last iBlk passed to freelistAppend
static int64_t s_last_iId = -1;            // Last iId passed to freelistAppend

// C linkage freelistAppend override used by lsmBlockRefree.
// This function mirrors the signature expected by the focal method.
// It records the call parameters so tests can assert correctness.
extern "C" int freelistAppend(struct lsm_db *db, uint32_t iBlk, int64_t iId) {
    (void)db; // pDb is not used by the test stub
    s_last_iBlk = iBlk;
    s_last_iId = iId;
    return s_next_return_code;
}

// Lightweight test framework (non-terminating, suitable for this setting)
static int g_total_tests = 0;
static int g_passed_tests = 0;

#define TEST_PASS(test_name) \
    do { \
        ++g_total_tests; \
        ++g_passed_tests; \
        std::cout << "[PASS] " << test_name << "\n"; \
    } while(0)

#define TEST_FAIL(test_name, msg) \
    do { \
        ++g_total_tests; \
        std::cerr << "[FAIL] " << test_name << ": " << msg << "\n"; \
    } while(0)

#define RUN_TEST(test_fn) \
    do { test_fn(); } while(0)

// Test 1: Verify that when freelistAppend returns success (0),
// lsmBlockRefree propagates the rc and passes the correct parameters.
static void test_lsmBlockRefree_propagates_success() {
    // Configure the stub to return success
    s_next_return_code = 0;
    s_last_iBlk = 0xFFFFFFFFu; // sentinel to detect modification
    s_last_iId = -2;             // sentinel to detect modification

    // Call the focal function with a test block ID
    int rc = lsmBlockRefree(nullptr, 123);

    const char* test_name = "lsmBlockRefree_propagates_success";

    // Assertions
    if (rc == 0 && s_last_iBlk == 123 && s_last_iId == 0) {
        TEST_PASS(test_name);
    } else {
        std::string msg;
        msg += "Expected rc=0, got rc=";
        msg += std::to_string(rc);
        msg += "; Expected iBlk=123, got ";
        msg += std::to_string(s_last_iBlk);
        msg += "; Expected iId=0, got ";
        msg += std::to_string(s_last_iId);
        TEST_FAIL(test_name, msg.c_str());
    }
}

// Test 2: Verify that when freelistAppend returns an error code,
// lsmBlockRefree propagates the error and still passes the correct parameters.
static void test_lsmBlockRefree_propagates_error() {
    // Configure the stub to return an error code
    s_next_return_code = -42;
    s_last_iBlk = 0;
    s_last_iId = -1;

    // Call the focal function with another test block ID
    int rc = lsmBlockRefree(nullptr, 999);

    const char* test_name = "lsmBlockRefree_propagates_error";

    // Assertions
    if (rc == -42 && s_last_iBlk == 999 && s_last_iId == 0) {
        TEST_PASS(test_name);
    } else {
        std::string msg;
        msg += "Expected rc=-42, got rc=";
        msg += std::to_string(rc);
        msg += "; Expected iBlk=999, got ";
        msg += std::to_string(s_last_iBlk);
        msg += "; Expected iId=0, got ";
        msg += std::to_string(s_last_iId);
        TEST_FAIL(test_name, msg.c_str());
    }
}

int main() {
    // Step 3 (Domain Knowledge): We run both tests to ensure coverage of the
    // positive and negative pathways of the underlying freelistAppend interaction.

    // Run the tests
    RUN_TEST(test_lsmBlockRefree_propagates_success);
    RUN_TEST(test_lsmBlockRefree_propagates_error);

    // Summary
    std::cout << "\nTest Summary: " << g_passed_tests << " / " << g_total_tests << " tests passed.\n";

    // Return non-zero if any test failed
    return (g_passed_tests == g_total_tests) ? 0 : 1;
}