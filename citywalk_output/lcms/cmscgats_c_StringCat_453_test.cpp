// Unit tests for cmsBool StringCat(string* s, const char* c)
// Target: verify true/false branches and integration with StringAppend.
// This test uses the real implementation from cmscgats.c via its C interface.
// No GTest; a lightweight test harness is provided and invoked from main.
// Notes:
// - We rely on StringAlloc to create a string object, StringPtr to inspect content,
//   and StringCat to perform the concatenation using StringAppend for each character.
// - We test successful concatenation, partial concatenation due to capacity, and empty/edge cases.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// We assume the library provides these types and functions with C linkage.
// We forward-declare only what is necessary for linking against the provided cmscgats.c.
extern "C" {

// Opaque string type used by the library
typedef struct string string;

// Forward-declare cmsIT8 as an opaque type (used in StringAlloc signature)
typedef struct cmsIT8 cmsIT8;

// cmsBool is typically an int in this codebase; keep a simple typedef for compatibility
typedef int cmsBool;
enum { TRUE = 1, FALSE = 0 };

// Prototypes of the relevant functions from the focal class/dependencies
string* StringAlloc(cmsIT8* it8, int max);
void StringClear(string* s);
cmsBool StringAppend(string* s, char c);
char* StringPtr(string* s);

// The function under test (already defined in cmscgats.c)
cmsBool StringCat(string* s, const char* c);

} // extern "C"

// Lightweight test harness
static int tests_total = 0;
static int tests_passed = 0;

// Helper macro to report test results without stopping at first failure
#define TEST_ASSERT(cond, msg) do { \
    tests_total++; \
    if (cond) { \
        tests_passed++; \
        std::cout << "[OK] " << msg << std::endl; \
    } else { \
        std::cerr << "[FAIL] " << msg << std::endl; \
    } \
} while (0)

// Test 1: Successful concatenation within capacity
bool test_StringCat_Succeeds() {
    // Arrange
    string* s = StringAlloc(nullptr, 10); // allow up to 10 chars
    if (s == nullptr) {
        std::cerr << "test_StringCat_Succeeds: StringAlloc returned NULL" << std::endl;
        return false;
    }

    // Act
    cmsBool ret = StringCat(s, "Hello");

    // Assert
    const char* content = StringPtr(s);
    bool ok = (ret != 0) && (content != nullptr) && (std::strcmp(content, "Hello") == 0);
    TEST_ASSERT(ok, "test_StringCat_Succeeds: StringCat returns TRUE and content is 'Hello'");

    // Cleanup
    StringClear(s);
    return ok;
}

// Test 2: Partial concatenation due to capacity limit (should fail on overflow)
bool test_StringCat_ExceedCapacity() {
    // Arrange: max = 3 means capacity for 3 characters
    string* s = StringAlloc(nullptr, 3);
    if (s == nullptr) {
        std::cerr << "test_StringCat_ExceedCapacity: StringAlloc returned NULL" << std::endl;
        return false;
    }

    // Act
    cmsBool ret = StringCat(s, "HELLO"); // 5 chars; should fail during processing

    // Assert
    // Expect FALSE due to capacity overflow
    bool ok_ret = (ret == FALSE);
    const char* content = StringPtr(s);
    // Expect the content to be the first 3 chars, if the implementation appends until max
    // If the real implementation differs, this test may need adjustment.
    bool ok_content = (content != nullptr) && (std::strcmp(content, "HEL") == 0);
    TEST_ASSERT(ok_ret, "test_StringCat_ExceedCapacity: StringCat returns FALSE when capacity is exceeded");
    TEST_ASSERT(ok_content, "test_StringCat_ExceedCapacity: Content equals first 'HEL' after overflow");

    // Cleanup
    StringClear(s);
    return ok_ret && ok_content;
}

// Test 3: Empty string input should succeed with no changes
bool test_StringCat_EmptyInput() {
    string* s = StringAlloc(nullptr, 10);
    if (s == nullptr) {
        std::cerr << "test_StringCat_EmptyInput: StringAlloc returned NULL" << std::endl;
        return false;
    }

    cmsBool ret = StringCat(s, "");
    const char* content = StringPtr(s);
    bool ok = (ret != 0) && (content != nullptr) && (content[0] == '\0');
    TEST_ASSERT(ok, "test_StringCat_EmptyInput: StringCat with empty C-string returns TRUE and leaves string empty");

    StringClear(s);
    return ok;
}

// Test 4: Single character when capacity allows exactly one character
bool test_StringCat_SingleChar() {
    string* s = StringAlloc(nullptr, 1);
    if (s == nullptr) {
        std::cerr << "test_StringCat_SingleChar: StringAlloc returned NULL" << std::endl;
        return false;
    }

    cmsBool ret = StringCat(s, "A");
    const char* content = StringPtr(s);
    bool ok = (ret != 0) && (content != nullptr) && (std::strcmp(content, "A") == 0);
    TEST_ASSERT(ok, "test_StringCat_SingleChar: StringCat with single char succeeds and content is 'A'");

    StringClear(s);
    return ok;
}

// Main: run all tests
int main() {
    std::cout << "Running unit tests for cmsBool StringCat(string* s, const char* c)" << std::endl;

    bool t1 = test_StringCat_Succeeds();
    bool t2 = test_StringCat_ExceedCapacity();
    bool t3 = test_StringCat_EmptyInput();
    bool t4 = test_StringCat_SingleChar();

    // Aggregate results
    int total = tests_total;
    int passed = tests_passed;

    std::cout << "\nTest results: " << passed << " / " << total << " tests passed." << std::endl;

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}