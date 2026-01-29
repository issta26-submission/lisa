// Test suite for the focal OpenOutput function in jpgicc.c
// Target environment: C++11, no Google Test, uses a lightweight custom harness.
// The test relies on the public symbol: cmsBool OpenOutput(const char* FileName)
// and the global symbol: FILE* OutFile (as declared in the production code).
// The production code uses libjpeg, so linking with libjpeg is expected.

#include <cstring>
#include <iccjpeg.h>
#include <cstdio>
#include <jpeglib.h>
#include <cstdlib>
#include <utils.h>


// Include the header that defines cmsBool if available.
// If iccjpeg.h defines cmsBool, this ensures the type matches OpenOutput's signature.

#ifdef __cplusplus
extern "C" {
#endif
    // Declaration of the focal function under test (C linkage)
    cmsBool OpenOutput(const char* FileName);

    // External global used by OpenOutput to hold the output stream.
    // We declare it here so the test can verify and close it when needed.
    FILE* OutFile;
#ifdef __cplusplus
}
#endif

// Lightweight test harness
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_ASSERT(cond, msg) do { \
    ++tests_run; \
    if (cond) { \
        ++tests_passed; \
    } else { \
        ++tests_failed; \
        fprintf(stderr, "Test failure: %s\n", msg); \
    } \
} while (0)


// Test 1: OpenOutput should succeed when given a writable path.
// This validates the positive code path (true branch) of the OpenOutput function.
// It also ensures that OutFile is a valid FILE* handle after a successful call.
static void Test_OpenOutput_Succeeds() {
    const char* fname = "./test_output_openoutput.jpg";

    // Ensure clean state
    remove(fname);

    // Act
    cmsBool ret = OpenOutput(fname);

    // Assert: function should indicate success (non-zero) and OutFile should be non-null.
    TEST_ASSERT(ret != 0, "OpenOutput should return non-zero (TRUE) for writable path");
    TEST_ASSERT(OutFile != NULL, "OutFile should be non-null after successful OpenOutput");

    // Cleanup
    if (OutFile != NULL) {
        fclose(OutFile);
        OutFile = NULL;
    }
    remove(fname);
}

// Test 2: OpenOutput should be able to reopen another output file after closing previous.
// This checks that repeated use of OpenOutput with different filenames works correctly
// and that the internal state supports successive initializations.
static void Test_OpenOutput_Reopen() {
    const char* fname1 = "./test_output_openoutput1.jpg";
    const char* fname2 = "./test_output_openoutput2.jpg";

    // Ensure clean state
    remove(fname1);
    remove(fname2);

    // First open
    cmsBool ret1 = OpenOutput(fname1);
    TEST_ASSERT(ret1 != 0, "First OpenOutput call should succeed");
    if (OutFile != NULL) {
        fclose(OutFile);
        OutFile = NULL;
    }

    // Second open with a different file
    cmsBool ret2 = OpenOutput(fname2);
    TEST_ASSERT(ret2 != 0, "Second OpenOutput call should succeed");
    if (OutFile != NULL) {
        fclose(OutFile);
        OutFile = NULL;
    }

    // Cleanup
    remove(fname1);
    remove(fname2);
}


// Entry point
int main() {
    printf("Starting OpenOutput unit tests (no GTest, simple harness)\n");

    Test_OpenOutput_Succeeds();
    Test_OpenOutput_Reopen();

    // Summary
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);

    if (tests_failed > 0) {
        printf("Some tests failed. See error messages above for details.\n");
        return 1;
    }
    printf("All tests passed.\n");
    return 0;
}