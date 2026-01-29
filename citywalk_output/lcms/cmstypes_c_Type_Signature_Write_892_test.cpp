// C++11 test suite for the focal method Type_Signature_Write in cmstypes.c
// This test does not use GTest and relies on a lightweight, self-contained harness.

// Purpose:
// - Validate that Type_Signature_Write correctly writes a 32-bit signature to the
//   provided cmsIOHANDLER using the internal _cmsWriteUInt32Number helper.
// - Cover both success and failure paths of the underlying write operation.
// - Do not rely on private/private-access semantics of the tested class beyond the
//   required public-like interface (as per the given focal method).

#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <cstdint>


// Include the LittleCMS internal headers to obtain necessary type definitions.
// The test environment is assumed to provide these headers and compilation against the library.

// Global test harness state (kept minimal to avoid per-test boilerplate).
static std::vector<uint8_t> g_write_buf; // Captured bytes written by the mock IO
static bool g_failNextWrite = false;     // Control whether the next write should fail

// Lightweight mock for the cms IO "Write" callback.
// This function signature matches the expected CMS prototype for a Write callback:
// cmsBool (*Write)(struct _cmsIOHANDLER*, cmsUInt32Number, const void*)
// We intentionally do not rely on the io context and instead push data into a
// global buffer so tests can inspect what would have been written.
extern "C" cmsBool Mock_Write(struct _cmsIOHANDLER* io, cmsUInt32Number Size, const void* Buffer)
{
    // Simulate a write failure when requested (to cover the false-branch).
    if (g_failNextWrite) {
        return 0; // cmsBool false
    }

    const uint8_t* data = static_cast<const uint8_t*>(Buffer);
    g_write_buf.insert(g_write_buf.end(), data, data + static_cast<size_t>(Size));
    return 1; // cmsBool true
}

// Minimal assertion utilities to mimic GTest-like checks without pulling in a test framework.
static int g_failures = 0;
#define EXPECT_TRUE(expr) do { if(!(expr)) { ++g_failures; std::cerr << "EXPECT_TRUE failed: " #expr " at " __FILE__ ":" << __LINE__ << "\n"; } } while(0)
#define EXPECT_EQ(a, b) do { if((a) != (b)) { ++g_failures; std::cerr << "EXPECT_EQ failed: " #a " != " #b " (" << (a) << " vs " << (b) << ") at " __FILE__ ":" << __LINE__ << "\n"; } } while(0)
#define EXPECT_FALSE(expr) EXPECT_TRUE(!(expr))

// Forward declaration of the focal function to ensure linkage with the library object.
// We declare the signature matching the library's C declaration.
extern "C" cmsBool Type_Signature_Write(struct _cms_typehandler_struct* self,
                                       cmsIOHANDLER* io,
                                       void* Ptr,
                                       cmsUInt32Number nItems);

// Test 1: Verify that a valid signature gets written in little-endian order and the
// function returns success.
void Test_Type_Signature_Write_Success()
{
    // Prepare a mock CMS IO handler with the mock write function.
    cmsIOHANDLER io;
    // Initialize the function pointers/members as needed by the library's struct.
    // We rely on the common field name 'Write' used by many cmsIOHANDLER definitions.
    // Some builds may require explicit initialization of all fields; keeping minimal here.
    io.Write = Mock_Write; // Route writes to our mock function
    io.Context = nullptr;   // Not used by Mock_Write

    // Prepare a signature value. Use a recognizable four-character code: 'abcd'
    // In ASCII hex: 'a' = 0x61, 'b' = 0x62, 'c' = 0x63, 'd' = 0x64
    // Depending on endianness, little-endian storage will yield bytes: 0x64, 0x63, 0x62, 0x61
    cmsSignature sig = ((cmsSignature)0x61 << 0)  | ((cmsSignature)0x62 << 8) |
                       ((cmsSignature)0x63 << 16) | ((cmsSignature)0x64 << 24);

    // Clear captured writes
    g_write_buf.clear();
    g_failNextWrite = false;

    // Call the focal method. Self is unused by the implementation; pass nullptr.
    cmsBool result = Type_Signature_Write(nullptr, &io, &sig, 1);

    // Assertions
    EXPECT_TRUE(result == 1);
    EXPECT_EQ(g_write_buf.size(), 4u); // 4 bytes written for the 32-bit signature

    // Validate endianness: expect little-endian [0x64, 0x63, 0x62, 0x61]
    const uint8_t expected[4] = { 0x64, 0x63, 0x62, 0x61 };
    EXPECT_TRUE(std::memcmp(g_write_buf.data(), expected, 4) == 0);
}

// Test 2: Verify that the false path is handled when the underlying write fails.
// We simulate a failure by instructing the mock to return false on the next write.
void Test_Type_Signature_Write_Failure()
{
    cmsIOHANDLER io;
    io.Write = Mock_Write;
    io.Context = nullptr;

    cmsSignature sig = ((cmsSignature)0x61 << 0)  | ((cmsSignature)0x62 << 8) |
                       ((cmsSignature)0x63 << 16) | ((cmsSignature)0x64 << 24);

    g_write_buf.clear();
    g_failNextWrite = true; // Force the next write to fail

    cmsBool result = Type_Signature_Write(nullptr, &io, &sig, 1);

    // Assertions
    EXPECT_TRUE(result == 0);
    EXPECT_TRUE(g_write_buf.empty()); // No data should be written on failure
}

// Entry point: run tests and report summary.
int main()
{
    Test_Type_Signature_Write_Success();
    Test_Type_Signature_Write_Failure();

    if (g_failures > 0) {
        std::cerr << "Type_Signature_Write test failed with " << g_failures << " failure(s).\n";
        return 1;
    } else {
        std::cout << "All Type_Signature_Write tests passed.\n";
        return 0;
    }
}