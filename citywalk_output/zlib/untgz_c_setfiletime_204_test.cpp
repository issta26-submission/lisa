/*
Step 1 - Program Understanding (Candidate Keywords)
- setfiletime: focal function under test (defined in untgz.c)
- Non-Windows path (the #else branch uses utime to set both access and modification times)
- utime: system call used to set file times on POSIX systems
- struct utimbuf: used to pass actime and modtime to utime
- fname / fname length: target file name
- ftime: time_t value used to set both times
- Return semantics: 0 on success, -1 on failure (POSIX path)
- Error handling for missing files (utime should fail, returning -1)

Step 2 - Unit Test Generation (Target: setfiletime in untgz.c)
- Focus on non-Windows path (POSIX) since tests will run in a typical CI Linux environment.
- Coverage goals:
  - Success path: set a known time and verify st_mtime equals the requested time.
  - Failure path: call on a non-existent file and verify -1 is returned.
  - State progression: call setfiletime twice on the same file with different times and verify the final time matches the last set value.

Step 3 - Test Case Refinement (Domain Knowledge: C++11, no GTest, self-contained)
- Use simple, non-terminating test harness: print failures but continue executing remaining tests.
- Use standard library only for test scaffolding; rely on C linkage to call setfiletime.
- Ensure static/time dependencies are handled correctly (no static state dependence in tests).
- Provide clear explanatory comments for each test case.

Code: C++11 test suite for setfiletime (POSIX path)
Note: This test suite assumes untgz.c (providing setfiletime) is compiled and linked into the test binary.
*/

#include <cstdio>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>


extern "C" int setfiletime(char *fname, time_t ftime); // POSIX path: uses utime

// Helper: create a temporary file and return its path
static bool create_temp_file(std::string &path_out) {
    // Use mkstemp to safely create a unique file in /tmp
    char template_path[] = "/tmp/untgz_testfileXXXXXX";
    int fd = mkstemp(template_path);
    if (fd == -1) {
        std::cerr << "Failed to create temp file.\n";
        return false;
    }
    // Optionally write something to ensure file exists
    const char *data = "untgz test file";
    ssize_t written = write(fd, data, std::strlen(data));
    (void)written; // ignore partial writes in test context
    close(fd);

    path_out = std::string(template_path);
    return true;
}

// Helper: get modification time of a file
static bool get_mtime(const std::string &path, time_t &mt) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) {
        return false;
    }
#if defined(__APPLE__) && defined(HAVE_STAT_STRUCT_STAT)
    // On some platforms st_mtime is time_t; keep compatibility by direct assignment
#endif
    mt = st.st_mtime;
    return true;
}

// Helper: remove a file if it exists
static void remove_file_if_exists(const std::string &path) {
    unlink(path.c_str());
}

// Simple test harness
static int failed_tests = 0;

#define TEST_FAIL(msg) do { std::cerr << "TEST FAIL: " << (msg) << std::endl; ++failed_tests; } while(false)
#define TEST_PASS(msg) do { std::cout << "TEST PASS: " << (msg) << std::endl; } while(false)

// Test 1: Basic successful setfiletime on a real file
static bool test_basic_setfiletime() {
    std::string path;
    if (!create_temp_file(path)) {
        TEST_FAIL("Could not create temporary file for test_basic_setfiletime.");
        return false;
    }

    // Known target time (UTC)
    time_t target_time = 1609459200; // 2021-01-01 00:00:00 UTC

    int ret = setfiletime(const_cast<char*>(path.c_str()), target_time);
    if (ret != 0) {
        TEST_FAIL("setfiletime failed to set time on existing file (return != 0).");
        remove_file_if_exists(path);
        return false;
    }

    time_t actual_time;
    if (!get_mtime(path, actual_time)) {
        TEST_FAIL("Failed to stat file after setfiletime in test_basic_setfiletime.");
        remove_file_if_exists(path);
        return false;
    }

    if (actual_time != target_time) {
        TEST_FAIL("Modification time does not match target_time in test_basic_setfiletime.");
        std::cerr << "Expected: " << target_time << " Actual: " << actual_time << std::endl;
        remove_file_if_exists(path);
        return false;
    }

    remove_file_if_exists(path);
    TEST_PASS("test_basic_setfiletime: modification time set correctly on existing file.");
    return true;
}

// Test 2: setfiletime should fail for non-existent file
static bool test_setfiletime_nonexistent() {
    // Generate a clearly non-existent path
    std::string path = "/tmp/untgz_nonexistent_test_" + std::to_string(std::time(nullptr)) + ".tmp";

    // Ensure it doesn't exist
    struct stat st;
    if (stat(path.c_str(), &st) == 0) {
        // Unlikely, but remove if present
        remove_file_if_exists(path);
    }

    time_t t = 1234567890; // some fixed time
    int ret = setfiletime(const_cast<char*>(path.c_str()), t);
    if (ret != -1) {
        TEST_FAIL("setfiletime unexpectedly succeeded on nonexistent file.");
        // Best effort cleanup, though there should be nothing to clean
        remove_file_if_exists(path);
        return false;
    }

    TEST_PASS("test_setfiletime_nonexistent: correctly returned -1 for nonexistent file.");
    return true;
}

// Test 3: Multiple successive calls on the same file; final time should reflect the last call
static bool test_setfiletime_multiple() {
    std::string path;
    if (!create_temp_file(path)) {
        TEST_FAIL("Could not create temp file for test_setfiletime_multiple.");
        return false;
    }

    time_t first_time = 1000000000; // some early time
    time_t second_time = 2000000000; // a later time

    int ret1 = setfiletime(const_cast<char*>(path.c_str()), first_time);
    if (ret1 != 0) {
        TEST_FAIL("First setfiletime in test_setfiletime_multiple returned non-zero.");
        remove_file_if_exists(path);
        return false;
    }

    int ret2 = setfiletime(const_cast<char*>(path.c_str()), second_time);
    if (ret2 != 0) {
        TEST_FAIL("Second setfiletime in test_setfiletime_multiple returned non-zero.");
        remove_file_if_exists(path);
        return false;
    }

    time_t actual_time;
    if (!get_mtime(path, actual_time)) {
        TEST_FAIL("Failed to stat file after two setfiletime calls in test_setfiletime_multiple.");
        remove_file_if_exists(path);
        return false;
    }

    if (actual_time != second_time) {
        TEST_FAIL("Final modification time does not match second_time in test_setfiletime_multiple.");
        std::cerr << "Expected: " << second_time << " Actual: " << actual_time << std::endl;
        remove_file_if_exists(path);
        return false;
    }

    remove_file_if_exists(path);
    TEST_PASS("test_setfiletime_multiple: final modification time matches the last set value.");
    return true;
}

// Main: run all tests
int main() {
    std::cout << "Starting test suite for setfiletime (POSIX path) ..." << std::endl;

    bool ok1 = test_basic_setfiletime();
    bool ok2 = test_setfiletime_nonexistent();
    bool ok3 = test_setfiletime_multiple();

    int passed = (ok1 ? 1 : 0) + (ok2 ? 1 : 0) + (ok3 ? 1 : 0);
    int total  = 3;

    std::cout << "Test summary: " << passed << " / " << total << " tests passed." << std::endl;

    if (failed_tests > 0) {
        std::cout << failed_tests << " tests failed." << std::endl;
        return EXIT_FAILURE;
    }
    // If all tests passed, return success
    return (passed == total) ? EXIT_SUCCESS : EXIT_FAILURE;
}