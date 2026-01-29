// This test suite targets the focal function slength(struct slist *s) from optimize.c.
// The function counts the number of nodes in a singly linked list where s->code != NOP.
//
// Focused Keywords (Step 1):
// - slist: linked-list node type
// - next: pointer to the next node
// - s.code: operation code stored in each node
// - NOP: marker for no-operation
// - counting non-NOP nodes
//
// Step 2: Generate unit tests for slength with a minimal C-compatible test harness in C++11.
// The tests are designed to cover:
// - Empty list
// - All NOP codes
// - Mixed codes (some NOP, some non-NOP)
// - Large list with mixed codes to exercise multiple iterations
//
// Step 3: Test Case Refinement
// - Use a lightweight test harness that continues after failures (non-terminating assertions).
// - Validate both edge cases (empty) and typical cases (varied codes).
// - Access static-like behavior via the function under test only; avoid private members.
// - Use plain C-compatible struct layout that matches the expected slist layout in optimize.c.

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <vector>
#include <memory.h>
#include <optimize.h>
#include <gencode.h>
#include <os-proto.h>
#include <stdio.h>
#include <limits.h>
#include <functional>
#include <stdlib.h>
#include <setjmp.h>
#include <config.h>
#include <pcap-int.h>
#include <string>
#include <iostream>
#include <pcap-types.h>


// Ensure compatibility for the C function prototype.
// We assume the C definition uses u_int / unsigned int and a struct slist with a nested s.code field.
// The exact layout is replicated here to create test nodes that are layout-compatible.
#ifndef NOP
#define NOP 0
#endif

// Forward declaration of the focal C function with C linkage.
extern "C" unsigned int slength(struct slist *s);

// Matching C layout for slist to construct test inputs.
// The structure must be compatible with the one used by optimize.c.
struct slist {
    slist* next;
    struct {
        int code;
    } s;
};

// Lightweight test harness (non-terminating assertions).
namespace tests {
    static int failures = 0;

    static void fail(const std::string &msg) {
        std::cerr << "FAIL: " << msg << std::endl;
        ++failures;
    }

    static void pass(const std::string &msg) {
        std::cout << "PASS: " << msg << std::endl;
    }

    // Convenience assertion helpers
    template <typename T, typename U>
    static void assert_eq(const std::string &name, const T &a, const U &b) {
        if (a == b) {
            pass(name);
        } else {
            fail(name + " | expected: " + std::to_string(static_cast<long long>(b)) +
                 " actual: " + std::to_string(static_cast<long long>(a)));
        }
    }

    // Test 1: Empty list should yield 0
    static void test_slength_empty_list() {
        struct slist *head = nullptr;
        unsigned int res = slength(head);
        assert_eq("slength(nullptr) == 0", res, 0u);
    }

    // Test 2: All nodes with NOP codes should yield 0
    static void test_slength_all_nop() {
        // Build a small list: NOP -> NOP -> NOP
        slist *n3 = new slist;
        n3->next = nullptr;
        n3->s.code = NOP;

        slist *n2 = new slist;
        n2->next = n3;
        n2->s.code = NOP;

        slist *n1 = new slist;
        n1->next = n2;
        n1->s.code = NOP;

        unsigned int res = slength(n1);
        assert_eq("slength(all NOP) == 0", res, 0u);

        // Cleanup
        delete n1;
        delete n2;
        delete n3;
    }

    // Test 3: Mixed codes: count only non-NOP entries
    static void test_slength_mixed_codes() {
        // Build a list: [NOP, nonNOP(1), NOP, nonNOP(7)]
        slist *n4 = new slist;
        n4->next = nullptr;
        n4->s.code = 7;

        slist *n3 = new slist;
        n3->next = n4;
        n3->s.code = NOP;

        slist *n2 = new slist;
        n2->next = n3;
        n2->s.code = 1;

        slist *n1 = new slist;
        n1->next = n2;
        n1->s.code = NOP;

        unsigned int res = slength(n1);
        // Non-NOP entries: n2 (code=1) and n4 (code=7) -> 2
        assert_eq("slength(mixed codes) == 2", res, 2u);

        // Cleanup
        delete n1;
        delete n2;
        delete n3;
        delete n4;
    }

    // Test 4: Longer list with several non-NOPs
    static void test_slength_long_chain() {
        // Build a list with codes: [NOP, 2, 0, 3, NOP, 4]
        slist *node6 = new slist; node6->next = nullptr; node6->s.code = 4;
        slist *node5 = new slist; node5->next = node6; node5->s.code = NOP;
        slist *node4 = new slist; node4->next = node5; node4->s.code = 3;
        slist *node3 = new slist; node3->next = node4; node3->s.code = 0;       // NOP
        slist *node2 = new slist; node2->next = node3; node2->s.code = 2;
        slist *node1 = new slist; node1->next = node2; node1->s.code = NOP;

        unsigned int res = slength(node1);
        // Non-NOPs: node2 (2), node4 (3), node6 (4) -> 3
        assert_eq("slength(long chain) == 3", res, 3u);

        // Cleanup
        delete node1;
        delete node2;
        delete node3;
        delete node4;
        delete node5;
        delete node6;
    }

    // Run all tests in a controlled sequence
    int run_all() {
        test_slength_empty_list();
        test_slength_all_nop();
        test_slength_mixed_codes();
        test_slength_long_chain();
        return failures;
    }
}

// Entry point to run tests
int main() {
    // Informational banner
    std::cout << "Running slength unit tests (C++ harness)..." << std::endl;
    int failures = tests::run_all();
    if (failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << failures << " test(s) failed." << std::endl;
        return 1;
    }
}