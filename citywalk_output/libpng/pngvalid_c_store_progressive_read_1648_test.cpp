/****************************************************
 * PNGVALID.C - Unit tests for store_progressive_read
 * 
 * This test harness is designed to be compiled as
 * a C++11 program (no GoogleTest) and to exercise
 * the focal function:
 *   void store_progressive_read(png_store *ps, png_structp pp, png_infop pi)
 *
 * It uses lightweight test doubles (mocks) for the
 * dependencies (store_read_buffer_avail, store_read_chunk,
 * png_process_data, png_error) to allow isolated testing
 * of the progressive reader logic.
 *
 * Notes:
 * - The test provides a minimal png_store type with only
 *   the fields accessed by the focal code: pread, current,
 *   next, and a couple of control fields we need to drive tests.
 * - We rely on C linkage for the external functions used by
 *   the focal method so we declare them extern "C".
 * - Tests avoid terminating the process on failure; instead,
 *   they log results and continue, enabling broader coverage.
 ****************************************************/

#include <cstddef>
#include <cstdint>
#include <vector>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <exception>
#include <math.h>


// Domain-specific: emulate libpng types for test harness
typedef void* png_structp;
typedef void* png_infop;
typedef unsigned char png_byte;
typedef unsigned int png_uint_32;

// Forward declare the focal type used by store_progressive_read
// We provide a minimal layout matching the fields accessed by the focal function.
// This layout is sufficient for tests since we control the field semantics in mocks.
struct png_store {
    png_structp pread;     // as used in the predicate: ps->pread != pp
    void *current;          // used for "damaged" check
    void *next;             // used for "damaged" check
    int available;            // drives store_read_buffer_avail(ps)
    int chunk_remaining;      // consumed by store_read_chunk(ps, ...)
};

// Prototypes for the focal method and its dependencies.
// The real implementation is in C; we provide C linkage to mocks.
extern "C" {
    // Focal function (signature as used by the library)
    void store_progressive_read(png_store *ps, png_structp pp, png_infop pi);

    // Dependency hooks to mock
    size_t store_read_buffer_avail(png_store *ps);
    size_t store_read_chunk(png_store *ps, png_byte *buffer, size_t max, size_t min);

    // PNG processing hooks (track data processed)
    void png_process_data(png_structp pp, png_infop pi, png_byte *buffer, size_t cb);

    // Error hook (we replace libpng's longjmp with exception for tests)
    void png_error(png_structp pp, const char *message);
}

// Custom exception to simulate longjmp-like aborts in tests
struct TestAbortException : public std::exception {
    std::string msg;
    explicit TestAbortException(const char* m) : msg(m) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

// Global test state to observe progress
static std::vector<size_t> g_processed_cb_lengths; // records cb passed to png_process_data
static bool g_last_error_was_called = false;
static std::string g_last_error_message;

// Mocks implementation (C linkage)
extern "C" {

size_t store_read_buffer_avail(png_store *ps) {
    // Return 1 if there is "buffer" available, else 0.
    // We simulate a single chunk per call to store_progressive_read,
    // controlled by ps->available.
    if (ps == nullptr) return 0;
    if (ps->available > 0) {
        ps->available--;
        return 1;
    }
    return 0;
}

size_t store_read_chunk(png_store *ps, png_byte *buffer, size_t max, size_t min) {
    // For test simplicity, pretend we cannot fill more than 'chunk_remaining'
    // bytes. Return the number actually "read".
    if (ps == nullptr) return 0;
    size_t toWrite = 0;
    if (ps->chunk_remaining > 0 && max > 0) {
        toWrite = (ps->chunk_remaining < (int)max) ? (size_t)ps->chunk_remaining : max;
        // Simulate writing zeros to the buffer (no observable effect needed for tests)
        if (buffer != nullptr && toWrite > 0) {
            std::memset(buffer, 0, toWrite);
        }
        ps->chunk_remaining -= (int)toWrite;
    }
    return toWrite;
}

void png_process_data(png_structp pp, png_infop pi, png_byte *buffer, size_t cb) {
    // Record the length of data that would have been processed
    (void)pp; (void)pi; // unused in tests, but keep param usage
    g_processed_cb_lengths.push_back(cb);
}

// Custom error: throw to simulate a non-local exit without longjmp
void png_error(png_structp pp, const char *message) {
    (void)pp;
    g_last_error_was_called = true;
    g_last_error_message = std::string(message ? message : "");
    throw TestAbortException("png_error invoked: " + g_last_error_message);
}

} // extern "C"

// Declaration of the focal function prototype (from production)
extern "C" void store_progressive_read(png_store *ps, png_structp pp, png_infop pi);

// Helper asserts (non-terminating, suitable for test suites)
static int g_total_tests = 0;
static int g_failed_tests = 0;
#define TEST(name) void test_##name()
#define RUN_TEST(name) do { std::cout << "Running " #name "...\n"; test_##name(); } while(0)
#define EXPECT_TRUE(cond) do { ++g_total_tests; if(!(cond)) { ++g_failed_tests; std::cerr << "EXPECT_TRUE failed: " #cond "\n"; } } while(0)
#define EXPECT_EQ(a,b) do { ++g_total_tests; if((a)!=(b)) { ++g_failed_tests; std::cerr << "EXPECT_EQ failed: " #a " == " #b " (got " << (a) << " vs " << (b) << ")\n"; } } while(0)
#define EXPECT_STR_EQ(a,b) do { ++g_total_tests; if(std::strcmp((a),(b))!=0) { ++g_failed_tests; std::cerr << "EXPECT_STR_EQ failed: " #a " vs " #b " (got " << (a) << " vs " << (b) << ")\n"; } } while(0)

// Test 1: Valid progressive path should call png_process_data with non-zero cb (at least once)
TEST(ProgressiveRead_CallsProcessData_OnValidState) {
    // Reset global logs
    g_processed_cb_lengths.clear();
    g_last_error_was_called = false;
    g_last_error_message.clear();

    // Prepare a plausible png_structp and png_infop
    png_structp pp = (png_structp)0xABCDEF;
    png_infop pi = (png_infop)0x12345;

    // Prepare a png_store instance with valid state
    png_store ps;
    ps.pread = pp;
    ps.current = (void*)0x1;
    ps.next = (void*)0x2;
    ps.available = 1;        // allow a single iteration per call
    ps.chunk_remaining = 100; // plenty to read

    // Invoke focal method a few times, simulating multiple progressive reads
    try {
        store_progressive_read(&ps, pp, pi);
        store_progressive_read(&ps, pp, pi);
        store_progressive_read(&ps, pp, pi);
    } catch (const TestAbortException&) {
        // If we hit an abort, that's a test failure for this path
    }

    // Expect that we observed at least one png_process_data call
    EXPECT_TRUE(!g_processed_cb_lengths.empty());
    // And ensure no error path was triggered
    EXPECT_TRUE(!g_last_error_was_called);
    // Also ensure that there were exactly 3 calls logged (one per iteration)
    EXPECT_EQ(g_processed_cb_lengths.size(), 3u);
}

// Test 2: Invalid pread mismatch should trigger png_error (simulated by exception)
TEST(ProgressiveRead_ReportsError_OnPreadMismatch) {
    g_processed_cb_lengths.clear();
    g_last_error_was_called = false;
    g_last_error_message.clear();

    png_structp pp = (png_structp)0xAA55AA;
    png_infop pi = (png_infop)0x55AA55;

    png_store ps;
    ps.pread = (png_structp)0xDEADBEEF; // intentionally different
    ps.current = (void*)0x1;
    ps.next = (void*)0x2;
    ps.available = 1;
    ps.chunk_remaining = 50;

    bool caught = false;
    try {
        store_progressive_read(&ps, pp, pi);
    } catch (const TestAbortException&) {
        caught = true;
    }

    // Expect that an error was reported via png_error (exception caught)
    EXPECT_TRUE(caught);
    // Ensure our error logging flag is set
    EXPECT_TRUE(g_last_error_was_called);
}

// Test 3: NULL current or next should trigger error as well
TEST(ProgressiveRead_ReportsError_OnNullCurrentOrNext) {
    // valid prestate but with NULL current and/or next to trigger error
    g_processed_cb_lengths.clear();
    g_last_error_was_called = false;
    g_last_error_message.clear();

    png_structp pp = (png_structp)0xFACE;
    png_infop pi = (png_infop)0xBEEF;

    // Case: current is NULL
    {
        png_store ps;
        ps.pread = pp;
        ps.current = NULL;
        ps.next = (void*)0x2;
        ps.available = 1;
        ps.chunk_remaining = 10;

        bool caught = false;
        try {
            store_progressive_read(&ps, pp, pi);
        } catch (const TestAbortException&) {
            caught = true;
        }
        EXPECT_TRUE(caught);
    }

    // Case: next is NULL
    {
        png_store ps;
        ps.pread = pp;
        ps.current = (void*)0x1;
        ps.next = NULL;
        ps.available = 1;
        ps.chunk_remaining = 10;

        bool caught = false;
        try {
            store_progressive_read(&ps, pp, pi);
        } catch (const TestAbortException&) {
            caught = true;
        }
        EXPECT_TRUE(caught);
    }
}

// Test 4: When there is no available data, png_process_data should not be called
TEST(ProgressiveRead_NoBuffer_NoProcessing) {
    g_processed_cb_lengths.clear();
    g_last_error_was_called = false;
    g_last_error_message.clear();

    png_structp pp = (png_structp)0x1234;
    png_infop pi = (png_infop)0x5678;

    png_store ps;
    ps.pread = pp;
    ps.current = (void*)0x1;
    ps.next = (void*)0x2;
    ps.available = 0;        // no iterations
    ps.chunk_remaining = 10;

    try {
        store_progressive_read(&ps, pp, pi);
    } catch (...) {
        // Do not expect exceptions here
    }

    // No processing should have occurred
    EXPECT_TRUE(g_processed_cb_lengths.empty());
}

// Main driver
int main(void) {
    // Run tests (from the test suite)
    RUN_TEST(ProgressiveRead_CallsProcessData_OnValidState);
    RUN_TEST(ProgressiveRead_ReportsError_OnPreadMismatch);
    RUN_TEST(ProgressiveRead_ReportsError_OnNullCurrentOrNext);
    RUN_TEST(ProgressiveRead_NoBuffer_NoProcessing);

    // Summary
    std::cout << "Total tests: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    if (g_failed_tests > 0)
        return 1;
    return 0;
}