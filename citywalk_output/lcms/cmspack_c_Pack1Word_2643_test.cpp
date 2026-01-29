// Unit test suite for the Pack1Word function in cmspack.c
// This test suite is designed for a C++11 environment without GTest.
// It uses a minimal, self-contained test harness and relies on the
// same types and macros as the CMS/LCMS project (e.g., cmsUInt8Number,
// cmsUInt16Number, cmsUInt32Number, CMSREGISTER, _cmsTRANSFORM).
// The tests assume the project headers are available in the include path.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Import project internal definitions to align types/macros

// Declare the focal function with C linkage so we can link against cmspack.c
extern "C" cmsUInt8Number* Pack1Word(CMSREGISTER _cmsTRANSFORM* info,
                          CMSREGISTER cmsUInt16Number wOut[],
                          CMSREGISTER cmsUInt8Number* output,
                          CMSREGISTER cmsUInt32Number Stride);

// Simple non-terminating test harness
struct TestCase {
    const char* name;
    bool (*func)();
};

static int g_failures = 0;

// Test 1: Basic behavior - writes the first 16-bit word to output and returns pointer to output+2
bool Test_Pack1Word_WritesFirstWord_Basic() {
    // Prepare a fake transform info (unused by Pack1Word, but provided for signature compatibility)
    _cmsTRANSFORM* info = nullptr;

    // Input word
    cmsUInt16Number wOut[1] = { 0xCAFE };

    // Output buffer (2 bytes exactly)
    cmsUInt8Number buffer[2] = { 0x00, 0x00 };

    // Call the focal function
    cmsUInt8Number* ret = Pack1Word(info, wOut, buffer, 0);

    // Read back the 16-bit value written into the buffer.
    cmsUInt16Number written = *(cmsUInt16Number*)buffer;

    // Expected: value equal to wOut[0], and return pointer equals buffer + 2
    bool ok = (written == wOut[0]) && (ret == buffer + 2);
    if (!ok) {
        std::cout << "Test_Pack1Word_WritesFirstWord_Basic failed: "
                  << "buffer=0x" << std::hex << written
                  << ", expected=0x" << wOut[0]
                  << ", ret_ptr_expected=0x" << reinterpret_cast<void*>(buffer + 2)
                  << ", ret_ptr_actual=0x" << reinterpret_cast<void*>(ret)
                  << std::dec << "\n";
    }
    return ok;
}

// Test 2: Zero value handling - ensure 0 is stored correctly
bool Test_Pack1Word_WritesZeroValue() {
    _cmsTRANSFORM* info = nullptr;
    cmsUInt16Number wOut[1] = { 0x0000 };
    cmsUInt8Number buffer[2] = { 0xAA, 0x55 };

    cmsUInt8Number* ret = Pack1Word(info, wOut, buffer, 0);

    cmsUInt16Number written = *(cmsUInt16Number*)buffer;
    bool ok = (written == wOut[0]) && (ret == buffer + 2);
    if (!ok) {
        std::cout << "Test_Pack1Word_WritesZeroValue failed: buffer=0x"
                  << std::hex << written << ", expected=0x" << wOut[0]
                  << ", ret_ptr=0x" << reinterpret_cast<void*>(ret)
                  << std::dec << "\n";
    }
    return ok;
}

// Test 3: Only uses the first element of wOut; second element (if any) must be ignored
bool Test_Pack1Word_OnlyFirstElementUsed() {
    _cmsTRANSFORM* info = nullptr;
    cmsUInt16Number wOut[2] = { 0x1234, 0xABCD };
    cmsUInt8Number buffer[2] = { 0 };

    cmsUInt8Number* ret = Pack1Word(info, wOut, buffer, 0);

    cmsUInt16Number written = *(cmsUInt16Number*)buffer;
    bool ok = (written == wOut[0]) && (ret == buffer + 2);

    // Ensure second value did not affect the output (i.e., memory wrote only using wOut[0])
    if (!ok) {
        std::cout << "Test_Pack1Word_OnlyFirstElementUsed failed: buffer=0x"
                  << std::hex << written << ", expected=0x" << wOut[0]
                  << ", ret_ptr=0x" << reinterpret_cast<void*>(ret) << std::dec << "\n";
    } else {
        // Additionally verify that wOut[1] did not alter the memory after the first two bytes (there are only 2 bytes anyway)
        // This is a sanity check; since output has exactly two bytes, this test is primarily about not using wOut[1].
    }
    return ok;
}

// Helper to run all tests
void RunAllTests() {
    struct TestCase tests[] = {
        { "Test_Pack1Word_WritesFirstWord_Basic", Test_Pack1Word_WritesFirstWord_Basic },
        { "Test_Pack1Word_WritesZeroValue", Test_Pack1Word_WritesZeroValue },
        { "Test_Pack1Word_OnlyFirstElementUsed", Test_Pack1Word_OnlyFirstElementUsed },
    };

    const int total = sizeof(tests) / sizeof(tests[0]);
    int failed = 0;
    for (int i = 0; i < total; ++i) {
        g_failures = 0;
        bool result = tests[i].func();
        if (!result) {
            ++failed;
        }
        // Non-terminating assertion: continue running all tests even if one fails.
    }

    if (failed == 0) {
        std::cout << "All tests passed for Pack1Word.\n";
    } else {
        std::cout << failed << " test(s) failed for Pack1Word.\n";
        g_failures = failed;
    }
}

int main() {
    // Run the unit tests
    RunAllTests();
    return g_failures;
}