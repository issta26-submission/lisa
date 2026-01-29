// High-quality C++11 unit tests for the focal method: NULLSeek
// Focal method under test: cmsBool NULLSeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset)
// The test harness uses the actual library headers and links against cmsio0.c (C code).
// The tests are written to be non-terminating: they report failures but continue execution.
// Explanatory comments accompany each test case.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>


// Include library internal definitions to get cmsIOHANDLER, cmsUInt32Number, FILENULL, etc.
// This header is part of the cms (LittleCMS) internal API and provides necessary types.
// The test assumes lcms2_internal.h is available in the include path.
extern "C" {
}

// Simple non-terminating test harness
static int g_failures = 0;

// Lightweight assertion helpers (non-terminating)
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << __FUNCTION__ << ": Expected true: " #cond " is false." << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    if( (a) != (b) ) { \
        std::cerr << "[FAIL] " << __FUNCTION__ << ": Expected " #a " == " #b \
                  << ", got " << (a) << " != " << (b) << "." << std::endl; \
        ++g_failures; \
    } \
} while(0)


// Test 1: Validate that NULLSeek sets the internal FILENULL Pointer to the requested offset
// and returns a non-zero cmsBool (TRUE). We test several representative offsets.
void test_NULLSeek_sets_pointer_and_returns_true() {
    // Arrange: obtain a cmsIOHANDLER instance and a FILENULL instance.
    // The library public headers expose cmsIOHANDLER and FILENULL.
    cmsIOHANDLER ioh;
    FILENULL res;

    // Link the stream to our memory object so NULLSeek can modify the Pointer field.
    ioh.stream = &res;

    // Offsets to test (cover 0, small, moderate, and max 32-bit unsigned range)
    const cmsUInt32Number test_offsets[] = {
        0u,
        1u,
        12345u,
        0xFFFFFFFFu  // maximum 32-bit unsigned value
    };

    for (cmsUInt32Number offset : test_offsets) {
        // Act: call the focal method
        cmsBool ret = NULLSeek(&ioh, offset);

        // Assert: return value is non-zero (TRUE) and Pointer field updated correctly
        EXPECT_TRUE(ret != 0);
        EXPECT_EQ(res.Pointer, offset);
    }
}

// Entry point to run all tests
int main() {
    std::cout << "Starting tests for cmsBool NULLSeek(cmsIOHANDLER*, cmsUInt32Number)" << std::endl;

    test_NULLSeek_sets_pointer_and_returns_true();

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}