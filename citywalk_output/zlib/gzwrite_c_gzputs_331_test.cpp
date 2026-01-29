// Test suite for gzputs (from gzwrite.c) using a lightweight C++11 test harness.
// This suite uses the public zlib/gzip API (gzopen, gzputs, gzread, gzclose) to exercise
// the behavior of gzputs in realistic usage, and validates error handling for NULL input
// and invalid file states without terminating the test run on failure.
// The tests avoid GTest and rely on non-terminating assertions (logging failures and continuing).

/*
Structure and rationale (Step 1: Program Understanding)
- Focal method: gzputs(gzFile file, const char *s)
  - Behavior:
    - If file is NULL -> return -1
    - Retrieve internal state from file; if mode != GZ_WRITE or err != Z_OK -> return -1
    - Compute len = strlen(s); verify it fits in int
    - Call gz_write(state, s, len) and return len on success, or -1 on failure
- Core dependent components to reflect in tests (Candidate Keywords):
  - gzFile, gz_statep, GZ_WRITE, Z_OK, Z_STREAM_ERROR, strlen, gz_write
  - Error signaling via gz_error, len checks, and the reliance on gzip write path
- Class/Dependency context (FOCAL_CLASS_DEP) highlights internal state handling, memory management,
  and write paths that gzputs delegates to; unit tests target behavior via the public API.
*/

/*
Step 2: Unit Test Generation
- We create tests that rely on the public API gzopen/gzputs/gzread/gzclose:
  1) test_gzputs_null_file: passing a NULL file should yield -1 (boundary check)
  2) test_gzputs_write_and_read_back: write a known string via gzputs and read it back with gzread to verify correctness
  3) test_gzputs_invalid_mode_write_only: open a file in read-only mode and ensure gzputs returns -1
- These tests aim to cover true/false branches where possible using the public API, while keeping
  tests executable in a standard C++11 environment without GTest.
- All tests use a lightweight non-terminating assertion approach (log failures but continue).
- Explanatory comments accompany each unit test.

Step 3: Test Case Refinement
- The tests focus on practical usage scenarios that exercise the end-to-end path of gzputs:
  - NULL file handling
  - Normal write path via gzputs followed by decompression verification
  - Misuse path via read-only mode to ensure error handling
- We avoid requiring internal/private function mocking (due to static linkage in the original code)
  and rely on the public API exposed by zlib/minizip.
*/

// Include necessary headers
#include <cstdio>
#include <cstring>
#include <stdarg.h>
#include <string>
#include <iostream>
#include <gzguts.h>
#include <zlib.h>


// Lightweight test harness (non-terminating assertions)
static int tests_run = 0;
static int tests_passed = 0;

static void log_pass(const std::string& name) {
    std::cout << "[PASS] " << name << std::endl;
    ++tests_run;
    ++tests_passed;
}

static void log_fail(const std::string& name, const std::string& reason) {
    std::cerr << "[FAIL] " << name << " - " << reason << std::endl;
    ++tests_run;
}

// Convenience assertion helpers
#define EXPECT_TRUE(cond, name) do { \
    if (cond) log_pass(name); else log_fail(name, "condition failed: " #cond); \
} while (0)

#define EXPECT_EQUAL(a, b, name) do { \
    if ((a) == (b)) log_pass(name); \
    else log_fail(name, "expected " #a " == " #b " (" + std::to_string((a)) + " != " + std::to_string((b)) + ")"); \
} while (0)


// Test 1: gzputs should return -1 when given a NULL file
static bool test_gzputs_null_file() {
    const char *test_str = "dummy";
    int ret = gzputs(nullptr, test_str); // boundary condition
    if (ret == -1) {
        log_pass("test_gzputs_null_file");
        return true;
    } else {
        log_fail("test_gzputs_null_file", "gzputs(NULL, ...) did not return -1");
        return false;
    }
}

// Test 2: Normal write via gzputs and subsequent read-back to verify data integrity
static bool test_gzputs_write_and_read_back() {
    const char *path = "test_gzputs_write.gz";
    const std::string payload = "This is a test string for gzputs.";
    
    // Write phase
    gzFile f = gzopen(path, "wb");
    if (f == NULL) {
        log_fail("test_gzputs_write_and_read_back", "gzopen for writing failed");
        return false;
    }
    int written = gzputs(f, payload.c_str());
    if (written != (int)payload.size()) {
        log_fail("test_gzputs_write_and_read_back", "gzputs did not report correct length written");
        gzclose(f);
        remove(path);
        return false;
    }
    gzclose(f);

    // Readback phase
    gzFile g = gzopen(path, "rb");
    if (g == NULL) {
        log_fail("test_gzputs_write_and_read_back", "gzopen for reading failed");
        remove(path);
        return false;
    }
    char buf[256];
    std::string decompressed;
    int r;
    while ((r = gzread(g, buf, sizeof(buf))) > 0) {
        decompressed.append(buf, buf + r);
    }
    gzclose(g);

    // Cleanup
    remove(path);

    if (decompressed == payload) {
        log_pass("test_gzputs_write_and_read_back");
        return true;
    } else {
        log_fail("test_gzputs_write_and_read_back", "Decompressed data does not match original payload");
        return false;
    }
}

// Test 3: gzputs should fail when file is opened in read-only mode
static bool test_gzputs_invalid_mode_write_only() {
    const char *path = "test_gzputs_readonly.gz";
    // Prepare a read-only file by opening it in read mode (will create an empty file if needed)
    // Use gzopen with "rb" so that the internal state is not GZ_WRITE
    // If there is no prior file, gzopen may create an error; to ensure a file exists, touch it first.
    FILE* tmp = std::fopen(path, "wb");
    if (tmp) {
        std::fclose(tmp);
    }
    gzFile g = gzopen(path, "rb"); // read-only mode
    if (g == NULL) {
        // If cannot open in read mode, skip test gracefully
        log_fail("test_gzputs_invalid_mode_write_only", "gzopen for reading failed; skipping test");
        remove(path);
        return false;
    }
    int ret = gzputs(g, "abcdef");
    gzclose(g);
    remove(path);

    if (ret == -1) {
        log_pass("test_gzputs_invalid_mode_write_only");
        return true;
    } else {
        log_fail("test_gzputs_invalid_mode_write_only", "gzputs did not fail for read-only gzFile");
        return false;
    }
}

// Main runner
int main() {
    // Header: Candidate Keywords extracted from the focal method
    // Candidate Keywords: gzputs, gzFile, gz_statep, GZ_WRITE, Z_OK, strlen, s, len, put, gz_write,
    //                     z_size_t, state, file, mode, err, Z_STREAM_ERROR, gz_error
    //
    // Run tests
    bool all_passed = true;

    all_passed &= test_gzputs_null_file();
    all_passed &= test_gzputs_write_and_read_back();
    all_passed &= test_gzputs_invalid_mode_write_only();

    // Summary
    std::cout << "\nTest suite finished. Passed: " << tests_passed
              << " / " << tests_run << " tests." << std::endl;

    return all_passed ? 0 : 1;
}