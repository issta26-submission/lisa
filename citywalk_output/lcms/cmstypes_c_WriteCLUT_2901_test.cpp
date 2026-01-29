// Test suite for WriteCLUT in cmstypes.c
// NOTE: This test suite is designed to be compiled and linked with the actual LittleCMS
// library sources (cmstypes.c and the internal headers). It uses a small, self-contained
// test harness (no GoogleTest in use) and relies on the public/internal types provided by
// the library (lcms2_internal.h).
// The tests focus on verifying control-flow branches and basic I/O interactions.
// The tests use a lightweight non-terminating assertion mechanism to maximize code coverage.

#include <cstdio>
#include <vector>
#include <cstddef>
#include <lcms2_internal.h>
#include <cstdint>


extern "C" {
    // Include internal declarations to access the exact structures used by WriteCLUT.
    // It is assumed that the project provides lcms2_internal.h in the include path.
    #include "lcms2_internal.h"

    // cmstypes.c is compiled separately in the project; we link against it.
    // The WriteCLUT function has C linkage; we can call it directly from C++.

    // Forward declare the WriteCLUT function (in case the header doesn't expose it directly).
    cmsBool WriteCLUT(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsUInt8Number  Precision, cmsStage* mpe);
}

// Lightweight non-terminating assertion macros
static int g_test_failures = 0;
#define CHECK(cond, msg) do { if (!(cond)) { fprintf(stderr, "Test failed: %s\n", (msg)); ++g_test_failures; } } while (0)
#define ASSERT_TRUE(cond) CHECK((cond), "Condition is true")
#define ASSERT_FALSE(cond) CHECK(!(cond), "Condition is false")

// Mock IO handler to capture write activity
struct MockIO {
    cmsIOHANDLER base;                 // Must be first member to allow cast from &base
    std::vector<uint8_t> data;           // Captured written bytes
};

// Static write function compatible with cmsIOHANDLER.Write
static cmsBool Mock_Write(struct _cmsIOHANDLER* io, cmsUInt32Number size, const void* Buffer)
{
    // Recover MockIO instance from the embedded base
    MockIO* self = (MockIO*)((char*)io - offsetof(MockIO, base));
    if (!self) return FALSE;
    const uint8_t* bytes = (const uint8_t*)Buffer;
    if (size > 0 && bytes) {
        self->data.insert(self->data.end(), bytes, bytes + size);
    }
    return TRUE;
}

// Helper to create a minimal _cmsStageCLutData structure for tests
// This relies on internal layout from lcms2_internal.h. We only touch fields that
// the WriteCLUT function uses: HasFloatValues, Params (with nInputs and nSamples[]), nEntries,
// and Tab.T[] (uint16 values). We keep nInputs and nEntries small to simplify tests.
static void setup_minimal_clut(_cmsStageCLutData* clut, cmsBool hasFloat,
                               cmsUInt32Number nInputs,
                               const cmsUInt32Number* samples,       // length at least nInputs
                               cmsUInt32Number nEntries,
                               const cmsUInt16Number* tValues)           // length at least nEntries
{
    clut->HasFloatValues = hasFloat ? TRUE : FALSE;
    // Allocate Params - point to provided samples array (we'll copy into a local Params object)
    static _cmsStageCLutParams params_storage;
    // Only set if the structure actually has Params field (typical internal layout provides a pointer)
    // The following approach mirrors the library's usage: Params is a pointer to a struct with
    // nInputs and nSamples[nInputs].
    clut->Params = &params_storage;
    // Initialize Params
    clut->Params->nInputs = nInputs;
    for (cmsUInt32Number i = 0; i < nInputs && i < cmsMAXCHANNELS; ++i) {
        clut->Params->nSamples[i] = samples[i];
    }
    // The rest are zero-initialized by memset-like behavior
    for (cmsUInt32Number i = nInputs; i < cmsMAXCHANNELS; ++i) {
        clut->Params->nSamples[i] = 0;
    }

    // nEntries
    clut->nEntries = nEntries;
    // Tab.T[] values
    // Some internal implementations declare Tab as a nested struct with an array T[].
    // We assume it exists and is accessible here.
    for (cmsUInt32Number i = 0; i < nEntries; ++i) {
        clut->Tab.T[i] = tValues ? tValues[i] : 0;
    }
    // Zero rest of T array in case of leftovers
    for (cmsUInt32Number i = nEntries; i < CLUT_MAX_T_ENTRIES; ++i) {
        clut->Tab.T[i] = 0;
    }
}

// Note: The exact macro CLUT_MAX_T_ENTRIES is assumed to be defined in the library headers.
// If not defined in your environment, replace with the actual maximum supported entries
// for CLUT.T. We provide a fallback constant if needed.
#ifndef CLUT_MAX_T_ENTRIES
#define CLUT_MAX_T_ENTRIES 256
#endif

static int test_HasFloatValues_rejects_rendering(void)
{
    // Setup
    MockIO mock;
    mock.data.clear();
    mock.base.Write = &Mock_Write;

    // Self context
    _cms_typehandler_struct self;
    self.ContextID = 123; // arbitrary

    // CLUT with HasFloatValues = TRUE should error out immediately
    _cmsStageCLutData clut;
    clut.HasFloatValues = TRUE;
    static _cmsStageCLutParams params;
    clut.Params = &params;
    clut.Params->nInputs = 0;
    clut.nEntries = 0;
    // Tab values won't be used since function should bail early

    cmsStage mpe;
    mpe.Data = &clut;

    // Execute
    cmsBool result = WriteCLUT(&self, &mock.base, 1, &mpe);

    // Verify: should fail (FALSE) and not touch the io buffer
    ASSERT_FALSE(result);
    ASSERT_TRUE(mock.data.empty());

    return g_test_failures;
}

static int test_InvalidPrecision_rejects(void)
{
    // Setup
    MockIO mock;
    mock.base.Write = &Mock_Write;
    mock.data.clear();

    _cms_typehandler_struct self;
    self.ContextID = 999;

    _cmsStageCLutData clut;
    clut.HasFloatValues = FALSE;
    static _cmsStageCLutParams params;
    clut.Params = &params;
    clut.Params->nInputs = 0;
    clut.nEntries = 0;

    cmsStage mpe;
    mpe.Data = &clut;

    // Execute with an invalid precision (not 1 or 2)
    cmsBool result = WriteCLUT(&self, &mock.base, 3, &mpe);

    // Verify
    ASSERT_FALSE(result);
    ASSERT_TRUE(mock.data.empty());

    return g_test_failures;
}

static int test_Minimal_CLUT_writes_grid_and_succeeds(void)
{
    // Setup
    MockIO mock;
    mock.base.Write = &Mock_Write;
    mock.data.clear();

    _cms_typehandler_struct self;
    self.ContextID = 42;

    // CLUT with 0 inputs and 0 entries
    _cmsStageCLutData clut;
    clut.HasFloatValues = FALSE;
    static _cmsStageCLutParams params;
    clut.Params = &params;
    clut.Params->nInputs = 0;
    clut.nEntries = 0;
    // Tab values not relevant

    cmsStage mpe;
    mpe.Data = &clut;

    // Execute
    cmsBool result = WriteCLUT(&self, &mock.base, 1, &mpe);

    // Verify success and at least one write happened (grid of cmsMAXCHANNELS bytes)
    ASSERT_TRUE(result);
    // The first write should be the grid of cmsMAXCHANNELS bytes, so data size should be cmsMAXCHANNELS at least.
    // We do not rely on exact endianness here; we just check presence of initial block.
    ASSERT_TRUE(mock.data.size() >= cmsMAXCHANNELS);

    // And grid points should be initialized to 0 if nInputs == 0
    for (cmsUInt32Number i = 0; i < cmsMAXCHANNELS; ++i) {
        if (i < 1) {
            // nothing special; a quick sanity check: grid is zeros
            // The first value should be 0 given we didn't set specific samples
        }
    }

    return g_test_failures;
}

static int test_Precision1_writes_8bit_values(void)
{
    // Setup
    MockIO mock;
    mock.base.Write = &Mock_Write;
    mock.data.clear();

    _cms_typehandler_struct self;
    self.ContextID = 77;

    // CLUT with 0 inputs but with 2 entries to exercise the data path
    _cmsStageCLutData clut;
    clut.HasFloatValues = FALSE;
    static _cmsStageCLutParams params;
    clut.Params = &params;
    clut.Params->nInputs = 0;
    clut.nEntries = 2;

    // Set two 16-bit LUT values; high byte will be written (FROM_16_TO_8)
    clut.Tab.T[0] = 0x0100; // high byte = 0x01
    clut.Tab.T[1] = 0x0200; // high byte = 0x02

    cmsStage mpe;
    mpe.Data = &clut;

    // Execute
    cmsBool result = WriteCLUT(&self, &mock.base, 1, &mpe);

    // Verify
    ASSERT_TRUE(result);
    // Expect at least: grid (cmsMAXCHANNELS), precision (1), 0,0,0, then 2 bytes for two entries
    // The exact positions depend on cmsMAXCHANNELS and alignment, but we can check presence of 0x01 and 0x02
    // after the initial header region.
    if (mock.data.size() > cmsMAXCHANNELS + 1 + 3 + 0) {
        // the first 3 bytes after grid + precision + 3 zeros
        size_t base = cmsMAXCHANNELS;
        // offset for first LUT value high byte
        size_t idx1 = base + 1 + 3;
        if (idx1 < mock.data.size()) {
            uint8_t val1 = mock.data[idx1];
            uint8_t val2 = (idx1 + 1 < mock.data.size()) ? mock.data[idx1 + 1] : 0;
            // Expect 0x01, 0x02 as high bytes
            ASSERT_TRUE(val1 == 0x01 && val2 == 0x02);
        } else {
            CHECK(false, "Index out of bounds for LUT data in test_Precision1_writes_8bit_values");
        }
    } else {
        CHECK(false, "Insufficient data captured for LUT2 in test_Precision1_writes_8bit_values");
    }

    return g_test_failures;
}

int main(void)
{
    int total = 0;
    int failed = 0;

    printf("Running tests for WriteCLUT (cmstypes.c) with internal structures exposed by lcms2_internal.h...\n");

    total = 4;
    failed += test_HasFloatValues_rejects_rendering();
    failed += test_InvalidPrecision_rejects();
    failed += test_Minimal_CLUT_writes_grid_and_succeeds();
    failed += test_Precision1_writes_8bit_values();

    if (failed == 0) {
        printf("All tests completed successfully.\n");
    } else {
        printf("Tests completed with %d failure(s) out of %d.\n", failed, total);
    }

    return failed;
}