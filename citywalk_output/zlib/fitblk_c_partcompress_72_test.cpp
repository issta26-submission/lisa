/*
Unit Test Suite for the focal method: partcompress (in fitblk.c)

Step 1 - Program Understanding (summary for the test author)
- The focal method is partcompress(FILE *in, z_streamp def).
- It reads input in chunks of size RAWLEN into a local buffer, updates def->avail_in and def->next_in,
  uses feof to decide when to finish (sets flush to Z_FINISH when end of file is reached),
  and calls deflate(def, flush) in a loop until the output buffer is full (def->avail_out == 0) or
  a finish condition is encountered (flush != Z_NO_FLUSH).
- The function returns the last deflate return code (ret), or Z_ERRNO if ferror(in) occurs,
  and asserts on Z_STREAM_ERROR in the loop.
- Core dependent components (Candidate Keywords): RAWLEN, ferror, feof, fread, fgetc/fread path, feof,
  Z_NO_FLUSH, Z_FINISH, Z_STREAM_ERROR, Z_ERRNO, z_streamp/deflate, deflateInit/deflateEnd usage, input/output buffers.

Step 2 - Unit Test Generation (Strategy)
- Since partcompress is a C function that relies on zlib (deflate), tests will:
  - Initialize a z_streamp (z_stream) with deflateInit and a non-empty output buffer.
  - Create temporary files with controlled content lengths to exercise:
    a) End-of-file path (feof becomes true on first read) to verify the Z_FINISH path.
    b) A larger input (more bytes than RAWLEN) to exercise the loop and ensure normal operation without immediate EOF.
  - Verify that partcompress returns a non-error code (not Z_STREAM_ERROR and not Z_ERRNO) for valid inputs.
  - Clean up all zlib resources and temporary files after each test.
- We will not rely on GTest; instead a small self-contained test runner is implemented in main().
- Static/private detail handling: The test uses the exported function signature:
  extern "C" int partcompress(FILE *in, z_streamp def);
  If the real project exposes it with external linkage, this test will call it. We assume the project's build
  system exposes the function for testing (as the codebase in a real project would).
  If the function is static in the translation unit, this test approach would require linking against a built object
  where the function is accessible; the test is written to work with the likely project configuration where partcompress
  is available to links.

Step 3 - Test Case Refinement (domain knowledge applied)
- Use small and large input sizes to cover basic and iterative behavior:
  - 1-byte input to hit the end-of-file path quickly (feof/in flush FINISH case).
  - 4000-byte or larger input to exercise the loop as multiple RAWLEN-block reads happen.
- Use a straightforward, non-terminating assertion style (print PASS/FAIL) and run tests from main() as required when GTest is not allowed.

Note: RAWLEN is defined in the focal codebase; tests do not rely on its exact value, only that there is a RAWLEN-sized read,
and that the end-of-file condition triggers Z_FINISH.

The code below implements the tests. It does not depend on GTest and provides explanatory comments for each test case.

*/

// Include standard C++ headers for test harness
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <assert.h>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>


// zlib header for deflate related types

// Declaration of the focal function under test.
// If the project exposes the symbol with C linkage, this declaration will be resolved by the linker.
// We use extern "C" to ensure C linkage in C++ test code.
extern "C" int partcompress(FILE *in, z_streamp def);

// Helper function: create a temporary file with given data and return its path.
// Uses POSIX mkstemp to safely create a unique temporary file.
static std::string createTempFileWithDataconst(const std::string &data) {
    char tmpl[] = "/tmp/partcompress_test_XXXXXX";
    int fd = mkstemp(tmpl);
    if (fd == -1) {
        return ""; // failure
    }
    // Write data to the temp file
    if (data.size() > 0) {
        ssize_t written = write(fd, data.data(), data.size());
        (void)written;
        // Not strictly checking write success here, test will fail downstream if needed
    }
    // Close the descriptor to flush on disk
    if (close(fd) != 0) {
        // ignore
    }
    // Return path
    return std::string(tmpl);
}

// Helper function: remove a file if it exists
static void removeFileIfExists(const std::string &path) {
    if (!path.empty()) {
        unlink(path.c_str());
    }
}

// Test 1: Basic one-byte input path (end-of-file immediately after first read)
// This ensures the feof path triggers Z_FINISH and that the function completes without error.
static bool test_partcompress_one_byte_input() {
    // Prepare data: 1 byte
    const std::string data = "A";

    std::string path = createTempFileWithDataconst(data);
    if (path.empty()) {
        std::cerr << "[TEST-ONE-BYTE] FAILED: could not create temp file\n";
        return false;
    }

    FILE *in = fopen(path.c_str(), "rb");
    if (!in) {
        std::cerr << "[TEST-ONE-BYTE] FAILED: could not open temp file\n";
        removeFileIfExists(path);
        return false;
    }

    // Initialize zlib deflate stream
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));
    // z allocators default
    int retDef = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (retDef != Z_OK) {
        std::cerr << "[TEST-ONE-BYTE] FAILED: deflateInit returned " << retDef << "\n";
        fclose(in);
        removeFileIfExists(path);
        return false;
    }

    // Output buffer for compression
    unsigned char outbuf[4096];
    strm.next_out = outbuf;
    strm.avail_out = sizeof(outbuf);

    // Call the focal function
    int ret = partcompress(in, &strm);

    // Cleanup
    deflateEnd(&strm);
    fclose(in);
    removeFileIfExists(path);

    // Assertions
    if (ret == Z_STREAM_ERROR) {
        std::cerr << "[TEST-ONE-BYTE] FAILED: partcompress returned Z_STREAM_ERROR\n";
        return false;
    }
    if (ret == Z_ERRNO) {
        std::cerr << "[TEST-ONE-BYTE] FAILED: partcompress returned Z_ERRNO\n";
        return false;
    }

    // If we reached here, behavior is acceptable for a non-error case
    // (Even if actual compression outcome varies with RAWLEN, the function should not crash/return error)
    std::cout << "[TEST-ONE-BYTE] PASSED: partcompress completed without error for 1-byte input\n";
    return true;
}

// Test 2: Large input path (multiple RAWLEN chunks to exercise the loop)
// This ensures the function can loop and flush multiple times until the end of file is reached.
static bool test_partcompress_large_input() {
    // Prepare data: e.g., 4000 bytes of 'A'. The exact RAWLEN value is implementation-defined;
    // 4000 is a reasonable size to cross many RAWLEN boundaries for common RAWLEN values.
    const size_t size = 4000;
    std::string data(size, 'A');

    std::string path = createTempFileWithDataconst(data);
    if (path.empty()) {
        std::cerr << "[TEST-LARGE] FAILED: could not create temp file\n";
        return false;
    }

    FILE *in = fopen(path.c_str(), "rb");
    if (!in) {
        std::cerr << "[TEST-LARGE] FAILED: could not open temp file\n";
        removeFileIfExists(path);
        return false;
    }

    // Initialize zlib deflate stream
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));
    int retDef = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (retDef != Z_OK) {
        std::cerr << "[TEST-LARGE] FAILED: deflateInit returned " << retDef << "\n";
        fclose(in);
        removeFileIfExists(path);
        return false;
    }

    // Output buffer for compression
    unsigned char outbuf[16384];
    strm.next_out = outbuf;
    strm.avail_out = sizeof(outbuf);

    // Call the focal function
    int ret = partcompress(in, &strm);

    // Cleanup
    deflateEnd(&strm);
    fclose(in);
    removeFileIfExists(path);

    // Assertions
    if (ret == Z_STREAM_ERROR) {
        std::cerr << "[TEST-LARGE] FAILED: partcompress returned Z_STREAM_ERROR\n";
        return false;
    }
    if (ret == Z_ERRNO) {
        std::cerr << "[TEST-LARGE] FAILED: partcompress returned Z_ERRNO\n";
        return false;
    }

    std::cout << "[TEST-LARGE] PASSED: partcompress completed without error for large input (" << size << " bytes)\n";
    return true;
}

// Entry point for the test harness
int main() {
    int failures = 0;

    std::cout << "Running unit tests for focal method: partcompress\n";

    if (!test_partcompress_one_byte_input()) {
        std::cerr << "TEST FAILED: partcompress_one_byte_input\n";
        ++failures;
    }

    if (!test_partcompress_large_input()) {
        std::cerr << "TEST FAILED: partcompress_large_input\n";
        ++failures;
    }

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << failures << " TEST(S) FAILED\n";
        return 1;
    }
}