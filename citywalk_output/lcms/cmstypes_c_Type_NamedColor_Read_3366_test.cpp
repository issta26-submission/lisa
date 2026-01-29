// Unit test suite for the focal method Type_NamedColor_Read in cmstypes.c
// This test suite is written in C++11 without GoogleTest and uses a lightweight
// in-memory CMS IO implementation to drive the C API.
// The tests cover multiple execution paths of Type_NamedColor_Read, including
// success and several failure branches, to maximize coverage.

#include <cstring>
#include <vector>
#include <cstddef>
#include <iostream>
#include <string>
#include <algorithm>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include LittleCMS internal interfaces to access internal structures when needed.
extern "C" {
}

// Forward declare the focal method for linkage with C code
extern "C" void *Type_NamedColor_Read(struct _cms_typehandler_struct* self,
                                    cmsIOHANDLER* io,
                                    cmsUInt32Number* nItems,
                                    cmsUInt32Number SizeOfTag);

// Lightweight in-memory IO to drive Type_NamedColor_Read
class MemIO {
public:
    MemIO() : pos(0), ctx(nullptr) {}

    // Buffer of bytes to be returned on Read
    std::vector<uint8_t> data;
    size_t pos;

    void reset() { pos = 0; data.clear(); }

    // Attach arbitrary user data to be retrieved in Read callback
    void* ctx() const { return this->_ctx; }
    void set_ctx(void* c) { this->_ctx = c; }

private:
    void* _ctx;
};

// C-compatible Read callback used by cmsIOHANDLER
extern "C" cmsUInt32Number TestIO_Read(cmsIOHANDLER* io, void* Buffer, cmsUInt32Number Size, cmsUInt32Number Count)
{
    MemIO* mem = static_cast<MemIO*>(io->Context);
    if (!mem) return 0;

    cmsUInt32Number totalBytesRequested = Size * Count;
    if (totalBytesRequested == 0) return 0;

    size_t remaining = mem->data.size() - mem->pos;
    size_t toCopy = static_cast<size_t>(std::min<size_t>(remaining, static_cast<size_t>(totalBytesRequested)));

    if (toCopy == 0) return 0;
    std::memcpy(Buffer, mem->data.data() + mem->pos, toCopy);
    mem->pos += toCopy;
    // Return number of items actually read
    return static_cast<cmsUInt32Number>(toCopy / static_cast<size_t>(Size));
}

// Helpers to encode little-endian data into the test stream
static void push32LE(std::vector<uint8_t>& v, uint32_t x) {
    v.push_back((uint8_t)(x & 0xFF));
    v.push_back((uint8_t)((x >> 8) & 0xFF));
    v.push_back((uint8_t)((x >> 16) & 0xFF));
    v.push_back((uint8_t)((x >> 24) & 0xFF));
}
static void push16LE(std::vector<uint8_t>& v, uint16_t x) {
    v.push_back((uint8_t)(x & 0xFF));
    v.push_back((uint8_t)((x >> 8) & 0xFF));
}
static void push32Block(std::vector<uint8_t>& v, const char* s) {
    // Copy up to 32 bytes from s into a 32-byte block, zero-padded
    char buf[32] = {0};
    if (s) {
        size_t len = std::min<size_t>(std::strlen(s), 31);
        std::memcpy(buf, s, len);
        buf[len] = '\0';
    }
    v.insert(v.end(), reinterpret_cast<uint8_t*>(buf), reinterpret_cast<uint8_t*>(buf) + 32);
}
static void pushRoot32(std::vector<uint8_t>& v, const char* s) {
    push32Block(v, s);
}

// Test 1: Successful read path
bool test_NamedColor_Read_Success() {
    // Build an in-memory stream that matches the expected layout
    std::vector<uint8_t> stream;
    // vendorFlag, count, nDeviceCoords
    push32LE(stream, 0x00000001); // vendorFlag (value is irrelevant as long as it's readable)
    push32LE(stream, 0x00000001); // count = 1
    push32LE(stream, 0x00000002); // nDeviceCoords = 2 (<= cmsMAXCHANNELS)

    // prefix and suffix (32 bytes each)
    push32Block(stream, "PREFIX_TEST_COLOR");
    push32Block(stream, "SUFFIX_TEST_COLOR");

    // For each color (count = 1)
    // Root (32 bytes)
    pushRoot32(stream, "RootColorName1");
    // PCS[3] 16-bit values
    push16LE(stream, 100); // PCS[0]
    push16LE(stream, 200); // PCS[1]
    push16LE(stream, 300); // PCS[2]
    // Colorant: nDeviceCoords (2 values)
    push16LE(stream, 5);
    push16LE(stream, 10);

    // Prepare the memory IO
    MemIO* mem = new MemIO();
    mem->data = stream;
    mem->pos = 0;

    cmsIOHANDLER io;
    io.Read = &TestIO_Read;
    io.Write = nullptr;
    io.Context = mem;
    io.ContextSize = 0; // not used
    io.SizeOfTag = 0;
    io.Reserved = nullptr;

    // Self context with ContextID = 0 (default CMS context)
    struct _cms_typehandler_struct self;
    // Accessing a member requires internal header visibility; include via lcms2_internal.h
    // The internal header defines the member ContextID; we assume it exists.
    self.ContextID = 0;

    cmsUInt32Number nItems = 0;

    void* res = Type_NamedColor_Read(&self, &io, &nItems, 0);

    bool ok = (res != NULL) && (nItems == 1);
    // Clean up
    delete mem;
    // Not strictly required to free res as we don't expose the NAMEDCOLORLIST free interface here,
    // but we should not leak in tests. The library uses its own allocator; we free if non-NULL.
    if (res != NULL) {
        cmsFreeNamedColorList(static_cast<cmsNAMEDCOLORLIST*>(res));
    }
    return ok;
}

// Test 2: Failure on first read (vendorFlag read fails) -> expect NULL
bool test_NamedColor_Read_Fail_On_First_Read() {
    // Empty stream to force _cmsReadUInt32Number to fail
    MemIO* mem = new MemIO();
    mem->data = {}; // empty
    mem->pos = 0;

    cmsIOHANDLER io;
    io.Read = &TestIO_Read;
    io.Write = nullptr;
    io.Context = mem;
    io.ContextSize = 0;
    io.SizeOfTag = 0;
    io.Reserved = nullptr;

    struct _cms_typehandler_struct self;
    self.ContextID = 0;

    cmsUInt32Number nItems = 0;
    void* res = Type_NamedColor_Read(&self, &io, &nItems, 0);

    // Expect NULL due to read failure
    bool ok = (res == NULL);
    delete mem;
    return ok;
}

// Test 3: Too many device coordinates triggers error path (nDeviceCoords > cmsMAXCHANNELS)
bool test_NamedColor_Read_TooManyDeviceCoords() {
    std::vector<uint8_t> stream;
    push32LE(stream, 0x00000001); // vendorFlag
    push32LE(stream, 0x00000001); // count
    push32LE(stream, cmsMAXCHANNELS + 1); // nDeviceCoords -> triggers error after allocation

    // 32-byte prefix and suffix (values don't matter since failure happens before loop)
    push32Block(stream, "PFX");
    push32Block(stream, "SFX");

    // The rest won't be read due to early error, but provide any data to complete stream format
    pushRoot32(stream, "RootName");        // Root
    push16LE(stream, 1);                   // PCS[0]
    push16LE(stream, 2);                   // PCS[1]
    push16LE(stream, 3);                   // PCS[2]
    push16LE(stream, 4);                   // Colorant[0]
    push16LE(stream, 5);                   // Colorant[1]

    MemIO* mem = new MemIO();
    mem->data = stream;
    mem->pos = 0;

    cmsIOHANDLER io;
    io.Read = &TestIO_Read;
    io.Write = nullptr;
    io.Context = mem;
    io.ContextSize = 0;
    io.SizeOfTag = 0;
    io.Reserved = nullptr;

    struct _cms_typehandler_struct self;
    self.ContextID = 0;

    cmsUInt32Number nItems = 0;

    void* res = Type_NamedColor_Read(&self, &io, &nItems, 0);

    bool ok = (res == NULL);
    delete mem;
    return ok;
}

// Test 4: Root read failure inside loop (simulate by providing insufficient Root data)
bool test_NamedColor_Read_RootReadFailure() {
    // VendorFlag, count, nDeviceCoords = 1
    std::vector<uint8_t> stream;
    push32LE(stream, 0x00000001);
    push32LE(stream, 0x00000001);
    push32LE(stream, 0x00000001);

    // prefix/suffix
    push32Block(stream, "PRFX");
    push32Block(stream, "SFX");

    // For the single color: provide insufficient Root data (less than 32 bytes)
    // We'll supply only 4 bytes for Root to force the Read to fail (since it expects 32 bytes)
    // Root (we must provide 4 bytes; the reading logic expects 32 bytes and will fail)
    const char badRoot[4] = { 'R','o','o','t' };
    stream.insert(stream.end(), reinterpret_cast<const uint8_t*>(badRoot), reinterpret_cast<const uint8_t*>(badRoot) + 4);

    // PCS (3 x 2 bytes)
    push16LE(stream, 100);
    push16LE(stream, 200);
    push16LE(stream, 300);
    // Colorant (nDeviceCoords = 1) -> 1 value
    push16LE(stream, 7);

    MemIO* mem = new MemIO();
    mem->data = stream;
    mem->pos = 0;

    cmsIOHANDLER io;
    io.Read = &TestIO_Read;
    io.Write = nullptr;
    io.Context = mem;
    io.ContextSize = 0;
    io.SizeOfTag = 0;
    io.Reserved = nullptr;

    struct _cms_typehandler_struct self;
    self.ContextID = 0;

    cmsUInt32Number nItems = 0;

    void* res = Type_NamedColor_Read(&self, &io, &nItems, 0);

    bool ok = (res == NULL);
    delete mem;
    return ok;
}

int main() {
    int total = 0;
    int passed = 0;

    std::cout << "Running tests for Type_NamedColor_Read...\n";

    // Test 1: Successful read path
    total++;
    if (test_NamedColor_Read_Success()) {
        ++passed;
        std::cout << "[PASS] Test_NamedColor_Read_Success\n";
    } else {
        std::cout << "[FAIL] Test_NamedColor_Read_Success\n";
    }

    // Test 2: Failure on first read
    total++;
    if (test_NamedColor_Read_Fail_On_First_Read()) {
        ++passed;
        std::cout << "[PASS] Test_NamedColor_Read_Fail_On_First_Read\n";
    } else {
        std::cout << "[FAIL] Test_NamedColor_Read_Fail_On_First_Read\n";
    }

    // Test 3: Too many device coordinates
    total++;
    if (test_NamedColor_Read_TooManyDeviceCoords()) {
        ++passed;
        std::cout << "[PASS] Test_NamedColor_Read_TooManyDeviceCoords\n";
    } else {
        std::cout << "[FAIL] Test_NamedColor_Read_TooManyDeviceCoords\n";
    }

    // Test 4: Root read failure inside loop
    total++;
    if (test_NamedColor_Read_RootReadFailure()) {
        ++passed;
        std::cout << "[PASS] Test_NamedColor_Read_RootReadFailure\n";
    } else {
        std::cout << "[FAIL] Test_NamedColor_Read_RootReadFailure\n";
    }

    std::cout << "Tests passed: " << passed << " / " << total << "\n";
    return (passed == total) ? 0 : 1;
}