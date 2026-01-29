// C++11 test suite for the focal method gztell64 in gzlib.c
// This test suite is self-contained: it includes the C source under test
// and uses a lightweight test harness (no Google Test).
// The tests exercise critical branches of gztell64:
//  - NULL input returns -1
//  - invalid mode returns -1
//  - valid modes (GZ_READ/GZ_WRITE) return state.x.pos (+ optional skip when state.seek is true)

#include <iostream>
#include <gzguts.h>
#include <gzlib.c>
#include <cstring>


// Bring in C code under test with C linkage
extern "C" {
}

// If gzlib.c defines gzFile as a specific pointer type, we assume it is a pointer type.
// We declare a prototype for gztell64 in a way that's compatible with typical zlib typedefs.
// The following prototype mirrors the common signature: z_off64_t gztell64(gzFile file);
extern "C" {
    //z_off64_t gztell64(gzFile file);
    // We avoid relying on another typedef here; instead, use a generic pointer type as gzFile.
    // This relies on gzFile being some pointer type (commonly void* or similar) in the actual library.
    typedef void* gzFile;
    typedef long long z_off64_t;
    z_off64_t gztell64(gzFile file);
}

// Lightweight test utilities
static int g_total = 0;
static int g_passed = 0;

#define TEST_PASSED() do { ++g_passed; std::cout << "[PASS] " << __func__ << "\n"; } while (0)
#define TEST_FAILED(msg) do { std::cerr << "[FAIL] " << __func__ << ": " << msg << "\n"; ++g_total; } while (0)
#define RUN_TEST(test_fn) do { if ((test_fn)()) TEST_PASSED(); else TEST_FAILED(#test_fn); ++g_total; } while (0)

// Helper to print test results meta
static void print_summary() {
    std::cout << "\nTest Summary: " << g_passed << "/" << g_total << " tests passed.\n";
}

// Test 1: gztell64(NULL) should return -1
static bool test_gztell64_null_input() {
    z_off64_t result = gztell64(nullptr);
    return result == -1;
}

// Test 2: gztell64 should return -1 for an invalid mode
static bool test_gztell64_invalid_mode() {
    gz_state state;
    // Ensure deterministic state
    std::memset(&state, 0, sizeof(state));

    // Set an invalid mode (not GZ_READ and not GZ_WRITE)
    state.mode = -1234; // clearly invalid
    state.x.pos = 0;
    state.seek = 0;
    state.skip = 0;

    gzFile f = (gzFile)&state;
    z_off64_t result = gztell64(f);
    return result == -1;
}

// Test 3: gztell64 with valid mode (GZ_READ) and zero seek/skip should return pos (0)
static bool test_gztell64_valid_read_pos_zero() {
    gz_state state;
    std::memset(&state, 0, sizeof(state));

    state.mode = GZ_READ;
    state.x.pos = 0;
    state.seek = 0;
    state.skip = 0;

    gzFile f = (gzFile)&state;
    z_off64_t result = gztell64(f);
    return result == 0;
}

// Test 4: gztell64 should add skip when seek is true
static bool test_gztell64_seek_and_skip() {
    gz_state state;
    std::memset(&state, 0, sizeof(state));

    state.mode = GZ_READ;
    state.x.pos = 12345;
    state.seek = 1;   // simulate a seek operation occurred
    state.skip = 7;   // additional skipped bytes after seek

    gzFile f = (gzFile)&state;
    z_off64_t result = gztell64(f);
    return result == 12345 + 7;
}

// Test 5: gztell64 with GZ_WRITE mode should also honor skip when seek is true
static bool test_gztell64_write_mode_with_seek() {
    gz_state state;
    std::memset(&state, 0, sizeof(state));

    state.mode = GZ_WRITE;
    state.x.pos = 500;
    state.seek = 1;
    state.skip = 20;

    gzFile f = (gzFile)&state;
    z_off64_t result = gztell64(f);
    return result == 520;
}

// Entry point
int main() {
    // Run tests
    RUN_TEST(test_gztell64_null_input);
    RUN_TEST(test_gztell64_invalid_mode);
    RUN_TEST(test_gztell64_valid_read_pos_zero);
    RUN_TEST(test_gztell64_seek_and_skip);
    RUN_TEST(test_gztell64_write_mode_with_seek);

    print_summary();

    // Return non-zero if any test failed
    return (g_total == g_passed) ? 0 : 1;
}