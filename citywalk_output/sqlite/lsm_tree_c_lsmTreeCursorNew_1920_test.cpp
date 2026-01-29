// Test suite for lsmTreeCursorNew in lsm_tree.c (C code) using a C++11 test harness.
// - We override the allocator lsmMalloc via a test-provided function to simulate both success and OOM scenarios.
// - We compile this test together with lsm_tree.c and the project headers (lsmInt.h).
// - This test does not use GoogleTest; it uses a minimal main() with simple checks and explanatory comments.

#include <string.h>
#include <lsmInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Import C headers in a C++ translation unit
extern "C" {
}

// Forward declaration for the allocator override.
// We will override the real lsmMalloc used by lsm_tree.c by providing our own
// implementation in this translation unit. The linker will resolve to this
// implementation when lsm_tree.c calls lsmMalloc.
static bool g_forceOOM = false; // Global flag to control allocation success/failure

extern "C" void* lsmMalloc(void* pEnv, size_t nByte) {
    (void)pEnv; // pEnv is unused in our test allocator
    if (g_forceOOM) return NULL; // Simulate allocation failure
    return malloc(nByte);      // Normal path
}

// Test 1: Verify that lsmTreeCursorNew succeeds when memory allocation succeeds.
// - Setup a minimal lsm_env and lsm_db with a valid pEnv.
// - Call lsmTreeCursorNew and ensure it returns LSM_OK and a non-NULL cursor.
// - Clean up resources by destroying the cursor.
static void test_lsmTreeCursorNew_success() {
    std::cout << "Test 1: lsmTreeCursorNew succeeds when allocation succeeds.\n";

    // Simulate successful allocation
    g_forceOOM = false;

    // Create a minimal environment object (opaque in tests)
    lsm_env* env = (lsm_env*)malloc(sizeof(lsm_env));
    if (!env) {
        std::cerr << "Test 1: Failed to allocate lsm_env\n";
        std::exit(EXIT_FAILURE);
    }

    // Prepare a db instance with the environment
    lsm_db db;
    std::memset(&db, 0, sizeof(db)); // Initialize to a known state
    db.pEnv = env;

    TreeCursor *pCsr = NULL;
    int rc = lsmTreeCursorNew(&db, 0, &pCsr); // bOld = 0
    if (rc != LSM_OK) {
        std::cerr << "Test 1: Unexpected rc: " << rc << "\n";
        std::free(env);
        std::exit(EXIT_FAILURE);
    }
    if (pCsr == NULL) {
        std::cerr << "Test 1: pCsr is NULL on success path\n";
        std::free(env);
        std::exit(EXIT_FAILURE);
    }

    // Destroy the cursor and clean up
    lsmTreeCursorDestroy(pCsr);
    // Do not call free(pCsr) explicitly; lsmTreeCursorDestroy is responsible for freeing
    std::free(env);

    std::cout << "Test 1: Passed.\n";
}

// Test 2: Verify that lsmTreeCursorNew returns NOMEM_BKPT when allocation fails (OOM path).
// - Force the allocator to fail via the test flag.
// - Ensure that the function returns LSM_NOMEM_BKPT and that ppCsr remains NULL.
static void test_lsmTreeCursorNew_oom() {
    std::cout << "Test 2: lsmTreeCursorNew returns NOMEM_BKPT when allocation fails (OOM).\n";

    // Simulate allocation failure
    g_forceOOM = true;

    // Create environment
    lsm_env* env = (lsm_env*)malloc(sizeof(lsm_env));
    if (!env) {
        std::cerr << "Test 2: Failed to allocate lsm_env\n";
        std::exit(EXIT_FAILURE);
    }

    lsm_db db;
    std::memset(&db, 0, sizeof(db));
    db.pEnv = env;

    TreeCursor *pCsr = NULL;
    int rc = lsmTreeCursorNew(&db, 1, &pCsr); // bOld = 1
    if (rc != LSM_NOMEM_BKPT) {
        std::cerr << "Test 2: Expected LSM_NOMEM_BKPT, got " << rc << "\n";
        std::free(env);
        std::exit(EXIT_FAILURE);
    }
    if (pCsr != NULL) {
        std::cerr << "Test 2: pCsr should be NULL on allocation failure.\n";
        std::free(env);
        std::exit(EXIT_FAILURE);
    }

    std::free(env);
    std::cout << "Test 2: Passed.\n";
}

int main() {
    // Run tests with explanations commented above each test.
    test_lsmTreeCursorNew_success();
    test_lsmTreeCursorNew_oom();

    std::cout << "All tests completed.\n";
    return EXIT_SUCCESS;
}