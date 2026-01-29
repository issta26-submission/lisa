// Lightweight unit test suite for UnrollLabDoubleTo16
// This test is self-contained and avoids external testing frameworks.
// It reimplements the focal function locally (same logic as in cmsPack/cmsDoubleTo16)
// to enable isolated, deterministic testing without the full CMS/LCMS build environment.

#include <cstring>
#include <iostream>
#include <cassert>
#include <lcms2_internal.h>
#include <cstdint>


// Domain knowledge: basic fixed-size types used by the focal method
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;
typedef double   cmsFloat64Number;
typedef float    cmsFloat32Number;

// Focal type used by UnrollLabDoubleTo16
struct cmsCIELab {
    cmsFloat64Number L;
    cmsFloat64Number a;
    cmsFloat64Number b;
};

// Transform info structure (partial, just what is needed for tests)
struct _cmsTRANSFORM {
    cmsUInt32Number InputFormat; // 0 = chunky/packed, 1 = planar (matching test macros)
};

// Lightweight inline helpers/macros to mirror the real code's behavior

// Match the intended behavior: planar if INPUTFORMAT_PLANAR
const cmsUInt32Number INPUTFORMAT_PLANAR = 1;
const cmsUInt32Number INPUTFORMAT_PACKED = 0;

// T_PLANAR(info -> InputFormat)
#define T_PLANAR(fmt) ((fmt) == INPUTFORMAT_PLANAR)

// T_EXTRA heuristic:
// In the non-planar (chunky) path, the original code adds extra space
// based on the input format. We model: 1 extra cmsFloat64Number for PACKED.
#define T_EXTRA(fmt) ((fmt) == INPUTFORMAT_PACKED ? 1 : 0)

// Forward declaration of the encoding function used by UnrollLabDoubleTo16
// In the real library, this converts Lab to 3 values in wIn (L, a, b).
// We'll implement a simple deterministic mapping for testing.
static void cmsFloat2LabEncoded(const cmsUInt16Number wIn[],
                               cmsCIELab* Lab)
{
    // Simple mapping: encode L, a, b as 16-bit integers by truncation/flooring
    // Use floor-like behavior by casting to integer (positive inputs in tests)
    if (Lab == nullptr) return;
    Lab->L = static_cast<cmsFloat64Number>(static_cast<uint16_t>(wIn[0]));
    Lab->a = static_cast<cmsFloat64Number>(static_cast<uint16_t>(wIn[1]));
    Lab->b = static_cast<cmsFloat64Number>(static_cast<uint16_t>(wIn[2]));
    // Decode: keep simple; the encoding simply reconstructs Lab from the given wIn
}

// Focal function under test: UnrollLabDoubleTo16
#define CMSREGISTER
#define CMS_INLINE inline

cmsUInt8Number* UnrollLabDoubleTo16(CMSREGISTER _cmsTRANSFORM* info,
                                   CMSREGISTER cmsUInt16Number wIn[],
                                   CMSREGISTER cmsUInt8Number* accum,
                                   CMSREGISTER cmsUInt32Number  Stride)
{
    // The body mirrors the provided focal method
    {
        if (T_PLANAR(info -> InputFormat)) {
            cmsCIELab Lab;
            cmsUInt8Number* pos_L;
            cmsUInt8Number* pos_a;
            cmsUInt8Number* pos_b;
            pos_L = accum;
            pos_a = accum + Stride;
            pos_b = accum + Stride * 2;
            Lab.L = *(cmsFloat64Number*) pos_L;
            Lab.a = *(cmsFloat64Number*) pos_a;
            Lab.b = *(cmsFloat64Number*) pos_b;
            cmsFloat2LabEncoded(wIn, &Lab);
            return accum + sizeof(cmsFloat64Number);
        }
        else {
            cmsFloat2LabEncoded(wIn, (cmsCIELab*) accum);
            accum += sizeof(cmsCIELab) + T_EXTRA(info ->InputFormat) * sizeof(cmsFloat64Number);
            return accum;
        }
    }
}

// Simple test harness

static int tests_run = 0;
static int tests_pass = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++tests_run; \
    if (cond) { \
        ++tests_pass; \
    } else { \
        std::cerr << "Test failed: " << msg << " (" << #cond << ")" << std::endl; \
    } \
} while(0)

#define EXPECT_EQ(actual, expected, msg) do { \
    ++tests_run; \
    if ((actual) == (expected)) { \
        ++tests_pass; \
    } else { \
        std::cerr << "Test failed: " << msg \
                  << " | Expected: " << (expected) << " Actual: " << (actual) << std::endl; \
    } \
} while(0)

// Helper to write a double into a buffer (little-endian on typical platforms)
static void writeDoubleToBuffer(void* dst, double value) {
    std::memcpy(dst, &value, sizeof(double));
}

// Test 1: Planar branch (true predicate)
// Arrange:
// - InputFormat = PLANAR
// - accum holds 3 doubles at offsets 0, Stride, Stride*2: L, a, b
// - wIn will be encoded by cmsFloat2LabEncoded via Lab read from accum
// Expect:
// - wIn[0] = floor(L), wIn[1] = floor(a), wIn[2] = floor(b)
// - Return value is accum + sizeof(double)
static void test_planar_branch_encodes_lab_from_planar_memory() {
    // Arrange
    cmsUInt8Number buffer[128];
    cmsUInt8Number* accum = buffer;

    // Stride and memory layout
    const cmsUInt32Number Stride = 8;
    const cmsUInt32Number required = Stride * 2 + 8; // ensure enough space for L, a, b
    // Fill L, a, b at positions 0, Stride, Stride*2
    double Lval = 12.0;
    double aval = 3.0;
    double bval = 2.0;
    writeDoubleToBuffer(accum + 0, Lval);
    writeDoubleToBuffer(accum + Stride, aval);
    writeDoubleToBuffer(accum + Stride * 2, bval);

    _cmsTRANSFORM info;
    info.InputFormat = INPUTFORMAT_PLANAR;

    cmsUInt16Number wIn[3] = {0, 0, 0};

    // Act
    cmsUInt8Number* result = UnrollLabDoubleTo16(&info, wIn, accum, Stride);

    // Assert
    EXPECT_TRUE(result == accum + sizeof(cmsFloat64Number), "Planar: return pointer advanced by 8 bytes");
    EXPECT_EQ(wIn[0], static_cast<cmsUInt16Number>(Lval), "Planar: wIn[0] encodes L value");
    EXPECT_EQ(wIn[1], static_cast<cmsUInt16Number>(aval), "Planar: wIn[1] encodes a value");
    EXPECT_EQ(wIn[2], static_cast< cmsUInt16Number>(bval), "Planar: wIn[2] encodes b value");
}

// Test 2: Packed/planar false branch (non-planar)
// Arrange:
// - InputFormat = PACKED
// - accum memory holds Lab values as 3 doubles (L, a, b) in a cmsCIELab struct
// - wIn will be filled by cmsFloat2LabEncoded from that Lab
// Expect:
// - wIn[0] = floor(L), wIn[1] = floor(a), wIn[2] = floor(b)
// - Return value is accum + sizeof(cmsCIELab) + sizeof(double) (8)
static void test_packed_branch_encodes_lab_from_packed_memory() {
    // Arrange
    cmsUInt8Number buffer[128];
    cmsUInt8Number* accum = buffer;

    // Prepare Lab in memory at accum
    cmsCIELab LabIn;
    LabIn.L = 5.5;
    LabIn.a = 7.7;
    LabIn.b = 9.9;
    std::memcpy(accum, &LabIn, sizeof(cmsCIELab)); // store Lab directly

    cmsUInt32Number Stride = 0; // not used in packed path
    _cmsTRANSFORM info;
    info.InputFormat = INPUTFORMAT_PACKED;

    cmsUInt16Number wIn[3] = {0, 0, 0};

    // Act
    cmsUInt8Number* result = UnrollLabDoubleTo16(&info, wIn, accum, Stride);

    // Assert
    // In packed path, Lab is read from accum as a Lab struct
    EXPECT_TRUE(result == accum + sizeof(cmsCIELab) + sizeof(cmsFloat64Number), "Packed: return pointer advanced by 24 + 8 bytes (32 total)");
    EXPECT_EQ(wIn[0], static_cast<cmsUInt16Number>(static_cast<int>(LabIn.L)), "Packed: wIn[0] encodes L value");
    EXPECT_EQ(wIn[1], static_cast<cmsUInt16Number>(static_cast<int>(LabIn.a)), "Packed: wIn[1] encodes a value");
    EXPECT_EQ(wIn[2], static_cast<cmsUInt16Number>(static_cast<int>(LabIn.b)), "Packed: wIn[2] encodes b value");
}

// Main entry point for test execution
int main() {
    std::cout << "Running UnrollLabDoubleTo16 unit tests (self-contained harness)..." << std::endl;

    test_planar_branch_encodes_lab_from_planar_memory();
    test_packed_branch_encodes_lab_from_packed_memory();

    std::cout << "Tests run: " << tests_run << ", Passed: " << tests_pass << std::endl;

    // Non-terminating assertion style: return non-zero if any test failed
    if (tests_run == tests_pass) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "Some tests failed." << std::endl;
        return 1;
    }
}