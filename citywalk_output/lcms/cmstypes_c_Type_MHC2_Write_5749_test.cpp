// Unit test suite for cmsBool Type_MHC2_Write (re-implemented in this test harness)
// This file provides a self-contained test environment (no GTest) suitable for
// compiling with C++11. It mocks the minimal dependencies required by Type_MHC2_Write
// and verifies both identity and non-identity matrix handling as well as a failure path.

// Notes:
// - This is a focused test harness not tied to the real cms library binaries.
// - We implement a lightweight in-memory I/O (cmsIOHANDLER) and logging to verify
//   the exact sequence and values written by Type_MHC2_Write.
// - Tests are designed to maximize branch coverage (identity vs non-identity; failure path).
// - All tests run from main(); non-terminating checks report failures but do not abort.

#include <cstring>
#include <cstdint>
#include <vector>
#include <cstddef>
#include <iostream>
#include <string>
#include <cassert>
#include <lcms2_internal.h>
#include <cmath>


// Domain-specific typedefs and mocks to replicate necessary CMS types
typedef int cmsBool;
#define TRUE 1
#define FALSE 0

typedef uint32_t cmsUInt32Number;
typedef int32_t  cmsInt32Number;
typedef double   cmsFloat64Number;

// Forward declarations of minimal structures used by Type_MHC2_Write
struct _cms_typehandler_struct;
struct _cmsTagBase { int dummy; };
struct cmsIOHANDLER {
    cmsUInt32Number (*Tell)(cmsIOHANDLER* io);
    cmsBool (*Seek)(cmsIOHANDLER* io, cmsUInt32Number pos);
    cmsBool (*Write)(cmsIOHANDLER* io, const void* data, cmsUInt32Number length);
    void* data;
};

// Definition of MHC2 type used by the focal method
typedef struct {
    cmsUInt32Number CurveEntries;
    cmsFloat64Number MinLuminance;
    cmsFloat64Number PeakLuminance;
    cmsFloat64Number XYZ2XYZmatrix[3][4];
    cmsFloat64Number* RedCurve;
    cmsFloat64Number* GreenCurve;
    cmsFloat64Number* BlueCurve;
} cmsMHC2Type;

// Predictor signature used by Type_MHC2_Write
typedef struct _cms_typehandler_struct cmsTypeHandlerStruct;

// Forward declaration of Type_MHC2_Write (we implement it in this test file)
extern "C" {
    typedef cmsBool (*PositionTableEntryFn)(void);
    cmsBool Type_MHC2_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems);
}

// Constant used by write for S15Fixed16ArrayType
static const cmsUInt32Number cmsSigS15Fixed16ArrayType = 0x53463146; // 'SF16'(placeholder)

// Utility: compare doubles with tolerance
static bool almost_equal(double a, double b, double tol = 1e-9) {
    return std::abs(a - b) <= tol;
}

// Identify whether an XYZ matrix is identity (3x4: 3 rows, 4 columns)
static bool IsIdentity(cmsFloat64Number XYZ2XYZmatrix[3][4])
{
    return (XYZ2XYZmatrix[0][0] == 1.0 && XYZ2XYZmatrix[0][1] == 0.0 && XYZ2XYZmatrix[0][2] == 0.0 && XYZ2XYZmatrix[0][3] == 0.0 &&
            XYZ2XYZmatrix[1][0] == 0.0 && XYZ2XYZmatrix[1][1] == 1.0 && XYZ2XYZmatrix[1][2] == 0.0 && XYZ2XYZmatrix[1][3] == 0.0 &&
            XYZ2XYZmatrix[2][0] == 0.0 && XYZ2XYZmatrix[2][1] == 0.0 && XYZ2XYZmatrix[2][2] == 1.0 && XYZ2XYZmatrix[2][3] == 0.0);
}

// Forward declarations for internal helpers used by Type_MHC2_Write
static cmsUInt32Number WriteDoublesFlag;
static cmsBool _cmsWriteUInt32Number(cmsIOHANDLER* io, cmsUInt32Number n);
static cmsBool _cmsWrite15Fixed16Number(cmsIOHANDLER* io, cmsFloat64Number v);
static cmsBool WriteDoubles(cmsIOHANDLER* io, cmsUInt32Number n, cmsFloat64Number* Values);
static cmsBool ioMakeIdentity(); // placeholder for potential future uses

// Core Type_MHC2_Write implementation (as in the focal code)
cmsBool Type_MHC2_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems)
{
    (void)self;
    cmsMHC2Type* mhc2 = (cmsMHC2Type*)Ptr;
    cmsUInt32Number BaseOffset = io->Tell(io) - sizeof(_cmsTagBase);
    cmsUInt32Number TablesOffsetPos;
    cmsUInt32Number MatrixOffset;
    cmsUInt32Number OffsetRedTable, OffsetGreenTable, OffsetBlueTable;

    if (!_cmsWriteUInt32Number(io, mhc2->CurveEntries)) return FALSE;
    if (!_cmsWrite15Fixed16Number(io, mhc2->MinLuminance)) return FALSE;
    if (!_cmsWrite15Fixed16Number(io, mhc2->PeakLuminance)) return FALSE;
    TablesOffsetPos = io->Tell(io);
    // Matrix/Curve R/G/B table placeholders
    if (!_cmsWriteUInt32Number(io, 0)) return FALSE;    // Matrix
    if (!_cmsWriteUInt32Number(io, 0)) return FALSE;    // Curve R
    if (!_cmsWriteUInt32Number(io, 0)) return FALSE;    // Curve G
    if (!_cmsWriteUInt32Number(io, 0)) return FALSE;    // Curve B

    if (IsIdentity(mhc2->XYZ2XYZmatrix))
    {
        MatrixOffset = 0;
    }
    else
    {
        MatrixOffset = io->Tell(io) - BaseOffset;
        if (!WriteDoubles(io, 3 * 4, &mhc2->XYZ2XYZmatrix[0][0])) return FALSE;
    }

    OffsetRedTable = io->Tell(io) - BaseOffset;
    if(!_cmsWriteUInt32Number(io, cmsSigS15Fixed16ArrayType)) return FALSE;
    if(!_cmsWriteUInt32Number(io, 0)) return FALSE;
    if (!WriteDoubles(io, mhc2->CurveEntries, mhc2->RedCurve)) return FALSE;

    OffsetGreenTable = io->Tell(io) - BaseOffset;
    if (!_cmsWriteUInt32Number(io, cmsSigS15Fixed16ArrayType)) return FALSE;
    if (!_cmsWriteUInt32Number(io, 0)) return FALSE;
    if (!WriteDoubles(io, mhc2->CurveEntries, mhc2->GreenCurve)) return FALSE;

    OffsetBlueTable = io->Tell(io) - BaseOffset;
    if (!_cmsWriteUInt32Number(io, cmsSigS15Fixed16ArrayType)) return FALSE;
    if (!_cmsWriteUInt32Number(io, 0)) return FALSE;
    if (!WriteDoubles(io, mhc2->CurveEntries, mhc2->BlueCurve)) return FALSE;

    if (!io->Seek(io, TablesOffsetPos)) return FALSE;
    if (!_cmsWriteUInt32Number(io, MatrixOffset)) return FALSE;
    if (!_cmsWriteUInt32Number(io, OffsetRedTable)) return FALSE;
    if (!_cmsWriteUInt32Number(io, OffsetGreenTable)) return FALSE;
    if (!_cmsWriteUInt32Number(io, OffsetBlueTable)) return FALSE;

    return TRUE;
    // cmsUNUSED_PARAMETER(self);
    // cmsUNUSED_PARAMETER(nItems);
}

// Minimal implementations of helper functions (used by Type_MHC2_Write)
static cmsBool _cmsWriteUInt32Number(cmsIOHANDLER* io, cmsUInt32Number n)
{
    // Log the value; write as little-endian 4 bytes into the buffer
    IORecorder* rec = (IORecorder*)io->data;
    // Optional: simulate a controlled failure for test case
    if (rec->failFirstUInt32 && rec->uint32_logs.empty()) {
        return FALSE;
    }
    uint8_t b[4];
    b[0] = (uint8_t)(n & 0xFF);
    b[1] = (uint8_t)((n >> 8) & 0xFF);
    b[2] = (uint8_t)((n >> 16) & 0xFF);
    b[3] = (uint8_t)((n >> 24) & 0xFF);
    if (io->Write(io, b, 4) == FALSE) return FALSE;
    rec->uint32_logs.push_back(n);
    return TRUE;
}

static cmsBool _cmsWrite15Fixed16Number(cmsIOHANDLER* io, cmsFloat64Number v)
{
    // Convert to 16.16 fixed-point (S15Fixed16)
    int32_t fixed = (int32_t)std::round(v * 65536.0);
    uint8_t b[4];
    b[0] = (uint8_t)(fixed & 0xFF);
    b[1] = (uint8_t)((fixed >> 8) & 0xFF);
    b[2] = (uint8_t)((fixed >> 16) & 0xFF);
    b[3] = (uint8_t)((fixed >> 24) & 0xFF);
    IORecorder* rec = (IORecorder*)io->data;
    if (io->Write(io, b, 4) == FALSE) return FALSE;
    rec->uint32_logs.push_back((uint32_t)fixed); // keep alignment with how tests inspect
    return TRUE;
}

static cmsBool WriteDoubles(cmsIOHANDLER* io, cmsUInt32Number n, cmsFloat64Number* Values)
{
    IORecorder* rec = (IORecorder*)io->data;
    // Log the doubles as one block
    std::vector<double> block;
    block.reserve(n);
    for (cmsUInt32Number i = 0; i < n; ++i) {
        block.push_back(Values[i]);
    }
    rec->doubles_logs.push_back(block);

    // Serialize raw bytes to simulate actual IO
    for (cmsUInt32Number i = 0; i < n; ++i) {
        cmsFloat64Number val = Values[i];
        uint8_t* p = reinterpret_cast<uint8_t*>(&val);
        if (io->Write(io, p, sizeof(cmsFloat64Number)) == FALSE) return FALSE;
    }
    return TRUE;
}

// IO helper struct for test harness
struct IORecorder {
    size_t pos = 0;
    std::vector<uint8_t> buffer;
    std::vector<uint32_t> uint32_logs;
    std::vector<std::vector<double>> doubles_logs;
    bool failFirstUInt32 = false;
};

// IO function implementations for the test harness
static cmsUInt32Number TellFn(cmsIOHANDLER* io)
{
    IORecorder* rec = (IORecorder*)io->data;
    return (cmsUInt32Number)rec->pos;
}
static cmsBool SeekFn(cmsIOHANDLER* io, cmsUInt32Number pos)
{
    IORecorder* rec = (IORecorder*)io->data;
    rec->pos = (size_t)pos;
    // In this mock, we do not adjust the actual buffer pointer; tests rely on logs
    return TRUE;
}
static cmsBool WriteFn(cmsIOHANDLER* io, const void* data, cmsUInt32Number length)
{
    IORecorder* rec = (IORecorder*)io->data;
    const uint8_t* p = static_cast<const uint8_t*>(data);
    rec->buffer.insert(rec->buffer.end(), p, p + length);
    rec->pos += length;
    return TRUE;
}

// Test harness helpers
static void test_identity_matrix();
static void test_non_identity_matrix();
static void test_failure_path();

static void CHECK(bool cond, const char* msg)
{
    if (!cond) {
        std::cerr << "Test FAILURE: " << msg << std::endl;
    }
}

// Implement the identity test
static void test_identity_matrix()
{
    // Setup MHc2 with identity matrix and simple color curves
    cmsMHC2Type mhc2;
    mhc2.CurveEntries = 2;
    mhc2.MinLuminance = 0.5;   // 0.5 in fixed16 => 32768
    mhc2.PeakLuminance = 1.0;  // 1.0 in fixed16 => 65536

    // Identity matrix
    mhc2.XYZ2XYZmatrix[0][0] = 1.0; mhc2.XYZ2XYZmatrix[0][1] = 0.0; mhc2.XYZ2XYZmatrix[0][2] = 0.0; mhc2.XYZ2XYZmatrix[0][3] = 0.0;
    mhc2.XYZ2XYZmatrix[1][0] = 0.0; mhc2.XYZ2XYZmatrix[1][1] = 1.0; mhc2.XYZ2XYZmatrix[1][2] = 0.0; mhc2.XYZ2XYZmatrix[1][3] = 0.0;
    mhc2.XYZ2XYZmatrix[2][0] = 0.0; mhc2.XYZ2XYZmatrix[2][1] = 0.0; mhc2.XYZ2XYZmatrix[2][2] = 1.0; mhc2.XYZ2XYZmatrix[2][3] = 0.0;

    // Red/Green/Blue curves
    double red[2]   = {1.0, 2.0};
    double green[2] = {0.5, 1.5};
    double blue[2]  = {2.0, 3.0};
    mhc2.RedCurve   = red;
    mhc2.GreenCurve = green;
    mhc2.BlueCurve  = blue;

    // Prepare IO
    IORecorder rec;
    cmsIOHANDLER io;
    io.Tell = TellFn;
    io.Seek = SeekFn;
    io.Write = WriteFn;
    io.data = &rec;

    // Call the focal method
    cmsBool result = Type_MHC2_Write(nullptr, &io, &mhc2, 1);

    // Assertions
    CHECK(result == TRUE, "Type_MHC2_Write should return TRUE for identity path");
    // Inspect first three 32-bit writes
    // CurveEntries, MinLuminance (fixed16 0.5), PeakLuminance (fixed16 1.0)
    if (rec.uint32_logs.size() < 3) {
        std::cerr << "Insufficient uint32 logs for identity test" << std::endl;
    } else {
        CHECK(rec.uint32_logs[0] == 2, "CurveEntries should be written first as 2");
        CHECK(rec.uint32_logs[1] == 32768, "MinLuminance fixed16 should be 32768 for 0.5");
        CHECK(rec.uint32_logs[2] == 65536, "PeakLuminance fixed16 should be 65536 for 1.0");
    }

    // The 4th to 7th entries should be zeros (Matrix placeholder and 2 curve header placeholders)
    if (rec.uint32_logs.size() >= 7) {
        CHECK(rec.uint32_logs[3] == 0, "Matrix placeholder should be 0");
        CHECK(rec.uint32_logs[4] == 0, "Curve header placeholder should be 0");
        CHECK(rec.uint32_logs[5] == 0, "Curve header placeholder should be 0");
        CHECK(rec.uint32_logs[6] == 0, "Curve header placeholder should be 0");
    }

    // The 8th entry should be the array type marker
    if (rec.uint32_logs.size() >= 8) {
        CHECK(rec.uint32_logs[7] == cmsSigS15Fixed16ArrayType, "RedCurve array type marker should match cmsSigS15Fixed16ArrayType");
    }

    // In identity matrix case, the first of final 4 offset numbers should be 0
    if (rec.uint32_logs.size() >= 17) {
        size_t total = rec.uint32_logs.size();
        CHECK(rec.uint32_logs[total - 4] == 0, "MatrixOffset should be 0 in identity path");
    }

    // Doubles: 3 WriteDoubles calls (red, green, blue) each with 2 numbers
    CHECK(rec.doubles_logs.size() == 3, "Expected 3 WriteDoubles blocks for 3 color curves");
    if (rec.doubles_logs.size() == 3) {
        // Red
        CHECK(almost_equal(rec.doubles_logs[0][0], red[0]), "RedCurve first value mismatch");
        CHECK(almost_equal(rec.doubles_logs[0][1], red[1]), "RedCurve second value mismatch");
        // Green
        CHECK(almost_equal(rec.doubles_logs[1][0], green[0]), "GreenCurve first value mismatch");
        CHECK(almost_equal(rec.doubles_logs[1][1], green[1]), "GreenCurve second value mismatch");
        // Blue
        CHECK(almost_equal(rec.doubles_logs[2][0], blue[0]), "BlueCurve first value mismatch");
        CHECK(almost_equal(rec.doubles_logs[2][1], blue[1]), "BlueCurve second value mismatch");
    }

    // footer: ensure matrices in identity path have zero MatrixOffset in the final table
    if (rec.uint32_logs.size() >= 4) {
        size_t total = rec.uint32_logs.size();
        CHECK(rec.uint32_logs[total - 4] == 0, "Final offset MatrixOffset should be 0 for identity");
    }
}

// Implement the non-identity test
static void test_non_identity_matrix()
{
    // Setup MHc2 with non-identity matrix
    cmsMHC2Type mhc2;
    mhc2.CurveEntries = 2;
    mhc2.MinLuminance = 0.25;  // 0.25 -> 16384
    mhc2.PeakLuminance = 1.0;

    // Non-identity matrix
    mhc2.XYZ2XYZmatrix[0][0] = 1.0; mhc2.XYZ2XYZmatrix[0][1] = 0.0; mhc2.XYZ2XYZmatrix[0][2] = 0.0; mhc2.XYZ2XYZmatrix[0][3] = 0.1;
    mhc2.XYZ2XYZmatrix[1][0] = 0.0; mhc2.XYZ2XYZmatrix[1][1] = 1.0; mhc2.XYZ2XYZmatrix[1][2] = 0.0; mhc2.XYZ2XYZmatrix[1][3] = 0.2;
    mhc2.XYZ2XYZmatrix[2][0] = 0.0; mhc2.XYZ2XYZmatrix[2][1] = 0.0; mhc2.XYZ2XYZmatrix[2][2] = 1.0; mhc2.XYZ2XYZmatrix[2][3] = 0.3;

    // Red/Green/Blue curves
    double red[2]   = {1.0, 2.0};
    double green[2] = {0.5, 1.5};
    double blue[2]  = {2.0, 3.0};
    mhc2.RedCurve   = red;
    mhc2.GreenCurve = green;
    mhc2.BlueCurve  = blue;

    // Prepare IO
    IORecorder rec;
    cmsIOHANDLER io;
    io.Tell = TellFn;
    io.Seek = SeekFn;
    io.Write = WriteFn;
    io.data = &rec;

    // Call the focal method
    cmsBool result = Type_MHC2_Write(nullptr, &io, &mhc2, 1);

    // Assertions
    CHECK(result == TRUE, "Type_MHC2_Write should return TRUE for non-identity path");

    // First 3 writes: CurveEntries, MinLuminance, PeakLuminance
    if (rec.uint32_logs.size() < 3) {
        std::cerr << "Insufficient uint32 logs for non-identity test" << std::endl;
    } else {
        CHECK(rec.uint32_logs[0] == 2, "CurveEntries should be 2 in non-identity test");
        CHECK(rec.uint32_logs[1] == 16384, "MinLuminance fixed16 should be 16384 for 0.25");
        CHECK(rec.uint32_logs[2] == 65536, "PeakLuminance fixed16 should be 65536 for 1.0");
    }

    // The 4th to 7th entries are placeholders (zeros)
    if (rec.uint32_logs.size() >= 7) {
        CHECK(rec.uint32_logs[3] == 0);
        CHECK(rec.uint32_logs[4] == 0);
        CHECK(rec.uint32_logs[5] == 0);
        CHECK(rec.uint32_logs[6] == 0);
    }

    // The 8th entry should be the RedCurve type marker
    if (rec.uint32_logs.size() >= 8) {
        CHECK(rec.uint32_logs[7] == cmsSigS15Fixed16ArrayType);
    }

    // Non-identity case should produce 4 blocks of doubles:
    // 1 matrix (12 values) + 3 color curves (2 values each) -> total 4 blocks
    CHECK(rec.doubles_logs.size() == 4, "Doubles blocks should be 4 (matrix + 3 curves)");
    if (rec.doubles_logs.size() == 4) {
        // First block: matrix 12 numbers
        CHECK(rec.doubles_logs[0].size() == 12);
        double expectedM[12] = {
            1.0,0.0,0.0,0.1,
            0.0,1.0,0.0,0.2,
            0.0,0.0,1.0,0.3
        };
        for (int i = 0; i < 12; ++i) {
            CHECK(almost_equal(rec.doubles_logs[0][i], expectedM[i]));
        }
        // Red
        CHECK(rec.doubles_logs[1].size() == 2);
        CHECK(almost_equal(rec.doubles_logs[1][0], red[0]));
        CHECK(almost_equal(rec.doubles_logs[1][1], red[1]));
        // Green
        CHECK(rec.doubles_logs[2].size() == 2);
        CHECK(almost_equal(rec.doubles_logs[2][0], green[0]));
        CHECK(almost_equal(rec.doubles_logs[2][1], green[1]));
        // Blue
        CHECK(rec.doubles_logs[3].size() == 2);
        CHECK(almost_equal(rec.doubles_logs[3][0], blue[0]));
        CHECK(almost_equal(rec.doubles_logs[3][1], blue[1]));
    }

    // Final matrix offsets should be non-zero in non-identity case
    if (rec.uint32_logs.size() >= 4) {
        size_t total = rec.uint32_logs.size();
        CHECK(rec.uint32_logs[total - 4] != 0, "MatrixOffset should be non-zero in non-identity case");
    }
}

// Test the failure-path behavior (first write fails)
static void test_failure_path()
{
    // Setup simple mhc2
    cmsMHC2Type mhc2;
    mhc2.CurveEntries = 2;
    mhc2.MinLuminance = 0.5;
    mhc2.PeakLuminance = 1.0;
    mhc2.XYZ2XYZmatrix[0][0] = 1.0; mhc2.XYZ2XYZmatrix[0][1] = 0.0; mhc2.XYZ2XYZmatrix[0][2] = 0.0; mhc2.XYZ2XYZmatrix[0][3] = 0.0;
    mhc2.XYZ2XYZmatrix[1][0] = 0.0; mhc2.XYZ2XYZmatrix[1][1] = 1.0; mhc2.XYZ2XYZmatrix[1][2] = 0.0; mhc2.XYZ2XYZmatrix[1][3] = 0.0;
    mhc2.XYZ2XYZmatrix[2][0] = 0.0; mhc2.XYZ2XYZmatrix[2][1] = 0.0; mhc2.XYZ2XYZmatrix[2][2] = 1.0; mhc2.XYZ2XYZmatrix[2][3] = 0.0;
    double red[2] = {1.0, 2.0}; double green[2] = {0.5, 1.5}; double blue[2] = {2.0, 3.0};
    mhc2.RedCurve = red; mhc2.GreenCurve = green; mhc2.BlueCurve = blue;

    // Prepare IO with a failure on the first uint32 write
    IORecorder rec;
    rec.failFirstUInt32 = true;
    cmsIOHANDLER io;
    io.Tell = TellFn;
    io.Seek = SeekFn;
    io.Write = WriteFn;
    io.data = &rec;

    cmsBool result = Type_MHC2_Write(nullptr, &io, &mhc2, 1);

    CHECK(result == FALSE, "Type_MHC2_Write should return FALSE when first uint32 write fails");
    // Ensure nothing else attempted (logs should be empty)
    if (!rec.uint32_logs.empty()) {
        std::cerr << "Failure path should prevent any uint32 logs" << std::endl;
    }
}

// Main entry: run all tests
int main() {
    std::cout << "Running Type_MHC2_Write unit tests (native harness)..." << std::endl;
    test_identity_matrix();
    test_non_identity_matrix();
    test_failure_path();

    std::cout << "Unit tests completed." << std::endl;
    return 0;
}