// Unit test suite for the focal method new_pcap_dag in pcap-dag.c
// This test is written for C++11 without using GTest.
// It focuses on the true-path behavior of new_pcap_dag and basic linked-list updates.
// Notes:
// - We cannot reliably simulate malloc failure from outside the translation unit, so the
//   malloc-failure path is not tested here (see comments within tests).
// - The tests operate on the actual C data structures and global state exported by pcap-dag.c.
// - Tests are designed to be non-terminating on assertion failure so that all tests run.

#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <cstdlib>
#include <endian.h>
#include <dagpci.h>
#include <pcap-util.h>
#include <unistd.h>
#include <pcap-dag.h>
#include <limits.h>
#include <dag_config_api.h>
#include <cstdio>
#include <stdlib.h>
#include <config.h>
#include <pcap-int.h>
#include <dagapi.h>


// Include the C header that defines pcap_t, pcap_dag_node_t, and the global pcap_dags.
// We rely on C linkage for the focal functions/variables.
extern "C" {
}

// Forward declare pcap_t alias as used by the C function signature.
// We do not need the full definition since new_pcap_dag only stores the pointer.
typedef struct pcap_t pcap_t;

// Non-terminating test harness
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        fprintf(stderr, "TEST_FAIL: %s\n", msg); \
        g_failures++; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if((a) != (b)) { \
        fprintf(stderr, "TEST_FAIL: %s (expect %ld, got %ld)\n", msg, (long)(b), (long)(a)); \
        g_failures++; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a, b, msg) do { \
    if((a) != (b)) { \
        fprintf(stderr, "TEST_FAIL: %s (ptr expect %p, got %p)\n", msg, (void*)(b), (void*)(a)); \
        g_failures++; \
    } \
} while(0)


// External C state used by the focal function
// pcap_dags is the head of the dag-linked list in the C file.
extern "C" pcap_dag_node_t *pcap_dags;

// Helper to clean up the dag list to ensure test isolation
static void cleanup_pcap_dag_list() {
    while (pcap_dags != nullptr) {
        pcap_dag_node_t *node = pcap_dags;
        pcap_dags = node->next;
        free(node);
    }
}

// Test 1: First call to new_pcap_dag should allocate a node, set its fields,
// and push it as the new head in the global pcap_dags list.
static void test_new_pcap_dag_first_call() {
    // Ensure clean slate
    cleanup_pcap_dag_list();
    pcap_dags = nullptr;

    // Use a non-null dummy pointer as the pcap_t input.
    pcap_t *dummy_ptr = reinterpret_cast<pcap_t*>(0x1001);

    int ret = new_pcap_dag(dummy_ptr);

    // Expect success
    EXPECT_EQ(ret, 0, "new_pcap_dag should return 0 on success (first call)");

    // The head of the dag list should now be non-null
    EXPECT_TRUE(pcap_dags != nullptr, "pcap_dags should point to a newly allocated node after first call");

    // The head node should have next equal to NULL (as it's a new head)
    EXPECT_TRUE(pcap_dags->next == nullptr, "New head's next should be NULL (no previous nodes on first call)");

    // The head's p field should match the input pointer
    EXPECT_PTR_EQ(pcap_dags->p, dummy_ptr, "Head node's p should equal the input pcap_t pointer");

    // The head's pid should equal the current process id
    pid_t current_pid = getpid();
    EXPECT_EQ(pcap_dags->pid, current_pid, "Head node's pid should match getpid()");
}


// Test 2: Second call should prepend a new node and link the old head as next.
// Verify that the new head contains the new pcap_t pointer and that the old head is preserved as next.
static void test_new_pcap_dag_second_call() {
    // Start with a known initial state
    cleanup_pcap_dag_list();
    pcap_dags = nullptr;

    // First pointer
    pcap_t *first_ptr = reinterpret_cast<pcap_t*>(0x2002);
    int ret1 = new_pcap_dag(first_ptr);
    EXPECT_EQ(ret1, 0, "First call should succeed (setup for second call)");

    // Capture the old head
    pcap_dag_node_t *old_head = pcap_dags;
    EXPECT_TRUE(old_head != nullptr, "After first call, head should exist");

    // Second input pointer
    pcap_t *second_ptr = reinterpret_cast<pcap_t*>(0x3003);
    int ret2 = new_pcap_dag(second_ptr);
    EXPECT_EQ(ret2, 0, "Second call should succeed and prepend a new node");

    // New head should be the latest node
    pcap_dag_node_t *new_head = pcap_dags;
    EXPECT_TRUE(new_head != nullptr, "New head should exist after second call");
    EXPECT_PTR_EQ(new_head->p, second_ptr, "New head's p should be equal to the second input pointer");

    // The new head's next should point to the previous head
    EXPECT_PTR_EQ(new_head->next, old_head, "New head's next should point to the previous head");

    // The old head should remain accessible via new_head->next
    EXPECT_PTR_EQ(old_head->p, first_ptr, "Second test: old head's p should be the first input pointer");
    pid_t current_pid = getpid();
    EXPECT_EQ(new_head->pid, current_pid, "New head's pid should match getpid()");
    EXPECT_EQ(old_head->pid, current_pid, "Old head's pid should also match getpid()");
}


// Entry point for running tests
int main() {
    // Run tests
    test_new_pcap_dag_first_call();
    test_new_pcap_dag_second_call();

    // Cleanup to avoid leaks in tests
    cleanup_pcap_dag_list();

    if (g_failures != 0) {
        fprintf(stderr, "Total test failures: %d\n", g_failures);
        return 1;
    }

    printf("All tests passed.\n");
    return 0;
}