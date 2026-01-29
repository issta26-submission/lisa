// Test suite for the focal method: Type_ProfileSequenceId_Write
// This test file provides a self-contained harness (no GTest) to verify
// the behavior of Type_ProfileSequenceId_Write and its core dependencies.
// It uses a lightweight in-memory IO emulation and simple assertion macros
// to maximize coverage of true/false branches.
// Note: This harness redefines the minimal surface of the original types
// required by the focal function to be self-contained.

#include <cstring>
#include <cstdio>
#include <cstddef>
#include <lcms2_internal.h>
#include <cstdint>


// Domain knowledge notes (mapped to test scaffolding):
// - The focal method relies on: _cmsTagBase size, cmsSEQ.n, cmsIOHANDLER with a Tell function,
//   _cmsWriteUInt32Number(io, value), WritePositionTable(...), and a PositionTableEntryFn like
//   WriteSeqID(...).
// - We provide lightweight stand-ins for those to exercise the control flow.
// - We test true/false branches by controlling the behavior of the dependent functions.

//////////////////////////////////////////////////////////////////
// Lightweight type definitions and test scaffolding
//////////////////////////////////////////////////////////////////

// Basic boolean and width types (mimicking the CMS types sufficiently for test)
typedef int cmsBool;
typedef uint32_t cmsUInt32Number;

#define TRUE 1
#define FALSE 0

// Minimal struct definitions to satisfy the focal function's type usage

// Represents the base tag header; its size is used in the base-offset calculation
typedef struct _cmsTagBase {
    int dummy;
} _cmsTagBase;

// Forward declaration of the typehandler struct (as used by the focal method)
struct _cms_typehandler_struct;

// IO handler with the subset the focal method uses (Tell)
typedef struct cmsIOHANDLER {
    cmsUInt32Number (*Tell)(struct cmsIOHANDLER* io);
} cmsIOHANDLER;

// Simple sequence structure used by the focal method
typedef struct cmsSEQ {
    cmsUInt32Number n;
} cmsSEQ;

// Position table element function signature (format used by the core code)
typedef cmsBool (*PositionTableEntryFn)(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Cargo, cmsUInt32Number n, cmsUInt32Number SizeOfTag);

// Typehandler placeholder (not used for logic beyond pointer type)
typedef struct _cms_typehandler_struct {
    int dummy;
} cms_typehandler_struct;

// ---- Prototypes of the dependencies used by the focal method ----
// In the real library these live in the library; for test harness we provide
// lightweight, controllable implementations.

static cmsBool _cmsWriteUInt32Number(cmsIOHANDLER* io, cmsUInt32Number n);
static cmsBool WritePositionTable(struct _cms_typehandler_struct* self,
                                  cmsIOHANDLER* io,
                                  cmsUInt32Number SizeOfTag,
                                  cmsUInt32Number Count,
                                  cmsUInt32Number BaseOffset,
                                  void* Cargo,
                                  PositionTableEntryFn ElementFn);
static cmsBool WriteSeqID(struct _cms_typehandler_struct* self,
                          cmsIOHANDLER* io,
                          void* Cargo,
                          cmsUInt32Number n,
                          cmsUInt32Number SizeOfTag);

// Forward declaration of the focal method (to be defined in this test file)
static cmsBool Type_ProfileSequenceId_Write(struct _cms_typehandler_struct* self,
                                             cmsIOHANDLER* io,
                                             void* Ptr,
                                             cmsUInt32Number nItems);

// Utility for test assertions and state
static int g_failures = 0;
#define EXPECT_TRUE(cond) do { if(!(cond)) { printf("EXPECT_TRUE failed: %s:%d: %s is false\n", __FILE__, __LINE__, #cond); g_failures++; } } while(0)
#define EXPECT_FALSE(cond) do { if((cond)) { printf("EXPECT_FALSE failed: %s:%d: %s is true\n", __FILE__, __LINE__, #cond); g_failures++; } } while(0)
#define EXPECT_EQ(a,b) do { if((a)!=(b)) { printf("EXPECT_EQ failed: %s:%d: %s == %s (0x%X) != %s (0x%X)\n", __FILE__, __LINE__, #a, #b, (unsigned)(a), #b, (unsigned)(b)); g_failures++; } } while(0)
#define EXPECT_BYTES_EQ(pos, len, expected) do { \
    if((len) != 0) { \
        if(memcmp(g_ioBuffer + (pos), (expected), (len)) != 0) { \
            printf("EXPECT_BYTES_EQ failed at pos %zu: buffer content mismatch\n", (size_t)(pos)); g_failures++; \
        } \
    } \
} while(0)
#define UNUSED(x) (void)(x)


// Global test state for mocking behavior
static unsigned char g_ioBuffer[4096];
static size_t g_ioPos = 0;

// For base offset calculation, we pretend the initial buffer contains the _cmsTagBase header.
// We'll initialize g_ioPos with sizeof(_cmsTagBase) to simulate the header already being written.
static const size_t g_tagBaseSize = sizeof(_cmsTagBase);
static int g_forceWriteUInt32FailureFirstCall = 0;
static int g_failPositionTable = 0;

// Helper: reset test state
static void reset_test_state() {
    g_ioPos = g_tagBaseSize;
    memset(g_ioBuffer, 0, sizeof(g_ioBuffer));
    g_forceWriteUInt32FailureFirstCall = 0;
    g_failPositionTable = 0;
}

// Mock implementation of Tell(io)
static cmsUInt32Number mock_Tell(struct cmsIOHANDLER* io) {
    (void)io;
    return (cmsUInt32Number)g_ioPos;
}

// Write a 32-bit unsigned int in little-endian into the in-memory buffer
static cmsBool _cmsWriteUInt32Number_raw_inbuffer(cmsUInt32Number n) {
    if (g_ioPos + 4 > sizeof(g_ioBuffer)) return FALSE;
    g_ioBuffer[g_ioPos + 0] = (unsigned char)(n & 0xFF);
    g_ioBuffer[g_ioPos + 1] = (unsigned char)((n >> 8) & 0xFF);
    g_ioBuffer[g_ioPos + 2] = (unsigned char)((n >> 16) & 0xFF);
    g_ioBuffer[g_ioPos + 3] = (unsigned char)((n >> 24) & 0xFF);
    g_ioPos += 4;
    return TRUE;
}

// Public-facing (tested) wrapper that uses in-buffer representation
static cmsBool _cmsWriteUInt32Number(cmsIOHANDLER* io, cmsUInt32Number n) {
    (void)io; // In this harness, io is unused for buffer writing beyond buffer state
    // Simulate possible forced failure on the first call
    if (g_forceWriteUInt32FailureFirstCall) {
        // Only fail on the very first call
        static int called = 0;
        if (called == 0) {
            called++;
            return FALSE;
        }
        called++;
    }
    // Write to the in-memory buffer
    return _cmsWriteUInt32Number_raw_inbuffer(n);
}

// PositionTable: execute the ElementFn for each item and optionally fail
static cmsBool WritePositionTable(struct _cms_typehandler_struct* self,
                                  cmsIOHANDLER* io,
                                  cmsUInt32Number SizeOfTag,
                                  cmsUInt32Number Count,
                                  cmsUInt32Number BaseOffset,
                                  void* Cargo,
                                  PositionTableEntryFn ElementFn) {
    UNUSED(self); UNUSED(io); UNUSED(SizeOfTag); UNUSED(BaseOffset);
    UNUSED(Cargo);
    if (g_failPositionTable) return FALSE;
    for (cmsUInt32Number i = 0; i < Count; ++i) {
        if (ElementFn) {
            if (!ElementFn(nullptr, nullptr, Cargo, i,  SizeOfTag)) return FALSE;
        }
    }
    return TRUE;
}

// WriteSeqID: per-entry writer used by the position-table mechanism
static cmsBool WriteSeqID(struct _cms_typehandler_struct* self,
                          cmsIOHANDLER* io,
                          void* Cargo,
                          cmsUInt32Number n,
                          cmsUInt32Number SizeOfTag) {
    UNUSED(self); UNUSED(io); UNUSED(SizeOfTag);
    UNUSED(Cargo);
    // In this harness, just write the index as 32-bit value
    return _cmsWriteUInt32Number_raw_inbuffer(n);
}

// The focal method under test (copied here to be exercised against the test harness)
static cmsBool Type_ProfileSequenceId_Write(struct _cms_typehandler_struct* self,
                                             cmsIOHANDLER* io,
                                             void* Ptr,
                                             cmsUInt32Number nItems)
{
{
    cmsSEQ* Seq = (cmsSEQ*) Ptr;
    cmsUInt32Number BaseOffset;
    // Keep the base offset
    BaseOffset = io ->Tell(io) - sizeof(_cmsTagBase);
    // This is the table count
    if (!_cmsWriteUInt32Number(io, Seq ->n)) return FALSE;
    // This is the position table and content
    if (!WritePositionTable(self, io, 0, Seq ->n, BaseOffset, Seq, WriteSeqID)) return FALSE;
    return TRUE;
    cmsUNUSED_PARAMETER(nItems);
}
}

// Utility to print final buffer for debugging (optional)
static void print_buffer_hex(size_t len) {
    printf("Buffer (%zu bytes):", len);
    for (size_t i = 0; i < len; ++i) {
        if (i % 16 == 0) printf("\n%04zu: ", i);
        printf("%02X ", g_ioBuffer[i]);
    }
    printf("\n");
}

// Public helper used by tests
static cmsBool Type_ProfileSequenceId_Write_wrapper(struct _cms_typehandler_struct* self,
                                                     cmsIOHANDLER* io,
                                                     void* Ptr,
                                                     cmsUInt32Number nItems) {
    return Type_ProfileSequenceId_Write(self, io, Ptr, nItems);
}

//////////////////////////////////////////////////////////////////
// Tests
//////////////////////////////////////////////////////////////////

// Test 1: _cmsWriteUInt32Number fails on first call -> Type_ProfileSequenceId_Write returns FALSE
static void test_ProfileSequenceId_Write_WriteUInt32_FailsOnFirstCall() {
    reset_test_state();

    // Prepare IO with mock Tell
    cmsIOHANDLER io;
    io.Tell = mock_Tell;
    // Seq with n = 10
    cmsSEQ seq;
    seq.n = 10;

    // Force the first call to _cmsWriteUInt32Number to fail
    g_forceWriteUInt32FailureFirstCall = 1;

    cmsBool res = Type_ProfileSequenceId_Write_wrapper(nullptr, &io, &seq, 0);

    // Expect failure and no bytes written
    EXPECT_FALSE(res);
    EXPECT_EQ(g_ioPos, (size_t)g_tagBaseSize); // No writes occurred beyond header
    // Clear flag for other tests
    g_forceWriteUInt32FailureFirstCall = 0;
}

// Test 2: _cmsWriteUInt32Number succeeds, but WritePositionTable fails -> FALSE
static void test_ProfileSequenceId_Write_PositionTable_Fails() {
    reset_test_state();

    cmsIOHANDLER io;
    io.Tell = mock_Tell;
    cmsSEQ seq;
    seq.n = 4; // some small count

    // Ensure _cmsWriteUInt32Number will succeed on first call
    g_forceWriteUInt32FailureFirstCall = 0;
    g_failPositionTable = 1; // Force position table failure

    cmsBool res = Type_ProfileSequenceId_Write_wrapper(nullptr, &io, &seq, 0);

    EXPECT_FALSE(res);
    // Buffer should contain exactly 4 bytes for the sequence count (Seq.n)
    // and no subsequent entries due to failure in position table
    // So total bytes should be 4 (starting header 4)
    EXPECT_EQ(g_ioPos, (size_t)g_tagBaseSize + 4);
    g_failPositionTable = 0;
}

// Test 3: Both dependent paths succeed -> Type_ProfileSequenceId_Write returns TRUE
static void test_ProfileSequenceId_Write_Successful() {
    reset_test_state();

    cmsIOHANDLER io;
    io.Tell = mock_Tell;
    cmsSEQ seq;
    seq.n = 2; // two entries to write in the position table

    g_forceWriteUInt32FailureFirstCall = 0;
    g_failPositionTable = 0;

    cmsBool res = Type_ProfileSequenceId_Write_wrapper(nullptr, &io, &seq, 0);

    EXPECT_TRUE(res);
    // Expect 4 bytes for n, plus 2 writes from WriteSeqID (0 and 1)
    // Total: 12 bytes
    EXPECT_EQ(g_ioPos, (size_t)g_tagBaseSize + 4 + 2 * 4);

    // Validate the content: first 4 bytes should be '2' (Seq.n)
    uint32_t v0 = (uint32_t)g_ioBuffer[0] |
                  ((uint32_t)g_ioBuffer[1] << 8) |
                  ((uint32_t)g_ioBuffer[2] << 16) |
                  ((uint32_t)g_ioBuffer[3] << 24);
    EXPECT_EQ(v0, 2u);

    // Next entries should be 0 and 1
    uint32_t v1 = (uint32_t)g_ioBuffer[4] |
                  ((uint32_t)g_ioBuffer[5] << 8) |
                  ((uint32_t)g_ioBuffer[6] << 16) |
                  ((uint32_t)g_ioBuffer[7] << 24);
    uint32_t v2 = (uint32_t)g_ioBuffer[8] |
                  ((uint32_t)g_ioBuffer[9] << 8) |
                  ((uint32_t)g_ioBuffer[10] << 16) |
                  ((uint32_t)g_ioBuffer[11] << 24);

    EXPECT_EQ(v1, 0u);
    EXPECT_EQ(v2, 1u);
}

// Entry point: execute tests
int main() {
    printf("Running Type_ProfileSequenceId_Write unit tests (C++11 harness, no GTest)\n");
    test_ProfileSequenceId_Write_WriteUInt32_FailsOnFirstCall();
    test_ProfileSequenceId_Write_PositionTable_Fails();
    test_ProfileSequenceId_Write_Successful();

    if (g_failures == 0) {
        printf("All tests passed.\n");
        return 0;
    } else {
        printf("Tests failed: %d failures.\n", g_failures);
        return 1;
    }
}

// Helper macro to satisfy potential private-like usage in the focal method (no-op here)
#ifndef cmsUNUSED_PARAMETER
#define cmsUNUSED_PARAMETER(x) (void)(x)
#endif