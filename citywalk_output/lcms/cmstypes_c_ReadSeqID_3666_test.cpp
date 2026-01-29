/*
  Unit test suite for cmsBool ReadSeqID (ReadSeqID in the focal method)
  - Target environment: C++11, no GoogleTest
  - Tests rely on the library's internal cmsSEQ / cmsPSEQDESC structures and cmsIOHANDLER
  - Test strategy:
      1) Success path: verify 16-byte ProfileID.ID8 is filled by io->Read and ReadEmbeddedText yields TRUE
      2) Early failure path: simulate io->Read returning 0 on first call, ensuring ReadSeqID returns FALSE
      3) Embedded-text failure path: simulate io->Read succeeding for the first call but failing for subsequent reads to trigger ReadEmbeddedText failure
  - Note: We implement a lightweight fake cmsIOHANDLER.Read by using a global test-mode switch and a simple buffer-filling behavior.
  - This file should be compiled and linked with the project that provides the actual cmsReadSeqID and dependencies
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Domain-specific: include internal CMS types. Adjust path if needed for your project.
extern "C" {
}

// Lightweight test harness (non-terminating assertions)
// - Keeps running after failures to maximize coverage
static int g_totalTests = 0;
static int g_failedTests = 0;
static int g_testMode = 0;       // 0 = success path, 1 = fail on first Read, 2 = fail after first Read
static int g_firstCallHit = 0;   // tracks first Read call

// Fake Read implementation for cmsIOHANDLER
// Behavior controlled by g_testMode:
//   - Mode 0 (success): first Read fills 16 bytes with 1..16, subsequent Reads fill with 0xAA and return 1
//   - Mode 1 (fail on first): first Read returns 0
//   - Mode 2 (embedded-text fail after first): first Read fills 16 bytes with 1..16, subsequent Reads return 0
static cmsUInt32Number fakeRead(struct _cmsIOHANDLER* self, void* Buffer, cmsUInt32Number Size, cmsUInt32Number Count)
{
    cmsUInt32Number bytes = Size * Count;
    if (g_testMode == 0) { // success path
        if (g_firstCallHit == 0) {
            unsigned char* p = (unsigned char*)Buffer;
            int limit = (bytes < 16) ? (int)bytes : 16;
            for (int i = 0; i < limit; ++i) p[i] = static_cast<unsigned char>(i + 1); // 1..16
            g_firstCallHit = 1;
            return 1;
        } else {
            unsigned char* p = (unsigned char*)Buffer;
            int limit = (bytes < 16) ? (int)bytes : 16;
            for (int i = 0; i < limit; ++i) p[i] = 0xAA;
            return 1;
        }
    } else if (g_testMode == 2) { // embedded-text fail after first
        if (g_firstCallHit == 0) {
            unsigned char* p = (unsigned char*)Buffer;
            int limit = (bytes < 16) ? (int)bytes : 16;
            for (int i = 0; i < limit; ++i) p[i] = static_cast<unsigned char>(i + 1);
            g_firstCallHit = 1;
            return 1;
        } else {
            // Subsequent reads fail, causing ReadEmbeddedText to fail
            return 0;
        }
    } else { // Mode 1: fail on first read
        return 0;
    }
}

// Global fake IO handler instance used by tests
static cmsIOHANDLER g_fakeIo;

// Assert-like helpers (non-terminating)
#define EXPECT_EQ(a, b, msg) do { \
    ++g_totalTests; \
    if ((a) != (b)) { \
        std::cerr << "FAIL: " << msg << " (expected " << (b) << ", got " << (a) << ")" << std::endl; \
        ++g_failedTests; \
    } \
} while (0)

#define EXPECT_TRUE(a, msg) do { \
    ++g_totalTests; \
    if (!(a)) { \
        std::cerr << "FAIL: " << msg << std::endl; \
        ++g_failedTests; \
    } \
} while (0)

#define TEST_BEGIN(label) std::cout << "[ RUN      ] " << label << std::endl;
#define TEST_END(label)   std::cout << "[  DONE    ] " << label << std::endl;

// Prototypes for the focal function and required types (from library headers)
extern "C" cmsBool ReadSeqID(struct _cms_typehandler_struct* self,
                             cmsIOHANDLER* io,
                             void* Cargo,
                             cmsUInt32Number n,
                             cmsUInt32Number SizeOfTag);

// Test 1: Success path - verify io->Read fills ProfileID.ID8 and ReadEmbeddedText returns TRUE
void test_ReadSeqID_Success_Path() {
    TEST_BEGIN("ReadSeqID_Success_Path");

    // Prepare IO
    g_testMode = 0;        // success path
    g_firstCallHit = 0;
    g_fakeIo = {};           // zero initialize
    g_fakeIo.Read = &fakeRead;

    // Prepare cargo: cmsSEQ with at least one element
    cmsSEQ OutSeq;
    // Allocate 1 element for safety (n = 0 used by test)
    OutSeq.seq = new cmsPSEQDESC[1];
    std::memset(OutSeq.seq, 0, sizeof(cmsPSEQDESC)); // clear

    // Self typehandler (dummy)
    struct _cms_typehandler_struct dummySelf;
    cmsBool result = ReadSeqID(&dummySelf, &g_fakeIo, &OutSeq, 0, 10);

    // Assertions
    EXPECT_TRUE(result == TRUE, "ReadSeqID should return TRUE on success path");
    // Verify 16-byte ID8 filled with 1..16 from the first Read call
    bool idOk = true;
    for (int i = 0; i < 16; ++i) {
        unsigned char val = OutSeq.seq[0].ProfileID.ID8[i];
        if (val != static_cast<unsigned char>(i + 1)) { idOk = false; break; }
    }
    EXPECT_TRUE(idOk, "ProfileID.ID8 should be filled with 1..16 by first Read");

    // Cleanup
    delete[] OutSeq.seq;

    TEST_END("ReadSeqID_Success_Path");
}

// Test 2: Early failure path - simulate io->Read returning 0 on the very first call
void test_ReadSeqID_Fail_On_First_Read() {
    TEST_BEGIN("ReadSeqID_Fail_On_First_Read");

    // Prepare IO
    g_testMode = 1;        // fail on first read
    g_firstCallHit = 0;
    g_fakeIo = {};
    g_fakeIo.Read = &fakeRead;

    cmsSEQ OutSeq;
    OutSeq.seq = new cmsPSEQDESC[1];
    std::memset(OutSeq.seq, 0, sizeof(cmsPSEQDESC));

    struct _cms_typehandler_struct dummySelf;
    cmsBool result = ReadSeqID(&dummySelf, &g_fakeIo, &OutSeq, 0, 10);

    EXPECT_TRUE(result == FALSE, "ReadSeqID should return FALSE when first Read fails");
    delete[] OutSeq.seq;

    TEST_END("ReadSeqID_Fail_On_First_Read");
}

// Test 3: Embedded-text failure path - allow first Read to succeed, then fail on subsequent Reads used by ReadEmbeddedText
void test_ReadSeqID_Fail_On_EmbeddedText() {
    TEST_BEGIN("ReadSeqID_Fail_On_EmbeddedText");

    // Prepare IO
    g_testMode = 2;        // fail after first read
    g_firstCallHit = 0;
    g_fakeIo = {};
    g_fakeIo.Read = &fakeRead;

    cmsSEQ OutSeq;
    OutSeq.seq = new cmsPSEQDESC[1];
    std::memset(OutSeq.seq, 0, sizeof(cmsPSEQDESC));

    struct _cms_typehandler_struct dummySelf;
    cmsBool result = ReadSeqID(&dummySelf, &g_fakeIo, &OutSeq, 0, 10);

    EXPECT_TRUE(result == FALSE, "ReadSeqID should return FALSE when embedded text read fails");
    delete[] OutSeq.seq;

    TEST_END("ReadSeqID_Fail_On_EmbeddedText");
}

// Main: run all tests and report summary
int main() {
    // Run tests
    test_ReadSeqID_Success_Path();
    test_ReadSeqID_Fail_On_First_Read();
    test_ReadSeqID_Fail_On_EmbeddedText();

    // Summary
    std::cout << "Tests run: " << g_totalTests << ", Failures: " << g_failedTests << std::endl;
    return g_failedTests == 0 ? 0 : 1;
}