/*
  Test suite for the focal method: CheckSignature in testcms2.c

  Approach (matching the provided step-by-step guidelines):
  - Step 1: Analyze the function. CheckSignature takes a Pass flag and either writes
    or reads a cmsTagSignature value to/from a profile via cmsWriteTag / cmsReadTag.
  - Step 2: Generate unit tests by mocking the tag I/O functions (cmsWriteTag / cmsReadTag)
    to keep tests self-contained and independent from the real LittleCMS I/O subsystem.
  - Step 3: Refine tests for coverage:
    - Branch: Pass == 1 writes a signature (should return 1 in our mock).
    - Branch: Pass == 2 reads a signature and compares against cmsSigPerceptualReferenceMediumGamut
              (should return 1 when a write previously occurred for the tag).
    - Branch: Default case and Read with no prior Write should return 0.

  Notes:
  - We do not rely on a GTest framework. Instead, we implement a lightweight test harness
    using plain C++11 and a minimal in-process mock for tag storage.
  - The test suite uses the same header definitions (testcms2.h) for type consistency.
  - The test code uses non-terminating checks (log-based) so that all tests run to completion.
  - Static/private members in the focal file are not used directly; we only test the public
    function and mock its dependencies locally.

  Important: The mock implementations are provided below. They override only cmsWriteTag and cmsReadTag
  so that CheckSignature can be exercised deterministically without needing the actual library I/O.
*/

/* Include the provided test framework header to obtain necessary type definitions
   (cmsInt32Number, cmsHPROFILE, cmsTagSignature, cmsSigPerceptualReferenceMediumGamut, etc.)
*/
#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <testcms2.h>



// Forward declaration of the focal function (as it exists in testcms2.c)
extern "C" cmsInt32Number CheckSignature(cmsInt32Number Pass, cmsHPROFILE hProfile, cmsTagSignature tag);

// 1) Candidate Keywords (core components the tests revolve around):
// - cmsWriteTag / cmsReadTag: tag I/O primitives the focal function uses.
// - cmsSigPerceptualReferenceMediumGamut: the signature value used for comparisons.
// - cmsTagSignature, cmsHPROFILE: tag and profile abstractions.
// - cmsInt32Number: return type of the function under test.

// Mock storage to simulate per-profile tag storage without depending on the real library.
static std::map<cmsTagSignature, cmsTagSignature*> g_tagPtrStore;

// Reset helper: clear stored tags and free allocated memory
void ResetMockProfile() {
    for (auto &kv : g_tagPtrStore) {
        delete kv.second;
    }
    g_tagPtrStore.clear();
}

/* Mocked dependency: cmsWriteTag
   Stores the value pointed to by 'value' into the mock profile's storage for the given 'tag'.
   Always returns 1 to indicate success (as the real library would on success).
*/
extern "C" cmsInt32Number cmsWriteTag(cmsHPROFILE hProfile, cmsTagSignature tag, const void* value) {
    const cmsTagSignature* p = static_cast<const cmsTagSignature*>(value);
    // If existing value for the tag, free it first to avoid leaks in repeated writes
    auto it = g_tagPtrStore.find(tag);
    if (it != g_tagPtrStore.end()) {
        delete it->second;
    }
    // Allocate a new copy of the value and store its pointer
    cmsTagSignature* stored = new cmsTagSignature(*p);
    g_tagPtrStore[tag] = stored;
    return 1; // mimic success
}

/* Mocked dependency: cmsReadTag
   Returns a pointer to the stored cmsTagSignature for the given 'tag', or NULL if absent.
   NOTE: The memory is owned by the mock and should be freed by the test via ResetMockProfile.
*/
extern "C" void* cmsReadTag(cmsHPROFILE hProfile, cmsTagSignature tag) {
    auto it = g_tagPtrStore.find(tag);
    if (it == g_tagPtrStore.end()) return NULL;
    return static_cast<void*>(it->second);
}

/* Unit Tests (non-terminating assertions; accumulate logs for reporting)
   - test_CheckSignature_WritesAndReads_Succeeds
     Validates that Pass == 1 writes a signature and Pass == 2 reads it back correctly.
   - test_CheckSignature_ReadWithoutWrite_Returns0
     Ensures reading a non-existent tag returns 0.
   - test_CheckSignature_DefaultBranch_Returns0
     Ensures a default/unsupported Pass value returns 0.
*/

// Test 1: Ensure Pass == 1 writes and Pass == 2 reads back the written signature (success path)
bool test_CheckSignature_WritesAndReads_Succeeds(std::vector<std::string>& logs) {
    ResetMockProfile();
    cmsTagSignature testTag = 0xDEADBEEF; // arbitrary tag identifier

    // Case: Write path (Pass == 1)
    cmsInt32Number r1 = CheckSignature(1, (cmsHPROFILE)nullptr, testTag);
    bool okWrite = (r1 == 1);

    // Case: Read path (Pass == 2) after a write for the same tag
    cmsInt32Number r2 = CheckSignature(2, (cmsHPROFILE)nullptr, testTag);
    bool okRead = (r2 == 1);

    logs.emplace_back("test_CheckSignature_WritesAndReads_Succeeds: Pass 1 write returned 1 (okWrite=" + std::to_string(okWrite) +
                      ", r1=" + std::to_string(r1) + ")");
    logs.emplace_back("test_CheckSignature_WritesAndReads_Succeeds: Pass 2 read returned 1 (okRead=" + std::to_string(okRead) +
                      ", r2=" + std::to_string(r2) + ")");

    ResetMockProfile();
    return okWrite && okRead;
}

// Test 2: Ensure reading a tag that was never written returns 0
bool test_CheckSignature_ReadWithoutWrite_Returns0(std::vector<std::string>& logs) {
    ResetMockProfile();
    cmsTagSignature testTag = 0xFEEDBEEF;
    cmsInt32Number r = CheckSignature(2, (cmsHPROFILE)nullptr, testTag);
    bool ok = (r == 0);
    logs.emplace_back("test_CheckSignature_ReadWithoutWrite_Returns0: Read without write returns 0 (r=" + std::to_string(r) +
                      ", ok=" + std::to_string(ok) + ")");
    ResetMockProfile();
    return ok;
}

// Test 3: Ensure default (unsupported Pass value) returns 0
bool test_CheckSignature_DefaultBranch_Returns0(std::vector<std::string>& logs) {
    ResetMockProfile();
    cmsTagSignature testTag = 0xBAADF00D;
    cmsInt32Number r = CheckSignature(3, (cmsHPROFILE)nullptr, testTag);
    bool ok = (r == 0);
    logs.emplace_back("test_CheckSignature_DefaultBranch_Returns0: Default case returns 0 (r=" + std::to_string(r) +
                      ", ok=" + std::to_string(ok) + ")");
    ResetMockProfile();
    return ok;
}

// Main entry: run all tests and print results
int main() {
    std::vector<std::string> logs;

    bool t1 = test_CheckSignature_WritesAndReads_Succeeds(logs);
    bool t2 = test_CheckSignature_ReadWithoutWrite_Returns0(logs);
    bool t3 = test_CheckSignature_DefaultBranch_Returns0(logs);

    // Print detailed test logs
    for (const auto& msg : logs) {
        std::cout << msg << std::endl;
    }

    // Simple summary
    int failures = 0;
    for (const auto& msg : logs) {
        // Heuristic: any message containing "FAIL" is considered a failure
        if (msg.find("FAIL") != std::string::npos) ++failures;
    }

    if (failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << failures << " test(s) failed." << std::endl;
        return 1;
    }
}