/*
Unit test suite for the focal method vstore in optimize.c

Notes:
- This test relies on the actual project headers to provide the correct
  struct stmt definition, the bpf_u_int32 type, and the VAL_UNKNOWN/NOP macros.
- We compile this test as a C++11 translation unit, but we link against the
  C source file that implements vstore (optimize.c). The vstore function has
  C linkage, so we wrap the header inclusion with extern "C" to ensure proper
  linkage.
- The tests avoid terminating on failures; instead, they collect failures
  and report a summary in main().
*/

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <memory.h>
#include <optimize.h>
#include <cstdint>
#include <gencode.h>
#include <os-proto.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <setjmp.h>
#include <config.h>
#include <pcap-int.h>
#include <iostream>
#include <pcap-types.h>


extern "C" {
    // Use project-provided definitions; ensure C linkage for the C API.
    #include "optimize.h"
}

// Simple non-terminating test assertion macros
static int g_test_failures = 0;

#define EXPECT_EQ_VAL(actual, expected, message) do {                 \
    if ((actual) != (expected)) {                                     \
        std::cerr << "TEST_FAIL: " << (message) << " | "              \
                  << "Expected: " << (expected)                        \
                  << ", Got: " << (actual) << " (line " << __LINE__ << ")" << std::endl; \
        ++g_test_failures;                                            \
    }                                                                 \
} while (0)

#define EXPECT_EQ_PTR(actual, expected, message) do {                  \
    if ((actual) != (expected)) {                                      \
        std::cerr << "TEST_FAIL: " << (message) << " | "               \
                  << "Expected pointer: " << (void*)(expected)          \
                  << ", Got pointer: " << (void*)(actual) << " (line " << __LINE__ << ")" << std::endl; \
        ++g_test_failures;                                             \
    }                                                                  \
} while (0)

/*
Test 1: alter is true, newval is known and equals *valp
- Expectation: s->code is set to NOP, and *valp is not modified.
*/
static void test_vstore_alter_true_sets_nop()
{
    struct stmt s;
    // Initialize with a non-NOP code to ensure the assignment happens.
    s.code = 1234;

    bpf_u_int32 val = 0xDEADBEEF; // some value
    bpf_u_int32 *valp = &val;
    bpf_u_int32 newval = (bpf_u_int32)val; // equals *valp
    int alter = 1;

    vstore(&s, valp, newval, alter);

    EXPECT_EQ_VAL(s.code, NOP, "test_vstore_alter_true_sets_nop: s.code should be NOP when alter && newval == *valp && newval != VAL_UNKNOWN");
    EXPECT_EQ_VAL(val, (bpf_u_int32)0xDEADBEEF, "test_vstore_alter_true_sets_nop: *valp should remain unchanged when condition true");
}

/*
Test 2: alter is true, newval != VAL_UNKNOWN and *valp != newval
- Expectation: *valp is updated to newval, and s->code remains unchanged.
*/
static void test_vstore_alter_true_updates_value_when_not_equal()
{
    struct stmt s;
    s.code = 999; // non-NOP initial

    bpf_u_int32 val = 7;
    bpf_u_int32 *valp = &val;
    bpf_u_int32 newval = 5; // different from *valp
    int alter = 1;

    vstore(&s, valp, newval, alter);

    EXPECT_EQ_VAL(val, newval, "test_vstore_alter_true_updates_value_when_not_equal: *valp should be updated to newval");
    EXPECT_EQ_VAL(s.code, 999, "test_vstore_alter_true_updates_value_when_not_equal: s.code should remain unchanged");
}

/*
Test 3: alter is false, regardless of newval
- Expectation: *valp is updated to newval, s->code remains unchanged.
*/
static void test_vstore_alter_false_updates_value()
{
    struct stmt s;
    s.code = 555;

    bpf_u_int32 val = 11;
    bpf_u_int32 *valp = &val;
    bpf_u_int32 newval = 42;
    int alter = 0;

    vstore(&s, valp, newval, alter);

    EXPECT_EQ_VAL(val, newval, "test_vstore_alter_false_updates_value: *valp should be updated to newval when alter is false");
    EXPECT_EQ_VAL(s.code, 555, "test_vstore_alter_false_updates_value: s.code should remain unchanged when alter is false");
}

/*
Test 4: alter is true, newval == VAL_UNKNOWN
- Expectation: Since newval == VAL_UNKNOWN, the condition fails, so *valp is updated to VAL_UNKNOWN.
*/
static void test_vstore_alter_true_newval_unknown()
{
    struct stmt s;
    s.code = 77;

    bpf_u_int32 val = 3;
    bpf_u_int32 *valp = &val;
    bpf_u_int32 newval = VAL_UNKNOWN; // known sentinel
    int alter = 1;

    vstore(&s, valp, newval, alter);

    EXPECT_EQ_VAL(val, VAL_UNKNOWN, "test_vstore_alter_true_newval_unknown: *valp should be VAL_UNKNOWN when newval == VAL_UNKNOWN");
    EXPECT_EQ_VAL(s.code, 77, "test_vstore_alter_true_newval_unknown: s.code should remain unchanged when condition is false");
}

/*
Test 5: alter true, newval equals valp value (repeat of Test 1 but with different initial values)
- Expectation: s->code is set to NOP; *valp remains unchanged.
*/
static void test_vstore_alter_true_equal_value()
{
    struct stmt s;
    s.code = 5;

    bpf_u_int32 val = 25;
    bpf_u_int32 *valp = &val;
    bpf_u_int32 newval = 25; // equal to *valp
    int alter = 1;

    vstore(&s, valp, newval, alter);

    EXPECT_EQ_VAL(s.code, NOP, "test_vstore_alter_true_equal_value: s.code should be NOP when newval == *valp and alter is true");
    EXPECT_EQ_VAL(val, (bpf_u_int32)25, "test_vstore_alter_true_equal_value: *valp should remain equal to newval when condition true");
}

static void run_all_tests()
{
    test_vstore_alter_true_sets_nop();
    test_vstore_alter_true_updates_value_when_not_equal();
    test_vstore_alter_false_updates_value();
    test_vstore_alter_true_newval_unknown();
    test_vstore_alter_true_equal_value();
}

int main()
{
    run_all_tests();

    if (g_test_failures == 0) {
        std::cout << "All vstore tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_test_failures << " vstore test(s) failed." << std::endl;
        return g_test_failures;
    }
}