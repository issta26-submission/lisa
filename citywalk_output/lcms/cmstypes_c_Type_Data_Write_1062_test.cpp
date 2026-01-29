// Lightweight C++11 unit tests for the focal method: cmsBool Type_Data_Write(...)
#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <cstdint>


// Domain-specific lightweight redefinitions to enable compilation of the focal method
typedef uint32_t cmsUInt32Number;
typedef int cmsBool;
#define TRUE 1
#define FALSE 0
#define cmsUNUSED_PARAMETER(x) (void)(x)

struct _cmsIOHANDLER;
struct _cms_typehandler_struct;

// Lightweight IO handler structure
typedef struct _cmsIOHANDLER {
    cmsBool (*Write)(_cmsIOHANDLER* io, cmsUInt32Number n, const void* buffer);
} cmsIOHANDLER;

// Fallback minimal type handler structure (as used by Type_Data_Write)
typedef struct _cms_typehandler_struct {
    int dummy;
} _cms_typehandler_struct;

// ICC data structure used by Type_Data_Write
typedef struct {
    cmsUInt32Number flag;
    cmsUInt32Number len;
    unsigned char* data;
} cmsICCData;

/* Global test hooks */
static cmsBool g_failNextUint32Write = FALSE; // controls _cmsWriteUInt32Number failure
struct FakeDevice {
    std::vector<unsigned char> written;
} g_fakeDevice;

// Forward declaration of the function under test and its helpers
static cmsBool _cmsWriteUInt32Number(cmsIOHANDLER* io, cmsUInt32Number value);
static cmsBool Type_Data_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems);

// Implementation of the pseudo IO Write callback used by tests
static cmsBool FakeIO_Write(cmsIOHANDLER* io, cmsUInt32Number n, const void* buffer)
{
    (void)io; // not used by test callback
    if (buffer == nullptr) {
        // If a NULL buffer is provided, still simulate a write of zeros
        for (cmsUInt32Number i = 0; i < n; ++i) {
            g_fakeDevice.written.push_back(0);
        }
    } else {
        const unsigned char* b = reinterpret_cast<const unsigned char*>(buffer);
        for (cmsUInt32Number i = 0; i < n; ++i) {
            g_fakeDevice.written.push_back(b[i]);
        }
    }
    return TRUE;
}

// Implementation of helper: write a 32-bit value in little-endian via the IO's Write
static cmsBool _cmsWriteUInt32Number(cmsIOHANDLER* io, cmsUInt32Number value)
{
    unsigned char bytes[4];
    bytes[0] = static_cast<unsigned char>(value & 0xFF);
    bytes[1] = static_cast<unsigned char>((value >> 8) & 0xFF);
    bytes[2] = static_cast<unsigned char>((value >> 16) & 0xFF);
    bytes[3] = static_cast<unsigned char>((value >> 24) & 0xFF);
    if (g_failNextUint32Write) {
        // Simulate a write failure for testing false-path
        return FALSE;
    }
    return io->Write(io, 4, bytes);
}

// Implementation of the focal method under test
static cmsBool Type_Data_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems)
{
{
   cmsICCData* BinData = (cmsICCData*) Ptr;
   if (!_cmsWriteUInt32Number(io, BinData ->flag)) return FALSE;
   return io ->Write(io, BinData ->len, BinData ->data);
   cmsUNUSED_PARAMETER(nItems);
   cmsUNUSED_PARAMETER(self);
}
}

// ===== Test helpers and test cases =====

static bool bytesEquals(const std::vector<unsigned char>& a, const std::vector<unsigned char>& b)
{
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i)
        if (a[i] != b[i]) return false;
    return true;
}

// Test 1: Successful write path (flag write succeeds, data buffer copied)
static bool test_Type_Data_Write_Success()
{
    // Prepare IO and target structures
    cmsIOHANDLER io;
    io.Write = FakeIO_Write;
    g_fakeDevice.written.clear();

    _cms_typehandler_struct dummySelf;
    cmsICCData bin;
    unsigned char payload[] = { 0x10, 0x20, 0x30 };
    bin.flag = 0x00000001; // LE: 01 00 00 00
    bin.len = sizeof(payload);
    bin.data = payload;

    // Execute
    cmsBool result = Type_Data_Write(&dummySelf, &io, &bin, 1);

    // Verify
    std::vector<unsigned char> expected;
    // 4 bytes for flag: 01 00 00 00
    expected.push_back(0x01);
    expected.push_back(0x00);
    expected.push_back(0x00);
    expected.push_back(0x00);
    // payload
    for (size_t i = 0; i < sizeof(payload); ++i) expected.push_back(payload[i]);

    bool ok = (result == TRUE) && bytesEquals(g_fakeDevice.written, expected);
    if (!ok) {
        std::cerr << "[Test 1] Expected result TRUE and exact bytes written, got result=" << result
                  << " bytes_written=" << g_fakeDevice.written.size() << "\n";
    }
    return ok;
}

// Test 2: Failure in _cmsWriteUInt32Number leads to FALSE (no data should be written)
static bool test_Type_Data_Write_UInt32Fail()
{
    // Prepare IO and target structures
    cmsIOHANDLER io;
    io.Write = FakeIO_Write;
    g_fakeDevice.written.clear();

    _cms_typehandler_struct dummySelf;
    cmsICCData bin;
    unsigned char payload[] = { 0xAA, 0xBB, 0xCC, 0xDD };
    bin.flag = 0x12345678;
    bin.len = sizeof(payload);
    bin.data = payload;

    // Enable failure for the 32-bit write
    g_failNextUint32Write = TRUE;

    cmsBool result = Type_Data_Write(&dummySelf, &io, &bin, 1);

    // Cleanup flag for subsequent tests
    g_failNextUint32Write = FALSE;

    // Verify: Should be FALSE and nothing written
    bool ok = (result == FALSE) && (g_fakeDevice.written.empty());
    if (!ok) {
        std::cerr << "[Test 2] Expected FALSE with no bytes written; got result=" << result
                  << " bytes_written=" << g_fakeDevice.written.size() << "\n";
    }
    return ok;
}

// Test 3: Zero-length data should still perform flag write and return success
static bool test_Type_Data_Write_ZeroLen()
{
    cmsIOHANDLER io;
    io.Write = FakeIO_Write;
    g_fakeDevice.written.clear();

    _cms_typehandler_struct dummySelf;
    cmsICCData bin;
    unsigned char* payload = nullptr;
    bin.flag = 0x00000002;
    bin.len = 0;        // zero length
    bin.data = payload;

    cmsBool result = Type_Data_Write(&dummySelf, &io, &bin, 1);

    // Expected: 4-byte flag then zero-length write (no payload)
    std::vector<unsigned char> expected;
    expected.push_back(0x02);
    expected.push_back(0x00);
    expected.push_back(0x00);
    expected.push_back(0x00);
    // No payload bytes

    bool ok = (result == TRUE) && bytesEquals(g_fakeDevice.written, expected);
    if (!ok) {
        std::cerr << "[Test 3] Expected TRUE with flag 0x02 and zero-length data; got result=" << result
                  << " bytes_written=" << g_fakeDevice.written.size() << "\n";
    }
    return ok;
}

// ===== Main test harness =====
int main()
{
    int passed = 0;
    int total = 0;

    std::cout << "Running unit tests for Type_Data_Write...\n";

    if (test_Type_Data_Write_Success()) {
        ++passed;
        std::cout << "Test 1 (Success path) PASSED\n";
    } else {
        std::cout << "Test 1 (Success path) FAILED\n";
    }
    ++total;

    if (test_Type_Data_Write_UInt32Fail()) {
        ++passed;
        std::cout << "Test 2 (UInt32 write failure) PASSED\n";
    } else {
        std::cout << "Test 2 (UInt32 write failure) FAILED\n";
    }
    ++total;

    if (test_Type_Data_Write_ZeroLen()) {
        ++passed;
        std::cout << "Test 3 (Zero-length data) PASSED\n";
    } else {
        std::cout << "Test 3 (Zero-length data) FAILED\n";
    }
    ++total;

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";
    return (passed == total) ? 0 : 1;
}