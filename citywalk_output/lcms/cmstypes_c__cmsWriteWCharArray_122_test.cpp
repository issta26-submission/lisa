// File: test_cmsWriteWCharArray.cpp
// Purpose: A self-contained, high-coverage unit test suite for the focal
//          function _cmsWriteWCharArray as declared in cmstypes.c.
//          The tests are designed to be compiled with a C++11 compiler
//          and without Google Test (GTest). A lightweight in-file test
//          harness is used to report results.
//
// Notes and constraints (as per the task requirements):
// - We exercise the core behavior: writing n 16-bit units corresponding to
//   wchar_t elements via _cmsWriteUInt16Number(io, value).
// - We cover true/false branches by simulating successful and failing writes.
// - We test edge cases: n == 0, non-NULL/NULL Array combos per the assertion
//   in the code (in release builds, _cmsAssert is often a no-op; in debug
//   builds it can abort; here we avoid crashing by not triggering asserts that
//   would abort in typical environments).
// - We use only the C++ standard library to implement the mock IO and test
//   harness; we do not rely on GoogleTest or other external testing libraries.
// - We assume a typical LittleCMS-like interface in which _cmsWriteUInt16Number
//   writes 2 bytes (the low and high byte of the 16-bit value) to the provided
//   io handler through its Write callback. The tests implement a minimal mock
//   of the IO layer to capture written bytes.
// - Static/internal helpers of the focal module are not accessed directly in tests;
//   we interact only via the focal function and its public-like IO interface.
// - The tests are designed to be portable across platforms (where wchar_t may be
//   16-bit or 32-bit) by validating the 16-bit truncation semantics of the
//   focal function as implemented (i.e., (cmsUInt16Number) Array[i]). This
//   ensures we exercise both the content-domain and length-domain of the code.
// - The code path under test uses io != NULL and Array != NULL || n == 0
//   (the latter is explicitly asserted when Array == NULL and n > 0). Our tests
//   avoid triggering hard asserts to keep the tests executable in release builds
//   while still exploring the behavior around these predicates.
//
// Implementation detail: The test suite includes a tiny mock for a CMS IO handler
// that records written bytes. We do not modify the focal function; instead we
// provide a compatible mock environment that mimics the behavior required by
// _cmsWriteUInt16Number, allowing us to verify that _cmsWriteWCharArray calls the
// Write callback exactly n times with the correct 16-bit representations.

#include <cstring>
#include <vector>
#include <iostream>
#include <cwchar>
#include <cassert>
#include <lcms2_internal.h>
#include <cstdint>


// Forward declare the types and function from the focal module's header.
// In a real environment, include the actual header, e.g. #include "lcms2.h"
// or the relevant cmstypes header. For the purposes of this test harness,
// we declare minimal, compatible types to enable compilation in a typical
// LittleCMS-like environment.

extern "C" {

// Typedefs commonly found in LittleCMS-like codebases (simplified for test).
typedef int cmsBool;
typedef unsigned int cmsUInt32Number;
typedef unsigned short cmsUInt16Number;
typedef struct cmsIOHANDLER cmsIOHANDLER;

// The focal function under test.
// Signature assumed from the provided snippet.
cmsBool _cmsWriteWCharArray(cmsIOHANDLER* io, cmsUInt32Number n, const wchar_t* Array);

// The helper function used by _cmsWriteWCharArray to write 16-bit values.
// We declare it here so the linker can resolve it from the real library in
// a typical environment. In our tests, we provide a mock implementation
// in terms of the assumed prototype.
cmsBool _cmsWriteUInt16Number(cmsIOHANDLER* io, cmsUInt16Number v);

} // extern "C"

// Lightweight mock IO infrastructure to capture writes.
// This mock mirrors the minimal interface that a cmsIOHANDLER would expose
// (in particular, a Write callback that _cmsWriteUInt16Number will invoke).
// We simulate a Write callback by placing a pointer to this MockIO in the
// io->UserData field (assuming such a field exists in the real struct).
// Since the exact layout of cmsIOHANDLER is not guaranteed here, this test
// harness provides a compatible stand-in for environments where io->Write
// calls are routed through the provided io object.
struct MockIO {
    // Accumulated output bytes written by the focal function via the 16-bit writer.
    std::vector<uint8_t> writtenBytes;

    // Config: when true, simulate a failure on the second write attempt.
    bool failOnSecondWrite;

    // Count of how many times a 16-bit value has been written.
    size_t writeCount;

    MockIO() : failOnSecondWrite(false), writeCount(0) {}

    // Helper to reset for a new test case.
    void reset() {
        writtenBytes.clear();
        failOnSecondWrite = false;
        writeCount = 0;
    }
};

// Our fake io structure used by the focal function.
// This is a minimal stand-in and is not meant to be feature-complete.
// It intentionally mirrors just enough to support the focal tests.
struct cmsIOHANDLER {
    // In the real library, this struct would contain function pointers to
    // Read/Write operations and possibly a context. We simulate the Write
    // path by providing a UserData pointer that the mock Write function can
    // interpret.
    void* UserData;
};

// We provide a mock implementation of _cmsWriteUInt16Number that operates on
// our MockIO instance. In a real environment, _cmsWriteUInt16Number will be
// resolved to the library function and will call io->Write(...) with 2 bytes.
// Here, we implement the same high-level contract: on success, return TRUE;
// on configured failure, return FALSE.
extern "C" cmsBool _cmsWriteUInt16Number(cmsIOHANDLER* io, cmsUInt16Number v) {
    // Resolve the MockIO pointer from the io->UserData field (if present).
    MockIO* m = static_cast<MockIO*>(io->UserData);
    if (!m) {
        // If the test didn't wire a mock, fail gracefully.
        return 0; // FALSE
    }

    // Simulate a 2-byte write in little-endian order: low byte first, then high byte.
    // This mirrors typical 16-bit write semantics in many CMS implementations.
    uint8_t low = static_cast<uint8_t>(v & 0xFF);
    uint8_t high = static_cast<uint8_t>((v >> 8) & 0xFF);

    // If configured to fail on the second write, emulate a failure on the second call.
    if (m->failOnSecondWrite && m->writeCount == 1) {
        // Do not advance writeCount to indicate a failure on this write.
        return 0; // FALSE
    }

    // Record two bytes to the buffer.
    m->writtenBytes.push_back(low);
    m->writtenBytes.push_back(high);
    m->writeCount++;
    return 1; // TRUE
}

// Note: _cmsWriteWCharArray's implementation from cmstypes.c is assumed to be
// linked with this test. We do not re-implement it here; we rely on the
// actual focal function. The MockIO wiring below ensures that writes go
// through our _cmsWriteUInt16Number mock, which records bytes into
// MockIO::writtenBytes.


// Helper: convert a C wide string to a std::wstring for tests (platform-safe).
static std::wstring to_wstring(const wchar_t* s) {
    if (!s) return std::wstring();
    return std::wstring(s);
}

// Test harness utilities

static void printHeader(const char* testName) {
    std::cout << "TEST: " << testName << std::endl;
}

static void testPass(const char* msg) {
    std::cout << "[PASS] " << msg << std::endl;
}

static void testFail(const char* msg) {
    std::cerr << "[FAIL] " << msg << std::endl;
}

// Test 1: Zero-length array should succeed without writing anything.
static void test_CmsWriteWCharArray_ZeroLength_NoWrites() {
    printHeader("Zero-length array (n == 0) should succeed and write nothing");

    // Prepare a fake IO and hook it up to the focal function.
    MockIO mock;
    cmsIOHANDLER io;
    io.UserData = &mock;

    mock.reset();
    // Call the focal function with n = 0 and a NULL array (permitted by the code path).
    // Note: For the test to be robust, we pass a non-null io to simulate a real environment.
    const wchar_t* arr = nullptr;
    cmsUInt32Number n = 0;

    // _cmsWriteWCharArray is provided by the library; in our harness, it is linked.
    cmsBool result = _cmsWriteWCharArray(&io, n, arr);

    if (result == 1 && mock.writtenBytes.empty()) {
        testPass("Zero-length write returned TRUE and no bytes were written.");
    } else {
        testFail("Zero-length write did not behave as expected.");
    }
}

// Test 2: Normal path with 3 characters should write 6 bytes (3 * 2) in correct order.
static void test_CmsWriteWCharArray_NormalPath_WritesBytesCorrectly() {
    printHeader("Normal path with 3 wchar_t elements writes 6 bytes in little-endian order");

    MockIO mock;
    cmsIOHANDLER io;
    io.UserData = &mock;

    mock.reset();

    // 3 characters: L'A' (0x0041), L'B' (0x0042), L'C' (0x0043)
    const wchar_t arr[] = { L'A', L'B', L'C' };
    cmsUInt32Number n = 3;

    cmsBool result = _cmsWriteWCharArray(&io, n, arr);

    // Expect success and 6 bytes: 0x41 0x00, 0x42 0x00, 0x43 0x00
    std::vector<uint8_t> expected = { 0x41, 0x00, 0x42, 0x00, 0x43, 0x00 };

    if (result == 1 && mock.writtenBytes == expected) {
        testPass("Normal path wrote 6 bytes in correct little-endian order for 3 chars.");
    } else {
        testFail("Normal path did not write expected bytes or returned FALSE.");
        // Helpful diagnostic
        std::cout << "  Expected: ";
        for (auto b : expected) std::cout << std::hex << static_cast<int>(b) << ' ';
        std::cout << "\n  Actual:   ";
        for (auto b : mock.writtenBytes) std::cout << std::hex << static_cast<int>(b) << ' ';
        std::cout << std::dec << "\n";
    }
}

// Test 3: Failure path where _cmsWriteUInt16Number reports a failure on the second write.
static void test_CmsWriteWCharArray_FailureOnSecondWrite() {
    printHeader("Failure on second write should return FALSE and stop after first write");

    MockIO mock;
    cmsIOHANDLER io;
    io.UserData = &mock;

    mock.reset();
    mock.failOnSecondWrite = true;

    // 2 characters: L'X'(0x0058), L'Y'(0x0059)
    const wchar_t arr[] = { L'X', L'Y' };
    cmsUInt32Number n = 2;

    cmsBool result = _cmsWriteWCharArray(&io, n, arr);

    // Expect FALSE due to simulated failure on second write (there are two writes)
    if (result == 0) {
        // Ensure first write occurred (2 bytes) and second write failed, so total bytes = 2
        if (mock.writeCount == 1 && mock.writtenBytes.size() == 2) {
            testPass("Failure on second write correctly returned FALSE with partial data recorded.");
        } else {
            testFail("Failure occurred but internal write accounting did not match expectations.");
        }
    } else {
        testFail("Expected FALSE due to simulated write failure, but got TRUE.");
    }
}

// Test 4: Edge case where wchar values exceed 0xFFFF are truncated to 16-bit.
// This mirrors the explicit cast in the focal function and ensures truncation is exercised.
static void test_CmsWriteWCharArray_WcharValueTruncation() {
    printHeader("Wchar_t values exceeding 0xFFFF are truncated to 16-bit before writing");

    MockIO mock;
    cmsIOHANDLER io;
    io.UserData = &mock;

    mock.reset();

    // Two characters with high values: 0x12345 and 0x0FED
    // On 0x12345, lower 16 bits are 0x2345. For 0x0FED, it's 0x0FED.
    const wchar_t arr[] = { static_cast<wchar_t>(0x12345), static_cast<wchar_t>(0x0FED) };
    cmsUInt32Number n = 2;

    cmsBool result = _cmsWriteWCharArray(&io, n, arr);

    // Expect TRUE (assuming _cmsWriteUInt16Number halves to 16-bit and writes both)
    // and the two bytes for 0x2345 then 0x0FED.
    std::vector<uint8_t> expected = {
        static_cast<uint8_t>(0x45), static_cast<uint8_t>(0x23), // 0x2345 -> little-endian
        static_cast<uint8_t>(0xED), static_cast<uint8_t>(0x0F)  // 0x0FED -> little-endian
    };

    if (result == 1 && mock.writtenBytes == expected) {
        testPass("Wchar value truncation into 16-bit writes produced expected byte sequence.");
    } else {
        testFail("Wchar truncation path did not produce expected bytes or returned FALSE.");
        std::cout << "  Expected: ";
        for (auto b : expected) std::cout << std::hex << static_cast<int>(b) << ' ';
        std::cout << "\n  Actual:   ";
        for (auto b : mock.writtenBytes) std::cout << std::hex << static_cast<int>(b) << ' ';
        std::cout << std::dec << "\n";
    }
}

// Main driver: run all tests and report results.
int main() {
    test_CmsWriteWCharArray_ZeroLength_NoWrites();
    test_CmsWriteWCharArray_NormalPath_WritesBytesCorrectly();
    test_CmsWriteWCharArray_FailureOnSecondWrite();
    test_CmsWriteWCharArray_WcharValueTruncation();

    std::cout << "All tests executed." << std::endl;
    return 0;
}