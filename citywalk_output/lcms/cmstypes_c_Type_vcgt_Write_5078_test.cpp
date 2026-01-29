/*
  Unit test suite for cmsBool Type_vcgt_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems)
  - Target: verify both branches (parametric gamma path and gamma-table path) of the implementation.
  - Environment assumptions:
      - LittleCMS-like structures are available (cmsToneCurve, cmsToneCurveSegment, cmsUInt32Number, cmsUInt16Number, cmsFloat64Number, etc.).
      - The function under test is Type_vcgt_Write (exported from the focal module cmstypes.c) and is linked during test compilation.
      - We provide a lightweight fake I/O handler to capture writes without touching real I/O facilities.
  - Notes:
      - The tests avoid GTest and use a small, self-contained harness with non-terminating assertions (print-and-continue style).
      - Two main tests are implemented:
          1) Parametric gamma path: three tone curves with parametric type 5 (gamma path). Verifies the first gamma-parameters block written to the IO stream.
          2) Gamma-table path: at least one tone curve not of parametric type 5 to trigger the else branch. Verifies the gamma-table header bytes are written (and that there are 3*256 16-bit entries following).
  - Build instructions (example):
      // g++ -std=c++11 -o test_vcgt_write test_vcgt_write.cpp -llcms2
  - This test is designed to be compiled in an environment where LittleCMS-like headers are available (e.g., lcms2.h).
*/

#include <cstdio>
#include <vector>
#include <iostream>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include LittleCMS headers (adjust include path as needed for your environment)

// Prototype of the focal function (provided in cmstypes.c)
extern cmsBool Type_vcgt_Write(struct _cms_typehandler_struct* self,
                               cmsIOHANDLER* io,
                               void* Ptr,
                               cmsUInt32Number nItems);

// Lightweight fake IO to capture writes
struct FakeIO {
    // Place the cmsIOHANDLER at the start for easy downcast
    cmsIOHANDLER io;
    std::vector<uint8_t> buffer;

    FakeIO() {
        // Initialize the I/O handler to a minimal, write-only stub
        io.ContextID = this;
        io.Write = FakeWrite;
        io.Read = nullptr; // not used in tests
        io.Close = nullptr;
        io.Flush = nullptr;
        // Ensure the user data context (if needed) points back to this structure
    }

    // Helper to reset buffer between tests
    void reset() { buffer.clear(); }

    // Access to internal buffer as bytes
    const std::vector<uint8_t>& getBuffer() const { return buffer; }

    // Static write callback used by the Type_vcgt_Write implementation
    static cmsBool FakeWrite(struct _cmsIOHANDLER* io, cmsUInt32Number size, const void* data) {
        // Since we place cmsIOHANDLER as the first member of FakeIO,
        // we can convert back to FakeIO by reinterpret_cast.
        FakeIO* self = reinterpret_cast<FakeIO*>(io);
        const uint8_t* bytes = static_cast<const uint8_t*>(data);
        if (!self) return FALSE;
        self->buffer.insert(self->buffer.end(), bytes, bytes + size);
        return TRUE;
    }
};

// Utility to create a parametric gamma tone curve (Type == 5)
static cmsToneCurve* create_parametric_curve(double gamma, double base, double minVal) {
    cmsToneCurve* t = new cmsToneCurve;
    t->nSegments = 1;
    t->Segments = new cmsToneCurveSegment[1];
    t->Segments[0].Type = 5; // parametric type as used by the focal method
    // Params layout as assumed by the focal method:
    // Params[0] = Gamma
    // Params[1] = base (exponent base)
    // Params[5] = Min
    t->Segments[0].Params[0] = gamma;
    t->Segments[0].Params[1] = base;
    t->Segments[0].Params[5] = minVal;
    // Other Params entries are not used by the test; initialize to 0
    for (int i = 2; i < 7; ++i) {
        if (i != 5) t->Segments[0].Params[i] = 0.0;
    }
    return t;
}

// Utility to create a non-parametric tone curve (Type != 5)
static cmsToneCurve* create_nonparametric_curve() {
    cmsToneCurve* t = new cmsToneCurve;
    t->nSegments = 1;
    t->Segments = new cmsToneCurveSegment[1];
    t->Segments[0].Type = 0; // non-parametric type
    for (int i = 0; i < 7; ++i) t->Segments[0].Params[i] = 0.0;
    return t;
}

// Helpers for 32-bit little-endian reading
static uint32_t readLE32(const uint8_t* p) {
    return (static_cast<uint32_t>(p[0])       ) |
           (static_cast<uint32_t>(p[1]) << 8  ) |
           (static_cast<uint32_t>(p[2]) << 16 ) |
           (static_cast<uint32_t>(p[3]) << 24 );
}
static uint16_t readLE16(const uint8_t* p) {
    return static_cast<uint16_t>(p[0] | (p[1] << 8));
}

// Assertions helpers (non-terminating)
static int g_failCount = 0;
#define ASSERT_TRUE(cond, msg) \
    do { if (!(cond)) { std::cerr << "TEST_FAIL: " << msg << "\n"; ++g_failCount; } } while (0)

#define ASSERT_EQ(a, b, msg) \
    do { if (!((a) == (b))) { std::cerr << "TEST_FAIL: " << msg << " | " << (a) << " != " << (b) << "\n"; ++g_failCount; } } while (0)

static void test_parametric_gamma_branch() {
    // Prepare three parametric curves (Type == 5) to trigger the gamma formula path
    cmsToneCurve* Curves[3];
    Curves[0] = create_parametric_curve(1.5, 2.0, -0.1);
    Curves[1] = create_parametric_curve(1.2, 2.5, -0.05);
    Curves[2] = create_parametric_curve(1.8, 1.5, -0.2);

    FakeIO fake;
    // Ptr points to array of three cmsToneCurve*
    cmsToneCurve* arr[3] = { Curves[0], Curves[1], Curves[2] };
    cmsBool result = Type_vcgt_Write(nullptr, &fake.io, arr, 3);

    // Expected: TRUE
    ASSERT_TRUE(result == TRUE, "Type_vcgt_Write should return TRUE for parametric gamma path");

    // Expected IO layout:
    // - 4 bytes: cmsVideoCardGammaFormulaType
    // - For each of 3 curves: gamma(4), min(4), max(4)
    // Total: 1 + 3*3 = 10 32-bit words = 40 bytes
    const auto& buf = fake.getBuffer();
    ASSERT_EQ(buf.size(), 4 + 3*3*4, "Parametric gamma path should write 40 bytes total (4-byte tags + 3*3 values per curve)");

    // Verify the first 4 bytes are the gamma formula type
    uint32_t firstTag = readLE32(buf.data());
    ASSERT_EQ(firstTag, cmsVideoCardGammaFormulaType, "First 4 bytes should be cmsVideoCardGammaFormulaType");

    // Validate gamma/min/max for each curve
    size_t pos = 4; // after first tag
    for (int i = 0; i < 3; ++i) {
        double gamma = Curves[i]->Segments[0].Params[0];
        double minv  = Curves[i]->Segments[0].Params[5];
        double maxv  = std::pow(Curves[i]->Segments[0].Params[1], gamma) + minv;

        int32_t gammaFixed = (int32_t)llround(gamma * 65536.0);
        int32_t minFixed   = (int32_t)llround(minv  * 65536.0);
        int32_t maxFixed   = (int32_t)llround(maxv  * 65536.0);

        uint32_t g32 = readLE32(buf.data() + pos); pos += 4;
        uint32_t m32 = readLE32(buf.data() + pos); pos += 4;
        uint32_t x32 = readLE32(buf.data() + pos); pos += 4;

        ASSERT_EQ((int32_t)g32, gammaFixed, "Gamma fixed-point value mismatch for curve " << i);
        ASSERT_EQ((int32_t)m32, minFixed,   "Min fixed-point value mismatch for curve "   << i);
        ASSERT_EQ((int32_t)x32, maxFixed,   "Max fixed-point value mismatch for curve "   << i);
    }

    // Cleanup
    for (int i = 0; i < 3; ++i) {
        delete[] Curves[i]->Segments;
        delete Curves[i];
    }
}

static void test_nonparametric_gamma_branch() {
    // Prepare three non-parametric curves (Type != 5) to trigger the gamma-table path
    cmsToneCurve* Curves[3];
    Curves[0] = create_nonparametric_curve();
    Curves[1] = create_nonparametric_curve();
    Curves[2] = create_nonparametric_curve();

    FakeIO fake;
    cmsToneCurve* arr[3] = { Curves[0], Curves[1], Curves[2] };
    cmsBool result = Type_vcgt_Write(nullptr, &fake.io, arr, 3);

    // Expected: TRUE
    ASSERT_TRUE(result == TRUE, "Type_vcgt_Write should return TRUE for gamma-table path");

    // Expected IO layout:
    // - 4 bytes: cmsVideoCardGammaTableType
    // - 2 bytes: 3
    // - 2 bytes: 256
    // - 2 bytes: 2
    // - Then 3 * 256 values of 16-bit words (1536 bytes)
    const auto& buf = fake.getBuffer();
    size_t expectedSize = 4 + 2 + 2 + 2 + (3 * 256 * 2);
    ASSERT_EQ(buf.size(), expectedSize, "Gamma-table path should write header + 3*256 16-bit values");

    // Inspect the header section
    uint32_t firstTag = readLE32(buf.data());
    ASSERT_EQ(firstTag, cmsVideoCardGammaTableType, "First 4 bytes should be cmsVideoCardGammaTableType");

    // Next 6 bytes: 3, 256, 2 (16-bit little-endian)
    uint16_t val1 = readLE16(buf.data() + 4);
    uint16_t val2 = readLE16(buf.data() + 6);
    uint16_t val3 = readLE16(buf.data() + 8);
    ASSERT_EQ(val1, 3,  "Second field should be 3 (nCurves)");
    ASSERT_EQ(val2, 256, "Third field should be 256 (nEntries per curve)");
    ASSERT_EQ(val3, 2,   "Fourth field should be 2 (Precision or similar)");
    
    // Cleanup
    for (int i = 0; i < 3; ++i) {
        delete[] Curves[i]->Segments;
        delete Curves[i];
    }
}

// Entry point for the test suite
int main() {
    std::cout << "Running Type_vcgt_Write unit tests...\n";

    g_failCount = 0;
    test_parametric_gamma_branch();
    test_nonparametric_gamma_branch();

    if (g_failCount == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << g_failCount << " TEST(S) FAILED\n";
        return 1;
    }
}