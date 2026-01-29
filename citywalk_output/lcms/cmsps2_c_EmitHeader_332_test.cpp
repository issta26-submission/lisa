/*
   cmsps2 EmitHeader Unit Tests (C++11, no GTest)

   This test suite targets the EmitHeader function defined in cmsps2.c.
   It uses a lightweight, self-contained harness with minimal stubs to
   exercise the output produced by EmitHeader under different tag-present
   scenarios.

   Notes and design decisions:
   - We assume the test run environment compiles cmsps2.c along with this test,
     with the LittleCMS-related headers available (lcms2_internal.h, etc.).
   - We provide C-callable stubs for _cmsIOPrintf, cmsReadTag, cmsMLUgetASCII,
     and RemoveCR so EmitHeader's interactions can be observed without
     requiring a full LittleCMS runtime.
   - We rely on a global string to capture the textual output of _cmsIOPrintf.
   - We test two scenarios:
     1) No profile tags (both Description and Copyright are absent).
     2) Both Description and Copyright tags present and return ASCII strings.
   - Assertions are basic and use simple string containment checks to verify the
     essential structure and content of the emitted header.
   - This test uses a minimal main() entry point; no external test framework is used.

   Important: This code is designed to be compiled together with cmsps2.c in a C++
   project configured for C++11. If your build system uses a separate test
   executable, ensure the C linkage for the mocked functions _cmsIOPrintf,
   cmsReadTag, cmsMLUgetASCII, and RemoveCR is preserved (extern "C").
*/

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstdarg>
#include <lcms2_internal.h>
#include <ctime>



// Global output capture for _cmsIOPrintf
static std::string g_output;

// Flags to simulate presence/absence of profile tags
static bool g_desc_present = false;
static bool g_cpy_present  = false;

// Forward declare the focal function (C linkage)
extern "C" void EmitHeader(cmsIOHANDLER* m, const char* Title, cmsHPROFILE hProfile);

// --------------------------------------------------------------------------------
// Mocked/Stubbed dependency implementations (C linkage) to support EmitHeader
// --------------------------------------------------------------------------------

// Mock: cmsReadTag - simulate presence/absence of tags based on global flags
extern "C" cmsMLU* cmsReadTag(cmsHPROFILE /*hProfile*/, cmsTagSignature tag)
{
    // When tag is ProfileDescriptionTag, return sentinel if enabled
    if (tag == cmsSigProfileDescriptionTag) {
        return g_desc_present ? (cmsMLU*)0x1 : NULL;
    }
    // When tag is CopyrightTag, return sentinel if enabled
    if (tag == cmsSigCopyrightTag) {
        return g_cpy_present ? (cmsMLU*)0x2 : NULL;
    }
    return NULL;
}

// Mock: cmsMLUgetASCII - write predefined ASCII for each sentinel pointer
extern "C" void cmsMLUgetASCII(cmsMLU* mlu, cmsUInt32Number /*lang*/, cmsUInt32Number /*country*/, char* buffer, cmsUInt32Number len)
{
    if (!buffer || len == 0) return;
    if (mlu == (cmsMLU*)0x1) {
        // Description tag ASCII
        const char* s = "DescFromTag";
        std::strncpy(buffer, s, (size_t)len);
        buffer[len - 1] = '\0';
    } else if (mlu == (cmsMLU*)0x2) {
        // Copyright tag ASCII
        const char* s = "CopyrightFromTag";
        std::strncpy(buffer, s, (size_t)len);
        buffer[len - 1] = '\0';
    } else {
        // Unknown pointer; ensure empty string
        buffer[0] = '\0';
        if (len > 1) buffer[1-1] = '\0';
    }
}

// Mock: RemoveCR - in tests, simply return the same string (no CR handling)
extern "C" char* RemoveCR(const char* txt)
{
    // Cast away constness for compatibility with the function signature in the code under test
    return (char*)txt;
}

// Mock: _cmsIOPrintf - accumulate formatted output into g_output
extern "C" int _cmsIOPrintf(cmsIOHANDLER* /*m*/, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buf[1024];
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    g_output.append(buf);
    return (int)strlen(buf);
}

// --------------------------------------------------------------------------------
// Tests
// --------------------------------------------------------------------------------

// Helper: simple assertion for strings
static void AssertContains(const std::string& haystack, const std::string& needle, const std::string& msg)
{
    if (haystack.find(needle) == std::string::npos) {
        std::cerr << "Assertion failed: " << msg << "\n"
                  << "Expected to find: \"" << needle << "\"\n"
                  << "In output: \"" << haystack << "\"\n";
        std::exit(1);
    }
}

// Test 1: EmitHeader with no tag-derived ASCII content (Description and Copyright absent)
static void Test_EmitHeader_NoTags()
{
    g_output.clear();
    g_desc_present = false;
    g_cpy_present  = false;

    // Call the function under test
    EmitHeader(nullptr, "TestTitle_NoTags", nullptr);

    // Validate structure and content
    AssertContains(g_output, "%%!PS-Adobe-3.0", "Header should start with PS header");
    AssertContains(g_output, "%%", "Header should contain a comment line");
    AssertContains(g_output, "%% TestTitle_NoTags", "Title should be echoed in header");
    AssertContains(g_output, "%% Source: ", "Source line should be present (empty when no tags)");
    AssertContains(g_output, "%% Created: ", "Created timestamp line should be present");
    AssertContains(g_output, "%%%%BeginResource", "End of header should begin resource section");
}

// Test 2: EmitHeader with Description and Copyright present (ASCII content provided)
static void Test_EmitHeader_WithTags()
{
    g_output.clear();
    g_desc_present = true;
    g_cpy_present  = true;

    EmitHeader(nullptr, "TestTitle_WithTags", nullptr);

    // Validate title and inclusion of tag-derived ASCII content
    AssertContains(g_output, "%% TestTitle_WithTags", "Title should be echoed with provided title");
    AssertContains(g_output, "%% Source: DescFromTag", "Description ASCII should be included in Source line");
    AssertContains(g_output, "%%         CopyrightFromTag", "Copyright ASCII should be included on its own line");
    AssertContains(g_output, "%% Created: ", "Created timestamp line should be present");
    AssertContains(g_output, "%%%%BeginResource", "End of header should begin resource section");
}

// --------------------------------------------------------------------------------
// Main: run tests
// --------------------------------------------------------------------------------

int main()
{
    // Run tests exercising the EmitHeader function under different tag conditions
    Test_EmitHeader_NoTags();
    Test_EmitHeader_WithTags();

    std::cout << "All EmitHeader tests passed.\n";
    return 0;
}