// cmsNewTag_unit_tests.cpp
// Unit tests for the focal method: cmsBool _cmsNewTag(_cmsICCPROFILE* Icc, cmsTagSignature sig, int* NewPos)
// This test suite is designed to be compiled and run as a C++11 program without Google Test.
// It directly calls the internal function prototype via extern "C" and exercises both branches:
// - Tag does not exist yet: creates a new slot (NewPos is assigned, TagCount incremented)
// - Tag already exists: deletes the existing tag and returns its position in NewPos
// - Overflow: when TagCount >= MAX_TABLE_TAG, the function returns FALSE and signals an error
//
// Note: This test relies on internal data structures from LittleCMS (lcms2). It is intended to be used
// in environments where the library sources are available and consistently compiled together.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Import internal types and constants from LittleCMS.
// These headers are part of the library's internal API. They expose _cmsICCPROFILE and related members.
// Depending on your build environment, you may need to adjust the include path to point to the exact header.

// Expose the internal function with C linkage so we can call it from C++.
extern "C" cmsBool _cmsNewTag(_cmsICCPROFILE* Icc, cmsTagSignature sig, int* NewPos);

// Helper macro-like simple test result printer
#define TEST_PASSED true
#define TEST_FAILED false

// Small helper to print booleans as PASS/FAIL
static const char* BoolToStr(cmsBool b) { return (b == TRUE) ? "TRUE" : "FALSE"; }

// Create a zeroed ICC profile structure for testing
static _cmsICCPROFILE* CreateZeroIccProfile()
{
    // Allocate and zero the memory for a _cmsICCPROFILE instance
    _cmsICCPROFILE* Icc = new _cmsICCPROFILE();
    if (!Icc) return nullptr;
    std::memset(Icc, 0, sizeof(_cmsICCPROFILE));

    // Initialize essential fields used by _cmsNewTag
    Icc->ContextID = 0;  // typical default
    Icc->TagCount = 0;   // start with an empty tag table
    // Other fields (TagNames, TagPtrs, etc.) are part of the struct and remain zeroed by memset
    return Icc;
}

// Simple test harness: run tests and report
int main()
{
    int total_tests = 0;
    int passed_tests = 0;

    // Test 1: No existing tag, should create a new slot (else-branch)
    {
        total_tests++;
        _cmsICCPROFILE* Icc = CreateZeroIccProfile();
        if (Icc == nullptr) {
            std::cerr << "Test 1: Failed to allocate ICC profile." << std::endl;
            goto END;
        }

        cmsTagSignature testSig = 0x01020304; // arbitrary signature
        int NewPos = -1;

        cmsBool res = _cmsNewTag(Icc, testSig, &NewPos);

        bool pass = (res == TRUE) && (NewPos == 0) && (Icc->TagCount == 1) && (Icc->TagNames[0] == testSig);
        std::cout << "Test 1 - No existing tag creates new slot: " << BoolToStr(res) 
                  << ", NewPos=" << NewPos
                  << ", TagCount=" << Icc->TagCount
                  << ", TagNames[0]=" << std::hex << Icc->TagNames[0] << std::dec
                  << " [Expected TRUE, NewPos=0, TagCount=1, TagNames[0]==sig]" << std::endl;

        if (pass) passed_tests++;
        delete Icc;
    }

    // Test 2: Tag already exists: should delete existing tag and return its position via NewPos
    {
        total_tests++;
        _cmsICCPROFILE* Icc = CreateZeroIccProfile();
        if (Icc == nullptr) {
            std::cerr << "Test 2: Failed to allocate ICC profile." << std::endl;
            goto END;
        }

        // Pre-populate a tag slot that _cmsSearchTag will find
        cmsTagSignature existingSig = 0xAABBCCDD;
        Icc->TagCount = 1;
        Icc->TagNames[0] = existingSig; // pretend a tag already exists at position 0

        int NewPos = -1;
        cmsBool res = _cmsNewTag(Icc, existingSig, &NewPos);

        // We expect the function to find i>=0, delete that tag by position, and return i through NewPos.
        bool pass = (res == TRUE) && (NewPos == 0);
        std::cout << "Test 2 - Existing tag is found and deleted: " << BoolToStr(res)
                  << ", NewPos=" << NewPos
                  << ", Post-TagCount=" << Icc->TagCount
                  << " [Expected TRUE, NewPos=0, TagCount may decrease or stay depending on deletion logic]" << std::endl;

        if (pass) passed_tests++;
        delete Icc;
    }

    // Test 3: Overflow scenario: TagCount already at MAX_TABLE_TAG should fail
    {
        total_tests++;
        _cmsICCPROFILE* Icc = CreateZeroIccProfile();
        if (Icc == nullptr) {
            std::cerr << "Test 3: Failed to allocate ICC profile." << std::endl;
            goto END;
        }

        // Simulate full tag table
        Icc->TagCount = MAX_TABLE_TAG;
        int NewPos = -1;
        cmsTagSignature anySig = 0x12345678;
        cmsBool res = _cmsNewTag(Icc, anySig, &NewPos);

        bool pass = (res == FALSE);
        std::cout << "Test 3 - Overflow when TagCount >= MAX_TABLE_TAG: " << BoolToStr(res)
                  << ", NewPos=" << NewPos << " [Expected FALSE]" << std::endl;

        if (pass) passed_tests++;
        delete Icc;
    }

END:
    // Summary
    std::cout << "Unit tests completed: " << passed_tests << " / " << total_tests << " tests PASSED." << std::endl;
    return (passed_tests == total_tests) ? 0 : 1;
}