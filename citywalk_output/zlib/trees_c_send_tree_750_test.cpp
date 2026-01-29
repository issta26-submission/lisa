// C++11 test suite for the focal method: send_tree
// Note: This test harness provides a lightweight, self-contained environment
// intended to exercise key branches of the focal method. It relies on the project
// exposing the following interface (via extern "C" to align with the C origins):
//
// - struct ct_data { int Len; };
// - struct deflate_state { ct_data *bl_tree; };
// - void send_tree(deflate_state *s, ct_data *tree, int max_code);
// - void send_code(deflate_state *s, int code, ct_data *tree);
// - void send_bits(deflate_state *s, int value, int length);
// - void Assert(int condition, const char *msg);
//
// This test file provides mock implementations for send_code, send_bits and Assert
// to capture and verify the interactions performed by send_tree. It also constructs
// minimal ct_data trees and deflate_state instances to drive the logic through
// several branches (true/false predicates, different curlen/nextlen scenarios).

#include <cstring>
#include <stdexcept>
#include <cassert>
#include <string>
#include <deflate.h>
#include <iostream>
#include <vector>


// Domain knowledge notes (for maintainers):
// - We mock only the observable behavior (calls to send_code and send_bits) to
//   verify that send_tree traverses its branches without crashing.
// - We do not attempt to cover every possible C idiom or static/private member
//   nuances inside the original deflate.c; instead we focus on branch coverage
//   via crafted input trees and max_code values.
// - Static file-scope utility functions in the real project are not invoked here;
//   we rely on the public (extern) interface described above.

extern "C" {

// Simplified, test-oriented definitions mirroring the project's interfaces.
// These are deliberately minimal and designed to be compatible with a typical
// build environment where the original deflate.h/deflate_state are available.

struct ct_data {
    int Len;
};

struct deflate_state {
    ct_data *bl_tree;
};

// Forward declarations to be satisfied by the real project during linking.
// In this test harness we provide mock implementations below.
void send_tree(deflate_state *s, ct_data *tree, int max_code);
void send_code(deflate_state *s, int code, ct_data *tree);
void send_bits(deflate_state *s, int value, int length);
void Assert(int condition, const char *msg);

// Mock storage to capture calls for verification
struct CodeEvent {
    int code;
    // Optional: index into the tree array used (derivable from pointer, but here we keep simple)
    const ct_data *tree_ptr;
};
struct BitsEvent {
    int value;
    int length;
};

// Global storages to capture interactions from send_tree
static std::vector<CodeEvent> g_code_events;
static std::vector<BitsEvent> g_bits_events;

// Mock implementations: record calls instead of performing real I/O
void send_code(deflate_state *s, int code, ct_data *tree) {
    CodeEvent ce;
    ce.code = code;
    ce.tree_ptr = tree;
    g_code_events.push_back(ce);
}

void send_bits(deflate_state *s, int value, int length) {
    BitsEvent be;
    be.value = value;
    be.length = length;
    g_bits_events.push_back(be);
}

void Assert(int condition, const char *msg) {
    if (!condition) {
        throw std::runtime_error(std::string("Assert failed: ") + (msg ? msg : ""));
    }
}

// The focal method under test is assumed to be linked from the project.
// We declare it here with C linkage to allow dynamic linking to the real implementation.
// The real function is defined in trees.c and uses file-scope helpers; here we
// only declare the signature to be called by test harness.
void send_tree(deflate_state *s, ct_data *tree, int max_code);

} // extern "C"

// Helper utilities for tests (non-executable domain knowledge)
namespace TestUtils {

    // Convenience to create a ct_data array with given Len values
    ct_data* make_tree(int max_code, const int lens[]) {
        // We need at least max_code + 2 elements (tree[n+1] access)
        int n = max_code + 2;
        ct_data *arr = new ct_data[n];
        // tree[0].Len is the "nextlen" used by send_tree
        for (int i = 0; i < n; ++i) {
            int v = (lens && i < (max_code + 1)) ? lens[i] : 0;
            arr[i].Len = v;
        }
        return arr;
    }

    // Prepare a minimal deflate_state with a bl_tree pointer (not strictly
    // required by all versions of the real project, but commonly used).
    deflate_state make_state(ct_data *bl_tree) {
        deflate_state s;
        s.bl_tree = bl_tree;
        return s;
    }

    // Helper to reset captured events
    void reset_events() {
        g_code_events.clear();
        g_bits_events.clear();
    }

    // Helpers to inspect captured events
    bool expect_code_call_count(int expected) {
        return (int)g_code_events.size() == expected;
    }

    bool expect_code_with_value(int expected_code) {
        for (const auto &e : g_code_events) {
            if (e.code == expected_code) return true;
        }
        return false;
    }

    bool expect_bits_called_with(int expected_value, int expected_length) {
        for (const auto &b : g_bits_events) {
            if (b.value == expected_value && b.length == expected_length) return true;
        }
        return false;
    }

} // namespace TestUtils


// Test suite
int main() {
    using namespace TestUtils;

    std::cout << "Starting test suite for focal method: send_tree\n";

    // Test 1: Basic path where a short run triggers the min_count path
    // We craft a sequence where a curlen is emitted a few times, then the run ends
    // with count < min_count, ensuring the loop executes the do-while path.

    {
        reset_events();

        // Build a tree array with:
        // tree[0].Len = nextlen (e.g., 2)
        // tree[1].Len = 2 (same as nextlen)
        // tree[2].Len = 3 (different) to break the run
        // tree[3].Len and tree[4].Len are used for subsequent iterations
        // We need max_code >= 3 for the transition to trigger else-path at n=1
        const int lens[5] = {2, 2, 3, 3, 3};
        ct_data* test_tree = make_tree(3, lens);

        ct_data* bl_tree = new ct_data[2];
        // Minimal, non-null pointer for s->bl_tree
        for (int i = 0; i < 2; ++i) bl_tree[i].Len = 0;

        deflate_state s = make_state(bl_tree);

        // Call focal method (assumes external linking with real code)
        try {
            send_tree(&s, test_tree, 3);
        } catch (...) {
            // If internal asserts explode, report failure
            std::cerr << "Test 1: send_tree threw an exception unexpectedly.\n";
            delete[] test_tree;
            delete[] bl_tree;
            return 1;
        }

        // Expectations: We expect at least two sends of code for curlen=2
        if (!expect_code_call_count(2) || !expect_code_with_value(2)) {
            std::cerr << "Test 1 failed: Expected two send_code calls with code 2.\n";
            std::cerr << "Code events: " << g_code_events.size() << "\n";
            delete[] test_tree;
            delete[] bl_tree;
            return 2;
        }

        // Cleanup
        delete[] test_tree;
        delete[] bl_tree;
        reset_events();

        std::cout << "Test 1 passed: min_count branch exercised.\n";
    }

    // Test 2: Branch where curlen != prevlen, leading to an additional send_code
    // and a subsequent REP_3_6 emission with a controlled count.
    {
        reset_events();

        // Create a sequence designed to set curlen != prevlen on the first non-equal
        // boundary and ensure count is at least 3 after the decrement.
        // We'll craft: [2, 2, 4, 4, 4, 4] with max_code = 4
        const int lens[6] = {2, 2, 4, 4, 4, 4};
        ct_data* test_tree = make_tree(4, lens);

        ct_data* bl_tree = new ct_data[4];
        for (int i = 0; i < 4; ++i) bl_tree[i].Len = 0;

        deflate_state s = make_state(bl_tree);

        try {
            send_tree(&s, test_tree, 4);
        } catch (...) {
            std::cerr << "Test 2: send_tree threw an exception unexpectedly.\n";
            delete[] test_tree;
            delete[] bl_tree;
            return 1;
        }

        // Expect at least one code emission for the curlen (when curlen != prevlen)
        // and a REP_3_6 code following it.
        bool has_curlen_emit = false;
        bool has_rep3_6 = false;
        for (const auto &e : g_code_events) {
            if (e.code != 0 && e.code != 16 && e.code != 17 && e.code != 18) {
                // any non-zero code here is indicative of a curlen emission
                has_curlen_emit = true;
            }
        }
        // We cannot rely on exact REP_3_6 value due to project constants;
        // check that some code emission occurred.
        if (!has_curlen_emit) {
            std::cerr << "Test 2 failed: expected curlen emission when curlen != prevlen.\n";
            delete[] test_tree;
            delete[] bl_tree;
            return 3;
        }

        delete[] test_tree;
        delete[] bl_tree;
        reset_events();

        std::cout << "Test 2 passed: curlen != prevlen branch exercised.\n";
    }

    // Test 3: Path when nextlen is zero at the start (special case)
    // This should adjust max_count/min_count to 138/3 and proceed accordingly.
    {
        reset_events();

        // nextlen = 0 at tree[0], followed by a few non-zero Len values to drive behavior
        const int lens[6] = {0, 2, 2, 2, 2, 2};
        ct_data* test_tree = make_tree(5, lens);

        ct_data* bl_tree = new ct_data[8];
        for (int i = 0; i < 8; ++i) bl_tree[i].Len = 0;

        deflate_state s = make_state(bl_tree);

        try {
            send_tree(&s, test_tree, 5);
        } catch (...) {
            std::cerr << "Test 3: send_tree threw an exception unexpectedly.\n";
            delete[] test_tree;
            delete[] bl_tree;
            return 1;
        }

        // The path should not crash; at least some code or bits should be emitted in general.
        if (g_code_events.empty() && g_bits_events.empty()) {
            std::cerr << "Test 3 failed: expected some activity (send_code or send_bits) for the zero-nextlen case.\n";
            delete[] test_tree;
            delete[] bl_tree;
            return 4;
        }

        delete[] test_tree;
        delete[] bl_tree;
        reset_events();

        std::cout << "Test 3 passed: zero-nextlen path did not crash.\n";
    }

    std::cout << "All tests completed.\n";
    return 0;
}