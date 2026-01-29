// Unit tests for the focal method Type_ColorantTable_Write
// This test suite is implemented in C++11 without GoogleTest.
// It uses a small, self-contained mocking framework to exercise the behavior
// of Type_ColorantTable_Write and its interactions with the cmsIOHANDLER I/O path.
// The tests are designed to compile against a build where the real cmstypes.c
// (containing Type_ColorantTable_Write) is available and linked with these mocks.

#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <cassert>
#include <lcms2_internal.h>
#include <cstdint>


// Lightweight test harness utilities
static int g_test_failures = 0;
static void report_failure(const std::string& msg) {
    std::cerr << "FAIL: " << msg << std::endl;
    ++g_test_failures;
}
static void report_success(const std::string& msg) {
    std::cout << "PASS: " << msg << std::endl;
}
#define CHECK(cond, msg) do { if(!(cond)) { report_failure(msg); } else { /* verbose success can be quiet */ } } while(0)

// Domain-like typedefs and structures to mirror needed parts of the CMS internals
extern "C" {

// Basic CMS-like boolean and types (minimal subset for testing)
typedef int cmsBool;
typedef unsigned int cmsUInt32Number;
typedef unsigned short cmsUInt16Number;

#define TRUE 1
#define FALSE 0

// Forward-declare the IO handler used by Type_ColorantTable_Write
struct _cms_typehandler_struct; // opaque to the test harness
typedef struct cmsIOHANDLER cmsIOHANDLER;

// Simple IO envelope used to capture writes performed by Type_ColorantTable_Write
// The Write callback writes 'size' bytes from 'data' into an internal buffer.
struct cmsIOHANDLER {
    cmsBool (*Write)(cmsIOHANDLER* io, cmsUInt32Number size, const void* data);
    void* context; // user data (we'll store a pointer to our buffer)
};

// Named-color list structure used by Type_ColorantTable_Write
typedef struct cmsNAMEDCOLORLIST cmsNAMEDCOLORLIST;

// Small in-test shim for named colors to drive Type_ColorantTable_Write
struct cmsNAMEDCOLORLIST {
    cmsUInt32Number Count;
    const char** Names;
    cmsUInt16Number PCS[256][3]; // PCS triplets per color
};

// External declaration of the focal function under test.
// We declare it with C linkage to match the original object file's symbol.
extern "C" cmsBool Type_ColorantTable_Write(struct _cms_typehandler_struct* self,
                                           cmsIOHANDLER* io,
                                           void* Ptr,
                                           cmsUInt32Number nItems);

} // extern "C"

// Global test-time knobs to control mock behavior
static int g_fail_uint32   = 0;  // when set, _cmsWriteUInt32Number fails
static int g_fail_root_only  = 0;  // when set, root writes (size == 32) fail
static int g_fail_named_color_info_index = -1; // index at which cmsNamedColorInfo should fail (simulate error)
static int g_next_uint32_fail    = 0; // one-shot failure toggle for _cmsWriteUInt32Number

// Helpers: a minimal in-test buffer to capture IO writes
struct Buffer {
    std::vector<uint8_t> data;
    void append(const void* src, size_t n) {
        const uint8_t* p = static_cast<const uint8_t*>(src);
        data.insert(data.end(), p, p + n);
    }
    const uint8_t* raw() const { return data.data(); }
    size_t size() const { return data.size(); }
    void clear() { data.clear(); }
};

// Simple IO Write callback used by the focal function
static cmsBool TestIO_Write(cmsIOHANDLER* io, cmsUInt32Number size, const void* data) {
    if (!io || !io->context) return FALSE;
    Buffer* buf = static_cast<Buffer*>(io->context);
    if (g_fail_root_only && size == 32) {
        // Simulate a root write failure
        return FALSE;
    }
    buf->append(data, static_cast<size_t>(size));
    return TRUE;
}

// Minimal Type struct as a placeholder for the focal function's self parameter
struct _cms_typehandler_struct {
    // Intentionally empty for testing
};

// Mock implementations of dependencies expected by the focal function

// The actual CMS uses cmsNamedColorCount to know how many colors are in a NamedColorList
static cmsUInt32Number cmsNamedColorCount(struct cmsNAMEDCOLORLIST* NamedColorList) {
    if (!NamedColorList) return 0;
    return NamedColorList->Count;
}

// Named color info: fills in color name (root) and PCS values.
// Only the first 32 chars of the color name are used by the focal function.
static cmsBool cmsNamedColorInfo(struct cmsNAMEDCOLORLIST* NamedColorList,
                                 cmsUInt32Number i,
                                 char* root,
                                 void* a,
                                 void* b,
                                 cmsUInt16Number PCS[3],
                                 void* d) {
    if (!NamedColorList) return FALSE;
    if (i >= NamedColorList->Count) return FALSE;
    // Copy up to 32 chars into root and terminate
    const char* name = NamedColorList->Names[i];
    std::size_t toCopy = name ? std::strlen(name) : 0;
    if (toCopy > 32) toCopy = 32;
    if (toCopy > 0) std::memcpy(root, name, toCopy);
    if (toCopy < 32) {
        // pad with zeros
        std::memset(root + toCopy, 0, 32 - toCopy);
    }
    root[32] = '\0';
    if (PCS) {
        PCS[0] = NamedColorList->PCS[i][0];
        PCS[1] = NamedColorList->PCS[i][1];
        PCS[2] = NamedColorList->PCS[i][2];
    }
    // Induce a test-time failure if configured
    if ((int)i == g_fail_named_color_info_index) {
        return FALSE;
    }
    return TRUE;
}

// The underlying CMS writes integers to the IO via these helper functions.
// We mock them to route through our buffer.
static cmsBool _cmsWriteUInt32Number(cmsIOHANDLER* io, cmsUInt32Number Number) {
    if (g_next_uint32_fail) { // one-shot failure
        g_next_uint32_fail = 0;
        return FALSE;
    }
    if (g_fail_uint32) return FALSE;
    uint8_t payload[4];
    payload[0] = static_cast<uint8_t>(Number & 0xFF);
    payload[1] = static_cast<uint8_t>((Number >> 8) & 0xFF);
    payload[2] = static_cast<uint8_t>((Number >> 16) & 0xFF);
    payload[3] = static_cast<uint8_t>((Number >> 24) & 0xFF);
    if (!io || !io->Write) return FALSE;
    return io->Write(io, 4, payload) ? TRUE : FALSE;
}

// Write 3x16-bit values (3*2 bytes) in little-endian format
static cmsBool _cmsWriteUInt16Array(cmsIOHANDLER* io, cmsUInt32Number Count, const cmsUInt16Number* Array) {
    if (Count == 0) return TRUE;
    // If test disables 16-bit writes, honor it as a potential fail path
    if (g_fail_uint32) { // reuse simple flag to trigger failure path
        return FALSE;
    }
    uint8_t payload[6]; // for Count==3 in our tests
    for (cmsUInt32Number i = 0; i < Count; ++i) {
        cmsUInt16Number v = Array[i];
        payload[i*2 + 0] = static_cast<uint8_t>(v & 0xFF);
        payload[i*2 + 1] = static_cast<uint8_t>((v >> 8) & 0xFF);
    }
    if (!io || !io->Write) return FALSE;
    return io->Write(io, Count * 2, payload) ? TRUE : FALSE;
}

// Public declarations for the test to link against the focal method
extern "C" cmsBool Type_ColorantTable_Write(struct _cms_typehandler_struct* self,
                                           cmsIOHANDLER* io,
                                           void* Ptr,
                                           cmsUInt32Number nItems);

// Test helpers: build NamedColorList for tests
static cmsNAMEDCOLORLIST create_named_color_list(const std::vector<std::string>& names,
                                                const std::vector<std::array<cmsUInt16Number, 3>>& pcs) {
    cmsNAMEDCOLORLIST list;
    list.Count = static_cast<cmsUInt32Number>(names.size());
    list.Names = new const char*[names.size()];
    for (size_t i = 0; i < names.size(); ++i) {
        // allocate c-string storage
        list.Names[i] = new char[names[i].size() + 1];
        std::strcpy(const_cast<char*>(list.Names[i]), names[i].c_str());
        // ensure a stable lifetime for our test objects
    }
    // PCS
    for (size_t i = 0; i < pcs.size() && i < 256; ++i) {
        list.PCS[i][0] = pcs[i][0];
        list.PCS[i][1] = pcs[i][1];
        list.PCS[i][2] = pcs[i][2];
    }
    // Note: This simple allocator is not freed here to keep test concise;
    // test harness will clean up at end if needed.
    return list;
}

// Cleanup for create_named_color_list
static void free_named_color_list(cmsNAMEDCOLORLIST& list) {
    if (!list.Names) return;
    for (size_t i = 0; i < static_cast<size_t>(list.Count); ++i) {
        delete[] const_cast<char*>(list.Names[i]);
    }
    delete[] list.Names;
}

// Test 1: Success path with two colors; verify buffer contents
static void test_ColorantTableWrite_Success() {
    Buffer buf;
    cmsIOHANDLER io;
    io.Write = TestIO_Write;
    io.context = &buf;

    // Prepare NamedColorList with two colors
    const char* n0 = "BlueColor";
    const char* n1 = "RedColor123";

    const char* names_arr[2] = { n0, n1 };
    cmsNAMEDCOLORLIST NamedColorList;
    NamedColorList.Count = 2;
    NamedColorList.Names = names_arr;
    NamedColorList.PCS[0][0] = 10; NamedColorList.PCS[0][1] = 20; NamedColorList.PCS[0][2] = 30;
    NamedColorList.PCS[1][0] = 100; NamedColorList.PCS[1][1] = 200; NamedColorList.PCS[1][2] = 300;

    // Clear global controls
    g_fail_uint32 = 0;
    g_fail_root_only = 0;
    g_fail_named_color_info_index = -1;
    g_next_uint32_fail = 0;

    // Call focal method
    cmsBool res = Type_ColorantTable_Write(nullptr, &io, &NamedColorList, 2);

    // Assertions
    CHECK(res == TRUE, "ColorantTable_Write should succeed for two colors");
    // Expected size: 4 bytes for color count + 2 * (32 bytes root + 6 bytes PCS)
    const size_t expected_size = 4 + 2 * (32 + 6);
    CHECK(buf.size() == expected_size, "Buffer size should equal 4 + 2*(32+6) for two colors");

    // Validate content: first 4 bytes are little-endian 2
    const uint8_t* d = buf.raw();
    uint32_t count = d[0] | (d[1] << 8) | (d[2] << 16) | (d[3] << 24);
    CHECK(count == 2, "First 4 bytes should encode 2 colors");

    // Validate per-color blocks
    const uint8_t* cursor = d + 4;
    for (int i = 0; i < 2; ++i) {
        // Root: 32 bytes (we only fill 32 chars; rest are zeros)
        char expectedRoot[33];
        std::memset(expectedRoot, 0, 33);
        const char* name = (i == 0) ? n0 : n1;
        // copy up to 32 chars
        std::strncpy(expectedRoot, name, 32);
        expectedRoot[32] = 0;

        // Compare 32 bytes
        if (std::memcmp(cursor, expectedRoot, 32) != 0) {
            report_failure("Root string for color " + std::to_string(i) + " does not match expected content");
            // still continue checks
        } else {
            report_success("Root string for color " + std::to_string(i) + " matches expected content");
        }
        cursor += 32;

        // PCS: 3 x 16-bit values, little-endian
        cmsUInt16Number expectedPCS[3] = { NamedColorList.PCS[i][0], NamedColorList.PCS[i][1], NamedColorList.PCS[i][2] };
        for (int p = 0; p < 3; ++p) {
            uint16_t val = cursor[p*2] | (cursor[p*2 + 1] << 8);
            if (val != expectedPCS[p]) {
                report_failure("PCS value for color " + std::to_string(i) +
                               " component " + std::to_string(p) + " mismatch");
            }
        }
        cursor += 6;
    }

    // Cleanup is not strictly required here since test program ends soon, but we do it for good hygiene
    free_named_color_list(NamedColorList);
}

// Test 2: NamedColorInfo reports failure for a particular index
static void test_ColorantTableWrite_NamedColorInfoFail() {
    Buffer buf;
    cmsIOHANDLER io;
    io.Write = TestIO_Write;
    io.context = &buf;

    const char* n0 = "Gradient";
    const char* n1 = "Vivid";

    const char* names_arr[2] = { n0, n1 };
    cmsNAMEDCOLORLIST NamedColorList;
    NamedColorList.Count = 2;
    NamedColorList.Names = names_arr;
    NamedColorList.PCS[0][0] = 1; NamedColorList.PCS[0][1] = 2; NamedColorList.PCS[0][2] = 3;
    NamedColorList.PCS[1][0] = 4; NamedColorList.PCS[1][1] = 5; NamedColorList.PCS[1][2] = 6;

    // Configure to fail at index 0 for cmsNamedColorInfo
    g_fail_uint32 = 0;
    g_fail_root_only = 0;
    g_fail_named_color_info_index = 0; // will cause cmsNamedColorInfo to return FALSE for i==0
    g_next_uint32_fail = 0;

    cmsBool res = Type_ColorantTable_Write(nullptr, &io, &NamedColorList, 2);

    CHECK(res == FALSE, "ColorantTable_Write should fail when cmsNamedColorInfo returns FALSE (index 0)");
    // No need to inspect buffer on failure
    free_named_color_list(NamedColorList);
}

// Test 3: Failure in _cmsWriteUInt32Number (count write) causes immediate failure
static void test_ColorantTableWrite_WriteUInt32Fail() {
    Buffer buf;
    cmsIOHANDLER io;
    io.Write = TestIO_Write;
    io.context = &buf;

    const char* names_arr[1] = { "OnlyOne" };
    cmsNAMEDCOLORLIST NamedColorList;
    NamedColorList.Count = 1;
    NamedColorList.Names = names_arr;
    NamedColorList.PCS[0][0] = 9; NamedColorList.PCS[0][1] = 0; NamedColorList.PCS[0][2] = 0;

    // Force _cmsWriteUInt32Number to fail on first call (writing count)
    g_fail_uint32 = 1;
    g_next_uint32_fail = 0;

    cmsBool res = Type_ColorantTable_Write(nullptr, &io, &NamedColorList, 1);

    CHECK(res == FALSE, "ColorantTable_Write should fail when _cmsWriteUInt32Number fails on count write");
    free_named_color_list(NamedColorList);
}

// Test 4: Failure when writing root (size == 32) via io->Write path
static void test_ColorantTableWrite_RootWriteFail() {
    Buffer buf;
    cmsIOHANDLER io;
    io.Write = TestIO_Write;
    io.context = &buf;

    const char* names_arr[2] = { "Alpha", "Beta" };
    cmsNAMEDCOLORLIST NamedColorList;
    NamedColorList.Count = 2;
    NamedColorList.Names = names_arr;
    NamedColorList.PCS[0][0] = 11; NamedColorList.PCS[0][1] = 12; NamedColorList.PCS[0][2] = 13;
    NamedColorList.PCS[1][0] = 21; NamedColorList.PCS[1][1] = 22; NamedColorList.PCS[1][2] = 23;

    g_fail_root_only = 1; // cause root write (size 32) to fail
    g_fail_uint32 = 0;
    g_next_uint32_fail = 0;
    g_fail_named_color_info_index = -1;

    cmsBool res = Type_ColorantTable_Write(nullptr, &io, &NamedColorList, 2);

    CHECK(res == FALSE, "ColorantTable_Write should fail when root write (size 32) fails");
    free_named_color_list(NamedColorList);
}

// Entry point for tests
int main() {
    std::cout << "Starting ColorantTable_Write unit tests..." << std::endl;

    test_ColorantTableWrite_Success();
    test_ColorantTableWrite_NamedColorInfoFail();
    test_ColorantTableWrite_WriteUInt32Fail();
    test_ColorantTableWrite_RootWriteFail();

    if (g_test_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_test_failures << " test(s) failed." << std::endl;
        return 1;
    }
}