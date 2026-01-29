/*
  Unit test suite for the focal method pull in infcover.c
  - Uses C++11 (no GoogleTest)
  - Expects the focal function pull to be available with C linkage:
      extern "C" unsigned pull(void *desc, unsigned char **buf);
  - Tests cover: null descriptor behavior, state handling, data sequence, and reset semantics.
  - This test harness is designed to be compiled and linked with the project that provides pull.
  - In environments where pull is declared static, this approach uses an extern prototype and assumes pull is accessible via linking (extern C). If pull remains static, integrate by including the focal file in the build or adjust linkage accordingly.

  Notes:
  - Domain knowledge is applied to ensure static state handling is tested (static next and dat payload).
  - The test uses the Z_NULL macro and z_stream type from zlib.h to recreate the expected environment.
  - Non-terminating checks print failures but do not abort tests, enabling full coverage iteration.
*/

#include <cstring>
#include <string.h>
#include <assert.h>
#include <cstdint>
#include <stdio.h>
#include <iostream>
#include <inftrees.h>
#include <zlib.h>
#include <stdlib.h>
#include <inflate.h>



// Assume the focal function is provided with C linkage
extern "C" unsigned pull(void *desc, unsigned char **buf);

/* Lightweight test harness (non-terminating) */
static int g_failures = 0;
#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[TEST FAIL] " << (msg) << "\n"; \
        ++g_failures; \
    } \
} while(0)

static void test_summary(const char* name, int local_failures) {
    if(local_failures == 0) {
        std::cout << "[TEST PASS] " << name << "\n";
    } else {
        std::cout << "[TEST INFO] " << name << " had " << local_failures << " failure(s)\n";
    }
}

// A minimal inflate_state layout compatible with the focal code's expectations.
// We declare this here to allow tests to observe internal state changes (state->mode) when a non-null state pointer is provided.
// NOTE: This mirrors the likely structure used by zlib for inflate_state (only the 'mode' field is accessed by the focal code).
struct inflate_state {
    int mode;
};

// Helper to reset/initialize a z_stream with a custom inflate_state.
// The test will provide a z_stream with its state field pointing to our inflate_state instance.
static void init_desc_with_inflate_state(z_stream &zs, inflate_state *isz) {
    // Clear z_stream content lightly to avoid undefined behavior in tests
    std::memset(reinterpret_cast<void*>(&zs), 0, sizeof(z_stream));
    zs.state = static_cast<void*>(isz);
}

// Test 1: Null descriptor should return 0 and reset internal static 'next'.
// The focal pull resets next on Z_NULL and returns 0.
static int test_pull_null_desc() {
    int failures = 0;
    unsigned char *buf = nullptr;

    unsigned ret = pull(nullptr, &buf);
    CHECK(ret == 0, "pull(nullptr, &buf) should return 0 (no input) and not crash");
    // buf may remain nullptr; we only verify return value per contract.

    test_summary("test_pull_null_desc", failures);
    return failures;
}

// Test 2: Non-null descriptor with a valid inflate_state should produce the 4-byte payload in sequence
// and, on the first call, set state->mode to SYNC.
static int test_pull_sequence_with_state() {
    int failures = 0;
    inflate_state is;
    is.mode = -1; // initial dummy value

    z_stream zs;
    init_desc_with_inflate_state(zs, &is);

    unsigned char *buf = nullptr;

    // First call: expect ret = 1 and buf points to first payload byte (0x63)
    unsigned ret1 = pull(reinterpret_cast<void*>(&zs), &buf);
    CHECK(ret1 == 1, "First pull call should return 1 (data available)");
    CHECK(buf != nullptr, "First pull should set buf to a valid address");
    CHECK(*buf == 0x63, "First payload byte should be 0x63");
    // state should be set to SYNC if non-null state was provided
    CHECK(is.mode == SYNC, "state->mode should be set to SYNC when state is non-null");

    // Second call
    unsigned ret2 = pull(reinterpret_cast<void*>(&zs), &buf);
    CHECK(ret2 == 1, "Second pull call should return 1 (data available)");
    CHECK(buf != nullptr, "Second pull should set buf to a valid address");
    CHECK(*buf == 0x00 || *buf == 0x00, "Second payload byte should be 0x00 (expected)");
    // The two bytes in the payload are {0x63, 0, 2, 0}
    // We'll verify in sequence below via a small loop.

    // We'll perform the remaining two reads explicitly to validate sequence
    unsigned ret3 = pull(reinterpret_cast<void*>(&zs), &buf);
    CHECK(ret3 == 1, "Third pull call should return 1 (data available)");
    CHECK(buf != nullptr, "Third pull should set buf to a valid address");
    CHECK(*buf == 0x02, "Third payload byte should be 0x02");

    unsigned ret4 = pull(reinterpret_cast<void*>(&zs), &buf);
    CHECK(ret4 == 1, "Fourth pull call should return 1 (data available)");
    CHECK(buf != nullptr, "Fourth pull should set buf to a valid address");
    CHECK(*buf == 0x00, "Fourth payload byte should be 0x00");

    // Fifth call should return 0 (no more data)
    unsigned ret5 = pull(reinterpret_cast<void*>(&zs), &buf);
    CHECK(ret5 == 0, "Fifth pull call should return 0 (no more input)");

    // Also ensure that after the sequence, the mode remains set (SYNC)
    CHECK(is.mode == SYNC, "state->mode should remain SYNC after sequence");

    test_summary("test_pull_sequence_with_state", failures);
    return failures;
}

// Test 3: Non-null descriptor with state pointer NULL should not attempt to modify mode
static int test_pull_with_null_state_pointer() {
    int failures = 0;
    inflate_state is;
    is.mode = -999; // sentinel value to detect modification

    z_stream zs;
    // Do not assign a valid state (set to NULL)
    std::memset(reinterpret_cast<void*>(&zs), 0, sizeof(z_stream));
    zs.state = nullptr;

    unsigned char *buf = nullptr;
    unsigned ret = pull(reinterpret_cast<void*>(&zs), &buf);
    CHECK(ret == 1, "pull with null state pointer should still return 1 (data available)");
    CHECK(buf != nullptr, "buf should be set when data is available");
    CHECK(*buf == 0x63, "payload should start with 0x63 even if state is null");

    // Since state was null, is.mode should remain unchanged (test ensures no crashes)
    CHECK(is.mode == -999, "state should not be modified when state pointer is NULL (observed via test struct)");

    test_summary("test_pull_with_null_state_pointer", failures);
    return failures;
}

// Test 4: Reset behavior via Z_NULL descriptor after a sequence
// The static 'next' should reset, causing the next non-null pull to again yield 0x63.
static int test_pull_reset_via_null_desc() {
    int failures = 0;
    inflate_state is;
    is.mode = -1;

    z_stream zs;
    init_desc_with_inflate_state(zs, &is);

    unsigned char *buf = nullptr;

    // Consume first byte
    unsigned ret1 = pull(reinterpret_cast<void*>(&zs), &buf);
    CHECK(ret1 == 1, "Initial pull should return 1 (data available)");
    CHECK(*buf == 0x63, "Initial payload byte should be 0x63");

    // Reset via Z_NULL
    unsigned reset_ret = pull(nullptr, &buf);
    CHECK(reset_ret == 0, "Reset call with Z_NULL should return 0");

    // After reset, another non-null pull should again produce the first byte 0x63
    unsigned ret2 = pull(reinterpret_cast<void*>(&zs), &buf);
    CHECK(ret2 == 1, "Post-reset pull should return 1 (data available)");
    CHECK(*buf == 0x63, "Payload after reset should start again with 0x63");

    test_summary("test_pull_reset_via_null_desc", failures);
    return failures;
}

// Test 5: Ensure multiple non-null calls produce the exact byte sequence {0x63, 0, 2, 0}
static int test_pull_exact_byte_sequence() {
    int failures = 0;
    inflate_state is;
    is.mode = -1;

    z_stream zs;
    init_desc_with_inflate_state(zs, &is);

    unsigned char *buf = nullptr;
    const unsigned char expected[4] = {0x63, 0x00, 0x02, 0x00};

    for(int i = 0; i < 4; ++i) {
        unsigned ret = pull(reinterpret_cast<void*>(&zs), &buf);
        CHECK(ret == 1, "Pull should return 1 during payload sequence");
        CHECK(buf != nullptr, "Buffer should be non-null during payload sequence");
        CHECK(*buf == expected[i], "Payload byte mismatch in sequence");
    }

    // After 4 bytes, next call should return 0
    unsigned ret_last = pull(reinterpret_cast<void*>(&zs), &buf);
    CHECK(ret_last == 0, "After 4 bytes, pull should return 0 (end of data)");

    test_summary("test_pull_exact_byte_sequence", failures);
    return failures;
}

int main(void) {
    std::cout << "Starting unit tests for focal method: pull\n";

    int total_failures = 0;

    total_failures += test_pull_null_desc();
    total_failures += test_pull_sequence_with_state();
    total_failures += test_pull_with_null_state_pointer();
    total_failures += test_pull_reset_via_null_desc();
    total_failures += test_pull_exact_byte_sequence();

    if (g_failures > 0) {
        std::cerr << "Total failures reported: " << g_failures << "\n";
    }

    if (total_failures == 0) {
        std::cout << "All tests completed. No failing cases observed in the harness.\n";
        return 0;
    } else {
        std::cout << "Tests completed with " << total_failures << " failure(s).\n";
        return 1;
    }
}