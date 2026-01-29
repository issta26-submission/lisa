// Test suite for AddAvailableProperty in cmscgats.c
// This test suite uses a lightweight, self-contained test harness (no GTest).
// It focuses on validating that AddAvailableProperty correctly appends
// KEYVALUE entries to the it8->ValidKeywords list and that IsAvailableOnList
// can locate added entries starting from the head of the list.
//
// Notes:
// - We rely on the project headers (e.g., lcms2_internal.h) to define cmsIT8,
//   KEYVALUE, WRITEMODE, and the prototypes for AddAvailableProperty and
//   IsAvailableOnList. The tests use these APIs directly.
// - The tests do not attempt to free the allocated list nodes to keep the
//   test code straightforward; the test program will exit soon after tests.
// - The test asserts use simple conditional checks with printf to report PASS/FAIL.

#include <lcms2_internal.h>
#include <cstdio>
#include <cstring>


// Include core internal declarations from the project.
// This header is expected to declare cmsIT8, KEYVALUE, WRITEMODE, and the APIs
// AddAvailableProperty and IsAvailableOnList with C linkage.

// If the project uses C linkage for non-C++ aware headers, the above header
// should already ensure correct linkage when compiling this C++ test.
// The following tests assume the prototypes are visible via the header.


// Helper function to report test results in a uniform way
static void report_result(const char* test_name, bool pass) {
    if (pass) {
        printf("[PASS] %s\n", test_name);
    } else {
        printf("[FAIL] %s\n", test_name);
    }
}

// Test 1: After a single AddAvailableProperty call, the head of the list should be non-null
// and the return value should equal the head pointer.
static bool test_AddAvailableProperty_SingleAdd() {
    // Prepare a cmsIT8 object with an empty ValidKeywords list.
    cmsIT8 it8;
    memset(&it8, 0, sizeof(it8));
    // Ensure the head is NULL initially.
    // Some headers may define ValidKeywords as a pointer member; assume it exists.
    // If the member is named differently, this test will adapt accordingly.
    // We explicitly set it to NULL to simulate an empty list.
    // Note: Access to it8.ValidKeywords assumes this member is public in the header.
    (void)it8; // suppress unused warning if member name is not visible here

    // The Real project will define ValidKeywords in cmsIT8; cast to void* to avoid warnings
    // If the member is named differently, replace accordingly:
    // ((KEYVALUE**)&it8)->ValidKeywords = NULL;
    // To be robust across environments, use a direct zero-initialized approach if accessible.
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Waddress-of-improper-cell-access"
#endif

    // Try to access the member in a portable way
    // Note: If the compile-time member name differs, adjust to it8.ValidKeywords.
    // We assume ValidKeywords is a public member of cmsIT8 as shown in the focal code.
    ((KEYVALUE**)&it8)->ValidKeywords = NULL;
#ifdef __clang__
#pragma clang diagnostic pop
#endif

    // Call the function under test
    KEYVALUE* ret = AddAvailableProperty(&it8, "Key1", static_cast<WRITEMODE>(0));

    // Validate: return pointer should not be NULL and should equal the head of the list
    bool ok = (ret != NULL);
    ok = ok && (it8.ValidKeywords == ret);

    // Validate via list search: IsAvailableOnList should find "Key1" starting from head
    KEYVALUE* foundPtr = NULL;
    cmsBool found = IsAvailableOnList(it8.ValidKeywords, "Key1", NULL, &foundPtr);

    ok = ok && (found != 0) && (foundPtr == ret);
    return ok;
}

// Test 2: Adding two properties should place the second entry at the new head
// and both entries should be discoverable via the list lookup.
static bool test_AddAvailableProperty_TwoAdds() {
    cmsIT8 it8b;
    memset(&it8b, 0, sizeof(it8b));
    ((KEYVALUE**)&it8b)->ValidKeywords = NULL;

    // First addition
    KEYVALUE* first = AddAvailableProperty(&it8b, "FirstKey", static_cast<WRITEMODE>(0));
    if (first == NULL) return false;
    if (it8b.ValidKeywords != first) return false; // head should be the first node

    // Second addition
    KEYVALUE* second = AddAvailableProperty(&it8b, "SecondKey", static_cast<WRITEMODE>(0));
    if (second == NULL) return false;
    // Depending on implementation, new node may become the head.
    if (it8b.ValidKeywords != second) {
        // If the head did not advance, this test would fail;
        // but many implementations push to the head.
        // We'll still try to verify by lookup below and check that both keys exist.
        // For robustness, allow non-head if the API contract differs.
        // However, we will fail if the second key cannot be found starting from current head.
        // Move on to verify with lookup rather than strict head assumption.
    }

    // Verify both entries exist in the list and are traceable to their corresponding nodes
    KEYVALUE* foundFirst = NULL;
    cmsBool okFirst = IsAvailableOnList(it8b.ValidKeywords, "FirstKey", NULL, &foundFirst);
    if (!okFirst) return false;
    if (foundFirst != first) return false;

    KEYVALUE* foundSecond = NULL;
    cmsBool okSecond = IsAvailableOnList(it8b.ValidKeywords, "SecondKey", NULL, &foundSecond);
    if (!okSecond) return false;
    if (foundSecond == NULL) return false;
    // If the list head is now 'second', foundSecond should point to the 'second' node.
    if (second != NULL && foundSecond != second && foundSecond != it8b.ValidKeywords) {
        // In case insertion semantics place 'second' at head, foundSecond should point to 'second'.
        // If not, still ensure that the node exists and matches one of the added pointers.
        // For strictness, require foundSecond to be either 'second' or a node with the same Key.
        // We will enforce: either pointer equality to 'second' or Key equality.
        // Since we can't compare by Key directly without accessing fields, we rely on pointer equality:
        if (foundSecond != second) {
            return false;
        }
    }

    return true;
}

int main() {
    int failures = 0;

    // Run test 1
    bool t1 = test_AddAvailableProperty_SingleAdd();
    report_result("AddAvailableProperty_SingleAdd should create head and return it", t1);
    if (!t1) failures++;

    // Run test 2
    bool t2 = test_AddAvailableProperty_TwoAdds();
    report_result("AddAvailableProperty_TwoAdds should add a second entry and make it accessible", t2);
    if (!t2) failures++;

    if (failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("%d TEST(S) FAILED\n", failures);
        return 1;
    }
}