// Test suite for the focal method gen_jmp_x in gencode.c
// This test suite is crafted to be compiled as C++11 code without Google Test.
// It relies on the project's existing C headers to provide the proper types
// (compiler_state_t, struct block, struct slist, etc.) and the focal function.
//
// Key ideas:
// - Use NULL for compiler_state_t to exercise the function with a minimal state.
// - Validate that the returned block is non-NULL.
// - Validate that the block's stmts field matches the provided input pointer.
// - Validate behavior when stmts is NULL as well as when it is non-NULL.
// - Check that multiple invocations yield distinct block objects to exercise
//   memory allocation behavior (typical in such builders).

#include <pflog.h>
#include <stdarg.h>
#include <gencode.h>
#include <ppp.h>
#include <stdio.h>
#include <ethertype.h>
#include <extract.h>
#include <iostream>
#include <nametoaddr.h>
#include <ieee80211.h>
#include <cstdlib>
#include <stdint.h>
#include <stddef.h>
#include <thread-local.h>
#include <pcap-util.h>
#include <scanner.h>
#include <cstdio>
#include <setjmp.h>
#include <llc.h>
#include <pcap/namedb.h>
#include <pcap/ipnet.h>
#include <diag-control.h>
#include <linux/filter.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <string.h>
#include <memory.h>
#include <linux/types.h>
#include <os-proto.h>
#include <linux/if_packet.h>
#include <config.h>
#include <pcap/sll.h>


// Bring in the C API for the focal function.
// Use extern "C" to ensure proper linkage of C headers when compiled under C++.
extern "C" {
}

// Simple, non-terminating test framework (EXPECT_* style) suitable for C++11
static int g_test_failures = 0;

#define TEST_PRINT_PREFIX "[TEST] "

#define EXPECT_TRUE(cond) \
    do { \
        if(!(cond)) { \
            std::cerr << TEST_PRINT_PREFIX << "Expectation failed: " #cond \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            ++g_test_failures; \
        } \
    } while(0)

#define EXPECT_EQ(a, b) \
    do { \
        if(!((a) == (b))) { \
            std::cerr << TEST_PRINT_PREFIX << "Expectation failed: " #a " == " #b \
                      << " (actual: " << (a) << " vs " << (b) << ")" \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            ++g_test_failures; \
        } \
    } while(0)

#define EXPECT_NE(a, b) \
    do { \
        if((a) == (b)) { \
            std::cerr << TEST_PRINT_PREFIX << "Expectation failed: " #a " != " #b \
                      << " (both are " << (a) << ")" \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            ++g_test_failures; \
        } \
    } while(0)


// Test 1: gen_jmp_x should return a non-NULL block and should preserve the provided stmts pointer.
// This asserts the basic contract: the function creates a block and assigns the input stmts.
static void test_gen_jmp_x_basic_assignment_with_nonnull_stmts() {
    // Prepare a non-NULL stmts list (dummy, could be empty list head)
    struct slist dummy_list;
    struct slist *stmts = &dummy_list;

    // Use a NULL compiler state to exercise the function in a minimal context.
    compiler_state_t *cstate = NULL;

    // Call the focal function
    struct block *b = gen_jmp_x(cstate, 0, stmts);

    // Checks
    EXPECT_TRUE(b != NULL);
    // The production code assigns the input stmts pointer to the block's stmts field.
    EXPECT_EQ(b->stmts, stmts);
}

// Test 2: gen_jmp_x should handle NULL stmts gracefully, returning a block whose stmts is NULL.
static void test_gen_jmp_x_basic_assignment_with_null_stmts() {
    compiler_state_t *cstate = NULL;
    struct block *b = gen_jmp_x(cstate, 0, NULL);

    EXPECT_TRUE(b != NULL);
    EXPECT_EQ(b->stmts, (struct slist *)NULL);
}

// Test 3: Repeated invocations should yield distinct block objects (ensuring allocator usage).
static void test_gen_jmp_x_multiple_calls_produce_distinct_blocks() {
    compiler_state_t *cstate = NULL;
    struct slist dummy_list;
    struct slist *stmts = &dummy_list;

    struct block *b1 = gen_jmp_x(cstate, 0, stmts);
    struct block *b2 = gen_jmp_x(cstate, 0, stmts);

    EXPECT_TRUE(b1 != NULL);
    EXPECT_TRUE(b2 != NULL);
    // Distinct allocations are expected for separate calls
    EXPECT_NE(b1, b2);

    // Sanity: both blocks should have the same stmts pointer as input
    EXPECT_EQ(b1->stmts, stmts);
    EXPECT_EQ(b2->stmts, stmts);
}


// Runner to execute all tests.
static void run_all_tests() {
    std::cout << "[TEST] Starting test suite for gen_jmp_x...\n";

    test_gen_jmp_x_basic_assignment_with_nonnull_stmts();
    test_gen_jmp_x_basic_assignment_with_null_stmts();
    test_gen_jmp_x_multiple_calls_produce_distinct_blocks();

    if (g_test_failures == 0) {
        std::cout << "[TEST] All tests passed." << std::endl;
    } else {
        std::cout << "[TEST] " << g_test_failures << " test(s) failed." << std::endl;
    }
}


int main() {
    run_all_tests();
    return g_test_failures != 0 ? 1 : 0;
}