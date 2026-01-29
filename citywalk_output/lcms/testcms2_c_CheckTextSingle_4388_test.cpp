/*
Unit Test Suite for: CheckTextSingle (cmsInt32Number CheckTextSingle(cmsInt32Number Pass, cmsHPROFILE hProfile, cmsTagSignature tag))

Step 1 - Program Understanding (summary for test author):
- Focal method CheckTextSingle manipulates a cmsMLU string via a tag in a cmsHPROFILE.
- Core flow:
  - Case 1: Create an MLU, set ASCII string to "Test test" for NoLanguage/NoCountry, write it to the profile with the provided tag, free MLU, return rc from cmsWriteTag.
  - Case 2: Read the tag back as an MLU; if NULL, return 0; extract ASCII into a buffer; compare against "Test test"; return 0 if mismatch, TRUE (non-zero) if equal.
  - Default: return 0.
- Dependencies (keywords to reflect core behavior): cmsMLUalloc, cmsMLUsetASCII, cmsWriteTag, cmsMLUfree, cmsReadTag, cmsMLUgetASCII, cmsSigProfileDescriptionTag (example tag used for MLU content), CreateTestProfiles (to obtain a test profile), cmsCloseProfile (cleanup).
- Testing goal: exercise both true and false branches of the condition predicates (Pt NULL vs non-NULL, matching string vs not matching), as well as the default branch.

Step 2 - Unit Test Generation
- We will target CheckTextSingle with a representative MLU tag (cmsSigProfileDescriptionTag) on a test profile created by CreateTestProfiles.
- Test cases to cover:
  1) Pass = 1: Write "Test test" into the profile tag. Expect non-zero rc (write success).
  2) Pass = 2 on the same profile: Read back and verify the string equals "Test test". Expect TRUE (non-zero, typically 1).
  3) Pass = 2 on a freshly created profile without previously written tag: Expect 0 (Pt == NULL branch).
  4) Default branch: Pass = 3 should return 0.
- Static aspects: We call the function CheckTextSingle via extern "C" in a C++11 test file, since the focal method is defined in testcms2.c (C linkage). We will also utilize existing test helper to obtain test profiles: CreateTestProfiles() and cleanup via cmsCloseProfile().

Step 3 - Test Case Refinement
- Use only standard C++11 facilities and the provided API (LittleCMS-like). Do not rely on GTest; implement a minimal non-terminating assertion harness with informative messages, and run tests from main().
- Use non-terminating assertions (do not abort on first failure) to maximize coverage. We will accumulate a small report and print per-test outcomes.
- Access external C APIs with extern "C" linkage to avoid name mangling.
- Ensure proper cleanup to avoid memory leaks (cmsCloseProfile).

Below is the test code (C++11):

*/

// Candidate Keywords (for reference in code comments):
// cmsMLUalloc, cmsMLUsetASCII, cmsWriteTag, cmsMLUfree, cmsReadTag, cmsMLUgetASCII,
// cmsSigProfileDescriptionTag, CreateTestProfiles, cmsCloseProfile, cmsMLU,
// cmsHPROFILE, cmsTagSignature, cmsInt32Number

#include <cstring>
#include <iostream>
#include <lcms2.h>
#include <testcms2.h>


// Include LittleCMS-like header for type definitions (adjust path if needed)

// Ensure the focal function is accessible from C++
extern "C" {
    // Forward declarations for the focal method and test helpers
    cmsInt32Number CheckTextSingle(cmsInt32Number Pass, cmsHPROFILE hProfile, cmsTagSignature tag);
    cmsHPROFILE CreateTestProfiles(void);
}

static int g_tests_total = 0;
static int g_tests_passed = 0;

// Simple non-terminating expectations for C++11 (non-fatal)
#define EXPECT_TRUE(cond, msg) do { \
    if ((cond)) { \
        std::cout << "[PASS] " << msg << std::endl; \
        ++g_tests_passed; \
    } else { \
        std::cerr << "[FAIL] " << msg << std::endl; \
    } \
    ++g_tests_total; \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if ((a) == (b)) { \
        std::cout << "[PASS] " << msg << std::endl; \
        ++g_tests_passed; \
    } else { \
        std::cerr << "[FAIL] " << msg << " (expected " << (a) << " == " << (b) << ")" << std::endl; \
    } \
    ++g_tests_total; \
} while(0)

int main() {
    // Test harness for CheckTextSingle

    // Create a test profile to operate on
    cmsHPROFILE hProfile = CreateTestProfiles();
    if (hProfile == nullptr) {
        std::cerr << "Failed to create test profile. Aborting tests." << std::endl;
        return 1;
    }

    // Tag used for MLU content. Assumes Profile Description tag is suitable for MLU text storage.
    // If the environment uses a different MLU-capable tag, replace cmsSigProfileDescriptionTag accordingly.
    cmsTagSignature testTag = cmsSigProfileDescriptionTag;

    // 1) Case 1: Pass = 1 should write "Test test" into the profile for the given tag.
    {
        // Intention: exercise the positive path of the write operation.
        cmsInt32Number rc = CheckTextSingle(1, hProfile, testTag);
        EXPECT_TRUE(rc != 0, "CheckTextSingle(Pass=1) should return non-zero rc after cmsWriteTag");
    }

    // 2) Case 2: Pass = 2 should read back the previously written string and verify content.
    {
        cmsInt32Number rc = CheckTextSingle(2, hProfile, testTag);
        // Expected: buffer contains "Test test" and function returns TRUE (non-zero)
        EXPECT_EQ(static_cast<int>(rc), 1, "CheckTextSingle(Pass=2) should return TRUE (1) when text matches 'Test test'");
    }

    // 3) Case 3: Pt == NULL branch (Pass=2 on a fresh profile without the tag written)
    {
        cmsHPROFILE hFresh = CreateTestProfiles();
        if (hFresh != nullptr) {
            cmsInt32Number rc = CheckTextSingle(2, hFresh, testTag);
            // Expected: Pt == NULL leads to return 0
            EXPECT_EQ(static_cast<int>(rc), 0, "CheckTextSingle(Pass=2) on fresh profile should return 0 when tag not set");
            // Cleanup
            cmsCloseProfile(hFresh);
        } else {
            std::cerr << "[WARN] Could not create fresh test profile for NULL-branch test." << std::endl;
        }
    }

    // 4) Case 4: Default path (Pass not 1 or 2)
    {
        cmsInt32Number rc = CheckTextSingle(3, hProfile, testTag);
        EXPECT_EQ(static_cast<int>(rc), 0, "CheckTextSingle(Pass=3) should return 0 (default branch)");
    }

    // Cleanup
    cmsCloseProfile(hProfile);

    // Final report
    std::cout << "Total tests run: " << g_tests_total << "\n";
    std::cout << "Total tests passed: " << g_tests_passed << "\n";

    // Exit code 0 if all tests passed; non-zero otherwise
    return (g_tests_passed == g_tests_total) ? 0 : 1;
}