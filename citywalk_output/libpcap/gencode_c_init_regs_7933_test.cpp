// test_init_regs.cpp
// Purpose: Validate the behavior of init_regs(compiler_state_t *cstate) from gencode.c
// Notes:
// - This test assumes the project provides compiler_state_t with members:
//   - int curreg;
//   - some array regused[] used by init_regs via memset.
// - We avoid GTest/GMock; provide a lightweight test harness with non-terminating checks.

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
#include <stdint.h>
#include <stddef.h>
#include <thread-local.h>
#include <pcap-util.h>
#include <scanner.h>
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
#include <cstring>


// Bring in C declarations (structure and function under test).
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Simple non-terminating assertion macros suitable for a light-weight test harness.
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        ++g_failures; \
        std::cout << "EXPECT_TRUE failed: " << (msg) << std::endl; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if( (a) != (b) ) { \
        ++g_failures; \
        std::cout << "EXPECT_EQ failed: " << (msg) \
                  << " (expected " << (b) << ", got " << (a) << ")" << std::endl; \
    } \
} while(0)

// Test 1: Basic reset behavior
// - Precondition: cstate->curreg != 0 and regused[] filled with non-zero values.
// - Action: call init_regs
// - Postcondition: curreg == 0 and all regused[] elements == 0
bool test_init_regs_resets_curreg_and_regused() {
    compiler_state_t st;

    // Precondition setup: non-zero initial state
    st.curreg = 5;
    size_t reg_len = sizeof(st.regused) / sizeof(st.regused[0]);
    for (size_t i = 0; i < reg_len; ++i) {
        st.regused[i] = 1; // non-zero to ensure memset has an effect
    }

    // Act
    init_regs(&st);

    // Assert
    bool pass = true;
    if (st.curreg != 0) {
        std::cout << "Test 1: curreg was not reset to 0 (value=" << st.curreg << ")" << std::endl;
        pass = false;
    }
    for (size_t i = 0; i < reg_len; ++i) {
        if (st.regused[i] != 0) {
            std::cout << "Test 1: regused[" << i << "] was not cleared (value=" << st.regused[i] << ")" << std::endl;
            pass = false;
            break;
        }
    }

    return pass;
}

// Test 2: Idempotence when already zero
// - Precondition: curreg == 0 and regused[] already zeros.
// - Action: call init_regs
// - Postcondition: curreg remains 0 and all regused[] remain 0
bool test_init_regs_with_already_zeroed() {
    compiler_state_t st;
    st.curreg = 0;
    size_t reg_len = sizeof(st.regused) / sizeof(st.regused[0]);
    for (size_t i = 0; i < reg_len; ++i) st.regused[i] = 0; // ensure zeros

    // Act
    init_regs(&st);

    // Assert
    bool pass = true;
    if (st.curreg != 0) {
        std::cout << "Test 2: curreg changed unexpectedly (value=" << st.curreg << ")" << std::endl;
        pass = false;
    }
    for (size_t i = 0; i < reg_len; ++i) {
        if (st.regused[i] != 0) {
            std::cout << "Test 2: regused[" << i << "] changed unexpectedly (value=" << st.regused[i] << ")" << std::endl;
            pass = false;
            break;
        }
    }

    return pass;
}

// Test 3: Idempotence over multiple consecutive calls
// - Precondition: regused[] non-zero and curreg non-zero
// - Action: call init_regs twice
// - Postcondition: curreg == 0 and all regused[] == 0
bool test_init_regs_idempotent_on_multiple_calls() {
    compiler_state_t st;
    st.curreg = 7;
    size_t reg_len = sizeof(st.regused) / sizeof(st.regused[0]);
    for (size_t i = 0; i < reg_len; ++i) st.regused[i] = 1234;

    // Act: multiple calls
    init_regs(&st);
    init_regs(&st);

    // Assert
    bool pass = true;
    if (st.curreg != 0) {
        std::cout << "Test 3: curreg not reset to 0 after multiple calls (value=" << st.curreg << ")" << std::endl;
        pass = false;
    }
    for (size_t i = 0; i < reg_len; ++i) {
        if (st.regused[i] != 0) {
            std::cout << "Test 3: regused[" << i << "] not cleared after multiple calls (value=" << st.regused[i] << ")" << std::endl;
            pass = false;
            break;
        }
    }
    return pass;
}

int main() {
    std::cout << "Running test_init_regs suite..." << std::endl;

    bool all_ok = true;

    // Run Test 1
    if (test_init_regs_resets_curreg_and_regused()) {
        std::cout << "Test 1 PASSED" << std::endl;
    } else {
        EXPECT_TRUE(false, "test_init_regs_resets_curreg_and_regused failed");
        all_ok = false;
        std::cout << "Test 1 FAILED" << std::endl;
    }

    // Run Test 2
    if (test_init_regs_with_already_zeroed()) {
        std::cout << "Test 2 PASSED" << std::endl;
    } else {
        EXPECT_TRUE(false, "test_init_regs_with_already_zeroed failed");
        all_ok = false;
        std::cout << "Test 2 FAILED" << std::endl;
    }

    // Run Test 3
    if (test_init_regs_idempotent_on_multiple_calls()) {
        std::cout << "Test 3 PASSED" << std::endl;
    } else {
        EXPECT_TRUE(false, "test_init_regs_idempotent_on_multiple_calls failed");
        all_ok = false;
        std::cout << "Test 3 FAILED" << std::endl;
    }

    if (g_failures > 0) {
        // Fallback for any unconditional failures captured by EXPECT_TRUE/EXPECT_EQ
        all_ok = false;
    }

    if (all_ok) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << "Some tests FAILED." << std::endl;
        return 1;
    }
}