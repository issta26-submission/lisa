#include <cstring>
#include <cstdio>
#include <zfstream.h>
#include <iostream>


// Lightweight, non-terminating test harness (no external test framework).
// We rely on the behavior of gzofstream as implemented in zfstream.h.
// The tests focus on the open(...) method's effects on the stream state.
//
// Test 1: test_gzofstream_open_success
//   - Attempts to open a normal, writable file in output mode.
//   - Expects the operation to succeed (stream remains GOOD, no failbit set).
//   - Cleans up by removing the created file.
// Test 2: test_gzofstream_open_failure
//   - Attempts to open an invalid/empty path.
//   - Expects the operation to fail (failbit set on the stream).

// Test 1: Open a valid file for writing. Expect success (no failbit).
bool test_gzofstream_open_success()
{
    // Use a temporary, normally writable filename.
    const char* fname = "test_open_success.gz";

    // Ensure any prior artifact is removed
    std::remove(fname);

    gzofstream ofs;
    // Open in write mode; the class internally ORs with std::ios_base::out as per implementation.
    ofs.open(fname, std::ios_base::out);

    // If the open succeeded, the stream should not be in a fail state.
    bool ok = ofs.good() && !ofs.fail();

    if (ok) {
        // Optional: try to perform a small write to ensure the underlying buffer is usable.
        ofs << "unit-test";
        ofs.flush();
    }

    // Close (best-effort; destructor will also close, but explicit close is clearer here).
    ofs.close();

    // Cleanup: remove the generated file to keep the test environment clean.
    std::remove(fname);

    // Expose result
    if (!ok) {
        std::cerr << "test_gzofstream_open_success: OPEN indicated failure unexpectedly.\n";
    }
    return ok;
}

// Test 2: Open an invalid path (empty string). Expect failure (failbit set).
bool test_gzofstream_open_failure()
{
    gzofstream ofs;
    // Use an invalid path to trigger failure in the underlying sb.open().
    ofs.open("", std::ios_base::out);

    // Expect the stream to be in a failed state.
    bool failed = ofs.fail();

    // Do not attempt to write; the open already failed.
    return failed;
}

// Entry point to run tests
int main()
{
    int total = 2;
    int passed = 0;

    std::cout << "Running unit tests for gzofstream::open(...)" << std::endl;

    if (test_gzofstream_open_success()) {
        ++passed;
        std::cout << "[OK] test_gzofstream_open_success" << std::endl;
    } else {
        std::cout << "[FAIL] test_gzofstream_open_success" << std::endl;
    }

    if (test_gzofstream_open_failure()) {
        ++passed;
        std::cout << "[OK] test_gzofstream_open_failure" << std::endl;
    } else {
        std::cout << "[FAIL] test_gzofstream_open_failure" << std::endl;
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}