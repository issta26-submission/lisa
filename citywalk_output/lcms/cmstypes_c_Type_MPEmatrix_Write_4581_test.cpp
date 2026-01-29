/*
Unit test suite for the focal method:
  cmsBool Type_MPEmatrix_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems)

Assumptions and approach:
- We build a lightweight, self-contained C++11 test harness (no GoogleTest).
- Tests target the behavior of Type_MPEmatrix_Write by simulating an in-memory IO channel (via cmsIOHANDLER) and a minimal in-memory matrix data.
- We exercise true branches (offsets present and not-present) and a false branch (write failure) to maximize coverage.
- We rely on the project’s existing types (cmsBool, cmsUInt16Number, cmsUInt32Number, cmsFloat32Number, cmsStage, _cmsStageMatrixData, cmsIOHANDLER) being provided by the lcms2 headers in the environment.
- We implement lightweight, non-terminating assertions (APPROX) and summarize test results in main().
- This test file is designed to be built and linked with the project’s existing cmstypes.c (which contains Type_MPEmatrix_Write) and the lcms2 library headers. It does not depend on GoogleTest.

Note: If your environment uses different signatures for cmsIOHANDLER Write functions, you may need to adjust the MockIO wiring to properly capture written bytes. The tests below follow the LCMS style APIs as commonly exposed in lcms2.h / lcms2_internal.h.

Compilation hints:
- Compile with C++11, link against the lcms2 library as needed by your project (e.g., -llcms2) and include the appropriate headers (lcms2.h or lcms2_internal.h).
- Ensure cmstypes.c is compiled and linked into the test executable so that Type_MPEmatrix_Write is a callable symbol.

Code begins here:
*/

#include <cstring>
#include <cstdio>
#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <cstdint>


/* 
  The test expects the following canonical lcms types to be defined by the project headers:
  - cmsBool, cmsUInt16Number, cmsUInt32Number, cmsFloat32Number
  - cmsIOHANDLER
  - struct cmsStage with fields Data, InputChannels, OutputChannels
  - struct _cmsStageMatrixData with Double[] and Offset[]
  - cmsStage*, cmsBoolean/Boolean types and related constants

  To minimize boilerplate and maximize portability, we rely on the project headers to define
  these types. If compiling in an environment where some of these types are not visible,
  you will need to adjust the includes accordingly (e.g., include <lcms2.h> or <lcms2_internal.h>).
*/

// Forward declaration of the focal function (as it is provided in cmstypes.c)
extern "C" {
    // The actual signature is:
    // cmsBool Type_MPEmatrix_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems);
    // We declare it here to obtain a proper linkage in the test harness.
    typedef int cmsBool;
    struct _cms_typehandler_struct; // opaque in our usage
    struct cmsIOHANDLER;            // IO abstraction (from lcms)
    typedef unsigned int cmsUInt32Number;
    typedef unsigned short cmsUInt16Number;
    typedef float cmsFloat32Number;

    cmsBool Type_MPEmatrix_Write(struct _cms_typehandler_struct* self,
                                 struct cmsIOHANDLER* io,
                                 void* Ptr,
                                 cmsUInt32Number nItems);
}

/* Lightweight assertion framework (non-terminating)
   - Keeps track of failures and prints a summary at the end.
   - Non-terminating: does not exit on first failure, continues to exercise more code paths. */
class SimpleTestRunner {
public:
    SimpleTestRunner() : failures(0), total(0) {}

    void expectTrue(bool cond, const std::string& msg) {
        ++total;
        if (!cond) {
            ++failures;
            std::cerr << "ASSERT TRUE FAILED: " << msg << "\n";
        }
    }

    void expectEqUInt32(uint32_t a, uint32_t b, const std::string& msg) {
        ++total;
        if (a != b) {
            ++failures;
            std::cerr << "ASSERT EQUAL FAILED: " << msg
                      << " (got " << a << ", expected " << b << ")\n";
        }
    }

    void expectEqBytes(const std::vector<uint8_t>& a,
                       const std::vector<uint8_t>& b,
                       const std::string& msg) {
        ++total;
        if (a.size() != b.size() || std::memcmp(a.data(), b.data(), a.size()) != 0) {
            ++failures;
            std::cerr << "ASSERT BYTES FAILED: " << msg
                      << " (buffer length " << a.size() << " vs " << b.size() << ")\n";
        }
    }

    int results() const { return failures; }

    void summary() const {
        std::cout << "\nTest summary: " << (total - failures) << " / " << total
                  << " tests passed, " << failures << " failed.\n";
    }

private:
    int failures;
    int total;
};

/*
  Mock I/O: We rely on Type_MPEmatrix_Write calling _cmsWriteUInt16Number and _cmsWriteFloat32Number,
  which in turn use the cmsIOHANDLER Write function to push bytes. We implement a mock cmsIOHANDLER
  with a Write function that captures raw bytes into a vector. The actual two write helpers will
  call into this Write function, so we can verify exact byte sequences.
*/

typedef unsigned char cmsUInt8Number;

/* The following MockIO mirrors the expected lcms cmsIOHANDLER interface sufficiently
   for the focal function to write data into an in-memory buffer. We only implement
   what is necessary for the tests. */
struct MockIO {
    // In LCMS, cmsIOHANDLER contains a UserData pointer and a Write function pointer.
    // We emulate a minimal version: a Write function taking (UserData, Buffer, Size)
    cmsBool (*Write)(void* UserData, const cmsUInt8Number* Buffer, cmsUInt32Number Count);
    void* UserData;
    std::vector<uint8_t> buffer;

    static cmsBool writeFunc(void* UserData, const cmsUInt8Number* Buffer, cmsUInt32Number Count) {
        MockIO* self = static_cast<MockIO*>(UserData);
        self->buffer.insert(self->buffer.end(), Buffer, Buffer + Count);
        // Simulate successful write
        return 1;
    }

    MockIO() {
        Write = &MockIO::writeFunc;
        UserData = this;
    }
};

/*
  Helper: Build a simple _cmsStageMatrixData-like object in memory.

  We do not rely on private layout knowledge here in external tests; instead, we rely on the
  library's internal definitions to interact with Type_MPEmatrix_Write. We allocate the minimal
  memory that the function expects: a _cmsStageMatrixData structure containing:
    - Double: array of cmsFloat64Number (double) values
    - Offset: array of cmsFloat64Number (double) pointers (or a pointer to array)
  To keep the test robust, we allocate a small struct with the fields used by Type_MPEmatrix_Write
  and fill them accordingly.

  Note: This approach assumes _cmsStageMatrixData has:
    cmsFloat64Number Double[N], cmsFloat64Number* Offset;
  which is typical for LCMS matrix data in MPE structures. If your version uses a different
  layout, adjust accordingly.
*/

double g_doubleMatrix[9]; // maximum we may need for 3x3
double g_offsets[3];

struct MatrixDataFixture {
    // Memory layout matching _cmsStageMatrixData: Double[] followed by Offset[]
    double Double[9];
    double* Offset;
};

struct StageFixture {
    // Minimal cmsStage-like struct used by Type_MPEmatrix_Write
    void* Data;
    unsigned short InputChannels;
    unsigned short OutputChannels;
};

static void fillMatrixFixture(MatrixDataFixture* mtx, int nElems, const double* values, const double* offs) {
    // Copy Double values
    for (int i = 0; i < nElems; ++i) {
        mtx->Double[i] = values[i];
    }
    // Fill Offset if provided
    if (offs != nullptr) {
        mtx->Offset = const_cast<double*>(offs);
    } else {
        mtx->Offset = nullptr;
    }
}

/*
  Helper to run a single test case:
  - Sets up a Stage with Matrix data (Double[] and Offset[])
  - Calls Type_MPEmatrix_Write
  - Verifies success/failure and the produced bytes
*/

void test_Type_MPEmatrix_Write_OffsetsNull(SimpleTestRunner& tester) {
    // Prepare matrix data: 2 inputs x 3 outputs => 6 elements
    double values[6] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
    double* offs = nullptr; // Offsets not provided (should write 0s)

    MockIO mock;
    // Build a fake _cmsStageMatrixData and Stage
    MatrixDataFixture mat;
    fillMatrixFixture(&mat, 6, values, offs);

    StageFixture stage;
    stage.Data = &mat;
    stage.InputChannels = 2;
    stage.OutputChannels = 3;

    // Call the focal method
    cmsBool result = Type_MPEmatrix_Write(nullptr, &mock.io, &stage, 0);

    // Prepare expected byte sequence:
    // - InputChannels: 2 bytes (little-endian)
    // - OutputChannels: 3
    // - 6 floats (32-bit) for the 6 matrix values: 6 * 4 bytes
    // - Offsets: since Offset == NULL, write three 0.0f
    // Build expected buffer accordingly
    std::vector<uint8_t> expected;
    // 2 -> little-endian
    expected.push_back(0x02);
    expected.push_back(0x00);
    // 3 -> little-endian
    expected.push_back(0x03);
    expected.push_back(0x00);
    // helper to append a 32-bit float to expected
    auto push_float = [&](float f) {
        uint32_t asInt;
        static_assert(sizeof(float) == 4, "float must be 4 bytes");
        std::memcpy(&asInt, &f, sizeof(asInt));
        // little-endian
        expected.push_back(static_cast<uint8_t>(asInt & 0xFF));
        expected.push_back(static_cast<uint8_t>((asInt >> 8) & 0xFF));
        expected.push_back(static_cast<uint8_t>((asInt >> 16) & 0xFF));
        expected.push_back(static_cast<uint8_t>((asInt >> 24) & 0xFF));
    };
    for (int i = 0; i < 6; ++i) {
        push_float(static_cast<float>(values[i]));
    }
    // Offsets: 3 zeros
    for (int i = 0; i < 3; ++i) {
        push_float(0.0f);
    }

    // Verify results
    tester.expectTrue(result == 1, "Type_MPEmatrix_Write should return TRUE when offsets are NULL");
    tester.expectEqBytes(mock.buffer, expected, "Buffer should contain correct serialized matrix with NULL Offsets");
}

void test_Type_MPEmatrix_Write_OffsetsProvided(SimpleTestRunner& tester) {
    // Prepare matrix data: 2x3 -> 6 elements
    double values[6] = { 1.5, -2.25, 3.0, 4.0, 5.5, 6.75 };
    double offsArr[3] = { 0.1, 0.2, 0.3 };

    MockIO mock;
    MatrixDataFixture mat;
    fillMatrixFixture(&mat, 6, values, offsArr);

    StageFixture stage;
    stage.Data = &mat;
    stage.InputChannels = 2;
    stage.OutputChannels = 3;

    cmsBool result = Type_MPEmatrix_Write(nullptr, &mock.io, &stage, 0);

    // Build expected bytes
    std::vector<uint8_t> expected;
    // 2
    expected.push_back(0x02);
    expected.push_back(0x00);
    // 3
    expected.push_back(0x03);
    expected.push_back(0x00);
    // 6 floats
    auto push_float = [&](float f) {
        uint32_t asInt;
        std::memcpy(&asInt, &f, sizeof(asInt));
        expected.push_back(static_cast<uint8_t>(asInt & 0xFF));
        expected.push_back(static_cast<uint8_t>((asInt >> 8) & 0xFF));
        expected.push_back(static_cast<uint8_t>((asInt >> 16) & 0xFF));
        expected.push_back(static_cast<uint8_t>((asInt >> 24) & 0xFF));
    };
    for (int i = 0; i < 6; ++i) {
        push_float(static_cast<float>(values[i]));
    }
    // 3 offsets
    for (int i = 0; i < 3; ++i) {
        push_float(static_cast<float>(offsArr[i]));
    }

    tester.expectTrue(result == 1, "Type_MPEmatrix_Write should return TRUE when Offsets are provided");
    tester.expectEqBytes(mock.buffer, expected, "Buffer should contain correct serialized matrix with provided Offsets");
}

void test_Type_MPEmatrix_Write_Failure(SimpleTestRunner& tester) {
    // We simulate a failure by replacing the IO Write path with a faulty one.
    // This environment assumes that the underlying _cmsWriteUInt16Number / _cmsWriteFloat32Number
    // use the cmsIOHANDLER->Write function. If the Write function returns FALSE anywhere,
    // Type_MPEmatrix_Write should propagate FALSE.

    // We'll implement a MockIO with a Write function that forces a failure on the second write.
    class FaultyMockIO : public MockIO {
    public:
        int writeCount;
        FaultyMockIO() : MockIO(), writeCount(0) {}

        static cmsBool writeFunc(void* UserData, const cmsUInt8Number* Buffer, cmsUInt32Number Count) {
            FaultyMockIO* self = static_cast<FaultyMockIO*>(UserData);
            ++self->writeCount;
            // Force failure on the second write operation
            if (self->writeCount == 2) {
                return 0; // simulate write failure
            }
            // Normal write
            self->buffer.insert(self->buffer.end(), Buffer, Buffer + Count);
            return 1;
        }

        void installFaultyBehavior() {
            this->Write = &FaultyMockIO::writeFunc;
            this->UserData = this;
        }
    };

    FaultyMockIO faulty;
    faulty.installFaultyBehavior();

    // Setup matrix with non-NULL offsets to trigger writes twice (and fail on second)
    MatrixDataFixture mat;
    double values[6] = { 0.5, 0.5, 0.5, 0.5, 0.5, 0.5 };
    double offsArr[3] = { 0.0, 0.0, 0.0 };
    fillMatrixFixture(&mat, 6, values, offsArr);

    StageFixture stage;
    stage.Data = &mat;
    stage.InputChannels = 1;
    stage.OutputChannels = 3;

    cmsBool result = Type_MPEmatrix_Write(nullptr, &faulty.io, &stage, 0);

    tester.expectTrue(result == 0, "Type_MPEmatrix_Write should return FALSE when a write operation fails");
}

int main() {
    SimpleTestRunner tester;

    // Run the tests
    test_Type_MPEmatrix_Write_OffsetsNull(tester);
    test_Type_MPEmatrix_Write_OffsetsProvided(tester);
    test_Type_MPEmatrix_Write_Failure(tester);

    tester.summary();
    return tester.results();
}