// Unit test suite for the focal method this_op in optimize.c
// This test is written in C++11 (no GoogleTest) and links against the C implementation.
// It intentionally avoids private members access and uses a lightweight, standalone test harness.

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <memory.h>
#include <optimize.h>
#include <gencode.h>
#include <os-proto.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <setjmp.h>
#include <config.h>
#include <pcap-int.h>
#include <iostream>
#include <pcap-types.h>


// Forward-declare the C function and the relevant struct layout from the focal file.
// We model the minimal needed layout for compatibility with the actual implementation:
//
// In the focal code: this_op(struct slist *s) iterates while (s != 0 && s->s.code == NOP) s = s->next; return s;
//
// To be compatible with the C source, we define struct slist with a member 's' that
// itself has a field 'code', and a pointer 'next' to the next slist node.
extern "C" {

struct slist {
    struct { int code; } s;
    struct slist *next;
};

// The focal function under test (assumed to be defined in optimize.c).
struct slist *this_op(struct slist *s);

} // extern "C"

int main() {
    // Helper: simple verbose printer for PASS/FAIL
    auto report = [](const std::string& test_name, bool passed) {
        if (passed) {
            std::cout << "PASS: " << test_name << "\n";
        } else {
            std::cerr << "FAIL: " << test_name << "\n";
        }
    };

    // Candidate Keywords (Step 1): NOP, slist, next, code
    // We assume NOP corresponds to 0 (common convention for No-Op opcodes).
    // This aligns with the focal predicate: while (s != 0 && s->s.code == NOP)

    // Test 1: NULL input should return NULL
    // The implementation should handle a null pointer gracefully and return 0.
    {
        struct slist *res = this_op(nullptr);
        bool ok = (res == nullptr);
        report("this_op(NULL) returns NULL", ok);
    }

    // Test 2: Single node with non-NOP code should be returned as-is
    // Given a single node where code != NOP (assumed non-zero), the while condition
    // should fail at the first check and return the original pointer.
    {
        struct slist *node = new slist;
        node->s.code = 5; // non-NOP
        node->next = nullptr;

        struct slist *res = this_op(node);
        bool ok = (res == node);
        report("this_op(single non-NOP node) returns same node", ok);

        delete node;
    }

    // Test 3: Chain of NOPs followed by a non-NOP
    // s0.code == 0 (NOP), s1.code == 0 (NOP), s2.code == 1 (non-NOP)
    // Expect the function to skip the first two and return s2.
    {
        // Allocate three nodes
        struct slist *s0 = new slist;
        struct slist *s1 = new slist;
        struct slist *s2 = new slist;

        // Configure codes
        s0->s.code = 0; // NOP
        s1->s.code = 0; // NOP
        s2->s.code = 1; // non-NOP

        // Link chain: s0 -> s1 -> s2
        s0->next = s1;
        s1->next = s2;
        s2->next = nullptr;

        struct slist *res = this_op(s0);
        bool ok = (res == s2);
        report("this_op(NOP -> NOP -> non-NOP) returns the first non-NOP node (s2)", ok);

        // Cleanup
        delete s0;
        delete s1;
        delete s2;
    }

    // Test 4: Chain of all NOPs -> should return NULL (no non-NOP found)
    // s0.code == 0, s1.code == 0, s2.code == 0
    {
        struct slist *a = new slist;
        struct slist *b = new slist;
        struct slist *c = new slist;

        a->s.code = 0;
        b->s.code = 0;
        c->s.code = 0;

        a->next = b;
        b->next = c;
        c->next = nullptr;

        struct slist *res = this_op(a);
        bool ok = (res == nullptr);
        report("this_op(all NOPs) returns NULL", ok);

        delete a;
        delete b;
        delete c;
    }

    // Finalize test results
    std::cout << "All tests finished.\n";
    return 0;
}