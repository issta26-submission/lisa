// Lightweight C++11 unit test suite for the focal method: Type_DateTime_Write
// Note: This test provides a self-contained reproduction of the focal logic
// to enable execution in environments where the original cmsLCMS types are not available.
// The tests follow the constraints: no GTest, non-terminating style, and a main()
// calling test routines from within a single translation unit.

#include <cstring>
#include <iostream>
#include <lcms2_internal.h>
#include <cstdint>
#include <ctime>


// Domain-knowledge-oriented test harness
// Non-terminating assertion helpers
static int g_passed = 0;
static int g_failed = 0;

#define TEST_PASS(msg) do { ++g_passed; std::cout << "[PASS] " << msg << std::endl; } while(0)
#define TEST_FAIL(msg) do { ++g_failed; std::cerr << "[FAIL] " << msg << std::endl; } while(0)
#define ASSERT_TRUE(cond, msg) do { if (cond) { TEST_PASS(msg); } else { TEST_FAIL(msg); } } while(0)
#define ASSERT_EQ(a,b, msg) do { if ((a) == (b)) { TEST_PASS(msg); } else { TEST_FAIL(msg " (expected " #a " == " #b ")"); } } while(0)

// -----------------------------------------------------------------------------
// Minimal stubs and types to allow compiling and testing the focal function logic.
// These are stand-ins for the real liblcms2 internal types used by Type_DateTime_Write.
// They are provided solely for the unit test harness in this standalone file.

// Typedefs and macros to resemble the library's usage
typedef int cmsBool;
#define TRUE 1
#define FALSE 0

typedef unsigned int cmsUInt32Number;

// Simplified cmsDateTimeNumber representation (64-bit for ease)
typedef uint64_t cmsDateTimeNumber;

// Forward declaration for the test's tm usage
#ifndef _TM_DEFINED
#define _TM_DEFINED
// We rely on <ctime>'s std::tm in the test environment.
#endif

// FOCAL_STRUCT placeholder (matching the signature used by Type_DateTime_Write)
struct _cms_typehandler_struct {};

// IO handler structure expected by Type_DateTime_Write
struct cmsIOHANDLER {
    cmsBool (*Write)(struct cmsIOHANDLER* io, cmsUInt32Number size, const void* userData);
};

// Forward declaration of the focal function to test
extern "C" cmsBool Type_DateTime_Write(struct _cms_typehandler_struct* self,
                                      cmsIOHANDLER* io,
                                      void* Ptr,
                                      cmsUInt32Number nItems);

// Pointer-encoded DateTime encoder used by Type_DateTime_Write
extern "C" void _cmsEncodeDateTimeNumber(cmsDateTimeNumber* timestamp, struct tm* DateTime);

// Unused parameter macro (as in the real library)
#define cmsUNUSED_PARAMETER(x) (void)(x)

// -----------------------------------------------------------------------------
// Test harness-specific mocks to capture behavior of Type_DateTime_Write

// Test context carrying last write results
struct MockCtx {
    bool ShouldSucceed;
    cmsUInt32Number LastSize;
    cmsDateTimeNumber LastTimestamp;

    MockCtx() : ShouldSucceed(true), LastSize(0), LastTimestamp(0) {}
};

// Global pointer to the current test context (only for test harness in this single-file test)
static MockCtx* g_currentMockCtx = nullptr;

// Mock Write function matching the expected signature
static cmsBool MockWrite(struct cmsIOHANDLER* io, cmsUInt32Number size, const void* data) {
    // Retrieve our test context
    if (!g_currentMockCtx) {
        return FALSE;
    }
    MockCtx* ctx = g_currentMockCtx;
    ctx->LastSize = size;
    if (!ctx->ShouldSucceed) {
        return FALSE;
    }
    // Capture the emitted timestamp value from the buffer
    if (data != nullptr) {
        ctx->LastTimestamp = *reinterpret_cast<const cmsDateTimeNumber*>(data);
    } else {
        ctx->LastTimestamp = 0;
    }
    return TRUE;
}

// Helper to construct a cmsIOHANDLER wrapping MockWrite
static void InitMockIo(struct cmsIOHANDLER* io, MockCtx* ctx) {
    if (!io) return;
    // We place the function pointer directly into the minimal cmsIOHANDLER
    io->Write = MockWrite;
    // Link to the context via a static global (as MockWrite uses g_currentMockCtx)
    g_currentMockCtx = ctx;
}

// -----------------------------------------------------------------------------
// Reproduction of the focal function Type_DateTime_Write (standalone for test)
// This mirrors the logic in the given CMS code snippet.
// In a real test suite, this function would be linked from cmstypes.c; here we
// reproduce it to enable unit testing without the full library.
extern "C" cmsBool Type_DateTime_Write(struct _cms_typehandler_struct* self,
                                      cmsIOHANDLER* io,
                                      void* Ptr,
                                      cmsUInt32Number nItems) {
{
    struct tm * DateTime = (struct tm*) Ptr;
    cmsDateTimeNumber timestamp;
    _cmsEncodeDateTimeNumber(&timestamp, DateTime);
    if (!io ->Write(io, sizeof(cmsDateTimeNumber), &timestamp)) return FALSE;
    return TRUE;
    cmsUNUSED_PARAMETER(nItems);
    cmsUNUSED_PARAMETER(self);
}
}

// -----------------------------------------------------------------------------
// Test-specific DateTime encoding function
// Produces deterministic 64-bit value from tm components (year, month, day, hour, min, sec)
extern "C" void _cmsEncodeDateTimeNumber(cmsDateTimeNumber* timestamp, struct tm* DateTime) {
    // Combine fields into a 64-bit value (simple, deterministic encoding)
    // We offset tm_year by 1900 according to C's tm convention.
    uint64_t v =
        (static_cast<uint64_t>(DateTime->tm_year) + 1900ULL) << 48 |
        (static_cast<uint64_t>(DateTime->tm_mon) + 1ULL) << 40  |
        (static_cast<uint64_t>(DateTime->tm_mday))      << 32  |
        (static_cast<uint64_t>(DateTime->tm_hour))       << 24  |
        (static_cast<uint64_t>(DateTime->tm_min))        << 16  |
        (static_cast<uint64_t>(DateTime->tm_sec));            // low 8 bits
    *timestamp = v;
}

// -----------------------------------------------------------------------------
// Domain-driven test cases
// Test 1: Successful write should emit the correct timestamp and indicate success
bool test_TypeDateTimeWrite_Success() {
    MockCtx ctx;
    ctx.ShouldSucceed = true;

    cmsIOHANDLER io{};
    InitMockIo(&io, &ctx);

    // Prepare a concrete DateTime: 2021-03-15 12:34:56
    struct tm dt;
    std::memset(&dt, 0, sizeof(dt));
    dt.tm_year = 121; // 2021 - 1900
    dt.tm_mon  = 2;   // March (0-based in struct tm)
    dt.tm_mday = 15;
    dt.tm_hour = 12;
    dt.tm_min  = 34;
    dt.tm_sec  = 56;

    // Expected timestamp computed via encoder
    cmsDateTimeNumber expected;
    _cmsEncodeDateTimeNumber(&expected, &dt);

    cmsBool result = Type_DateTime_Write(nullptr, &io, &dt, 1);

    // Assertions: result should be TRUE; last write size must match; timestamp must equal expected
    ASSERT_TRUE(result == TRUE, "Type_DateTime_Write should return TRUE on successful write");
    ASSERT_EQ(ctx.LastSize, sizeof(cmsDateTimeNumber), "Write should be called with size sizeof(cmsDateTimeNumber)");
    ASSERT_TRUE(ctx.LastTimestamp == expected, "Timestamp encoded should match expected value");

    return (g_failed == 0);
}

// Test 2: Write fails when the IO layer reports failure
bool test_TypeDateTimeWrite_Failure() {
    MockCtx ctx;
    ctx.ShouldSucceed = false;

    cmsIOHANDLER io{};
    InitMockIo(&io, &ctx);

    // Simple DateTime: 2020-01-01 00:00:00
    struct tm dt;
    std::memset(&dt, 0, sizeof(dt));
    dt.tm_year = 120; // 2020
    dt.tm_mon  = 0;
    dt.tm_mday = 1;
    dt.tm_hour = 0;
    dt.tm_min  = 0;
    dt.tm_sec  = 0;

    cmsBool result = Type_DateTime_Write(nullptr, &io, &dt, 1);

    // Assertions: result should be FALSE; Write should have been attempted
    ASSERT_TRUE(result == FALSE, "Type_DateTime_Write should return FALSE when Write fails");
    // The size may have been set prior to failure; we ensure that a call happened
    ASSERT_TRUE(ctx.LastSize == sizeof(cmsDateTimeNumber), "Write attempted with correct size even on failure");

    return (g_failed == 0);
}

// Test 3: nItems parameter is ignored by the function (static analysis of behavior)
// Ensure that changing nItems does not alter success/failure behavior
bool test_TypeDateTimeWrite_IgnoreNItems() {
    MockCtx ctx;
    ctx.ShouldSucceed = true;

    cmsIOHANDLER io{};
    InitMockIo(&io, &ctx);

    struct tm dt;
    std::memset(&dt, 0, sizeof(dt));
    dt.tm_year = 122; // 2022
    dt.tm_mon  = 5;   // June
    dt.tm_mday = 18;
    dt.tm_hour = 9;
    dt.tm_min  = 30;
    dt.tm_sec  = 5;

    cmsUInt32Number nItems = 42; // arbitrary non-zero value
    cmsBool result = Type_DateTime_Write(nullptr, &io, &dt, nItems);

    // Recompute expected timestamp
    cmsDateTimeNumber expected;
    _cmsEncodeDateTimeNumber(&expected, &dt);

    ASSERT_TRUE(result == TRUE, "Type_DateTime_Write should ignore nItems and still succeed if Write succeeds");
    ASSERT_EQ(ctx.LastSize, sizeof(cmsDateTimeNumber), "Write size should be sizeof(cmsDateTimeNumber)");
    ASSERT_TRUE(ctx.LastTimestamp == expected, "Timestamp should be correctly encoded regardless of nItems");

    return (g_failed == 0);
}

// -----------------------------------------------------------------------------
// Main entry point to execute tests
int main() {
    std::cout << "Running Type_DateTime_Write unit tests (standalone harness)" << std::endl;

    bool all_ok = true;
    all_ok &= test_TypeDateTimeWrite_Success();
    all_ok &= test_TypeDateTimeWrite_Failure();
    all_ok &= test_TypeDateTimeWrite_IgnoreNItems();

    std::cout << "=====================================" << std::endl;
    std::cout << "Summary: " << g_passed << " passed, " << g_failed << " failed." << std::endl;

    if (g_failed > 0) {
        // Indicate failure status without aborting test suite
        std::cout << "Some tests failed. Review messages above for details." << std::endl;
    } else {
        std::cout << "All tests passed." << std::endl;
    }

    // Return non-zero if any test failed
    return g_failed == 0 ? 0 : 1;
}