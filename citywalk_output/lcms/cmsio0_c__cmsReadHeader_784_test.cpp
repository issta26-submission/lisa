// This test suite targets the focal method _cmsReadHeader in cmsio0.c.
// It uses a minimal, self-contained harness (no GTest) to exercise
// failure paths and a couple of scenarios for the header read path.
// The tests drive a fake in-memory IO handler to provide a crafted ICC header
// and tag directory data. The tests are written in C++11 but exercise C code
// from the Little CMS library (lcms2). Extern "C" is used where needed.

#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include core Little CMS interfaces and internal structures for test access.
extern "C" {

// Expose the focal function (not part of public API)
cmsBool _cmsReadHeader(_cmsICCPROFILE* Icc);
}

// Simple testing framework (non-terminating assertions)
static int g_tests_total = 0;
static int g_tests_passed = 0;

#define TEST_CASE(name) bool name()

#define ASSERT_TRUE(cond, msg) do { \
    if (cond) { \
        ++g_tests_passed; \
    } else { \
        std::cerr << "Assertion failed: " << (msg ? msg : "") << " (line=" << __LINE__ << ")" << std::endl; \
    } \
    ++g_tests_total; \
} while(0)

#define ASSERT_FALSE(cond, msg) do { \
    if (!(cond)) { \
        ++g_tests_passed; \
    } else { \
        std::cerr << "Assertion failed (expected false): " << (msg ? msg : "") << " (line=" << __LINE__ << ")" << std::endl; \
    } \
    ++g_tests_total; \
} while(0)


// Test IO layer: a memory-backed Read function to feed header and tag data.
typedef struct _TestIO {
    cmsIOHANDLER base;                     // Must be first for pointer casts
    std::vector<unsigned char> data;       // contiguous data stream
    size_t pos;                              // current read position
    bool failHeader;                         // trigger header read failure
} TestIO;

// Forward declaration for memory read
static cmsUInt32Number MemoryRead(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count);

// Helper: create a TestIO instance and attach to cmsICCPROFILE
static TestIO makeTestIO() {
    TestIO t;
    std::memset(&t, 0, sizeof(t));
    t.pos = 0;
    t.failHeader = false;
    t.base.Read = MemoryRead;
    // To keep things simple, no mutex or extra functionality required for tests
    return t;
}

// Implementation of memory Read used by the test IO
static cmsUInt32Number MemoryRead(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    // Cast to our test IO container. The cmsIOHANDLER is the first member, so the cast is valid.
    TestIO* t = (TestIO*)iohandler;

    if (t->failHeader) {
        // Simulate an IO failure on header read
        return 0;
    }

    cmsUInt32Number toRead = size * count;
    if (toRead == 0) return 0;

    if (t->pos >= t->data.size()) return 0;
    cmsUInt32Number available = (cmsUInt32Number)(t->data.size() - t->pos);
    if (toRead > available) toRead = available;

    if (toRead > 0) {
        std::memcpy(Buffer, t->data.data() + t->pos, toRead);
        t->pos += toRead;
    }

    // Return how many "elements" were read
    return (cmsUInt32Number)(toRead / size);
}

// Helpers to build specific header data scenarios

// Build a valid-ish ICC header with TagCount = 0 (successful path, zero tags)
static void buildHeaderWithZeroTags(TestIO& t) {
    cmsICCHeader hdr;
    std::memset(&hdr, 0, sizeof(hdr));
    hdr.magic = cmsMagicNumber;
    hdr.cmmId = 0;
    hdr.deviceClass = cmsSigDisplayClass; // assume this is a valid device class
    hdr.colorSpace = 0;
    hdr.pcs = 0;
    hdr.renderingIntent = 0;
    hdr.platform = 0;
    hdr.flags = 0;
    hdr.manufacturer = 0;
    hdr.model = 0;
    hdr.creator = 0;
    hdr.attributes = 0;
    hdr.version = 0;
    hdr.size = 1024; // header size
    // date and profileID left at zero (memset already did)
    // Copy header into data
    unsigned char* p = (unsigned char*)&hdr;
    for (size_t i = 0; i < sizeof(hdr); ++i) t.data.push_back(p[i]);
    // TagCount = 0 (4 bytes)
    uint32_t tagCount = 0;
    unsigned char* tc = (unsigned char*)&tagCount;
    for (size_t i = 0; i < 4; ++i) t.data.push_back(tc[i]);
}

// Build a header with invalid signature (header signature mismatch)
static void buildHeaderWithInvalidSignature(TestIO& t) {
    cmsICCHeader hdr;
    std::memset(&hdr, 0, sizeof(hdr));
    hdr.magic = 0xDEADBEEF;            // not cmsMagicNumber
    hdr.cmmId = 0;
    hdr.deviceClass = cmsSigDisplayClass;
    hdr.size = 1024;
    unsigned char* p = (unsigned char*)&hdr;
    for (size_t i = 0; i < sizeof(hdr); ++i) t.data.push_back(p[i]);
    uint32_t tagCount = 0;
    unsigned char* tc = (unsigned char*)&tagCount;
    for (size_t i = 0; i < 4; ++i) t.data.push_back(tc[i]);
}

// Build a header with two identical tag entries to trigger duplicate tag error
static void buildHeaderWithTwoIdenticalTags(TestIO& t) {
    cmsICCHeader hdr;
    std::memset(&hdr, 0, sizeof(hdr));
    hdr.magic = cmsMagicNumber;
    hdr.cmmId = 0;
    hdr.deviceClass = cmsSigDisplayClass;
    hdr.colorSpace = 0;
    hdr.pcs = 0;
    hdr.renderingIntent = 0;
    hdr.platform = 0;
    hdr.flags = 0;
    hdr.manufacturer = 0;
    hdr.model = 0;
    hdr.creator = 0;
    hdr.attributes = 0;
    hdr.version = 0;
    hdr.size = 1024; // HeaderSize
    unsigned char* p = (unsigned char*)&hdr;
    for (size_t i = 0; i < sizeof(hdr); ++i) t.data.push_back(p[i]);

    // TagCount = 2
    uint32_t tagCount = 2;
    unsigned char* tc = (unsigned char*)&tagCount;
    for (size_t i = 0; i < 4; ++i) t.data.push_back(tc[i]);

    // Tag 0: sig=0x11111111, offset=100, size=10
    uint32_t sig1 = 0x11111111;
    uint32_t off1 = 100;
    uint32_t sz1  = 10;

    unsigned char* s1 = (unsigned char*)&sig1;
    unsigned char* o1 = (unsigned char*)&off1;
    unsigned char* z1 = (unsigned char*)&sz1;
    for (size_t i = 0; i < 4; ++i) t.data.push_back(s1[i]);
    for (size_t i = 0; i < 4; ++i) t.data.push_back(o1[i]);
    for (size_t i = 0; i < 4; ++i) t.data.push_back(z1[i]);

    // Tag 1: sig=0x11111111 (duplicate), offset=200, size=10
    uint32_t sig2 = 0x11111111;
    uint32_t off2 = 200;
    uint32_t sz2  = 10;
    unsigned char* s2 = (unsigned char*)&sig2;
    unsigned char* o2 = (unsigned char*)&off2;
    unsigned char* z2 = (unsigned char*)&sz2;
    for (size_t i = 0; i < 4; ++i) t.data.push_back(s2[i]);
    for (size_t i = 0; i < 4; ++i) t.data.push_back(o2[i]);
    for (size_t i = 0; i < 4; ++i) t.data.push_back(z2[i]);
}

// Success case: header valid but TagCount = 0 paths through successfully
static bool runTest_HeaderRead_Success_Path() {
    TestIO t = makeTestIO();
    buildHeaderWithZeroTags(t);
    // Prepare ICC object
    _cmsICCPROFILE icc;
    std::memset(&icc, 0, sizeof(_cmsICCPROFILE));
    icc.IOhandler = &t.base;
    icc.ContextID = 1;

    // Call the focal function
    cmsBool res = _cmsReadHeader(&icc);
    // True path expected
    if (res != TRUE) {
        std::cerr << "Test: HeaderRead_Success_Path failed: _cmsReadHeader returned FALSE" << std::endl;
        return false;
    }

    // Verify some key fields were populated from header
    // Version should be derived from header.version (0 in our test)
    ASSERT_TRUE(icc.Version == 0, "Version should be 0 for zeroed header version");
    // DeviceClass should be preserved from header (DisplayClass in our header)
    ASSERT_TRUE(icc.DeviceClass == cmsSigDisplayClass, "DeviceClass should be cmsSigDisplayClass");
    // TagCount should be 0
    ASSERT_TRUE(icc.TagCount == 0, "TagCount should be 0 in success path with zero tags");
    return true;
}

// Failure path: header read fails
static bool runTest_HeaderRead_FailOnHeaderRead() {
    TestIO t = makeTestIO();
    t.failHeader = true; // Force header read failure
    // Do not push actual header bytes since Read will fail
    icms;
    _cmsICCPROFILE icc;
    std::memset(&icc, 0, sizeof(_cmsICCPROFILE));
    icc.IOhandler = &t.base;
    icc.ContextID = 1;

    cmsBool res = _cmsReadHeader(&icc);
    // Expect FALSE due to IO failure
    ASSERT_FALSE(res, "HeaderRead should fail when header read cannot be completed");
    return true;
}

// Failure path: header read but bad signature
static bool runTest_HeaderRead_BadSignature() {
    TestIO t = makeTestIO();
    buildHeaderWithInvalidSignature(t);

    _cmsICCPROFILE icc;
    std::memset(&icc, 0, sizeof(_cmsICCPROFILE));
    icc.IOhandler = &t.base;
    icc.ContextID = 1;

    cmsBool res = _cmsReadHeader(&icc);
    // Expect FALSE due to bad signature
    ASSERT_FALSE(res, "HeaderRead should fail when header signature is invalid");
    return true;
}

// Failure path: duplicate tags detected
static bool runTest_HeaderRead_DuplicateTags() {
    TestIO t = makeTestIO();
    buildHeaderWithTwoIdenticalTags(t);

    _cmsICCPROFILE icc;
    std::memset(&icc, 0, sizeof(_cmsICCPROFILE));
    icc.IOhandler = &t.base;
    icc.ContextID = 1;

    cmsBool res = _cmsReadHeader(&icc);
    // Expect FALSE due to duplicate tag found
    ASSERT_FALSE(res, "HeaderRead should fail when duplicate tags are detected");
    return true;
}


int main() {
    // Run tests with explanatory comments in code
    std::cout << "CMS Read Header Test Suite (CMSReadHeader) - begin\n";

    TEST_CASE(Test1) {
        return runTest_HeaderRead_Success_Path();
    } // Test1: Success path (zero tags)

    TEST_CASE(Test2) {
        return runTest_HeaderRead_FailOnHeaderRead();
    } // Test2: Header read failure

    TEST_CASE(Test3) {
        return runTest_HeaderRead_BadSignature();
    } // Test3: Invalid signature

    TEST_CASE(Test4) {
        return runTest_HeaderRead_DuplicateTags();
    } // Test4: Duplicate tags (should fail)

    // Execute all tests
    int total = g_tests_total;
    int passed = g_tests_passed;
    std::cout << "CMS Read Header Test Suite - finished: " << passed << " / " << total << " tests passed.\n";

    // Return success status
    return (passed == total) ? 0 : 1;
}