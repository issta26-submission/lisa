/*
  Unit test suite for the focal method:
  lsm_config_log in the lsm_main.c/C API.

  Step 1 (Program Understanding): The function simply stores the provided
  log callback and context into the lsm_db object.

  Step 2 (Unit Test Generation): Tests cover basic assignment behavior,
  NULL handling, and re-assignment to a different callback.

  Step 3 (Test Case Refinement): We exercise both non-NULL and NULL cases,
  ensure that the pointers are stored correctly, and verify that the stored
  callback can be invoked with the stored context.

  Notes:
  - This test suite is written in C++11, without GoogleTest, using a small
    in-file test harness with non-terminating assertions (tests continue on
    failure to maximize code coverage).
  - It depends on the C header/type definitions from lsmInt.h and the C
    implementation providing lsm_config_log. The function is declared with extern "C".
  - We only rely on standard C++ library facilities.
*/

#include <lsmInt.h>
#include <iostream>
#include <cstring>
#include <cstdint>


// Include the C header that defines lsm_db and related types.
extern "C" {
}

// Forward declare the focal function with C linkage
extern "C" void lsm_config_log(lsm_db *pDb,
                               void (*xLog)(void *, int, const char *),
                               void *pCtx);

// Simple non-terminating test framework
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Assertions that do not stop test execution
#define EXPECT_TRUE(cond) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << " - Expected: " #cond << std::endl; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a, b) do { \
    ++g_total_tests; \
    if((a) != (b)) { \
        ++g_failed_tests; \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << " - Pointer mismatch: " #a " (0x" << std::hex \
                  << reinterpret_cast<void*>(a) << std::dec << ") != " \
                  << #b " (0x" << std::hex << reinterpret_cast<void*>(b) \
                  << std::dec << ")" << std::endl; \
    } \
} while(0)

#define EXPECT_EQ_PTR(a, b) EXPECT_PTR_EQ((a), (b))  // alias for readability

// Test logger implementations
static bool g_logger1_called = false;
static bool g_logger2_called = false;

static void test_logger1(void* ctx, int rc, const char* z) {
    (void)ctx; (void)rc; (void)z;
    g_logger1_called = true;
}

static void test_logger2(void* ctx, int rc, const char* z) {
    (void)ctx; (void)rc; (void)z;
    g_logger2_called = true;
}

// Test 1: Verify that lsm_config_log stores the provided function pointer and context
// - Non-null function pointer and non-null context
static void test_config_log_stores_values() {
    std::cout << "Test 1: Stores provided xLog and pCtx into pDb" << std::endl;

    lsm_db db;
    // Initialize fields to known values
    db.xLog = nullptr;
    db.pLogCtx = nullptr;

    g_logger1_called = false;

    // Call the function under test
    lsm_config_log(&db, test_logger1, (void*)0x1234);

    // Validate that the internal fields were updated
    EXPECT_PTR_EQ(db.xLog, test_logger1);
    EXPECT_PTR_EQ(db.pLogCtx, (void*)0x1234);

    // Invoke the stored callback to ensure it is callable with the stored context
    (db.xLog)(db.pLogCtx, 0, "test");
    EXPECT_TRUE(g_logger1_called);
}

// Test 2: NULL handling test
// - NULL xLog and NULL pCtx should be stored as such
static void test_config_log_stores_nulls() {
    std::cout << "Test 2: Stores NULL xLog and NULL pCtx when provided" << std::endl;

    lsm_db db;
    db.xLog = (void*)0xDEAD;  // non-null initial value to prove it gets overridden
    db.pLogCtx = (void*)0xBEEF;

    // Call with NULLs
    lsm_config_log(&db, nullptr, nullptr);

    // Verify both fields are NULL
    EXPECT_PTR_EQ(db.xLog, nullptr);
    EXPECT_PTR_EQ(db.pLogCtx, nullptr);

    // After this, invoking xLog would be invalid; we don't invoke to avoid UB.
}

// Test 3: Reassignment test
// - Reassign to a different non-null callback and non-null context
static void test_config_log_reassigns_callback() {
    std::cout << "Test 3: Reassigns to a new callback and context" << std::endl;

    lsm_db db;
    db.xLog = nullptr;
    db.pLogCtx = nullptr;

    g_logger2_called = false;

    // First assignment with logger2
    lsm_config_log(&db, test_logger2, (void*)0xABCD);

    // Validate
    EXPECT_PTR_EQ(db.xLog, test_logger2);
    EXPECT_PTR_EQ(db.pLogCtx, (void*)0xABCD);

    // Call the new callback via stored function pointer
    (db.xLog)(db.pLogCtx, 42, "payload");
    EXPECT_TRUE(g_logger2_called);
}

// Helper: Run all tests
static void run_all_tests() {
    test_config_log_stores_values();
    test_config_log_stores_nulls();
    test_config_log_reassigns_callback();
}

int main() {
    run_all_tests();

    std::cout << "Test summary: Total = " << g_total_tests
              << ", Passed = " << (g_total_tests - g_failed_tests)
              << ", Failed = " << g_failed_tests << std::endl;

    // Return non-zero if any tests failed
    return (g_failed_tests == 0) ? 0 : 1;
}