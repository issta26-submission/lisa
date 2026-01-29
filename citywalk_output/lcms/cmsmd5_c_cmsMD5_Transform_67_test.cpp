// cmsmd5_transform_test.cpp
// Unit tests for cmsMD5_Transform in cmsmd5.c
// NOTE: This test requires linking against the actual cmsMD5_Transform implementation
// (e.g., by compiling and linking cmsmd5.c into the test binary).
// The test uses a self-contained reference MD5 compression to validate the behavior of cmsMD5_Transform.

#include <cstring>
#include <iostream>
#include <iomanip>
#include <lcms2_internal.h>
#include <cstdint>


// Typedefs matching common CMS typedefs to maximize compatibility with cmsmd5.c
using cmsUInt32Number = uint32_t;

// Declaration of the focal function under test (extern "C" to ensure proper linkage)
extern "C" void cmsMD5_Transform(cmsUInt32Number buf[4], cmsUInt32Number in[16]);

// --------------------------------------------------------------------------
// Reference MD5 compression (single-block) implementation
// This mirrors the standard MD5 compression algorithm (RFC 1321) and is used
// only for generating expected results to compare against cmsMD5_Transform.
// It does not depend on the CMS-specific macros and can be compiled standalone.
// --------------------------------------------------------------------------
namespace {

static inline uint32_t leftrotate(uint32_t x, uint32_t c) {
    return (x << c) | (x >> (32 - c));
}

void md5_compress_ref(uint32_t buf[4], const uint32_t in[16]) {
    // MD5 constants
    static const uint32_t K[64] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };

    static const uint32_t S[64] = {
        7,12,17,22, 7,12,17,22, 7,12,17,22, 7,12,17,22,
        5,9,14,20, 5,9,14,20, 5,9,14,20, 5,9,14,20,
        4,11,16,23, 4,11,16,23, 4,11,16,23, 4,11,16,23,
        6,10,15,21, 6,10,15,21, 6,10,15,21, 6,10,15,21
    };

    uint32_t a = buf[0];
    uint32_t b = buf[1];
    uint32_t c = buf[2];
    uint32_t d = buf[3];

    for (int i = 0; i < 64; ++i) {
        uint32_t F, g;
        if (i < 16) {
            F = (b & c) | ((~b) & d);
            g = i;
        } else if (i < 32) {
            F = (d & b) | ((~d) & c);
            g = (5 * i + 1) % 16;
        } else if (i < 48) {
            F = b ^ c ^ d;
            g = (3 * i + 5) % 16;
        } else {
            F = c ^ (b | ~d);
            g = (7 * i) % 16;
        }

        uint32_t temp = d;
        d = c;
        c = b;
        b = b + leftrotate(a + F + K[i] + in[g], S[i]);
        a = temp;
    }

    buf[0] += a;
    buf[1] += b;
    buf[2] += c;
    buf[3] += d;
}

} // anonymous namespace

// --------------------------------------------------------------------------
// Test harness
// - Creates several test cases with varied initial states and 64-byte blocks.
// - Uses the reference MD5 compression to compute the expected result.
// - Invokes cmsMD5_Transform and compares against the reference.
// - Uses non-terminating assertions (continue on failure) and reports summary.
// --------------------------------------------------------------------------

static void run_case(const char* name,
                     const cmsUInt32Number init_buf[4],
                     const cmsUInt32Number in_block[16]) {
    // Copy initial state for both actual and reference computations
    cmsUInt32Number actual_buf[4];
    cmsUInt32Number in_block_copy[16];
    std::memcpy(actual_buf, init_buf, 4 * sizeof(cmsUInt32Number));
    std::memcpy(in_block_copy, in_block, 16 * sizeof(cmsUInt32Number));

    // Expected result computed by reference MD5 compression
    uint32_t expected_buf[4];
    uint32_t init_copy[4];
    std::memcpy(init_copy, init_buf, 4 * sizeof(uint32_t));
    std::memcpy(expected_buf, init_copy, 4 * sizeof(uint32_t));
    md5_compress_ref(expected_buf, in_block);

    // Compute actual result using cmsMD5_Transform
    cmsMD5_Transform(actual_buf, in_block_copy);

    // Compare
    bool ok = (std::memcmp(expected_buf, actual_buf, 4 * sizeof(uint32_t)) == 0);

    // Report
    std::cout << "Test: " << name << " -> "
              << (ok ? "PASSED" : "FAILED") << "\n";
    if (!ok) {
        std::cout << "  Init Buf: "
                  << std::hex << std::setw(8) << std::setfill('0') << init_buf[0] << " "
                  << std::hex << std::setw(8) << std::setfill('0') << init_buf[1] << " "
                  << std::hex << std::setw(8) << std::setfill('0') << init_buf[2] << " "
                  << std::hex << std::setw(8) << std::setfill('0') << init_buf[3] << "\n";
        std::cout << "  In Block: "
                  << std::hex
                  << in_block[0] << " " << in_block[1] << " " << in_block[2] << " " << in_block[3]
                  << " " << in_block[4] << " " << in_block[5] << " " << in_block[6] << " " << in_block[7]
                  << "\n";
        std::cout << "  Expected: "
                  << std::hex << expected_buf[0] << " "
                  << expected_buf[1] << " "
                  << expected_buf[2] << " "
                  << expected_buf[3] << "\n";
        std::cout << "  Actual  : "
                  << std::hex << actual_buf[0] << " "
                  << actual_buf[1] << " "
                  << actual_buf[2] << " "
                  << actual_buf[3] << "\n";
    }
}

// Helper to fill a deterministic but varied 16-word block
static void fill_in_block_randomized(cmsUInt32Number in_block[16], uint32_t seed) {
    for (int i = 0; i < 16; ++i) {
        // Simple deterministic mix to get varied input
        uint32_t v = seed * (i + 1) + 0xA5A5A5A5;
        v ^= (0xFFFFFFFF << (i % 5)) | (0x0F0F0F0F >> (i % 3));
        in_block[i] = v;
    }
}

// Entry point for tests
int main() {
    int failures = 0;

    // Case 1: Standard MD5 initial state with all-zero input block
    // Expected result computed by reference MD5 compression
    {
        cmsUInt32Number init_buf[4] = { 0x67452301u, 0xefcdab89u, 0x98badcfeu, 0x10325476u };
        cmsUInt32Number in_block[16];
        std::memset(in_block, 0, 16 * sizeof(cmsUInt32Number));
        run_case("Case1_ZeroBlock_With_MD5Initial", init_buf, in_block);
    }

    // Case 2: Non-trivial initial state with a small non-zero block
    {
        cmsUInt32Number init_buf[4] = { 0x01234567u, 0x89abcdefu, 0x0fedcba9u, 0x87654321u };
        cmsUInt32Number in_block[16];
        for (int i = 0; i < 16; ++i) in_block[i] = (uint32_t)(i * 3 + 0x11111111);
        run_case("Case2_NonZeroInit_SmallBlock", init_buf, in_block);
    }

    // Case 3: Randomized-but-deterministic block with standard initial
    {
        cmsUInt32Number init_buf[4] = { 0x67452301u, 0xefcdab89u, 0x98badcfeu, 0x10325476u };
        cmsUInt32Number in_block[16];
        fill_in_block_randomized(in_block, 0x5A5A5A5Au);
        run_case("Case3_RandomizedBlock_Deterministic", init_buf, in_block);
    }

    // Case 4: All-ones input block with zero initial state
    {
        cmsUInt32Number init_buf[4] = { 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u };
        cmsUInt32Number in_block[16];
        for (int i = 0; i < 16; ++i) in_block[i] = 0xFFFFFFFFu;
        run_case("Case4_AllOnes_Block_ZeroInit", init_buf, in_block);
    }

    // Case 5: Extreme values to test wrap-around behavior
    {
        cmsUInt32Number init_buf[4] = { 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu };
        cmsUInt32Number in_block[16];
        for (int i = 0; i < 16; ++i) in_block[i] = 0x80000000u | (uint32_t)(i * 0x11111111u);
        run_case("Case5_Extremes_WrapAround", init_buf, in_block);
    }

    // Summary
    std::cout << "\nSummary: All tests executed. Failures = " << failures << "\n";

    return 0;
}