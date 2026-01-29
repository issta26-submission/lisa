/*
Unit test suite for gztack (gzappend.c)

Step 1 - Program Understanding (for test author)
- The focal method is gztack(char *name, int gd, z_stream *strm, int last)
  - It opens a file named by 'name' (if non-NULL) for reading.
  - It allocates input/output buffers (CHUNK sized).
  - It reads data from the input file, feeds it to the provided z_stream (strm),
    updates adler32, and calls deflate to produce gzip data, writing to the
    destination file descriptor 'gd'.
  - When 'last' is true and no more input remains, it finalizes the stream with
    Z_FINISH, writes a trailer (adler and total_in), and closes the destination.
  - It cleans up buffers and input file descriptor at the end.

- Dependencies implied by the class/declaration block include integration with zlib (deflate/inflate),
  file I/O (open/read/write/close), and memory management (malloc/free).

Step 2 - Unit Test Generation
- Given the static/internal nature of gztack in a typical C file, the pragmatic approach
  is to exercise gztack via inclusion in a single translation unit and drive it with a
  real input file. We'll test:
  1) Basic end-to-end behavior: compress a small input file and verify the produced
     gzip payload decompresses back to the original content.
- We'll implement tests in a C++11 harness that includes gzappend.c in the same TU (static linkage
  is assumed to allow direct calls). The harness uses a minimal, non-terminating assertion style.

Step 3 - Test Case Refinement
- Use a real, small input file to ensure the entire IO path (open, read, write) is exercised.
- Decompress the produced gzip file using zlib inflate to verify correctness.
- Use a static global test runner that executes before program entry (via static constructor)
  so that tests run when the translation unit is loaded. Output is non-terminating (does not exit on first failure).

Notes:
- This harness relies on gzip-related functions from zlib being linked (link with -lz).
- The tests are designed to avoid blocking I/O (we provide a real input file).
- We aim for high coverage around the main IO path and the final trailer write by asserting
  decompressibility equals the original input.

Code (single translation unit): test_gzappend.cpp
- Includes gzappend.c to exercise gztack in a single TU, enabling direct access to the function.
- Implements a minimal test harness with non-terminating assertions.
- Provides a single thorough test (basic compression case) with commentary.

Code follows:
*/

#include <cstdio>
#include <cstring>
#include <string.h>
#include <fcntl.h>
#include <gzappend.c>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <vector>
#include <zlib.h>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>


// Forward declaration for C function (assuming gzappend.c provides it with C linkage)
extern "C" void gztack(char *name, int gd, z_stream *strm, int last);

// Step 1: Include the focal C source to ensure gztack and its dependencies are linked in this TU.
// Note: This relies on the build environment allowing including gzappend.c in a C++ translation unit.
// If the project uses separate compilation units, this approach should be adapted accordingly.

// Step 2: Lightweight test harness (non-terminating assertions)
static int g_test_failures = 0;

static void log_result(const std::string& test_name, bool ok, const std::string& detail = "") {
    if (ok) {
        std::cout << "[PASS] " << test_name;
        if (!detail.empty()) std::cout << " - " << detail;
        std::cout << std::endl;
    } else {
        std::cerr << "[FAIL] " << test_name;
        if (!detail.empty()) std::cerr << " - " << detail;
        std::cerr << std::endl;
        ++g_test_failures;
    }
}

// Helper: Decompress a gzip file created by gztack to verify its contents.
// Uses zlib inflate with gzip header support (15 + 16).
static bool decompress_gzip_file(const std::string& path, std::string& out) {
    FILE* f = fopen(path.c_str(), "rb");
    if (!f) return false;

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    std::vector<unsigned char> inbuf(fsize);
    if (fread(inbuf.data(), 1, (size_t)fsize, f) != (size_t)fsize) {
        fclose(f);
        return false;
    }
    fclose(f);

    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int ret = inflateInit2(&strm, 15 + 16); // 15 + 16 => gzip with automatic header
    if (ret != Z_OK) return false;

    strm.next_in = inbuf.data();
    strm.avail_in = static_cast<unsigned int>(inbuf.size());

    const size_t CHUNK = 16384;
    unsigned char outbuf[CHUNK];
    int status;
    do {
        strm.next_out = outbuf;
        strm.avail_out = CHUNK;
        status = inflate(&strm, Z_NO_FLUSH);
        if (status != Z_STREAM_ERROR && status != Z_DATA_ERROR && status != Z_NEED_DICT) {
            size_t have = CHUNK - strm.avail_out;
            out.append(reinterpret_cast<char*>(outbuf), have);
        } else {
            inflateEnd(&strm);
            return false;
        }
    } while (status != Z_STREAM_END);

    inflateEnd(&strm);
    return status == Z_STREAM_END;
}

// Helper: Create a temporary file with given content, return its path.
static std::string write_temp_file(const std::string& content) {
    char tmpl_in[] = "/tmp/gztack_input_XXXXXX";
    int fd = mkstemp(tmpl_in);
    if (fd == -1) return "";
    if (write(fd, content.data(), content.size()) != (ssize_t)content.size()) {
        close(fd);
        return "";
    }
    close(fd);
    return std::string(tmpl_in);
}

// Test 1: Basic end-to-end compression of a small input file using gztack
// - Purpose: Validate that gztack correctly reads input, compresses it via the provided z_stream,
//   writes gzip data to the destination fd, and finalizes with a trailer when last=1.
// - Verification: Decompress the produced gzip file and compare with the original input.
static bool test_basic_gztack_compression() {
    // Prepare input content
    const std::string input_content = "The quick brown fox jumps over the lazy dog\n";
    std::string input_path = write_temp_file(input_content);
    if (input_path.empty()) {
        log_result("test_basic_gztack_compression", false, "failed to create input temp file");
        return false;
    }

    // Prepare output path (gzip file)
    char tmpl_out[] = "/tmp/gztack_output_XXXXXX.gz";
    int out_fd = mkstemp(tmpl_out);
    if (out_fd == -1) {
        log_result("test_basic_gztack_compression", false, "failed to create output temp file");
        // cleanup input
        unlink(input_path.c_str());
        return false;
    }
    // Opened FD may be closed by gztack; ensure we have a valid FD for passing in
    // The following keeps compatibility with gztack's behavior of closing 'gd' when last is true.

    // Initialize zlib stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        log_result("test_basic_gztack_compression", false, "deflateInit2 failed");
        close(out_fd);
        unlink(input_path.c_str());
        unlink(tmpl_out); // safety
        return false;
    }

    // Call gztack: name, destination fd, stream, last
    gztack(const_cast<char*>(input_path.c_str()), out_fd, &strm, 1);

    // Clean up zlib stream
    deflateEnd(&strm);

    // Decompress the produced gzip file to verify contents
    std::string decompressed;
    bool ok_decompress = decompress_gzip_file(tmpl_out, decompressed);

    // Cleanup temporary files (best effort)
    // Note: The destination file may be closed by gztack; it's safe to remove afterwards.
    close(out_fd);
    unlink(input_path.c_str());
    unlink(tmpl_out);

    bool ok = ok_decompress && (decompressed == input_content);
    log_result("test_basic_gztack_compression", ok,
               ok ? "decompressed matches input" : "decompressed content mismatch or decompression failed");
    return ok;
}

// Step 3: Test harness launcher (executed before main)
struct GZTestHarness {
    GZTestHarness() {
        // Run tests
        bool t1 = test_basic_gztack_compression();
        // Potential future tests can be added here following the same pattern.
        if (t1) {
            // Mark success
            // Already logged inside test function
        }
        // Final summary will be printed in destructor
    }
    ~GZTestHarness() {
        if (g_test_failures > 0) {
            std::cerr << "TOTAL TEST FAILURES: " << g_test_failures << std::endl;
        } else {
            std::cerr << "ALL TESTS COMPLETED (non-terminating checks)." << std::endl;
        }
    }
};

// Global static harness instance to trigger tests before main()
static GZTestHarness __gztest_harness__;

// End of test_gzappend.cpp