/*
Unit test suite for the focal method:
  u8 *lsmFsPageData(Page *pPage, int *pnData)

Objective:
- Validate behavior of the function under true/false branches of the conditional (pnData != NULL).
- Ensure return value equals pPage->aData and that *pnData correctly copies pPage->nData when provided.
- Use a lightweight C++11 test harness without relying on Google Test.
- Provide explanatory comments for each test case.

Notes:
- We rely on the project's existing headers (lsmInt.h) to obtain the Page type.
- The function is C linkage; we declare it with C linkage in the test to avoid name mangling.
- Static helpers are implemented to record and display non-terminating test results.
*/

#include <sys/types.h>
#include <lsmInt.h>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstdlib>
#include <cstdint>



// Declare the focal function with C linkage to match the C implementation.
// This avoids C++ name mangling issues when linking with the C source.
#ifdef __cplusplus
extern "C" {
#endif
unsigned char *lsmFsPageData(Page *pPage, int *pnData);
#ifdef __cplusplus
}
#endif

// Global failure counter to accumulate non-terminating test results.
static int g_failures = 0;

// Lightweight non-terminating assertion helpers.
// They print results but do not abort the test run, enabling thorough coverage.

static void expect_ptr_eq(void* a, void* b, const char* msg) {
    if (a != b) {
        fprintf(stderr, "EXPECT_FAIL: %s: got %p, expected %p\n", msg, a, b);
        ++g_failures;
    } else {
        printf("EXPECT_PASS: %s\n", msg);
    }
}

static void expect_int_eq(int a, int b, const char* msg) {
    if (a != b) {
        fprintf(stderr, "EXPECT_FAIL: %s: got %d, expected %d\n", msg, a, b);
        ++g_failures;
    } else {
        printf("EXPECT_PASS: %s\n", msg);
    }
}

// Test 1: pnData is non-null; verify that *pnData is written and the return value equals aData.
static bool test_lsmFsPageData_nonNullPnData() {
    Page p;
    unsigned char buf[16];
    p.aData = buf;
    p.nData = 12345;

    int out;
    unsigned char *ret = lsmFsPageData(&p, &out);

    expect_ptr_eq((void*)ret, (void*)buf, "Return pointer should equal pPage->aData when pnData is non-null");
    expect_int_eq(out, p.nData, "pnData should be set to pPage->nData when pnData is non-null");

    return true;
}

// Test 2: pnData is null; ensure function returns aData and does not attempt to write via pnData.
static bool test_lsmFsPageData_nullPnData() {
    Page p;
    unsigned char buf[8];
    p.aData = buf;
    p.nData = -7;

    unsigned char *ret = lsmFsPageData(&p, nullptr);

    expect_ptr_eq((void*)ret, (void*)buf, "Return pointer should equal pPage->aData when pnData is null");
    // No access to *pnData occurs since pnData is null; test passes if no crash occurs and return matches.
    return true;
}

// Test 3: aData is null; verify that the function returns nullptr and still writes pnData when provided.
static bool test_lsmFsPageData_nullAData() {
    Page p;
    p.aData = nullptr;
    p.nData = 55;

    int out;
    unsigned char *ret = lsmFsPageData(&p, &out);

    expect_ptr_eq((void*)ret, nullptr, "Return pointer should be nullptr when pPage->aData is null");
    expect_int_eq(out, p.nData, "pnData should be set to pPage->nData even if aData is null");

    return true;
}

// Entry point and harness
int main() {
    printf("Starting tests for lsmFsPageData...\n");

    // Execute tests (true indicates test ran; result is captured via expectations)
    test_lsmFsPageData_nonNullPnData();
    test_lsmFsPageData_nullPnData();
    test_lsmFsPageData_nullAData();

    // Summary
    if (g_failures == 0) {
        printf("ALL TESTS PASSED: lsmFsPageData behavior is correct.\n");
    } else {
        printf("TESTS COMPLETED with %d failure(s).\n", g_failures);
    }

    // Return nonzero if any test failed
    return g_failures ? 1 : 0;
}