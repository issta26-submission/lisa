/*
  Test Suite for sqlite3_value_subtype (C++11)

  Notes:
  - The focal function is:
      unsigned int sqlite3_value_subtype(sqlite3_value *pVal){
        Mem *pMem = (Mem*)pVal;
        return ((pMem->flags & MEM_Subtype) ? pMem->eSubtype : 0);
      }
  - We rely on the sqlite internal headers to provide Mem and MEM_Subtype.
  - Tests are written in C++11, without GoogleTest. Non-terminating assertions collect failures and report at the end.
  - The tests cast a Mem instance to sqlite3_value* to exercise the function's casting and bit-check logic.
  - The test suite is designed to be compiled and linked with the sqlite source tree (vdbeInt.h / sqliteInt.h etc.).
*/

#include <vector>
#include <opcodes.h>
#include <sstream>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>


// Import internal SQLite structures and constants required by the focal function.
// This allows us to create a Mem object and set MEM_Subtype flag correctly.
extern "C" {
// If vdbeInt.h is not directly available, ensure the test environment provides equivalent definitions.
}

typedef struct sqlite3_value sqlite3_value;

// Forward declaration of the focal method (C linkage to avoid name mangling in C++)
extern "C" unsigned int sqlite3_value_subtype(sqlite3_value *pVal);

// Global container for non-terminating test failures (to maximize code coverage)
static std::vector<std::string> g_failures;

// Helper: record a failure with a readable message
static void record_failure(const std::string& msg) {
    g_failures.push_back(msg);
}

// Helper: generic equality expectation with non-terminating behavior
template <typename T, typename U>
static void expect_eq(const T& actual, const U& expected,
                      const char* actual_expr, const char* expected_expr,
                      const char* file, int line) {
    if (!(actual == expected)) {
        std::ostringstream oss;
        oss << "Expectation failed at " << file << ":" << line
            << " - " << actual_expr << " == " << actual
            << " but " << expected_expr << " == " << expected;
        record_failure(oss.str());
    }
}

// Macro to capture expression text for better failure messages
#define EXPECT_EQ(a, b) expect_eq((a), (b), #a, #b, __FILE__, __LINE__)

// Test 1: True branch when MEM_Subtype flag is set; ensure eSubtype is returned
static void test_subtype_flag_set_returns_eSubtype() {
    // Ensure we are using the library's MEM_Subtype flag
    Mem m;
    m.flags = MEM_Subtype;     // Set the MEM_Subtype flag (bit-mask)
    m.eSubtype = 0xABCDu;       // Arbitrary subtype value

    // Cast Mem to sqlite3_value* as expected by sqlite3_value_subtype
    sqlite3_value *pVal = (sqlite3_value *)&m;

    unsigned int result = sqlite3_value_subtype(pVal);

    // Validate: since MEM_Subtype is set, result should equal eSubtype
    EXPECT_EQ(result, m.eSubtype);
}

// Test 2: False branch when MEM_Subtype flag is not set; ensure 0 is returned
static void test_subtype_flag_not_set_returns_zero() {
    Mem m;
    m.flags = 0;              // MEM_Subtype bit not set
    m.eSubtype = 0x1234u;       // Value should be ignored when flag is not set

    sqlite3_value *pVal = (sqlite3_value *)&m;

    unsigned int result = sqlite3_value_subtype(pVal);

    // Validate: when flag is not set, result must be 0
    EXPECT_EQ(result, 0u);
}

// Test 3: Edge case: MEM_Subtype set but eSubtype is 0; result should be 0
static void test_subtype_flag_set_with_zero_subtype() {
    Mem m;
    m.flags = MEM_Subtype;
    m.eSubtype = 0u;

    sqlite3_value *pVal = (sqlite3_value *)&m;

    unsigned int result = sqlite3_value_subtype(pVal);

    // Validate: eSubtype is returned even if it's 0 (consistent with function logic)
    EXPECT_EQ(result, 0u);
}

// Run all tests and report aggregated results
static void run_all_tests() {
    test_subtype_flag_set_returns_eSubtype();
    test_subtype_flag_not_set_returns_zero();
    test_subtype_flag_set_with_zero_subtype();

    // Summary
    if (g_failures.empty()) {
        std::cout << "ALL TESTS PASSED: sqlite3_value_subtype" << std::endl;
    } else {
        std::cout << g_failures.size() << " TEST(S) FAILED:" << std::endl;
        for (size_t i = 0; i < g_failures.size(); ++i) {
            std::cout << "  [" << i + 1 << "] " << g_failures[i] << std::endl;
        }
        std::cout << "END OF TEST SUMMARY" << std::endl;
    }
}

// Entry point
int main() {
    // Run the test suite
    run_all_tests();
    return g_failures.empty() ? 0 : 1;
}