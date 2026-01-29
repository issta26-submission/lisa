// Test suite for the focal method Type_Data_Read from cmstypes.c
// Note: This test suite is written in C++11 and does not use GTest.
// It relies on the Little CMS internal API (lcms2_internal.h) and the public API (lcms2.h).
// The tests implement a lightweight test harness with non-terminating assertions.

#include <cstring>
#include <cstdio>
#include <vector>
#include <climits>
#include <lcms2_internal.h>
#include <lcms2.h>



extern "C" {

// Declare the focal function from the library (signature provided in the prompt)
void *Type_Data_Read(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsUInt32Number* nItems, cmsUInt32Number SizeOfTag);

// We also use the internal memory free helper declared in the library
void _cmsFree(cmsContext ContextID, void* Ptr);
}

// Simple non-terminating assertion macros
static int gFailures = 0;
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        fprintf(stderr, "ASSERTION FAILED: %s is false at %s:%d\n", #cond, __FILE__, __LINE__); \
        ++gFailures; \
    } \
} while(0)

#define EXPECT_NULL(ptr) do { \
    if((ptr) != NULL) { \
        fprintf(stderr, "ASSERTION FAILED: %s is not NULL at %s:%d\n", #ptr, __FILE__, __LINE__); \
        ++gFailures; \
    } \
} while(0)

#define EXPECT_NOT_NULL(ptr) do { \
    if((ptr) == NULL) { \
        fprintf(stderr, "ASSERTION FAILED: %s is NULL at %s:%d\n", #ptr, __FILE__, __LINE__); \
        ++gFailures; \
    } \
} while(0)

#define EXPECT_EQ_U32(a, b) do { \
    if(((a) != (b))) { \
        fprintf(stderr, "ASSERTION FAILED: %s == %u, actual %u at %s:%d\n", #a, (unsigned)(a), (unsigned)(b), __FILE__, __LINE__); \
        ++gFailures; \
    } \
} while(0)

#define EXPECT_MEMEQ(a, b, n) do { \
    if(std::memcmp((a), (b), (n)) != 0) { \
        fprintf(stderr, "ASSERTION FAILED: memory differs for %s at %s:%d\n", #a, __FILE__, __LINE__); \
        ++gFailures; \
    } \
} while(0)


// IO buffer helper to feed data to the cmsIOHANDLER Read function
struct IOBuffer {
    std::vector<uint8_t> bytes;
    size_t pos;
};

// Read function that simulates a real IORead: returns requested bytes from the buffer
static cmsUInt32Number Read_Success(struct _cmsIOHANDLER* io, cmsUInt32Number size, cmsUInt8Number* data) {
    IOBuffer* buf = (IOBuffer*) io->Context;
    if (buf == nullptr) return 0;
    // size * count is not directly provided; the API uses size as element size and
    // count is passed as '1' in the pattern used by _cmsReadUInt32Number when reading a 32-bit int.
    // So we take 'size' as the number of bytes to read for this call.
    cmsUInt32Number toRead = size;
    if (buf->pos >= buf->bytes.size()) return 0;
    if (buf->pos + toRead > buf->bytes.size()) toRead = buf->bytes.size() - buf->pos;
    if (toRead == 0) return 0;
    std::memcpy(data, buf->bytes.data() + buf->pos, (size_t)toRead);
    buf->pos += (size_t)toRead;
    return toRead;
}

// Read function that simulates failure on the first Read (to test early-exit on flag read)
static cmsUInt32Number Read_FailOnFlag(struct _cmsIOHANDLER* io, cmsUInt32Number size, cmsUInt8Number* data) {
    IOBuffer* buf = (IOBuffer*) io->Context;
    if (buf == nullptr) return 0;
    // Fail on the very first read (attempt to read 4 bytes for the flag)
    if (buf->pos == 0) {
        return 0;
    }
    // Otherwise behave like success
    cmsUInt32Number toRead = size;
    if (buf->pos >= buf->bytes.size()) return 0;
    if (buf->pos + toRead > buf->bytes.size()) toRead = buf->bytes.size() - buf->pos;
    if (toRead == 0) return 0;
    std::memcpy(data, buf->bytes.data() + buf->pos, (size_t)toRead);
    buf->pos += (size_t)toRead;
    return toRead;
}

// Read function that simulates failure during the data portion (after flag has been read)
static cmsUInt32Number Read_FailOnData(struct _cmsIOHANDLER* io, cmsUInt32Number size, cmsUInt8Number* data) {
    IOBuffer* buf = (IOBuffer*) io->Context;
    if (buf == nullptr) return 0;
    // After the first 4-byte flag read, allow reading only first 3 bytes of data;
    // The subsequent data read should fail (simulate truncated data).
    if (buf->pos >= 4) { // after flag read
        if (buf->pos >= 7) { // if we've already attempted reading >3 bytes of data, fail
            return 0;
        }
    }
    cmsUInt32Number toRead = size;
    if (buf->pos >= buf->bytes.size()) return 0;
    if (buf->pos + toRead > buf->bytes.size()) toRead = buf->bytes.size() - buf->pos;
    if (toRead == 0) return 0;
    std::memcpy(data, buf->bytes.data() + buf->pos, (size_t)toRead);
    buf->pos += (size_t)toRead;
    return toRead;
}
} // extern "C"


// Helper to create a small 32-bit little-endian value
static void packLE24(uint8_t* dst, uint32_t v) {
    dst[0] = (uint8_t)(v & 0xFF);
    dst[1] = (uint8_t)((v >> 8) & 0xFF);
    dst[2] = (uint8_t)((v >> 16) & 0xFF);
    dst[3] = (uint8_t)((v >> 24) & 0xFF);
}

// Test 1: SizeOfTag smaller than cmsUInt32Number should return NULL (true branch)
static void test_Type_Data_Read_SizeTooSmall() {
    // Prepare a dummy self with null context
    struct _cms_typehandler_struct self;
    self.ContextID = NULL;

    cmsUInt32Number nItems = 0;
    cmsUInt32Number SizeOfTag = 3; // less than sizeof(cmsUInt32Number) which is 4

    cmsIOHANDLER io;
    std::memset(&io, 0, sizeof(io)); // ensure fields are zeroed
    // Not used in this path

    void* res = Type_Data_Read(&self, &io, &nItems, SizeOfTag);
    EXPECT_NULL(res);
}

// Test 2: SizeOfTag causes LenOfData > INT_MAX -> should return NULL (true branch)
static void test_Type_Data_Read_LenOfDataTooBig() {
    struct _cms_typehandler_struct self;
    self.ContextID = NULL;

    cmsUInt32Number nItems = 0;
    // SizeOfTag = sizeof(cmsUInt32Number) + (INT_MAX + 1)
    cmsUInt32Number SizeOfTag = (cmsUInt32Number)(sizeof(cmsUInt32Number) + (INT_MAX) + 1);

    cmsIOHANDLER io;
    std::memset(&io, 0, sizeof(io));

    void* res = Type_Data_Read(&self, &io, &nItems, SizeOfTag);
    EXPECT_NULL(res);
}

// Test 3: Successful read path
static void test_Type_Data_Read_Success() {
    // Create a real cmsContext to allow proper memory allocation
    cmsContext ctx = cmsCreateContext(NULL, NULL);

    struct _cms_typehandler_struct self;
    self.ContextID = ctx;

    cmsUInt32Number nItems = 0;

    // LenOfData = 5; total size = 4 (flag) + 5 = 9
    const cmsUInt32Number LenOfData = 5;
    const cmsUInt32Number SizeOfTag = (cmsUInt32Number)(sizeof(cmsUInt32Number) + LenOfData);

    // Build IO buffer: 4 bytes flag little-endian (0x01020304), followed by 5 data bytes
    std::vector<uint8_t> dataBuffer;
    // flag = 0x01020304 in little-endian -> 04 03 02 01
    dataBuffer.push_back(0x04);
    dataBuffer.push_back(0x03);
    dataBuffer.push_back(0x02);
    dataBuffer.push_back(0x01);
    // data bytes
    dataBuffer.push_back(0xAA);
    dataBuffer.push_back(0xBB);
    dataBuffer.push_back(0xCC);
    dataBuffer.push_back(0xDD);
    dataBuffer.push_back(0xEE);

    IOBuffer ioBuf;
    ioBuf.bytes = dataBuffer;
    ioBuf.pos = 0;

    cmsIOHANDLER io;
    std::memset(&io, 0, sizeof(io));
    io.Context = &ioBuf;
    io.Read = Read_Success;

    void* res = Type_Data_Read(&self, &io, &nItems, SizeOfTag);
    EXPECT_NOT_NULL(res);
    if (res != NULL) {
        cmsICCData* BinData = (cmsICCData*)res;
        // Verify len
        EXPECT_EQ_U32(BinData->len, LenOfData);
        // Verify flag
        EXPECT_EQ_U32(BinData->flag, 0x01020304);
        // Verify data payload
        uint8_t expected[LenOfData] = {0xAA,0xBB,0xCC,0xDD,0xEE};
        EXPECT_MEMEQ(BinData->data, expected, LenOfData);
        // Free allocated memory using the same context
        _cmsFree(ctx, res);
    }
    cmsCloseContext(ctx);
}

// Test 4: Failure to read the 4-byte flag (simulate Read failure on flag)
static void test_Type_Data_Read_FailOnFlagRead() {
    cmsContext ctx = cmsCreateContext(NULL, NULL);

    struct _cms_typehandler_struct self;
    self.ContextID = ctx;

    cmsUInt32Number nItems = 0;
    const cmsUInt32Number LenOfData = 3;
    const cmsUInt32Number SizeOfTag = (cmsUInt32Number)(sizeof(cmsUInt32Number) + LenOfData);

    // Prepare buffer (flag bytes won't be consumed because Read fails on first call)
    std::vector<uint8_t> dataBuffer;
    dataBuffer.push_back(0x04);
    dataBuffer.push_back(0x03);
    dataBuffer.push_back(0x02);
    dataBuffer.push_back(0x01);
    dataBuffer.push_back(0xAA);
    dataBuffer.push_back(0xBB);
    dataBuffer.push_back(0xCC);

    IOBuffer ioBuf;
    ioBuf.bytes = dataBuffer;
    ioBuf.pos = 0;

    cmsIOHANDLER io;
    std::memset(&io, 0, sizeof(io));
    io.Context = &ioBuf;
    io.Read = Read_FailOnFlag; // This test triggers failure on the first Read

    void* res = Type_Data_Read(&self, &io, &nItems, SizeOfTag);
    EXPECT_NULL(res);

    cmsCloseContext(ctx);
}

// Test 5: Failure to read data payload after flag (simulate Read failure during data)
static void test_Type_Data_Read_FailOnDataRead() {
    cmsContext ctx = cmsCreateContext(NULL, NULL);

    struct _cms_typehandler_struct self;
    self.ContextID = ctx;

    cmsUInt32Number nItems = 0;
    const cmsUInt32Number LenOfData = 5;
    const cmsUInt32Number SizeOfTag = (cmsUInt32Number)(sizeof(cmsUInt32Number) + LenOfData);

    // Prepare buffer with flag and partial data (3 bytes; 2 bytes short)
    std::vector<uint8_t> dataBuffer;
    // flag = 0x01020304 -> 04 03 02 01
    dataBuffer.push_back(0x04);
    dataBuffer.push_back(0x03);
    dataBuffer.push_back(0x02);
    dataBuffer.push_back(0x01);
    // partial data (only 3 bytes instead of 5)
    dataBuffer.push_back(0xAA);
    dataBuffer.push_back(0xBB);
    dataBuffer.push_back(0xCC);
    // Note: We intentionally do not provide the remaining 2 bytes to trigger read failure

    IOBuffer ioBuf;
    ioBuf.bytes = dataBuffer;
    ioBuf.pos = 0;

    cmsIOHANDLER io;
    std::memset(&io, 0, sizeof(io));
    io.Context = &ioBuf;
    io.Read = Read_FailOnData; // This test triggers failure during the data payload read

    void* res = Type_Data_Read(&self, &io, &nItems, SizeOfTag);
    EXPECT_NULL(res);

    cmsCloseContext(ctx);
}


int main() {
    printf("Starting Type_Data_Read unit tests (non-GTest, non-exit assertions)...\n");

    test_Type_Data_Read_SizeTooSmall();
    test_Type_Data_Read_LenOfDataTooBig();
    test_Type_Data_Read_Success();
    test_Type_Data_Read_FailOnFlagRead();
    test_Type_Data_Read_FailOnDataRead();

    if (gFailures == 0) {
        printf("ALL tests PASSED (non-terminating assertions).\n");
    } else {
        printf("Tests completed with %d failure(s).\n", gFailures);
    }
    return gFailures;
}