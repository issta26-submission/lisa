/*
  Test Suite: sqlite3_sourceid
  Language: C++11 (no GTest)
  Purpose: Verify the focal method sqlite3_sourceid() returns the expected textual
           SQLITE_SOURCE_ID and exhibits stable, non-null output across calls.

  Notes:
  - This test assumes the project under test provides a C function:
      const char *sqlite3_sourceid(void);
    whose implementation returns SQLITE_SOURCE_ID.
  - We link against the existing compilation unit(s) that define sqlite3_sourceid (e.g., main.c).
  - We rely on sqlite3.h for the SQLITE_SOURCE_ID macro to compare against the runtime result.
  - Tests are non-terminating; they report pass/fail for each test and return a non-zero exit code
    if any test fails.
*/

#include <functional>
#include <vector>
#include <sqlite3.h>
#include <string>
#include <sqliteInt.h>
#include <iostream>


// Include sqlite3 header to obtain SQLITE_SOURCE_ID macro and the prototype

// Ensure linkage to the C symbol sqlite3_sourceid
extern "C" const char *sqlite3_sourceid(void);

// Test 1: Ensure the function returns a non-null, non-empty string.
// This verifies basic contract of the focal method.
bool test_sqlite3_sourceid_non_null_and_nonempty() {
    const char *id = sqlite3_sourceid();
    if (id == nullptr) {
        std::cerr << "Failure: sqlite3_sourceid() returned null." << std::endl;
        return false;
    }
    if (id[0] == '\0') {
        std::cerr << "Failure: sqlite3_sourceid() returned an empty string." << std::endl;
        return false;
    }
    return true;
}

// Test 2: Ensure the returned string matches the SQLITE_SOURCE_ID macro exactly
// (content equality, not pointer equality).
// This validates that the focal method returns the intended source identifier.
bool test_sqlite3_sourceid_matches_macro() {
    const char *id = sqlite3_sourceid();
    if (id == nullptr) {
        std::cerr << "Failure: sqlite3_sourceid() returned null." << std::endl;
        return false;
    }
    // Compare content against the macro-expanded literal
    if (std::string(id) != std::string(SQLITE_SOURCE_ID)) {
        std::cerr << "Failure: sqlite3_sourceid() content mismatch. "
                  << "Expected: " << SQLITE_SOURCE_ID << ", "
                  << "Got: " << id << std::endl;
        return false;
    }
    return true;
}

// Test 3: Consistency test - multiple invocations should yield identical content.
// This ensures deterministic behavior across calls.
bool test_sqlite3_sourceid_consistency() {
    const char *id1 = sqlite3_sourceid();
    const char *id2 = sqlite3_sourceid();
    if (id1 == nullptr || id2 == nullptr) {
        std::cerr << "Failure: sqlite3_sourceid() returned null on one of the calls." << std::endl;
        return false;
    }
    if (std::string(id1) != std::string(id2)) {
        std::cerr << "Failure: sqlite3_sourceid() returned different strings on subsequent calls. "
                  << "First: " << id1 << ", Second: " << id2 << std::endl;
        return false;
    }
    // Also compare with macro to ensure long-term consistency with macro value
    if (std::string(id1) != std::string(SQLITE_SOURCE_ID)) {
        std::cerr << "Failure: sqlite3_sourceid() content mismatch with macro on consistency test. "
                  << "Expected: " << SQLITE_SOURCE_ID << ", Got: " << id1 << std::endl;
        return false;
    }
    return true;
}

int main() {
    // Assemble tests
    using TestFn = std::function<bool()>;
    struct TestCase {
        const char *name;
        TestFn fn;
    };

    std::vector<TestCase> tests = {
        { "sqlite3_sourceid_non_null_and_nonempty", test_sqlite3_sourceid_non_null_and_nonempty },
        { "sqlite3_sourceid_matches_macro",       test_sqlite3_sourceid_matches_macro },
        { "sqlite3_sourceid_consistency",          test_sqlite3_sourceid_consistency }
    };

    // Run tests and report results
    int passed = 0;
    for (const auto &t : tests) {
        bool result = false;
        try {
            result = t.fn();
        } catch (...) {
            std::cerr << "Exception occurred in test: " << t.name << std::endl;
            result = false;
        }
        if (result) {
            std::cout << "[OK] " << t.name << std::endl;
            ++passed;
        } else {
            std::cout << "[FAIL] " << t.name << std::endl;
        }
    }

    std::cout << passed << " of " << tests.size() << " tests passed." << std::endl;
    return (passed == static_cast<int>(tests.size())) ? 0 : 1;
}