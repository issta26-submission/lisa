/*
Unit test suite for the focal method unz64local_getLong64 from unzip.c
- Approach: Include unzip.c directly in this test TU so that static/local functions
  inside unzip.c can be exercised. We override key macros (ZREAD64, ZERROR64)
  with test harness implementations to simulate I/O behavior without touching
  the real zlib/zip file system.
- The tests cover:
  1) Normal path: exactly 8 bytes read -> success (UNZ_OK) and correct 64-bit value.
  2) EOF path: fewer than 8 bytes read with ZERROR64 returning 0 -> UNZ_EOF and pX = 0.
  3) Errno path: fewer than 8 bytes read with ZERROR64 returning non-zero -> UNZ_ERRNO and pX = 0.
- This test harness uses no external testing framework (GTest not used), and keeps
  assertions non-terminating (prints results and continues).
- The test code is designed to compile with a C++11 compiler.

Notes:
- unzip.c is included with a minimal test macro override:
  ZREAD64(...) -> fake_read(...) and ZERROR64(...) -> (g_errno != 0)
- fake_read copies bytes from an internal test buffer to the destination buffer.
- We pass a dummy zlib_filefunc64_32_def object to the function under test; its
  actual content is ignored by the test harness (we only validate the behavior).

Usage:
- Compile this file together with unzip.c (the test TU includes unzip.c).
- Run the resulting binary. It will print test results and return 0 if all tests pass.

*/
#include <cstring>
#include <string.h>
#include <cstdint>
#include <stdio.h>
#include <crypt.h>
#include <iostream>
#include <unzip.c>
#include <zlib.h>
#include <stdlib.h>
#include <unzip.h>


// Provide a minimal test harness environment for unzip.c to compile and run.
// We override the ZREAD64 and ZERROR64 macros to control behavior without needing
// a full zlib filesystem implementation.

static unsigned char g_read_buf[256];      // test data buffer
static unsigned long long g_read_len = 0;  // length of valid data in g_read_buf
static unsigned long long g_read_pos = 0;  // current read position
static int g_errno = 0;                   // 0 => no errno, non-zero => errno

// Provide a fake reader that the macro ZREAD64 will call.
// It copies up to 'len' bytes from g_read_buf into 'buf' and returns the number
// of bytes actually read. If no data left, returns 0.
static int fake_read(unsigned char* buf, unsigned long long len)
{
    unsigned long long remaining = (g_read_len > g_read_pos) ? (g_read_len - g_read_pos) : 0;
    unsigned long long to_copy = (len < remaining) ? len : remaining;
    if (to_copy > 0)
    {
        std::memcpy(buf, g_read_buf + g_read_pos, (size_t)to_copy);
        g_read_pos += to_copy;
        return (int)to_copy;
    }
    return 0;
}

// Provide the ZERROR64 macro override.
// We just check a global flag (g_errno) to decide whether an error condition is raised.
static int test_ZERROR_OVERRIDE_dummy(void* /*def*/, void* /*filestream*/)
{
    return g_errno != 0;
}

// Forward declarations for test harness (macros refer to these replacements).
// We redefine the macros immediately before including unzip.c, so actual
// function calls inside unzip.c are redirected to our test harness.
//
// Note: The following macro redefinitions are to be placed before including unzip.c.
// We simulate:
// - ZREAD64(...): call fake_read by ignoring the first argument (def) and passing buf/len.
// - ZERROR64(...): call test_ZERROR_OVERRIDE_dummy and use its boolean result.
// The types used by unzip.c (e.g., zlib_filefunc64_32_def and ZPOS64_T) are
// provided by the included unzip.c along with zlib.h; our tests pass a dummy
// object where needed.

// Redefine macros (will take effect for the included unzip.c)
#define ZREAD64(def, stream, buf, len) fake_read((unsigned char*)(buf), (unsigned long long)(len))
#define ZERROR64(def, stream) test_ZERROR_OVERRIDE_dummy(NULL, NULL)

extern "C" {
    // Expose the fake_read function with C linkage for compatibility with unzip.c
    int fake_read(unsigned char* buf, unsigned long long len);
    // Expose the dummy ZERROR function as well if unzip.c references a symbol.
    int test_ZERROR_OVERRIDE_dummy(void* def, void* stream);
}

// Implement the test harness functions declared above
int fake_read(unsigned char* buf, unsigned long long len)
{
    // Copy from the test buffer (g_read_buf) into the provided destination
    unsigned long long remaining = (g_read_len > g_read_pos) ? (g_read_len - g_read_pos) : 0;
    unsigned long long to_copy = (len < remaining) ? len : remaining;
    if (to_copy > 0)
    {
        std::memcpy(buf, g_read_buf + g_read_pos, (size_t)to_copy);
        g_read_pos += to_copy;
        return (int)to_copy;
    }
    return 0;
}

// The test ZERROR override (used by unzip.c)
int test_ZERROR_OVERRIDE_dummy(void* def, void* stream) { (void)def; (void)stream; return g_errno != 0; }

// Helper test utilities
static void setup_read_bytes(const unsigned char* data, unsigned long long len)
{
    std::memcpy(g_read_buf, data, (size_t)len);
    g_read_len = len;
    g_read_pos = 0;
}

// Simple non-terminating assertion helper
static bool expect_eq_uint64(uint64_t a, uint64_t b, const char* msg)
{
    if (a != b) {
        std::cerr << "FAILED: " << msg << " - expected " << b << ", got " << a << std::endl;
        return false;
    }
    return true;
}

static bool expect_eq_int(int a, int b, const char* msg)
{
    if (a != b) {
        std::cerr << "FAILED: " << msg << " - expected " << b << ", got " << a << std::endl;
        return false;
    }
    return true;
}

// Include the unzip.c with test macro overrides.
// The include is placed after the macro redefinitions to ensure the proper behavior.

// The unzip.c logic will now be compiled in this translation unit, with the
// fake_read and test ZERROR logic applied.

// We also need a dummy instance for pzlib_filefunc_def parameter. The actual
// type is provided by unzip.h (via zlib.h); we just create a local one to pass to
// the function under test. Its internal fields are ignored by our test harness.
static struct zlib_filefunc64_32_def g_dummy_def;


int main() {
    int all_ok = 1;

    // Test 1: Successful 8-byte read path
    {
        // 8 bytes little-endian value: 0x0807060504030201
        unsigned char data8[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
        setup_read_bytes(data8, 8);
        g_errno = 0; // not used in success path, but set for completeness

        ZPOS64_T result = 0;
        int ret = unz64local_getLong64(&g_dummy_def, NULL, &result);

        uint64_t expected = 0x0807060504030201ULL;
        bool ok = true;
        ok &= expect_eq_int(ret, UNZ_OK, "unz64local_getLong64 should return UNZ_OK on 8-byte read");
        ok &= expect_eq_uint64((uint64_t)result, expected, "unz64local_getLong64 should assemble 64-bit value correctly (little-endian)");
        if (!ok) all_ok = 0;
        std::cout << "Test 1 (success path) " << (ok ? "PASSED" : "FAILED") << std::endl;
    }

    // Test 2: EOF path (fewer than 8 bytes read, errno not set)
    {
        unsigned char data3[3] = {0xAA, 0xBB, 0xCC};
        setup_read_bytes(data3, 3);
        g_errno = 0; // EOF scenario
        ZPOS64_T result = 0;
        int ret = unz64local_getLong64(&g_dummy_def, NULL, &result);

        bool ok = true;
        ok &= expect_eq_int(ret, UNZ_EOF, "unz64local_getLong64 should return UNZ_EOF when bytes < 8 and no errno");
        ok &= expect_eq_uint64((uint64_t)result, 0ULL, "unz64local_getLong64 should set output to 0 on EOF");
        if (!ok) all_ok = 0;
        std::cout << "Test 2 (EOF path) " << (ok ? "PASSED" : "FAILED") << std::endl;
    }

    // Test 3: Errno path (fewer than 8 bytes read, errno set)
    {
        unsigned char data3b[3] = {0xDE, 0xAD, 0xBE};
        setup_read_bytes(data3b, 3);
        g_errno = 1; // errno path
        ZPOS64_T result = 0;
        int ret = unz64local_getLong64(&g_dummy_def, NULL, &result);

        bool ok = true;
        ok &= expect_eq_int(ret, UNZ_ERRNO, "unz64local_getLong64 should return UNZ_ERRNO when errno is set");
        ok &= expect_eq_uint64((uint64_t)result, 0ULL, "unz64local_getLong64 should set output to 0 on errno");
        if (!ok) all_ok = 0;
        std::cout << "Test 3 (errno path) " << (ok ? "PASSED" : "FAILED") << std::endl;
    }

    return all_ok ? 0 : 1;
}