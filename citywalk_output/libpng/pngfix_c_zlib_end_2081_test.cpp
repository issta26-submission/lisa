// C++11 based unit tests for the focal C function: zlib_end(struct zlib *zlib)
// This test suite does not use GoogleTest; it uses a small in-file test harness.
// It relies on the project-provided C headers (pngfix.h, etc.) being available
// in the include path during linking. The tests capture stdout output to verify
// behavior of the zlib_end function across its branches.
//
// Note: The environment must compile/link the C sources (e.g., pngfix.c) with
// the C++ test file. We assume the public API and structs (struct zlib, etc.)
// are exposed via pngfix.h. The tests only interact with public APIs and
// non-private static helpers through the provided header.

#include <functional>
#include <sys/types.h>
#include <stdio.h>
#include <vector>
#include <sys/stat.h>
#include <limits.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <stdlib.h>
#include <errno.h>
#include <setjmp.h>
#include <string.h>
#include <string>
#include <zlib.h>
#include <iostream>
#include <pngfix.h>
#include <cstring>
#include <ctype.h>


// Include the C header defining zlib and related structures.
// The header is C; wrap in extern "C" to avoid C++ name mangling issues.
extern "C" {
}

// Simple in-file tiny test framework
#define RUN_TEST(name)  do { if (name()) { pass_count++; } else { fail_count++; } } while(0)

static int pass_count = 0;
static int fail_count = 0;

// Helper: capture stdout output produced by a callable.
// Implementation uses POSIX dup/dup2 to redirect stdout to a temp file.
static std::string capture_stdout(const std::function<void()> &fn)
{
    // Create a temporary file
    char tmpl[] = "/tmp/pngfix_test_output_XXXXXX";
    int fd = mkstemp(tmpl);
    if (fd == -1) {
        // Fallback: return empty on failure
        return "";
    }

    // Save current stdout
    int saved_stdout = dup(fileno(stdout));
    // Redirect stdout to our temp file
    if (dup2(fd, fileno(stdout)) == -1) {
        // Restore and cleanup if redirection fails
        close(fd);
        close(saved_stdout);
        return "";
    }

    // Close the duplicate fd (we've redirected using it)
    close(fd);

    // Run the test function
    fn();

    // Flush and restore stdout
    fflush(stdout);
    dup2(saved_stdout, fileno(stdout));
    close(saved_stdout);

    // Read the captured content
    std::string output;
    FILE *fp = fopen(tmpl, "r");
    if (fp) {
        char buf[4096];
        while (fgets(buf, sizeof(buf), fp)) {
            output += buf;
        }
        fclose(fp);
    }

    // Remove temp file
    unlink(tmpl);
    return output;
}

// Helper: create a minimal zlib instance with required fields for tests.
// This function relies on the public types exposed by pngfix.h.
static struct zlib* make_minimal_zlib(bool quiet,
                                    int ok_bits,
                                    int file_bits,
                                    int cksum,
                                    int state,
                                    const char *filename,
                                    png_uint_32 chunk_type_val)
{
    // Allocate zlib structure
    struct zlib *z = (struct zlib*)calloc(1, sizeof(struct zlib));
    if (!z) return nullptr;

    // Global
    struct global g;
    memset(&g, 0, sizeof(g));
    g.quiet = quiet;
    z->global = &g;

    // File
    struct file f;
    memset(&f, 0, sizeof(f));
    f.file_name = filename ? filename : "";
    z->file = &f;

    // Chunk
    struct chunk ch;
    memset(&ch, 0, sizeof(ch));
    ch.chunk_type = chunk_type_val;
    z->chunk = &ch;

    // zlib internals
    z->ok_bits = ok_bits;
    z->file_bits = file_bits;
    z->cksum = cksum;
    z->state = state;
    z->rc = 0;
    z->z.zalloc = Z_NULL;
    z->z.zfree = Z_NULL;
    z->z.opaque = Z_NULL;

    // Bytes counters
    z->compressed_bytes = 0;
    z->compressed_digits = 0;
    z->uncompressed_bytes = 0;
    z->uncompressed_digits = 0;

    // zmsg
    z->z.msg = NULL;

    // Ensure structures are accessible
    return z;
}

// Test 1: quiet == false, ok_bits < 16, checksum is present -> expect "CHK" reason.
// This exercises the first inner branch that outputs a detailed summary line.
static bool test_zlib_end_quiet_false_cksum_branch()
{
    // Prepare zlib and dependencies
    struct zlib *z = make_minimal_zlib(false, 10 /*ok_bits<16*/, 12, 1 /*cksum*/, 
                                       0 /*state*/, "test1.png",
                                       0x49444154 /* 'IDAT' */);
    if (!z) return false;

    bool passed = false;
    std::string out = capture_stdout([&]() {
        zlib_end(z);
    });

    // Expected: output should contain IDAT, CHK, and the filename
    if (out.find("IDAT") != std::string::npos &&
        out.find("CHK") != std::string::npos &&
        out.find("test1.png") != std::string::npos) {
        passed = true;
    }

    free(z); // free allocated zlib struct
    return passed;
}

// Test 2: quiet == true -> nothing should be printed regardless of other fields.
static bool test_zlib_end_quiet_true_no_output()
{
    struct zlib *z = make_minimal_zlib(true /*quiet*/, 8, 8, 0, -1, "quiet.png", 0);
    if (!z) return false;

    std::string out = capture_stdout([&]() {
        zlib_end(z);
    });

    bool passed = out.empty();

    free(z);
    return passed;
}

// Test 3: quiet == false, ok_bits >= 16 -> SKP path.
// To keep things simple, we avoid inflating by setting state < 0 so the inflateEnd
// block is skipped, focusing on the SKP output line.
static bool test_zlib_end_quiet_false_skp_branch()
{
    struct zlib *z = make_minimal_zlib(false, 16 /*ok_bits>=16*/, 20, 0, -1, "skp.png", 0x49444154);
    if (!z) return false;

    bool passed = false;
    std::string out = capture_stdout([&]() {
        zlib_end(z);
    });

    if (out.find("IDAT") != std::string::npos &&
        out.find("SKP") != std::string::npos &&
        out.find("skp.png") != std::string::npos) {
        passed = true;
    }

    free(z);
    return passed;
}

// Test 4: ok_bits == file_bits -> "OK " reason branch.
// We'll also ensure a line is printed with OK and the filename.
static bool test_zlib_end_ok_branch()
{
    struct zlib *z = make_minimal_zlib(false, 24 /*ok_bits>, file_bits*/, 18, 0, -1, "ok.png", 0x49444154);
    if (!z) return false;

    // Adjust file_bits to equal ok_bits; since our structure is minimal, we set manually.
    z->file_bits = z->ok_bits; // mimic equality

    bool passed = false;
    std::string out = capture_stdout([&]() {
        zlib_end(z);
    });

    if (out.find("IDAT") != std::string::npos &&
        out.find("OK ") != std::string::npos &&
        out.find("ok.png") != std::string::npos) {
        passed = true;
    }

    free(z);
    return passed;
}

// Main test runner
int main() {
    std::vector<std::string> test_names = {
        "test_zlib_end_quiet_false_cksum_branch",
        "test_zlib_end_quiet_true_no_output",
        "test_zlib_end_quiet_false_skp_branch",
        "test_zlib_end_ok_branch"
    };

    // Run tests with the simple harness
    RUN_TEST(test_zlib_end_quiet_false_cksum_branch);
    RUN_TEST(test_zlib_end_quiet_true_no_output);
    RUN_TEST(test_zlib_end_quiet_false_skp_branch);
    RUN_TEST(test_zlib_end_ok_branch);

    // Summary
    std::cout << "Tests passed: " << pass_count << "/" << (pass_count + fail_count) << std::endl;
    if (fail_count > 0) {
        std::cerr << "Tests failed: " << fail_count << std::endl;
    }

    return (fail_count == 0) ? 0 : 1;
}

/*
Notes for maintainers:
- The tests rely on the public API exposed via pngfix.h. If the actual project
  uses different struct layouts or private members, adjust make_minimal_zlib()
  to populate the exact fields required by zlib_end and the helper functions it
  calls (zlib_flevel, zlib_rc, type_name, uarb_print, emit_string, etc.).

- The tests capture stdout to validate the presence of key tokens in the output
  (e.g., "IDAT", "CHK", "SKP", "OK "). We avoid asserting exact numeric values
  for the printing of compressed/uncompressed byte counts to remain robust
  against internal formatting changes.

- The tests use a small, in-file test harness and avoid GTest/GMock as requested.
  They are designed to be portable across environments that provide a POSIX API
  (mkstemp, dup2, etc.). If the target environment is non-POSIX, replace the
  stdout capture mechanism accordingly.

- Static/private helper functions in pngfix.c are not invoked directly; the tests
  exercise the focal function via the public header and rely on the existing
  behavior of the library.
*/