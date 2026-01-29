// C++11 test suite for cmsBool Type_MPEclut_Write (no gtest, no gmock)
//
// This test harness directly exercises the focal function Type_MPEclut_Write
// from the cms library (lcms2). It uses a lightweight in-process mock CMS I/O
// handler to validate write behavior and data flow for both success and
// failure scenarios. The test suite is designed to compile with a C++11
// compiler and link against the LittleCMS-like library this code targets.
// Static/internal structures (_cmsStageCLutData, _cmsStageCLutParams, cmsStage)
// are accessed via the internal header to construct a realistic mpe/clut payload.
//
// Important: This code is intended as a unit-test harness and should be built
// in an environment where the cms headers (lcms2.h, lcms2_internal.h) are
// available and the Type_MPEclut_Write symbol is linkable.

#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>


// Bring in internal and public CMS types for test construction.
// We use extern "C" to ensure C linkage for the C-based CMS headers.
extern "C" {
}

// Lightweight non-terminating test assertion helpers
// (do not exit on failure; just report and continue)
#define TEST_PASSED  true
#define TEST_FAILED  false

// Simple test result container
struct TestResult {
    const char* name;
    bool ok;
    std::string message;
};

// Lightweight mock of cmsIOHANDLER that captures writes into a byte buffer
// Layout: first member is cmsIOHANDLER so that &m.io can be cast to cmsIOHANDLER*
struct MockIO {
    cmsIOHANDLER io;                 // interface expected by Type_MPEclut_Write
    std::vector<unsigned char> data;  // captured written bytes
    int write_calls;                   // number of Write invocations
    int failOnWrite;                   // 1-based index of the call on which to fail; -1 means never fail
};

// Forward declaration of the focal function
extern "C" cmsBool Type_MPEclut_Write(struct _cms_typehandler_struct* self,
                                    cmsIOHANDLER* io,
                                    void* Ptr,
                                    cmsUInt32Number nItems);

static cmsBool MockIO_Write(struct _cmsIOHANDLER* io, cmsUInt32Number Size, const void* Buffer)
{
    // io is actually the address of MockIO::io, which is the first member.
    MockIO* m = (MockIO*)io;

    // Optionally simulate a write failure on a specific invocation (1-based index)
    if (m->failOnWrite >= 1 && (m->write_calls + 1) == m->failOnWrite) {
        return FALSE;
    }

    const unsigned char* p = (const unsigned char*)Buffer;
    m->data.insert(m->data.end(), p, p + Size);
    m->write_calls++;
    return TRUE;
}

// Initialize a MockIO instance
static void initMockIO(MockIO& m)
{
    // Zero initialize the cmsIOHANDLER struct
    std::memset(&m.io, 0, sizeof(cmsIOHANDLER));
    m.io.Write = MockIO_Write;
    m.io.Read = nullptr;
    m.io.Close = nullptr;
    m.io.User = (void*)(&m);  // user data pointer if needed
    m.data.clear();
    m.write_calls = 0;
    m.failOnWrite = -1;
}

// Helper to create a simple _cmsStageCLutData payload for testing
static _cmsStageCLutData* createSampleCLUTData(int inputChannels, int nEntries)
{
    // Allocate CLUT data block
    _cmsStageCLutData* clut = (_cmsStageCLutData*) std::malloc(sizeof(_cmsStageCLutData));
    std::memset(clut, 0, sizeof(_cmsStageCLutData));

    clut->HasFloatValues = TRUE;
    clut->nEntries = nEntries;

    // Allocate Params with nSamples array (one sample per channel)
    clut->Params = (_cmsStageCLutParams*) std::malloc(sizeof(_cmsStageCLutParams));
    std::memset(clut->Params, 0, sizeof(_cmsStageCLutParams));

    // Fill per-channel sample counts (0..max-1 admissible)
    // We rely on internal layout: nSamples remains an array accessible by index
    for (int i = 0; i < inputChannels; ++i) {
        clut->Params->nSamples[i] = (cmsUInt8Number)(i + 1);
    }

    // Fill a small CLUT row table with floating values
    for (int i = 0; i < nEntries; ++i) {
        clut->Tab.TFloat[i] = (cmsFloat32Number)(0.25f + i * 0.25f);
    }

    return clut;
}

// Helper to create and initialize a minimal cmsStage (mpe) that references the CLUT
static cmsStage* createStageWithCLUT(_cmsStageCLutData* clut, int inputChannels, int outputChannels)
{
    cmsStage* mpe = (cmsStage*) std::malloc(sizeof(cmsStage));
    std::memset(mpe, 0, sizeof(cmsStage));

    mpe->InputChannels = (cmsUInt16Number)inputChannels;
    mpe->OutputChannels = (cmsUInt16Number)outputChannels;
    mpe->Data = (void*)clut;

    return mpe;
}

// Test 1: Normal path - all predicates pass and data is written as expected
static bool test_Type_MPEclut_Write_Normal()
{
    MockIO mock;
    initMockIO(mock);

    // Prepare CLUT with 4 entries and 4 input channels
    int inputChannels = 4;
    int outputChannels = 4;
    int nEntries = 4;
    _cmsStageCLutData* clut = createSampleCLUTData(inputChannels, nEntries);

    cmsStage* mpe = createStageWithCLUT(clut, inputChannels, outputChannels);

    // Call focal function
    cmsBool result = Type_MPEclut_Write(nullptr, &mock.io, mpe, 0);

    // Expected: success and 4 + 16 + (4 * 4) = 36 bytes written
    int expectedBytes = 4 /* two 16-bit numbers */ + 16 /* Dimensions8 write (16 bytes) */ + (nEntries * 4);
    bool ok = (result != 0) && (mock.data.size() == (size_t)expectedBytes);

    // Cleanup
    std::free(mpe);
    std::free(clut->Params);
    std::free(clut);

    if (!ok) {
        std::cerr << "[Test] test_Type_MPEclut_Write_Normal: failed. "
                  << "Result=" << (result != 0 ? "TRUE" : "FALSE")
                  << ", bytes_written=" << mock.data.size()
                  << ", expected=" << expectedBytes << "\n";
    }

    return ok;
}

// Test 2: False predicate - InputChannels > MAX_INPUT_DIMENSIONS should fail early
static bool test_Type_MPEclut_Write_InputChannelsTooHigh()
{
    MockIO mock;
    initMockIO(mock);

    // Prepare CLUT with a valid configuration
    int inputChannels = 999; // intentionally oversized
    int outputChannels = 4;
    int nEntries = 2;
    _cmsStageCLutData* clut = createSampleCLUTData(inputChannels, nEntries); // may still allocate; only InputChannels matters for mpe

    cmsStage* mpe = createStageWithCLUT(clut, inputChannels, outputChannels);
    // Intentionally set the input channels to a value known to be above MAX_INPUT_DIMENSIONS
    mpe->InputChannels = (cmsUInt16Number)inputChannels;

    cmsBool result = Type_MPEclut_Write(nullptr, &mock.io, mpe, 0);

    bool ok = (result == FALSE);
    // No data should have been written in this early exit
    if (mock.data.size() != 0) ok = false;

    // Cleanup
    std::free(mpe);
    std::free(clut->Params);
    std::free(clut);

    if (!ok) {
        std::cerr << "[Test] test_Type_MPEclut_Write_InputChannelsTooHigh: failed. "
                  << "Result=" << (result != 0 ? "TRUE" : "FALSE")
                  << ", bytes_written=" << mock.data.size() << "\n";
    }

    return ok;
}

// Test 3: False predicate - HasFloatValues == FALSE should fail
static bool test_Type_MPEclut_Write_HasFloatValuesFalse()
{
    MockIO mock;
    initMockIO(mock);

    int inputChannels = 3;
    int outputChannels = 3;
    int nEntries = 2;
    _cmsStageCLutData* clut = createSampleCLUTData(inputChannels, nEntries);
    // Force HasFloatValues to FALSE to exercise early rejection
    clut->HasFloatValues = FALSE;

    cmsStage* mpe = createStageWithCLUT(clut, inputChannels, outputChannels);

    cmsBool result = Type_MPEclut_Write(nullptr, &mock.io, mpe, 0);

    bool ok = (result == FALSE);
    if (mock.data.size() != 0) ok = false; // should not write anything

    // Cleanup
    std::free(mpe);
    std::free(clut->Params);
    std::free(clut);

    if (!ok) {
        std::cerr << "[Test] test_Type_MPEclut_Write_HasFloatValuesFalse: failed. "
                  << "Result=" << (result != 0 ? "TRUE" : "FALSE") << "\n";
    }

    return ok;
}

// Test 4: Simulated I/O failure during the final float entries write
static bool test_Type_MPEclut_Write_WriteFailure()
{
    MockIO mock;
    initMockIO(mock);
    mock.failOnWrite = 4; // fail on the 4th write call (first two 16-bit writes + 16-byte Dimensions8 = 3 writes, so 4th is first float write)

    int inputChannels = 4;
    int outputChannels = 4;
    int nEntries = 4;
    _cmsStageCLutData* clut = createSampleCLUTData(inputChannels, nEntries);
    clut->Params->nSamples[0] = 2;
    clut->Params->nSamples[1] = 2;
    clut->Params->nSamples[2] = 2;
    clut->Params->nSamples[3] = 2;

    cmsStage* mpe = createStageWithCLUT(clut, inputChannels, outputChannels);
    mpe->InputChannels = inputChannels;
    mpe->OutputChannels = outputChannels;

    cmsBool result = Type_MPEclut_Write(nullptr, &mock.io, mpe, 0);

    bool ok = (result == FALSE);
    // There may be partial writes before the failure; the test focuses on the function returning FALSE

    // Cleanup
    std::free(mpe);
    std::free(clut->Params);
    std::free(clut);

    if (!ok) {
        std::cerr << "[Test] test_Type_MPEclut_Write_WriteFailure: failed. "
                  << "Result=" << (result != 0 ? "TRUE" : "FALSE") << "\n";
    }

    return ok;
}

// Runner
int main()
{
    std::vector<TestResult> results;

    auto run = [&](const char* name, bool ok, const std::string& msg){
        TestResult r;
        r.name = name;
        r.ok = ok;
        r.message = msg;
        results.push_back(r);
        return ok;
    };

    // Execute tests
    bool r1 = test_Type_MPEclut_Write_Normal();
    run("test_Type_MPEclut_Write_Normal", r1,
        r1 ? "PASSED" : "FAILED");

    bool r2 = test_Type_MPEclut_Write_InputChannelsTooHigh();
    run("test_Type_MPEclut_Write_InputChannelsTooHigh", r2,
        r2 ? "PASSED" : "FAILED");

    bool r3 = test_Type_MPEclut_Write_HasFloatValuesFalse();
    run("test_Type_MPEclut_Write_HasFloatValuesFalse", r3,
        r3 ? "PASSED" : "FAILED");

    bool r4 = test_Type_MPEclut_Write_WriteFailure();
    run("test_Type_MPEclut_Write_WriteFailure", r4,
        r4 ? "PASSED" : "FAILED");

    // Print summary
    int failures = 0;
    for (const auto& t : results) {
        std::cout << t.name << ": " << (t.ok ? "PASSED" : "FAILED") 
                  << " - " << t.message << "\n";
        if (!t.ok) ++failures;
    }

    // Return number of failures as exit code (0 means all tests passed)
    return failures;
}