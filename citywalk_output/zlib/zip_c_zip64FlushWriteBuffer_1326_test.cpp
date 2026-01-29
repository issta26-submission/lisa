/*
  Lightweight, self-contained unit tests for the focal method:
  zip64FlushWriteBuffer(zip64_internal* zi)

  Approach:
  - Recreate minimal stand-ins for the dependencies used by the focal method.
  - Implement zip64FlushWriteBuffer exactly as in the provided snippet (adapted to this test harness).
  - Provide targeted test cases to cover true/false branches and interactions:
    * Encryption off/on
    * Successful write vs failed write
    * ZIP/BZIP2 specific path
  - Use a small, non-terminating assertion mechanism (EXPECT_*) to maximize coverage without aborting on failures.
  - No GTest; a simple main() drives tests and prints summary.

  Notes:
  - This is a self-contained harness designed to compile under C++11.
  - It purposefully focuses on the logic flow of zip64FlushWriteBuffer and its dependent fields.
  - The test mimics the necessary parts of the real types and constants from the provided code.
  - Prospective users can extend tests by adding more scenarios following the same pattern.
*/

#include <cstdio>
#include <cstring>
#include <string.h>
#include <time.h>
#include <cassert>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <crypt.h>
#include <vector>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <zip.h>


// Domain: lightweight mock of needed types and constants

// Basic type aliases (matching intent from provided code)
typedef unsigned int  uInt;
typedef unsigned long uLong;
typedef unsigned long long ZPOS64_T;
typedef void* voidpf;
typedef unsigned char Byte;

// Macros used by the focal method
#define ZIP_OK 0
#define ZIP_ERRNO (-1)
#define Z_BUFSIZE (64*1024)
#define HAVE_BZIP2  // Enable the bzip2 related code path in the test
#define Z_BZIP2ED 2 // Arbitrary ID for the BZIP2 method (used in test)

// Forward declaration of zencode to satisfy compile-time reference from focal method
extern "C" unsigned char zencode(unsigned long keys[3], const unsigned long* pcrc_32_tab, unsigned char c, int t);

// Very small, test-specific stub for ZWRITE64 (function-like call in the focal method)
static unsigned int g_write_calls = 0;
static unsigned int g_next_write_should_fail = 0;
static unsigned int ZWRITE64(const void* pzlib_filefunc_def, void* filestream, const void* buf, ZPOS64_T len) {
    (void)pzlib_filefunc_def; // unused in test stub
    (void)filestream;          // unused in test stub
    (void)buf;                 // we don't mutate the input buffer in tests
    if (g_next_write_should_fail) {
        g_next_write_should_fail = 0;
        // Simulate partial write: return less than requested
        if (len == 0) return 0;
        return (unsigned int)(len - 1);
    }
    ++g_write_calls;
    // Simulate successful write of all requested bytes
    return (unsigned int)len;
}

// Stub for zlib_filefunc64_32_def (not used in tests, only to satisfy structure references)
struct zlib_filefunc64_32_def { };

// Minimal z_stream used by the test
struct z_stream {
    uLong total_in;
};

// Optional bz_stream for HAVE_BZIP2 path
struct bz_stream {
    uLong total_in_lo32;
    uLong total_in_hi32;
};

// The per-file information structure (subset needed by the focal method)
struct curfile64_info {
    z_stream stream;
#ifdef HAVE_BZIP2
    bz_stream bstream;
#endif
    int  stream_initialised;
    uInt pos_in_buffered_data;
    ZPOS64_T pos_local_header;
    char* central_header;
    uLong size_centralExtra;
    uLong size_centralheader;
    uLong size_centralExtraFree;
    uLong flag;
    int  method;
    int  raw;
    Byte buffered_data[Z_BUFSIZE];
    uLong dosDate;
    uLong crc32;
    int  encrypt;
    int  zip64;
    ZPOS64_T pos_zip64extrainfo;
    ZPOS64_T totalCompressedData;
    ZPOS64_T totalUncompressedData;
#ifndef NOCRYPT
    unsigned long keys[3];
    const unsigned long* pcrc_32_tab;
    unsigned crypt_header_size;
#endif
};

// Main internal ZIP structure (subset)
struct zip64_internal {
    zlib_filefunc64_32_def z_filefunc;
    voidpf filestream;
    struct {
        // central_dir not used in tests
        // minimal placeholder
    } central_dir;
    int  in_opened_file_inzip;
    curfile64_info ci;
    ZPOS64_T begin_pos;
    ZPOS64_T add_position_when_writing_offset;
    ZPOS64_T number_entry;
};

// Provide zencode so that the encryption branch can be exercised in tests
static std::vector<unsigned char> g_zencode_inputs;
static std::vector<unsigned char> g_zencode_outputs;
unsigned char zencode(unsigned long keys[3], const unsigned long* pcrc_32_tab, unsigned char c, int t) {
    (void)keys; (void)pcrc_32_tab; (void)t;
    // Simple deterministic transformation to verify encryption branch
    g_zencode_inputs.push_back(static_cast<unsigned char>(c));
    unsigned char out = static_cast<unsigned char>(c ^ 0x01);
    g_zencode_outputs.push_back(out);
    return out;
}

// The focal function under test (copied/adapted for the harness)
int zip64FlushWriteBuffer(zip64_internal* zi) {
    int err=ZIP_OK;
    if (zi->ci.encrypt != 0)
    {
#ifndef NOCRYPT
        uInt i;
        int t;
        for (i=0;i<zi->ci.pos_in_buffered_data;i++)
            zi->ci.buffered_data[i] = zencode(zi->ci.keys, zi->ci.pcrc_32_tab, zi->ci.buffered_data[i],t);
#endif
    }
    if (ZWRITE64(zi->z_filefunc,zi->filestream,zi->ci.buffered_data,zi->ci.pos_in_buffered_data) != zi->ci.pos_in_buffered_data)
      err = ZIP_ERRNO;
    zi->ci.totalCompressedData += zi->ci.pos_in_buffered_data;
#ifdef HAVE_BZIP2
    if(zi->ci.method == Z_BZIP2ED)
    {
      zi->ci.totalUncompressedData += zi->ci.bstream.total_in_lo32;
      zi->ci.bstream.total_in_lo32 = 0;
      zi->ci.bstream.total_in_hi32 = 0;
    }
    else
#endif
    {
      zi->ci.totalUncompressedData += zi->ci.stream.total_in;
      zi->ci.stream.total_in = 0;
    }

    zi->ci.pos_in_buffered_data = 0;
    return err;
}

// Simple non-terminating test assertion helper
static int g_fail_count = 0;
#define EXPECT_EQ(a,b,desc) do { \
    if ((a) != (b)) { \
        std::fprintf(stderr, "EXPECT_EQ FAILED: %s | %s:%d -> %llu != %llu\n", (desc), __FILE__, __LINE__, (unsigned long long)(a), (unsigned long long)(b)); \
        ++g_fail_count; \
    } \
} while(0)

#define EXPECT_TRUE(cond,desc) do { \
    if (!(cond)) { std::fprintf(stderr, "EXPECT_TRUE FAILED: %s | %s:%d\n", (desc), __FILE__, __LINE__); ++g_fail_count; } \
} while(0)

// Test helpers to initialize structures
static void init_basic_zip64_internal(zip64_internal& zi) {
    std::memset(&zi, 0, sizeof(zi));
    zi.filestream = nullptr;
    zi.z_filefunc = zlib_filefunc64_32_def();
    zi.ci.stream.total_in = 0;
    zi.ci.pos_in_buffered_data = 0;
    zi.ci.encrypt = 0;
    zi.ci.method = 0;
    zi.ci.totalCompressedData = 0;
    zi.ci.totalUncompressedData = 0;
    std::memset(zi.ci.buffered_data, 0, sizeof(zi.ci.buffered_data));
#ifndef NOCRYPT
    zi.ci.keys[0] = zi.ci.keys[1] = zi.ci.keys[2] = 0;
    zi.ci.pcrc_32_tab = nullptr;
#endif
}

// Test 1: No encryption, successful write, normal (non-BZIP2) path
static void test_zip64FlushWriteBuffer_no_encrypt_success() {
    zip64_internal zi;
    init_basic_zip64_internal(zi);

    // Setup: no encryption; some buffered data
    zi.ci.pos_in_buffered_data = 4;
    unsigned char input_buf[4] = {1,2,3,4};
    std::memcpy(zi.ci.buffered_data, input_buf, 4);
    zi.ci.stream.total_in = 7;
    zi.ci.totalCompressedData = 0;
    zi.ci.totalUncompressedData = 10; // starting value to verify accumulation

    // Filestream stub
    zi.filestream = (void*)0x1;

    int err = zip64FlushWriteBuffer(&zi);

    EXPECT_EQ(err, ZIP_OK, "no_encrypt_success: err should be ZIP_OK");
    EXPECT_EQ(zi.ci.totalCompressedData, 4ULL, "no_encrypt_success: totalCompressedData after write");
    EXPECT_EQ(zi.ci.totalUncompressedData, 17ULL, "no_encrypt_success: totalUncompressedData after write");
    EXPECT_EQ(zi.ci.pos_in_buffered_data, 0U, "no_encrypt_success: pos_in_buffered_data reset to 0");
    // Also ensure data was written (write_calls incremented)
    EXPECT_TRUE(g_write_calls == 1, "no_encrypt_success: ZWRITE64 should be called exactly once");
}

// Test 2: Encryption enabled, per-byte encoding occurs
static void test_zip64FlushWriteBuffer_encrypt_per_byte() {
    zip64_internal zi;
    init_basic_zip64_internal(zi);

    // Enable encryption
    zi.ci.encrypt = 1;
    zi.ci.pos_in_buffered_data = 3;
    zi.ci.stream.total_in = 8;
    zi.ci.totalCompressedData = 0;
    zi.ci.totalUncompressedData = 0;

    // Data to encode: 10, 11, 12
    zi.ci.buffered_data[0] = 10;
    zi.ci.buffered_data[1] = 11;
    zi.ci.buffered_data[2] = 12;

    // Filestream stub
    zi.filestream = (void*)0x2;

    int err = zip64FlushWriteBuffer(&zi);

    // Encryption should have altered the data: 10^1=11, 11^1=10, 12^1=13
    unsigned char expected_after_encode[3] = {11, 10, 13};
    bool matched = std::memcmp(zi.ci.buffered_data, expected_after_encode, 3) == 0;
    EXPECT_TRUE(matched, "encrypt_per_byte: buffered_data after zencode transformation");

    // Write should succeed
    EXPECT_EQ(err, ZIP_OK, "encrypt_per_byte: err should be ZIP_OK");
    EXPECT_EQ(zi.ci.totalCompressedData, 3ULL, "encrypt_per_byte: totalCompressedData after write");
    EXPECT_EQ(zi.ci.totalUncompressedData, 8ULL, "encrypt_per_byte: totalUncompressedData after write");
    EXPECT_EQ(zi.ci.pos_in_buffered_data, 0U, "encrypt_per_byte: pos_in_buffered_data reset to 0");
}

// Test 3: Write failure handling (len mismatch)
static void test_zip64FlushWriteBuffer_write_failure() {
    zip64_internal zi;
    init_basic_zip64_internal(zi);

    // No encryption for simplicity
    zi.ci.encrypt = 0;
    zi.ci.pos_in_buffered_data = 2;
    zi.ci.buffered_data[0] = 0xAA;
    zi.ci.buffered_data[1] = 0xBB;
    zi.ci.stream.total_in = 5;
    zi.ci.totalCompressedData = 0;
    zi.ci.totalUncompressedData = 0;

    // Trigger a partial write on next call
    g_next_write_should_fail = 1;

    zi.filestream = (void*)0x3;

    int err = zip64FlushWriteBuffer(&zi);

    // Since write failed (partial), err should be ZIP_ERRNO
    EXPECT_EQ(err, ZIP_ERRNO, "write_failure: err should be ZIP_ERRNO");
    // Data accounting should still update
    EXPECT_EQ(zi.ci.totalCompressedData, 2ULL, "write_failure: totalCompressedData updated even on failure");
    EXPECT_EQ(zi.ci.totalUncompressedData, 5ULL, "write_failure: totalUncompressedData updated even on failure");
    EXPECT_EQ(zi.ci.pos_in_buffered_data, 0U, "write_failure: pos_in_buffered_data reset to 0");
}

// Test 4: BZIP2 path accumulation
static void test_zip64FlushWriteBuffer_bzip2_path() {
    zip64_internal zi;
    init_basic_zip64_internal(zi);
    // Enable BZIP2 path
    zi.ci.encrypt = 0;
    zi.ci.method = Z_BZIP2ED;
    zi.ci.pos_in_buffered_data = 1;
    zi.ci.buffered_data[0] = 0x77;
    zi.ci.stream.total_in = 123;
    zi.ci.totalCompressedData = 0;
    zi.ci.totalUncompressedData = 0;
    // Simulate bstream totals
#ifdef HAVE_BZIP2
    zi.ci.bstream.total_in_lo32 = 42;
    zi.ci.bstream.total_in_hi32 = 0;
#endif
    zi.filestream = (void*)0x4;

    // Run
    int err = zip64FlushWriteBuffer(&zi);

    // Write should succeed
    EXPECT_EQ(err, ZIP_OK, "bzip2_path: err should be ZIP_OK");
    // totalCompressedData updated by 1
    EXPECT_EQ(zi.ci.totalCompressedData, 1ULL, "bzip2_path: totalCompressedData after write");
    // totalUncompressedData updated from bstream (lo32)
#ifdef HAVE_BZIP2
    EXPECT_EQ(zi.ci.totalUncompressedData, 42ULL, "bzip2_path: totalUncompressedData should include bstream.total_in_lo32");
    EXPECT_EQ(zi.ci.bstream.total_in_lo32, 0ULL, "bzip2_path: bstream.total_in_lo32 reset to 0");
    EXPECT_EQ(zi.ci.bstream.total_in_hi32, 0ULL, "bzip2_path: bstream.total_in_hi32 reset to 0");
#endif
    EXPECT_EQ(zi.ci.pos_in_buffered_data, 0U, "bzip2_path: pos_in_buffered_data reset to 0");
}

// Test 5: Non-BZIP2 path fallback when method != Z_BZIP2ED
static void test_zip64FlushWriteBuffer_non_bzip2_path() {
    zip64_internal zi;
    init_basic_zip64_internal(zi);
    // Method not BZIP2ED
    zi.ci.encrypt = 0;
    zi.ci.method = 0;
    zi.ci.pos_in_buffered_data = 2;
    zi.ci.buffered_data[0] = 0x11;
    zi.ci.buffered_data[1] = 0x22;
    zi.ci.stream.total_in = 13;
    zi.ci.totalCompressedData = 0;
    zi.ci.totalUncompressedData = 0;

    zi.filestream = (void*)0x5;

    int err = zip64FlushWriteBuffer(&zi);

    // Write should succeed
    EXPECT_EQ(err, ZIP_OK, "non_bzip2_path: err should be ZIP_OK");
    EXPECT_EQ(zi.ci.totalCompressedData, 2ULL, "non_bzip2_path: totalCompressedData after write");
    EXPECT_EQ(zi.ci.totalUncompressedData, 13ULL, "non_bzip2_path: totalUncompressedData updated from stream.total_in");
    EXPECT_EQ(zi.ci.pos_in_buffered_data, 0U, "non_bzip2_path: pos_in_buffered_data reset to 0");
}

// Driver
int main() {
    // Initialize a fresh printf-based summary
    g_fail_count = 0;

    test_zip64FlushWriteBuffer_no_encrypt_success();
    test_zip64FlushWriteBuffer_encrypt_per_byte();
    test_zip64FlushWriteBuffer_write_failure();
    test_zip64FlushWriteBuffer_bzip2_path();
    test_zip64FlushWriteBuffer_non_bzip2_path();

    // Summary
    if (g_fail_count == 0) {
        std::cout << "All tests PASSED.\n";
    } else {
        std::cout << "Tests finished with " << g_fail_count << " failure(s).\n";
    }

    return g_fail_count != 0;
}

/*
  Explanatory notes for tests:
  - test_zip64FlushWriteBuffer_no_encrypt_success:
    Verifies normal path: no encryption, successful write, and subsequent accounting of totals.
  - test_zip64FlushWriteBuffer_encrypt_per_byte:
    Forces encryption path and validates per-byte encoding using the dummy zencode transformation.
  - test_zip64FlushWriteBuffer_write_failure:
    Simulates a partial write; ensures ZIP_ERRNO is returned and internal counters still update accordingly.
  - test_zip64FlushWriteBuffer_bzip2_path:
    Exercises the ZIP64/BZIP2-specific branch, ensuring totalUncompressedData comes from bstream.total_in_lo32 and buffers reset.
  - test_zip64FlushWriteBuffer_non_bzip2_path:
    Ensures non-BZIP2 path uses stream.total_in as the source for uncompressed data and respects resetting of state.

  This harness is designed to be self-contained and compilable under C++11 without external testing frameworks.
*/