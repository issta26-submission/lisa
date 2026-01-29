// Test suite for the focal method: file_compress in minigzip_fuzzer.c
// This test suite is written for C++11 (no GTest) and uses a small custom
// test harness with simple pass/fail reporting. It relies on the actual
// implementation of the C functions provided by the project under test.
// Notes:
// - We declare the focal function as extern "C" to ensure correct linkage from C.
// - We perform a set of targeted tests to exercise different execution paths
//   (non-existent input, overly long filename, gzopen failure path, and
//   successful compression with input removal).
// - The tests use real filesystem interactions where appropriate (temporary
//   files) and clean up after themselves.

#include <cstdio>
#include <cstring>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <inttypes.h>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>


// Declaration of the focal function from the C source.
// We use extern "C" to prevent C++ name mangling during linkage.
extern "C" int file_compress(char *file, char *mode);

// Helper: check if a path exists (file or directory)
static bool pathExists(const std::string &path) {
    struct stat st;
    return (stat(path.c_str(), &st) == 0);
}

// Helper: create a temporary file with given content, returning its path via outPath
static bool createTempInputFile(const std::string &prefix, const std::string &content, std::string &outPath) {
    char templatePath[256];
    // Prefix provided by caller; ensure we have a writable dir for the template
    // Use /tmp-based template for portability
    std::snprintf(templatePath, sizeof(templatePath), "/tmp/%sXXXXXX", prefix.c_str());
    int fd = mkstemp(templatePath);
    if (fd == -1) return false;
    // Write content
    if (!content.empty()) {
        ssize_t written = write(fd, content.c_str(), content.size());
        (void)written;
    }
    close(fd);
    outPath = templatePath;
    return true;
}

// Test 1: Non-existent input file
// Purpose: Verify behavior when the input file does not exist. The code should
//         return 0 and not crash (it uses perror in this code path).
static bool test_nonexistent_input_file() {
    const char *mode = "wb";
    // Intentionally non-existent path
    const char *path = "minigzip_fuzzer_nonexistent_input.tmp";
    int ret = file_compress((char*)path, (char*)mode);
    // Expect an early exit with 0
    return ret == 0;
}

// Test 2: Filename too long (trigger the length check for outfile)
 // Purpose: Ensure the function detects an overly long input filename and returns 0.
static bool test_filename_too_long() {
    // Create an extremely long filename to trigger the size check:
    // We don't need the file to exist because this path is checked before fopen.
    std::string longName;
    longName.assign(6000, 'a'); // length well beyond typical MAX_NAME_LEN
    int ret = file_compress((char*)longName.c_str(), (char*)"wb");
    return ret == 0;
}

// Test 3: gzopen failure path (invalid mode string)
// Purpose: Ensure the path where gzopen fails is exercised by passing an
//          invalid mode string. We still provide a valid input file so the code
//          reaches gzopen and then fails there.
static bool test_gzopen_failure_due_to_mode() {
    // Create a small temporary input file
    std::string inputPath;
    if (!createTempInputFile("minigzip_fuzzer_input", "hello", inputPath))
        return false;

    // Use an invalid mode string to force gzopen failure
    int ret = file_compress((char*)inputPath.c_str(), (char*)"wx"); // invalid mode
    // Cleanup input file (if still exists, may have been unlink'd only on success)
    unlink(inputPath.c_str());

    return ret == 0;
}

// Test 4: Successful compression path and input removal
// Purpose: Ensure that when compression is performed, the input file is removed
//          and a .gz output file is created alongside it.
static bool test_successful_compression_removes_input_and_creates_gz() {
    // Create a temporary input file with some content
    std::string inputPath;
    if (!createTempInputFile("minigzip_fuzzer_input", "content for compression", inputPath))
        return false;

    // Expected gz path (inputPath + ".gz")
    std::string gzPath = inputPath + ".gz";

    // Make sure there isn't a pre-existing gz file
    unlink(gzPath.c_str());

    int ret = file_compress((char*)inputPath.c_str(), (char*)"wb");

    // Check results: input should be removed, gz should exist
    bool inputExists = pathExists(inputPath);
    bool gzExists = pathExists(gzPath);

    // Cleanup: remove gz if created
    if (gzExists) unlink(gzPath.c_str());

    // The function should return 0 on success path
    return (ret == 0) && (!inputExists) && gzExists;
}

// Simple test runner
static void runTest(const std::string &name, bool(*testFn)()) {
    bool ok = testFn();
    std::cout << name << ": " << (ok ? "PASSED" : "FAILED") << std::endl;
}

int main() {
    // Run the tests in a controlled sequence
    runTest("test_nonexistent_input_file", test_nonexistent_input_file);
    runTest("test_filename_too_long", test_filename_too_long);
    runTest("test_gzopen_failure_due_to_mode", test_gzopen_failure_due_to_mode);
    runTest("test_successful_compression_removes_input_and_creates_gz", test_successful_compression_removes_input_and_creates_gz);
    return 0;
}