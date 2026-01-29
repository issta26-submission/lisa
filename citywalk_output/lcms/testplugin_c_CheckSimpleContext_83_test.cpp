// Test suite for the focal method: CheckSimpleContext (located in testplugin.c)
// This suite is designed to compile under C++11 without GTest and to exercise the
// CheckSimpleContext function through a lightweight, non-terminating assertion
// framework.

#include <iostream>
#include <testcms2.h>


// We declare the focal function with C linkage. The exact signature in the focal
// code is cmsInt32Number CheckSimpleContext(void); which is typically typedef'd as int.
// If cmsInt32Number is not exactly int on a given platform, this declaration may need
// adjustment, but in most cases int suffices for linkage in C++ tests.
extern "C" int CheckSimpleContext(void);

// Lightweight test state (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

// Non-terminating assertion macro: logs failure message but continues executing.
// This helps maximize coverage by allowing multiple tests to run even if one fails.
#define ASSERT_TRUE(cond, msg) \
    do { \
        ++g_total; \
        if (!(cond)) { \
            std::cerr << "Assertion failed: " << (msg) << std::endl; \
            ++g_failed; \
        } \
    } while (0)

// Candidate Keywords extracted from the focal method and its dependencies (for traceability):
// - WatchDogContext, cmsDeleteContext, DupContext, cmsCreateContext, cmsGetContextUserData
// - cmsGetContextUserData, DebugMemDontCheckThis, Fail
// - cmsContext, cmsInt32Number, and related context manipulation utilities
// - User data propagation and verification via cmsGetContextUserData
// These keywords guide test intent and coverage understanding.


// Test 1: Basic invocation should succeed (return non-zero) for the initial context setup path.
// This exercise ensures that the first pass of context creation and user data propagation
// yields the expected truthy result as asserted by the focal function.
void test_CheckSimpleContext_BasicInvocation() {
    int rc = CheckSimpleContext();
    ASSERT_TRUE(rc != 0, "CheckSimpleContext should return non-zero on basic invocation (first run)");
}

// Test 2: Repeated invocations should also succeed and produce non-zero results.
// This exercises the function's ability to allocate and propagate user data across multiple
// duplications and context creations. It helps verify stability across successive calls.
void test_CheckSimpleContext_RepeatedInvocations() {
    int rc1 = CheckSimpleContext();
    int rc2 = CheckSimpleContext();
    ASSERT_TRUE(rc1 != 0, "CheckSimpleContext should return non-zero on first repeated run");
    ASSERT_TRUE(rc2 != 0, "CheckSimpleContext should return non-zero on second repeated run");
}

// Entry point to run all tests and report summary.
// The tests are designed to run to completion even if individual assertions fail.
int main() {
    // Execute test cases with explanatory comments
    test_CheckSimpleContext_BasicInvocation();
    test_CheckSimpleContext_RepeatedInvocations();

    // Summary
    std::cout << "Total tests executed: " << g_total << std::endl;
    std::cout << "Total failures: " << g_failed << std::endl;

    // Non-zero exit code indicates test failures
    return (g_failed == 0) ? 0 : 1;
}