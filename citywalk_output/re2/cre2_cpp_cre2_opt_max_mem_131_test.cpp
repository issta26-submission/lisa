/*
Test suite for cre2_opt_max_mem (cre2_options_t)

Step 1 (Program Understanding)
- focal method: cre2_opt_max_mem (cre2_options_t *opt)
- Implementation (from provided snippet):
  cre2_opt_max_mem (cre2_options_t *opt)
  {
  {
    return TO_OPT(opt)->max_mem();
  }

- Key dependencies:
  - cre2_options_t: opaque options structure
  - cre2_opt_new(): create a new options object
  - cre2_opt_delete(cre2_options_t *): destroy options object
  - cre2_opt_set_max_mem (cre2_options_t *opt, int64_t m): set max_mem
  - cre2_opt_max_mem (cre2_options_t *opt): get max_mem via internal representation

- The test should verify that cre2_opt_max_mem returns the underlying max_mem value and that changes via cre2_opt_set_max_mem are reflected. It should exercise a default value path (before modification) and changes to a large value to ensure 64-bit handling.

Step 2 (Unit Test Generation)
- Candidate Keywords extracted: cre2_options_t, cre2_opt_new, cre2_opt_delete, cre2_opt_set_max_mem, cre2_opt_max_mem, max_mem, int64_t
- Tests will cover:
  - Default value retrieval, then setting to 1024 and verifying
  - Setting to 0 and verifying
  - Setting to a large 64-bit value and verifying

Step 3 (Test Case Refinement)
- Use a lightweight C++11 test harness (no Google Test) that runs two tests from main().
- Use non-terminating test style: tests return a bool; main prints PASS/FAIL per test.
- Access static/global API strictly through cre2.h public API.

Code (unit tests)

*/

#include <functional>
#include <cre2.h>
#include <re2/re2.h>
#include <cstdlib>
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Include the C API for CRE2 options

/*
Test 1: Cre2_opt_max_mem should reflect the value set by cre2_opt_set_max_mem.
- Steps:
  - Create a new options object with cre2_opt_new.
  - Read default max_mem (should be a valid int64_t, value not assumed).
  - Set max_mem to 1024 and verify cre2_opt_max_mem returns 1024.
  - Set max_mem to 0 and verify cre2_opt_max_mem returns 0.
  - Clean up with cre2_opt_delete.
*/
static bool test_cre2_opt_max_mem_basic() {
    cre2_options_t *opt = cre2_opt_new();
    if (opt == nullptr) {
        std::cerr << "test_cre2_opt_max_mem_basic: failed to create options object.\n";
        return false;
    }

    int64_t default_mem = cre2_opt_max_mem(opt);

    // Set to 1024 and verify
    cre2_opt_set_max_mem(opt, (int64_t)1024);
    int64_t v1 = cre2_opt_max_mem(opt);

    // Set to 0 and verify
    cre2_opt_set_max_mem(opt, (int64_t)0);
    int64_t v2 = cre2_opt_max_mem(opt);

    cre2_opt_delete(opt);

    bool ok = (v1 == (int64_t)1024) && (v2 == (int64_t)0);

    if (!ok) {
        std::cerr << "test_cre2_opt_max_mem_basic: "
                  << "default_mem=" << default_mem
                  << ", v1 (after 1024) = " << v1
                  << ", v2 (after 0) = " << v2 << "\n";
    }

    return ok;
}

/*
Test 2: Cre2_opt_max_mem should correctly handle large 64-bit values.
- Steps:
  - Create new options object.
  - Compute a large 64-bit value (1 << 40).
  - Set max_mem to that large value and verify retrieval equals the value.
  - Clean up.
*/
static bool test_cre2_opt_max_mem_large() {
    cre2_options_t *opt = cre2_opt_new();
    if (opt == nullptr) {
        std::cerr << "test_cre2_opt_max_mem_large: failed to create options object.\n";
        return false;
    }

    int64_t big = ((int64_t)1) << 40; // Large 64-bit value (1 TB in bytes, if interpreted as memory)
    cre2_opt_set_max_mem(opt, big);
    int64_t v = cre2_opt_max_mem(opt);

    cre2_opt_delete(opt);

    bool ok = (v == big);
    if (!ok) {
        std::cerr << "test_cre2_opt_max_mem_large: expected " << big << ", got " << v << "\n";
    }
    return ok;
}

/*
Main test harness
- Executes all tests and reports PASS/FAIL for each.
- Returns non-zero if any test fails.
*/
int main() {
    // Register tests
    struct TestCase {
        const char* name;
        std::function<bool()> fn;
        const char* description;
    };

    std::vector<TestCase> tests;
    tests.push_back({"test_cre2_opt_max_mem_basic",
                     test_cre2_opt_max_mem_basic,
                     "Verify default behavior not failing and updates reflect via setters."});
    tests.push_back({"test_cre2_opt_max_mem_large",
                     test_cre2_opt_max_mem_large,
                     "Verify large 64-bit values are supported by max_mem."});

    int pass = 0;
    for (const auto& t : tests) {
        bool ok = t.fn();
        std::cout << "[" << (ok ? "PASS" : "FAIL") << "] "
                  << t.name << ": " << t.description << std::endl;
        if (ok) ++pass;
    }

    std::cout << pass << "/" << tests.size() << " tests passed." << std::endl;
    return (pass == (int)tests.size()) ? 0 : 1;
}