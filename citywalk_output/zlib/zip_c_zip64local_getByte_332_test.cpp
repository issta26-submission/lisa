/*
  Unit Test Suite for zip64local_getByte (C function inside zip.c)
  - Approach: include zip.c into this test translation unit with the 'local' macro defined to be empty.
    This makes the static functions (declared as 'local' in the source) non-static and accessible for testing.
  - We provide a minimal fake zlib.h to satisfy types/macros used by zip64local_getByte:
    - zlib_filefunc64_32_def with read and err callbacks
    - ZREAD64 and ZERROR64 macros
    - ZIP_OK, ZIP_ERRNO, ZIP_EOF codes
  - Tests cover:
    1) Successful read (err == 1) path
    2) Read failure with error (ZERROR64 true) path
    3) Read failure without error (EOF) path
  - No Google Test or external test framework; a small in-process test harness prints PASS/FAIL per case.
  - All tests run from main(), no private members accessed directly.
*/

#define local // make all 'local' functions in zip.c non-static for testing

// Minimal fake zlib.h to satisfy the dependencies of zip64local_getByte
// This header provides just enough for the unit tests to exercise the function.
#ifndef ZIP_TEST_FAKE_ZLIB_H
#define ZIP_TEST_FAKE_ZLIB_H

typedef void* voidpf; // mirror of zlib's voidpf

// Basic return codes (mapping close to minizip-style codes)
#define ZIP_OK    0
#define ZIP_ERRNO -1
#define ZIP_EOF   -2

// Forward declaration for the test's callback context
struct ReadCtx;

// 64-bit file function struct used by zip64local_getByte
typedef struct zlib_filefunc64_32_def {
    // Pointer to a read function: int (*read)(voidpf opaque, void* buf, unsigned int size);
    int (*read)(voidpf opaque, void* buf, unsigned int size);

    // Pointer to an error/status-check function: int (*err)(voidpf opaque);
    int (*err)(voidpf opaque);
} zlib_filefunc64_32_def;

// Macros used by zip64local_getByte to perform IO
#define ZREAD64(p, f, buf, len) ((p).read((void*)(f), (buf), (len)))
#define ZERROR64(p, f)    ((p).err((void*)(f)))

#endif // ZIP_TEST_FAKE_ZLIB_H

// Provide the fake header to the test translation unit's include path
// The real zip.c will include "zlib.h" and pull in these definitions.
// We place this after the macro above to ensure the fake header is used.
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <crypt.h>
#include <zip.c>
#include <iostream>
#include <zlib.h>
#include <zip_test_fake_zlib.h>
#include <stdlib.h>
#include <zip.h>


// Since the test harness includes zip.c directly, we need to provide the actual implementation.
// The zip.c is expected to define zip64local_getByte with the signature:
// int zip64local_getByte(const zlib_filefunc64_32_def* pzlib_filefunc_def, voidpf filestream, int* pi);

// Include the actual zip.c source file to pull in the function under test.
// zip.c is expected to be present in the build environment.

// Now implement the test harness

// Forward declare the function under test (C linkage)
extern "C" int zip64local_getByte(const zlib_filefunc64_32_def* pzlib_filefunc_def, voidpf filestream, int* pi);

// Context used by the fake IO callbacks
struct ReadCtx {
    int mode;       // 1 for success (read a byte), 0 otherwise
    unsigned char value; // value to place in buffer on success
    int error;      // 1 means ZERROR64 should indicate an error
};

// Fake read callback: if mode == 1, write value into buffer and return 1; else return 0
int test_read(voidpf opaque, void* buffer, unsigned int size) {
    ReadCtx* ctx = static_cast<ReadCtx*>(opaque);
    (void)size; // size is 1 in our tests, but keep unused warning away
    if (ctx && ctx->mode == 1) {
        if (buffer && size > 0) {
            ((unsigned char*)buffer)[0] = ctx->value;
        }
        return 1;
    }
    return 0;
}

// Fake error callback: return non-zero if error flag is set
int test_err(voidpf opaque) {
    ReadCtx* ctx = static_cast<ReadCtx*>(opaque);
    if (ctx) return ctx->error ? 1 : 0;
    return 0;
}

// Simple test helper
static int failed = 0;
void report_test(const std::string& name, bool passed, const std::string& detail = "") {
    if (passed) {
        std::cout << "[PASS] " << name;
        if (!detail.empty()) std::cout << " - " << detail;
        std::cout << std::endl;
    } else {
        std::cout << "[FAIL] " << name;
        if (!detail.empty()) std::cout << " - " << detail;
        std::cout << std::endl;
        failed++;
    }
}

// Test 1: Successful read path (err == 1)
void test_case_read_success() {
    ReadCtx ctx{1, 'A', 0};
    zlib_filefunc64_32_def fdef;
    fdef.read = test_read;
    fdef.err  = test_err;
    voidpf filestream = &ctx;

    int out = -1;
    int ret = zip64local_getByte(&fdef, filestream, &out);

    bool ok = (ret == ZIP_OK) && (out == static_cast<int>('A'));
    report_test("zip64local_getByte: successful read path", ok,
                ok ? "ret=ZIP_OK, value=65" : "unexpected return/value");
}

// Test 2: Read returns 0 and error indicated (ZERROR64 true) -> ZIP_ERRNO
void test_case_read_error() {
    ReadCtx ctx{0, 0, 1}; // mode 0 means read will fail to read any byte; error flag set
    zlib_filefunc64_32_def fdef;
    fdef.read = test_read;
    fdef.err  = test_err;
    voidpf filestream = &ctx;

    int out = -1;
    int ret = zip64local_getByte(&fdef, filestream, &out);

    bool ok = (ret == ZIP_ERRNO);
    report_test("zip64local_getByte: read fails with error (ZIP_ERRNO)", ok,
                ok ? "ret=ZIP_ERRNO" : "unexpected return");
}

// Test 3: Read returns 0 and error not indicated (EOF)
void test_case_read_eof() {
    ReadCtx ctx{0, 0, 0}; // mode 0; error flag not set
    zlib_filefunc64_32_def fdef;
    fdef.read = test_read;
    fdef.err  = test_err;
    voidpf filestream = &ctx;

    int out = -1;
    int ret = zip64local_getByte(&fdef, filestream, &out);

    bool ok = (ret == ZIP_EOF);
    report_test("zip64local_getByte: EOF path (ZIP_EOF) when no error", ok,
                ok ? "ret=ZIP_EOF" : "unexpected return");
}

int main() {
    // Run all tests
    test_case_read_success();
    test_case_read_error();
    test_case_read_eof();

    if (failed == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << failed << " test(s) FAILED." << std::endl;
        return 1;
    }
}