/*
Unit test suite for the focal method:
    cmsBool  ReadCountAndString(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsMLU* mlu, cmsUInt32Number* SizeOfTag, const char* Section)

Purpose:
- Validate behavior across success and failure branches.
- Exercise size checks, read operations, memory allocation, and tag size accounting.
- Not using Google Test; provide a lightweight test harness with non-terminating expectations.

Notes:
- This test suite uses a lightweight in-process CMS IO mock to simulate reads from an in-memory buffer.
- We do not rely on inspecting the internal cmsMLU contents (to keep tests API-agnostic); we only verify return values and SizeOfTag adjustments where feasible.
- All tests are written in C++11, but exercise C API from the LittleCMS-like library this project provides.

Assumptions:
- The project exposes cmsIOHANDLER and related types through lcms headers.
- The CMS internal Read/Write function signatures align with the typical lcms2 interface:
  - cmsUInt32Number Read(struct _cms_iohandler_struct* io, void* Buffer, cmsUInt32Number Size, cmsUInt32Number Count)
- The field name ContextID exists within cmsIOHANDLER so we can attach a small memory buffer context to the IO object for the Read function.
- The function _cmsReadUInt32Number(io, &Count) uses io->Read(io, Buffer, 4, 1) semantics (i.e., reads 4 bytes for a cmsUInt32Number). Our mock implements the corresponding behavior.

Build notes:
- This code includes cms headers from the project. It uses a small in-memory IO mock to reproduce the Read behavior required by ReadCountAndString.
- The tests are driven from main() and use lightweight EXPECT-like macros (non-terminating) to accumulate failures while continuing test execution.

Code begins here:
*/

#include <cstring>
#include <vector>
#include <iostream>
#include <climits>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include the project headers (public API)

// Define simple non-terminating test helpers (EXPECT-like)
static int g_test_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << __FUNCTION__ << ": " << (msg) << " (expected true)" << std::endl; \
        ++g_test_failures; \
    } else { \
        std::cout << "[PASS] " << __FUNCTION__ << ": " << (msg) << std::endl; \
    } \
} while (0)

#define EXPECT_FALSE(cond, msg) do { \
    if (cond) { \
        std::cerr << "[FAIL] " << __FUNCTION__ << ": " << (msg) << " (expected false)" << std::endl; \
        ++g_test_failures; \
    } else { \
        std::cout << "[PASS] " << __FUNCTION__ << ": " << (msg) << std::endl; \
    } \
} while (0)

#define EXPECT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        std::cerr << "[FAIL] " << __FUNCTION__ << ": " << (msg) \
                  << " (expected " << (a) << " == " << (b) << ")" << std::endl; \
        ++g_test_failures; \
    } else { \
        std::cout << "[PASS] " << __FUNCTION__ << ": " << (msg) \
                  << " (value " << (a) << ")" << std::endl; \
    } \
} while (0)

// Memory-backed IO mock to simulate cmsIOHANDLER operations
// We attach a small memory buffer as the "device" to the io handler via ContextID.
extern "C" {

// Lightweight context for the in-memory Read implementation
struct MemIoContext {
    const unsigned char* data;
    size_t size;
    size_t pos;
};

// Forward declaration of the Read function to be assigned to cmsIOHANDLER.Read
cmsUInt32Number MockIoRead(struct _cms_iohandler_struct* io, void* Buffer, cmsUInt32Number Size, cmsUInt32Number Count);

} // extern "C"

// Global shim to allow linking with _cmsReadUInt32Number(io, &Count) to fetch data
// We implement the actual Read logic in MockIoRead below.

static cmsUInt32Number MockIoRead(struct _cms_iohandler_struct* io, void* Buffer, cmsUInt32Number Size, cmsUInt32Number Count)
{
    // Cast to our context
    MemIoContext* ctx = reinterpret_cast<MemIoContext*>(io->ContextID);
    if (!ctx) return 0;

    // Total bytes the caller wants to read
    size_t bytesRequested = static_cast<size_t>(Size) * static_cast<size_t>(Count);
    // Ensure we only read if the full requested amount is available
    if (ctx->pos + bytesRequested > ctx->size) {
        // Short read: do not modify Buffer and indicate failure to read
        return 0;
    }

    // Copy bytes and advance position
    if (Buffer != nullptr && bytesRequested > 0) {
        std::memcpy(Buffer, ctx->data + ctx->pos, bytesRequested);
    }
    ctx->pos += bytesRequested;
    return Count;
}

// Helper to build a cmsIOHANDLER with our mock read
static void InitMemoryIo(cmsIOHANDLER& io, MemIoContext& ctx)
{
    // Zero init
    std::memset(&io, 0, sizeof(io));

    // Attach our mock read function
    io.Read = &MockIoRead;
    io.ContextID = &ctx;
}

// Step 1: Candidate Keywords (from ReadCountAndString focal method dependencies)
// - SizeOfTag (cmsUInt32Number pointer) and tag boundaries
// - _cmsReadUInt32Number (reads a Count from io)
// - io->Read (to fetch Count bytes for the string)
// - _cmsMalloc / _cmsFree (memory management for the temporary buffer)
// - cmsMLUsetASCII (store parsed string in MLU under Section)
// - Copy and null-terminate the string (Text[Count] = 0)
// - Update *SizeOfTag by Count + sizeof(cmsUInt32Number)
// - Section (string key for MLU entry)

// Step 2: Test Suite for ReadCountAndString
static void test_ReadCountAndString_Success()
{
    // Payload: Count = 3 (0x03 0x00 0x00 0x00) followed by 3 bytes text: 'A','B','C'
    std::vector<unsigned char> payload = {
        0x03, 0x00, 0x00, 0x00, // Count = 3
        'A', 'B', 'C'            // Text
    };

    MemIoContext ctx{ payload.data(), payload.size(), 0 };
    cmsIOHANDLER io;
    InitMemoryIo(io, ctx);

    // Self object with a valid ContextID (not used meaningfully here beyond _cmsMalloc)
    struct _cms_typehandler_struct self;
    self.ContextID = nullptr; // in practice this can be 0; _cmsMalloc expects a valid context in real usage

    cmsMLU* mlu = cmsMLUalloc(nullptr); // allocate a minimal MLU object
    cmsUInt32Number SizeOfTag = 8;       // ensure enough space for Count + SizeOfTag
    const char* Section = "Sec1";

    cmsBool res = ReadCountAndString(&self, &io, mlu, &SizeOfTag, Section);

    // Assertions
    EXPECT_TRUE(res == TRUE, "ReadCountAndString should succeed for valid payload");
    EXPECT_TRUE(SizeOfTag == 1, "SizeOfTag should be decremented by Count + sizeof(Count)");
    // We also expect that the IO consumed Count + 4 bytes
    EXPECT_EQ(ctx.pos, 7u, "IO position advanced by Count + sizeof(Count)");
    cmsMLUfree(mlu);
}

static void test_ReadCountAndString_SizeOfTagTooSmall()
{
    // Any payload (even empty) should fail early if SizeOfTag < 4
    std::vector<unsigned char> payload = {
        0x01, 0x00, 0x00, 0x00 // Attempted Count = 1; but SizeOfTag check should short-circuit
    };

    MemIoContext ctx{ payload.data(), payload.size(), 0 };
    cmsIOHANDLER io;
    InitMemoryIo(io, ctx);

    struct _cms_typehandler_struct self;
    self.ContextID = nullptr;

    cmsMLU* mlu = cmsMLUalloc(nullptr);
    cmsUInt32Number SizeOfTag = 3; // too small (less than sizeof(cmsUInt32Number))
    const char* Section = "SecX";

    cmsBool res = ReadCountAndString(&self, &io, mlu, &SizeOfTag, Section);

    EXPECT_FALSE(res != TRUE, "ReadCountAndString should fail when SizeOfTag < sizeof(cmsUInt32Number)");
    EXPECT_EQ(SizeOfTag, 3u, "SizeOfTag should remain unchanged on early failure");
    cmsMLUfree(mlu);
}

static void test_ReadCountAndString_UInt32ReadFailure()
{
    // Prepare payload that is insufficient to complete a 4-byte Count read (only 2 bytes)
    std::vector<unsigned char> payload = {
        0x01, 0x02 // incomplete 4-byte Count
    };

    MemIoContext ctx{ payload.data(), payload.size(), 0 };
    cmsIOHANDLER io;
    InitMemoryIo(io, ctx);

    struct _cms_typehandler_struct self;
    self.ContextID = nullptr;

    cmsMLU* mlu = cmsMLUalloc(nullptr);
    cmsUInt32Number SizeOfTag = 16;
    const char* Section = "SecFailRead";

    cmsBool res = ReadCountAndString(&self, &io, mlu, &SizeOfTag, Section);

    EXPECT_FALSE(res != TRUE, "ReadCountAndString should fail if Count read cannot complete (IO short)");
    cmsMLUfree(mlu);
}

static void test_ReadCountAndString_CountTooLarge()
{
    // Count = 0xFFFFFFFF (4294967295); ensure Count > UINT_MAX - 4 triggers failure
    std::vector<unsigned char> payload = {
        0xFF, 0xFF, 0xFF, 0xFF // Count
        // No string data needed; should fail before attempting to read string
    };

    MemIoContext ctx{ payload.data(), payload.size(), 0 };
    cmsIOHANDLER io;
    InitMemoryIo(io, ctx);

    struct _cms_typehandler_struct self;
    self.ContextID = nullptr;

    cmsMLU* mlu = cmsMLUalloc(nullptr);
    cmsUInt32Number SizeOfTag = 32;
    const char* Section = "SecLarge";

    cmsBool res = ReadCountAndString(&self, &io, mlu, &SizeOfTag, Section);

    EXPECT_FALSE(res != TRUE, "ReadCountAndString should fail for Count > UINT_MAX - sizeof(Count)");
    cmsMLUfree(mlu);
}

static void test_ReadCountAndString_ReadBytesMismatch()
{
    // Count = 5, but only 3 bytes of text data provided
    std::vector<unsigned char> payload = {
        0x05, 0x00, 0x00, 0x00, // Count = 5
        'X', 'Y', 'Z'           // Only 3 bytes available for the string
    };

    MemIoContext ctx{ payload.data(), payload.size(), 0 };
    cmsIOHANDLER io;
    InitMemoryIo(io, ctx);

    struct _cms_typehandler_struct self;
    self.ContextID = nullptr;

    cmsMLU* mlu = cmsMLUalloc(nullptr);
    cmsUInt32Number SizeOfTag = 16;
    const char* Section = "SecMismatch";

    cmsBool res = ReadCountAndString(&self, &io, mlu, &SizeOfTag, Section);

    EXPECT_FALSE(res != TRUE, "ReadCountAndString should fail when Read for string bytes returns fewer bytes than Count");
    cmsMLUfree(mlu);
}

int main()
{
    std::cout << "Running ReadCountAndString test suite...\n";

    test_ReadCountAndString_Success();
    test_ReadCountAndString_SizeOfTagTooSmall();
    test_ReadCountAndString_UInt32ReadFailure();
    test_ReadCountAndString_CountTooLarge();
    test_ReadCountAndString_ReadBytesMismatch();

    if (g_test_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_test_failures << " test(s) failed.\n";
        return 1;
    }
}