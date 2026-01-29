// Unit test suite for the focal deflateStateCheck method (deflateStateCheck(z_streamp strm))
// This test suite is designed to be used in a C++11 environment without Google Test.
// It relies on the existing deflate.h/deflate.c in the project and compiles with them.
// Each test case includes explanatory comments describing the scenario being validated.

#include <iostream>
#include <string>
#include <deflate.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Ensure C linkage for the focal function when called from C++
#ifdef __cplusplus
extern "C" {
#endif
int deflateStateCheck(z_streamp strm);
#ifdef __cplusplus
}
#endif

// Dummy allocator/free functions to satisfy function pointer requirements
static voidpf dummyAlloc(voidpf opaque, uInt items, uInt size) {
    (void)opaque; (void)items; (void)size;
    return nullptr;
}
static void dummyFree(voidpf opaque, voidpf address) {
    (void)opaque; (void)address;
}

// Convenience macro for simple test result reporting
#define TEST_RESULT(name, condition) \
    do { \
        bool passed = (condition); \
        std::cout << (passed ? "[PASS] " : "[FAIL] ") << name << "\n"; \
        if (!passed) g_failed = true; \
    } while (0)

// Global flag to accumulate failures across tests
static bool g_failed = false;

/*
Test 1: Null strm (nullptr) should return 1 (error)
Scenario:
- Call deflateStateCheck with a null z_streamp.
- Expect return value 1.
*/
static void test_null_strm() {
    TEST_RESULT("test_null_strm: strm == nullptr => return 1", deflateStateCheck(nullptr) == 1);
}

/*
Test 2: strm with zalloc == 0 should return 1
Scenario:
- Create a valid z_stream with zalloc set to null function pointer and zfree set to a non-null function.
- Since zalloc is 0, the function should short-circuit and return 1.
*/
static void test_zalloc_zero() {
    z_stream strm;
    // Ensure non-null zfree and a null zalloc to trigger the first guard
    strm.zalloc = (alloc_func)0;
    strm.zfree = dummyFree;
    strm.state = nullptr;

    TEST_RESULT("test_zalloc_zero: zalloc == 0 => return 1", deflateStateCheck(&strm) == 1);
}

/*
Test 3: strm with zfree == 0 should return 1
Scenario:
- Create a valid z_stream with zalloc non-null and zfree == 0.
- The first guard should trigger and return 1.
*/
static void test_zfree_zero() {
    z_stream strm;
    strm.zalloc = dummyAlloc;
    strm.zfree = (free_func)0; // set to null-like to trigger guard
    strm.state = nullptr;

    TEST_RESULT("test_zfree_zero: zfree == 0 => return 1", deflateStateCheck(&strm) == 1);
}

/*
Test 4: s == NULL after strm->state (s->strm != strm) should return 1
Scenario:
- Provide a valid z_stream with non-null allocator/freeer.
- Allocate a deflate_state s with s->strm pointing to something other than strm (nullptr here).
- Expect return 1 due to mismatched s->strm.
*/
static void test_state_strm_mismatch() {
    z_stream strm;
    strm.zalloc = dummyAlloc;
    strm.zfree = dummyFree;
    // Prepare a deflate_state whose strm does not point to the test strm
    deflate_state s;
    s.strm = nullptr; // intentionally not equal to &strm
    s.status = INIT_STATE;

    strm.state = &s;

    TEST_RESULT("test_state_strm_mismatch: s->strm != strm => return 1",
                deflateStateCheck(&strm) == 1);
}

/*
Test 5: s is valid and status is an allowed value => return 0
Scenario:
- strm and s are proper, with s->strm == strm and s->status = INIT_STATE (an allowed state).
- Expect successful check (return 0).
*/
static void test_valid_status_init() {
    z_stream strm;
    strm.zalloc = dummyAlloc;
    strm.zfree = dummyFree;

    deflate_state s;
    s.strm = &strm;
    s.status = INIT_STATE; // allowed

    strm.state = &s;

    TEST_RESULT("test_valid_status_init: status == INIT_STATE => return 0", deflateStateCheck(&strm) == 0);
}

/*
Test 6: s has a status not in the allowed set => return 1
Scenario:
- s->strm == strm but s->status is not among the allowed states (e.g., 9999).
- Expect failure (return 1).
*/
static void test_status_not_allowed() {
    z_stream strm;
    strm.zalloc = dummyAlloc;
    strm.zfree = dummyFree;

    deflate_state s;
    s.strm = &strm;
    s.status = 9999; // not an allowed status

    strm.state = &s;

    TEST_RESULT("test_status_not_allowed: status not in allowed set => return 1", deflateStateCheck(&strm) == 1);
}

// Main function to run all tests
int main() {
    test_null_strm();
    test_zalloc_zero();
    test_zfree_zero();
    test_state_strm_mismatch();
    test_valid_status_init();
    test_status_not_allowed();

    if (g_failed) {
        std::cout << "[OVERALL] Some tests FAILED.\n";
        return 1;
    } else {
        std::cout << "[OVERALL] All tests PASSED.\n";
        return 0;
    }
}