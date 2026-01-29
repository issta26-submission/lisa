/*
Unit test suite for ReadMatrix function in cmstypes.c
Target: ReadMatrix(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsUInt32Number Offset)

Assumptions and approach:
- We create a minimal C++11 test harness (no GoogleTest, no GMock) that links against the project under test.
- We exercise:
  1) The Seek failure path (io->Seek returns false) -> ReadMatrix should return NULL.
  2) The successful path where all 15.16 numbers are read (we fake Read to always succeed and provide fixed data, e.g., zeros). ReadMatrix should return a non-NULL Mat.
  3) The failure path during Read of the numeric data after several reads -> ReadMatrix should return NULL.
- We craft a lightweight cmsIOHANDLER mock by exploiting the real type in the library:
  - The library's cmsIOHANDLER contains function pointers like Seek, Read, Write and a ContextID field (used to carry per-test state). We populate those to route to our test harness.
- For the _cms_typehandler_struct self parameter, we instantiate a real object of that opaque type from the library header (we only set its ContextID for safety). ReadMatrix only accesses self->ContextID, so this is sufficient for the test.
- We implement lightweight “fake” IO functions in C++ that match the expected signatures and drive the test scenarios.
- We call ReadMatrix via extern "C" linkage to ensure correct symbol resolution.
- We clean up any allocated Mat using cmsStageFree to avoid leaks.

Notes:
- This test suite uses only the C++ standard library plus the library headers that ship with the project. It does not rely on GTest or any other testing framework.
- The approach focuses on maximizing coverage of the control flow: Seek success/failure, and Read success/failure in various phases.
- All tests are self-contained in a single file for ease of integration.

Compile and run:
- Ensure linking against the library that provides cmstypes.c (and the rest of LittleCMS or the project under test).
- Example (adjust to your build system):
  g++ -std=c++11 -I/path/to/include -L/path/to/lib test_readmatrix.cpp -l<your_lib> -o test_readmatrix
- Run: ./test_readmatrix

Code:
*/

#include <cstring>
#include <cstdio>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include library headers for types and the function under test

// Declare the ReadMatrix function as it's defined in cmstypes.c
extern "C" cmsStage* ReadMatrix(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsUInt32Number Offset);

// Simple wrappers to access the opaque structure from the test
// We rely on the fact that ReadMatrix uses only self->ContextID in the provided snippet.

struct _cms_typehandler_struct; // forward declaration (actual layout is in the library)
 
// Test IO context to drive the mock IO behavior
struct TestIoContext {
    bool seek_ok;            // whether Seek should succeed
    int read_calls;            // number of Read invocations performed
    int max_reads_before_fail; // if >= 0, fail after this many reads (0-based)
};

// Forward declarations of the mock IO functions matching the library's expected signatures.
// The exact function pointer types come from the header; we align with typical LittleCMS prototypes.

extern "C" {

// Seek function: returns success or failure based on test context
static cmsBool test_io_seek(struct _cms_iohandler_struct* io, cmsUInt32Number Offset) {
    // io should expose ContextID as a TestIoContext*
;   void* ctx_ptr = io->ContextID;
    if (!ctx_ptr) return 0;
    TestIoContext* ctx = static_cast<TestIoContext*>(ctx_ptr);
    // Use the per-test flag to decide
    return ctx->seek_ok ? 1 : 0;
}

// Read function: fill Buffer with zeros, or fail after a limit depending on test
static cmsBool test_io_read(struct _cms_iohandler_struct* io, cmsUInt8Number* Buffer, cmsUInt32Number Size) {
    void* ctx_ptr = io->ContextID;
    if (!ctx_ptr) return 0;
    TestIoContext* ctx = static_cast<TestIoContext*>(ctx_ptr);

    // If max_reads_before_fail is set, fail after max_reads_before_fail reads
    if (ctx->max_reads_before_fail >= 0) {
        if (ctx->read_calls > ctx->max_reads_before_fail) {
            return 0;
        }
    }

    // Fill with zeros (0.0 in 15.16 representation)
    if (Buffer && Size > 0) {
        std::memset(Buffer, 0, Size);
    }

    ctx->read_calls++;
    return 1;
}

// Write function (not used in tests but required to complete the struct)
static cmsBool test_io_write(struct _cms_iohandler_struct* io, const cmsUInt8Number* Buffer, cmsUInt32Number Size) {
    // No-op for tests
    (void)io; (void)Buffer; (void)Size;
    return 1;
}
}

// Helper utilities
static bool test_ReadMatrix_seek_failure() {
    // Prepare a test Self object (opaque in the library; we only set ContextID indirectly via io)
    struct _cms_typehandler_struct testSelf; // layout assumed to begin with ContextID in this codebase
    // The library reads only ContextID; we set something non-null as the cmsStageAllocMatrix may use the context in momentum
    testSelf.ContextID = (cmsContext)0xDEADBEEF; // arbitrary non-null

    // Prepare IO with Seek failure
    cmsIOHANDLER io;
    TestIoContext ctx;
    ctx.seek_ok = false;            // Seek will fail
    ctx.read_calls = 0;
    ctx.max_reads_before_fail = -1; // no extra failure
    io.ContextID = &ctx;
    io.Seek = test_io_seek;
    io.Read = test_io_read;
    io.Write = test_io_write;

    // Offset to test
    cmsUInt32Number offset = 0;

    cmsStage* Mat = ReadMatrix(&testSelf, &io, offset);

    // Expect NULL due to Seek failure
    bool ok = (Mat == NULL);
    if (!ok) {
        std::cerr << "ReadMatrix_seek_failure: Expected NULL, got non-NULL. Potential bug in Seek handling.\n";
    }

    // No need to free Mat since it's NULL
    return ok;
}

static bool test_ReadMatrix_success_zeroes() {
    // Prepare a test Self object
    struct _cms_typehandler_struct testSelf;
    testSelf.ContextID = (cmsContext)0xCAFEBABE;

    // Prepare IO that succeeds on Seek and reads zeros for all 12+3 numbers
    cmsIOHANDLER io;
    TestIoContext ctx;
    ctx.seek_ok = true;              // Seek succeeds
    ctx.read_calls = 0;
    ctx.max_reads_before_fail = -1;  // never fail
    io.ContextID = &ctx;
    io.Seek = test_io_seek;
    io.Read = test_io_read;
    io.Write = test_io_write;

    cmsUInt32Number offset = 0;

    cmsStage* Mat = ReadMatrix(&testSelf, &io, offset);

    bool ok = (Mat != NULL);
    if (ok) {
        // Clean up to avoid leaks
        cmsStageFree(Mat);
    } else {
        std::cerr << "ReadMatrix_success_zeroes: Expected non-NULL, got NULL. Read path failed unexpectedly.\n";
    }
    return ok;
}

static bool test_ReadMatrix_partial_read_failure() {
    // Prepare a test Self object
    struct _cms_typehandler_struct testSelf;
    testSelf.ContextID = (cmsContext)0xFEEDFACE;

    // IO: Seek succeeds, but reads fail after 6 reads
    cmsIOHANDLER io;
    TestIoContext ctx;
    ctx.seek_ok = true;
    ctx.read_calls = 0;
    ctx.max_reads_before_fail = 6; // fail after 6 reads; the 7th read should fail
    io.ContextID = &ctx;
    io.Seek = test_io_seek;
    io.Read = test_io_read;
    io.Write = test_io_write;

    cmsUInt32Number offset = 0;

    cmsStage* Mat = ReadMatrix(&testSelf, &io, offset);

    bool ok = (Mat == NULL);
    if (!ok) {
        // If ReadMatrix returned non-NULL, we must still free to avoid leaks
        cmsStageFree(Mat);
        std::cerr << "ReadMatrix_partial_read_failure: Expected NULL due to mid-read failure, got non-NULL.\n";
    } else {
        // Test passed; ensure no leak
    }

    return ok;
}
 
int main() {
    int tests_passed = 0;
    int total_tests = 3;

    std::cout << "Running ReadMatrix unit tests (CMS ReadMatrix) ...\n";

    if (test_ReadMatrix_seek_failure()) {
        std::cout << "[PASS] Seek failure path.\n";
        ++tests_passed;
    } else {
        std::cout << "[FAIL] Seek failure path.\n";
    }

    if (test_ReadMatrix_success_zeroes()) {
        std::cout << "[PASS] Success path with zero data (reads all numbers).\n";
        ++tests_passed;
    } else {
        std::cout << "[FAIL] Success path with zero data.\n";
    }

    if (test_ReadMatrix_partial_read_failure()) {
        std::cout << "[PASS] Partial read failure path.\n";
        ++tests_passed;
    } else {
        std::cout << "[FAIL] Partial read failure path.\n";
    }

    std::cout << "Tests completed: " << tests_passed << "/" << total_tests << "\n";
    return (tests_passed == total_tests) ? 0 : 1;
}