// Minimal C++11 test suite for the focal method: unzGoToFirstFile (in unzip.c)
// This test avoids GTest and uses a lightweight, non-terminating assertion scheme.
// It is designed to be compiled together with the existing unzip.c and unzip.h
// by the build system that hosts the focal code.
// The tests focus on the behavior described by the focal method and its related struct.

#include <cstdio>
#include <cstring>
#include <string.h>
#include <stdio.h>
#include <crypt.h>
#include <zlib.h>
#include <stdlib.h>
#include <unzip.h>
#include <cstdlib>


// Include C headers with C linkage to ensure proper name resolution.
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Lightweight test harness (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

// Non-terminating assertion: compare two integers
#define ASSERT_EQ_INT(actual, expected, msg) do {             \
    ++g_total;                                                  \
    if ((actual) != (expected)) {                               \
        ++g_failed;                                             \
        printf("TEST FAIL: %s | expected: %d, actual: %d\n", msg, (expected), (actual)); \
    } else {                                                     \
        printf("TEST PASS: %s\n", msg);                         \
    }                                                           \
} while (0)

// Test 1: unzGoToFirstFile(NULL) should return UNZ_PARAMERROR
// This validates the function's NULL parameter check path.
void test_unzGoToFirstFile_null_file_returns_paramerror() {
    // Domain-specific explanation:
    // - Call with a NULL file should not dereference and must return UNZ_PARAMERROR.
    // - This confirms basic input validation in the focal method.
    int result = unzGoToFirstFile(NULL);

    ASSERT_EQ_INT(result, UNZ_PARAMERROR, "unzGoToFirstFile(NULL) should return UNZ_PARAMERROR");
}

// Test 2: When provided with a valid (non-NULL) unz64_s instance,
// ensure key state transitions occur as described by the focal method.
//
// Expected behaviors asserted deterministically:
// - pos_in_central_dir is set to offset_central_dir
// - num_file is reset to 0
// - current_file_ok reflects whether the internal "GetCurrentFileInfoInternal" call
//   returned UNZ_OK (current_file_ok == 1) or not (current_file_ok == 0)
// - The function returns the value produced by unz64local_GetCurrentFileInfoInternal
//
void test_unzGoToFirstFile_basic_state_changes() {
    // Allocate and zero-initialize a fictitious ZIP context structure.
    // In the real library, unz64_s holds the central directory state and current file info.
    unz64_s* s = new unz64_s();
    std::memset(s, 0, sizeof(unz64_s));

    // Initialize fields that the focal method touches.
    // offset_central_dir determines the starting position of the central directory.
    s->offset_central_dir = 12345;  // arbitrary non-zero value
    s->pos_in_central_dir = 0;       // will be overwritten by the method
    s->num_file = 999;               // will be reset by the method

    // Call the focal method with a non-NULL file handle.
    int result = unzGoToFirstFile((unzFile)s);

    // After the call:
    // - pos_in_central_dir should equal offset_central_dir
    // - num_file should be reset to 0
    // - current_file_ok is set to 1 if result == UNZ_OK, otherwise 0
    // - result should equal the return value of unz64local_GetCurrentFileInfoInternal
    ASSERT_EQ_INT((int)s->pos_in_central_dir, (int)s->offset_central_dir,
                  "pos_in_central_dir should be set to offset_central_dir");
    ASSERT_EQ_INT((int)s->num_file, 0,
                  "num_file should be reset to 0");
    int expected_current_ok = (result == UNZ_OK) ? 1 : 0;
    ASSERT_EQ_INT((int)s->current_file_ok, expected_current_ok,
                  "current_file_ok should reflect whether internal call returned UNZ_OK");
    // We don't assert on 'result' beyond the relation above, since internal behavior
    // may vary depending on the stubbed internal function. The deterministic part
    // is the state mutation described by the focal method.

    delete s;
}

// Entry point: run all tests and report summary
int main() {
    printf("Starting tests for unzGoToFirstFile...\n\n");

    test_unzGoToFirstFile_null_file_returns_paramerror();
    test_unzGoToFirstFile_basic_state_changes();

    printf("\nTest summary: total=%d, failed=%d\n", g_total, g_failed);

    // Return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}