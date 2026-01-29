// Unit tests for unzCloseCurrentFile (C function) using a lightweight, non-GTest test harness.
// This test suite is designed to be compiled in a C++11 environment and linked against the
// original unzip.c (and zlib) sources in the project under test.
// It relies on the public API/types provided by unzip.h. The tests cover key branches of the function.

#include <cstring>
#include <string.h>
#include <stdio.h>
#include <crypt.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <unzip.h>
#include <cstdlib>


// Include the unzip library headers to access the real types and constants.
// We assume unzip.h exposes types like unzFile, unz64_s, file_in_zip64_read_info_s and
// constants UNZ_OK, UNZ_PARAMERROR, UNZ_CRCERROR, etc.
extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;

static void expect_eq(int a, int b, const char* desc) {
    if (a != b) {
        std::cerr << "[FAIL] " << desc << " Expected " << b << ", got " << a << "\n";
        ++g_failures;
    }
}

static void expect_true(bool cond, const char* desc) {
    if (!cond) {
        std::cerr << "[FAIL] " << desc << "\n";
        ++g_failures;
    }
}

// Test 1: Passing a NULL file must return UNZ_PARAMERROR
static void test_unzCloseCurrentFile_param_null_file() {
    // Call with NULL pointer
    int res = unzCloseCurrentFile(nullptr);
    expect_eq(res, UNZ_PARAMERROR, "unzCloseCurrentFile(nullptr) should return UNZ_PARAMERROR");
}

// Test 2: File object provided but without an initialized inner read-info should return UNZ_PARAMERROR
static void test_unzCloseCurrentFile_param_null_pfile() {
    unz64_s s;
    // Ensure the inner read info pointer is NULL to simulate an uninitialized state
    s.pfile_in_zip_read = NULL;
    unzFile f = (unzFile)&s;
    int res = unzCloseCurrentFile(f);
    expect_eq(res, UNZ_PARAMERROR, "unzCloseCurrentFile with NULL pfile_in_zip_read should return UNZ_PARAMERROR");
}

// Test 3: CRC mismatch should yield UNZ_CRCERROR when rest_read_uncompressed == 0 and not raw
static void test_unzCloseCurrentFile_crc_mismatch() {
    // Prepare a valid-looking file structure with a CRC mismatch
    unz64_s s;
    file_in_zip64_read_info_s* info = (file_in_zip64_read_info_s*)malloc(sizeof(file_in_zip64_read_info_s));
    if (info == NULL) {
        std::cerr << "[FATAL] malloc failed for test buffer\n";
        g_failures++;
        return;
    }
    std::memset(info, 0, sizeof(*info));

    info->rest_read_uncompressed = 0;   // trigger CRC check
    info->raw = 0;                      // not raw
    info->crc32 = 0xDEADBEEF;           // arbitrary CRC value
    info->crc32_wait = 0x12345678;      // different value to force mismatch
    info->read_buffer = (char*)std::malloc(16);
    info->stream_initialised = 0;       // 0 avoids calling inflateEnd
    s.pfile_in_zip_read = info;

    unzFile f = (unzFile)&s;
    int res = unzCloseCurrentFile(f);

    expect_eq(res, UNZ_CRCERROR, "CRC mismatch should yield UNZ_CRCERROR");
    // After close, the library should NULL the pfile_in_zip_read pointer
    expect_true(s.pfile_in_zip_read == NULL, "s.pfile_in_zip_read should be NULL after close");

    // No need to free info or its buffer; the library is responsible for freeing
}

// Test 4: CRC match should yield UNZ_OK and NULL the pfile_in_zip_read pointer
static void test_unzCloseCurrentFile_crc_match() {
    unz64_s s;
    file_in_zip64_read_info_s* info = (file_in_zip64_read_info_s*)malloc(sizeof(file_in_zip64_read_info_s));
    if (info == NULL) {
        std::cerr << "[FATAL] malloc failed for test buffer\n";
        g_failures++;
        return;
    }
    std::memset(info, 0, sizeof(*info));

    info->rest_read_uncompressed = 0;   // trigger CRC check
    info->raw = 0;                      // not raw
    info->crc32 = 0xABCDEF01;
    info->crc32_wait = 0xABCDEF01;      // CRC matches
    info->read_buffer = (char*)std::malloc(16);
    info->stream_initialised = 0;       // 0 avoids calling inflateEnd
    s.pfile_in_zip_read = info;

    unzFile f = (unzFile)&s;
    int res = unzCloseCurrentFile(f);

    expect_eq(res, UNZ_OK, "CRC match should yield UNZ_OK");
    // Ensure the inner pointer is NULL after closing
    expect_true(s.pfile_in_zip_read == NULL, "s.pfile_in_zip_read should be NULL after close");

    // Library frees info; do not free here
}

// Test 5: CRC should not be checked when rest_read_uncompressed != 0 (branch where CRC is skipped)
static void test_unzCloseCurrentFile_crc_skipped_due_to_rest() {
    unz64_s s;
    file_in_zip64_read_info_s* info = (file_in_zip64_read_info_s*)malloc(sizeof(file_in_zip64_read_info_s));
    if (info == NULL) {
        std::cerr << "[FATAL] malloc failed for test buffer\n";
        g_failures++;
        return;
    }
    std::memset(info, 0, sizeof(*info));

    info->rest_read_uncompressed = 5;   // non-zero -> CRC check skipped
    info->raw = 0;
    info->crc32 = 0xFFFFFFFF;
    info->crc32_wait = 0x0;             // irrelevant
    info->read_buffer = (char*)std::malloc(16);
    info->stream_initialised = 0;
    s.pfile_in_zip_read = info;

    unzFile f = (unzFile)&s;
    int res = unzCloseCurrentFile(f);

    expect_eq(res, UNZ_OK, "CRC check skipped due to rest_read_uncompressed != 0 should yield UNZ_OK");
    expect_true(s.pfile_in_zip_read == NULL, "s.pfile_in_zip_read should be NULL after close");

    // Memory freed by library; some memory (info) is freed by library
}

// Runner
static void run_all_tests() {
    test_unzCloseCurrentFile_param_null_file();
    test_unzCloseCurrentFile_param_null_pfile();
    test_unzCloseCurrentFile_crc_mismatch();
    test_unzCloseCurrentFile_crc_match();
    test_unzCloseCurrentFile_crc_skipped_due_to_rest();
}

int main() {
    run_all_tests();
    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed.\n";
        return 1;
    }
}