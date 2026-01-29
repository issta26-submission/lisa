// Test suite for the focal method adler32_z
// This test suite is implemented in C++11 without any external testing framework.
// It exercises key control-flow paths of adler32_z and compares results against a
// straightforward reference implementation of Adler-32.
// Note: The actual adler32_z implementation is linked in (from adler32.c with zutil.h).
// The tests focus on functional correctness and coverage of branches.

#include <cstring>
#include <cstddef>
#include <iostream>
#include <vector>
#include <zutil.h>


// Declare the C function under test (avoid C++ name mangling)
extern "C" unsigned long adler32_z(unsigned long adler, const unsigned char *buf, size_t len);

// Reference implementation of Adler-32 to validate adler32_z
static unsigned long Adler32Ref(unsigned long adler, const unsigned char* buf, size_t len)
{
    const unsigned long BASE = 65521UL;
    unsigned long A = adler & 0xFFFF;            // lower 16 bits
    unsigned long B = (adler >> 16) & 0xFFFF;    // upper 16 bits

    for (size_t i = 0; i < len; ++i) {
        A += static_cast<unsigned long>(buf[i]);
        if (A >= BASE) A -= BASE;
        B += A;
        if (B >= BASE) B -= BASE;
    }
    return (B << 16) | A;
}

// Simple non-terminating test harness (collects pass/fail without aborting)
static bool test_len1_path()
{
    // Branch: len == 1
    // adler starts with 1 (A=1, B=0), buf[0] = 1
    // A' = 1 + 1 = 2; B' = 0 + A' = 2; result = 0x00020002
    unsigned char b = 1;
    unsigned long res = adler32_z(1, &b, 1);
    unsigned long expected = 0x00020002UL; // 2 | (2 << 16)
    bool ok = (res == expected);
    if (!ok) {
        std::cerr << "[TEST_LEN1] FAIL: got " << res << " expected " << expected << "\n";
    }
    return ok;
}

static bool test_null_buf_path()
{
    // Branch: buf == Z_NULL (null pointer)
    // Should return 1 (the initial Adler value with B=0 and A=1)
    unsigned long res = adler32_z(1, nullptr, 12345);
    bool ok = (res == 1UL);
    if (!ok) {
        std::cerr << "[TEST_NULL_BUF] FAIL: got " << res << " expected 1\n";
    }
    return ok;
}

static bool test_len_small_path()
{
    // Branch: len < 16 and non-null buf
    // Use a small 10-byte payload to exercise incremental accumulation
    unsigned char buf[10] = {0,1,2,3,4,5,6,7,8,9};
    unsigned long res = adler32_z(1, buf, 10);
    unsigned long expected = Adler32Ref(1, buf, 10);
    bool ok = (res == expected);
    if (!ok) {
        std::cerr << "[TEST_LEN_SMALL] FAIL: got " << res << " expected " << expected << "\n";
    }
    return ok;
}

static bool test_large_block_path()
{
    // Branch: Large len triggers NMAX-block processing
    const size_t LEN = 6000; // >= NMAX (typically 5552)
    std::vector<unsigned char> buf(LEN);
    for (size_t i = 0; i < LEN; ++i) {
        buf[i] = static_cast<unsigned char>(i & 0xFF);
    }
    unsigned long res = adler32_z(1, buf.data(), LEN);
    unsigned long expected = Adler32Ref(1, buf.data(), LEN);
    bool ok = (res == expected);
    if (!ok) {
        std::cerr << "[TEST_LARGE_BLOCK] FAIL: got " << res << " expected " << expected << "\n";
    }
    return ok;
}

static bool test_large_block_with_remainder_path()
{
    // Branch: After processing one large block, test remaining bytes (< NMAX)
    // Use LEN = NMAX + 20 to ensure remainder path is taken.
    const size_t LEN = 5552 + 20;
    std::vector<unsigned char> buf(LEN);
    for (size_t i = 0; i < LEN; ++i) {
        buf[i] = static_cast<unsigned char)((i * 7) & 0xFF);
    }
    unsigned long res = adler32_z(1, buf.data(), LEN);
    unsigned long expected = Adler32Ref(1, buf.data(), LEN);
    bool ok = (res == expected);
    if (!ok) {
        std::cerr << "[TEST_LARGE_REMAINDER] FAIL: got " << res << " expected " << expected << "\n";
    }
    return ok;
}

static bool test_non_default_initial_adler()
{
    // Additional coverage: use a non-default initial adler value
    // adler starts with 0 (A=0, B=0), process a small payload
    unsigned char buf[5] = {10, 20, 30, 40, 50};
    unsigned long res = adler32_z(0, buf, 5);
    unsigned long expected = Adler32Ref(0, buf, 5);
    bool ok = (res == expected);
    if (!ok) {
        std::cerr << "[TEST_NONDEFAULT_ADLER] FAIL: got " << res << " expected " << expected << "\n";
    }
    return ok;
}

int main()
{
    int total = 0;
    int passed = 0;

    std::cout << "Running adler32_z unit tests (no GTest, C++11):\n";

    if (test_len1_path()) { ++passed; } ++total;
    if (test_null_buf_path()) { ++passed; } ++total;
    if (test_len_small_path()) { ++passed; } ++total;
    if (test_large_block_path()) { ++passed; } ++total;
    if (test_large_block_with_remainder_path()) { ++passed; } ++total;
    if (test_non_default_initial_adler()) { ++passed; } ++total;

    std::cout << "Tests passed: " << passed << "/" << total << "\n";

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}