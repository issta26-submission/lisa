// Unit test suite for the focal function atexit_handler from pcap-dag.c
// This test suite is written for C++11 without using Google Test.
// It relies on symbol interposition to override C functions used by atexit_handler
// (getpid, dag_platform_cleanup, delete_pcap_dag) and on direct manipulation of the
// pcap_dags linked list as defined by the project headers.
//
// NOTE: This test assumes the internal structures exposed by pcap-dag.h include:
//   - a global linked list head named `pcap_dags` of a struct type with at least:
//       pid_t pid;
//       pcap_t *p;
//       struct Node *next;
//   - the existence of functions:
//       void dag_platform_cleanup(pcap_t *p);
//       void delete_pcap_dag(pcap_t *p);
//   - the ability to instantiate dummy pcap_t objects and link them into pcap_dags.
// The test uses symbol interposition to control behavior and advance the list.

#include <string.h>
#include <errno.h>
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
#include <cstring>
#include <dagapi.h>


// Include the project's header to get the actual types and globals.

// Global test state to observe calls
static int g_cleanup_calls = 0;
static int g_delete_calls = 0;

// Control the value returned by getpid() via interposition.
// By overriding getpid() in this test executable, the atexit_handler will compare
// against this value.
static pid_t TEST_GETPID = 12345;

// Forward declare to let the linker resolve the real atexit_handler symbol from pcap-dag.c
// (we're calling it directly, so no prototype is strictly necessary beyond what's provided by the header)
extern "C" void atexit_handler(void);

// Interposed getpid() to control the "current process id" seen by atexit_handler
extern "C" pid_t getpid(void) {
    return TEST_GETPID;
}

// Interposed dag_platform_cleanup to observe and advance the list
extern "C" void dag_platform_cleanup(pcap_t *p) {
    ++g_cleanup_calls;
    // Progress the pcap_dags list if possible to avoid infinite loop in the test.
    // We assume the true struct provides a 'next' pointer (pcap_dags->next).
    // If there is no next, set to NULL to terminate the loop.
    // Access through the project's type; the header should expose `pcap_dags` and its fields.
    extern pcap_dag_t *pcap_dags; // forward reference to the global head used by atexit_handler
    if (pcap_dags != NULL) {
        if (pcap_dags->next != NULL) {
            pcap_dags = pcap_dags->next;
        } else {
            pcap_dags = NULL;
        }
    }
}

// Interposed delete_pcap_dag to observe and advance the list
extern "C" void delete_pcap_dag(pcap_t *p) {
    ++g_delete_calls;
    extern pcap_dag_t *pcap_dags;
    if (pcap_dags != NULL) {
        if (pcap_dags->next != NULL) {
            pcap_dags = pcap_dags->next;
        } else {
            pcap_dags = NULL;
        }
    }
}

// Simple non-terminating test assertion macro
static int g_test_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        fprintf(stderr, "TEST FAILED: %s\n", msg); \
        ++g_test_failures; \
    } \
} while(0)

// Helper to create a dummy pcap_t object (opaque to the test)
static pcap_t* make_dummy_pcap() {
    // Allocate a placeholder pcap_t object. We don't dereference its internals in tests.
    return reinterpret_cast<pcap_t*>(malloc(sizeof(pcap_t)));
}

// Helper to create a dag node linking into the global pcap_dags list
// This relies on the project's struct having fields: pid, p, next.
static void link_node(pid_t pid_value, pcap_t* p, pcap_dag_t** out_node) {
    pcap_dag_t* node = reinterpret_cast<pcap_dag_t*>(malloc(sizeof(pcap_dag_t)));
    node->pid = pid_value;
    node->p = p;
    node->next = NULL;

    // Link to existing head
    extern pcap_dag_t *pcap_dags;
    node->next = pcap_dags;
    pcap_dags = node;
    if (out_node) *out_node = node;
}

// Test 1: Equal pid path with a non-null p pointer should invoke dag_platform_cleanup
static void test_equal_pid_calls_cleanup() {
    // Reset state
    g_cleanup_calls = 0;
    g_delete_calls = 0;
    TEST_GETPID = 12345; // ensure this equals node->pid

    // Create a single node where pid == TEST_GETPID and p != NULL
    pcap_t* p = make_dummy_pcap();
    // Clear existing list
    extern pcap_dag_t *pcap_dags;
    pcap_dags = NULL;
    // Link node: pid matches TEST_GETPID
    link_node(TEST_GETPID, p, NULL);

    // Run the function under test
    atexit_handler();

    // Validate expectations
    EXPECT_TRUE(g_cleanup_calls == 1, "dag_platform_cleanup should be called exactly once");
    EXPECT_TRUE(g_delete_calls == 0, "delete_pcap_dag should not be called in equal pid path");
    EXPECT_TRUE(pcap_dags == NULL, "pcap_dags should be advanced to NULL after processing");

    // Cleanup
    free(p);
    // free the created node; it was the head
    if (pcap_dags != NULL) {
        free(pcap_dags);
    }
    pcap_dags = NULL;
}

// Test 2: Different pid path should invoke delete_pcap_dag
static void test_different_pid_calls_delete() {
    // Reset state
    g_cleanup_calls = 0;
    g_delete_calls = 0;
    TEST_GETPID = 99999; // override so getpid() returns this value

    // Create a single node where pid != TEST_GETPID
    pcap_t* p = make_dummy_pcap();
    extern pcap_dag_t *pcap_dags;
    pcap_dags = NULL;
    // Link node: pid different from TEST_GETPID
    link_node(1, p, NULL);

    // Run the function under test
    atexit_handler();

    // Validate expectations
    EXPECT_TRUE(g_cleanup_calls == 0, "dag_platform_cleanup should not be called when pid differs");
    EXPECT_TRUE(g_delete_calls == 1, "delete_pcap_dag should be called exactly once in different-pid path");
    EXPECT_TRUE(pcap_dags == NULL, "pcap_dags should be advanced to NULL after processing");

    // Cleanup
    free(p);
    if (pcap_dags != NULL) {
        free(pcap_dags);
    }
    pcap_dags = NULL;
}

// Test 3: Chain of two nodes: first equal pid, second different pid
static void test_chain_equal_then_diff() {
    // Reset state
    g_cleanup_calls = 0;
    g_delete_calls = 0;
    TEST_GETPID = 11111; // first node will match this
    extern pcap_dag_t *pcap_dags;
    pcap_dags = NULL;

    // Allocate two pcap_t placeholders
    pcap_t* p1 = make_dummy_pcap();
    pcap_t* p2 = make_dummy_pcap();

    // Build chain: first node (pid == TEST_GETPID), next node (pid != TEST_GETPID)
    pcap_dag_t* first = reinterpret_cast<pcap_dag_t*>(malloc(sizeof(pcap_dag_t)));
    first->pid = TEST_GETPID;
    first->p = p1;
    first->next = NULL;

    pcap_dag_t* second = reinterpret_cast<pcap_dag_t*>(malloc(sizeof(pcap_dag_t)));
    second->pid = 999; // different
    second->p = p2;
    second->next = NULL;

    // Link: first points to second
    first->next = second;
    pcap_dags = first;

    // Run the function under test
    atexit_handler();

    // Validate expectations
    EXPECT_TRUE(g_cleanup_calls == 1, "dag_platform_cleanup should be called once for first node");
    EXPECT_TRUE(g_delete_calls == 1, "delete_pcap_dag should be called once for second node");
    EXPECT_TRUE(pcap_dags == NULL, "pcap_dags should be NULL after processing chain");

    // Cleanup
    free(p1);
    free(p2);
    free(first);
    free(second);
    pcap_dags = NULL;
}

// Simple runner
int main() {
    // Run tests
    test_equal_pid_calls_cleanup();
    test_different_pid_calls_delete();
    test_chain_equal_then_diff();

    // Report summary
    if (g_test_failures == 0 && g_cleanup_calls >= 0) {
        printf("All tests completed without assertion failures.\n");
    } else {
        printf("There were %d test failure(s).\n", g_test_failures);
    }
    return g_test_failures != 0 ? 1 : 0;
}