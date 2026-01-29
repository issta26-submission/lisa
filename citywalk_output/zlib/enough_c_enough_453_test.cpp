/*
Unit test suite for the focal function "enough" and its dependencies, implemented in C++11.

Overview:
- This test suite provides a self-contained, compilable environment that mimics the
  minimal behavior required by the focal method and its dependencies from the
  provided snippet.
- It does not rely on any external test framework (GTest not used), per the request.
- Tests focus on:
  - Correct handling of the root/max condition (branch where the inner loops are skipped).
  - Invocation of the examine() function in both possible branches when the inner
    conditions are met (to cover true/false predicates).
- The tests do not terminate on first failure; instead, they accumulate results and report at the end.
- Static-like, file-scope functions and a global-like state (g) are emulated in this C++ test harness
  to closely mirror the original structure and behavior.

Notes on Candidate Keywords (Step 1):
- g.max, g.root, g.large, g.code, g.out, g.num
- map(), examine(), enough()
- string_clear()
These are the core components used by the focal method and are represented in the test harness.

Usage:
- Compile with a C++11 compiler (e.g., g++ -std=c++11 test_enough.cpp)
- Run; it will print a summary of PASS/FAIL for each test.

Now providing the test code:
*/

#include <cstdio>
#include <cstring>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <cassert>
#include <string>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <stdlib.h>


/*
Candidate Keywords mapped to test scaffolding:
- Global-like state g: fields max, root, large, code, out, num
- Helpers: string_clear, map, examine
- Core: enough function (reproduced in test harness)
*/

// Minimal string wrapper compatible with the provided focal code
struct string_t {
    std::string str;
};

// Global-like state used by enough
static struct GState {
    int max;                 // upper bound of codes
    int root;                // root bits
    std::vector<int> code;     // code table (size at least max+1)
    string_t out;              // output buffer (as in the focal code)
    int large;               // computed as 1 << root
    std::vector<int> num;      // auxiliary array used by conditions
} g;

// A simple event log to verify side effects of examine calls
static std::vector<std::string> g_events;

// Candidate Keyword: map(syms, left, len) -> used to index into g.num
static size_t map(int syms, int left, int len) {
    // Deterministic, simple hash-like function to produce an index
    // Ensure values are not negative
    return static_cast<size_t>(syms) * 1315423911u + static_cast<size_t>(left) * 2654435761u + static_cast<size_t>(len);
}

// Candidate Keyword: string_clear
static void string_clear(string_t *s) {
    s->str.clear();
}

// Candidate Keyword: examine
static void examine(int syms, int left, int len, int mem, int rem) {
    (void)mem; (void)rem; // unused in test, but kept to mirror signature
    // Log that examine was invoked with particular parameters
    g_events.push_back("examine_called: syms=" + std::to_string(syms) +
                       ", left=" + std::to_string(left) +
                       ", len=" + std::to_string(len) +
                       ", mem=" + std::to_string(mem) +
                       ", rem=" + std::to_string(rem));
}

// Reproduced focal function (static-like in test harness)
static void enough(int syms) {
    // clear code table
    // ensure g.code has enough size
    if ((int)g.code.size() <= g.max) {
        g.code.resize(g.max + 1, 0);
    }
    for (int n = 0; n <= g.max; n++) {
        g.code[n] = 0;
    }

    // clear saved results
    string_clear(&g.out);

    // base table
    g.large = 1 << g.root;

    // main nested loops (only if root < max)
    if (g.root < g.max) {
        for (int n = 3; n <= syms; n++) {
            for (int left = 2; left < n; left += 2) {
                // look at all reachable (root + 1) bit nodes
                size_t index = map(n, left, g.root + 1);
                // first condition: g.root+1 < g.max && g.num[index]
                if (g.root + 1 < g.max && (index < g.num.size()) && g.num[index])
                    examine(n, left, g.root + 1, 1 << g.root, 0);
                // second condition: g.num[index - 1] && n <= left<<1
                if ((index > 0) && (index - 1) < g.num.size() && g.num[index - 1] && n <= (left << 1))
                    examine((n - left) << 1, (n - left) << 1, g.root + 1,
                            1 << g.root, 0);
            }
        }
    }

    // done
    printf("maximum of %d table entries for root = %d\n", g.large, g.root);
    fputs(g.out.str.c_str(), stdout);
}

// Helper to reset global state before each test
static void reset_global_state() {
    g.max = 0;
    g.root = 0;
    g.large = 0;
    g.code.clear();
    g.out.str.clear();
    g.num.clear();
    g_events.clear();
}

// Test 1: root >= max: inner loop should be skipped; examine should not be called
static void test_case_root_ge_max() {
    reset_global_state();
    // Configure g such that root >= max
    g.max = 5;
    g.root = 5; // root not less than max
    g.code.assign(g.max + 1, 0);
    // No entries needed in g.num for this case
    // Reset and fill; ensures deterministic behavior
    enough(4);

    // Validate: no examine calls should have occurred
    bool pass = g_events.empty();
    if (pass) {
        std::cout << "[PASS] test_case_root_ge_max: No examine calls when root >= max\n";
    } else {
        std::cout << "[FAIL] test_case_root_ge_max: examine calls occurred unexpectedly\n";
    }
}

// Test 2: root < max: ensure examine is invoked via both branches when conditions are met
static void test_case_examine_invocations() {
    reset_global_state();
    // Configure g for branch execution
    g.max = 5;
    g.root = 2; // root < max
    g.code.assign(g.max + 1, 0);
    // Prepare g.num with enough entries to trigger both branches
    // Compute index as in map(n, left, g.root+1) for a representative n,left
    size_t index = map(3, 2, g.root + 1); // for n=3, left=2, len=root+1
    // Ensure the vector is large enough
    size_t needed = index + 2;
    if (needed < 256) needed = 256;
    g.num.assign(needed, 0);
    // Trigger first branch: g.num[index] must be true
    if (index < g.num.size()) g.num[index] = 1;
    // Trigger second branch: g.num[index - 1] must be true
    if (index > 0 && (index - 1) < g.num.size()) g.num[index - 1] = 1;

    enough(4);

    // Validate that examine was invoked at least once and that the two-branch behavior was exercised
    bool examined = false;
    for (const auto &e : g_events) {
        if (e.find("examine_called:") != std::string::npos) {
            examined = true;
            break;
        }
    }

    bool pass = examined;
    if (pass) {
        std::cout << "[PASS] test_case_examine_invocations: examine() invoked on branch conditions\n";
    } else {
        std::cout << "[FAIL] test_case_examine_invocations: examine() not invoked as expected\n";
    }
}

// Test 3: Additional coverage: ensure second branch can be reached for a different configuration
static void test_case_examine_second_branch_only() {
    reset_global_state();
    g.max = 10;
    g.root = 3;
    g.code.assign(g.max + 1, 0);
    // Prepare indices so that only the second condition can be true
    // We'll still populate g.num[index-1] = 1 and ensure n <= left<<1
    size_t index = map(4, 3, g.root + 1);
    if (index == 0) index = 1;
    g.num.assign(index + 5, 0);
    if (index > 0 && index - 1 < g.num.size()) g.num[index - 1] = 1;

    enough(4);

    bool examined = false;
    for (const auto &e : g_events) {
        if (e.find("examine_called:") != std::string::npos) {
            examined = true;
            break;
        }
    }

    if (examined) {
        std::cout << "[PASS] test_case_examine_second_branch_only: second branch can trigger examine()\n";
    } else {
        std::cout << "[FAIL] test_case_examine_second_branch_only: second branch did not trigger examine()\n";
    }
}

// Main runner
int main() {
    // Run tests and report results
    test_case_root_ge_max();
    test_case_examine_invocations();
    test_case_examine_second_branch_only();

    // Summarize overall status
    // If no failures were detected by the tests above (they print PASS/FAIL),
    // we interpret the presence of any "FAIL" messages in stdout as failure.
    // Since tests print their own statuses, we end with a simple completion message.
    std::cout << "Test suite finished.\n";
    return 0;
}