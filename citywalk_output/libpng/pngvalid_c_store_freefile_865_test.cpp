// Self-contained C++11 test harness for the focal method: store_freefile
// Note: This test reproduces a minimal, self-contained environment to
// validate the control-flow of the focal function as described in the prompt.
// It does not require the external pngvalid.c sources to be present.
// The real project should be tested with its actual headers and linkage.
// Explanations are provided for each test case in comments.

#include <signal.h>
#include <string.h>
#include <float.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Minimal reproductions of the needed types to drive the focal method.
// This mirrors the layout used by store_freefile in the provided snippet.

struct png_store_buffer {
    int dummy; // sentinel field to simulate real buffer content
};

struct png_store_file {
    png_store_file *next;
    png_store_buffer data;
    size_t datacount;
    void *palette;      // simulate a dynamically allocated palette
    size_t npalette;
};

// Forward declarations to mimic the original separation of concerns.
// In the real project, these would come from the corresponding headers.
/* extern "C" */ void store_freebuffer(png_store_buffer *psb); // frees a store buffer
/* extern "C" */ void store_freefile(png_store_file **ppf);     // focal method under test

// Global counter to verify that store_freebuffer is invoked for each node.
static int freebuffer_calls = 0;

// Minimal stand-in for the real store_freebuffer to observe recursion behavior.
// In the real project, this would be provided by the library; here we count calls.
void store_freebuffer(png_store_buffer *psb)
{
    (void)psb; // suppress unused warning in debug builds
    ++freebuffer_calls;
    // In a real scenario, memory used by the buffer would be freed here.
}

// Focal method under test (reproduced in this test harness with C linkage).
// This mirrors the exact control-flow of the provided snippet.
extern "C" void store_freefile(png_store_file **ppf)
{
    if (*ppf != NULL)
    {
        store_freefile(&(*ppf)->next);
        store_freebuffer(&(*ppf)->data);
        (*ppf)->datacount = 0;
        if ((*ppf)->palette != NULL)
        {
            free((*ppf)->palette);
            (*ppf)->palette = NULL;
            (*ppf)->npalette = 0;
        }
        free(*ppf);
        *ppf = NULL;
    }
}

// Simple test harness helpers
#define TEST_ASSERT(cond, msg)                                  \
    do {                                                          \
        if (!(cond)) {                                            \
            std::cerr << "ASSERTION FAILED: " << (msg) << "\n";  \
            return false;                                         \
        }                                                         \
    } while (0)

#define TEST_PRINT_PASS(name)  \
    do { std::cout << "[PASS] " << (name) << "\n"; } while (0)
#define TEST_PRINT_FAIL(name)  \
    do { std::cout << "[FAIL] " << (name) << "\n"; } while (0)

// Test 1: Null input should be a no-op (no crashes, no recursion).
bool test_null_input()
{
    const char* test_name = "test_null_input";
    png_store_file *root = NULL;
    png_store_file **ppf = &root;

    freebuffer_calls = 0;
    store_freefile(ppf);

    // Expect no changes: root remains NULL, no buffer freed (count remains 0)
    TEST_ASSERT(root == NULL, "root should remain NULL after calling with NULL input");
    TEST_ASSERT(freebuffer_calls == 0, "store_freebuffer should not be called when input is NULL");

    TEST_PRINT_PASS(test_name);
    return true;
}

// Test 2: Single node chain with no palette.
// The function should call store_freebuffer once, set datacount to 0, free the node, and set ppf to NULL.
bool test_single_node_no_palette()
{
    const char* test_name = "test_single_node_no_palette";

    // Allocate a single node
    png_store_file *node = (png_store_file*)malloc(sizeof(png_store_file));
    std::memset(node, 0, sizeof(png_store_file));
    node->next = NULL;
    node->data.dummy = 42;
    node->datacount = 123;
    node->palette = NULL;
    node->npalette = 0;

    png_store_file **ppf = &node; // pointer to the single node
    freebuffer_calls = 0;

    store_freefile(ppf);

    // After the call, the node should be freed and the root pointer set to NULL
    TEST_ASSERT(*ppf == NULL, "ppf should be NULL after freeing the single node");
    TEST_ASSERT(freebuffer_calls == 1, "store_freebuffer should be called exactly once for the single node");

    // Note: We cannot inspect the freed memory, but we can ensure no crash and correct pointer state.

    TEST_PRINT_PASS(test_name);
    return true;
}

// Test 3: Chain of two nodes with a non-NULL palette in the second node.
// Verifies recursive freeing and proper freeing of buffers for each node.
// We also allocate a palette in the second node to exercise the palette-free path.
bool test_chain_two_nodes_with_palette()
{
    const char* test_name = "test_chain_two_nodes_with_palette";

    // Node 2 (last)
    png_store_file *node2 = (png_store_file*)malloc(sizeof(png_store_file));
    std::memset(node2, 0, sizeof(png_store_file));
    node2->next = NULL;
    node2->data.dummy = 2;
    node2->datacount = 7;

    // Palette allocated for node2
    node2->palette = malloc(16); // non-NULL to trigger palette free path
    node2->npalette = 2;

    // Node 1 (first)
    png_store_file *node1 = (png_store_file*)malloc(sizeof(png_store_file));
    std::memset(node1, 0, sizeof(png_store_file));
    node1->next = node2;
    node1->data.dummy = 3;
    node1->datacount = 9;
    node1->palette = NULL;
    node1->npalette = 0;

    png_store_file **ppf = &node1;
    freebuffer_calls = 0;

    store_freefile(ppf);

    // Both nodes should be freed and the top-level pointer NULL
    TEST_ASSERT(*ppf == NULL, "ppf should be NULL after freeing the two-node chain");
    TEST_ASSERT(freebuffer_calls == 2, "store_freebuffer should be called for each node in the chain (2)");

    // Palette in node2 should have been freed by the focal method.
    // We can't verify after-free memory, but we ensure no crash and correct call count.

    TEST_PRINT_PASS(test_name);
    return true;
}

// Main driver: run all tests and report overall result.
int main(void)
{
    bool ok = true;

    ok &= test_null_input();
    ok &= test_single_node_no_palette();
    ok &= test_chain_two_nodes_with_palette();

    if (ok) {
        std::cout << "[OVERALL PASS] All tests succeeded.\n";
        return 0;
    } else {
        std::cout << "[OVERALL FAIL] One or more tests failed.\n";
        return 1;
    }
}