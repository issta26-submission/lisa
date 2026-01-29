// C++11 unit test suite for the focal method finish_parse in gencode.c
// This test suite is designed to be compiled alongside the provided C sources
// (gencode.c and its dependencies) in a C++11 compatible build environment.
// It uses a lightweight, non-terminating assertion approach to maximize code
// coverage without terminating on first failure, as per domain guidance.

// NOTE:
// - The test relies on the project's public interface exposed via gencode.h.
// - We link against the project's object containing finish_parse and its helpers.
// - We avoid GTest; instead, we implement simple EXPECT-like macros that do not
//   abort the test suite, allowing multiple checks to run in a single process.

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
#include <stdint.h>
#include <stddef.h>
#include <thread-local.h>
#include <pcap-util.h>
#include <scanner.h>
#include <setjmp.h>
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
#include <cstring>


// Bring C interfaces into the C++ test using C linkage.
extern "C" {
}

// Lightweight non-terminating test assertion helpers
static bool g_tests_failed = false;

#define EXPECT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "[FAIL] " << (msg) << " (expected true, got false)\n"; \
            g_tests_failed = true; \
        } \
    } while (0)

#define EXPECT_EQ(a, b, msg) \
    do { \
        if (!((a) == (b))) { \
            std::cerr << "[FAIL] " << (msg) << " (expected: " << (a) << " == " << (b) \
                      << ")\n"; \
            g_tests_failed = true; \
        } \
    } while (0)

#define EXPECT_PTR_EQ(a, b, msg) \
    do { \
        if ((a) != (b)) { \
            std::cerr << "[FAIL] " << (msg) << " (expected pointer " \
                      << (void*)(a) << " == " << (void*)(b) << ")\n"; \
            g_tests_failed = true; \
        } \
    } while (0)


// Helper: run a basic finish_parse invocation with a freshly created root block.
// This exercises the non-PPi branch (default) and ensures the function completes
// without error. It asserts that the function returns 0 (success) and that the
// ic.root is assigned based on the root's head.
static bool test_finish_parse_basic_path()
{
    // Initialize the compiler state to a clean slate.
    compiler_state_t cstate;
    std::memset(&cstate, 0, sizeof(cstate));

    // Create a root block; rely on project's allocator (externed in gencode.h)
    struct block *root = new_block(&cstate, 0);

    // Ensure non-PPi path by leaving linktype as default (0) if DLT_PPI is non-zero.
    // This exercises the else-branch: "if (cstate->linktype == DLT_PPI) { ... }"
    // Since we can't guarantee the exact value mapping here, we leave as zero
    // and rely on the project to treat zero as non-PPi in tests.
    int ret = finish_parse(&cstate, root);

    // Assertions: function should succeed (return 0) and root assignment should occur.
    EXPECT_EQ(ret, 0, "finish_parse should return 0 on normal path");
    // Accessing ic.root and head fields relies on public structure; compare pointers.
    // We consider a successful root assignment if ic.root is set to either root->head
    // or is equal to the root's internal head (if available). If not accessible,
    // at least ensure function returned 0 above.
    // We verify that ic.root matches the current p->head after processing.
    EXPECT_PTR_EQ(cstate.ic.root, root->head, "cstate.ic.root should be set to root->head");

    // Cleanup is managed by project memory management; if not, it's harmless to skip here.
    return !g_tests_failed;
}

// Helper: run a finish_parse invocation with the DLT_PPI (PPi) path enabled.
// This exercises the true-branch of the linktype check: (cstate->linktype == DLT_PPI)
static bool test_finish_parse_ppi_path()
{
    compiler_state_t cstate;
    std::memset(&cstate, 0, sizeof(cstate));

    // Create a root block as before.
    struct block *root = new_block(&cstate, 0);

    // Force the DLT_PPI path by setting the linktype to DLT_PPI.
    // DLT_PPI is typically defined in pcap headers; we assume it's available via gencode.h.
    cstate.linktype = DLT_PPI;

    int ret = finish_parse(&cstate, root);

    EXPECT_EQ(ret, 0, "finish_parse should return 0 on PPi path");
    EXPECT_PTR_EQ(cstate.ic.root, root->head, "cstate.ic.root should be set to root->head on PPi path");

    return !g_tests_failed;
}

int main()
{
    std::cout << "Running finish_parse unit tests (non-GTest) ...\n";

    // Reset global failure flag
    g_tests_failed = false;

    // Run Test 1: Basic path (non-PPi)
    bool ok_basic = test_finish_parse_basic_path();
    if (ok_basic) {
        std::cout << "[PASS] test_finish_parse_basic_path\n";
    } else {
        std::cout << "[WARN] test_finish_parse_basic_path encountered failures\n";
    }

    // Run Test 2: PPi path
    bool ok_ppi = test_finish_parse_ppi_path();
    if (ok_ppi) {
        std::cout << "[PASS] test_finish_parse_ppi_path\n";
    } else {
        std::cout << "[WARN] test_finish_parse_ppi_path encountered failures\n";
    }

    bool all_pass = ok_basic && ok_ppi && !g_tests_failed;
    if (all_pass) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED\n";
        return 1;
    }
}