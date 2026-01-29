// Test suite for sqlite3Fts5ParseNodeFree
// - Target: ensure correct behavior for freeing Fts5ExprNode trees
// - Non-GTest, simple C++11 test harness with manual assertions
// - Mocks for sqlite3_free and sqlite3Fts5ParseNearsetFree to observe calls
// - Uses actual types from fts5Int.h / fts5parse.h when available

#include <vector>
#include <fts5Int.h>
#include <fts5parse.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


extern "C" {
    // Include the real headers to obtain proper type definitions
    // (these should exist in the project tree)
    #include "fts5Int.h"
    #include "fts5parse.h"

    // Forward declarations for the function under test (C linkage)
    void sqlite3Fts5ParseNodeFree(Fts5ExprNode *p);
}

// Mock state tracked in tests
static std::vector<void*> g_free_order;
static std::vector<Fts5ParseNearset*> g_near_order;

// Mock: intercept sqlite3_free to observe deallocation order
extern "C" void sqlite3_free(void* p){
    g_free_order.push_back(p);
    // Use standard free to actually release memory allocated by tests
    free(p);
}

// Mock: intercept sqlite3Fts5ParseNearsetFree to observe nearset freeing
extern "C" void sqlite3Fts5ParseNearsetFree(Fts5ParseNearset *p){
    g_near_order.push_back(p);
    // Do not attempt to free p here; the test controls memory lifecycle
}

// Helper to reset mock state between tests
static void reset_mock_state() {
    g_free_order.clear();
    g_near_order.clear();
}

// Helper to print test results
static void print_summary(int passed, int total) {
    std::cout << "Tests passed: " << passed << " / " << total << std::endl;
}

// Test 1: Null input should be a no-op (no crashes, no frees)
static bool test_null_input_noop() {
    reset_mock_state();
    // Should not crash
    sqlite3Fts5ParseNodeFree(nullptr);

    bool ok = g_free_order.empty() && g_near_order.empty();
    if(!ok){
        std::cerr << "Test 1 (null input) unexpectedly freed memory or called nearset free." << std::endl;
    }
    return ok;
}

// Test 2: Node with no children, non-null pNear argument
static bool test_node_no_children_frees_self_and_near() {
    reset_mock_state();

    // Allocate a single node
    Fts5ExprNode *p = (Fts5ExprNode*)malloc(sizeof(Fts5ExprNode));
    // Initialize fields used by sqlite3Fts5ParseNodeFree
    // The actual layout is defined by the project's headers; we set commonly used fields.
    p->nChild = 0;
    p->apChild = nullptr;
    p->pNear = (Fts5ParseNearset*)0xDEADBEEF; // sentinel pointer

    sqlite3Fts5ParseNodeFree(p);

    // Expectations:
    // - Only p should be freed (order should be [p])
    // - sqlite3Fts5ParseNearsetFree should be called with pNear (non-null)
    bool near_called_ok = (g_near_order.size() == 1 && g_near_order[0] == p->pNear);
    bool free_order_ok = (g_free_order.size() == 1 && g_free_order[0] == p);

    if(!near_called_ok){
        std::cerr << "Test 2: nearset free not called with correct pointer." << std::endl;
    }
    if(!free_order_ok){
        std::cerr << "Test 2: free order incorrect or pointer not freed as expected." << std::endl;
    }

    // Cleanup (in case free didn't actually free all memory due to mocks)
    // We freed p in the call; no explicit delete needed here.

    return near_called_ok && free_order_ok;
}

// Test 3: Node with two children, verify recursive frees order (child1, child2, parent)
static bool test_node_with_two_children_recursive_free() {
    reset_mock_state();

    // Child 1
    Fts5ExprNode *child1 = (Fts5ExprNode*)malloc(sizeof(Fts5ExprNode));
    child1->nChild = 0;
    child1->apChild = nullptr;
    child1->pNear = (Fts5ParseNearset*)0x1111;

    // Child 2
    Fts5ExprNode *child2 = (Fts5ExprNode*)malloc(sizeof(Fts5ExprNode));
    child2->nChild = 0;
    child2->apChild = nullptr;
    child2->pNear = (Fts5ParseNearset*)0x2222;

    // Parent with two children
    Fts5ExprNode *parent = (Fts5ExprNode*)malloc(sizeof(Fts5ExprNode));
    parent->nChild = 2;
    parent->apChild = (Fts5ExprNode**)malloc(sizeof(Fts5ExprNode*) * 2);
    parent->apChild[0] = child1;
    parent->apChild[1] = child2;
    parent->pNear = (Fts5ParseNearset*)0x3333;

    sqlite3Fts5ParseNodeFree(parent);

    // Expected frees:
    // - Free child1, then child1's nearset, then free child2, then its nearset,
    //   then free parent after freeing its nearset.
    // For our simplified check, we verify order of freed pointers is:
    // [child1, child2, parent]
    bool freed_order_ok = false;
    if(g_free_order.size() >= 3) {
        freed_order_ok = (g_free_order[0] == child1) &&
                         (g_free_order[1] == child2) &&
                         (g_free_order[2] == parent);
    }

    // Nearset frees observed (order corresponding to each node freed)
    // We expect 3 nearset frees: child1.pNear, child2.pNear, parent.pNear in some internal order.
    bool near_order_len_ok = (g_near_order.size() >= 3);
    if(near_order_len_ok){
        // Just ensure the pointers observed include all expected nearsets
        bool has_child1 = false, has_child2 = false, has_parent = false;
        for(auto pn : g_near_order){
            if(pn == child1->pNear) has_child1 = true;
            if(pn == child2->pNear) has_child2 = true;
            if(pn == parent->pNear) has_parent = true;
        }
        near_order_len_ok = has_child1 && has_child2 && has_parent;
    }

    if(!freed_order_ok){
        std::cerr << "Test 3: Free order is incorrect. Expected [child1, child2, parent]." << std::endl;
    }
    if(!near_order_len_ok){
        std::cerr << "Test 3: Nearset free calls did not include all expected pointers." << std::endl;
    }

    return freed_order_ok && near_order_len_ok;
}

int main() {
    int total = 3;
    int passed = 0;

    if(test_null_input_noop()) {
        ++passed;
    } else {
        std::cerr << "Failure in Test 1: Null input should be a no-op." << std::endl;
    }

    if(test_node_no_children_frees_self_and_near()) {
        ++passed;
    } else {
        std::cerr << "Failure in Test 2: Node with no children should free itself and call nearset free." << std::endl;
    }

    if(test_node_with_two_children_recursive_free()) {
        ++passed;
    } else {
        std::cerr << "Failure in Test 3: Node with two children should recursively free both children then itself." << std::endl;
    }

    print_summary(passed, total);
    return (passed == total) ? 0 : 1;
}