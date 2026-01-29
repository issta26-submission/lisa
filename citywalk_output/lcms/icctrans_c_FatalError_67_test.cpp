// File: lcms2_stub.h
#ifndef LCMS2_STUB_H
#define LCMS2_STUB_H

// Minimal stub definitions to allow compilation of icctrans.c in a non-MATLAB environment.
// This is only for unit testing FatalError; it does not implement full functionality.

typedef int cmsBool;
#define FALSE 0
#define TRUE 1

typedef unsigned int cmsUInt32Number;
typedef void* cmsHPROFILE;
typedef unsigned long cmsColorSpaceSignature;
typedef unsigned long cmsContext;

#endif // LCMS2_STUB_H


// File: mex_mock.cpp
#include <cstring>
#include <mex.h>
#include <stdarg.h>
#include <iostream>
#include <string>
#include <lcms2.h>
#include <string.h>


// Forward declaration to be consistent with icctrans.c expectations.
// The real mex.h defines this; we provide a C-style function here to intercept calls.
extern "C" void mexErrMsgTxt(const char* error);

// Global storage for the last captured MATLAB-like error message.
static std::string g_last_mex_error;

// Mock implementation of the MATLAB mexErrMsgTxt to capture error messages without terminating the program.
extern "C" void mexErrMsgTxt(const char* error) {
    if (error) {
        g_last_mex_error = error;
    } else {
        g_last_mex_error.clear();
    }
}

// Provide a helper for tests to read the last error message.
extern "C" const char* get_last_mex_message() {
    // Return a pointer to internal std::string data for simple checks in tests.
    // Note: This pointer becomes invalid if g_last_mex_error is modified; tests should use immediately after call.
    return g_last_mex_error.c_str();
}

// Helper to reset captured message between tests.
extern "C" void reset_last_mex_message() {
    g_last_mex_error.clear();
}


// File: test_fatal_error.cpp

using std::string;

// Declarations of the production function under test.
// FatalError is implemented in icctrans.c with C linkage.
extern "C" cmsBool FatalError(const char* frm, ...);

// Accessors from mex_mock.cpp to inspect captured messages.
extern "C" const char* get_last_mex_message();
extern "C" void reset_last_mex_message();
extern "C" int // dummy to ensure compilation in some toolchains
{
};

// Test helper: run a single test and print result.
// Each test function returns true on pass, false on fail.
static bool runTest(const char* testName, bool (*testFunc)()) {
    std::cout << "[ RUN      ] " << testName << "\n";
    bool ok = testFunc();
    std::cout << "[ " << (ok ? "PASSED" : "FAILED") << " ] " << testName << "\n";
    return ok;
}

// Test 1: Ensure FatalError formats an integer correctly and returns FALSE.
static bool testFatalError_formatsInteger() {
    reset_last_mex_message();
    cmsBool ret = FatalError("Error: %d", 42);

    bool ok = true;
    if (ret != FALSE) {
        std::cerr << "ERROR: Expected FALSE return from FatalError when called with format string.\n";
        ok = false;
    }

    const char* msg = get_last_mex_message();
    if (msg == nullptr) {
        std::cerr << "ERROR: No mexErrMsgTxt message captured.\n";
        ok = false;
    } else {
        string s = msg;
        if (s != "Error: 42") {
            std::cerr << "ERROR: Expected message 'Error: 42', got '" << s << "'.\n";
            ok = false;
        }
    }

    return ok;
}

// Test 2: Ensure FatalError formats a pointer correctly and returns FALSE.
// We only check prefix and that a hex-like representation exists.
static bool testFatalError_formatsPointer() {
    reset_last_mex_message();
    cmsBool ret = FatalError("Ptr: %p", (void*)0x1234);

    bool ok = true;
    if (ret != FALSE) {
        std::cerr << "ERROR: Expected FALSE return from FatalError when called with pointer format.\n";
        ok = false;
    }

    const char* msg = get_last_mex_message();
    if (msg == nullptr) {
        std::cerr << "ERROR: No mexErrMsgTxt message captured.\n";
        ok = false;
    } else {
        string s = msg;
        // Expect the message to start with "Ptr: "
        if (s.substr(0, 5) != "Ptr: ") {
            std::cerr << "ERROR: Message does not start with 'Ptr: '. Got '" << s << "'.\n";
            ok = false;
        }
        // Expect a hexadecimal-like pointer representation, typically containing 0x
        if (s.find("0x") == string::npos) {
            std::cerr << "ERROR: Message does not contain hexadecimal pointer representation. Got '" << s << "'.\n";
            ok = false;
        }
    }

    return ok;
}

// Test 3: Ensure FatalError with no format specifiers works as expected.
static bool testFatalError_noFormatSpecifiers() {
    reset_last_mex_message();
    cmsBool ret = FatalError("Fixed message");

    bool ok = true;
    if (ret != FALSE) {
        std::cerr << "ERROR: Expected FALSE return from FatalError for fixed message.\n";
        ok = false;
    }

    const char* msg = get_last_mex_message();
    if (msg == nullptr) {
        std::cerr << "ERROR: No mexErrMsgTxt message captured.\n";
        ok = false;
    } else {
        string s = msg;
        if (s != "Fixed message") {
            std::cerr << "ERROR: Expected message 'Fixed message', got '" << s << "'.\n";
            ok = false;
        }
    }

    return ok;
}

int main() {
    std::cout << "Starting FatalError unit test suite for icctrans.c...\n";

    int total = 0;
    int passed = 0;

    total++; if (runTest("FatalError_formatsInteger", testFatalError_formatsInteger)) passed++;
    total++; if (runTest("FatalError_formatsPointer", testFatalError_formatsPointer)) passed++;
    total++; if (runTest("FatalError_noFormatSpecifiers", testFatalError_noFormatSpecifiers)) passed++;

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}