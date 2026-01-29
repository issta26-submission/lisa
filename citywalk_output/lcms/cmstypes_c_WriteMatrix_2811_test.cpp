// Unit tests for the focal function: cmsBool WriteMatrix(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsStage* mpe)
// This test suite targets the WriteMatrix path in cmstypes.c, leveraging the same data structures used by the production code.
// Notes:
// - We rely on the public and internal CMS headers to assemble a realistic cmsStage and underlying matrix data structure.
// - This test suite is written for C++11, without Google Test. A tiny in-house assertion framework is provided.
// - The tests focus on:
//   1) Successful writes when an Offset array is provided.
//   2) Successful writes when Offset is NULL (zeroed writes).
//   3) Failure propagation when the underlying I/O write path fails.
// - The tests use a lightweight in-process cmsIOHANDLER with a custom Write callback to capture written bytes and to simulate failures.

#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include CMS internal headers to access the relevant structures used by WriteMatrix.
// The exact headers may vary by repository layout; ensure the paths match your build environment.

// Lightweight in-house test framework (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define ASSERT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        std::cerr << "Test failure: " << (msg) << " (" << #cond << ")" << std::endl; \
        ++g_tests_failed; \
    } \
} while (0)

#define ASSERT_FALSE(cond, msg) ASSERT_TRUE(!(cond), msg)
#define ASSERT_EQ(a, b, msg) do { \
    ++g_tests_run; \
    if (!((a) == (b))) { \
        std::cerr << "Test failure: " << (msg) << " (expected " << (b) << ", got " << (a) << ")" << std::endl; \
        ++g_tests_failed; \
    } \
} while (0)

// Helper assertion to ensure the test reports cleanly even without a full test framework
static void print_summary() {
    std::cerr << "\nTest summary: "
              << "Run=" << g_tests_run << ", Failures=" << g_tests_failed << std::endl;
}

// Forward declaration of the focal function under test
// The function prototype is taken from the provided <FOCAL_METHOD> snippet.
extern "C" cmsBool WriteMatrix(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsStage* mpe);

// A minimal shim to simulate a CMS IO write path.
// We implement the Write callback to capture written bytes without altering library behavior.
struct TestIOContext {
    std::vector<uint8_t> writtenBytes; // capture raw bytes written by _cmsWrite15Fixed16Number
    bool failOnWrite;                    // when true, simulate a write failure to exercise the error path
    // Optional: track the number of write calls
    size_t writeCalls;

    TestIOContext() : failOnWrite(false), writeCalls(0) {}
};

// The CMS Write callback signature varies by CMS version.
// This test assumes a signature compatible with the internal CMS IO handler used by WriteMatrix.
// We implement a static function and register it in the cmsIOHANDLER instance.
// Note: If your repository uses a different exact typedef for the Write callback,
// adjust the prototype accordingly (return type cmsBool, parameters (struct _cmsIOHANDLER*, cmsUInt8Number*, cmsUInt32Number)).
static cmsBool TestWriteCallback(struct _cmsIOHANDLER* io, cmsUInt8Number* Buffer, cmsUInt32Number Count) {
    // The io->ContextID is used to retrieve our TestIOContext instance
    TestIOContext* ctx = reinterpret_cast<TestIOContext*>(io->ContextID);
    if (!ctx) return FALSE;

    ctx->writeCalls++;
    if (ctx->failOnWrite) {
        // Simulate a failure to write
        return FALSE;
    }

    // Record the written data as-is (byte-for-byte)
    if (Buffer != nullptr && Count > 0) {
        ctx->writtenBytes.insert(ctx->writtenBytes.end(), Buffer, Buffer + Count);
    }
    // Indicate success
    return TRUE;
}

// Helper to create and initialize a cmsIOHANDLER with our test callback
static void InitTestIOHANDLER(cmsIOHANDLER* io, TestIOContext* ctx) {
    // Zero initialize to avoid accidental usage of uninitialized fields
    std::memset(io, 0, sizeof(*io));

    // Attach our test context
    io->ContextID = static_cast<void*>(ctx);

    // Register the callback; adjust member names if your CMS header uses different field names
    io->Write = &TestWriteCallback;
    // Read/Close are not used by this test, but can be set if needed
    io->Read = nullptr;
    io->Close = nullptr;
}

// Test 1: WriteMatrix with a non-NULL Offset array; expect successful write path and correct number of bytes written
static void test_WriteMatrix_WithOffset_Succeeds() {
    // Prepare a stage with a 2x3 matrix (InputChannels=2, OutputChannels=3)
    cmsStage mpe;
    std::memset(&mpe, 0, sizeof(mpe));
    mpe.InputChannels = 2;
    mpe.OutputChannels = 3;

    // Set up matrix data: Double[6] values and Offset[3] values
    _cmsStageMatrixData matrix;
    // The real library defines 'Double' as an array member inside _cmsStageMatrixData;
    // We'll provide a fixed array with 6 values.
    double doubles[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
    matrix.Double = doubles;

    double offsets[3] = { 0.25, -0.5, 1.0 };
    matrix.Offset = offsets;

    // Attach matrix data to mpe
    mpe.Data = &matrix;

    // Prepare a test IO
    cmsIOHANDLER io;
    TestIOContext ctx;
    InitTestIOHANDLER(&io, &ctx);

    // Call the focal function
    cmsBool result = WriteMatrix(nullptr, &io, &mpe);

    // Expected: TRUE and a number of bytes written equal to (n + OutputChannels) * 4 bytes (assuming 4-byte 15.16 numbers)
    // Here n = InputChannels * OutputChannels = 6, OutputChannels = 3, total 9 numbers -> 9 * 4 = 36 bytes
    const size_t expectedBytes = (mpe.InputChannels * mpe.OutputChannels + mpe.OutputChannels) * 4;

    ASSERT_TRUE(result == TRUE, "WriteMatrix should succeed when Offset is provided");
    ASSERT_TRUE(ctx.writtenBytes.size() == expectedBytes, "WriteMatrix should write the expected number of bytes for 9 numbers (6 matrix + 3 offsets)");
    // Clear and print a success message
    if (result == TRUE && ctx.writtenBytes.size() == expectedBytes) {
        std::cout << "test_WriteMatrix_WithOffset_Succeeds: PASS" << std::endl;
    } else {
        std::cout << "test_WriteMatrix_WithOffset_Succeeds: FAIL" << std::endl;
    }
}

// Test 2: WriteMatrix with a NULL Offset array; expect successful write path where zeros are written for each OutputChannel
static void test_WriteMatrix_OffsetNull_Succeeds() {
    // Prepare a stage with a 1x2 matrix (InputChannels=1, OutputChannels=2)
    cmsStage mpe;
    std::memset(&mpe, 0, sizeof(mpe));
    mpe.InputChannels = 1;
    mpe.OutputChannels = 2;

    // Matrix with two doubles; Offset is NULL
    _cmsStageMatrixData matrix;
    double doubles[2] = { 7.5, -3.25 };
    matrix.Double = doubles;
    matrix.Offset = nullptr; // Trigger the "zeros" path

    mpe.Data = &matrix;

    // Prepare a test IO
    cmsIOHANDLER io;
    TestIOContext ctx;
    InitTestIOHANDLER(&io, &ctx);

    // Call the focal function
    cmsBool result = WriteMatrix(nullptr, &io, &mpe);

    // Expect 2 numbers for the matrix plus 2 zeros for the offset path;
    // total 4 numbers * 4 bytes = 16 bytes
    const size_t expectedBytes = (mpe.InputChannels * mpe.OutputChannels + mpe.OutputChannels) * 4;

    ASSERT_TRUE(result == TRUE, "WriteMatrix should succeed when Offset is NULL (zeros written)");
    ASSERT_TRUE(ctx.writtenBytes.size() == expectedBytes, "WriteMatrix should write the expected number of bytes for 4 numbers (2 matrix + 2 zeros)");
    if (result == TRUE && ctx.writtenBytes.size() == expectedBytes) {
        std::cout << "test_WriteMatrix_OffsetNull_Succeeds: PASS" << std::endl;
    } else {
        std::cout << "test_WriteMatrix_OffsetNull_Succeeds: FAIL" << std::endl;
    }
}

// Test 3: WriteMatrix failure path when the underlying IO write fails
static void test_WriteMatrix_WriteFailurePropgates() {
    // Prepare a stage with a small matrix
    cmsStage mpe;
    std::memset(&mpe, 0, sizeof(mpe));
    mpe.InputChannels = 1;
    mpe.OutputChannels = 1;

    _cmsStageMatrixData matrix;
    double doubles[1] = { 12.0 };
    matrix.Double = doubles;
    matrix.Offset = nullptr; // Not used if OutputChannels==1; but either way, the failure path should short-circuit

    mpe.Data = &matrix;

    // Prepare a test IO that will fail on first write
    cmsIOHANDLER io;
    TestIOContext ctx;
    InitTestIOHANDLER(&io, &ctx);
    ctx.failOnWrite = true; // Force the Write callback to fail

    cmsBool result = WriteMatrix(nullptr, &io, &mpe);

    ASSERT_TRUE(result == FALSE, "WriteMatrix should return FALSE if underlying IO write fails");
    if (result == FALSE) {
        std::cout << "test_WriteMatrix_WriteFailurePropgates: PASS" << std::endl;
    } else {
        std::cout << "test_WriteMatrix_WriteFailurePropgates: FAIL" << std::endl;
    }
}

// Main test runner
int main() {
    // Run tests
    test_WriteMatrix_WithOffset_Succeeds();
    test_WriteMatrix_OffsetNull_Succeeds();
    test_WriteMatrix_WriteFailurePropgates();

    // Print summary
    print_summary();
    return g_tests_failed > 0 ? 1 : 0;
}