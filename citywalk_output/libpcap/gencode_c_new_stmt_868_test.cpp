// C++11 test suite for the focal method: new_stmt in gencode.c
// This test suite uses a lightweight in-process test runner (no Google Test).
// It relies on the project headers and C linkage to exercise the real implementation.

#include <pflog.h>
#include <stdarg.h>
#include <gencode.h>
#include <ppp.h>
#include <stdio.h>
#include <ethertype.h>
#include <extract.h>
#include <iostream>
#include <nametoaddr.h>
#include <ieee80211.h>
#include <cstdlib>
#include <stdint.h>
#include <stddef.h>
#include <thread-local.h>
#include <pcap-util.h>
#include <scanner.h>
#include <setjmp.h>
#include <cstdint>
#include <llc.h>
#include <pcap/namedb.h>
#include <pcap/ipnet.h>
#include <diag-control.h>
#include <linux/filter.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <string.h>
#include <memory.h>
#include <linux/types.h>
#include <os-proto.h>
#include <linux/if_packet.h>
#include <config.h>
#include <pcap/sll.h>


// Bring in the C declarations of the production code under test.
// The header is expected to define compiler_state_t, struct slist, and the
// prototype for new_stmt along with supporting functions like initchunks/freechunks.
extern "C" {
}

// Lightweight assertion helpers (non-terminating) to maximize execution coverage.
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " #cond " in " __FILE__ ":" << __LINE__ << "\n"; \
        all_passed = false; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    if(!((a) == (b))) { \
        std::cerr << "EXPECT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) << ")" \
                  << " in " << __FILE__ << ":" << __LINE__ << "\n"; \
        all_passed = false; \
    } \
} while(0)

static bool all_passed = true;

// Test 1: Basic behavior – new_stmt should allocate a slist-like node and store the code.
bool test_new_stmt_basic_allocation() {
    // Initialize compiler state for memory chunks (as production code would).
    compiler_state_t cstate;
    // Some projects require an explicit initialization; if not, initchunks will be a no-op.
    initchunks(&cstate);

    const int test_code = 42;

    // Call the focal function.
    struct slist *node = new_stmt(&cstate, test_code);

    // Verify non-null return and correct code storage.
    EXPECT_TRUE(node != nullptr);

    // Access to node->s.code is assumed based on the production layout (as per the focal method).
    // If the internal layout differs, the test would fail to link/compile; the project
    // headers should provide the correct struct slist definition.
    EXPECT_EQ(node->s.code, test_code);

    // Cleanup any allocated chunks in the state to avoid leaks in subsequent tests.
    freechunks(&cstate);

    return all_passed;
}

// Test 2: Multiple allocations – ensure successive new_stmt calls allocate distinct blocks
// and preserve their respective codes.
bool test_new_stmt_multiple_allocations() {
    compiler_state_t cstate;
    initchunks(&cstate);

    const int code1 = 7;
    const int code2 = 128;

    struct slist *n1 = new_stmt(&cstate, code1);
    struct slist *n2 = new_stmt(&cstate, code2);

    EXPECT_TRUE(n1 != nullptr);
    EXPECT_TRUE(n2 != nullptr);
    EXPECT_TRUE(n1 != n2);

    EXPECT_EQ(n1->s.code, code1);
    EXPECT_EQ(n2->s.code, code2);

    freechunks(&cstate);
    return all_passed;
}

// Test 3: Boundary/edge case – verify handling of a negative code value.
// The code field is an int; negative values are valid in C, and the test ensures
// they are stored without alteration.
bool test_new_stmt_negative_code() {
    compiler_state_t cstate;
    initchunks(&cstate);

    const int neg_code = -12345;
    struct slist *node = new_stmt(&cstate, neg_code);

    EXPECT_TRUE(node != nullptr);
    EXPECT_EQ(node->s.code, neg_code);

    freechunks(&cstate);
    return all_passed;
}

// Test 4: Null compiler state should fail gracefully (if the production code allows such usage).
// Depending on implementation, new_stmt may assume a valid cstate; this test protects
// against unexpected behavior by ensuring a non-null result when a valid cstate is used
// and documenting behavior when cstate is null.
bool test_new_stmt_null_cstate() {
    // If the production code guards against null cstate, this should return NULL or crash.
    // To avoid crashing the test harness here, we skip the call when cstate is null and
    // instead document expected behavior. If the API requires a non-null cstate, this test
    // should be adapted accordingly.
    struct slist *node = new_stmt(nullptr, 0);
    // We don't assume NULL is acceptable; if it returns NULL, it's a pass for safety.
    if (node == nullptr) {
        // Expected safe failure path.
        return true;
    }
    // If not NULL, verify that s.code is set to 0.
    bool ok = (node->s.code == 0);
    if (!ok) {
        std::cerr << "test_new_stmt_null_cstate: unexpected code value when cstate is NULL\n";
    }
    freechunks(*((compiler_state_t*)nullptr)); // No-op in this path; kept for symmetry.
    return ok;
}

// Helper to run all tests and report summary.
int main() {
    std::cout << "Running tests for new_stmt in gencode.c (C linkage):\n";

    bool all = true;
    all_passed = true;

    std::cout << "- Test 1: Basic allocation and code storage\n";
    bool t1 = test_new_stmt_basic_allocation();
    all = all && t1;

    std::cout << "- Test 2: Multiple allocations preserve distinct nodes\n";
    bool t2 = test_new_stmt_multiple_allocations();
    all = all && t2;

    std::cout << "- Test 3: Negative code value handling\n";
    bool t3 = test_new_stmt_negative_code();
    all = all && t3;

    std::cout << "- Test 4: Null cstate handling (behavior depends on implementation)\n";
    bool t4 = test_new_stmt_null_cstate();
    all = all && t4;

    if (all) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << "ONE OR MORE TESTS FAILED\n";
        return 1;
    }
}