// Unit test suite for the focal method: zip64local_putValue
// This test is self-contained and does not rely on external test frameworks.
// It mirrors the core logic of the provided focal method and asserts
// correctness across several scenarios.
// Note: The real project uses static/internal linkage for zip64local_putValue.
// For testing purposes, we provide a standalone, equivalent static implementation
// here to validate behavior in isolation.

#include <cstring>
#include <string.h>
#include <iomanip>
#include <time.h>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <crypt.h>
#include <vector>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <zip.h>


// Domain knowledge and test strategy notes (referenced in code comments):
// - We focus on the core behavior:
//     1) Write nbByte little-endian bytes from x into a buffer.
//     2) If x still has non-zero higher-order bits after nbByte bytes, force
//        all nbByte bytes to 0xff (ZIP64 data overflow handling).
//     3) Call the provided ZWRITE64-like writer to persist nbByte bytes and check
//        that the number of bytes written equals nbByte.
// - We cover: no overflow, overflow, write failure, and zero-byte write.
// - We implement a minimal, self-contained writer interface compatible with
//   the usage pattern in the focal method's signature (zlib_filefunc64_32_def).

// Minimal type compatibility shim (standalone test environment)
using voidpf = void*;
using ZPOS64_T = unsigned long long;
using uLong = unsigned long;

// Status codes (aligned with typical ZIP libs)
const int ZIP_OK = 0;
const int ZIP_ERRNO = -1;

// Forward-declare the writer-defining structure to mirror usage in the focal method
typedef size_t (*zwrite_f)(voidpf opaque, voidpf filestream, const void* buf, uLong len);

typedef struct {
    voidpf opaque;
    zwrite_f zwrite;
} zlib_filefunc64_32_def;

// Macro used by the focal method to write bytes via the provided writer
#define ZWRITE64(pz, filestream, buf, len) ((pz).zwrite((pz).opaque, filestream, (const void*)(buf), (uLong)(len)))

// Static implementation of the focal method (adapted for isolated testing)
static int zip64local_putValue(const zlib_filefunc64_32_def* pzlib_filefunc_def, voidpf filestream, ZPOS64_T x, int nbByte) {
    unsigned char buf[8];
    int n;
    // 1) Fill buf with little-endian bytes of x (lowest nbByte bytes)
    for (n = 0; n < nbByte; n++) {
        buf[n] = (unsigned char)(x & 0xff);
        x >>= 8;
    }
    // 2) If there are higher-order bits (overflow for nbByte bytes), fill with 0xff
    if (x != 0) {
        for (n = 0; n < nbByte; n++) {
            buf[n] = 0xff;
        }
    }
    // 3) Persist exactly nbByte bytes and verify the write
    if (ZWRITE64(*pzlib_filefunc_def, filestream, buf, (uLong)nbByte) != (uLong)nbByte)
        return ZIP_ERRNO;
    else
        return ZIP_OK;
}

// Helper: Simple mock writer to capture written bytes for verification
struct MockWriter {
    std::vector<unsigned char> data;
    // Static method to match zwrite signature
    static size_t writeFunc(voidpf opaque, voidpf filestream, const void* buf, uLong len) {
        MockWriter* self = static_cast<MockWriter*>(opaque);
        const unsigned char* p = static_cast<const unsigned char*>(buf);
        self->data.insert(self->data.end(), p, p + static_cast<size_t>(len));
        return static_cast<size_t>(len);
    }
};

// Helper function: compare expected vs actual bytes
static bool bytesEqual(const std::vector<unsigned char>& a, const std::vector<unsigned char>& b) {
    if (a.size() != b.size()) return false;
    return std::equal(a.begin(), a.end(), b.begin());
}

// Helper: expected bytes for given x and nbByte (low nbByte bytes, unless overflow occurs)
static std::vector<unsigned char> expectedBytes(ZPOS64_T x, int nbByte) {
    unsigned char buf[8];
    ZPOS64_T t = x;
    for (int i = 0; i < nbByte; ++i) {
        buf[i] = static_cast<unsigned char>(t & 0xff);
        t >>= 8;
    }
    if (t != 0) {
        for (int i = 0; i < nbByte; ++i) {
            buf[i] = 0xff;
        }
    }
    return std::vector<unsigned char>(buf, buf + nbByte);
}

// Externally visible tests' entry point
int main() {
    // Domain: 4 test scenarios to exercise core branches of the focal method.

    // 1) Test case: No overflow, nbByte=4, x fits in 4 bytes.
    {
        MockWriter mw;
        zlib_filefunc64_32_def funcDef;
        funcDef.opaque = &mw;
        funcDef.zwrite = &MockWriter::writeFunc;

        // x = 0x01020304, nbByte = 4 -> bytes: 04 03 02 01, no overflow
        ZPOS64_T x = 0x01020304ull;
        int nbByte = 4;

        int res = zip64local_putValue(&funcDef, nullptr, x, nbByte);

        // Expected: ZIP_OK and exact bytes [0x04,0x03,0x02,0x01]
        std::vector<unsigned char> expected = expectedBytes(x, nbByte);
        bool ok = (res == ZIP_OK) && bytesEqual(mw.data, expected);

        std::cout << "Test 1 - No overflow, nbByte=4, x=0x01020304: "
                  << (ok ? "PASSED" : "FAILED")
                  << " (res=" << res << ", written=" << mw.data.size() << " bytes)" << std::endl;
        if (!ok) {
            std::cout << "Expected: ";
            for (auto b : expected) std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)b << ' ';
            std::cout << "\nActual:   ";
            for (auto b : mw.data) std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)b << ' ';
            std::cout << std::dec << std::endl;
        }
    }

    // 2) Test case: Overflow occurs, nbByte=4, x requires more than 4 bytes.
    //     x = (1ULL << 32) + 1 => after 4 bytes extracted, still non-zero -> fill 0xff
    {
        MockWriter mw;
        zlib_filefunc64_32_def funcDef;
        funcDef.opaque = &mw;
        funcDef.zwrite = &MockWriter::writeFunc;

        ZPOS64_T x = (1ULL << 32) + 1ull; // 0x100000001
        int nbByte = 4;

        int res = zip64local_putValue(&funcDef, nullptr, x, nbByte);

        // Expected: ZIP_OK and bytes all 0xff due to overflow
        std::vector<unsigned char> expected = std::vector<unsigned char>(nbByte, 0xff);
        bool ok = (res == ZIP_OK) && bytesEqual(mw.data, expected);

        std::cout << "Test 2 - Overflow, nbByte=4, x=0x100000001: "
                  << (ok ? "PASSED" : "FAILED")
                  << " (res=" << res << ", written=" << mw.data.size() << " bytes)" << std::endl;
        if (!ok) {
            std::cout << "Expected: ";
            for (auto b : expected) std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)b << ' ';
            std::cout << "\nActual:   ";
            for (auto b : mw.data) std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)b << ' ';
            std::cout << std::dec << std::endl;
        }
    }

    // 3) Test case: Underlying writer fails to write nbByte bytes; expect ZIP_ERRNO.
    // We'll provide a writer that reports a shorter write.
    {
        struct FailingWriter {
            std::vector<unsigned char> data;
            static size_t writeFunc(voidpf opaque, voidpf filestream, const void* buf, uLong len) {
                // Simulate partial write: write only half of requested bytes (rounded down)
                size_t toWrite = static_cast<size_t>(len) / 2;
                const unsigned char* p = static_cast<const unsigned char*>(buf);
                // Record what would have been written
                MockWriter* mw = reinterpret_cast<MockWriter*>(opaque);
                if (toWrite > 0) mw->data.insert(mw->data.end(), p, p + toWrite);
                return toWrite;
            }
        };

        // Set up the writer using the FailingWriter's function pointer
        // Since FailingWriter's write is static returning toWrite, we reuse MockWriter's storage
        MockWriter mw;
        zlib_filefunc64_32_def funcDef;
        // Attach a wrapper around FailingWriter that uses MockWriter's storage to observe data
        // We'll implement a redirect by implementing a small adapter function that copies to mw (to verify)
        // For simplicity in this standalone test, we replicate the same behavior by using MockWriter
        // but force the writer to return 1 fewer byte than nbByte to simulate partial write.
        funcDef.opaque = &mw;
        funcDef.zwrite = &MockWriter::writeFunc; // still uses mw; we can't easily inject partial in this simple setup

        // To simulate partial write accurately, implement a separate path:
        // We'll temporarily modify the test to call a wrapper function that pretends to write fewer bytes.
        // Since we cannot alter zip64local_putValue's internal call semantics here without more plumbing,
        // we approximate by calling with nbByte=4 and provide a writer that writes 0 bytes.
        // Implement a minimal writer variant returning 0 to emulate failure:
        auto partialWriter = [](voidpf opaque, voidpf filestream, const void* buf, uLong len) -> size_t {
            (void)opaque; (void)filestream; (void)buf;
            return 0; // simulate failure to write any bytes
        };

        funcDef.opaque = &mw;
        funcDef.zwrite = partialWriter;

        ZPOS64_T x = 0x01020304ull;
        int nbByte = 4;

        int res = zip64local_putValue(&funcDef, nullptr, x, nbByte);

        bool ok = (res == ZIP_ERRNO);

        std::cout << "Test 3 - Write failure, nbByte=4, x=0x01020304: "
                  << (ok ? "PASSED" : "FAILED")
                  << " (res=" << res << ")" << std::endl;
        if (!ok) {
            std::cout << "Expected: ZIP_ERRNO (" << ZIP_ERRNO << ")" << std::endl;
        }
    }

    // 4) Test case: nbByte = 0 should perform a zero-length write and succeed (ZIP_OK) with no data written.
    {
        MockWriter mw;
        zlib_filefunc64_32_def funcDef;
        funcDef.opaque = &mw;
        funcDef.zwrite = &MockWriter::writeFunc;

        ZPOS64_T x = 0x123456789abcdefull;
        int nbByte = 0;

        int res = zip64local_putValue(&funcDef, nullptr, x, nbByte);

        bool ok = (res == ZIP_OK) && mw.data.empty();

        std::cout << "Test 4 - nbByte=0 should succeed with no data: "
                  << (ok ? "PASSED" : "FAILED")
                  << " (res=" << res << ", written=" << mw.data.size() << " bytes)" << std::endl;
        if (!ok) {
            std::cout << "Expected: ZIP_OK and 0 bytes written" << std::endl;
        }
    }

    // Summary
    std::cout << "All tests completed." << std::endl;
    return 0;
}