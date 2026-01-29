// cms_curve_write_test.cpp
// Minimal unit tests for cmsBool Type_Curve_Write (from cms/types cmstypes.c)
// This test is designed to be compiled and linked against the LittleCMS (lcms2) library.
// It uses plain C++11 and does not rely on Google Test. A lightweight EXPECT-like macro
// set is implemented to preserve test flow (non-terminating assertions).
// Notes:
// - We test both branches: single gamma path and multi-entry path.
// - We simulate IO by providing a cmsIOHANDLER with a custom Write callback that records written bytes.
// - We also test negative paths by simulating failures in the Write callback.
// - The tests assume LittleCMS internal types (cmsToneCurve, cmsIOHANDLER, etc.) from <lcms2.h>.
// - If your environment uses different header paths, adjust the #include accordingly.

#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


extern "C" {
}

// Lightweight test assertion helpers (non-terminating)
#define EXPECT_TRUE(x) do { if(!(x)) { std::cerr << "EXPECT_TRUE failed: " #x " at line " << __LINE__ << "\n"; } } while(0)
#define EXPECT_FALSE(x) do { if((x)) { std::cerr << "EXPECT_FALSE failed: " #x " at line " << __LINE__ << "\n"; } } while(0)
#define EXPECT_EQ(a, b) do { if((a) != (b)) { std::cerr << "EXPECT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) << ") at line " << __LINE__ << "\n"; } } while(0)
#define EXPECT_BYTES_EQ(a, b, n) do { if(std::memcmp((a), (b), (n)) != 0) { std::cerr << "EXPECT_BYTES_EQ failed: bytes differ at line " << __LINE__ << "\n"; } } while(0)

// Simple IO recorder to capture what Type_Curve_Write emits via _cmsWriteUInt32Number/_cmsWriteUInt16Number/_cmsWriteUInt16Array
// We hook into the cmsIOHANDLER.Write callback by providing a custom io.Context that collects bytes.
struct IORecorderContext {
    std::vector<uint8_t> bytes;     // captured bytes
    // Control behavior for negative tests
    enum FailureMode { NONE = 0, FAIL_ON_FIRST_4BYTES_1, FAIL_ON_FIRST_4BYTES_3 } mode;
    bool failureTriggered;
    IORecorderContext() : mode(NONE), failureTriggered(false) {}
};

// Forward declare the Write callback that matches cmsIOHANDLER expected signature.
// Signature may vary slightly by library version; most versions use: cmsBool Write(struct _cmsIOHANDLER*, const cmsUInt8Number*, cmsUInt32Number)
static cmsBool RecorderWrite(struct _cmsIOHANDLER* io, const void* Buffer, cmsUInt32Number Size)
{
    // Retrieve our context
    IORecorderContext* ctx = reinterpret_cast<IORecorderContext*>(io->Context);

    // Optional failure simulation
    if (ctx && !ctx->failureTriggered) {
        if (ctx->mode == IORecorderContext::FAIL_ON_FIRST_4BYTES_1 && Size == 4) {
            const uint8_t* data = static_cast<const uint8_t*>(Buffer);
            // Little-endian representation of 1 is 01 00 00 00
            if (data[0] == 1 && data[1] == 0 && data[2] == 0 && data[3] == 0) {
                ctx->failureTriggered = true;
                return CMS_FALSE;
            }
        }
        if (ctx->mode == IORecorderContext::FAIL_ON_FIRST_4BYTES_3 && Size == 4) {
            const uint8_t* data = static_cast<const uint8_t*>(Buffer);
            // Little-endian representation of 3 is 03 00 00 00
            if (data[0] == 3 && data[1] == 0 && data[2] == 0 && data[3] == 0) {
                ctx->failureTriggered = true;
                return CMS_FALSE;
            }
        }
    }

    // Append bytes to the internal buffer
    const uint8_t* data = static_cast<const uint8_t*>(Buffer);
    if (ctx) {
        ctx->bytes.insert(ctx->bytes.end(), data, data + Size);
    }
    return CMS_TRUE;
}

// Helper to create a cmsIOHANDLER with our RecorderWrite bound
static void InitRecorderIO(IORecorderContext* ctx, cmsIOHANDLER* io)
{
    // Clear existing structure (best effort)
    std::memset(io, 0, sizeof(cmsIOHANDLER));

    io->Context = ctx;
    // The actual field names in cmsIOHANDLER may vary slightly by library version.
    // We assume a typical layout where a Write function pointer exists:
    io->Write = RecorderWrite;
}

// Helper to reset IO recorder
static void ResetRecorder(IORecorderContext* ctx)
{
    if (ctx) {
        ctx->bytes.clear();
        ctx->failureTriggered = false;
    }
}

// Test 1: Single gamma path (Curve.nSegments == 1 and Curve.Segments[0].Type == 1)
// We set gamma to 0.0 to yield a known 8.8 fixed value of 0.
// Expected bytes: [1 as uint32 little-endian] followed by [0 as uint16 little-endian] -> 6 bytes total
static void test_single_gamma_zero()
{
    // Prepare tone-curve data (library structs)
    cmsToneCurve Curve;
    // Initialize with at least the fields used by Type_Curve_Write
    Curve.nSegments = 1;
    // Access through the library's nested types (Segments[0].Type, Params[0])
    Curve.Segments[0].Type = 1;
    Curve.Segments[0].Params[0] = 0.0; // gamma = 0.0

    // IO recorder
    IORecorderContext ctx;
    cmsIOHANDLER io;
    InitRecorderIO(&ctx, &io);

    // Call the focal function
    cmsBool res = Type_Curve_Write(nullptr, &io, &Curve, 0);

    // Expectations
    EXPECT_TRUE(res);
    // Expected bytes: 01 00 00 00 00 00
    const unsigned char expected[] = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 };
    EXPECT_EQ(ctx.bytes.size(), sizeof(expected));
    EXPECT_BYTES_EQ(ctx.bytes.data(), expected, sizeof(expected));

    // Clean up if needed (Curve is usually stack-allocated here)
}

// Test 2: Non-single gamma path (Curve.nSegments != 1) -> write nEntries and Table16
// Expect first 4 bytes to be nEntries, followed by 2*nEntries bytes.
static void test_non_single_gamma_path()
{
    cmsToneCurve Curve;
    Curve.nSegments = 2;            // triggers else branch
    Curve.Segments[0].Type = 0;     // non-single gamma
    Curve.Segments[0].Params[0] = 1.23; // arbitrary

    Curve.nEntries = 3;
    static cmsUInt16Number arr[3] = { 0x00AA, 0x00BB, 0x00CC };
    Curve.Table16 = arr;

    IORecorderContext ctx;
    cmsIOHANDLER io;
    InitRecorderIO(&ctx, &io);

    cmsBool res = Type_Curve_Write(nullptr, &io, &Curve, 0);

    EXPECT_TRUE(res);
    // Expected: nEntries = 3 (0x03 00 00 00) then 3 16-bit values: AA 00, BB 00, CC 00
    const unsigned char expected[] = {
        0x03, 0x00, 0x00, 0x00,
        0xAA, 0x00,
        0xBB, 0x00,
        0xCC, 0x00
    };
    EXPECT_EQ(ctx.bytes.size(), sizeof(expected));
    EXPECT_BYTES_EQ(ctx.bytes.data(), expected, sizeof(expected));
}

// Test 3: Failure in first write in single gamma path
// We simulate the first 4-byte write (writing 1) failing.
static void test_failure_first_write_single_gamma()
{
    cmsToneCurve Curve;
    Curve.nSegments = 1;
    Curve.Segments[0].Type = 1;
    Curve.Segments[0].Params[0] = 0.0;

    IORecorderContext ctx;
    ctx.mode = IORecorderContext::FAIL_ON_FIRST_4BYTES_1;
    cmsIOHANDLER io;
    InitRecorderIO(&ctx, &io);

    cmsBool res = Type_Curve_Write(nullptr, &io, &Curve, 0);

    EXPECT_FALSE(res);
}

// Test 4: Failure in first write in non-single gamma path
// We simulate writing nEntries (3) failing.
static void test_failure_first_write_non_single_gamma()
{
    cmsToneCurve Curve;
    Curve.nSegments = 2;
    Curve.Segments[0].Type = 0;
    Curve.Segments[0].Params[0] = 1.23;

    Curve.nEntries = 3;
    static cmsUInt16Number arr[3] = { 0x00AA, 0x00BB, 0x00CC };
    Curve.Table16 = arr;

    IORecorderContext ctx;
    ctx.mode = IORecorderContext::FAIL_ON_FIRST_4BYTES_3;
    cmsIOHANDLER io;
    InitRecorderIO(&ctx, &io);

    cmsBool res = Type_Curve_Write(nullptr, &io, &Curve, 0);

    // We expect false because first write (nEntries) fails
    EXPECT_FALSE(res);
}

// Main runner
int main()
{
    std::cout << "Running Type_Curve_Write unit tests (without GTest)..." << std::endl;

    test_single_gamma_zero();
    test_non_single_gamma_path();
    test_failure_first_write_single_gamma();
    test_failure_first_write_non_single_gamma();

    std::cout << "Tests completed." << std::endl;
    return 0;
}