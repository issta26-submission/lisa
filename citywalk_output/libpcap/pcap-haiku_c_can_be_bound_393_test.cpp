/*
Step 1 (Conceptual): Candidate Keywords extracted from the focal method
- name
- loop
- strcmp
- badversions
- hrev56578, hrev55182, hrev54154, hrev52295, hrev44702
- uts (struct utsname)
- uname
- strlen, strncmp
- return values (1 for bindable, 0 for non-bindable)

Step 2/3 (Implementation): A small, self-contained C++11 test suite for the focal
method can_be_bound(const char *name) found in pcap-haiku.c.

Notes:
- We do not rely on a heavy test framework (no gtest/gmock). A minimal harness is used.
- We test both branches of the predicate:
  - Non-"loop" names should yield 1 (can be bound).
  - "loop" name depends on the system uname(). We re-derive the expected value by duplicating the same logic locally to compare against the actual function output.
- We provide explanatory comments for each test case and for the helper that computes the expected value.

Build note:
- Link this test with the object containing can_be_bound (e.g., the compiled pcap-haiku.c) so the symbol can_be_bound is resolved.

*/

#include <string.h>
#include <net/if_media.h>
#include <errno.h>
#include <config.h>
#include <sys/socket.h>
#include <net/if.h>
#include <stdint.h>
#include <net/if_types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <OS.h>
#include <net/if_dl.h>
#include <pcap-int.h>
#include <sys/sockio.h>
#include <iostream>
#include <cstring>


// Declaration of the focal function from pcap-haiku.c
extern "C" int can_be_bound(const char *name);

/*
Helper: Compute the expected result for the "loop" case by duplicating the same
runtime logic used inside can_be_bound. This is used to verify the actual
implementation against an independently computed expectation based on uname().
This keeps tests deterministic with respect to the system's uname data.
*/
int compute_expected_loop_boundability() {
    const char *badversions[] = {
        "hrev56578",
        "hrev55182",
        "hrev54154",
        "hrev52295",
        "hrev44702",
        NULL
    };

    struct utsname uts;
    if (uname(&uts) != 0) {
        // If uname fails for some reason, conservatively assume non-bindable to avoid false positives.
        return 0;
    }

    for (const char **s = badversions; *s != nullptr; ++s) {
        // Match the version prefix with the bad version strings
        if (strncmp(uts.version, *s, strlen(*s)) == 0) {
            return 0;
        }
    }
    return 1;
}

// Simple test harness helpers
#define TEST_OK(msg) \
    do { std::cout << "[OK] " msg "\n"; } while (0)
#define TEST_FAIL(msg) \
    do { std::cerr << "[FAIL] " msg "\n"; failures++; } while (0)

int main() {
    int failures = 0;

    // Test 1: Non-loop names should be bindable (return 1)
    // Rationale: can_be_bound returns 1 if the name != "loop".
    {
        int result = can_be_bound("eth0");
        if (result == 1) {
            TEST_OK("can_be_bound(\"eth0\") == 1 as expected (non-loop name).");
        } else {
            TEST_FAIL("can_be_bound(\"eth0\") did not return 1 (expected non-loop branch).");
        }
    }

    // Test 2: Loop name behavior should match the independently computed expectation
    // Rationale: can_be_bound("loop") should return 0 if uts.version matches any badversion prefix;
    // otherwise it should return 1. We re-derive the expected value at runtime.
    {
        int actual = can_be_bound("loop");
        int expected = compute_expected_loop_boundability();
        if (actual == expected) {
            TEST_OK("can_be_bound(\"loop\") matches independently computed expectation.");
        } else {
            TEST_FAIL("can_be_bound(\"loop\") does not match independent expectation.");
            std::cerr << "  actual: " << actual << ", expected: " << expected << "\n";
        }
    }

    // Test 3: Re-check stability by repeated invocation of a non-loop name
    // Rationale: Ensure stable behavior across multiple calls.
    {
        int r1 = can_be_bound("looper"); // distinct from "loop" to avoid edge-case confusion
        int r2 = can_be_bound("looper");
        if (r1 == 1 && r2 == 1) {
            TEST_OK("Repeated calls to can_be_bound(\"looper\") consistently return 1.");
        } else {
            TEST_FAIL("Inconsistent results on repeated can_be_bound(\"looper\") calls.");
            std::cerr << "  first: " << r1 << ", second: " << r2 << "\n";
        }
    }

    // Summary
    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << failures << " test(s) failed.\n";
        return 1;
    }
}