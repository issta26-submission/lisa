// High-quality C++11 unit tests for the focal function unz64local_CheckCurrentFileCoherencyHeader
// Approach: include unzip.c in the same translation unit after redefining 'local' as empty
// so that the focal function becomes accessible for testing. The tests use a small in-memory
// fake IO to exercise different branches of the function without real filesystem access.

// Note: This test suite is designed to compile with a C++11 compiler and without Google Test.
// It uses a lightweight test harness: prints PASS/FAIL per test and continues execution.

#include <cstring>
#include <cstdio>
#include <string.h>
#include <stdio.h>
#include <crypt.h>
#include <iostream>
#include <unzip.c>
#include <zlib.h>
#include <stdlib.h>
#include <unzip.h>


// Step 1: Make the focal function accessible by turning file-scope 'local' (static) into non-static.
// Assumes unzip.c relies on the macro 'local' for its static qualifiers.
#define local
#undef local

// Lightweight test harness
static int g_total = 0;
static int g_passed = 0;

static void report(const char* test_name, bool passed, const char* detail = nullptr) {
    ++g_total;
    if (passed) {
        ++g_passed;
        std::cout << "[PASS] " << test_name << "\n";
        if (detail) std::cout << "       " << detail << "\n";
    } else {
        std::cout << "[FAIL] " << test_name << "\n";
        if (detail) std::cout << "       " << detail << "\n";
    }
}

// Test helpers and scaffolding for in-memory IO to feed unz64local_CheckCurrentFileCoherencyHeader

// Memory-backed filestream for test scenarios
struct MemoryFile {
    const unsigned char* data;
    size_t size;
    size_t pos;
};

// Test 1: Force ZSEEK64 to fail (seek returns non-zero)
static uLong test1_read64(voidpf opaque, voidpf stream, void* buf, uLong size) {
    // Not used in this path
    return 0;
}
static ZPOS64_T test1_seek64(voidpf opaque, voidpf stream, ZPOS64_T offset, int origin) {
    // Force seek to fail
    (void)opaque; (void)stream; (void)offset; (void)origin;
    return 1; // non-zero -> error path
}
static int test1_close64(voidpf opaque, voidpf stream) {
    (void)opaque; (void)stream;
    return 0;
}

static void test_seek_failurePath() {
    // Prepare a minimal environment; seek will fail immediately
    MemoryFile mem;
    mem.data = nullptr;
    mem.size = 0;
    mem.pos  = 0;

    zlib_filefunc64_32_def filefunc;
    std::memset(&filefunc, 0, sizeof(filefunc));
    filefunc.opaque  = &mem;
    // Assign test-specific IO functions
    filefunc.zread64 = test1_read64; // not used due to early seek error
    filefunc.zseek64 = test1_seek64;
    filefunc.zclose64 = test1_close64;
    // zopen64 may be required by some builds; set to nullptr to avoid accidental calls

    unz64_s s;
    std::memset(&s, 0, sizeof(s));
    s.filestream = (void*)(&mem);
    s.z_filefunc = filefunc;
    s.byte_before_the_zipfile = 0;
    s.cur_file_info_internal.offset_curfile = 0;

    uInt   uiSizeVar = 0;
    ZPOS64_T poffset_local_extrafield = 0;
    uInt   psize_local_extrafield = 0;

    int res = unz64local_CheckCurrentFileCoherencyHeader(&s, &uiSizeVar,
                                        &poffset_local_extrafield,
                                        &psize_local_extrafield);
    report("unz64local_CheckCurrentFileCoherencyHeader - Seek failure path",
           res == UNZ_ERRNO,
           "Expected UNZ_ERRNO due to seek failure.");
}

// Test 2: Magic mismatch triggers UNZ_BADZIPFILE
static uLong test2_read64(voidpf opaque, voidpf stream, void* buf, uLong size) {
    // Read zeros (to keep behavior deterministic); 4-byte magic will be 0
    MemoryFile* m = (MemoryFile*)opaque;
    if (m->pos >= m->size) return 0;
    uLong toCopy = (size < (m->size - m->pos)) ? size : (m->size - m->pos);
    std::memcpy(buf, m->data + m->pos, toCopy);
    m->pos += toCopy;
    return toCopy;
}
static ZPOS64_T test2_seek64(voidpf opaque, voidpf stream, ZPOS64_T offset, int origin) {
    MemoryFile* m = (MemoryFile*)opaque;
    switch (origin) {
        case 0: m->pos = static_cast<size_t>(offset); break; // SEEK_SET
        case 1: m->pos += static_cast<size_t>(offset); break;   // SEEK_CUR
        case 2: m->pos = m->size + static_cast<size_t>(offset); break; // SEEK_END
        default: return (ZPOS64_T) -1;
    }
    if (m->pos > m->size) m->pos = m->size;
    return 0;
}
static int test2_close64(voidpf opaque, voidpf stream) {
    (void)opaque; (void)stream;
    return 0;
}

static void test_magic_mismatchPath() {
    // 4-byte magic: 0 (instead of 0x04034b50)
    const unsigned char data[] = { 0x00, 0x00, 0x00, 0x00 }; // magic will be 0
    MemoryFile mem;
    mem.data = data;
    mem.size = sizeof(data);
    mem.pos  = 0;

    zlib_filefunc64_32_def filefunc;
    std::memset(&filefunc, 0, sizeof(filefunc));
    filefunc.opaque  = &mem;
    filefunc.zread64 = test2_read64;
    filefunc.zseek64 = test2_seek64;
    filefunc.zclose64 = test2_close64;

    unz64_s s;
    std::memset(&s, 0, sizeof(s));
    s.filestream = (void*)(&mem);
    s.z_filefunc = filefunc;
    // Prepare a dummy cur_file_info to avoid uninitialized accesses:
    s.cur_file_info_internal.offset_curfile = 0;
    s.cur_file_info.compression_method = 0; // not used to pass
    s.cur_file_info.crc = 0;
    s.cur_file_info.compressed_size = 0;
    s.cur_file_info.uncompressed_size = 0;
    s.cur_file_info.size_filename = 0;

    uInt   uiSizeVar = 0;
    ZPOS64_T poffset_local_extrafield = 0;
    uInt   psize_local_extrafield = 0;

    int res = unz64local_CheckCurrentFileCoherencyHeader(&s, &uiSizeVar,
                                        &poffset_local_extrafield,
                                        &psize_local_extrafield);
    report("unz64local_CheckCurrentFileCoherencyHeader - Magic mismatch path",
           res == UNZ_BADZIPFILE,
           "Expected UNZ_BADZIPFILE due to bad magic.");
}

// Test 3: Successful path with consistent data; verify size var and extrafield offset calculations
static uLong test3_read64(voidpf opaque, voidpf stream, void* buf, uLong size) {
    MemoryFile* m = (MemoryFile*)opaque;
    if (m->pos >= m->size) return 0;
    uLong toCopy = (size < (m->size - m->pos)) ? size : (m->size - m->pos);
    std::memcpy(buf, m->data + m->pos, toCopy);
    m->pos += toCopy;
    return toCopy;
}
static ZPOS64_T test3_seek64(voidpf opaque, voidpf stream, ZPOS64_T offset, int origin) {
    MemoryFile* m = (MemoryFile*)opaque;
    switch (origin) {
        case 0: m->pos = static_cast<size_t>(offset); break;
        case 1: m->pos += static_cast<size_t>(offset); break;
        case 2: m->pos = m->size + static_cast<size_t>(offset); break;
        default: return (ZPOS64_T) -1;
    }
    if (m->pos > m->size) m->pos = m->size;
    return 0;
}
static int test3_close64(voidpf opaque, voidpf stream) {
    (void)opaque; (void)stream;
    return 0;
}

static void test_successPath() {
    // 30-byte data payload for the successful path
    // Layout (little-endian):
    // magic 0x04034b50, data 0, flags 0, compression_method 0,
    // date/time 0, crc 0x12345678, compressed_size 1000, uncompressed_size 2000,
    // size_filename 4, size_extra_field 6
    const unsigned char data[30] = {
        0x50, 0x4B, 0x03, 0x04, // magic
        0x00, 0x00,             // uData
        0x00, 0x00,             // uFlags
        0x00, 0x00,             // uData (compression_method)
        0x00, 0x00, 0x00, 0x00,   // date/time
        0x78, 0x56, 0x34, 0x12,   // crc 0x12345678
        0xE8, 0x03, 0x00, 0x00,   // compressed_size 1000
        0xD0, 0x07, 0x00, 0x00,   // uncompressed_size 2000
        0x04, 0x00,               // size_filename 4
        0x06, 0x00                // size_extra_field 6
    };

    MemoryFile mem;
    mem.data = data;
    mem.size = sizeof(data);
    mem.pos  = 0;

    zlib_filefunc64_32_def filefunc;
    std::memset(&filefunc, 0, sizeof(filefunc));
    filefunc.opaque  = &mem;
    filefunc.zread64 = test3_read64;
    filefunc.zseek64 = test3_seek64;
    filefunc.zclose64 = test3_close64;

    unz64_s s;
    std::memset(&s, 0, sizeof(s));
    s.filestream = (void*)(&mem);
    s.z_filefunc = filefunc;
    s.byte_before_the_zipfile = 0;
    s.cur_file_info_internal.offset_curfile = 0;

    // Populate current file info with values expected by the path
    s.cur_file_info.compression_method = 0;
    s.cur_file_info.crc = 0x12345678;
    s.cur_file_info.compressed_size = 1000;
    s.cur_file_info.uncompressed_size = 2000;
    s.cur_file_info.size_filename = 4;

    uInt   uiSizeVar = 0;
    ZPOS64_T poffset_local_extrafield = 0;
    uInt   psize_local_extrafield = 0;

    int res = unz64local_CheckCurrentFileCoherencyHeader(&s, &uiSizeVar,
                                        &poffset_local_extrafield,
                                        &psize_local_extrafield);

    bool ok = (res == UNZ_OK) && (uiSizeVar == 10) && (poffset_local_extrafield == (SIZEZIPLOCALHEADER + 4)
                 ) && (psize_local_extrafield == 6);
    // Note: SIZEZIPLOCALHEADER is 0x1e (30) in unzip.c; poffset_local_extrafield should be 34.

    // Compute expected offset in test: 0 + 0x1e + size_filename (4) = 34
    ok = (res == UNZ_OK) && (uiSizeVar == 10) && (poffset_local_extrafield == (size_t)34) && (psize_local_extrafield == 6);

    report("unz64local_CheckCurrentFileCoherencyHeader - Successful path", ok,
           "Expected UNZ_OK with piSizeVar=10, poffset_local_extrafield=34, psize_local_extrafield=6.");
}

// Entry point for tests
int main() {
    test_seek_failurePath();
    test_magic_mismatchPath();
    test_successPath();

    std::cout << "Tests run: " << g_total << ", Passed: " << g_passed << std::endl;
    // Return non-zero if any test failed (optional, but helpful for CI)
    return (g_total == g_passed) ? 0 : 1;
}