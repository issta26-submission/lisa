// Test suite for focal method: lsmTreeCursorKey
// This test is crafted for C/C++ interop without GoogleTest.
// It focuses on the behavior of lsmTreeCursorKey when the cursor key is already saved
// in pCsr->pSave (bypassing csrGetKey) and validates both branches of the internal
// pFlags handling (non-NULL vs NULL).

#include <lsmInt.h>
#include <cstdio>
#include <string.h>
#include <cstring>


// Bring in the C-headers with C linkage for interop with the existing C implementation.
// We rely on the real types TreeCursor, TreeKey, TreeBlob and the symbol LSM_OK.
// Ensure this header is accessible in your build environment.
extern "C" {
}

// Lightweight test harness (no external test framework)
static bool g_all_tests_passed = true;

#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::fprintf(stderr, "TEST_FAIL: %s\n", (msg)); \
        g_all_tests_passed = false; \
    } else { \
        std::fprintf(stderr, "TEST_PASS: %s\n", (msg)); \
    } \
} while(0)

// Step 1: Program Understanding (Candidate Keywords)
// - Focused method under test: lsmTreeCursorKey
// - Key dependencies: TreeCursor (cursor state), TreeKey (key representation), TreeBlob (blob state),
//   pCsr->pSave (cached key), csrGetKey (fallback path), LSM_OK (success code),
//   pointer arithmetic on TreeKey (ppKey points to the memory just after TreeKey).
// - Dependency hints (from <FOCAL_CLASS_DEP>): lsmTreeCursorValid, csrGetKey, pCsr->blob, the key fields
// - This test exercises the primary path where pCsr->pSave is non-NULL (bypasses csrGetKey),
//   ensuring pnKey, pFlags (when non-NULL), and ppKey are set correctly.
// - The domain knowledge suggests using real library types via lsmInt.h and avoiding GTest.
// - We will cover true/false branches for the pFlags != NULL predicate by providing
//   cases with pFlags non-NULL and NULL respectively.

// Step 2: Unit Test Generation
// Test 1: pCsr->pSave is non-NULL and pFlags is provided
//         Verifies pnKey, pFlags, and ppKey are correctly populated.
// Test 2: pCsr->pSave is non-NULL and pFlags is NULL
//         Verifies pnKey and ppKey are correctly populated while pFlags is intentionally not written.

static bool test_lsmTreeCursorKey_savedKey_withFlags()
{
    // This test exercises the primary path where the key is already saved in the cursor
    // (pCsr->pSave is non-NULL). It validates the values returned via pnKey, pFlags, and ppKey.

    // Prepare a sample key and cursor state
    TreeKey key;
    key.nKey = 12345;
    key.flags = 0xAB;

    // pCsr->blob is a structure in the cursor; initialize a dummy blob
    TreeBlob blob;
    std::memset(&blob, 0, sizeof(TreeBlob));

    // Initialize a TreeCursor and inject the saved key
    TreeCursor csr;
    std::memset(&csr, 0, sizeof(TreeCursor));
    csr.pSave = &key;     // Bypass csrGetKey path
    csr.blob = blob;      // blob is present but will be unused in this path

    // Outputs
    int outFlags = -1;
    void *ppKey = nullptr;
    int nKey = -1;

    // Call the function under test
    int rc = lsmTreeCursorKey(&csr, &outFlags, &ppKey, &nKey);

    // Expected: rc == LSM_OK, nKey == key.nKey, outFlags == key.flags, ppKey == (void*)&key[1]
    bool ok = true;
    ok = ok && (rc == LSM_OK);
    ok = ok && (nKey == key.nKey);
    ok = ok && (outFlags == key.flags);
    void *expected = (void *)(&key + 1);
    ok = ok && (ppKey == expected);

    TEST_ASSERT(ok, "lsmTreeCursorKey with saved key and flags should set pnKey, pFlags, and ppKey correctly");

    return ok;
}

static bool test_lsmTreeCursorKey_savedKey_withoutFlags()
{
    // This test exercises the path where pCsr->pSave is non-NULL and pFlags is NULL.
    // It ensures pnKey and ppKey are populated, while pFlags is not written.

    // Prepare a sample key and cursor state
    TreeKey key;
    key.nKey = 54321;
    key.flags = 0x5A55;

    // pCsr->blob is a structure in the cursor; initialize a dummy blob
    TreeBlob blob;
    std::memset(&blob, 0, sizeof(TreeBlob));

    // Initialize a TreeCursor and inject the saved key
    TreeCursor csr;
    std::memset(&csr, 0, sizeof(TreeCursor));
    csr.pSave = &key;     // Bypass csrGetKey path
    csr.blob = blob;      // blob is present but will be unused in this path

    // Outputs
    int outFlags = -1;
    void *ppKey = nullptr;
    int nKey = -1;

    // Call the function under test with pFlags = NULL
    int rc = lsmTreeCursorKey(&csr, nullptr, &ppKey, &nKey);

    // Expected: rc == LSM_OK, nKey == key.nKey, ppKey == (void*)&key[1]
    bool ok = true;
    ok = ok && (rc == LSM_OK);
    ok = ok && (nKey == key.nKey);
    void *expected = (void *)(&key + 1);
    ok = ok && (ppKey == expected);

    TEST_ASSERT(ok, "lsmTreeCursorKey with saved key and NULL flags should set pnKey and ppKey, ignoring pFlags");

    return ok;
}

// Step 3: Test Case Refinement
// We aggregate results and provide a simple summary in main.
// Note: The tests rely on the real lsmInt.h type definitions and on the linker
//       to provide the lsmTreeCursorKey implementation (from lsm_tree.c).

int main(void)
{
    // Run tests and report results
    bool t1 = test_lsmTreeCursorKey_savedKey_withFlags();
    bool t2 = test_lsmTreeCursorKey_savedKey_withoutFlags();

    if(!t1) std::fprintf(stderr, "Test 1 failed.\n");
    if(!t2) std::fprintf(stderr, "Test 2 failed.\n");

    if(t1 && t2) {
        std::fprintf(stderr, "ALL TESTS PASSED\n");
        return 0;
    } else {
        std::fprintf(stderr, "SOME TESTS FAILED\n");
        return 1;
    }
}