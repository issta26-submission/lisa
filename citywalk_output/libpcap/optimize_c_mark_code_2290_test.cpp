// Unit test suite for the focal method mark_code in optimize.c
// This test harness is written for C++11, avoids any GTest usage,
// and relies on the project's provided headers.
// It focuses on verifying the observable effect of mark_code:
//  - ic->cur_mark is incremented by 1 on each call
//  - make_marks(ic, ic->root) is invoked (observed indirectly via side-effects if possible)
// Notes:
// - The tests operate on a minimally initialized struct icode instance.
// - Root is set to NULL to avoid dependencies on the full block graph.
// - Static helpers within optimize.c are not mocked; we rely on the actual implementation.

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <memory.h>
#include <optimize.h>
#include <gencode.h>
#include <os-proto.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <setjmp.h>
#include <config.h>
#include <pcap-int.h>
#include <cstring>
#include <iostream>
#include <pcap-types.h>


extern "C" {
}

// Ensure correct linkage for the C function mark_code when including from C++
extern "C" void mark_code(struct icode *ic);

// Simple non-terminating assertion macro: logs failure but continues execution
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "EXPECT_TRUE failed: " << (msg) << std::endl; \
            ++g_failures; \
        } \
    } while(0)


// Test 1: Verify that mark_code increments cur_mark by 1 when starting from 0.
// Rationale: Core behavior of the focal method is to increment cur_mark and then call make_marks.
// If ic->cur_mark is not incremented, the test will fail, indicating a regression.
void test_mark_code_increments_from_zero() {
    struct icode ic;
    std::memset(&ic, 0, sizeof(ic));  // Safe initialization for test
    ic.cur_mark = 0;
    ic.root = NULL;  // Avoid dependencies on a full icode graph

    mark_code(&ic);

    EXPECT_TRUE(ic.cur_mark == 1,
        "When starting from 0, ic.cur_mark should be 1 after mark_code()");
}


// Test 2: Verify that mark_code increments cur_mark by 1 when starting from a negative value.
// Rationale: Edge case where cur_mark is negative should still increment correctly.
void test_mark_code_increments_from_negative() {
    struct icode ic;
    std::memset(&ic, 0, sizeof(ic));
    ic.cur_mark = -1;
    ic.root = NULL;

    mark_code(&ic);

    EXPECT_TRUE(ic.cur_mark == 0,
        "When starting from -1, ic.cur_mark should be 0 after mark_code()");
}


// Test 3: Verify that multiple successive calls to mark_code increment cur_mark accordingly.
// Rationale: Ensure consistent cumulative behavior across repeated invocations.
void test_mark_code_multiple_calls() {
    struct icode ic;
    std::memset(&ic, 0, sizeof(ic));
    ic.cur_mark = 0;
    ic.root = NULL;

    const int calls = 5;
    for (int i = 0; i < calls; ++i) {
        mark_code(&ic);
    }

    EXPECT_TRUE(ic.cur_mark == calls,
        "After " << calls << " calls, ic.cur_mark should equal the number of invocations");
}


// Main runner: executes all tests and reports summary.
// This avoids any external testing framework as required.
int main() {
    test_mark_code_increments_from_zero();
    test_mark_code_increments_from_negative();
    test_mark_code_multiple_calls();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}