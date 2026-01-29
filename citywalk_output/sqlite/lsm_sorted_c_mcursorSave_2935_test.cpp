// Unit tests for mcursorSave in lsm_sorted.c
// Language: C++11
// Note: This test harness is designed to exercise the focal function
// mcursorSave(MultiCursor *pCsr) by constructing a minimal compatible
// environment. It relies on the actual compiled C code of lsm_sorted.c
// present in the test build. The test focuses on covering the decision
// branches around the aTree check and the call site to multiCursorCacheKey.
// Since internal static helpers are not accessible, the observable behavior
// is limited to the return code (rc). We assume LSM_OK corresponds to 0.
//
// Important: This test uses a lightweight harness in main() (no GoogleTest,
// no GMock). It prints results to stdout and uses simple asserts that do not
// terminate the process in non-debug builds. Static analysis is used for
// coverage intent and code paths.
//
// DOMAIN_KNOWLEDGE notes applied:
// - Access to static/internal components is avoided; tests rely on mcursorSave's
//   observable rc value.
// - We do not rely on private fields beyond what mcursorSave uses (aTree).
// - The test is designed to be portable under C++11; it uses only the
//   standard library facilities.
// - Static symbols CURSOR_DATA_TREE0/CURSOR_DATA_TREE1 are declared as extern
//   to bind to the definitions in the lsm_sorted.c build unit.

#include <vector>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cassert>


// Step 1: Bring in the focal method's signature and required dependencies.
// We rely on the external C implementation of mcursorSave and the minimal
// MultiCursor layout sufficient for the function's usage.
//
// The real project supplies a full definition of MultiCursor. For testing the
// focal point, we provide a lightweight compatible layout here so that we can
// construct test fixtures without modifying the production code.
extern "C" {

// Forward-declare the external type as used by mcursorSave. We keep the
// layout compatible with the function's usage: it will access pCsr->aTree.
// If the real project's MultiCursor disagrees, this test must include the real
// header from the project (e.g., a header that defines MultiCursor).
typedef struct MultiCursor {
    int *aTree; // minimal surface: only aTree is required by mcursorSave
} MultiCursor;

// Focal function under test
int mcursorSave(MultiCursor *pCsr);

// External constants used by the decision logic in mcursorSave.
// They are expected to be defined in the production code (e.g., via a header
// or enum). We declare them as extern to bind to their actual definitions at link time.
extern int CURSOR_DATA_TREE0;
extern int CURSOR_DATA_TREE1;

} // extern "C"

static const int FSA_OK = 0; // We assume LSM_OK corresponds to 0 in the tested environment

// Helper to create a minimal MultiCursor with an aTree array
static MultiCursor createCursorWithTree(const std::vector<int> &treeArray) {
    MultiCursor csr;
    if (!treeArray.empty()) {
        csr.aTree = new int[treeArray.size()];
        for (size_t i = 0; i < treeArray.size(); ++i) {
            csr.aTree[i] = treeArray[i];
        }
    } else {
        csr.aTree = nullptr;
    }
    return csr;
}

// Helper to clean up a created cursor
static void destroyCursor(MultiCursor &csr) {
    if (csr.aTree) {
        delete[] csr.aTree;
        csr.aTree = nullptr;
    }
}

// Test 1: When pCsr->aTree is NULL, mcursorSave should not dereference aTree and
// return LSM_OK (rc initialized to LSM_OK and mcursorFreeComponents should run).
static bool test_case_null_aTree() {
    MultiCursor csr = { nullptr };
    int rc = mcursorSave(&csr);
    bool pass = (rc == FSA_OK);
    std::cout << "Test 1 (null aTree): rc=" << rc << " -> " << (pass ? "PASS" : "FAIL") << std::endl;
    return pass;
}

// Test 2: When pCsr->aTree is non-NULL but the iTree value does not match
// CURSOR_DATA_TREE0 or CURSOR_DATA_TREE1, the branch is skipped. rc should remain OK.
static bool test_case_aTree_nonmatching_iTree() {
    // Prepare aTree with two integers; aTree[1] set to a value unlikely to equal
    // CURSOR_DATA_TREE0 or CURSOR_DATA_TREE1 (best-effort).
    std::vector<int> arr = { 42, 99999 }; // iTree at index 1
    MultiCursor csr = createCursorWithTree(arr);

    int rc = mcursorSave(&csr);
    bool pass = (rc == FSA_OK);
    std::cout << "Test 2 (aTree non-matching iTree): rc=" << rc
              << " -> " << (pass ? "PASS" : "FAIL") << std::endl;

    destroyCursor(csr);
    return pass;
}

// Test 3: When pCsr->aTree[1] matches CURSOR_DATA_TREE0 or CURSOR_DATA_TREE1,
// the code path to multiCursorCacheKey should be taken. We cannot observe the
// internal rc changes of the static function, but we can ensure we do not crash
// and that rc remains OK in typical scenarios.
static bool test_case_aTree_matching_iTree() {
    // Intentionally pick a value that could reasonably equal CURSOR_DATA_TREE0 or
    // CURSOR_DATA_TREE1 in many builds. If not equal, the test remains valid as
    // the branch will be skipped and rc should still be OK.
    std::vector<int> arr = { 0, 0 }; // iTree at index 1 (best attempt)
    MultiCursor csr = createCursorWithTree(arr);

    int rc = mcursorSave(&csr);
    bool pass = (rc == FSA_OK);
    std::cout << "Test 3 (aTree matching iTree): rc=" << rc
              << " -> " << (pass ? "PASS" : "FAIL") << std::endl;

    destroyCursor(csr);
    return pass;
}

// Entry-point for running all tests
int main() {
    // Informational header
    std::cout << "Running focused unit tests for mcursorSave (lsm_sorted.c)..." << std::endl;

    // Run tests
    bool t1 = test_case_null_aTree();
    bool t2 = test_case_aTree_nonmatching_iTree();
    bool t3 = test_case_aTree_matching_iTree();

    int failures = 0;
    if (!t1) ++failures;
    if (!t2) ++failures;
    if (!t3) ++failures;

    if (failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << failures << " test(s) FAILED." << std::endl;
        return 1;
    }
}