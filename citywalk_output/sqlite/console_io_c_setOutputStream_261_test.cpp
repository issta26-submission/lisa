/***** Unit tests for the focal method: setOutputStream *****
   - FOCAL_METHOD: FILE *setOutputStream(FILE *pf) { return designateEmitStream(pf, 1); }
   - The tests are designed as plain C++11 test harnesses (no GoogleTest) that link against the console_io.c translation unit.
   - We assume setOutputStream delegates to an internal static function designateEmitStream and returns the same FILE* passed in (a common design for "designate" helpers).
   - The tests cover typical valid inputs (stdout, tmpfile streams) and verify pointer identity (pf passed through).
   - Note: If the internal implementation changes, adapt these expectations accordingly.

#include <cstdlib>
#include <cstdio>


#ifdef __cplusplus
extern "C" {
#endif
// Declared here to allow linking against the C implementation in console_io.c
FILE* setOutputStream(FILE* pf);
#ifdef __cplusplus
}
#endif

// Lightweight non-terminating test harness helpers
static int assert_ptr_equal(FILE* a, FILE* b, const char* testName) {
    if (a == b) {
        printf("[PASS] %s: pointers are equal (%p)\n", testName, (void*)a);
        return 0;
    } else {
        printf("[FAIL] %s: pointers differ. expected %p, got %p\n",
               testName, (void*)a, (void*)b);
        return 1;
    }
}

/*
 Test 1: setOutputStream(stdout) should return the same pointer (pf) if the implementation delegates
 to designateEmitStream with the same stream.
 Rationale: A typical design is to return the designated stream unchanged when the chix (1) selects stdout-like behavior.
*/
static int test_setOutputStream_stdout_returns_same_pointer() {
    FILE* pf = stdout;
    FILE* res = setOutputStream(pf);
    const char* testName = "setOutputStream(stdout) returns stdout pointer";
    int fail = assert_ptr_equal(res, pf, testName);
    return fail;
}

/*
 Test 2: setOutputStream should work consistently for separate valid FILE* streams.
 - Create two independent tmpfile streams.
 - Each should be returned unchanged by setOutputStream.
 Rationale: Ensures the function delegates correctly for distinct streams and does not cache or alias results.
*/
static int test_setOutputStream_tmpfiles_return_same_pointer() {
    FILE* f1 = tmpfile();
    if (f1 == NULL) {
        printf("[WARN] test_setOutputStream_tmpfiles_return_same_pointer: tmpfile() failed to create f1. Skipping test.\n");
        return 0; // skip rather than fail
    }
    FILE* f2 = tmpfile();
    if (f2 == NULL) {
        fclose(f1);
        printf("[WARN] test_setOutputStream_tmpfiles_return_same_pointer: tmpfile() failed to create f2. Skipping test.\n");
        return 0; // skip
    }

    FILE* r1 = setOutputStream(f1);
    FILE* r2 = setOutputStream(f2);

    int fail1 = assert_ptr_equal(r1, f1, "setOutputStream(tmpfile1) returns same pointer");
    int fail2 = assert_ptr_equal(r2, f2, "setOutputStream(tmpfile2) returns same pointer");

    fclose(f1);
    fclose(f2);

    return (fail1 ? 1 : 0) + (fail2 ? 1 : 0);
}

/*
 Optional: Run all tests and report summary.
*/
static int run_all_tests() {
    int totalFailures = 0;

    totalFailures += test_setOutputStream_stdout_returns_same_pointer();
    totalFailures += test_setOutputStream_tmpfiles_return_same_pointer();

    if (totalFailures == 0) {
        printf("[SUMMARY] All tests passed.\n");
    } else {
        printf("[SUMMARY] Total failures: %d\n", totalFailures);
    }
    return totalFailures;
}

int main(void) {
    // Execute test suite
    int failures = run_all_tests();
    // Return non-zero if any test failed to indicate failure to test harness
    return (failures == 0) ? 0 : 1;
}