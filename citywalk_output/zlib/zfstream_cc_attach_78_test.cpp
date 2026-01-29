// This is a standalone C++11 test suite for gzfilebuf::attach from zfstream.cc.
// It avoids Google Test and uses a small, custom test harness.
// Notes:
// - It relies on zlib (gzdopen, gzopen, etc.). Compile/link with -lz.
// - Tests focus on public interfaces to exercise the attach() logic without peeking into private state.
// - Each test prints a brief explanatory message and returns a boolean indicating pass/fail.
// - The test suite covers key branches: 
//   1) early fail when both in and out are requested, 
//   2) failure when provided fd is invalid (simulating gzdopen failure),
//   3) success path when a valid gzip file descriptor is attached.

#include <cstring>
#include <cstdio>
#include <string>
#include <ios>
#include <sys/stat.h>
#include <iostream>
#include <zlib.h>
#include <zfstream.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>



/*
  Domain knowledge used:
  - gzdopen attaches an existing file descriptor to a gzip stream.
  - The attach() method returns a pointer to the object on success, or NULL on failure.
  - We must exercise true/false branches of key predicates in attach():
      a) is_open() check
      b) mode predicate: (mode & in) && (mode & out)
      c) open_mode(mode, char_mode) result
      d) gzdopen(fd, char_mode) result
  - We use real files (gzipped content) to reliably trigger gzdopen success path.
  - We clean up created temporary files after tests.
*/

// Lightweight assertion helpers (non-terminating on failure)
#define TEST_OK(cond, msg) do { if (!(cond)) { std::cerr << "[FAIL] " << msg << " (Line " << __LINE__ << ")\n"; return false; } } while (0)
#define TEST_PASS(cond, msg) do { if (cond) { std::cout << "[PASS] " << msg << "\n"; } else { std::cerr << "[FAIL] " << msg << " (Line " << __LINE__ << ")\n"; } } while (0)


// Helper: create a small gzip file with given content using zlib (gzopen)
bool create_gzip_file(const char* filename, const std::string& content) {
    gzFile gz = gzopen(filename, "wb");
    if (!gz) {
        std::cerr << "Failed to create gzip file: " << filename << "\n";
        return false;
    }
    int written = gzwrite(gz, content.data(), static_cast<unsigned int>(content.size()));
    gzclose(gz);
    if (written != static_cast<int>(content.size())) {
        std::cerr << "Failed to write complete content to gzip file: " << filename << "\n";
        return false;
    }
    return true;
}

// Test A: attach should return NULL when mode requests both input and output
bool test_attach_both_in_out_returns_null() {
    gzfilebuf buf;
    // Both in and out requested should fail early
    std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out;
    gzfilebuf* res = buf.attach(-1, mode); // fd unused due to early check
    TEST_OK(res == NULL, "attach() returns NULL when mode requests both in and out");
    return true;
}

// Test B: attach should return NULL when fd is invalid (simulate gzdopen failure)
bool test_attach_invalid_fd_returns_null() {
    gzfilebuf buf;
    // Use a valid mode (in only) but provide an invalid fd to trigger gzdopen failure
    std::ios_base::openmode mode = std::ios_base::in;
    gzfilebuf* res = buf.attach(-1, mode);
    TEST_OK(res == NULL, "attach() returns NULL when gzdopen would fail (invalid fd)");
    return true;
}

// Test C: attach should succeed given a valid gzip file descriptor
bool test_attach_success_with_valid_gzip_fd() {
    // Create a small gzip file to obtain a valid gzip-formatted fd
    const char* test_gz = "test_ok.gz";
    if (!create_gzip_file(test_gz, "TEST_CONTENT")) {
        std::cerr << "Could not create test gzip file for attach success test.\n";
        return false;
    }

    // Open the gzip file to obtain a file descriptor
    int fd = ::open(test_gz, O_RDONLY);
    if (fd < 0) {
        std::cerr << "Failed to open test gzip file descriptor for attach success test.\n";
        remove(test_gz);
        return false;
    }

    gzfilebuf buf;
    std::ios_base::openmode mode = std::ios_base::in;

    gzfilebuf* res = buf.attach(fd, mode);
    bool ok = (res != NULL);

    // Cleanup: close fd and remove file
    ::close(fd);
    remove(test_gz);

    TEST_OK(ok, "attach() succeeds with a valid gzip file descriptor");
    return true;
}


// Main driver: run all tests and print summary
int main() {
    bool overall = true;

    std::cout << "Running unit tests for gzfilebuf::attach (zfstream.cc)\n";

    overall &= test_attach_both_in_out_returns_null();
    overall &= test_attach_invalid_fd_returns_null();
    overall &= test_attach_success_with_valid_gzip_fd();

    if (overall) {
        std::cout << "All tests completed. Summary: PASS\n";
        return 0;
    } else {
        std::cerr << "Some tests failed. Summary: FAIL\n";
        return 1;
    }
}