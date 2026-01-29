// Lightweight unit test suite for WriteSegmentedCurve (cmsBool WriteSegmentedCurve(cmsIOHANDLER*, cmsToneCurve*))
// This test suite avoids Google Test and uses a small in-file test harness.
// It relies on the project's lc/ms headers being available (lcms2.h) and the focal function
// being linked from the cmstypes.c/object under test.

#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include the Little CMS public API headers to get type definitions.
// The focal code uses internal structures (cmsToneCurve, cmsCurveSegment, cmsIOHANDLER, etc.),
// which are exposed by lcsm2.h. If your environment uses a different include path, adjust accordingly.

extern "C" {
    // Declaration of the focal function under test (C linkage).
    cmsBool WriteSegmentedCurve(cmsIOHANDLER* io, cmsToneCurve* g);
}

// Global test harness state for the mock IO
static std::vector<uint8_t> g_output;
static bool g_failNextWrite = false;

// Mock Write callback for cmsIOHANDLER
static cmsBool MockWrite(struct _cmsIOHANDLER* io, void* Buffer, cmsUInt32Number Size) {
    // If a failure is forced for this write, simulate a write failure
    if (g_failNextWrite) {
        g_failNextWrite = false;
        return FALSE;
    }
    // Copy raw bytes into the test buffer to simulate the IO stream
    const uint8_t* data = reinterpret_cast<const uint8_t*>(Buffer);
    g_output.insert(g_output.end(), data, data + Size);
    return TRUE;
}

// Small helper to run tests; returns true on success, false on failure, printing details.
#define RUN_TEST(test_fn) do { \
    if (!(test_fn())) { \
        std::cerr << "Test failed: " #test_fn "\n"; \
        return EXIT_FAILURE; \
    } else { \
        std::cout << "Test passed: " #test_fn "\n"; \
    } \
} while (0)

// Test 1: Basic positive path
// - Two segments: first is a sampled curve (Type == 0), second is a formula-based curve (Type == 7 -> Type = 1)
// - Verifies the exact binary layout written by WriteSegmentedCurve (sig, header, x1, segment records, and parameters)
bool Test_WriteSegmentedCurve_Basic_Positive() {
    // Prepare the tone-curve with 2 segments
    cmsToneCurve* g = new cmsToneCurve;
    g->nSegments = 2;
    cmsCurveSegment* Segments = new cmsCurveSegment[2];

    // Segment 0: Sampled curve
    Segments[0].Type = 0;
    Segments[0].nGridPoints = 3;
    Segments[0].x1 = 0.5f;
    Segments[0].SampledPoints = new cmsFloat32Number[3];
    Segments[0].SampledPoints[0] = 0.0f; // unused
    Segments[0].SampledPoints[1] = 1.0f;
    Segments[0].SampledPoints[2] = 2.0f;

    // Segment 1: Formula-based (Type 1 => actual Type = 7)
    Segments[1].Type = 7;
    Segments[1].nGridPoints = 0; // not used for formula segments
    Segments[1].Params[0] = 1.1f;
    Segments[1].Params[1] = 2.2f;
    Segments[1].Params[2] = 3.3f;
    Segments[1].Params[3] = 4.4f;
    Segments[1].Params[4] = 5.5f;

    g->Segments = Segments;

    // Reset mock IO
    g_output.clear();
    g_failNextWrite = false;

    // Set up the mock IO
    struct {
        cmsIOHANDLER handler;
    } mock;
    mock.handler.Write = MockWrite;
    mock.handler.Read = nullptr;

    // Call the focal function
    cmsBool result = WriteSegmentedCurve(&mock.handler, g);

    // Validate the result is TRUE
    if (result != TRUE) {
        std::cerr << "Basic_Positive: WriteSegmentedCurve returned FALSE (expected TRUE).\n";
        // Cleanup
        delete[] Segments[0].SampledPoints;
        delete[] Segments;
        delete g;
        return false;
    }

    // Validate the produced binary output against the expected layout
    size_t pos = 0;
    auto read32 = [&]() -> uint32_t {
        if (pos + 4 > g_output.size()) throw std::runtime_error("Buffer underflow reading uint32");
        uint32_t v = static_cast<uint32_t>(g_output[pos])
                   | (static_cast<uint32_t>(g_output[pos + 1]) << 8)
                   | (static_cast<uint32_t>(g_output[pos + 2]) << 16)
                   | (static_cast<uint32_t>(g_output[pos + 3]) << 24);
        pos += 4;
        return v;
    };
    auto read16 = [&]() -> uint16_t {
        if (pos + 2 > g_output.size()) throw std::runtime_error("Buffer underflow reading uint16");
        uint16_t v = static_cast<uint16_t>(g_output[pos])
                   | (static_cast<uint16_t>(g_output[pos + 1]) << 8);
        pos += 2;
        return v;
    };
    auto readFloat = [&]() -> float {
        if (pos + 4 > g_output.size()) throw std::runtime_error("Buffer underflow reading float");
        float v;
        std::memcpy(&v, &g_output[pos], sizeof(float));
        pos += 4;
        return v;
    };

    try {
        uint32_t sig = read32();
        if (sig != cmsSigSegmentedCurve) {
            std::cerr << "Basic_Positive: Wrong signature: expected " << cmsSigSegmentedCurve << ", got " << sig << "\n";
            throw std::runtime_error("Signature mismatch");
        }
        uint32_t reserved = read32();
        if (reserved != 0) {
            std::cerr << "Basic_Positive: Reserved header value not zero: " << reserved << "\n";
            throw std::runtime_error("Header reserved mismatch");
        }
        uint16_t nSegments = read16();
        if (nSegments != 2) {
            std::cerr << "Basic_Positive: nSegments expected 2, got " << nSegments << "\n";
            throw std::runtime_error("nSegments mismatch");
        }
        uint16_t reserved2 = read16();
        if (reserved2 != 0) {
            std::cerr << "Basic_Positive: Reserved second header value not zero: " << reserved2 << "\n";
            throw std::runtime_error("Header reserved mismatch2");
        }

        // x1 of first segment
        float x1_0 = readFloat();
        if (std::abs(x1_0 - 0.5f) > 1e-6f) {
            std::cerr << "Basic_Positive: x1 of first segment mismatch. expected 0.5, got " << x1_0 << "\n";
            throw std::runtime_error("First segment x1 mismatch");
        }

        // First segment type: sampled
        uint32_t seg0_marker = read32();
        if (seg0_marker != cmsSigSampledCurveSeg) {
            std::cerr << "Basic_Positive: First segment marker mismatch. expected " << cmsSigSampledCurveSeg
                      << ", got " << seg0_marker << "\n";
            throw std::runtime_error("First segment marker mismatch");
        }

        uint32_t seg0_reserved = read32();
        if (seg0_reserved != 0) {
            std::cerr << "Basic_Positive: First segment reserved not zero: " << seg0_reserved << "\n";
            throw std::runtime_error("First segment reserved mismatch");
        }

        uint32_t seg0_nGridMinus1 = read32();
        if (seg0_nGridMinus1 != (Segments[0].nGridPoints - 1)) {
            std::cerr << "Basic_Positive: seg0 nGridPoints-1 mismatch. expected " << (Segments[0].nGridPoints - 1)
                      << ", got " << seg0_nGridMinus1 << "\n";
            throw std::runtime_error("First segment grid mismatch");
        }

        // Sampled points 1..nGridPoints-1
        float sp1 = readFloat();
        float sp2 = readFloat();
        if (std::abs(sp1 - Segments[0].SampledPoints[1]) > 1e-6f ||
            std::abs(sp2 - Segments[0].SampledPoints[2]) > 1e-6f) {
            std::cerr << "Basic_Positive: First segment sampled points mismatch: "
                      << sp1 << ", " << sp2 << " vs " << Segments[0].SampledPoints[1] << ", "
                      << Segments[0].SampledPoints[2] << "\n";
            throw std::runtime_error("First segment sampled points mismatch");
        }

        // Second segment: formula curve
        uint32_t sig_formula = read32();
        if (sig_formula != cmsSigFormulaCurveSeg) {
            std::cerr << "Basic_Positive: Second segment marker mismatch. expected FormulaSeg, got " << sig_formula << "\n";
            throw std::runtime_error("Second segment marker mismatch");
        }
        uint32_t zero_after_formula = read32();
        if (zero_after_formula != 0) {
            std::cerr << "Basic_Positive: Second segment reserved not zero: " << zero_after_formula << "\n";
            throw std::runtime_error("Second segment reserved mismatch");
        }

        int Type = static_cast<int>(Segments[1].Type - 6);
        if (Type != 1) {
            std::cerr << "Basic_Positive: Second segment Type expected 1, got " << Type << "\n";
            throw std::runtime_error("Second segment type mismatch");
        }

        uint16_t type_read = read16();
        if (type_read != static_cast<uint16_t>(Type)) {
            std::cerr << "Basic_Positive: Second segment Type read mismatch: expected " << Type << ", got " << type_read << "\n";
            throw std::runtime_error("Second segment type read mismatch");
        }
        uint16_t zero_after_type = read16();
        if (zero_after_type != 0) {
            std::cerr << "Basic_Positive: Second segment zero after type not zero: " << zero_after_type << "\n";
            throw std::runtime_error("Second segment zero after type mismatch");
        }

        // 5 parameters
        for (int k = 0; k < 5; ++k) {
            float p = readFloat();
            if (std::abs(p - Segments[1].Params[k]) > 1e-6f) {
                std::cerr << "Basic_Positive: Second segment Param[" << k << "] mismatch: expected "
                          << Segments[1].Params[k] << ", got " << p << "\n";
                throw std::runtime_error("Second segment param mismatch");
            }
        }

        // Should be exactly consumed
        if (pos != g_output.size()) {
            std::cerr << "Basic_Positive: Extra bytes remaining in output: " << (g_output.size() - pos) << "\n";
            throw std::runtime_error("Extra output bytes");
        }

    } catch (...) {
        // Cleanup
        delete[] Segments[0].SampledPoints;
        delete[] Segments;
        delete g;
        return false;
    }

    // Cleanup
    delete[] Segments[0].SampledPoints;
    delete[] Segments;
    delete g;
    return true;
}

// Test 2: Invalid formula type triggers error
bool Test_WriteSegmentedCurve_InvalidFormulaType() {
    // Prepare the tone-curve with 2 segments
    cmsToneCurve* g = new cmsToneCurve;
    g->nSegments = 2;
    cmsCurveSegment* Segments = new cmsCurveSegment[2];

    // Segment 0: Sampled curve
    Segments[0].Type = 0;
    Segments[0].nGridPoints = 3;
    Segments[0].x1 = 0.5f;
    Segments[0].SampledPoints = new cmsFloat32Number[3];
    Segments[0].SampledPoints[0] = 0.0f;
    Segments[0].SampledPoints[1] = 1.0f;
    Segments[0].SampledPoints[2] = 2.0f;

    // Segment 1: Invalid formula type (Type = 9 -> Type-6 = 3 which is out of [0,2])
    Segments[1].Type = 9;
    Segments[1].nGridPoints = 0;
    for (int i = 0; i < 5; ++i) Segments[1].Params[i] = 0.0f;

    g->Segments = Segments;

    // Reset mock IO
    g_output.clear();
    g_failNextWrite = false;

    // Prepare mock IO
    struct {
        cmsIOHANDLER handler;
    } mock;
    mock.handler.Write = MockWrite;
    mock.handler.Read = nullptr;

    // Call function
    cmsBool result = WriteSegmentedCurve(&mock.handler, g);

    // Expect failure
    if (result != FALSE) {
        std::cerr << "InvalidFormulaType: Expected FALSE return, got TRUE\n";
        delete[] Segments[0].SampledPoints;
        delete[] Segments;
        delete g;
        return false;
    }

    // Cleanup
    delete[] Segments[0].SampledPoints;
    delete[] Segments;
    delete g;
    return true;
}

// Test 3: IO write failure propagates to FALSE return (no buffer written)
bool Test_WriteSegmentedCurve_WriteFailureHandling() {
    // Prepare the tone-curve with 1 segment (simplified; just the header shouldn't be enough to complete)
    cmsToneCurve* g = new cmsToneCurve;
    g->nSegments = 1;
    cmsCurveSegment* Segments = new cmsCurveSegment[1];

    Segments[0].Type = 0;
    Segments[0].nGridPoints = 2;
    Segments[0].x1 = 0.25f;
    Segments[0].SampledPoints = new cmsFloat32Number[2];
    Segments[0].SampledPoints[0] = 0.0f;
    Segments[0].SampledPoints[1] = 0.5f;

    g->Segments = Segments;

    // Reset IO and force a write failure on first write
    g_output.clear();
    g_failNextWrite = true; // Force the first _cmsWrite* call to fail

    // Setup mock IO
    struct {
        cmsIOHANDLER handler;
    } mock;
    mock.handler.Write = MockWrite;
    mock.handler.Read = nullptr;

    cmsBool result = WriteSegmentedCurve(&mock.handler, g);

    // Expect false due to write failure
    if (result != FALSE) {
        std::cerr << "WriteFailureHandling: Expected FALSE due to IO write failure, got TRUE\n";
        delete[] Segments[0].SampledPoints;
        delete[] Segments;
        delete g;
        return false;
    }

    // Ensure no data was written to the buffer
    if (!g_output.empty()) {
        std::cerr << "WriteFailureHandling: Expected no data written on failure, buffer size = " << g_output.size() << "\n";
        delete[] Segments[0].SampledPoints;
        delete[] Segments;
        delete g;
        return false;
    }

    // Cleanup
    delete[] Segments[0].SampledPoints;
    delete[] Segments;
    delete g;
    return true;
}

// Main entry: run all tests
int main() {
    std::cout << "Starting WriteSegmentedCurve test suite (no GTest)..." << std::endl;

    // Run tests
    RUN_TEST(Test_WriteSegmentedCurve_Basic_Positive);
    RUN_TEST(Test_WriteSegmentedCurve_InvalidFormulaType);
    RUN_TEST(Test_WriteSegmentedCurve_WriteFailureHandling);

    std::cout << "All tests completed." << std::endl;
    return EXIT_SUCCESS;
}