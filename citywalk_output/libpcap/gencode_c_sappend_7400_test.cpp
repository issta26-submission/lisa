// C++11 test suite for the C function: sappend(struct slist *s0, struct slist *s1)
// The tests are designed to be lightweight, rely only on the C standard library,
// and do not use GTest. A small non-terminating test harness is implemented via
// EXPECT_* macros that accumulate failures and report at the end.

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


// Forward declaration of the C function under test with C linkage.
// We also provide a minimal definition of struct slist here to match the expected
// memory layout for the operation performed by sappend.
extern "C" {
struct slist {
    slist *next;
};

void sappend(struct slist *s0, struct slist *s1);
}

// Lightweight non-terminating test assertions (like EXPECT_* in GTest)
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE FAILED: " << (msg) \
                  << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a, b, msg) do { \
    if((void*)(a) != (void*)(b)) { \
        std::cerr << "EXPECT_PTR_EQ FAILED: " << (msg) \
                  << " expected " << (void*)(b) << " but got " << (void*)(a) \
                  << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ_INT(a, b, msg) do { \
    if((a) != (b)) { \
        std::cerr << "EXPECT_EQ_INT FAILED: " << (msg) \
                  << " expected " << (b) << " but got " << (a) \
                  << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_failures; \
    } \
} while(0)

// Test 1: When s0 has a single node and s1 is a two-node list (B -> C),
// after sappend, s0 should point to B and the chain should be B -> C -> nullptr.
void test_sappend_case_single_s0_multi_s1() {
    // Arrange
    slist a; a.next = nullptr;       // s0: A
    slist b; b.next = nullptr;       // s1 head: B
    slist c; c.next = nullptr;       // s1 tail: C
    b.next = &c;                     // B -> C

    // Act
    sappend(&a, &b);

    // Assert: verify head connection and preserved s1 chain
    EXPECT_TRUE(a.next == &b, "After append, s0 should point to the head of s1 (B).");
    EXPECT_TRUE(b.next == &c, "s1 chain should remain intact: B -> C.");
    EXPECT_TRUE(c.next == nullptr, "End of s1 chain should be nullptr.");

    // Additional traversal verification: A -> B -> C
    slist *p = a.next; // B
    EXPECT_PTR_EQ(p, &b, "First node after A should be B.");
    p = p->next;       // C
    EXPECT_PTR_EQ(p, &c, "Second node after A should be C.");
    p = p->next;       // nullptr
    EXPECT_TRUE(p == nullptr, "End of traversal should be nullptr.");
}

// Test 2: When s0 has multiple nodes (A1 -> A2 -> A3) and s1 is a single node (S1),
// after sappend, the tail of s0 (A3) should point to S1.
void test_sappend_case_multi_s0_single_s1() {
    // Arrange
    slist a1, a2, a3;
    a1.next = &a2;
    a2.next = &a3;
    a3.next = nullptr;

    slist s1; s1.next = nullptr;    // S1 is a single node with no next

    // Act
    sappend(&a1, &s1);

    // Assert: tail of A1..A3 should now point to S1
    slist *p = &a1;
    p = p->next; // A2
    EXPECT_PTR_EQ(p, &a2, "First node after A1 should be A2.");
    p = p->next; // A3
    EXPECT_PTR_EQ(p, &a3, "Second node after A1 should be A3.");
    p = p->next; // S1
    EXPECT_PTR_EQ(p, &s1, "Tail should connect to S1 (S1 head).");
    p = p->next; // nullptr
    EXPECT_TRUE(p == nullptr, "End of traversal should be nullptr.");

    // Also verify S1.next remains nullptr
    EXPECT_TRUE(s1.next == nullptr, "S1 should remain a single node with no next.");
}

// Test 3: When s1 is NULL, sappend should set the end of s0 to NULL (effectively
// truncating the list tail at the end of s0).
void test_sappend_case_s1_null() {
    // Arrange
    slist a1b, a1c;
    a1b.next = &a1c; // A chain: A -> B
    a1c.next = nullptr;

    // Act
    sappend(&a1b, nullptr);

    // Assert: the tail (A1C) should now have next == nullptr (unchanged in this case)
    EXPECT_TRUE(a1b.next == &a1c, "Head's next should still be A1C after appending NULL.");
    EXPECT_TRUE(a1c.next == nullptr, "Tail's next should remain nullptr after append with NULL.");
}

int main() {
    // Run tests
    test_sappend_case_single_s0_multi_s1();
    test_sappend_case_multi_s0_single_s1();
    test_sappend_case_s1_null();

    // Summary
    if (g_failures == 0) {
        std::cout << "ALL_TESTS_PASSED\n";
    } else {
        std::cerr << g_failures << " TEST(S) FAILED\n";
    }
    return g_failures;
}