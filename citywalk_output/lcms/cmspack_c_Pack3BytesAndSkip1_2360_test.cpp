#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstddef>


// Include CMS/LCMS internal definitions to match the focal function's environment.
// This header is expected to provide: cmsUInt8Number, cmsUInt16Number, cmsUInt32Number,
// _cmsTRANSFORM, CMSREGISTER, and the macro FROM_16_TO_8 as used by Pack3BytesAndSkip1.

// Declare the focal function with C linkage to match the real implementation in cmspack.c.
// This allows linking from C++ test code without name mangling issues.
extern "C" cmsUInt8Number* Pack3BytesAndSkip1(CMSREGISTER _cmsTRANSFORM* info,
                                            CMSREGISTER cmsUInt16Number wOut[],
                                            CMSREGISTER cmsUInt8Number* output,
                                            CMSREGISTER cmsUInt32Number Stride);

// Lightweight, self-contained test harness (no GTest).
// We implement a small set of tests that verify the exact byte outputs and pointer arithmetic
// performed by Pack3BytesAndSkip1. The tests use only the standard library facilities.

static bool test_Pack3BytesAndSkip1_BasicBehavior()
{
    // Purpose:
    // - Validate that the function writes the low bytes of the first three 16-bit inputs.
    // - Ensure a skipped byte between the written bytes (i.e., fourth byte is not written).
    // - Verify the returned pointer is advanced by 4 bytes from the start of the output buffer.

    // Arrange
    cmsUInt16Number wOut[3] = { 0x1234, 0xABCD, 0x00FF }; // Low bytes: 0x34, 0xCD, 0xFF
    cmsUInt8Number buffer[8];
    // Initialize buffer with a known pattern to detect unintended writes
    for (size_t i = 0; i < 8; ++i) buffer[i] = static_cast<cmsUInt8Number>(0xAA);

    // Act
    cmsUInt8Number* start = buffer;
    cmsUInt8Number* ret = Pack3BytesAndSkip1(nullptr, wOut, buffer, 0);

    // Assert
    bool ok = true;

    if (buffer[0] != 0x34) {
        std::cerr << "test_BasicBehavior: buffer[0] expected 0x34, got 0x"
                  << std::hex << static_cast<int>(buffer[0]) << std::dec << "\n";
        ok = false;
    }
    if (buffer[1] != 0xCD) {
        std::cerr << "test_BasicBehavior: buffer[1] expected 0xCD, got 0x"
                  << std::hex << static_cast<int>(buffer[1]) << std::dec << "\n";
        ok = false;
    }
    if (buffer[2] != 0xFF) {
        std::cerr << "test_BasicBehavior: buffer[2] expected 0xFF, got 0x"
                  << std::hex << static_cast<int>(buffer[2]) << std::dec << "\n";
        ok = false;
    }
    // The 4th byte should be skipped; it should remain as the initial value (0xAA)
    if (buffer[3] != 0xAA) {
        std::cerr << "test_BasicBehavior: buffer[3] (skipped) unexpectedly modified to 0x"
                  << std::hex << static_cast<int>(buffer[3]) << std::dec << "\n";
        ok = false;
    }
    if (ret != start + 4) {
        std::cerr << "test_BasicBehavior: returned pointer should be start+4, got "
                  << static_cast<const void*>(ret) << " expected " << static_cast<const void*>(start + 4) << "\n";
        ok = false;
    }

    return ok;
}

static bool test_Pack3BytesAndSkip1_SecondCallIndependence()
{
    // Purpose:
    // - Ensure that a second invocation with a fresh buffer also writes the correct bytes
    //   and advances the pointer by 4 from its start, independent of previous state.

    // Arrange
    cmsUInt16Number wOut[3] = { 0x0001, 0x0002, 0x0003 }; // Low bytes: 0x01, 0x02, 0x03
    cmsUInt8Number buffer[8];
    for (size_t i = 0; i < 8; ++i) buffer[i] = static_cast<cmsUInt8Number>(0x55);

    // Act
    cmsUInt8Number* ret = Pack3BytesAndSkip1(nullptr, wOut, buffer, 0);

    // Assert
    bool ok = true;
    if (buffer[0] != 0x01) {
        std::cerr << "test_SecondCallIndependence: buffer[0] expected 0x01, got 0x"
                  << std::hex << static_cast<int>(buffer[0]) << std::dec << "\n";
        ok = false;
    }
    if (buffer[1] != 0x02) {
        std::cerr << "test_SecondCallIndependence: buffer[1] expected 0x02, got 0x"
                  << std::hex << static_cast<int>(buffer[1]) << std::dec << "\n";
        ok = false;
    }
    if (buffer[2] != 0x03) {
        std::cerr << "test_SecondCallIndependence: buffer[2] expected 0x03, got 0x"
                  << std::hex << static_cast<int>(buffer[2]) << std::dec << "\n";
        ok = false;
    }
    if (buffer[3] != 0x55) {
        std::cerr << "test_SecondCallIndependence: buffer[3] should remain 0x55, got 0x"
                  << std::hex << static_cast<int>(buffer[3]) << std::dec << "\n";
        ok = false;
    }
    if (ret != buffer + 4) {
        std::cerr << "test_SecondCallIndependence: returned pointer should be buffer+4\n";
        ok = false;
    }

    return ok;
}

static bool test_Pack3BytesAndSkip1_ZeroInputs()
{
    // Purpose:
    // - Check behavior when input words are zero. The resulting bytes should be zero as low bytes.
    // - Ensure skip byte is still not overwritten.

    // Arrange
    cmsUInt16Number wOut[3] = { 0x0000, 0x0000, 0x0000 };
    cmsUInt8Number buffer[6];
    for (size_t i = 0; i < 6; ++i) buffer[i] = static_cast<cmsUInt8Number>(0x11);

    // Act
    cmsUInt8Number* ret = Pack3BytesAndSkip1(nullptr, wOut, buffer, 0);

    // Assert
    bool ok = true;
    if (buffer[0] != 0x00) {
        std::cerr << "test_ZeroInputs: buffer[0] expected 0x00, got 0x"
                  << std::hex << static_cast<int>(buffer[0]) << std::dec << "\n";
        ok = false;
    }
    if (buffer[1] != 0x00) {
        std::cerr << "test_ZeroInputs: buffer[1] expected 0x00, got 0x"
                  << std::hex << static_cast<int>(buffer[1]) << std::dec << "\n";
        ok = false;
    }
    if (buffer[2] != 0x00) {
        std::cerr << "test_ZeroInputs: buffer[2] expected 0x00, got 0x"
                  << std::hex << static_cast<int>(buffer[2]) << std::dec << "\n";
        ok = false;
    }
    if (buffer[3] != 0x11) {
        std::cerr << "test_ZeroInputs: buffer[3] should remain 0x11 (unchanged). Got 0x"
                  << std::hex << static_cast<int>(buffer[3]) << std::dec << "\n";
        ok = false;
    }
    if (ret != buffer + 4) {
        std::cerr << "test_ZeroInputs: returned pointer should be buffer+4\n";
        ok = false;
    }

    return ok;
}

int main()
{
    // Run all tests and report a concise summary. Non-terminating style: continue running all tests.
    int failed_count = 0;

    if (!test_Pack3BytesAndSkip1_BasicBehavior()) {
        std::cerr << "Test 1 failed: Basic behavior of Pack3BytesAndSkip1\n";
        ++failed_count;
    } else {
        std::cout << "Test 1 passed: Basic behavior of Pack3BytesAndSkip1\n";
    }

    if (!test_Pack3BytesAndSkip1_SecondCallIndependence()) {
        std::cerr << "Test 2 failed: Second call independence of Pack3BytesAndSkip1\n";
        ++failed_count;
    } else {
        std::cout << "Test 2 passed: Second call independence of Pack3BytesAndSkip1\n";
    }

    if (!test_Pack3BytesAndSkip1_ZeroInputs()) {
        std::cerr << "Test 3 failed: Zero inputs handling in Pack3BytesAndSkip1\n";
        ++failed_count;
    } else {
        std::cout << "Test 3 passed: Zero inputs handling in Pack3BytesAndSkip1\n";
    }

    if (failed_count > 0) {
        std::cout << "Summary: " << failed_count << " test(s) failed.\n";
        return 1;
    } else {
        std::cout << "Summary: All tests passed.\n";
        return 0;
    }
}