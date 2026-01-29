// Unit test suite for image_transform_test_counter (pngvalid.c) using C++11 (no GTest)
// This test suite exercises the focal function by constructing in-memory
// mock lists that mimic the real image_transform chain, while relying on
// the external sentinel image_transform_end provided by the target C codebase.

#include <cstdint>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <math.h>


// Import C declarations and types from the focal codebase
extern "C" {

// Minimal replica of the library's image_transform structure for test harness.
// The real project defines a matching struct; this must align layout-wise.
struct image_transform {
    unsigned int local_use;
    struct image_transform *list;
};

// External symbols provided by the pngvalid.c (or linked object):
extern image_transform *image_transform_first;
extern image_transform image_transform_end;

// Focal function under test
int image_transform_test_counter(png_uint_32 counter, unsigned int max);
}

// Helper: build a small chain of image_transform nodes.
// n: number of nodes to chain (0 allowed to simulate empty list, but we handle empty list
//    by pointing image_transform_first directly at image_transform_end in tests).
// uses: array of local_use values for each node.
// Returns void; the created chain is assigned to the global image_transform_first.
static void build_chain(int n, const unsigned int uses[], image_transform *out_first)
{
    // Use a local buffer to hold the nodes valid for the lifetime of the test call.
    // The array is sized to accommodate typical test cases (up to 8 nodes should be enough here).
    image_transform nodes[8];

    for (int i = 0; i < n; ++i) {
        nodes[i].local_use = uses[i];
        if (i < n - 1)
            nodes[i].list = &nodes[i + 1];
        else
            nodes[i].list = &image_transform_end; // last node points to sentinel
    }

    // Link the global first pointer to the first node in this local chain
    *out_first = &nodes[0];
    // NOTE: The lifetime of 'nodes' is limited to this function invocation;
    // the subsequent call to the focal function occurs within this scope,
    // so the chain remains valid for that call.
}

// Test 1: Empty list, counter=0, max=0 -> should return true (1)
static bool test_empty_list_counter0_max0()
{
    // Point the list head directly at the sentinel to simulate empty list
    image_transform_first = &image_transform_end;

    int result = image_transform_test_counter(0u, 0u);
    bool pass = (result == 1);
    std::cout << "Test 1 (empty list, c=0, max=0): " << (pass ? "PASS" : "FAIL")
              << " [expected 1, got " << result << "]\n";
    return pass;
}

// Test 2: Empty list, counter nonzero, max=0 -> should return false (0)
static bool test_empty_list_counter5_max0()
{
    image_transform_first = &image_transform_end;

    int result = image_transform_test_counter(5u, 0u);
    bool pass = (result == 0);
    std::cout << "Test 2 (empty list, c=5, max=0): " << (pass ? "PASS" : "FAIL")
              << " [expected 0, got " << result << "]\n";
    return pass;
}

// Test 3: Non-empty list, max=1, counter after 3 shifts becomes 0 -> returns 1
static bool test_three_node_counter7_max1()
{
    // Build a chain of 3 nodes with arbitrary local_use values (not used when max<=1)
    image_transform first_node;
    image_transform second_node;
    image_transform third_node;
    image_transform_end = image_transform_end; // ensure sentinel exists (no-op)

    const unsigned int uses[3] = {0, 0, 0};
    image_transform *out_first = nullptr;
    // Place in local scope and set up chain
    first_node.local_use = uses[0];
    second_node.local_use = uses[1];
    third_node.local_use = uses[2];
    first_node.list = &second_node;
    second_node.list = &third_node;
    third_node.list = &image_transform_end;
    image_transform_first = &first_node;

    // The test expects 7 (111) >> 3 == 0
    int result = image_transform_test_counter(7u, 1u);
    bool pass = (result == 1);
    std::cout << "Test 3 (3-node chain, c=7, max=1): " << (pass ? "PASS" : "FAIL")
              << " [expected 1, got " << result << "]\n";
    return pass;
}

// Test 4: Non-empty list, max=1, counter after 3 shifts remains nonzero -> returns 0
static bool test_three_node_counter8_max1()
{
    image_transform first_node;
    image_transform second_node;
    image_transform third_node;
    image_transform_end = image_transform_end; // sentinel

    const unsigned int uses[3] = {0, 0, 0};
    first_node.local_use = uses[0];
    second_node.local_use = uses[1];
    third_node.local_use = uses[2];
    first_node.list = &second_node;
    second_node.list = &third_node;
    third_node.list = &image_transform_end;
    image_transform_first = &first_node;

    // 8 >> 3 == 1
    int result = image_transform_test_counter(8u, 1u);
    bool pass = (result == 0);
    std::cout << "Test 4 (3-node chain, c=8, max=1): " << (pass ? "PASS" : "FAIL")
              << " [expected 0, got " << result << "]\n";
    return pass;
}

// Test 5: Single node, max=2 (branch taken), local_use < max -> early return 1
static bool test_single_node_early_exit()
{
    image_transform node;
    image_transform_end = image_transform_end; // sentinel
    node.local_use = 0; // < max
    node.list = &image_transform_end;
    image_transform_first = &node;

    int result = image_transform_test_counter(0u, 2u);
    bool pass = (result == 1);
    std::cout << "Test 5 (single node, c=0, max=2): " << (pass ? "PASS" : "FAIL")
              << " [expected 1, got " << result << "]\n";
    return pass;
}

// Test 6: Two nodes, max=2, all local_use >= max -> no early exit and max>1 so final result should be 0
static bool test_two_nodes_no_early_exit()
{
    image_transform first_node;
    image_transform second_node;
    image_transform_end = image_transform_end; // sentinel
    first_node.local_use = 2; // equal to max
    second_node.local_use = 2; // equal to max
    first_node.list = &second_node;
    second_node.list = &image_transform_end;
    image_transform_first = &first_node;

    int result = image_transform_test_counter(3u, 2u);
    bool pass = (result == 0);
    std::cout << "Test 6 (2-node chain, c=3, max=2): " << (pass ? "PASS" : "FAIL")
              << " [expected 0, got " << result << "]\n";
    return pass;
}

// Main entry: run all tests and report summary
int main(void)
{
    bool passes =
        test_empty_list_counter0_max0() &
        test_empty_list_counter5_max0() &
        test_three_node_counter7_max1() &
        test_three_node_counter8_max1() &
        test_single_node_early_exit() &
        test_two_nodes_no_early_exit();

    // Count how many tests actually passed
    // (Since we used bitwise AND in a single expression, the above already
    // reported per-test results. Here, a simple pass/fail summary is provided.)

    // For a precise summary, re-run with a counter (omitted for brevity).
    // We'll simply return 0 if all tests passed, non-zero otherwise.
    // Since we used individual booleans, recompute here by re-running quickly:
    int total_passed = 0;
    total_passed += test_empty_list_counter0_max0() ? 1 : 0;
    total_passed += test_empty_list_counter5_max0() ? 1 : 0;
    total_passed += test_three_node_counter7_max1() ? 1 : 0;
    total_passed += test_three_node_counter8_max1() ? 1 : 0;
    total_passed += test_single_node_early_exit() ? 1 : 0;
    total_passed += test_two_nodes_no_early_exit() ? 1 : 0;

    std::cout << "Summary: " << total_passed << " / 6 tests passed.\n";
    return (total_passed == 6) ? 0 : 1;
}