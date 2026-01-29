// Test suite for gzofstream::close() without using a test framework (GTest disallowed).
// This suite targets the close behavior as implemented in gzofstream::close(),
// which delegates to sb.close() and sets failbit on the stream if the underlying
// buffer close fails.
// We create two scenarios to cover both branches of the predicate:
// 1) Not opened (sb.close() would presumably fail) -> expect failbit to be set.
// 2) Opened successfully and closed -> expect no failbit to be set.
//
// Notes:
// - We rely only on the public interface of gzofstream and its standard behavior.
// - We avoid private members access and do not rely on mocking (as per constraints).
// - We provide lightweight, non-terminating assertions and a simple test runner.

#include <cstring>
#include <cstdio>
#include <string>
#include <iostream>
#include <zfstream.h>



// Simple lightweight test harness
static int g_test_failures = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            ++g_test_failures; \
            std::cerr << "EXPECT_TRUE failed at line " << __LINE__ << ": " << (msg) << std::endl; \
        } \
    } while (0)

#define EXPECT_FALSE(cond, msg) \
    do { \
        if ((cond)) { \
            ++g_test_failures; \
            std::cerr << "EXPECT_FALSE failed at line " << __LINE__ << ": " << (msg) << std::endl; \
        } \
    } while (0)

#define SKIP_MSG(msg) \
    do { \
        std::cerr << "SKIP: " << (msg) << std::endl; \
    } while (0)


// Test 1: When gzofstream has not been opened/attached, close() should cause sb.close()
// to fail, and the stream should reflect a failbit.
void test_close_not_open_sets_failbit()
{
    // Create a gzofstream without opening/attaching a file
    gzofstream ofs;

    // Ensure initial state is not failed (best-effort; some environments may differ)
    std::ios_base::iostate initial_state = ofs.rdstate();
    EXPECT_FALSE((initial_state & std::ios_base::failbit) != 0,
                 "Initial state should not have failbit set");

    // Call close() on the unopened stream
    ofs.close();

    // After attempting to close an unopened buffer, the implementation is expected
    // to set failbit on the stream if the underlying sb.close() failed.
    std::ios_base::iostate post_state = ofs.rdstate();
    bool has_fail = (post_state & std::ios_base::failbit) != 0;
    EXPECT_TRUE(has_fail, "gzofstream::close() on unopened stream should set failbit");
}


// Test 2: When gzofstream is opened and used, close() should not set failbit on success.
void test_close_opened_no_failbit()
{
    // Prepare a temporary file path for the gzipped output
    // We choose a deterministic name in the current directory; will try to clean up afterwards.
    const char* fname = "test_gzofstream_opened_output.gz";

    // Remove any existing file from a prior test run
    std::remove(fname);

    gzofstream ofs;

    // Attempt to open the file in binary write mode
    ofs.open(fname, std::ios_base::out | std::ios_base::binary);

    // If open failed, report and skip the rest of this test (cannot guarantee behavior)
    std::ios_base::iostate state_after_open = ofs.rdstate();
    if ((state_after_open & std::ios_base::failbit) != 0) {
        SKIP_MSG("Opening gzofstream for test failed; skipping open-close success test.");
        // Cleanup any partial file
        std::remove(fname);
        return;
    }

    // Try to write something to ensure the buffer is actually usable
    ofs << "Test data for gzofstream.close() success path.";

    // Now attempt to close the file
    ofs.close();

    // After a successful close, there should be no failbit set
    std::ios_base::iostate final_state = ofs.rdstate();
    bool has_fail = (final_state & std::ios_base::failbit) != 0;
    EXPECT_FALSE(has_fail, "gzofstream::close() after a successful open should not set failbit");

    // Cleanup: remove the created file
    std::remove(fname);
}


// Main: run all tests and report summary
int main()
{
    std::cout << "Running gzofstream::close() unit tests (no GTest)..." << std::endl;

    test_close_not_open_sets_failbit();
    test_close_opened_no_failbit();

    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_test_failures << " TEST(S) FAILED" << std::endl;
        return g_test_failures;
    }
}