// Unit test suite for the focal method Pack3BytesOptimized in cmspack.c
// Targets C++11, uses a lightweight homegrown test harness (no Google Test).
// The tests call the function with C linkage to match the C implementation.
// Explanation: each test focuses on the core behavior of Pack3BytesOptimized:
//   - It writes the lower 8 bits of wOut[0], wOut[1], wOut[2] into output.
//   - It returns a pointer to the position immediately after the 3 written bytes.
//   - Stride is unused by this function, but we verify behavior with various Stride values.

#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <cstdint>


// Lightweight compatibility macros and type definitions to allow
// linking with the C implementation in cmspack.c.
// We declare the function with C linkage to avoid C++ name mangling.

#ifndef CMSREGISTER
#define CMSREGISTER
#endif

// Forward declaration of the C struct type as used by the function.
extern "C" {
    typedef struct _cmsTRANSFORM _cmsTRANSFORM;

    // Typedefs matching the project types (cmsUInt8Number, cmsUInt16Number, cmsUInt32Number)
    typedef unsigned char cmsUInt8Number;
    typedef unsigned short cmsUInt16Number;
    typedef unsigned int cmsUInt32Number;

    // Prototype of the focal function (defined in cmspack.c)
    cmsUInt8Number* Pack3BytesOptimized(CMSREGISTER _cmsTRANSFORM* info,
                                        CMSREGISTER cmsUInt16Number wOut[],
                                        CMSREGISTER cmsUInt8Number* output,
                                        CMSREGISTER cmsUInt32Number Stride);
}

// Lightweight test harness state
static int gFailures = 0;

// Helper: convert a single test label and result
static void reportTestResult(const std::string& testName, bool passed) {
    if (passed) {
        std::cout << "[PASS] " << testName << std::endl;
    } else {
        std::cout << "[FAIL] " << testName << std::endl;
        ++gFailures;
    }
}

/*
Test 1: Basic correctness with small values
- Input wOut = {0x0123, 0x0456, 0x0789}
- Expected low-byte outputs: {0x23, 0x56, 0x89}
- Expected returned pointer = output + 3
*/
static int test_Pack3BytesOptimized_BasicCase() {
    const char* testName = "Pack3BytesOptimized_BasicCase";
    cmsUInt16Number wOut[3] = {0x0123, 0x0456, 0x0789};
    cmsUInt8Number output[3] = {0};
    _cmsTRANSFORM* info = nullptr;

    cmsUInt8Number* endPtr = Pack3BytesOptimized(info, wOut, output, 0);

    bool ok = true;
    if (endPtr != output + 3) {
        ok = false;
        std::cerr << testName << ": end pointer mismatch (expected " << reinterpret_cast<void*>(output + 3)
                  << ", got " << reinterpret_cast<void*>(endPtr) << ")\n";
    }
    if (output[0] != 0x23 || output[1] != 0x56 || output[2] != 0x89) {
        ok = false;
        std::cerr << testName << ": output bytes mismatch (got "
                  << std::hex
                  << static_cast<int>(output[0]) << " "
                  << static_cast<int>(output[1]) << " "
                  << static_cast<int>(output[2])
                  << std::dec << ")\n";
    }

    reportTestResult(testName, ok);
    return ok ? 0 : 1;
}

/*
Test 2: High-byte values to ensure LSB extraction is correct
- Input wOut = {0x1234, 0xABCD, 0x0F0F}
- Expected low-byte outputs: {0x34, 0xCD, 0x0F}
*/
static int test_Pack3BytesOptimized_HighBytesCase() {
    const char* testName = "Pack3BytesOptimized_HighBytesCase";
    cmsUInt16Number wOut[3] = {0x1234, 0xABCD, 0x0F0F};
    cmsUInt8Number output[3] = {0};
    _cmsTRANSFORM* info = nullptr;

    cmsUInt8Number* endPtr = Pack3BytesOptimized(info, wOut, output, 0);

    bool ok = true;
    if (endPtr != output + 3) {
        ok = false;
        std::cerr << testName << ": end pointer mismatch (expected " << reinterpret_cast<void*>(output + 3)
                  << ", got " << reinterpret_cast<void*>(endPtr) << ")\n";
    }
    if (output[0] != 0x34 || output[1] != 0xCD || output[2] != 0x0F) {
        ok = false;
        std::cerr << testName << ": output bytes mismatch (got "
                  << std::hex
                  << static_cast<int>(output[0]) << " "
                  << static_cast<int>(output[1]) << " "
                  << static_cast<int>(output[2])
                  << std::dec << ")\n";
    }

    reportTestResult(testName, ok);
    return ok ? 0 : 1;
}

/*
Test 3: Non-zero Stride value should not affect actual data written
- Input wOut = {0x0001, 0x0002, 0x0003}
- Stride = 12345
- Expected low-byte outputs: {0x01, 0x02, 0x03}
- Returned end pointer should still be output + 3
*/
static int test_Pack3BytesOptimized_StrideIgnored() {
    const char* testName = "Pack3BytesOptimized_StrideIgnored";
    cmsUInt16Number wOut[3] = {0x0001, 0x0002, 0x0003};
    cmsUInt8Number output[3] = {0};
    _cmsTRANSFORM* info = nullptr;
    cmsUInt32Number stride = 12345;

    cmsUInt8Number* endPtr = Pack3BytesOptimized(info, wOut, output, stride);

    bool ok = true;
    if (endPtr != output + 3) {
        ok = false;
        std::cerr << testName << ": end pointer mismatch (expected " << reinterpret_cast<void*>(output + 3)
                  << ", got " << reinterpret_cast<void*>(endPtr) << ")\n";
    }
    if (output[0] != 0x01 || output[1] != 0x02 || output[2] != 0x03) {
        ok = false;
        std::cerr << testName << ": output bytes mismatch (got "
                  << std::hex
                  << static_cast<int>(output[0]) << " "
                  << static_cast<int>(output[1]) << " "
                  << static_cast<int>(output[2])
                  << std::dec << ")\n";
    }

    reportTestResult(testName, ok);
    return ok ? 0 : 1;
}

// Runner to execute all Pack3BytesOptimized tests
static int run_Pack3BytesOptimized_Tests() {
    int failures = 0;
    failures += test_Pack3BytesOptimized_BasicCase();
    failures += test_Pack3BytesOptimized_HighBytesCase();
    failures += test_Pack3BytesOptimized_StrideIgnored();

    if (failures == 0) {
        std::cout << "[INFO] All Pack3BytesOptimized tests passed.\n";
    } else {
        std::cout << "[INFO] Pack3BytesOptimized tests completed with " << failures << " failure(s).\n";
    }
    return failures;
}

int main() {
    // Run the small test suite and return non-zero on failure
    int totalFailures = run_Pack3BytesOptimized_Tests();
    return totalFailures;
}