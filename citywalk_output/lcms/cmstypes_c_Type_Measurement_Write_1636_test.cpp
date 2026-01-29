// This test suite is handcrafted to exercise the focal method:
//     cmsBool  Type_Measurement_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems)
// within a C++11 compatible environment, without using GTest.
// It relies on user-provided stubs for dependent CMS I/O helpers
// (_cmsWriteUInt32Number, _cmsWriteXYZNumber, _cmsWrite15Fixed16Number)
// so that the test can run in a pure C/C++ testing fashion.
// The stubs log the sequence of writes to an in-memory log so that assertions
// can verify both true/false branches and the exact order of operations.
//
// Notes on candidate keywords (Step 1):
// - Observer, Backing, Geometry, Flare, IlluminantType: fields accessed by Type_Measurement_Write
// - _cmsWriteUInt32Number, _cmsWriteXYZNumber, _cmsWrite15Fixed16Number: dependent write helpers
// - cmsICCMeasurementConditions: input structure pointer (Ptr) carrying the measurement data
// - cmsIOHANDLER: IO context used by the writer
// The tests focus on true/false branches by simulating success and failure of those write calls.

#include <cstring>
#include <cstdio>
#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <cmath>


// Minimal typedefs and forward declarations to allow linking with the real cmstypes.c
// in a real environment. These are simplified to enable unit testing in isolation.

typedef unsigned int cmsUInt32Number;
typedef int cmsBool;
#define TRUE 1
#define FALSE 0

// Opaque forward declaration for the CMS internal typehandler struct used by the focal method
struct _cms_typehandler_struct;

// Simplified cmsIOHANDLER type for test harness
struct cmsIOHANDLER {};

// Forward declaration of the function under test
extern "C" cmsBool Type_Measurement_Write(struct _cms_typehandler_struct* self,
                                         cmsIOHANDLER* io,
                                         void* Ptr,
                                         cmsUInt32Number nItems);

// Local test IO and logging infrastructure

// A simple log to capture the sequence of "writes" performed by the _cmsWrite* helpers.
static std::vector<std::string> g_writeLog;

// Helper to format doubles with consistent precision for logs
static std::string dto(double v) {
    char buf[64];
    // Use fixed with 6 decimals for deterministic output
    std::snprintf(buf, sizeof(buf), "%.6f", v);
    return std::string(buf);
}

// Lightweight representations matching the library's expected shapes

// cmsXYZNumber: simplified positional 3D coordinates
typedef struct {
    double X;
    double Y;
    double Z;
} cmsXYZNumber;

// cmsS15Fixed16Number: signed 16.16 fixed-point number (stored as 32-bit int)
typedef int cmsS15Fixed16Number;

// cmsICCMeasurementConditions: simplified version used by the focal method
typedef struct {
    cmsUInt32Number Observer;
    cmsXYZNumber Backing;
    cmsUInt32Number Geometry;
    cmsS15Fixed16Number Flare;
    cmsUInt32Number IlluminantType;
} cmsICCMeasurementConditions;

// Implementations of the external write helpers that the focal method calls.
// They are provided here as C-linkage so that cmstypes.c will link against them.

extern "C" {

    // Write a 32-bit unsigned integer to the io handle.
    cmsBool _cmsWriteUInt32Number(cmsIOHANDLER* io, cmsUInt32Number value) {
        (void)io; // io is not used by test harness
        char buf[64];
        std::snprintf(buf, sizeof(buf), "UInt32:%u", static_cast<unsigned int>(value));
        g_writeLog.push_back(buf);
        return TRUE;
    }

    // Write an XYZ structure: X, Y, Z as doubles.
    cmsBool _cmsWriteXYZNumber(cmsIOHANDLER* io, const cmsXYZNumber* xyz) {
        (void)io;
        if (!xyz) return FALSE;
        char buf[128];
        std::snprintf(buf, sizeof(buf), "XYZ:%s,%s,%s",
                      dto(xyz->X).c_str(),
                      dto(xyz->Y).c_str(),
                      dto(xyz->Z).c_str());
        g_writeLog.push_back(std::string(buf));
        // For testing true/false branches, introduce a simple validation:
        // If any component is negative, simulate a failure.
        if (xyz->X < 0.0 || xyz->Y < 0.0 || xyz->Z < 0.0) {
            return FALSE;
        }
        return TRUE;
    }

    // Write a 15.16 fixed-point number
    cmsBool _cmsWrite15Fixed16Number(cmsIOHANDLER* io, cmsS15Fixed16Number v) {
        (void)io;
        char buf[64];
        std::snprintf(buf, sizeof(buf), "Fixed16:%d", static_cast<int>(v));
        g_writeLog.push_back(std::string(buf));
        return TRUE;
    }
}

// Helper to reset the log between tests
static void resetLog() {
    g_writeLog.clear();
}

// Helpers to access the log for assertions
static const std::vector<std::string>& getLog() {
    return g_writeLog;
}

// Test 1: All writes succeed - verify correct sequence and final TRUE
// This corresponds to the "happy path" where every _cmsWrite* helper returns TRUE.
static void test_Type_Measurement_Write_Succeeds() {
    resetLog();

    // Prepare a cmsICCMeasurementConditions instance with representative values
    cmsICCMeasurementConditions mc;
    mc.Observer = 5;                 // Observer
    mc.Backing.X = 1.0;              // Backing (XYZ)
    mc.Backing.Y = 2.0;
    mc.Backing.Z = 3.0;
    mc.Geometry = 7;                 // Geometry
    mc.Flare = 0x00014000;           // 1.25 in 16.16 fixed-point
    mc.IlluminantType = 9;           // IlluminantType

    cmsIOHANDLER io;                   // Minimal io object (not used by stubs)
    cmsUInt32Number nItems = 1;          // Unused in implementation but passed

    // Call the function under test
    cmsBool result = Type_Measurement_Write(nullptr, &io, &mc, nItems);

    // Assertions
    bool pass = (result == TRUE);
    const std::vector<std::string>& log = getLog();

    // Expected sequence
    // UInt32:Observer
    // XYZ:X,Y,Z
    // UInt32:Geometry
    // Fixed16:Flare
    // UInt32:IlluminantType
    bool expectedLogOk = false;
    if (log.size() == 5) {
        bool a = (log[0] == "UInt32:5");
        bool b = (log[1].rfind("XYZ:", 0) == 0); // starts with XYZ
        bool c = (log[2] == "UInt32:7");
        bool d = (log[3].rfind("Fixed16:", 0) == 0);
        bool e = (log[4] == "UInt32:9");
        expectedLogOk = a && b && c && d && e;
    }

    if (!pass) {
        std::cerr << "[Test] Type_Measurement_Write_Succeeds: FAILED - return value not TRUE\n";
    }
    if (!expectedLogOk) {
        std::cerr << "[Test] Type_Measurement_Write_Succeeds: FAILED - log sequence mismatch\n";
        std::cerr << "Log contents:\n";
        for (const auto& s : log) std::cerr << "  " << s << "\n";
    } else {
        std::cout << "[Test] Type_Measurement_Write_Succeeds: PASSED\n";
    }
}

// Test 2: Failure in XYZ write should cause overall function to return FALSE
// The XYZ writer will detect a negative component and return FALSE.
// This tests the true/false branch coverage for the second predicate.
static void test_Type_Measurement_Write_XYZ_Fails() {
    resetLog();

    cmsICCMeasurementConditions mc;
    mc.Observer = 42;
    mc.Backing.X = -1.0; // Negative to trigger failure in _cmsWriteXYZNumber
    mc.Backing.Y = 2.0;
    mc.Backing.Z = 3.0;
    mc.Geometry = 11;
    mc.Flare = 0x00018000; // e.g., 1.5
    mc.IlluminantType = 4;

    cmsIOHANDLER io;
    cmsUInt32Number nItems = 1;

    cmsBool result = Type_Measurement_Write(nullptr, &io, &mc, nItems);

    // Expect FALSE due to XYZ write failing
    if (result != FALSE) {
        std::cerr << "[Test] Type_Measurement_Write_XYZ_Fails: FAILED - expected FALSE return\n";
    } else {
        std::cout << "[Test] Type_Measurement_Write_XYZ_Fails: PASSED\n";
    }

    // The log should contain at least the first two writes before the failure
    // 1) UInt32 Observer
    // 2) XYZ with negative X value (the function logs before failing)
    const std::vector<std::string>& log = getLog();
    if (log.empty() || log[0] != "UInt32:42") {
        std::cerr << "[Test] Type_Measurement_Write_XYZ_Fails: WARNING - unexpected log start\n";
    } else {
        std::cout << "[Test] Type_Measurement_Write_XYZ_Fails: Log shows first write recorded as expected.\n";
    }
}

// Entry point
int main() {
    // Run tests sequentially. In a real unit test framework, you'd have test registrations.
    test_Type_Measurement_Write_Succeeds();
    test_Type_Measurement_Write_XYZ_Fails();

    // Summary
    std::cout << "All tests executed. Review individual test outputs for results.\n";
    return 0;
}