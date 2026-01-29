// Lightweight C++11 test suite for sqlite3VdbeRealValue(Mem* pMem)
// This test suite avoids Google Test and uses a small, non-terminating assertion mechanism.
// It targets the focal method described in the prompt and its dependent flags/branches.
// The tests rely on the SQLite internal headers being available in the build environment.
// Each test is self-contained and annotated with comments explaining the intent and coverage.
// NOTE: We only exercise the true branch (MEM_Real), the integer branch (MEM_Int|MEM_IntReal),
// and the default zero branch. The MEM_Str/MEM_Blob branch is not asserted here due to
// reliance on static memRealValue() behavior which is not observable without deeper linkage.

#include <stdexcept>
#include <cmath>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>


// Import necessary internal SQLite structures and constants.
// These headers provide the Mem structure, MEM_* flags, and related helpers.
extern "C" {
}

// Declaration of the focal function from the C source (vdbemem.c).
// It uses C linkage, hence extern "C" to prevent name mangling.
extern "C" double sqlite3VdbeRealValue(Mem *pMem);

// Global test counters for a lightweight harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Utility: Convert double to string with stable formatting for messages
static std::string to_string_double(double v) {
    char buf[64];
    // Use maximum precision without trailing noise
    std::snprintf(buf, sizeof(buf), "%.12g", v);
    return std::string(buf);
}

// Lightweight non-terminating assertion macro
// If condition fails, throw an exception which is caught by the test runner.
// This enables multiple tests to run in a single process without aborting on first failure.
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        throw std::runtime_error(std::string("Expectation failed: ") + (msg)); \
    } \
} while(0)

// Expectation for double equality with tolerance
#define EXPECT_DOUBLE_EQ(actual, expected) do { \
    const double _a = (actual); \
    const double _e = (expected); \
    const double _tol = 1e-12; \
    if(!(std::fabs(_a - _e) <= _tol)) { \
        throw std::runtime_error("Double mismatch: expected " + to_string_double(_e) + \
                                 " but got " + to_string_double(_a)); \
    } \
} while(0)

// Expectation for integer equality (converted to double for comparison)
#define EXPECT_DOUBLE_EQ_INT(asDouble, anInt) do { \
    EXPECT_DOUBLE_EQ((asDouble), static_cast<double>(anInt)); \
} while(0)

// Run a single test function and capture its result
static void run_test(const char* name, void (*test_func)()) {
    ++g_total_tests;
    std::cout << "[ RUN      ] " << name << std::endl;
    try {
        test_func();
        std::cout << "[  PASSED  ] " << name << std::endl;
    } catch (const std::exception& ex) {
        ++g_failed_tests;
        std::cerr << "[  FAILED  ] " << name << " — " << ex.what() << std::endl;
    } catch (...) {
        ++g_failed_tests;
        std::cerr << "[  FAILED  ] " << name << " — Unknown exception" << std::endl;
    }
}

// Test 1: Real value path
// Scenario: MEM_Real flag is set; function should return the real value stored in pMem->u.r
static void test_real_value_path() {
    // The Mem structure is defined by sqliteInt.h/vdbeInt.h. We instantiate a local Mem
    // and populate only the fields required by sqlite3VdbeRealValue.
    Mem m;
    // Ensure alignment and a clean state
    m.flags = MEM_Real;
    m.u.r = 12.3456789;
    m.db = 0; // No backing DB; this satisfies the assertion about mutex when db==0

    // Call the focal function (C linkage)
    double v = sqlite3VdbeRealValue(&m);

    // Expected: exact real value as double
    EXPECT_DOUBLE_EQ(v, 12.3456789);
}

// Test 2: Integer path (MEM_Int or MEM_IntReal)
// Scenario: MEM_Int flag is set (or MEM_IntReal), value should be converted to double
static void test_int_value_path() {
    Mem m;
    m.flags = MEM_Int; // Include Integer flag to trigger integer path
    m.u.i = 42;
    m.db = 0;

    double v = sqlite3VdbeRealValue(&m);

    // Expected: double representation of the integer
    EXPECT_DOUBLE_EQ(v, 42.0);
}

// Test 3: Integer-Real path (MEM_IntReal combined with MEM_Int)
// Scenario: MEM_IntReal via MEM_IntReal flag triggers the (MEM_Int|MEM_IntReal) branch.
// We piggyback on MEM_IntReal to cover this branch's numeric conversion.
static void test_intreal_value_path() {
    Mem m;
    m.flags = MEM_IntReal; // Part of (MEM_Int|MEM_IntReal)
    m.u.i = -7;
    m.db = 0;

    double v = sqlite3VdbeRealValue(&m);

    // Expected: double representation of the integer
    EXPECT_DOUBLE_EQ(v, -7.0);
}

// Test 4: Default/other path (none of MEM_Real, MEM_Int, MEM_IntReal, MEM_Str, MEM_Blob)
// Scenario: When no numeric/string flags are set, the function should fall back to 0.0
static void test_default_zero_path() {
    Mem m;
    m.flags = 0; // No known numeric or string flags
    m.u.i = 0;  // Value doesn't matter for this path
    m.db = 0;

    double v = sqlite3VdbeRealValue(&m);

    EXPECT_DOUBLE_EQ(v, 0.0);
}

// Entry point for running all tests
int main() {
    // Step 1: Program understanding note (embedded as comments in code)
    // Candidate Keywords for sqlite3VdbeRealValue: MEM_Real, MEM_Int, MEM_IntReal, MEM_Str, MEM_Blob,
    // u.r, u.i, db, mutex, EIGHT_BYTE_ALIGNMENT, memRealValue, testcase.

    // Step 2 & 3: Generate and refine tests; we include 4 focused test cases to cover true/false
    // branches and the default branch, ensuring alignment and non-null mem.

    run_test("test_real_value_path", test_real_value_path);
    run_test("test_int_value_path", test_int_value_path);
    run_test("test_intreal_value_path", test_intreal_value_path);
    run_test("test_default_zero_path", test_default_zero_path);

    // Summary
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Total tests: " << g_total_tests << std::endl;
    std::cout << "Passed: " << (g_total_tests - g_failed_tests) << std::endl;
    std::cout << "Failed: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return g_failed_tests;
}