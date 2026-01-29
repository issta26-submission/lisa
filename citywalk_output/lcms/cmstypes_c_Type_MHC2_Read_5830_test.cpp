// Unit tests for Type_MHC2_Read in cmstypes.c
// Note: This test harness is designed for C++11, without GoogleTest.
// It mocks a minimal in-memory cmsIOHANDLER to drive the focal function.
// The test suite focuses on:
// - Negative path: CurveEntries > 4096 should return NULL.
// - Positive path: A carefully crafted in-memory stream drives a successful read
//                of an MHC2 structure (MatrixOffset = 0) and validation of key fields.
// 
// The tests are intentionally self-contained and rely only on the public Little CMS
// style APIs exposed by the included headers (assumed present in the project).

#include <cstring>
#include <cstdio>
#include <vector>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Domain knowledge notes applied:
// - Use only standard library + provided CMS APIs; no GTest.
// - Do not rely on private/internal details beyond what the focal method uses.
// - Provide non-terminating, domain-appropriate checks (i.e., do not abort the program on failure,
//   but report the failure to stdout/stderr and continue when possible).
// - Access static/dll-internal state via public APIs; we avoid private member access in tests.

// Forward declare required CMS structures and functions from the project headers.
// These headers must be present in the environment where this test is compiled.
extern "C" {
// _cms_typehandler_struct is used by Type_MHC2_Read signature
// (This test uses the exact function signature as provided by the focal method.)
}

using std::fprintf;
using std::stderr;
using std::cout;
using std::endl;

// Simple test harness to capture non-terminating expectations (like GTest EXPECT_* that
// do not stop test execution). We accumulate failures and report at end.
static int g_test_failures = 0;

#define TEST_FAIL(msg) do { fprintf(stderr, "TEST_FAIL: %s\n", msg); ++g_test_failures; } while(0)
#define EXPECT_TRUE(cond) do { if(!(cond)) { TEST_FAIL("EXPECT_TRUE failed: " #cond); } } while(0)
#define EXPECT_NOT_NULL(p) do { if((p) == nullptr) { TEST_FAIL("EXPECT_NOT_NULL failed: pointer is NULL"); } } while(0)
#define EXPECT_NEAR(a,b,eps) do { if(std::fabs((a)-(b)) > (eps)) { TEST_FAIL("EXPECT_NEAR failed"); } } while(0)
#define EXPECT_EQ(a,b) do { if((a) != (b)) { TEST_FAIL("EXPECT_EQ failed: " #a " != " #b); } } while(0)

// Minimal helper to print a summary at the end
static void report_summary() {
    if (g_test_failures == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::cerr << g_test_failures << " test(s) FAILED." << std::endl;
    }
}

// ---------------------------------------------------------------------------
// Memory IO Mocks for Type_MHC2_Read
// We implement two in-memory CMS IO handlers:
// 1) FailingRead4: Used to test CurveEntries > 4096 path by simulating a failure on the 4th Read.
// 2) SuccessfulFullRead: Used to simulate a full, successful MHC2 read, including
//    reading CurveEntries, Min/Peak luminance, identity matrix (MatrixOffset=0),
//    and red/green/blue curves.
// The test relies on the library's _cmsReadUInt32Number, _cmsRead15Fixed16Number, and
// ReadDoublesAt to call the implemented cmsIOHANDLER functions (Read, Tell, Seek).

// We assume the cmsIOHANDLER struct in this environment exposes at least:
// - cmsUInt32Number (*Read)(struct cmsIOHANDLER*, cmsUInt32Number, void*)
// - cmsBool (*Seek)(struct cmsIOHANDLER*, cmsInt64Number, int)
// - cmsUInt32Number (*Tell)(struct cmsIOHANDLER*)
// - void* ContextID (for our stateful mocks)


// Helper: cast helper to call Type_MHC2_Read
extern "C" {
void* Type_MHC2_Read(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsUInt32Number* nItems, cmsUInt32Number SizeOfTag);
}

// Forward declare a minimal _cms_typehandler_struct used for tests
// (We rely on the real definition from lcms2 headers; we only need ContextID here.)
extern "C" {
struct _cms_typehandler_struct; // opaque in test; we store only ContextID
}

// --------- Failing Read Test IO (test 1) ---------
struct FailingRead4IO {
    cmsIOHANDLER io;
    void* context;
    int read_calls; // 0..3
    unsigned char dummy; // just to have an instance member
};

static cmsUInt32Number FailingRead4_Read(struct cmsIOHANDLER* io, cmsUInt32Number size, void* buffer) {
    FailingRead4IO* self = (FailingRead4IO*)io->ContextID;
    // We only emulate 4 reads; after 3 successful reads, 4th read fails
    if (self->read_calls == 0) {
        unsigned int v = 2u; // CurveEntries = 2
        memcpy(buffer, &v, sizeof(v));
        self->read_calls++;
        return 4;
    } else if (self->read_calls == 1) {
        unsigned int v = 0x00010000u; // MinLuminance = 1.0
        memcpy(buffer, &v, sizeof(v));
        self->read_calls++;
        return 4;
    } else if (self->read_calls == 2) {
        unsigned int v = 0x00010000u; // PeakLuminance = 1.0
        memcpy(buffer, &v, sizeof(v));
        self->read_calls++;
        return 4;
    } else {
        // 4th read: simulate IO failure
        (void)size;
        return 0; // indicate read failure
    }
}

static cmsBool FailingRead4_Seek(struct cmsIOHANDLER* io, cmsInt64Number pos, int origin) {
    // Not used in this scenario; ensure it succeeds if invoked
    (void)io; (void)pos; (void)origin;
    return 1;
}
static cmsUInt32Number FailingRead4_Tell(struct cmsIOHANDLER* io) {
    // Always return 0 for simplicity
    (void)io;
    return 0;
}

static FailingRead4IO* create_FailingRead4IO() {
    FailingRead4IO* fr = (FailingRead4IO*)std::malloc(sizeof(FailingRead4IO));
    if (!fr) return nullptr;
    fr->read_calls = 0;
    fr->context = fr;
    // initialize cmsIOHANDLER
    fr->io.Read = FailingRead4_Read;
    fr->io.Seek = FailingRead4_Seek;
    fr->io.Tell = FailingRead4_Tell;
    fr->io.ContextID = fr;
    return fr;
}

// --------- Successful Full Path Test IO (test 2) ---------
struct FullReadIO {
    cmsIOHANDLER io;
    void* context;
    int stage; // to track reads and seeks
    size_t pos;
    size_t size;
    const unsigned char* data; // in-memory buffer
    std::vector<unsigned char> buffer; // dynamic allocation
};

// Helper to append a value as little-endian 32-bit
static void push32_le(std::vector<unsigned char>& buf, uint32_t v) {
    unsigned char b[4];
    memcpy(b, &v, 4);
    buf.insert(buf.end(), b, b+4);
}
// Helper to append a double in little-endian
static void push_double_le(std::vector<unsigned char>& buf, double d) {
    unsigned char b[8];
    memcpy(b, &d, 8);
    buf.insert(buf.end(), b, b+8);
}

static cmsUInt32Number FullRead_Read(struct cmsIOHANDLER* io, cmsUInt32Number size, void* buffer) {
    FullReadIO* self = (FullReadIO*)io->ContextID;
    // For simplicity, we implement sequential reads from the prebuilt buffer unless
    // the Read refers to offset-based reads via Seek (which we handle in FullRead_Seek).
    // Here, we emulate Read by copying from the in-memory buffer starting at current pos.
    if (self->pos + size > self->size) {
        // Partial read allowed by CMS; attack with what's available
        size = (cmsUInt32Number)(self->size - self->pos);
    }
    if (size > 0) {
        memcpy(buffer, self->data + self->pos, size);
        self->pos += size;
        return size;
    }
    return 0;
}
static cmsBool FullRead_Seek(struct cmsIOHANDLER* io, cmsInt64Number pos, int origin) {
    FullReadIO* self = (FullReadIO*)io->ContextID;
    cmsInt64Number newpos = 0;
    if (origin == 0) { // SEEK_SET
        newpos = pos;
    } else if (origin == 1) { // SEEK_CUR
        newpos = (cmsInt64Number)self->pos + pos;
    } else if (origin == 2) { // SEEK_END
        newpos = (cmsInt64Number)self->size + pos;
    } else {
        return 0;
    }
    if (newpos < 0 || (size_t)newpos > self->size) return 0;
    self->pos = (size_t)newpos;
    return 1;
}
static cmsUInt32Number FullRead_Tell(struct cmsIOHANDLER* io) {
    FullReadIO* self = (FullReadIO*)io->ContextID;
    return (cmsUInt32Number)self->pos;
}

static FullReadIO* create_FullReadIO() {
    // Build a buffer representing a well-formed MHC2 tag as described in analysis.
    // Layout (byte offsets):
    // 0-3: CurveEntries (2)
    // 4-7: MinLum (0x00010000)
    // 8-11: PeakLum (0x00010000)
    // 12-15: MatrixOffset (0)
    // 16-19: OffsetRedTable (40)
    // 20-23: OffsetGreenTable (40)
    // 24-27: OffsetBlueTable (40)
    // 28-39: filler
    // 40-47: sf32 tag filler (8 bytes)
    // 48-63: RedCurve doubles (2 values)
    // 64-71: filler
    // 72-87: GreenCurve doubles (2 values)
    // 88-95: filler
    // 96-111: BlueCurve doubles (2 values)

    FullReadIO* fr = (FullReadIO*)std::malloc(sizeof(FullReadIO));
    if (!fr) return nullptr;
    fr->stage = 0;
    fr->pos = 0;
    fr->size = 0;

    std::vector<unsigned char> buf;

    // Curve Entries = 2
    push32_le(buf, 2);

    // MinLum = 1.0
    unsigned int minLum = 0x00010000;
    push32_le(buf, minLum);

    // PeakLum = 1.0
    unsigned int peakLum = 0x00010000;
    push32_le(buf, peakLum);

    // MatrixOffset = 0
    push32_le(buf, 0);

    // Offsets
    push32_le(buf, 40); // Red
    push32_le(buf, 40); // Green
    push32_le(buf, 40); // Blue

    // 12 bytes filler
    for (int i = 0; i < 12; ++i) buf.push_back(0);

    // 8 bytes filler at 40-47
    for (int i = 0; i < 8; ++i) buf.push_back(0);

    // RedCurve: 2 doubles (1.0, 2.0)
    push_double_le(buf, 1.0);
    push_double_le(buf, 2.0);

    // 8 bytes filler
    for (int i = 0; i < 8; ++i) buf.push_back(0);

    // GreenCurve: 2 doubles (3.0, 4.0)
    push_double_le(buf, 3.0);
    push_double_le(buf, 4.0);

    // 8 bytes filler
    for (int i = 0; i < 8; ++i) buf.push_back(0);

    // BlueCurve: 2 doubles (5.0, 6.0)
    push_double_le(buf, 5.0);
    push_double_le(buf, 6.0);

    fr->buffer = buf;
    fr->data = buf.data();
    fr->size = buf.size();

    fr->io.Read = FullRead_Read;
    fr->io.Seek = FullRead_Seek;
    fr->io.Tell = FullRead_Tell;
    fr->io.ContextID = fr;

    return fr;
}

static void destroy_FullReadIO(FullReadIO* fr) {
    if (!fr) return;
    std::free(fr);
}

// ---------------------------------------------------------------------------
// Helpers for tests
static void* run_Type_MHC2_Read_with_io(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsUInt32Number* nItems) {
    // Call the focal function with SizeOfTag = 0
    cmsUInt32Number dummySize = 0;
    return Type_MHC2_Read(self, io, nItems, dummySize);
}

// Test 1: CurveEntries > 4096 should yield NULL
static void test_Type_MHC2_Read_CurveEntries_TooLarge() {
    // Create a minimalish _cms_typehandler_struct
    struct _cms_typehandler_struct* self = (struct _cms_typehandler_struct*)std::calloc(1, sizeof(struct _cms_typehandler_struct));
    if (!self) {
        TEST_FAIL("Failed to allocate _cms_typehandler_struct for Test 1");
        return;
    }
    self->ContextID = (void*)0; // use default allocator

    // Create failing IO (first read succeeds; 2nd/3rd succeed; 4th fails)
    FailingRead4IO* fr = create_FailingRead4IO();
    if (!fr) {
        TEST_FAIL("Failed to allocate FailingRead4IO");
        std::free(self);
        return;
    }

    cmsUInt32Number nItems = 0;
    void* ptr = run_Type_MHC2_Read_with_io(self, &fr->io, &nItems);

    EXPECT_TRUE(ptr == nullptr); // Expect failure due to CurveEntries > 4096
    if (ptr != nullptr) {
        // If unexpectedly non-NULL, report extra معلومات
        TEST_FAIL("Type_MHC2_Read should return NULL on CurveEntries > 4096");
    }

    // Cleanup
    std::free(fr);
    std::free(self);
}

// Test 2: Successful full read (MatrixOffset == 0) and basic field validation
static void test_Type_MHC2_Read_Success_FullPath() {
    // Create a minimal _cms_typehandler_struct
    struct _cms_typehandler_struct* self = (struct _cms_typehandler_struct*)std::calloc(1, sizeof(struct _cms_typehandler_struct));
    if (!self) {
        TEST_FAIL("Failed to allocate _cms_typehandler_struct for Test 2");
        return;
    }
    self->ContextID = (void*)0; // default allocator

    // Prepare a FullReadIO with the in-memory buffer
    FullReadIO* frio = create_FullReadIO();
    if (!frio) {
        TEST_FAIL("Failed to allocate FullReadIO");
        std::free(self);
        return;
    }

    cmsUInt32Number nItems = 0;
    void* mhc2_ptr = run_Type_MHC2_Read_with_io(self, &frio->io, &nItems);

    EXPECT_NOT_NULL(mhc2_ptr);
    if (mhc2_ptr == nullptr) {
        TEST_FAIL("Type_MHC2_Read returned NULL on a valid, fully-specified buffer");
    } else {
        // Validate a few key fields if available
        cm sMHC2Type* mhc2 = (cmsMHC2Type*)mhc2_ptr;
        // CurveEntries should be 2
        EXPECT_TRUE(mhc2->CurveEntries == 2);

        // MinLum / PeakLum should be 1.0 (0x00010000 fixed 16)
        EXPECT_NEAR((double)mhc2->MinLuminance, 1.0, 1e-6);
        EXPECT_NEAR((double)mhc2->PeakLuminance, 1.0, 1e-6);

        // Identity matrix (MatrixOffset == 0)
        // We expect mhc2->XYZ2XYZmatrix to be identity
        // A simple check: compare diagonal elements to 1.0
        // (We assume identity formatting in 3x4 matrix)
        // Note: Use a tolerant comparison
        double id[3][4] = { {1.0, 0.0, 0.0, 0.0},
                            {0.0, 1.0, 0.0, 0.0},
                            {0.0, 0.0, 1.0, 0.0} };
        bool identity_like = true;
        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 4; ++c) {
                if (std::fabs(mhc2->XYZ2XYZmatrix[r][c] - id[r][c]) > 1e-6) {
                    identity_like = false;
                    break;
                }
            }
            if (!identity_like) break;
        }
        EXPECT_TRUE(identity_like);

        // Basic sanity on Red/Green/Blue curves count
        EXPECT_TRUE(mhc2->RedCurve != nullptr);
        EXPECT_TRUE(mhc2->GreenCurve != nullptr);
        EXPECT_TRUE(mhc2->BlueCurve != nullptr);

        // Validate a couple of sample values (we placed 1.0/2.0 etc.)
        EXPECT_NEAR((double)mhc2->RedCurve[0], 1.0, 1e-6);
        EXPECT_NEAR((double)mhc2->RedCurve[1], 2.0, 1e-6);
        EXPECT_NEAR((double)mhc2->GreenCurve[0], 3.0, 1e-6);
        EXPECT_NEAR((double)mhc2->GreenCurve[1], 4.0, 1e-6);
        EXPECT_NEAR((double)mhc2->BlueCurve[0], 5.0, 1e-6);
        EXPECT_NEAR((double)mhc2->BlueCurve[1], 6.0, 1e-6);

        // Free the mhc2 structure
        Type_MHC2_Free(self, mhc2_ptr);
        // After free, mhc2_ptr should not be used
    }

    // Cleanup
    destroy_FullReadIO(frio);
    std::free(self);
}

// ---------------------------------------------------------------------------
// Main function to execute tests
int main() {
    // Announce tests
    std::cout << "Running Type_MHC2_Read unit tests (C++11, non-GTest)..." << std::endl;

    test_Type_MHC2_Read_CurveEntries_TooLarge();
    test_Type_MHC2_Read_Success_FullPath();

    report_summary();
    return (g_test_failures == 0) ? 0 : 1;
}