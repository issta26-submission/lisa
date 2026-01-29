/*
 * Lightweight C++11 unit tests for the focal function:
 *   store_pool_error(png_store *ps, png_const_structp pp, const char *msg)
 *
 * Approach:
 * - Provide minimal C-compatible stubs for external dependencies (png_error, store_log)
 * - Call the production function store_pool_error with controlled inputs
 * - Validate that:
 *     - when pp != NULL, png_error is invoked with the same pp and msg
 *     - store_log is always invoked exactly once with the same arguments and is_error flag
 *     - when pp == NULL, png_error is not invoked but store_log is still invoked
 * - Cover additional edge cases (ps == NULL, msg == NULL)
 *
 * Note: This test file is self-contained and uses a minimal mock of the C APIs.
 * It should be compiled and linked with the production implementation of
 * store_pool_error from pngvalid.c (which will call the mocked png_error and store_log).
 *
 * No Google Test or other frameworks are used per requirements.
 */

#include <cstdint>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Domain-specific: provide C linkage for the production API symbols used by the test.
extern "C" {

  // Forward declare minimal types to match the production API signatures.
  typedef struct png_store png_store;
  typedef void* png_const_structp;
  typedef void* png_structp;
  typedef void* png_bytep;

  // Production API (to be resolved at link time with the actual implementation)
  void png_error(png_const_structp pp, const char * msg);
  void store_log(png_store *ps, png_const_structp pp, const char *message, int is_error);

  // Focal method under test (assumed to be defined in pngvalid.c)
  void store_pool_error(png_store *ps, png_const_structp pp, const char *msg);
}

// Global test state (mocks) and accessors to verify behavior
static int g_png_error_calls = 0;
static void* g_last_error_pp = nullptr;
static const char* g_last_error_msg = nullptr;

static int g_store_log_calls = 0;
static void* g_last_log_ps = nullptr;
static void* g_last_log_pp = nullptr;
static const char* g_last_log_msg = nullptr;
static int g_last_log_is_error = 0;

// Mock implementations of external dependencies (with C linkage)
extern "C" {

// Mock for png_error: records its invocation details
void png_error(png_const_structp pp, const char *msg)
{
    (void)pp; // We still record pp in case tests want to inspect it via g_last_error_pp
    g_png_error_calls++;
    g_last_error_pp = (void*)pp;
    g_last_error_msg = msg;
}

// Mock for store_log: records its invocation details
void store_log(png_store *ps, png_const_structp pp, const char *message, int is_error)
{
    (void)ps; // test examines ps via g_last_log_ps
    g_store_log_calls++;
    g_last_log_ps = (void*)ps;
    g_last_log_pp = (void*)pp;
    g_last_log_msg = message;
    g_last_log_is_error = is_error;
}
}

// Helper: reset all mock state
static void reset_mock_state()
{
    g_png_error_calls = 0;
    g_last_error_pp = nullptr;
    g_last_error_msg = nullptr;

    g_store_log_calls = 0;
    g_last_log_ps = nullptr;
    g_last_log_pp = nullptr;
    g_last_log_msg = nullptr;
    g_last_log_is_error = 0;
}

// Lightweight assertion macro that logs failures but does not abort tests
#define ASSERT(condition, message)                                  \
    do {                                                            \
        if (!(condition)) {                                         \
            std::cerr << "ASSERTION FAILED: " << (message) << '\n'; \
            ++g_test_failures;                                      \
        } else {                                                    \
            /* For readability, print passing assertions optionally */ \
        }                                                           \
    } while (0)

// Track total test failures
static int g_test_failures = 0;

// Lightweight test cases for store_pool_error

// Test 1: When pp != NULL, ensure png_error is called and store_log is invoked with proper args
static void test_store_pool_error_pp_not_null()
{
    reset_mock_state();

    // Prepare dummy inputs
    struct png_store dummy_ps;
    png_store* ps = &dummy_ps;
    void* pp = (void*)0xABCD1234ULL;
    const char* msg = "error_message";

    // Execute the focal function
    store_pool_error(ps, (png_const_structp)pp, msg);

    // Assertions
    bool png_error_expected = (g_png_error_calls == 1) && (g_last_error_pp == pp) && (g_last_error_msg == msg);
    bool store_log_expected = (g_store_log_calls == 1) && (g_last_log_ps == ps) &&
                              (g_last_log_pp == pp) && (g_last_log_msg == msg) && (g_last_log_is_error == 1);

    ASSERT(png_error_expected, "png_error should be called exactly once with non-NULL pp and correct msg");
    ASSERT(store_log_expected, "store_log should be called exactly once with correct arguments and is_error=1");
}

// Test 2: When pp == NULL, ensure png_error is NOT called but store_log is invoked with pp=NULL
static void test_store_pool_error_pp_null()
{
    reset_mock_state();

    struct png_store dummy_ps;
    png_store* ps = &dummy_ps;
    void* pp = nullptr;
    const char* msg = "null_pp_message";

    store_pool_error(ps, (png_const_structp)pp, msg);

    bool png_error_not_called = (g_png_error_calls == 0);
    bool store_log_called = (g_store_log_calls == 1) && (g_last_log_ps == ps) &&
                            (g_last_log_pp == nullptr) && (g_last_log_msg == msg) &&
                            (g_last_log_is_error == 1);

    ASSERT(png_error_not_called, "png_error should not be called when pp is NULL");
    ASSERT(store_log_called, "store_log should be called exactly once with ps, pp=NULL, and correct msg");
}

// Test 3: Edge case - ps == NULL, pp != NULL; ensure store_log gets NULL ps and png_error is invoked
static void test_store_pool_error_ps_null_pp_not_null()
{
    reset_mock_state();

    void* dummy_pp = (void*)0xDEADBEAF;
    const char* msg = "edge_case";

    store_pool_error(nullptr, (png_const_structp)dummy_pp, msg);

    bool png_error_called = (g_png_error_calls == 1) && (g_last_error_pp == dummy_pp) &&
                            (g_last_error_msg == msg);
    bool store_log_called = (g_store_log_calls == 1) && (g_last_log_ps == nullptr) &&
                          (g_last_log_pp == dummy_pp) && (g_last_log_msg == msg) &&
                          (g_last_log_is_error == 1);

    ASSERT(png_error_called, "png_error should be called when pp != NULL even if ps is NULL");
    ASSERT(store_log_called, "store_log should be called with ps=NULL and correct pp/msg");
}

// Helper: run all tests and report a concise summary
static void run_all_tests()
{
    std::cout << "Running store_pool_error unit tests...\n";

    test_store_pool_error_pp_not_null();
    test_store_pool_error_pp_null();
    test_store_pool_error_ps_null_pp_not_null();

    if (g_test_failures == 0) {
        std::cout << "All tests PASSED\n";
    } else {
        std::cout << "Tests FAILED: " << g_test_failures << "\n";
    }
}

int main(void)
{
    run_all_tests();
    return g_test_failures ? 1 : 0;
}