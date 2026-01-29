// C++11 unit tests for the focal method: sqlite3VListNameToNum
// This test-suite avoids GoogleTest and uses a lightweight, non-terminating assertion style.

#include <math.h>
#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>


// The focal function is declared with C linkage.
// We assume VList is just an alias for int (as used by sqlite3VListNameToNum).
typedef int VList;
extern "C" int sqlite3VListNameToNum(VList *pIn, const char *zName, int nName);

static int g_tests_run = 0;
static int g_failures = 0;

// Lightweight, non-terminating assertion macro.
// Increments test count and reports PASS/FAIL without aborting the test run.
#define EXPECT_EQ(expected, actual, msg) do { \
    ++g_tests_run; \
    if ((expected) == (actual)) { \
        std::cout << "[OK] " << msg << std::endl; \
    } else { \
        std::cerr << "[FAIL] " << msg << " (expected " << (expected) \
                  << ", got " << (actual) << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)


// Test 1: Null pointer should yield 0 (as per early-return guard).
// This covers the pIn==0 branch of sqlite3VListNameToNum.
void test_null_pointer_returns_zero() {
    // Domain knowledge: passing a null pointer must not crash and must return 0.
    int res = sqlite3VListNameToNum(nullptr, "anything", 3);
    EXPECT_EQ(0, res, "Null pIn must return 0");
}


// Test 2: Found entry with exact name match should return its associated value.
// We construct a minimal VList layout:
// - pIn[1] (mx) = 6
// - pIn[2] = 42 (return value if found)
// - pIn[3] = 4  (step: i += pIn[i+1], so i becomes 6; single-entry case)
// - Memory at pIn[4] stores the string "test" followed by a null terminator.
//   The string starts at &pIn[i+2] => &pIn[4].
// - We pass zName = "test", nName = 4.
// The function should return 42.
void test_single_entry_found() {
    std::vector<int> p(9, 0); // ensure enough space for indices used below
    // Layout according to the function's expectations
    p[0] = 0;
    p[1] = 6;   // mx
    p[2] = 42;  // value to return if match
    p[3] = 4;   // i += pIn[i+1] -> i becomes 6 (loop ends since 6 < mx is false)

    // Place the string "test" starting at p[4]
    char *mem = reinterpret_cast<char*>(&p[4]);
    mem[0] = 't';
    mem[1] = 'e';
    mem[2] = 's';
    mem[3] = 't';
    mem[4] = '\0';

    int res = sqlite3VListNameToNum(p.data(), "test", 4);
    EXPECT_EQ(42, res, "Found entry should return its associated number when name matches exactly");
}


// Test 3: Not-found due to a name mismatch (string stored is "tesx" while zName is "test").
// Expect 0 since the name does not match exactly (including the null terminator position).
void test_single_entry_not_found_due_to_mismatch() {
    std::vector<int> p(9, 0);
    p[0] = 0;
    p[1] = 6;   // mx
    p[2] = 42;  // value (unused since name mismatches)
    p[3] = 4;   // step to next entry (not reached for match)

    // Place the string "tesx" starting at p[4], with a null terminator at position 5
    char *mem = reinterpret_cast<char*>(&p[4]);
    mem[0] = 't';
    mem[1] = 'e';
    mem[2] = 's';
    mem[3] = 'x';
    mem[4] = '\0';

    int res = sqlite3VListNameToNum(p.data(), "test", 4);
    EXPECT_EQ(0, res, "Name mismatch should result in 0 (not found)");
}


// Entry point: run all tests and report a summary.
// Note: This test suite uses only standard C++11 without any external testing framework.
int main() {
    test_null_pointer_returns_zero();
    test_single_entry_found();
    test_single_entry_not_found_due_to_mismatch();

    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_failures << std::endl;
    // Return non-zero if any test failed to aid integration with build systems.
    return (g_failures == 0) ? 0 : 1;
}