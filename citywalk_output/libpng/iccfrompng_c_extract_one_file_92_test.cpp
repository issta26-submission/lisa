// Custom test harness for the focal method extract_one_file in iccfrompng.c
// This test uses light-weight, self-contained mocks to drive the behavior
// of the external dependencies (extract(), verbose, no_profile) without GTest.
// Compile with: g++ -std=c++11 -o test_suite test_suite.cpp iccfrompng.c -lpng -lpthread (as needed)

#include <unistd.h>
#include <setjmp.h>
#include <string.h>
#include <sys/stat.h>
#include <cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <png.h>



extern "C" {
   // Declaration of the function under test (to be linked from iccfrompng.c)
   int extract_one_file(const char* filename);

   // The focal code relies on these external symbols; we provide mocks below.
   // Global verbosity flag used by iccfrompng.c
   int verbose = 0;

   // Sentinel for "no profile" as used by the focal code.
   png_bytep no_profile;
}

// Forward declaration of the mock extractor to satisfy linker
extern "C" png_bytep extract(FILE* fp, png_uint_32* proflen);

// Test-driven globals to control mock behavior
static int g_case = 0;                // 1 => HAVE PROFILE, 2 => NO_PROFILE, 3 => NULL
static png_bytep g_profile = NULL;    // memory buffer to be returned as profile
static png_uint_32 g_proflen = 0;      // length of the profile

// No-profile sentinel value (must match the same symbol used by focal code)
static png_bytep const NO_PROFILE_SENTINEL = (png_bytep)0xFFFFFFFF;

// Provide the no_profile symbol that the focal code compares against
png_bytep no_profile = NO_PROFILE_SENTINEL;

// Mock implementation of extract() used by the focal method
extern "C" png_bytep extract(FILE* /*fp*/, png_uint_32* proflen) {
    // Do not actually read the input; the test controls behavior via g_case
    *proflen = g_proflen;
    if (g_case == 1) {
        // Return a real profile buffer (must be malloc/free compatible as focal code frees it)
        return g_profile;
    } else if (g_case == 2) {
        // Signal "no profile" by returning the sentinel
        return no_profile;
    } else if (g_case == 3) {
        // Simulate NULL profile
        return NULL;
    }
    return NULL;
}

// Helper to write a small file (to simulate input PNG)
static bool write_file(const char* path, const unsigned char* data, size_t len) {
    FILE* f = fopen(path, "wb");
    if (!f) return false;
    if (fwrite(data, 1, len, f) != len) { fclose(f); return false; }
    fclose(f);
    return true;
}

// Test 1: Successful extraction and ICC file creation
// - Case: extract() returns a valid profile
// - Expect: extract_one_file returns 1 and creates input.icc with matching data
bool test_successful_write() {
    printf("Running test_successful_write...\n");

    // Prepare mock: HAVE PROFILE
    g_case = 1;
    unsigned char* mem = (unsigned char*)malloc(4);
    mem[0] = 0x11; mem[1] = 0x22; mem[2] = 0x33; mem[3] = 0x44;
    g_profile = mem;
    g_proflen = 4;

    // Prepare input file icc_test_input1.png
    const char* input = "icc_test_input1.png";
    const unsigned char inputdata[] = { 0x89, 'P', 'N', 'G' };
    write_file(input, inputdata, sizeof(inputdata));

    // Ensure any previous output is removed
    remove("icc_test_input1.icc");

    int ret = extract_one_file(input);
    bool ok = true;
    if (ret != 1) {
        ok = false;
        printf("  [FAIL] expected return 1, got %d\n", ret);
    } else {
        FILE* f = fopen("icc_test_input1.icc", "rb");
        if (!f) {
            ok = false;
            printf("  [FAIL] output ICC file not created\n");
        } else {
            unsigned char buf[4];
            size_t read = fread(buf, 1, 4, f);
            fclose(f);
            if (read != 4 || memcmp(buf, mem, 4) != 0) {
                ok = false;
                printf("  [FAIL] ICC file content mismatch\n");
            }
        }
    }

    // Cleanup
    remove(input);
    remove("icc_test_input1.icc");
    if (mem) free(mem);

    // Reset global state
    g_profile = NULL; g_proflen = 0; g_case = 0;
    printf("test_successful_write %s\n", ok ? "PASSED" : "FAILED");
    return ok;
}

// Test 2: No profile case
// - Case: extract() returns no_profile sentinel
// - Expect: extract_one_file returns 0 and no .icc file is created
bool test_no_profile() {
    printf("Running test_no_profile...\n");

    g_case = 2;
    unsigned char dummy[1] = {0};
    g_profile = dummy;
    g_proflen = 1;

    const char* input = "icc_test_input2.png";
    const unsigned char inputdata[] = { 0x89, 'P', 'N', 'G' };
    write_file(input, inputdata, sizeof(inputdata));

    // Ensure no output created
    remove("icc_test_input2.icc");

    int ret = extract_one_file(input);
    bool ok = true;
    if (ret != 0) {
        ok = false;
        printf("  [FAIL] expected return 0, got %d\n", ret);
    }
    FILE* f = fopen("icc_test_input2.icc", "rb");
    if (f) {
        // Should not create output file
        fclose(f);
        remove("icc_test_input2.icc");
        ok = false;
        printf("  [FAIL] ICC file should not be created when no profile\n");
    }

    // Cleanup
    remove(input);
    g_case = 0; g_profile = NULL; g_proflen = 0;
    printf("test_no_profile %s\n", ok ? "PASSED" : "FAILED");
    return ok;
}

// Test 3: Input file not found
// - Case: non-existent input file results in error path and 0 return
bool test_input_file_not_found() {
    printf("Running test_input_file_not_found...\n");

    g_case = 1;
    unsigned char* mem = (unsigned char*)malloc(4);
    mem[0]=1; mem[1]=2; mem[2]=3; mem[3]=4;
    g_profile = mem; g_proflen = 4;

    const char* input = "this_file_should_not_exist_404.png";
    int ret = extract_one_file(input);
    bool ok = (ret == 0);

    if (!ok) printf("  [FAIL] expected return 0 for non-existent input, got %d\n", ret);
    if (mem) free(mem);

    g_case = 0; g_profile = NULL; g_proflen = 0;
    printf("test_input_file_not_found %s\n", ok ? "PASSED" : "FAILED");
    return ok;
}

// Test 4: Output file open failure due to read-only directory
// - Case: Input exists, but output (.icc) cannot be created due to directory permissions
bool test_output_file_open_failure() {
#ifndef _WIN32
    printf("Running test_output_file_open_failure...\n");

    // Create read-only directory
    mkdir("readonly_dir", 0555);
    const char* input = "readonly_dir/input.png";
    const unsigned char inputdata[] = {0x01,0x02,0x03};
    write_file(input, inputdata, sizeof(inputdata));

    g_case = 1;
    unsigned char* mem = (unsigned char*)malloc(4);
    mem[0]=9; mem[1]=8; mem[2]=7; mem[3]=6;
    g_profile = mem; g_proflen = 4;

    int ret = extract_one_file(input);
    bool ok = (ret == 0);

    // Cleanup
    free(mem);
    remove(input);
    // Try to remove ICC file if somehow created
    remove("readonly_dir/input.icc");
    rmdir("readonly_dir");
    g_case = 0; g_profile = NULL; g_proflen = 0;

    printf("test_output_file_open_failure %s\n", ok ? "PASSED" : "FAILED");
    return ok;
#else
    // Skipping on Windows due to permission handling differences
    printf("test_output_file_open_failure skipped on Windows\n");
    return true;
#endif
}

// Simple runner
int main() {
    printf("Starting test suite for extract_one_file with custom mocks\n");
    int total = 0, failed = 0;

    if (test_successful_write()) { /* PASS */ } else { failed++; }
    total++;

    if (test_no_profile()) { /* PASS */ } else { failed++; }
    total++;

    if (test_input_file_not_found()) { /* PASS */ } else { failed++; }
    total++;

    if (test_output_file_open_failure()) { /* PASS */ } else { failed++; }
    total++;

    printf("Summary: %d/%d tests passed\n", total - failed, total);
    return failed;
}