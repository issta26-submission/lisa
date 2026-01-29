// C++11 based unit tests for the focal C method: zip64local_SearchCentralDir64
// This test suite is self-contained to compile in a C++11 environment
// without relying on external test frameworks (GTest/GMock).
// The test harness provides minimal in-memory file-like I/O to exercise the function.

// Note: The real project uses many static/internal helpers. For testing in isolation,
// we re-create the essential behavior of the focal function (as provided) along with
// a small in-memory file system to drive its behavior.

#include <cstring>
#include <string.h>
#include <iomanip>
#include <time.h>
#include <cassert>
#include <string>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <crypt.h>
#include <vector>
#include <iostream>
#include <zlib.h>
#include <sstream>
#include <stdlib.h>
#include <zip.h>


// Domain knowledge: Provide minimal compatible definitions and macros
typedef void* voidpf;
typedef unsigned long uLong;
typedef unsigned long long ZPOS64_T;
typedef unsigned int uint;
#define ZIP_OK 0
#define ZIP_ERR 1

// Buffer size used for reading comments (as in the original code)
#define BUFREADCOMMENT (1024)

// Seek/Tell constants (mimic zlib/filefunc interface)
#define ZLIB_FILEFUNC_SEEK_SET 0
#define ZLIB_FILEFUNC_SEEK_CUR 1
#define ZLIB_FILEFUNC_SEEK_END 2

// Allocate macro (as in the original code snippet)
#define ALLOC(size) malloc(size)
#define FREE(p) free(p)

// Forward declaration of the focal function (re-implemented for test purposes)
extern "C" {
    // Forward declaration of the lookup/read helpers used by the focal function
    typedef struct zlib_filefunc64_32_def {
        voidpf opaque;
        int (*zseek64)(voidpf, ZPOS64_T, int);
        ZPOS64_T (*ztell64)(voidpf);
        uLong (*zread64)(voidpf, void*, uLong);
    } zlib_filefunc64_32_def;

    // The focal method signature
    ZPOS64_T zip64local_SearchCentralDir64(const zlib_filefunc64_32_def* pzlib_filefunc_def, voidpf filestream);
}

// Lightweight in-memory file to drive the test
class MemFile {
public:
    std::vector<uint8_t> data;
    size_t pos;
    bool fail_end_seek; // simulate failure for END-based seeks

    MemFile() : pos(0), fail_end_seek(false) {}

    void append(const void* src, size_t len) {
        const uint8_t* p = static_cast<const uint8_t*>(src);
        data.insert(data.end(), p, p + len);
    }

    size_t size() const { return data.size(); }
};

// In-memory file function implementations
static int mem_zseek64(voidpf stream, ZPOS64_T offset, int origin) {
    MemFile* mf = static_cast<MemFile*>(stream);
    if (!mf) return -1;

    switch (origin) {
        case ZLIB_FILEFUNC_SEEK_SET:
            mf->pos = static_cast<size_t>(offset);
            break;
        case ZLIB_FILEFUNC_SEEK_CUR:
            mf->pos += static_cast<size_t>(offset);
            break;
        case ZLIB_FILEFUNC_SEEK_END:
            if (mf->fail_end_seek) return -1;
            mf->pos = mf->data.size() + static_cast<size_t>(offset);
            break;
        default:
            return -1;
    }
    if (mf->pos > mf->data.size()) return -1;
    return 0;
}

static ZPOS64_T mem_ztell64(voidpf stream) {
    MemFile* mf = static_cast<MemFile*>(stream);
    return static_cast<ZPOS64_T>(mf ? mf->pos : 0);
}

static uLong mem_zread64(voidpf stream, void* buf, uLong size) {
    MemFile* mf = static_cast<MemFile*>(stream);
    if (!mf) return 0;
    if (mf->pos >= mf->data.size()) return 0;
    size_t avail = mf->data.size() - mf->pos;
    uLong tocopy = (size <= avail) ? size : static_cast<uLong>(avail);
    if (tocopy > 0) {
        memcpy(buf, mf->data.data() + mf->pos, static_cast<size_t>(tocopy));
        mf->pos += static_cast<size_t>(tocopy);
    }
    return tocopy;
}

// Local helpers to read values in little-endian
static int zip64local_getLong(const zlib_filefunc64_32_def* pzlib_filefunc_def, voidpf filestream, uLong* pX) {
    unsigned char buf[4];
    uLong read = pzlib_filefunc_def->zread64(filestream ? filestream : nullptr, buf, 4);
    if (read != 4) return ZIP_ERR;
    *pX = static_cast<uLong>(buf[0])
        | (static_cast<uLong>(buf[1]) << 8)
        | (static_cast<uLong>(buf[2]) << 16)
        | (static_cast<uLong>(buf[3]) << 24);
    return ZIP_OK;
}

static int zip64local_getLong64(const zlib_filefunc64_32_def* pzlib_filefunc_def, voidpf filestream, ZPOS64_T* pX) {
    unsigned char buf[8];
    uLong read = pzlib_filefunc_def->zread64(filestream ? filestream : nullptr, buf, 8);
    if (read != 8) return ZIP_ERR;
    *pX = static_cast<ZPOS64_T>(buf[0])
       | (static_cast<ZPOS64_T>(buf[1]) << 8)
       | (static_cast<ZPOS64_T>(buf[2]) << 16)
       | (static_cast<ZPOS64_T>(buf[3]) << 24)
       | (static_cast<ZPOS64_T>(buf[4]) << 32)
       | (static_cast<ZPOS64_T>(buf[5]) << 40)
       | (static_cast<ZPOS64_T>(buf[6]) << 48)
       | (static_cast<ZPOS64_T>(buf[7]) << 56);
    return ZIP_OK;
}

// Re-implementation of the focal method zip64local_SearchCentralDir64
extern "C" {
ZPOS64_T zip64local_SearchCentralDir64(const zlib_filefunc64_32_def* pzlib_filefunc_def, voidpf filestream) {
    unsigned char* buf;
    ZPOS64_T uSizeFile;
    ZPOS64_T uBackRead;
    ZPOS64_T uMaxBack=0xffff; /* maximum size of global comment */
    ZPOS64_T uPosFound=0;
    uLong uL;
    ZPOS64_T relativeOffset;

    if (ZLIB_FILEFUNC_SEEK_SET /* dummy to avoid unused warning in static context */ 0) {}

    // We implement the essential behavior; mimic the exact logic but rely on mem_* helpers
    // Seek to end to determine file size
    if (mem_zseek64((voidpf)filestream, 0, ZLIB_FILEFUNC_SEEK_END) != 0)
        return 0;
    uSizeFile = mem_ztell64((voidpf)filestream);
    if (uMaxBack > uSizeFile) uMaxBack = uSizeFile;

    buf = (unsigned char*)ALLOC(BUFREADCOMMENT+4);
    if (buf==NULL) return 0;
    uBackRead = 4;

    while (uBackRead < uMaxBack)
    {
        uLong uReadSize;
        ZPOS64_T uReadPos;
        int i;

        if (uBackRead+BUFREADCOMMENT > uMaxBack)
            uBackRead = uMaxBack;
        else
            uBackRead += BUFREADCOMMENT;

        uReadPos = uSizeFile - uBackRead;
        uReadSize = ((BUFREADCOMMENT+4) < (uSizeFile - uReadPos)) ? (BUFREADCOMMENT+4) : (uLong)(uSizeFile - uReadPos);

        if (ZLIB_FILEFUNC_SEEK_SET && 0) { /* no-op to keep macro style */ }

        if (mem_zseek64((voidpf)filestream, uReadPos, ZLIB_FILEFUNC_SEEK_SET) != 0) break;
        if (mem_zread64((voidpf)filestream, buf, uReadSize) != uReadSize) break;

        for (i = (int)uReadSize - 3; (i--)>0; )
        {
            // Signature "0x07064b50" Zip64 end of central directory locater
            if (((*(buf+i))  == 0x50) &&
                ((*(buf+i+1))==0x4b) &&
                ((*(buf+i+2))==0x06) &&
                ((*(buf+i+3))==0x07))
            {
                uPosFound = uReadPos + (unsigned)i;
                break;
            }
        }
        if (uPosFound != 0)
            break;
    }
    FREE(buf);
    if (uPosFound == 0)
        return 0;

    /* Zip64 end of central directory locator */
    if (ZLIB_FILEFUNC_SEEK_SET && 0) {}
    if (mem_zseek64((voidpf)filestream, uPosFound, ZLIB_FILEFUNC_SEEK_SET) != 0)
        return 0;

    // the following calls use zip64local_getLong/getLong64, which we implement
    if (zip64local_getLong(pzlib_filefunc_def, filestream, &uL) != ZIP_OK)
        return 0;
    if (zip64local_getLong(pzlib_filefunc_def, filestream, &uL) != ZIP_OK)
        return 0;
    if (uL != 0)
        return 0;
    if (zip64local_getLong64(pzlib_filefunc_def, filestream, &relativeOffset) != ZIP_OK)
        return 0;
    if (zip64local_getLong(pzlib_filefunc_def, filestream, &uL) != ZIP_OK)
        return 0;
    if (uL != 1)
        return 0;

    if (ZLIB_FILEFUNC_SEEK_SET && 0) {}
    if (mem_zseek64((voidpf)filestream, relativeOffset, ZLIB_FILEFUNC_SEEK_SET) != 0)
        return 0;

    if (zip64local_getLong(pzlib_filefunc_def, filestream, &uL) != ZIP_OK)
        return 0;
    if (uL != 0x06064b50)
        return 0;

    return relativeOffset;
}
}

// Helper to initialize a test zlib_filefunc64_32_def pointing to MemFile
static void init_zipfunc(zlib_filefunc64_32_def& f, MemFile* mf) {
    f.opaque = mf;
    f.zseek64 = &mem_zseek64;
    f.ztell64 = &mem_ztell64;
    f.zread64 = &mem_zread64;
}

// Simple test harness
static int total_tests = 0;
static int failed_tests = 0;
static std::vector<std::string> failure_msgs;

static void log_failure(const std::string& msg) {
    failure_msgs.push_back(msg);
    ++failed_tests;
}

// Test 1: Success path - locator found and eocd signature valid
static void test_success_locator_found() {
    ++total_tests;
    MemFile mf;
    // Build a file with a locator located at offset 120 and eocd at 140
    const size_t file_size = 200;
    mf.data.resize(file_size, 0);

    // Locator at offset 120: [signature][diskStart][relativeOffset(8)][totalDisks]
    size_t loc_off = 120;
    uint8_t locator_sig[4] = {0x50, 0x4B, 0x06, 0x07}; // 0x07064b50 little-endian
    MFIO:
    // Copy signature
    std::copy(locator_sig, locator_sig+4, mf.data.begin() + loc_off);
    // Disk start of Zip64 ECD
    mf.data[loc_off+4+0] = 0x00;
    mf.data[loc_off+4+1] = 0x00;
    mf.data[loc_off+4+2] = 0x00;
    mf.data[loc_off+4+3] = 0x00;
    // Relative offset (8 bytes little-endian) -> 140
    ZPOS64_T rel = 140;
    mf.data[loc_off+8+0]  = (uint8_t)(rel & 0xFF);
    mf.data[loc_off+8+1]  = (uint8_t)((rel>>8) & 0xFF);
    mf.data[loc_off+8+2]  = (uint8_t)((rel>>16) & 0xFF);
    mf.data[loc_off+8+3]  = (uint8_t)((rel>>24) & 0xFF);
    mf.data[loc_off+8+4]  = (uint8_t)((rel>>32) & 0xFF);
    mf.data[loc_off+8+5]  = (uint8_t)((rel>>40) & 0xFF);
    mf.data[loc_off+8+6]  = (uint8_t)((rel>>48) & 0xFF);
    mf.data[loc_off+8+7]  = (uint8_t)((rel>>56) & 0xFF);
    // Total disks = 1
    mf.data[loc_off+16] = 0x01;
    mf.data[loc_off+17] = 0x00;
    mf.data[loc_off+18] = 0x00;
    mf.data[loc_off+19] = 0x00;

    // EOCd signature at position rel (140)
    mf.data[140+0] = 0x50;
    mf.data[140+1] = 0x4B;
    mf.data[140+2] = 0x06;
    mf.data[140+3] = 0x06;

    zlib_filefunc64_32_def f;
    init_zipfunc(f, &mf);

    ZPOS64_T result = zip64local_SearchCentralDir64(&f, (void*)(&mf));
    if (result != 140) {
        log_failure("test_success_locator_found: expected relative offset 140, got " + std::to_string(result));
    }
}

// Test 2: No locator found -> returns 0
static void test_no_locator_found() {
    ++total_tests;
    MemFile mf;
    // Create a file with no locator signature
    mf.data.resize(200, 0);
    zlib_filefunc64_32_def f;
    init_zipfunc(f, &mf);

    ZPOS64_T result = zip64local_SearchCentralDir64(&f, (void*)(&mf));
    if (result != 0) {
        log_failure("test_no_locator_found: expected 0, got " + std::to_string(result));
    }
}

// Test 3: Locator found but eocd signature invalid -> returns 0
static void test_incorrect_eocd_signature() {
    ++total_tests;
    MemFile mf;
    // Locator at offset 100
    size_t loc_off = 100;
    uint8_t locator_sig[4] = {0x50, 0x4B, 0x06, 0x07};
    std::copy(locator_sig, locator_sig+4, mf.data.begin() + loc_off);
    mf.data[loc_off+4] = 0x00; mf.data[loc_off+5] = 0x00; mf.data[loc_off+6] = 0x00; mf.data[loc_off+7] = 0x00;
    ZPOS64_T rel = 120; // eocd offset
    mf.data[loc_off+8+0] = (uint8_t)(rel & 0xFF);
    mf.data[loc_off+8+1] = (uint8_t)((rel>>8) & 0xFF);
    mf.data[loc_off+8+2] = (uint8_t)((rel>>16) & 0xFF);
    mf.data[loc_off+8+3] = (uint8_t)((rel>>24) & 0xFF);
    mf.data[loc_off+8+4] = (uint8_t)((rel>>32) & 0xFF);
    mf.data[loc_off+8+5] = (uint8_t)((rel>>40) & 0xFF);
    mf.data[loc_off+8+6] = (uint8_t)((rel>>48) & 0xFF);
    mf.data[loc_off+8+7] = (uint8_t)((rel>>56) & 0xFF);
    mf.data[loc_off+16] = 0x01; mf.data[loc_off+17] = 0x00; mf.data[loc_off+18] = 0x00; mf.data[loc_off+19] = 0x00;

    // At rel (120), set wrong eocd signature (not 0x06064b50)
    mf.data[120+0] = 0x11;
    mf.data[120+1] = 0x22;
    mf.data[120+2] = 0x33;
    mf.data[120+3] = 0x44;

    zlib_filefunc64_32_def f;
    init_zipfunc(f, &mf);

    ZPOS64_T result = zip64local_SearchCentralDir64(&f, (void*)(&mf));
    if (result != 0) {
        log_failure("test_incorrect_eocd_signature: expected 0, got " + std::to_string(result));
    }
}

// Test 4: Seek end failure simulation -> should return 0
static void test_seek_end_failure() {
    ++total_tests;
    MemFile mf;
    // We'll cause end seek to fail by enabling fail_end_seek flag
    mf.data.resize(200, 0);
    mf.fail_end_seek = true;
    zlib_filefunc64_32_def f;
    init_zipfunc(f, &mf);

    ZPOS64_T result = zip64local_SearchCentralDir64(&f, (void*)(&mf));
    if (result != 0) {
        log_failure("test_seek_end_failure: expected 0 due to end seek fail, got " + std::to_string(result));
    }
}

// Driver
int main() {
    // Run tests
    test_success_locator_found();
    test_no_locator_found();
    test_incorrect_eocd_signature();
    test_seek_end_failure();

    // Report
    if (failed_tests == 0) {
        std::cout << "All " << total_tests << " tests passed." << std::endl;
        return 0;
    } else {
        std::cout << failed_tests << " / " << total_tests << " tests failed." << std::endl;
        for (const auto& s : failure_msgs) {
            std::cout << "  - " << s << std::endl;
        }
        return 1;
    }
}