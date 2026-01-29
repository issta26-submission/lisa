// Comprehensive C++11 unit tests for the focal function delete_pcap_dag
// This test suite is designed to run without Google Test (GTest) and uses a
// lightweight, custom test harness with non-terminating expectations.
// The goal is to cover true/false branches of the predicate and linkage updates.

#include <string.h>
#include <errno.h>
#include <vector>
#include <endian.h>
#include <dagpci.h>
#include <pcap-util.h>
#include <unistd.h>
#include <pcap-dag.h>
#include <limits.h>
#include <dag_config_api.h>
#include <stdlib.h>
#include <config.h>
#include <pcap-int.h>
#include <iostream>
#include <dagapi.h>


// Domain knowledge and candidate keywords:
// - Candidate Keywords: pcap_t, pcap_dag_node_t, pcap_dags (global head), next pointer
// - Core behavior: traverse list, find node with curr->p == p, unlink it from the list
// - Edge cases: deleting head, deleting in middle, deleting tail, not found, duplicates

// Focal dependencies (simplified stubs to allow compilation and testing in C++11)
struct pcap_t {
    int id; // simple identifier for debugging
};

// Node of the DAG list as used by delete_pcap_dag
struct pcap_dag_node_t {
    pcap_t *p;
    pcap_dag_node_t *next;
};

// Global head of the DAG list (as in the real project)
static pcap_dag_node_t *pcap_dags = nullptr;

// The focal function under test (reproduced minimal version for test harness)
static void delete_pcap_dag(const pcap_t *p)
{
    pcap_dag_node_t *curr = nullptr, *prev = nullptr;
    for (prev = nullptr, curr = pcap_dags; curr != nullptr && curr->p != p; prev = curr, curr = curr->next) {
        /* empty loop body, just traversing until match or end */
    }
    if (curr != nullptr && curr->p == p) {
        if (prev != nullptr) {
            prev->next = curr->next;
        } else {
            pcap_dags = curr->next;
        }
    }
}

// Simple memory management helper to avoid leaks in tests.
// We keep track of all allocated nodes and free them after each test.
static std::vector<pcap_dag_node_t*> allocated_nodes;
static pcap_dag_node_t* make_node(pcap_t *p) {
    pcap_dag_node_t* n = new pcap_dag_node_t{p, nullptr};
    allocated_nodes.push_back(n);
    return n;
}
static void clear_all_nodes() {
    // delete all allocated DAG nodes
    for (pcap_dag_node_t* n : allocated_nodes) {
        delete n;
    }
    allocated_nodes.clear();
    pcap_dags = nullptr;
}

// Lightweight test harness
static int tests_run = 0;
static int tests_failed = 0;
#define EXPECT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "EXPECT_TRUE failed: " << (msg) << " [Line " << __LINE__ << "]" << std::endl; \
            tests_failed++; \
        } \
        tests_run++; \
    } while (0)

#define EXPECT_PTR_EQ(a, b, msg) \
    do { \
        if ((a) != (b)) { \
            std::cerr << "EXPECT_PTR_EQ failed: " << (msg) << " (expected " << (void*)(b) \
                      << ", got " << (void*)(a) << ") [Line " << __LINE__ << "]" << std::endl; \
            tests_failed++; \
        } \
        tests_run++; \
    } while (0)

// Test 1: Deleting from an empty list should leave the list empty
static void test_delete_from_empty_list()
{
    // Comment: Candidate Keywords - pcap_dags (global), empty traversal
    clear_all_nodes();
    pcap_t dummy;
    delete_pcap_dag(&dummy);

    EXPECT_TRUE(pcap_dags == nullptr, "Deleting from an empty list should keep the list empty (pcap_dags == nullptr).");
}

// Test 2: Deleting the only node in the list should result in an empty list
static void test_delete_head_only()
{
    // Comment: Deleting head when it's the only node
    clear_all_nodes();
    pcap_t a;
    a.id = 1;
    pcap_dag_node_t *n1 = make_node(&a);
    pcap_dags = n1;

    delete_pcap_dag(&a);

    EXPECT_TRUE(pcap_dags == nullptr, "After deleting the only node, the list should be empty (nullptr).");
    // No manual free here; clear_all_nodes will clean up allocated nodes
    clear_all_nodes();
}

// Test 3: Deleting the head in a multi-node list should update the head to the second node
static void test_delete_head_in_multi()
{
    // Comment: Deleting head when multiple nodes exist
    clear_all_nodes();

    pcap_t a, b, c;
    a.id = 10; b.id = 20; c.id = 30;

    pcap_dag_node_t *n1 = make_node(&a);
    pcap_dag_node_t *n2 = make_node(&b);
    pcap_dag_node_t *n3 = make_node(&c);

    n1->next = n2;
    n2->next = n3;
    n3->next = nullptr;
    pcap_dags = n1;

    delete_pcap_dag(&a);

    // After deletion, head should be the second node
    EXPECT_PTR_EQ(pcap_dags, n2, "Head should be updated to the second node after deleting the first node.");
    EXPECT_PTR_EQ(pcap_dags->p, &b, "New head should point to the second node (p == &b).");
    EXPECT_PTR_EQ(pcap_dags->next, n3, "New head's next should be the third node after deletion.");

    clear_all_nodes();
}

// Test 4: Deleting a node from the middle should link previous node to next
static void test_delete_middle()
{
    // Comment: Deleting a middle node should bypass it in the list
    clear_all_nodes();

    pcap_t x, y, z;
    x.id = 100; y.id = 200; z.id = 300; // y is the target

    pcap_dag_node_t *nx = make_node(&x);
    pcap_dag_node_t *ny = make_node(&y);
    pcap_dag_node_t *nz = make_node(&z);

    nx->next = ny;
    ny->next = nz;
    nz->next = nullptr;
    pcap_dags = nx;

    delete_pcap_dag(&y);

    // Head should remain the same, but ny should be bypassed
    EXPECT_PTR_EQ(pcap_dags, nx, "Head should remain unchanged after deleting a middle node.");
    EXPECT_PTR_EQ(nx->next, nz, "Previous node should now point directly to the next node after deletion.");

    clear_all_nodes();
}

// Test 5: Deleting the tail should set the previous node's next to nullptr
static void test_delete_tail()
{
    // Comment: Deleting last element should terminate the list correctly
    clear_all_nodes();

    pcap_t a1, a2, a3;
    a1.id = 1; a2.id = 2; a3.id = 3;

    pcap_dag_node_t *n1 = make_node(&a1);
    pcap_dag_node_t *n2 = make_node(&a2);
    pcap_dag_node_t *n3 = make_node(&a3);

    n1->next = n2;
    n2->next = n3;
    n3->next = nullptr;
    pcap_dags = n1;

    delete_pcap_dag(&a3);

    // After deletion, head should still be n1, n2 should be tail with next == nullptr
    EXPECT_PTR_EQ(pcap_dags, n1, "Head should remain unchanged after deleting tail.");
    EXPECT_PTR_EQ(n1->next, n2, "Head's next should remain the second node after tail deletion.");
    EXPECT_TRUE(n2->next == nullptr, "Tail deletion should result in the previous node's next being nullptr.");

    clear_all_nodes();
}

// Test 6: Deleting the first of two identical p entries should move head to the second
static void test_delete_first_of_two_duplicates()
{
    // Comment: If duplicates exist, first match should be deleted (head case)
    clear_all_nodes();

    pcap_t p1, p2;
    p1.id = 11; p2.id = 22;

    pcap_dag_node_t *n1 = make_node(&p1);
    pcap_dag_node_t *n2 = make_node(&p2);

    n1->next = n2;
    n2->next = nullptr;
    pcap_dags = n1;

    delete_pcap_dag(&p1);

    // Now head should be the second node
    EXPECT_PTR_EQ(pcap_dags, n2, "After deleting first duplicate, head should become the second node.");
    EXPECT_TRUE(pcap_dags->next == nullptr, "New tail should be the second node with next == nullptr.");

    clear_all_nodes();
}

int main()
{
    // Run tests
    test_delete_from_empty_list();
    test_delete_head_only();
    test_delete_head_in_multi();
    test_delete_middle();
    test_delete_tail();
    test_delete_first_of_two_duplicates();

    // Report results
    std::cout << "Tests run: " << tests_run << ", Failures: " << tests_failed << std::endl;
    return tests_failed; // non-zero if any test failed
}