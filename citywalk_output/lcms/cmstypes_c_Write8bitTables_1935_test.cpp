// Test suite for Write8bitTables in cmstypes.c
// This test suite is written in C++11 (no Google Test) and uses the internal
// CMS structures to build representative inputs for Write8bitTables.
// It relies on the internal lcms2_internal.h definitions to access the
// _cmsStageToneCurvesData and cmsToneCurve structures.

#include <cstring>
#include <vector>
#include <iostream>
#include <cassert>
#include <lcms2_internal.h>
#include <lcms2.h>


// Import CMS internal definitions and API
extern "C" {
}

// Prototypes (provide explicit extern to avoid linkage surprises)
extern "C" cmsBool Write8bitTables(cmsContext ContextID, cmsIOHANDLER* io, cmsUInt32Number n, _cmsStageToneCurvesData* Tables);

// Global write capture buffer for the fake IO
static std::vector<cmsUInt8Number> g_Written;

// Simple non-terminating test macros
static int gFailures = 0;
#define EXPECT_TRUE(cond) do { if(!(cond)) { \
    std::cerr << "EXPECT_TRUE failed at " __FILE__ ":" << __LINE__ << " : " #cond "\n"; \
    ++gFailures; } } while(0)

#define EXPECT_FALSE(cond) do { if((cond)) { \
    std::cerr << "EXPECT_FALSE failed at " __FILE__ ":" << __LINE__ << " : " #cond "\n"; \
    ++gFailures; } } while(0)

#define EXPECT_EQ(a,b) do { if( (a) != (b) ) { \
    std::cerr << "EXPECT_EQ failed at " __FILE__ ":" << __LINE__ \
              << " : " << (a) << " != " << (b) << "\n"; \
    ++gFailures; } } while(0)

#define EXPECT_GE(a,b) do { if( (a) < (b) ) { \
    std::cerr << "EXPECT_GE failed at " __FILE__ ":" << __LINE__ \
              << " : " << (a) << " < " << (b) << "\n"; \
    ++gFailures; } } while(0)

// Fake IO Write callback
// We assume the internal Write callback signature is:
static cmsBool FakeWrite(struct _cmsIOHANDLER_struct* io, cmsUInt32Number n, const cmsUInt8Number* Buffer)
{
    // Append n bytes from Buffer to global buffer
    if (Buffer && n > 0) {
        g_Written.insert(g_Written.end(), Buffer, Buffer + n);
    }
    return TRUE;
}

// Helper to reset the global written buffer
static void ResetWrittenBuffer() {
    g_Written.clear();
}

// Helper to build a single curve with 2-entry identity (Table16[0]=0, Table16[1]=65535)
static cmsToneCurve* BuildIdentityCurve() {
    cmsToneCurve* c = new cmsToneCurve;
    // The internal cmsToneCurve presumably has nEntries and Table16[256].
    c->nEntries = 2;
    c->Table16[0] = 0;
    c->Table16[1] = 65535;
    // Leave other entries uninitialized since nEntries=2 for identity case
    return c;
}

// Helper to build a 256-entry curve with Table16[j] = j*257 (so FROM_16_TO_8 maps to j)
static cmsToneCurve* Build256EntryCurve() {
    cmsToneCurve* c = new cmsToneCurve;
    c->nEntries = 256;
    for (int j = 0; j < 256; ++j) {
        c->Table16[j] = (cmsUInt16Number)(j * 257); // ensures mapping to 0..255
    }
    return c;
}

// Helper to clean up a _cmsStageToneCurvesData structure
static void FreeStageToneCurves(_cmsStageToneCurvesData* Tables)
{
    if (!Tables) return;
    if (Tables->TheCurves) {
        // TheCurves is a NULL-terminated array of pointers; we allocated individually
        for (cmsUInt32Number i = 0; i < Tables->Count; ++i) {
            delete Tables->TheCurves[i];
        }
        delete[] Tables->TheCurves;
        Tables->TheCurves = NULL;
        Tables->Count = 0;
    }
}

// Test 1: Identity curve with nEntries == 2 should write 256 bytes: 0..255
static void test_identity_two_entries_single_curve() {
    // Prepare IO
    cmsIOHANDLER io;
    std::memset(&io, 0, sizeof(io));
    io.Write = FakeWrite;
    io.Context = NULL;

    // Prepare Tables with one identity curve
    _cmsStageToneCurvesData Tables;
    Tables.TheCurves = new cmsToneCurve*[1];
    Tables.TheCurves[0] = BuildIdentityCurve();
    Tables.Count = 1;

    ResetWrittenBuffer();
    cmsBool ok = Write8bitTables(0, &io, 1, &Tables);

    EXPECT_TRUE(ok == TRUE);
    EXPECT_EQ((cmsUInt32Number)g_Written.size(), (cmsUInt32Number)256);
    for (int i = 0; i < 256; ++i) {
        EXPECT_EQ((cmsUInt32Number)g_Written[i], (cmsUInt8Number)i);
    }

    // Cleanup
    FreeStageToneCurves(&Tables);
    delete[] Tables.TheCurves;
    // TheCurves array elements freed by FreeStageToneCurves
}

// Test 2: Non-identity 256-entry curve should also write 256 bytes: 0..255
static void test_256_entries_curve_mapping() {
    // Prepare IO
    cmsIOHANDLER io;
    std::memset(&io, 0, sizeof(io));
    io.Write = FakeWrite;
    io.Context = NULL;

    // Prepare Tables with one 256-entry curve
    _cmsStageToneCurvesData Tables;
    Tables.TheCurves = new cmsToneCurve*[1];
    Tables.TheCurves[0] = Build256EntryCurve();
    Tables.Count = 1;

    ResetWrittenBuffer();
    cmsBool ok = Write8bitTables(0, &io, 1, &Tables);

    EXPECT_TRUE(ok == TRUE);
    EXPECT_EQ((cmsUInt32Number)g_Written.size(), (cmsUInt32Number)256);
    for (int i = 0; i < 256; ++i) {
        EXPECT_EQ((cmsUInt32Number)g_Written[i], (cmsUInt8Number)i);
    }

    // Cleanup
    FreeStageToneCurves(&Tables);
    delete[] Tables.TheCurves;
}

// Test 3: Multiple curves: first identity, second 256-entry curve
static void test_multiple_curves_identity_then_256() {
    // Prepare IO
    cmsIOHANDLER io;
    std::memset(&io, 0, sizeof(io));
    io.Write = FakeWrite;
    io.Context = NULL;

    // Prepare Tables with two curves
    _cmsStageToneCurvesData Tables;
    Tables.TheCurves = new cmsToneCurve*[2];

    // Curve 0: identity
    Tables.TheCurves[0] = BuildIdentityCurve();
    // Curve 1: 256-entry
    Tables.TheCurves[1] = Build256EntryCurve();
    Tables.Count = 2;

    ResetWrittenBuffer();
    cmsBool ok = Write8bitTables(0, &io, 2, &Tables);

    EXPECT_TRUE(ok == TRUE);
    EXPECT_EQ((cmsUInt32Number)g_Written.size(), (cmsUInt32Number)512);
    // First 256 bytes 0..255
    for (int i = 0; i < 256; ++i) {
        EXPECT_EQ((cmsUInt32Number)g_Written[i], (cmsUInt8Number)i);
    }
    // Next 256 bytes also 0..255
    for (int i = 0; i < 256; ++i) {
        EXPECT_EQ((cmsUInt32Number)g_Written[256 + i], (cmsUInt8Number)i);
    }

    // Cleanup
    FreeStageToneCurves(&Tables);
    delete[] Tables.TheCurves;
}

// Test 4: NULL Tables pointer should result in TRUE with no writes
static void test_null_tables_pointer() {
    // Prepare IO
    cmsIOHANDLER io;
    std::memset(&io, 0, sizeof(io));
    io.Write = FakeWrite;
    io.Context = NULL;

    ResetWrittenBuffer();
    cmsBool ok = Write8bitTables(0, &io, 1, NULL);

    EXPECT_TRUE(ok == TRUE);
    EXPECT_EQ((cmsUInt32Number)g_Written.size(), (cmsUInt32Number)0);
}

// Test 5: Error branch: a curve with nEntries not equal to 256 and not identity should fail
static void test_error_on_non_256_entries() {
    // Prepare IO (Write will not be called due to early error, but supply a buffer)
    cmsIOHANDLER io;
    std::memset(&io, 0, sizeof(io));
    io.Write = FakeWrite;
    io.Context = NULL;

    // Build non-256-entry curve (e.g., 128 entries)
    cmsToneCurve* c = new cmsToneCurve;
    c->nEntries = 128;
    // Populate first 128 16-bit values to be safe (mapping won't be used due to early error)
    for (int j = 0; j < 128; ++j) {
        c->Table16[j] = (cmsUInt16Number)(j);
    }

    _cmsStageToneCurvesData Tables;
    Tables.TheCurves = new cmsToneCurve*[1];
    Tables.TheCurves[0] = c;
    Tables.Count = 1;

    ResetWrittenBuffer();
    cmsBool ok = Write8bitTables(0, &io, 1, &Tables);

    // Expect failure (FALSE)
    EXPECT_FALSE(ok == TRUE);

    // No writes should have occurred
    EXPECT_EQ((cmsUInt32Number)g_Written.size(), (cmsUInt32Number)0);

    // Cleanup
    FreeStageToneCurves(&Tables);
    delete[] Tables.TheCurves;
}

// Main runner
int main() {
    // Run tests
    test_identity_two_entries_single_curve();
    test_256_entries_curve_mapping();
    test_multiple_curves_identity_then_256();
    test_null_tables_pointer();
    test_error_on_non_256_entries();

    if (gFailures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << gFailures << " test(s) failed.\n";
        return 1;
    }
}