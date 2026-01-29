// High-quality C++11 unit tests for the focal method ReadNumbers in cmscgats.c
// No Google Test is used. A small, self-contained test harness is provided.
// The tests rely on the real LittleCMS internal helpers by creating a cmsIT8
// object, feeding a temporary file as the input source, seeding the first
// token, and invoking ReadNumbers. Each test explains its purpose via comments.

#include <cstring>
#include <cstdio>
#include <limits>
#include <vector>
#include <iostream>
#include <string>
#include <cassert>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cmath>


// Include LittleCMS internal headers to access cmsIT8, ReadNumbers, InSymbol, etc.
// It is expected that the environment provides these headers and a build
// environment capable of linking cmscgats.c with the test.
extern "C" {
}

// Forward declarations are provided by the internal header; use them directly.

static int g_failures = 0;

// Lightweight assertion helpers that do not terminate the test run.
#define EXPECT_TRUE(cond) \
    do { if (!(cond)) { std::cerr << "Expectation failed: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; ++g_failures; } } while (0)

#define EXPECT_EQ(a, b) \
    do { if (!((a) == (b))) { std::cerr << "Equality failed: " #a " == " #b " (" << (a) << " != " << (b) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; ++g_failures; } } while (0)

#define EXPECT_NEAR(a, b, tol) \
    do { if (std::fabs((a) - (b)) > (tol)) { std::cerr << "Nearby check failed: " #a " vs " #b " (" << (a) << " != " << (b) << "), tol=" << (tol) << " at " << __FILE__ << ":" << __LINE__ << "\n"; ++g_failures; } } while (0)

// Helper to create a unique temporary file path.
// Uses tmpnam for simplicity in test environments; should be safe for unit tests.
static std::string make_temp_file(const std::string& content) {
    char tmpName[L_tmpnam];
    if (tmpnam(tmpName) == nullptr) {
        throw std::runtime_error("Failed to generate temp file name");
    }
    std::string path = std::string(tmpName);
    FILE* f = std::fopen(path.c_str(), "wt");
    if (!f) {
        throw std::runtime_error("Failed to create temp file");
    }
    if (!content.empty()) {
        std::fwrite(content.data(), 1, content.size(), f);
    }
    std::fclose(f);
    return path;
}

// Delete temporary file if needed
static void remove_file(const std::string& path) {
    std::remove(path.c_str());
}

// Test 1: Basic SINUM path. Seed first value as 1, supply remaining tokens "2 3" in file.
// Expected arr = {1, 2, 3}
static void test_ReadNumbers_SINUM_basic() {
    std::cout << "[TEST] ReadNumbers with SINUM tokens (basic case)" << std::endl;

    // Prepare input: remaining tokens after the seed value
    std::string content = "2 3\n";
    std::string fname = make_temp_file(content);

    cmsIT8* cube = cmsIT8Alloc(NULL);
    EXPECT_TRUE(cube != NULL);

    // Seed first number as INUM = 1
    cube->IsCUBE = 1;             // crude boolean, as in the original code TRUE
    cube->sy = SINUM;              // first token is integer
    cube->inum = 1;                 // seed value for arr[0]

    // Open the input stream
    cube->FileStack[0]->Stream = fopen(fname.c_str(), "rt");
    EXPECT_TRUE(cube->FileStack[0]->Stream != NULL);

    cmsFloat64Number arr[3];
    cmsBool ok = ReadNumbers(cube, 3, arr);
    EXPECT_TRUE(ok);
    EXPECT_EQ(arr[0], 1.0);
    EXPECT_EQ(arr[1], 2.0);
    EXPECT_EQ(arr[2], 3.0);

    // Cleanup
    fclose(cube->FileStack[0]->Stream);
    cmsIT8Free((cmsHANDLE) cube);
    remove_file(fname);
    std::cout << "[TEST] Completed: ReadNumbers SINUM basic case" << std::endl;
}

// Test 2: Basic SDNUM path. Seed first value as 1 (SINUM), file contains "2.5 3.25".
// Expected arr = {1, 2.5, 3.25}
static void test_ReadNumbers_SDNUM_basic() {
    std::cout << "[TEST] ReadNumbers with SDNUM tokens (float path)" << std::endl;

    std::string content = "2.5 3.25\n";
    std::string fname = make_temp_file(content);

    cmsIT8* cube = cmsIT8Alloc(NULL);
    EXPECT_TRUE(cube != NULL);

    cube->IsCUBE = 1;
    cube->sy = SINUM;
    cube->inum = 1; // seed first value

    cube->FileStack[0]->Stream = fopen(fname.c_str(), "rt");
    EXPECT_TRUE(cube->FileStack[0]->Stream != NULL);

    cmsFloat64Number arr[3];
    cmsBool ok = ReadNumbers(cube, 3, arr);
    EXPECT_TRUE(ok);
    EXPECT_EQ(arr[0], 1.0);
    EXPECT_NEAR(arr[1], 2.5, 1e-12);
    EXPECT_NEAR(arr[2], 3.25, 1e-12);

    fclose(cube->FileStack[0]->Stream);
    cmsIT8Free((cmsHANDLE) cube);
    remove_file(fname);
    std::cout << "[TEST] Completed: ReadNumbers SDNUM basic" << std::endl;
}

// Test 3: Mixed SINUM/SDNUM path. Seed first as 1 (SINUM), file tokens "2.0 3".
// Expected arr = {1, 2.0, 3.0}
static void test_ReadNumbers_mixed_SINUM_SDNUM() {
    std::cout << "[TEST] ReadNumbers with mixed SINUM/SDNUM tokens" << std::endl;

    std::string content = "2.0 3\n";
    std::string fname = make_temp_file(content);

    cmsIT8* cube = cmsIT8Alloc(NULL);
    EXPECT_TRUE(cube != NULL);

    cube->IsCUBE = 1;
    cube->sy = SINUM;
    cube->inum = 1; // seed first

    cube->FileStack[0]->Stream = fopen(fname.c_str(), "rt");
    EXPECT_TRUE(cube->FileStack[0]->Stream != NULL);

    cmsFloat64Number arr[3];
    cmsBool ok = ReadNumbers(cube, 3, arr);
    EXPECT_TRUE(ok);
    EXPECT_EQ(arr[0], 1.0);
    EXPECT_NEAR(arr[1], 2.0, 1e-12);
    EXPECT_NEAR(arr[2], 3.0, 1e-12);

    fclose(cube->FileStack[0]->Stream);
    cmsIT8Free((cmsHANDLE) cube);
    remove_file(fname);
    std::cout << "[TEST] Completed: ReadNumbers mixed SINUM/SDNUM" << std::endl;
}

// Test 4: Error path. Set the current symbol to an invalid value (not SINUM or SDNUM) to trigger SynError.
// We do not rely on any file contents for this path; the first iteration should fail.
static void test_ReadNumbers_error_path() {
    std::cout << "[TEST] ReadNumbers error path when sy is invalid" << std::endl;

    // No need to create a file for this test; InSymbol won't be called because the
    // first branch checks cube->sy and should fail immediately.
    cmsIT8* cube = cmsIT8Alloc(NULL);
    EXPECT_TRUE(cube != NULL);

    cube->IsCUBE = 1;
    cube->sy = 9999; // invalid symbol value (not SINUM/SDNUM)

    cmsFloat64Number arr[2];
    cmsBool ok = ReadNumbers(cube, 2, arr);
    // Expect failure (SynError returns a false-like value)
    EXPECT_TRUE(!ok);

    cmsIT8Free((cmsHANDLE) cube);
    std::cout << "[TEST] Completed: ReadNumbers error path" << std::endl;
}

// Test 5: Edge case n = 0. Ensure that the function handles zero-length input gracefully
// and returns a success (i.e., CheckEOLN passes on empty input).
static void test_ReadNumbers_zero_n() {
    std::cout << "[TEST] ReadNumbers with n = 0" << std::endl;

    // Empty file is fine
    std::string fname = make_temp_file("");

    cmsIT8* cube = cmsIT8Alloc(NULL);
    EXPECT_TRUE(cube != NULL);

    cube->IsCUBE = 1;
    // Do not seed any number since n == 0
    cube->sy = SINUM; // arbitrary

    cube->FileStack[0]->Stream = fopen(fname.c_str(), "rt");
    EXPECT_TRUE(cube->FileStack[0]->Stream != NULL);

    cmsFloat64Number arr[1];
    cmsBool ok = ReadNumbers(cube, 0, arr);
    EXPECT_TRUE(ok);

    fclose(cube->FileStack[0]->Stream);
    cmsIT8Free((cmsHANDLE) cube);
    remove_file(fname);
    std::cout << "[TEST] Completed: ReadNumbers zero-length input" << std::endl;
}

// Main function to run tests
int main() {
    // Run all tests; they print their own progress.
    test_ReadNumbers_SINUM_basic();
    test_ReadNumbers_SDNUM_basic();
    test_ReadNumbers_mixed_SINUM_SDNUM();
    test_ReadNumbers_error_path();
    test_ReadNumbers_zero_n();

    if (g_failures == 0) {
        std::cout << "\nALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "\n" << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}