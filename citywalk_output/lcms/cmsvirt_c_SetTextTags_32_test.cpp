// Test suite for SetTextTags in cmsvirt.c
// This test suite uses the real LCMS2 API to verify SetTextTags populates
// the Profile Description and Copyright tags as expected.
// Note: This test requires linking against the Little CMS library (lcms2).

#include <vector>
#include <iostream>
#include <string>
#include <cwchar>
#include <cassert>
#include <lcms2_internal.h>
#include <lcms2.h>



// Helper: Read the Profile Description MLU (en_US) from the profile.
// Returns true on success and fills descOut with the wide string.
static bool ReadProfileDescription(cmsHPROFILE hProfile, std::wstring &descOut)
{
    void* data = NULL;
    if (!cmsReadTag(hProfile, cmsSigProfileDescriptionTag, &data))
        return false;
    if (data == NULL)
        return false;

    const cmsMLU* mlu = (const cmsMLU*) data;
    // Buffer large enough for typical test strings
    wchar_t buffer[2048] = {0};
    // cmsMLUgetWide returns cmsBool (TRUE on success)
    if (!cmsMLUgetWide((cmsMLU*)mlu, "en", "US", buffer, (cmsUInt32Number) (sizeof(buffer)/sizeof(buffer[0]))))
        return false;

    descOut.assign(buffer);
    return true;
}

// Helper: Read the Copyright MLU (en_US) from the profile.
// Returns true on success and fills copyrightOut with the wide string.
static bool ReadCopyright(cmsHPROFILE hProfile, std::wstring &copyrightOut)
{
    void* data = NULL;
    if (!cmsReadTag(hProfile, cmsSigCopyrightTag, &data))
        return false;
    if (data == NULL)
        return false;

    const cmsMLU* mlu = (const cmsMLU*) data;
    wchar_t buffer[1024] = {0};
    if (!cmsMLUgetWide((cmsMLU*)mlu, "en", "US", buffer, (cmsUInt32Number)(sizeof(buffer)/sizeof(buffer[0]))))
        return false;

    copyrightOut.assign(buffer);
    return true;
}

// Simple test harness to report results without a full testing framework.
static int gFailures = 0;
#define FAIL_IF(cond, msg) if (!(cond)) { std::wcerr << L"TEST FAILURE: " << msg << std::endl; ++gFailures; }

// Test 1: Basic success path - ensure SetTextTags writes back the Description and Copyright tags correctly.
static void Test_SetTextTags_Basic()
{
    std::wcout << L"Running Test_SetTextTags_Basic..." << std::endl;

    // Create a placeholder profile (ContextID = 0)
    cmsContext ContextID = 0;
    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(ContextID);
    FAIL_IF(hProfile != NULL, L"cmsCreateProfilePlaceholder returned NULL.");

    const std::wstring inputDesc = L"Unit test: SetTextTags basic path";

    // Call the focal function
    cmsBool rc = SetTextTags(hProfile, inputDesc.c_str());
    FAIL_IF(rc != TRUE, L"SetTextTags did not return TRUE on valid input.");

    // Read back Description tag
    std::wstring readDesc;
    bool okDesc = ReadProfileDescription(hProfile, readDesc);
    FAIL_IF(!okDesc, L"Failed to read Profile Description tag after SetTextTags.");
    FAIL_IF(readDesc != inputDesc, L"Profile Description content mismatch. Expected: " + inputDesc + L" Got: " + readDesc);

    // Read back Copyright tag
    std::wstring readCopyright;
    bool okCopyright = ReadCopyright(hProfile, readCopyright);
    FAIL_IF(!okCopyright, L"Failed to read Copyright tag after SetTextTags.");
    FAIL_IF(readCopyright != std::wstring(L"No copyright, use freely"),
            L"Copyright content mismatch. Expected: No copyright, use freely");

    cmsCloseProfile(hProfile);
    std::wcout << L"Test_SetTextTags_Basic completed." << std::endl;
}

// Test 2: Long Description path - ensure SetTextTags handles long strings without error.
static void Test_SetTextTags_LongDescription()
{
    std::wcout << L"Running Test_SetTextTags_LongDescription..." << std::endl;

    cmsContext ContextID = 0;
    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(ContextID);
    FAIL_IF(hProfile != NULL, L"cmsCreateProfilePlaceholder returned NULL.");

    // Create a long description (e.g., 4000 wide chars)
    std::wstring longDesc;
    longDesc.reserve(4000);
    for (int i = 0; i < 4000; ++i) {
        longDesc.push_back((L'a' + (i % 26)));
    }

    cmsBool rc = SetTextTags(hProfile, longDesc.c_str());
    FAIL_IF(rc != TRUE, L"SetTextTags failed on long Description input.");

    // Read back Description tag
    std::wstring readDesc;
    bool okDesc = ReadProfileDescription(hProfile, readDesc);
    FAIL_IF(!okDesc, L"Failed to read Profile Description tag after SetTextTags with long input.");
    FAIL_IF(readDesc != longDesc, L"Profile Description content mismatch for long input.");

    // Check Copyright tag still set to default
    std::wstring readCopyright;
    bool okCopyright = ReadCopyright(hProfile, readCopyright);
    FAIL_IF(!okCopyright, L"Failed to read Copyright tag after SetTextTags long input.");
    FAIL_IF(readCopyright != std::wstring(L"No copyright, use freely"),
            L"Copyright content mismatch after long Description input.");

    cmsCloseProfile(hProfile);
    std::wcout << L"Test_SetTextTags_LongDescription completed." << std::endl;
}

int main()
{
    // Run tests
    Test_SetTextTags_Basic();
    Test_SetTextTags_LongDescription();

    if (gFailures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << gFailures << " test(s) failed." << std::endl;
        return 1;
    }
}