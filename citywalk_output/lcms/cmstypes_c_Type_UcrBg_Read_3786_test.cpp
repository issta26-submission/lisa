#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <cassert>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include Little CMS internal/public headers.
// We rely on the library to provide the function Type_UcrBg_Read in C code.
// The test harness is built in C++11 and uses a minimal, non-terminating assertion style.

extern "C" {
}

// Forward declare the focal function and its related cleanup function to ensure linkage.
extern "C" void *Type_UcrBg_Read(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsUInt32Number* nItems, cmsUInt32Number SizeOfTag);
extern "C" void Type_UcrBg_Free(struct _cms_typehandler_struct* self, void *Ptr);

// Global test IO buffer management for a simple mock Read function
static const unsigned char* g_ioBuffer = nullptr;
static size_t g_ioLen = 0;
static size_t g_ioPos = 0;

// Mock Read function to feed a prepared buffer to the focal function.
// Signature matches cmsIOHANDLER.Read(io, Buffer, Size, Count)
extern "C" cmsUInt32Number MockRead(struct _cmsIOHANDLER* io, void* Buffer, cmsUInt32Number Size, cmsUInt32Number Count)
{
    // If Count or Size is zero, nothing to read
    if (Size == 0 || Count == 0) {
        return 0;
    }
    size_t totalBytes = (size_t)Size * (size_t)Count;
    if (g_ioBuffer == nullptr || g_ioLen == 0) {
        return 0;
    }
    // Calculate how many bytes we can still read
    size_t remaining = (g_ioLen > g_ioPos) ? (g_ioLen - g_ioPos) : 0;
    size_t toRead = (totalBytes <= remaining) ? totalBytes : remaining;

    if (toRead > 0) {
        std::memcpy(Buffer, g_ioBuffer + g_ioPos, toRead);
        g_ioPos += toRead;
    }
    // Return how many elements were read
    size_t readCount = (toRead / (size_t)Size);
    return (cmsUInt32Number)readCount;
}

// Simple testing harness helpers
static bool g_any_failure = false;

static void ExpectTrue(bool cond, const std::string& note)
{
    if (!cond) {
        std::cerr << "EXPECT_TRUE FAILED: " << note << "\n";
        g_any_failure = true;
    }
}

// Helper to print success/failure summary
static void PrintSummary(const std::string& title, bool ok)
{
    std::cout << title << ": " << (ok ? "PASS" : "FAIL") << std::endl;
}

// Test 1: Basic positive path (reads two UCR points and one BG, zero-length description)
static bool test_Type_UcrBg_Read_basic()
{
    // Prepare a minimal, consistent data stream:
    // CountUcr = 2 (0x02 00 00 00)
    // Ucr Table16: 0x0010, 0x0020 (16, 32)
    // CountBg = 1 (0x01 00 00 00)
    // Bg  Table16: 0x0005 (5)
    const unsigned char data1[] = {
        0x02,0x00,0x00,0x00,        // CountUcr = 2
        0x10,0x00,0x20,0x00,        // Ucr Table16: 16, 32
        0x01,0x00,0x00,0x00,        // CountBg = 1
        0x05,0x00                   // Bg Table16: 5
        // No description (SignedSizeOfTag will be 0)
    };

    // Context and handler setup
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    struct _cms_typehandler_struct handler;
    // Initialize minimal fields required by the focal method
    handler.ContextID = context;

    cmsIOHANDLER io;
    io.Read = MockRead;

    // Point the global IO buffer
    g_ioBuffer = data1;
    g_ioLen = sizeof(data1);
    g_ioPos = 0;

    cmsUInt32Number nItems = 0;
    void *res = Type_UcrBg_Read(&handler, &io, &nItems, 14); // SizeOfTag = 14 (4 + 4 + 4 + 2)

    bool ok = true;
    if (res == NULL) {
        std::cerr << "Failed: Type_UcrBg_Read_basic returned NULL\n";
        ok = false;
    } else {
        cmsUcrBg* n = (cmsUcrBg*)res;
        // Check that two Ucr values were read
        if (n->Ucr == NULL || n->Ucr->Table16 == nullptr) {
            std::cerr << "Failed: Ucr or Ucr->Table16 is NULL\n";
            ok = false;
        } else {
            if (n->Ucr->Table16[0] != 16 || n->Ucr->Table16[1] != 32) {
                std::cerr << "Failed: Ucr Table16 values incorrect (expected 16,32, got "
                          << n->Ucr->Table16[0] << "," << n->Ucr->Table16[1] << ")\n";
                ok = false;
            }
        }
        // Check BG
        if (n->Bg == NULL || n->Bg->Table16 == nullptr) {
            std::cerr << "Failed: Bg or Bg->Table16 is NULL\n";
            ok = false;
        } else {
            if (n->Bg->Table16[0] != 5) {
                std::cerr << "Failed: Bg Table16 value incorrect (expected 5, got "
                          << n->Bg->Table16[0] << ")\n";
                ok = false;
            }
        }
        // Desc can be NULL or non-NULL depending on allocator; we just ensure no crash
        if (n->Desc == nullptr) {
            // It's acceptable for no description in this test case, but if present, it's okay as well
        }

        // Clean up
        Type_UcrBg_Free(&handler, res);
    }

    cmsDestroyContext(context);
    return ok;
}

// Test 2: Positive path with a non-empty ASCII description (Desc length = 4)
static bool test_Type_UcrBg_Read_with_desc()
{
    // Data stream including a 4-byte description "ABCD"
    // CountUcr = 2
    // Ucr Table16: 17, 34
    // CountBg = 1
    // Bg Table16: 7
    // Description length = 4, followed by "ABCD" (ASCII)
    const unsigned char data2[] = {
        0x02,0x00,0x00,0x00,        // CountUcr = 2
        0x11,0x00,0x22,0x00,        // Ucr Table16: 17, 34
        0x01,0x00,0x00,0x00,        // CountBg = 1
        0x07,0x00,                   // Bg Table16: 7
        0x41,0x42,0x43,0x44            // Description: "ABCD"
    };

    cmsContext context = cmsCreateContext(nullptr, nullptr);
    struct _cms_typehandler_struct handler;
    handler.ContextID = context;

    cmsIOHANDLER io;
    io.Read = MockRead;

    g_ioBuffer = data2;
    g_ioLen = sizeof(data2);
    g_ioPos = 0;

    cmsUInt32Number nItems = 0;
    void *res = Type_UcrBg_Read(&handler, &io, &nItems, 18); // SizeOfTag includes extra Desc (14 + 4)

    bool ok = true;
    if (res == NULL) {
        std::cerr << "Failed: Type_UcrBg_Read_with_desc returned NULL\n";
        ok = false;
    } else {
        cmsUcrBg* n = (cmsUcrBg*)res;
        if (n->Ucr == NULL || n->Ucr->Table16 == nullptr) {
            std::cerr << "Failed: Ucr or Ucr->Table16 is NULL\n";
            ok = false;
        } else {
            if (n->Ucr->Table16[0] != 17 || n->Ucr->Table16[1] != 34) {
                std::cerr << "Failed: Ucr Table16 values incorrect (expected 17,34, got "
                          << n->Ucr->Table16[0] << "," << n->Ucr->Table16[1] << ")\n";
                ok = false;
            }
        }
        if (n->Bg == NULL || n->Bg->Table16 == nullptr) {
            std::cerr << "Failed: Bg or Bg->Table16 is NULL\n";
            ok = false;
        } else {
            if (n->Bg->Table16[0] != 7) {
                std::cerr << "Failed: Bg Table16 value incorrect (expected 7, got "
                          << n->Bg->Table16[0] << ")\n";
                ok = false;
            }
        }

        // Retrieve the ASCII description to verify it's "ABCD"
        // Expect cmsMLUgetASCII to return the exact string
        char descBuf[64] = {0};
        cmsBool hasDesc =
            cmsMLUgetASCII(n->Desc, cmsNoLanguage, cmsNoCountry, descBuf);
        if (!hasDesc) {
            std::cerr << "Failed: cmsMLUgetASCII could not fetch description\n";
            ok = false;
        } else {
            if (std::strncmp(descBuf, "ABCD", 4) != 0) {
                std::cerr << "Failed: Description content mismatch (expected 'ABCD', got '"
                          << descBuf << "')\n";
                ok = false;
            }
        }

        Type_UcrBg_Free(&handler, res);
    }

    cmsDestroyContext(context);
    return ok;
}

// Test 3: Early exit when SizeOfTag is too small (< 4) should return NULL
static bool test_Type_UcrBg_Read_sizeTooSmall()
{
    const unsigned char dummy[] = { 0x00 }; // not used in this path
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    struct _cms_typehandler_struct handler;
    handler.ContextID = context;

    cmsIOHANDLER io;
    io.Read = MockRead;

    g_ioBuffer = dummy;
    g_ioLen = sizeof(dummy);
    g_ioPos = 0;

    cmsUInt32Number nItems = 0;
    void* res = Type_UcrBg_Read(&handler, &io, &nItems, 3); // SizeOfTag < 4 should fail early

    bool ok = (res == NULL);
    if (!ok) {
        std::cerr << "Failed: Type_UcrBg_Read_sizeTooSmall should return NULL\n";
    }

    cmsDestroyContext(context);
    return ok;
}

int main()
{
    bool all_pass = true;

    bool t1 = test_Type_UcrBg_Read_basic();
    PrintSummary("Test 1 - Basic read (no description)", t1);
    all_pass = all_pass && t1;

    bool t2 = test_Type_UcrBg_Read_with_desc();
    PrintSummary("Test 2 - Read with description", t2);
    all_pass = all_pass && t2;

    bool t3 = test_Type_UcrBg_Read_sizeTooSmall();
    PrintSummary("Test 3 - SizeOfTag too small", t3);
    all_pass = all_pass && t3;

    if (all_pass) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "ONE OR MORE TESTS FAILED" << std::endl;
        return 1;
    }
}