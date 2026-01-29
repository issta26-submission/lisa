// Test suite for the focal method EmitXYZ2Lab in cmsps2.c
// This test uses a memory-based CMS IO handler to capture the PostScript code emitted
// by EmitXYZ2Lab and compares it against the expected output.
// The test is written in plain C++11 without GTest, and uses a small, non-terminating
// assertion framework implemented in this file.

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <lcms2.h>


// Forward declaration of the focal function from cmsps2.c.
// The function uses the cmsIOHANDLER* type defined by the Little CMS library.
extern "C" void EmitXYZ2Lab(cmsIOHANDLER* m);

// Include Little CMS core header to get cmsIOHANDLER, cmsOpenIOhandlerFromMem, etc.
// This path may vary depending on the build environment. If your setup uses a different include path,
// adjust accordingly (e.g., #include "lcms2.h" or <lcms2.h>).

// Simple non-terminating assertion framework.
// It increments a global failure counter and prints a message but does not abort the test run.
static int gTestFailures = 0;

#define TEST_EXPECT_TRUE(cond) \
    do { if(!(cond)) { \
        fprintf(stderr, "TEST_FAIL: Condition failed at %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        ++gTestFailures; \
    } } while(0)

#define TEST_EXPECT_EQ(a, b) \
    do { if( (a) != (b) ) { \
        fprintf(stderr, "TEST_FAIL: Expected %ld, got %ld at %s:%d\n", (long)(b), (long)(a), __FILE__, __LINE__); \
        ++gTestFailures; \
    } } while(0)

#define TEST_EXPECT_STR_EQ(actual, expected) \
    do { if(std::strcmp((actual), (expected)) != 0) { \
        fprintf(stderr, "TEST_FAIL: Strings mismatch at %s:%d\nExpected: \"%s\"\nActual:   \"%s\"\n", __FILE__, __LINE__, (expected), (actual)); \
        ++gTestFailures; \
    } } while(0)


// Helper: Run test that EmitXYZ2Lab prints expected content to a memory-based IO handler.
static bool test_EmitXYZ2Lab_OutputMatches()
{
    // Allocate a reasonably large buffer to hold the emitted PostScript code.
    // The exact size of the emitted content is known from the focal method.
    const std::size_t BUF_SIZE = 8192;
    char* buffer = new char[BUF_SIZE];
    if (!buffer) {
        std::cerr << "Failed to allocate test buffer.\n";
        return false;
    }
    // Initialize buffer with zeros to make length/termination deterministic.
    std::memset(buffer, 0, BUF_SIZE);

    // Create a memory-based IO handler that writes into the provided buffer.
    // ContextID is typically cmsContext; passing NULL/0 is acceptable for most test scenarios.
    cmsIOHANDLER* m = cmsOpenIOhandlerFromMem(NULL, buffer, BUF_SIZE, "w");
    if (m == nullptr) {
        std::cerr << "Failed to create memory-based IO handler.\n";
        delete[] buffer;
        return false;
    }

    // Call the focal method. It should write exactly the expected PS code into the buffer.
    EmitXYZ2Lab(m);

    // Close the IO handler to finalize writes.
    cmsCloseIOhandler(m);

    // Determine how many bytes were actually written into the buffer.
    // Since we started with a zero-initialized buffer, the first null-terminator position gives the length.
    std::size_t actualLen = std::strnlen(buffer, BUF_SIZE);

    // Expected content exactly as emitted by the focal method.
    const char* expected =
        "/RangeLMN [ -0.635 2.0 0 2 -0.635 2.0 ]\n"
        "/EncodeLMN [\n"
        "{ 0.964200  div dup 0.008856 le {7.787 mul 16 116 div add}{1 3 div exp} ifelse } bind\n"
        "{ 1.000000  div dup 0.008856 le {7.787 mul 16 116 div add}{1 3 div exp} ifelse } bind\n"
        "{ 0.824900  div dup 0.008856 le {7.787 mul 16 116 div add}{1 3 div exp} ifelse } bind\n"
        "]\n"
        "/MatrixABC [ 0 1 0 1 -1 1 0 0 -1 ]\n"
        "/EncodeABC [\n"
        "{ 116 mul  16 sub 100 div  } bind\n"
        "{ 500 mul 128 add 256 div  } bind\n"
        "{ 200 mul 128 add 256 div  } bind\n"
        "]\n";

    // Verify exact length and exact content.
    std::size_t expectedLen = std::strlen(expected);
    TEST_EXPECT_EQ(actualLen, expectedLen);

    if (actualLen == expectedLen) {
        // Compare content exactly.
        TEST_EXPECT_STR_EQ(buffer, expected);
    }

    delete[] buffer;
    return (gTestFailures == 0);
}


// Entry point for the test suite.
// As per domain knowledge, run test methods from main when not using a test framework.
int main()
{
    int totalFailuresBefore = gTestFailures;

    // Run the primary test that checks the emitted content matches exactly.
    bool passed1 = test_EmitXYZ2Lab_OutputMatches();

    // Report results.
    if (passed1 && gTestFailures == totalFailuresBefore) {
        std::cout << "All tests passed for EmitXYZ2Lab_OutputMatches." << std::endl;
        return 0;
    } else {
        std::cerr << "EmitXYZ2Lab_OutputMatches tests failed with " << gTestFailures
                  << " failure(s)." << std::endl;
        return 1;
    }
}