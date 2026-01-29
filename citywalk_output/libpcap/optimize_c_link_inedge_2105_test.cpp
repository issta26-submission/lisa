// Lightweight C++11 unit tests for the focal method: link_inedge
// This test suite exercises the behavior of linking a new edge (parent)
// into the in_edges list of a block (child).
// Notes:
// - We assume the following minimal C structs exist in the target codebase:
//
//   struct edge { struct edge *next; };
//   struct block { struct edge *in_edges; };
//
// - The function under test (extern "C") is:
//   void link_inedge(struct edge *parent, struct block *child);
//
// - Tests are non-terminating: they report failures but continue executing.
// - This file does not depend on Google Test; it uses a tiny custom harness.

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


// Forward declare the focal C structures and function to enable testing from C++
extern "C" {
struct edge { struct edge *next; };
struct block { struct edge *in_edges; };

// Focal method under test
void link_inedge(struct edge *parent, struct block *child);
}

// Simple non-terminating test framework
static int g_test_count = 0;
static int g_failure_count = 0;

// Expectation: two pointers are equal
template <typename T>
static void expect_ptr_eq(T* a, T* b, const char* msg = "")
{
    ++g_test_count;
    if (a != b) {
        std::cerr << "FAIL [" << g_test_count << "] " << (msg ? msg : "")
                  << " - expected: " << static_cast<void*>(a)
                  << ", actual: " << static_cast<void*>(b) << std::endl;
        ++g_failure_count;
    }
}

// Expectation: two values are equal
template <typename T>
static void expect_eq(T a, T b, const char* msg = "")
{
    ++g_test_count;
    if (!(a == b)) {
        std::cerr << "FAIL [" << g_test_count << "] " << (msg ? msg : "")
                  << " - expected: " << a << ", actual: " << b << std::endl;
        ++g_failure_count;
    }
}

// Helper to print a small header for each test
static void print_test_header(const char* name)
{
    std::cout << "Running test: " << name << " ..." << std::endl;
}

/*
Test 1: Basic insertion when the child's in_edges is initially NULL
Expected:
  - parent->next == NULL (since there was no prior in-edge)
  - child->in_edges == parent (the new head of the in_edges list)
*/
static void test_link_inedge_basic_null_initialization()
{
    print_test_header("test_link_inedge_basic_null_initialization");

    edge parent;
    parent.next = nullptr;

    block child;
    child.in_edges = nullptr;

    link_inedge(&parent, &child);

    expect_ptr_eq(parent.next, nullptr, "parent.next should be NULL after linking when child's in_edges was NULL");
    expect_ptr_eq(child.in_edges, &parent, "child.in_edges should point to the new edge (parent) after linking");
}

// Test 2: Insertion when there is an existing head in child's in_edges
// Precondition: child.in_edges points to an existing edge (old_head)
static void test_link_inedge_inserts_at_head_with_existing_head()
{
    print_test_header("test_link_inedge_inserts_at_head_with_existing_head");

    edge old_head;
    old_head.next = nullptr;

    block child;
    child.in_edges = &old_head;

    edge new_parent;
    new_parent.next = nullptr;

    link_inedge(&new_parent, &child);

    // After linking, new_parent becomes the new head
    expect_ptr_eq(child.in_edges, &new_parent, "child.in_edges should be updated to the new head (new_parent)");
    // The new head's next should point to the previous head
    expect_ptr_eq(new_parent.next, &old_head, "new head's next should point to the old head (old_head)");
}

// Test 3: Chain insertion to verify LIFO order of multiple in-edges
// Sequence: initial insert makes head = A; second insert makes head = B and B.next = A
static void test_link_inedge_chain_order()
{
    print_test_header("test_link_inedge_chain_order");

    // First insertion: create child with no existing in_edges
    edge headA;
    headA.next = nullptr;

    block child;
    child.in_edges = nullptr;

    edge A;
    A.next = nullptr;

    // Link A as the first in-edge
    link_inedge(&A, &child);

    // Validate first insertion
    expect_ptr_eq(child.in_edges, &A, "After first insertion, child.in_edges should be A");
    expect_ptr_eq(A.next, nullptr, "After first insertion, A.next should remain NULL");

    // Second insertion: B becomes new head; B.next should point to A
    edge B;
    B.next = nullptr;

    link_inedge(&B, &child);

    // Validate second insertion creates chain B -> A
    expect_ptr_eq(child.in_edges, &B, "After second insertion, child.in_edges should be B");
    expect_ptr_eq(B.next, &A, "B.next should point to A to form chain B -> A");

    // Ensure the previous head (A) remains consistent in the chain
    expect_ptr_eq(A.next, nullptr, "A.next should remain NULL after being linked behind B");
}

// Entry point: run all tests and report a final summary
int main()
{
    // Run tests
    test_link_inedge_basic_null_initialization();
    test_link_inedge_inserts_at_head_with_existing_head();
    test_link_inedge_chain_order();

    // Summary
    std::cout << "Test summary: " << g_test_count << " tests executed, "
              << g_failure_count << " failures." << std::endl;

    // Return non-zero if any test failed
    return g_failure_count;
}