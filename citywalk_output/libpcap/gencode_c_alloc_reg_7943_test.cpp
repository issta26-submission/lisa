/*
 * C++11 test suite for the focal method alloc_reg (from gencode.c)
 * 
 * Notes:
 * - We treat the production function as extern "C" and call it with a
 *   void* parameter to avoid strict C++ type coupling, relying on the
 *   binary layout of compiler_state_t used by the function.
 * - We assume BPF_MEMWORDS is defined in the project headers (via gencode.h).
 * - We implement a lightweight, non-terminating test harness with EXPECT_*
 *   style macros to maximize code execution coverage without depending on
 *   any external testing framework (as requested).
 * - Tests focus on:
 *   1) false branch: first available slot is returned when regused[curreg] is 0
 *   2) true branch: when regused[curreg] is 1, the search continues and the next
 *      free slot is allocated
 *   3) wrap-around / multiple true branches leading to the last free slot
 * - We do not attempt to mock private/internal functions; we only exercise
 *   alloc_reg through its public linkage.
 *
 * Important: This test assumes the project is buildable with C code (gencode.c)
 * and that BPF_MEMWORDS is defined consistently with the production build.
 */

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
#include <vector>
#include <linux/filter.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <string.h>
#include <memory.h>
#include <linux/types.h>
#include <cstddef>
#include <os-proto.h>
#include <linux/if_packet.h>
#include <config.h>
#include <pcap/sll.h>
#include <cstring>


// Bring in C declarations for the focal function.
// We declare the function with a generic void* parameter to avoid C++-side
// type coupling; the actual implementation will treat the pointer as
// a pointer to its compiler_state_t structure.
extern "C" int alloc_reg(void *cstate);

// Include the project's header to obtain BPF_MEMWORDS (and related types)
extern "C" {
}

// Lightweight non-terminating test harness
static int test_failures = 0;

#define EXPECT_TRUE(cond, msg) \
    do { if (!(cond)) { std::cerr << "EXPECT_TRUE failed: " << (msg) << "\n"; ++test_failures; } } while (0)

#define EXPECT_FALSE(cond, msg) \
    do { if ((cond)) { std::cerr << "EXPECT_FALSE failed: " << (msg) << "\n"; ++test_failures; } } while (0)

#define EXPECT_EQ(a, b, msg) \
    do { if ((a) != (b)) { std::cerr << "EXPECT_EQ failed: " << (msg) \
        << " | expected: " << (b) << " got: " << (a) << "\n"; ++test_failures; } } while (0)

#define EXPECT_NE(a, b, msg) \
    do { if ((a) == (b)) { std::cerr << "EXPECT_NE failed: " << (msg) \
        << " | both are: " << (a) << "\n"; ++test_failures; } } while (0)

// Helper structure mirroring the essential fields used by alloc_reg
// We rely on the actual binary layout of compiler_state_t for regused[]
// and curreg; thus we mirror only the necessary parts here.
// We use BPF_MEMWORDS from the production header to size the array.
struct TestState {
    // regused should be large enough to hold BPF_MEMWORDS entries
    int regused[BPF_MEMWORDS];
    int curreg;
};

// Helper to reset the TestState to a known clean state
static void reset_state(TestState &st) {
    for (size_t i = 0; i < BPF_MEMWORDS; ++i) {
        st.regused[i] = 0;
    }
    st.curreg = 0;
}

/*
 * Test 1: False branch
 * - regused[curreg] == 0 on entry
 * - alloc_reg should set regused[curreg] = 1 and return curreg (0)
 */
static void test_alloc_reg_false_branch() {
    TestState st;
    reset_state(st);
    st.curreg = 0;

    int ret = alloc_reg(static_cast<void*>(&st));

    EXPECT_EQ(ret, 0, "alloc_reg should return initial curreg when free");
    EXPECT_EQ(st.regused[0], 1, "regused[0] should be set to 1 after allocation");
    EXPECT_EQ(st.curreg, 0, "curreg should remain unchanged after successful allocation");
}

/*
 * Test 2: True branch followed by allocation of next free slot
 * - regused[0] is set to 1 (occupied)
 * - curreg starts at 0; alloc_reg should skip 0, set curreg to 1, allocate reg 1
 *   and return 1.
 */
static void test_alloc_reg_true_branch() {
    TestState st;
    reset_state(st);
    st.curreg = 0;
    st.regused[0] = 1; // first slot taken

    int ret = alloc_reg(static_cast<void*>(&st));

    EXPECT_EQ(ret, 1, "alloc_reg should skip used slot and allocate next free slot");
    EXPECT_EQ(st.regused[1], 1, "regused[1] should be set to 1 after allocation");
    EXPECT_EQ(st.curreg, 1, "curreg should now point to the allocated register (1)");
}

/*
 * Test 3: Wrap-around / all but last slots used
 * - Mark all slots as used except the last one; allocation should return the
 *   last slot (BPF_MEMWORDS-1) and set it used.
 */
static void test_alloc_reg_wraparound_last_slot() {
    TestState st;
    reset_state(st);

    // Mark all but the last as used
    for (size_t i = 0; i < BPF_MEMWORDS - 1; ++i) {
        st.regused[i] = 1;
    }
    st.curreg = 0;
    st.regused[BPF_MEMWORDS - 1] = 0; // last slot free

    int ret = alloc_reg(static_cast<void*>(&st));

    EXPECT_EQ(ret, static_cast<int>(BPF_MEMWORDS - 1),
              "alloc_reg should allocate the last free slot (BPF_MEMWORDS-1)");
    EXPECT_EQ(st.regused[BPF_MEMWORDS - 1], 1, "last slot should be marked used after allocation");
    EXPECT_EQ(st.curreg, static_cast<int>(BPF_MEMWORDS - 1),
              "curreg should be set to the allocated slot (last index)");
}

/*
 * Test Runner
 * Execute all tests and report overall results.
 */
static void run_all_tests() {
    std::cout << "Running alloc_reg test suite...\n";

    test_alloc_reg_false_branch();
    test_alloc_reg_true_branch();
    test_alloc_reg_wraparound_last_slot();

    if (test_failures == 0) {
        std::cout << "All alloc_reg tests PASSED.\n";
    } else {
        std::cout << "alloc_reg tests completed with " << test_failures << " failure(s).\n";
    }
}

int main() {
    // Run tests
    run_all_tests();
    // Return non-zero if any test failed
    return test_failures ? 1 : 0;
}