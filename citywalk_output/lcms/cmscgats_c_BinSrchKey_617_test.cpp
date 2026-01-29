/*
Unit test suite for the focal method BinSrchKey (located in cmscgats.c)

Step 1 - Program Understanding (brief notes in comments)
- BinSrchKey performs a binary search over an array TabKeys of KEYWORDs to find an entry whose
  id matches the given id (case-insensitive via cmsstrcasecmp). If found, it returns the
  corresponding sy value; otherwise it returns SUNDEFINED.
- Key dependent components (candidate keywords): id (const char*), TabKeys[] (array of KEYWORD),
  sy (SYMBOL), cmsstrcasecmp comparison function, SUNDEFINED sentinel.

Step 2 - Test Generation
- We test typical and edge conditions:
  1) id matches first element -> returns first sy.
  2) id matches last element -> returns last sy.
  3) id not present -> returns SUNDEFINED.
  4) NumKeys = 0 -> returns SUNDEFINED.
  5) Case-insensitive match (e.g., "AAA" vs "aaa") -> returns corresponding sy.
- Tests rely on project headers/types (KEYWORD, SYMBOL, SUNDEFINED) and the signature:
  SYMBOL BinSrchKey(const char *id, int NumKeys, const KEYWORD* TabKeys)

Step 3 - Test Refinement
- Non-terminating assertions implemented as pass/fail messages to maximize execution.
- Test methods are invoked from main (as GTest/mocking frameworks are not allowed per domain rules).

Note: This test file expects the project headers to provide the actual KEYWORD, SYMBOL, SUNDEFINED types
and the BinSrchKey function with C linkage. Compile with the project sources/linker as usual.

*/
#include <cmscgats.h>
#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Include the project's header that declares KEYWORD, SYMBOL, SUNDEFINED and BinSrchKey.
// The exact header name may vary across environments; adjust if necessary.

#ifdef __cplusplus
extern "C" {
#endif
// Ensure the BinSrchKey declaration is visible with C linkage.
// If cmscgats.h already provides this, this is a harmless re-declaration.
SYMBOL BinSrchKey(const char *id, int NumKeys, const KEYWORD* TabKeys);
#ifdef __cplusplus
}
#endif

// Simple test harness
static int gTotalTests = 0;
static int gFailedTests = 0;

static void report_pass(const char* testName) {
    std::cout << "[PASS] " << testName << std::endl;
}

static void report_fail(const char* testName, const char* detail) {
    std::cout << "[FAIL] " << testName;
    if (detail && strlen(detail) > 0) {
        std::cout << " - " << detail;
    }
    std::cout << std::endl;
    ++gFailedTests;
}

// Test 1: id matches the first element -> returns keys[0].sy
static void test_BinSrchKey_FindsFirst() {
    const KEYWORD keys[] = {
        { "aaa", (SYMBOL)1 },
        { "bbb", (SYMBOL)2 },
        { "ccc", (SYMBOL)3 }
    };
    const int NumKeys = 3;

    SYMBOL res = BinSrchKey("aaa", NumKeys, keys);
    const SYMBOL expected = keys[0].sy;

    ++gTotalTests;
    if (res == expected) {
        report_pass("Finds first element (id == first key)");
    } else {
        report_fail("Finds first element (id == first key)", 
                    std::string("expected ").append(std::to_string((long long)expected))
                    .append(", got ").append(std::to_string((long long)res)).c_str());
    }
}

// Test 2: id matches the last element -> returns keys[2].sy
static void test_BinSrchKey_FindsLast() {
    const KEYWORD keys[] = {
        { "aaa", (SYMBOL)10 },
        { "bbb", (SYMBOL)20 },
        { "ccc", (SYMBOL)30 }
    };
    const int NumKeys = 3;

    SYMBOL res = BinSrchKey("ccc", NumKeys, keys);
    const SYMBOL expected = keys[2].sy;

    ++gTotalTests;
    if (res == expected) {
        report_pass("Finds last element (id == last key)");
    } else {
        report_fail("Finds last element (id == last key)", 
                    std::string("expected ").append(std::to_string((long long)expected))
                    .append(", got ").append(std::to_string((long long)res)).c_str());
    }
}

// Test 3: id not present -> SUNDEFINED
static void test_BinSrchKey_NotFound() {
    const KEYWORD keys[] = {
        { "alpha", (SYMBOL)1 },
        { "beta",  (SYMBOL)2 },
        { "gamma", (SYMBOL)3 }
    };
    const int NumKeys = 3;

    SYMBOL res = BinSrchKey("delta", NumKeys, keys);

    ++gTotalTests;
    if (res == SUNDEFINED) {
        report_pass("Not found returns SUNDEFINED");
    } else {
        report_fail("Not found returns SUNDEFINED", 
                    std::string("expected SUNDEFINED, got ").append(std::to_string((long long)res)).c_str());
    }
}

// Test 4: NumKeys = 0 -> SUNDEFINED
static void test_BinSrchKey_ZeroKeys() {
    const KEYWORD* keys = nullptr;
    const int NumKeys = 0;

    SYMBOL res = BinSrchKey("anything", NumKeys, keys);

    ++gTotalTests;
    if (res == SUNDEFINED) {
        report_pass("Zero keys returns SUNDEFINED");
    } else {
        report_fail("Zero keys returns SUNDEFINED", 
                    std::string("expected SUNDEFINED, got ").append(std::to_string((long long)res)).c_str());
    }
}

// Test 5: Case-insensitive match (e.g., "AAA" matches "aaa")
static void test_BinSrchKey_CaseInsensitive() {
    const KEYWORD keys[] = {
        { "aaa", (SYMBOL)7 },
        { "bbb", (SYMBOL)8 },
        { "ccc", (SYMBOL)9 }
    };
    const int NumKeys = 3;

    SYMBOL res = BinSrchKey("AAA", NumKeys, keys);
    const SYMBOL expected = keys[0].sy;

    ++gTotalTests;
    if (res == expected) {
        report_pass("Case-insensitive match (AAA vs aaa)");
    } else {
        report_fail("Case-insensitive match", 
                    std::string("expected ").append(std::to_string((long long)expected))
                    .append(", got ").append(std::to_string((long long)res)).c_str());
    }
}

int main() {
    // Run all defined tests
    test_BinSrchKey_FindsFirst();
    test_BinSrchKey_FindsLast();
    test_BinSrchKey_NotFound();
    test_BinSrchKey_ZeroKeys();
    test_BinSrchKey_CaseInsensitive();

    // Summary
    std::cout << "Total tests: " << gTotalTests << ", Failures: " << gFailedTests << std::endl;
    return gFailedTests;
}