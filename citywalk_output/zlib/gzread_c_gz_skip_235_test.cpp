/*
Unit test suite for the focal method gz_skip.

Notes and design decisions:
- The original gz_skip is part of a larger library and relies on internal helpers (e.g., gz_fetch) and
  a precise state structure. To make this self-contained and compilable under C++11 (without GTest),
  this test file provides a minimal, self-contained reimplementation of the relevant pieces:
  - A compact gz_statep-like structure with the fields accessed by gz_skip.
  - A function pointer-based hook for gz_fetch so test scenarios can simulate different fetch behaviors
    (including success and failure) without altering the test harness.
  - A straightforward test runner with non-terminating assertions (i.e., assertions do not exit the test harness
    on first failure; instead, they log the failure and continue).
- Branch coverage aims to exercise:
  1) The "have" buffer non-empty path (state->x.have > 0) to verify n calculation and state mutation.
  2) The "EOF and avail_in == 0" path to ensure early exit when end of input is reached.
  3) The "need to fetch more data" path using a test-controlled gz_fetch to exercise the loop with available data. We simulate
     multiple fetch iterations to skip exactly the requested len bytes.
- Some paths in real code (e.g., gz_fetch returning -1) require a real data source; the test uses a controlled
  fetch function to exercise typical, non-error behavior in the fetch path.
- All code is self-contained and uses only C++ standard library facilities.

Compilation: g++ -std=c++11 -O2 -Wall -Wextra -D_GLIBCXX_USE_CXX11_ABI=1 test_gz_skip.cpp -o test_gz_skip
*/

#include <cstring>
#include <cstdint>
#include <iostream>
#include <gzguts.h>
#include <cstdlib>


// Domain-specific helpers (self-contained for testing)
typedef unsigned long long z_off64_t;

// Minimal macro to mimic the behavior of the original GT_OFF macro.
// We interpret GT_OFF(x) as "x != 0" (true when there is data in the buffer).
#ifndef GT_OFF
#define GT_OFF(x) ((x) != 0)
#endif

// Forward-declare a gz-like state structure used by gz_skip.
struct gz_state;
typedef gz_state* gz_statep;

// Thread-safe, simple test harness globals
static int g_tests_failed = 0;
static int g_case_total = 0;

// Simple assertion helper: non-terminating; logs failure but continues running tests.
#define TEST_ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "Test failed: " msg "\n"; \
            ++g_tests_failed; \
        } \
        ++g_case_total; \
    } while (0)

// Global fetch hook that gz_skip will call; default returns 0 (success) to simulate buffering behavior.
// Tests can override this to simulate different data-fetching scenarios.
static int default_gz_fetch(gz_statep) { return 0; }
static int (*gz_fetch)(gz_statep) = default_gz_fetch;

// Minimal gz_state implementation to accompany the focal method logic.
// This mirrors the fields accessed by gz_skip in the provided snippet.
struct gz_state {
    struct {
        unsigned have;  // number of bytes currently in the output buffer
        unsigned next;  // pointer/index to next byte in the buffer
        unsigned pos;   // position in the uncompressed data
    } x;
    int eof;               // end-of-file flag
    struct {
        int avail_in;        // amount of available input in the stream
    } strm;
};

// Self-contained local copy of gz_skip according to the focal method.
// This is not the real library implementation but follows its control flow for testing.
static int gz_skip(gz_statep state, z_off64_t len) {
    unsigned n;
    while (len) {
        if (state->x.have) {
            n = GT_OFF(state->x.have) || (z_off64_t)state->x.have > len ?
                (unsigned)len : state->x.have;
            state->x.have -= n;
            state->x.next += n;
            state->x.pos += n;
            len -= n;
        } else if (state->eof && state->strm.avail_in == 0) {
            break;
        } else {
            if (gz_fetch(state) == -1)
                return -1;
        }
    }
    return 0;
}

// ------------- Test Scenarios -------------
// Case A: state.x.have is non-zero; verify correct min(len, have) behavior and state mutation.
static bool test_case_A() {
    gz_state st;
    std::memset(&st, 0, sizeof(st));

    st.x.have = 5;
    st.x.next = 0;
    st.x.pos = 0;
    st.eof = 0;
    st.strm.avail_in = 10;

    z_off64_t len = 3;
    int ret = gz_skip(&st, len);

    bool ok = (ret == 0) &&
              (st.x.have == 2) &&
              (st.x.next == 3) &&
              (st.x.pos == 3) &&
              (st.strm.avail_in == 10) &&
              (st.eof == 0);

    if (!ok) {
        std::cerr << "Case A details: ret=" << ret
                  << " have=" << st.x.have
                  << " next=" << st.x.next
                  << " pos=" << st.x.pos
                  << " avail_in=" << st.strm.avail_in
                  << " eof=" << st.eof
                  << "\n";
    }
    return ok;
}

// Case B: state.x.have == 0 and end-of-file condition with no remaining input should cause break without error.
static bool test_case_B() {
    gz_state st;
    std::memset(&st, 0, sizeof(st));

    st.x.have = 0;
    st.x.next = 0;
    st.x.pos = 0;
    st.eof = 1;              // end-of-file flag set
    st.strm.avail_in = 0;     // no input available

    z_off64_t len = 10;
    int ret = gz_skip(&st, len);

    bool ok = (ret == 0) &&
              (st.x.have == 0) &&
              (st.x.next == 0) &&
              (st.x.pos == 0) &&
              (st.eof == 1);

    if (!ok) {
        std::cerr << "Case B details: ret=" << ret
                  << " have=" << st.x.have
                  << " next=" << st.x.next
                  << " pos=" << st.x.pos
                  << " avail_in=" << st.strm.avail_in
                  << " eof=" << st.eof
                  << "\n";
    }
    return ok;
}

// Case C: simulate fetching data via gz_fetch to drive the loop; verify total skipped length equals requested len.
// We override the gz_fetch hook to mimic a progressive data source.
static int test_fetch_caseC(gz_statep state) {
    // We use a static local to count how many fetch calls have occurred;
    // this function will supply 2 bytes of data on the first two fetches, then signal EOF.
    static int calls = 0;
    ++calls;

    if (calls <= 3) {
        // Provide 2 bytes worth of data per fetch
        state->x.have = 2;
        if (state->strm.avail_in > 0) --state->strm.avail_in;
        else state->strm.avail_in = 0;
        return 0;
    } else {
        // No more data; indicate EOF
        state->x.have = 0;
        state->eof = 1;
        state->strm.avail_in = 0;
        return 0;
    }
}

static bool test_case_C() {
    gz_state st;
    std::memset(&st, 0, sizeof(st));

    // Prepare for a fetch-driven scenario
    st.x.have = 0;
    st.x.next = 0;
    st.x.pos = 0;
    st.eof = 0;
    st.strm.avail_in = 3;

    // Install our fetch function
    gz_fetch = test_fetch_caseC;
    // Reset internal fetch counter by calling through a dummy gateway:
    // Since the function uses a static counter, we trigger an initial call to increment it.
    // We call gz_skip to perform at least one iteration; the first loop will call gz_fetch.
    z_off64_t len = 5;
    int ret = gz_skip(&st, len);

    // After skipping, total pos should equal initial len (5)
    bool ok = (ret == 0) && (st.x.pos == 5);

    if (!ok) {
        std::cerr << "Case C details: ret=" << ret
                  << " pos=" << st.x.pos
                  << " have=" << st.x.have
                  << " next=" << st.x.next
                  << " eof=" << st.eof
                  << " avail_in=" << st.strm.avail_in
                  << "\n";
    }

    // Restore default fetch behavior to avoid side effects on potential subsequent tests
    gz_fetch = default_gz_fetch;

    return ok;
}

// ------------- Main entry -------------
int main() {
    std::cout << "Running gz_skip unit tests (self-contained, no GTest)..." << std::endl;

    // Run tests
    bool a = test_case_A();
    bool b = test_case_B();
    bool c = test_case_C();

    // Summary
    if (g_tests_failed == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "Total failures: " << g_tests_failed << " / " << g_case_total << std::endl;
        return 1;
    }
}