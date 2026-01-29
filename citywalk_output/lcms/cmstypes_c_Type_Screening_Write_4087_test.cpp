/*
 * Lightweight C++11 test harness for the focal method:
 * cmsBool Type_Screening_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems)
 *
 * Notes:
 * - This test suite is designed to exercise both true and false branches of the
 *   condition predicates inside Type_Screening_Write.
 * - We implement a minimal, memory-backed io (cmsIOHANDLER) with a controllable
 *   Write function to simulate success and failure scenarios without requiring
 *   the full application I/O system.
 * - Tests are written without GTest; a small, explicit harness is used with
 *   non-terminating assertions (returning false keeps running tests; main prints results).
 * - The tests rely on public/internal LittleCMS structures (lcms2.h / lcms2_internal.h).
 * - The tests assume typical LittleCMS types: cmsBool, cmsUInt32Number, cmsUInt8Number,
 *   cmsScreening, cmsScreeningChannel (or equivalent) as provided by the headers.
 * - The tests call the focal function directly. We provide extern "C" declarations
 *   for the function and necessary types to ensure linkage with the C code in cmstypes.c.
 *
 * Important: This test code is designed to be placed in a standalone .cpp file
 * and compiled/link with the library that provides the focal function (cmstypes.c).
 * Ensure the build links against the same version of LittleCMS headers used by
 * the library under test.
 *
 * Compilation note (example):
 *   g++ -std=c++11 -I<path-to-lcms-include> test_cmstypes.cpp -lcms2 -o test_cmstypes
 */

// Domain knowledge: Use only the standard library and provided APIs; no GTest; main drives tests.

#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// External C headers providing LittleCMS types and the focal function
extern "C" {
}

// Forward declaration of the focal function (as it is defined in cmstypes.c)
extern "C" cmsBool Type_Screening_Write(struct _cms_typehandler_struct* self,
                                       cmsIOHANDLER* io,
                                       void* Ptr,
                                       cmsUInt32Number nItems);

// Simple non-GTest test harness with explicit tests

// Minimal test assertion macro (non-terminating)
#define ASSERT_MSG(cond, msg) \
    do { if (!(cond)) { failures++; std::cerr << "[FAIL] " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")\n"; } else { /* no-op on pass */ } } while (0)

static int failures = 0;

// Minimal memory-backed IO with controllable Write behavior
// We rely on the fact that LittleCMS defines cmsIOHANDLER with a Write callback
// that has the signature: cmsBool Write(struct _cmsIOHANDLER* io, cmsUInt8Number* Buffer, cmsUInt32Number Size)
struct FakeIOContext {
    std::vector<uint8_t> written;
    // When true, the next call to Write will fail
    bool failMode;
    // If failMode is true, the nextWriteIndex specifies which call should fail (1-based)
    int failNextCallIndex;
    int callCount; // number of Write attempts made
};

static cmsBool fake_write(struct _cmsIOHANDLER* io, cmsUInt8Number* Buffer, cmsUInt32Number Size)
{
    // Retrieve our context
    FakeIOContext* ctx = reinterpret_cast<FakeIOContext*>(io->Context);
    if (!ctx) {
        // If no context configured, fail safe
        return FALSE;
    }

    ctx->callCount++;

    // Optional: store what would be written (not strictly necessary for tests)
    if (Buffer && Size) {
        ctx->written.insert(ctx->written.end(), Buffer, Buffer + Size);
    }

    if (ctx->failMode && ctx->failNextCallIndex == ctx->callCount) {
        // Simulate a failure on a specific call
        return FALSE;
    }

    return TRUE;
}

// Helper to prepare a cmsScreening with a given number of channels
static cmsScreening* make_screening(int nChannels,
                                    cmsUInt32Number flag)
{
    // Allocate screening struct
    cmsScreening* sc = new cmsScreening();
    // Initialize fields
    sc->Flag = flag;
    sc->nChannels = static_cast<cmsUInt32Number>(nChannels);

    if (nChannels > 0) {
        // Allocate channel array (assuming cmsScreeningChannel type exists)
        sc->Channels = new cmsScreeningChannel[nChannels];
        // Initialize with deterministic test values
        for (int i = 0; i < nChannels; ++i) {
            // Use distinct values for each channel to avoid aliasing in tests
            sc->Channels[i].Frequency   = static_cast<cmsUInt32Number>(0x00010000 * (i + 1)); // 1.0, 2.0, ...
            sc->Channels[i].ScreenAngle = static_cast<cmsUInt32Number>(0x00020000 * (i + 1)); // 2.0, 4.0, ...
            sc->Channels[i].SpotShape  = static_cast<cmsUInt32Number>(i + 1);                  // 1, 2, ...
        }
    } else {
        sc->Channels = nullptr;
    }

    return sc;
}

// Cleanup helper
static void free_screening(cmsScreening* sc)
{
    if (!sc) return;
    // If Channels allocated, delete[]
    if (sc->nChannels > 0 && sc->Channels) {
        delete[] sc->Channels;
        sc->Channels = nullptr;
    }
    delete sc;
}

// Test A: Normal success path with two channels
static bool test_Type_Screening_Write_Success_TwoChannels()
{
    // Prepare a two-channel screening
    cmsScreening* sc = make_screening(2, 0x01); // Flag arbitrary
    // Prepare a fake IO
    FakeIOContext ctx;
    ctx.failMode = false;
    ctx.failNextCallIndex = -1;
    ctx.callCount = 0;

    cmsIOHANDLER io;
    std::memset(&io, 0, sizeof(io));
    // Bind our fake IO's write function and context
    io.Write = fake_write;
    io.Context = &ctx;

    // Call the focal function
    cmsUInt32Number nItems = 1;
    cmsBool res = Type_Screening_Write(nullptr, &io, sc, nItems);

    // Cleanup
    free_screening(sc);

    // Assertions
    ASSERT_MSG(res == TRUE, "Type_Screening_Write should return TRUE when all writes succeed (two channels).");
    // Additionally, ensure we attempted some writes
    // At least 2 (Flag and nChannels) + 3*2 (for two channels) = 8 writes
    ASSERT_MSG(ctx.callCount >= 2, "Type_Screening_Write should perform multiple write calls for two channels.");
    return failures == 0;
}

// Test B: Failure on the very first write (Flag write)
static bool test_Type_Screening_Write_Fail_On_First_Write()
{
    cmsScreening* sc = make_screening(1, 0x02);
    FakeIOContext ctx;
    ctx.failMode = true;
    ctx.failNextCallIndex = 1; // Fail on first call
    ctx.callCount = 0;

    cmsIOHANDLER io;
    std::memset(&io, 0, sizeof(io));
    io.Write = fake_write;
    io.Context = &ctx;

    cmsUInt32Number nItems = 1;
    cmsBool res = Type_Screening_Write(nullptr, &io, sc, nItems);

    free_screening(sc);

    ASSERT_MSG(res == FALSE, "Type_Screening_Write should return FALSE when the first write (Flag) fails.");
    return failures == 0;
}

// Test C: Failure on a specific later write (Frequency of first channel)
static bool test_Type_Screening_Write_Fail_On_Third_Call()
{
    cmsScreening* sc = make_screening(1, 0x03); // single channel
    FakeIOContext ctx;
    ctx.failMode = true;
    ctx.failNextCallIndex = 3; // Third call corresponds to Frequency write
    ctx.callCount = 0;

    cmsIOHANDLER io;
    std::memset(&io, 0, sizeof(io));
    io.Write = fake_write;
    io.Context = &ctx;

    cmsUInt32Number nItems = 1;
    cmsBool res = Type_Screening_Write(nullptr, &io, sc, nItems);

    free_screening(sc);

    ASSERT_MSG(res == FALSE, "Type_Screening_Write should return FALSE when Frequency write (third call) fails.");
    return failures == 0;
}

// Test D: Edge case with zero channels (nChannels == 0)
static bool test_Type_Screening_Write_ZeroChannels()
{
    cmsScreening* sc = make_screening(0, 0x04);
    FakeIOContext ctx;
    ctx.failMode = false;
    ctx.failNextCallIndex = -1;
    ctx.callCount = 0;

    cmsIOHANDLER io;
    std::memset(&io, 0, sizeof(io));
    io.Write = fake_write;
    io.Context = &ctx;

    cmsUInt32Number nItems = 1;
    cmsBool res = Type_Screening_Write(nullptr, &io, sc, nItems);

    free_screening(sc);

    ASSERT_MSG(res == TRUE, "Type_Screening_Write should return TRUE when there are zero channels (only header written).");
    return failures == 0;
}

// Main: run all tests and report summary
int main() {
    int total = 0;
    int ok = 0;

    if (test_Type_Screening_Write_Success_TwoChannels()) ok++;
    total++;

    if (test_Type_Screening_Write_Fail_On_First_Write()) ok++;
    total++;

    if (test_Type_Screening_Write_Fail_On_Third_Call()) ok++;
    total++;

    if (test_Type_Screening_Write_ZeroChannels()) ok++;
    total++;

    std::cout << "Test results: " << ok << "/" << total << " tests passed." << std::endl;

    // Non-terminating: do not call exit with non-zero status based on this harness
    // Return 0 to indicate test binary finished (even if some tests fail, detailed output already shown)
    return (failures == 0) ? 0 : 1;
}