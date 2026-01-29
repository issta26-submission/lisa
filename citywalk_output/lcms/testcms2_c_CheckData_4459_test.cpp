// Lightweight unit test suite for the CheckData function (no GTest, C++11)

#include <cstring>
#include <map>
#include <iostream>
#include <testcms2.h>
#include <cstdint>


// Domain-specific types (minimal mock to enable unit testing without linking to full library)
typedef int cmsInt32Number;
typedef void* cmsHPROFILE;
typedef uint32_t cmsTagSignature;

// Minimal ICC data structure as used by the focal method
// Layout inferred from initialization: cmsICCData { len, flag, { data[...] } }
typedef struct cmsICCData {
    cmsUInt32_t len;              // length field
    cmsUInt32_t flag;             // flag field
    unsigned char data[8];          // payload, at least data[0] used in tests
} cmsICCData;

// Forward declare the function under test (C linkage)
extern "C" cmsInt32Number CheckData(cmsInt32Number Pass, cmsHPROFILE hProfile, cmsTagSignature tag);

// Prototypes for the mock/tag I/O operations used by the focal method
extern "C" cmsInt32Number cmsWriteTag(cmsHPROFILE hProfile, cmsTagSignature tag, const cmsICCData* data);
extern "C" cmsICCData* cmsReadTag(cmsHPROFILE hProfile, cmsTagSignature tag);

// Lightweight mock of a profile to store per-tag cmsICCData on the heap
struct MockProfile {
    std::map<cmsTagSignature, cmsICCData*> tags;
};

// Helpers to create/destroy a mock profile
extern "C" cmsHPROFILE CreateMockProfile() {
    return static_cast<cmsHPROFILE>(new MockProfile());
}
extern "C" void DestroyMockProfile(cmsHPROFILE h) {
    if (!h) return;
    MockProfile* p = static_cast<MockProfile*>(h);
    for (auto &kv : p->tags) {
        delete kv.second;
    }
    delete p;
}

// Implementation of cmsWriteTag and cmsReadTag using the mock profile
extern "C" cmsInt32Number cmsWriteTag(cmsHPROFILE hProfile, cmsTagSignature tag, const cmsICCData* data) {
    if (!hProfile || !data) return 0;
    MockProfile* p = static_cast<MockProfile*>(hProfile);
    // Clean up previous data for this tag if present
    auto it = p->tags.find(tag);
    if (it != p->tags.end()) {
        delete it->second;
        p->tags.erase(it);
    }
    // Deep copy data to a new heap-allocated struct
    cmsICCData* copy = new cmsICCData;
    *copy = *data;
    p->tags[tag] = copy;
    return 1;
}

extern "C" cmsICCData* cmsReadTag(cmsHPROFILE hProfile, cmsTagSignature tag) {
    MockProfile* p = static_cast<MockProfile*>(hProfile);
    if (!p) return NULL;
    auto it = p->tags.find(tag);
    if (it == p->tags.end()) return NULL;
    return it->second;
}

// Global test harness state
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Simple non-terminating assertion helper
#define ASSERT_EQ(expected, actual, msg) \
    do { \
        (++g_total_tests); \
        if ((expected) != (actual)) { \
            ++g_failed_tests; \
            std::cerr << "TEST FAILED: " << msg \
                      << " | Expected: " << (expected) \
                      << " | Actual: " << (actual) << std::endl; \
        } \
    } while (0)

#define ASSERT_TRUE(cond, msg) \
    do { \
        (++g_total_tests); \
        if (!(cond)) { \
            ++g_failed_tests; \
            std::cerr << "TEST FAILED: " << msg \
                      << " | Condition is false." << std::endl; \
        } \
    } while (0)


// Domain constants for tests
static const cmsTagSignature TEST_TAG = 0x01020304; // arbitrary tag identifier


// Test 1: Pass = 1 should delegate to cmsWriteTag and return its result
void test_CheckData_Pass1_WritesTag_ReturnsWriteCode() {
    // Arrange
    cmsHPROFILE h = CreateMockProfile();
    cmsICCData d = { 1, 0, { '?' } }; // as in the focal method
    // Act
    cmsInt32Number rc = CheckData(1, h, TEST_TAG);
    // Assert: We cannot rely on internal cmsWriteTag return value without controlling it,
    // but the contract requires that the function returns the same as cmsWriteTag would.
    // Our cmsWriteTag implementation returns 1 on success, so expect rc == 1.
    ASSERT_EQ(1, rc, "CheckData with Pass=1 should return 1 on successful cmsWriteTag");
    // Cleanup
    DestroyMockProfile(h);
}

// Test 2: Pass = 2 and a properly written tag should yield Pt->data[0] == '?' && flag==0 && len==1
void test_CheckData_Pass2_ReadBackSuccess() {
    // Arrange
    cmsHPROFILE h = CreateMockProfile();
    cmsICCData d = { 1, 0, { '?' } };
    cmsInt32Number write_rc = cmsWriteTag(h, TEST_TAG, &d);
    ASSERT_EQ(1, write_rc, "Prepare tag with valid data for Pass=2");
    // Act
    cmsInt32Number rc = CheckData(2, h, TEST_TAG);
    // Assert
    ASSERT_EQ(1, rc, "CheckData with Pass=2 should validate stored Pt fields as true");
    // Cleanup
    DestroyMockProfile(h);
}

// Test 3: Pass = 2 when tag is missing should return 0
void test_CheckData_Pass2_ReadBackNull() {
    // Arrange
    cmsHPROFILE h = CreateMockProfile();
    // Do not write any tag
    // Act
    cmsInt32Number rc = CheckData(2, h, TEST_TAG);
    // Assert
    ASSERT_EQ(0, rc, "CheckData with Pass=2 and missing tag should return 0");
    // Cleanup
    DestroyMockProfile(h);
}

// Test 4: Pass = 3 should return 0 (default branch)
void test_CheckData_DefaultCaseReturnsZero() {
    // Arrange
    cmsHPROFILE h = CreateMockProfile();
    // Act
    cmsInt32Number rc = CheckData(3, h, TEST_TAG);
    // Assert
    ASSERT_EQ(0, rc, "CheckData with unsupported Pass should return 0");
    // Cleanup
    DestroyMockProfile(h);
}

// Test 5: Pass = 2 with invalid stored data should return 0
void test_CheckData_Pass2_InvalidStoredData() {
    // Arrange
    cmsHPROFILE h = CreateMockProfile();
    cmsICCData bad = { 2, 1, { 'X', 'Y' } }; // not matching expected { '?', ... } and flag != 0 or len != 1
    cmsInt32Number wrc = cmsWriteTag(h, TEST_TAG, &bad);
    ASSERT_EQ(1, wrc, "Store intentionally invalid data for Pass=2 test");
    // Act
    cmsInt32Number rc = CheckData(2, h, TEST_TAG);
    // Assert
    ASSERT_EQ(0, rc, "CheckData should fail when stored data does not meet required criteria");
    // Cleanup
    DestroyMockProfile(h);
}


// Entry point
int main() {
    std::cout << "Starting CheckData unit tests (no GTest, C++11)..." << std::endl;

    test_CheckData_Pass1_WritesTag_ReturnsWriteCode();
    test_CheckData_Pass2_ReadBackSuccess();
    test_CheckData_Pass2_ReadBackNull();
    test_CheckData_DefaultCaseReturnsZero();
    test_CheckData_Pass2_InvalidStoredData();

    std::cout << "Tests completed. Passed: " << (g_total_tests - g_failed_tests)
              << " / " << g_total_tests << "." << std::endl;

    if (g_failed_tests > 0) {
        std::cout << "Some tests FAILED." << std::endl;
        return 1;
    }
    return 0;
}