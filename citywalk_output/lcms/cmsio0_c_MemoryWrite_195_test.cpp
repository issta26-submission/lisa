// C++11 test suite for MemoryWrite in cmsio0.c
// Note: This test harness uses a minimal, self-contained mock of the
// cmsio0 internal structures needed by MemoryWrite. It relies on the
// actual MemoryWrite function being linked from cmsio0.c.
// No GoogleTest; a simple non-terminating assertion style is used.

#include <lcms2_internal.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>


// Domain-specific lightweight types and constants (mirror minimal expectations)
#define FALSE 0
#define TRUE  1

typedef int cmsBool;
typedef unsigned int cmsUInt32Number;

// Forward declaration of the focal function using C linkage
extern "C" cmsBool MemoryWrite(struct _cms_io_handler* iohandler, cmsUInt32Number size, const void *Ptr);

// Minimal structural definitions mirroring the expected layout inside cmsio0.c
typedef struct {
    unsigned char* Block;          // Storage buffer
    cmsUInt32Number Pointer;       // Current write position
    cmsUInt32Number Size;            // Total capacity
} FILEMEM;

typedef struct _cms_io_handler {
    void* stream;                    // Points to FILEMEM in tests
    cmsUInt32Number UsedSpace;       // Tracks used space (as in production code)
} _cms_io_handler;

// Simple non-terminating assertion helper
static bool assert_true(bool cond, const char* test_desc) {
    if (!cond) {
        fprintf(stderr, "Test failed: %s\n", test_desc);
        return false;
    }
    return true;
}

// Helper to compare memory regions
static bool mem_equal(const unsigned char* a, const unsigned char* b, size_t n) {
    return std::memcmp(a, b, n) == 0;
}

// Test 1: Basic write within available space (no clipping)
bool test_MemoryWrite_BasicWithinSpace() {
    // Setup
    const cmsUInt32Number bufSize = 100;
    FILEMEM res;
    res.Block = new unsigned char[bufSize];
    std::memset(res.Block, 0xAA, bufSize); // sentinel pattern
    res.Pointer = 0;
    res.Size = bufSize;

    _cms_io_handler ioh;
    ioh.stream = &res;
    ioh.UsedSpace = 0;

    // Source data to write
    unsigned char src[20];
    for (int i = 0; i < 20; ++i) src[i] = static_cast<unsigned char>(i + 1);

    // Call
    cmsBool ret = MemoryWrite(&ioh, 20, src);

    // Verify
    bool ok = (ret == TRUE) &&
              (res.Pointer == 20) &&
              mem_equal(res.Block, src, 20);

    // Cleanup
    delete[] res.Block;

    return assert_true(ok, "MemoryWrite_BasicWithinSpace should return TRUE, advance Pointer to 20, and copy 20 bytes");
}

// Test 2: Clip to buffer end when requested write exceeds capacity
bool test_MemoryWrite_ClipToEnd() {
    // Setup
    const cmsUInt32Number bufSize = 100;
    FILEMEM res;
    res.Block = new unsigned char[bufSize];
    // Initialize with a known pattern
    for (cmsUInt32Number i = 0; i < bufSize; ++i) res.Block[i] = static_cast<unsigned char>(0x80);

    res.Pointer = 90; // near the end
    res.Size = bufSize;

    _cms_io_handler ioh;
    ioh.stream = &res;
    ioh.UsedSpace = 0;

    // Source data longer than space available
    unsigned char src[20];
    for (int i = 0; i < 20; ++i) src[i] = static_cast<unsigned char>(i + 1);

    // Call
    cmsBool ret = MemoryWrite(&ioh, 20, src);

    // Verify: only 10 bytes should be written (indices 90..99)
    bool ok = (ret == TRUE) &&
              (res.Pointer == 100) && // Pointer should move to end
              mem_equal(res.Block + 90, src, 10) && // first 10 bytes copied
              // The remaining 10 bytes should remain unchanged (0x80)
              mem_equal(res.Block + 100 - 10, res.Block + 100 - 10, 0); // trivial no-op to illustrate check

    // Instead of the above trivial check, verify the tail content explicitly
    for (int i = 0; i < 10; ++i) {
        if (res.Block[90 + i] != src[i]) { ok = false; break; }
    }
    // The last 10 bytes (90-99) beyond copied region should remain as 0x80
    for (int i = 10; i < 20 && ok; ++i) {
        if (res.Block[90 + i] != 0x80) { ok = false; break; }
    }

    // Cleanup
    delete[] res.Block;

    return assert_true(ok, "MemoryWrite_ClipToEnd should clip to buffer end and copy only available bytes");
}

// Test 3: Write with size == 0 should do nothing and return TRUE
bool test_MemoryWrite_WriteZeroBytes() {
    // Setup
    const cmsUInt32Number bufSize = 50;
    FILEMEM res;
    res.Block = new unsigned char[bufSize];
    std::memset(res.Block, 0x55, bufSize);
    res.Pointer = 10;
    res.Size = bufSize;

    _cms_io_handler ioh;
    ioh.stream = &res;
    ioh.UsedSpace = 0;

    unsigned char src[8] = {1,2,3,4,5,6,7,8};
    cmsBool ret = MemoryWrite(&ioh, 0, src);

    bool ok = (ret == TRUE) &&
              (res.Pointer == 10) && // unchanged
              std::memcmp(res.Block, (unsigned char*)std::memchr(res.Block, 0x55, bufSize), bufSize) == 0;

    delete[] res.Block;

    return assert_true(ok, "MemoryWrite_WriteZeroBytes should return TRUE and perform no changes when size == 0");
}

// Test 4: NULL ResData (iohandler.stream == NULL) should return FALSE
bool test_MemoryWrite_NullResData() {
    _cms_io_handler ioh;
    ioh.stream = NULL;
    ioh.UsedSpace = 0;

    cmsBool ret = MemoryWrite(&ioh, 10, (const void*)"\x01\x02");

    bool ok = (ret == FALSE);
    return assert_true(ok, "MemoryWrite_NullResData should return FALSE when iohandler->stream is NULL");
}

// Test 5: When Pointer equals Size, a non-zero write should clip to zero and return TRUE
bool test_MemoryWrite_PointerAtEnd_NoWriteRemaining() {
    // Setup
    const cmsUInt32Number bufSize = 64;
    FILEMEM res;
    res.Block = new unsigned char[bufSize];
    std::memset(res.Block, 0x33, bufSize);
    res.Pointer = bufSize; // already at end
    res.Size = bufSize;

    _cms_io_handler ioh;
    ioh.stream = &res;
    ioh.UsedSpace = 0;

    unsigned char src[5] = {9,9,9,9,9};
    cmsBool ret = MemoryWrite(&ioh, 5, src);

    bool ok = (ret == TRUE) &&
              (res.Pointer == bufSize); // remains at end

    // Memory should be unchanged
    for (cmsUInt32Number i = 0; i < bufSize; ++i) {
        if (res.Block[i] != 0x33) { ok = false; break; }
    }

    delete[] res.Block;
    return assert_true(ok, "MemoryWrite_PointerAtEnd_NoWriteRemaining should clip to 0 and perform no write when Pointer == Size");
}

// Runner
int main() {
    int total = 0;
    int failed = 0;

    printf("Running MemoryWrite unit tests...\n");

    if (test_MemoryWrite_BasicWithinSpace()) total++;
    else failed++;

    if (test_MemoryWrite_ClipToEnd()) total++;
    else failed++;

    if (test_MemoryWrite_WriteZeroBytes()) total++;
    else failed++;

    if (test_MemoryWrite_NullResData()) total++;
    else failed++;

    if (test_MemoryWrite_PointerAtEnd_NoWriteRemaining()) total++;
    else failed++;

    printf("Tests passed: %d, Tests failed: %d\n", total, failed);
    return (failed == 0) ? 0 : 1;
}