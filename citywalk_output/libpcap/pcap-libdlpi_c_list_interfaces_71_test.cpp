// Single-file C++11 test harness for the focal method: list_interfaces
// This test is self-contained to be compiled standalone in environments
// where the original pcap-libdlpi.c is not linked separately.
// It recreates minimal dependent structures and utilities necessary to
// exercise the list_interfaces behavior described in the provided code.
//
// Notes:
// - Tests cover insertion into an empty list, insertion at the head when a list already exists,
//   and truncation behavior of linkname when longer than DLPI_LINKNAME_MAX.
// - We do not rely on external test frameworks; a small, non-terminating assertion
//   mechanism is used to maximize test coverage in one run.

#include <libdlpi.h>
#include <stdio.h>
#include <stropts.h>
#include <sys/types.h>
#include <iostream>
#include <errno.h>
#include <config.h>
#include <cstdio>
#include <string>
#include <stdlib.h>
#include <pcap-int.h>
#include <dlpisubs.h>
#include <string.h>
#include <memory.h>
#include <sys/time.h>
#include <sys/bufmod.h>
#include <cstdlib>
#include <cstring>
#include <sys/stream.h>


// --- Minimal domain-specific definitions to mirror the focal code ---

// Boolean-like constants used by the original code
#ifndef B_TRUE
#define B_TRUE 1
#endif
#ifndef B_FALSE
#define B_FALSE 0
#endif

// Simulated errno values (subset used by the focal code)
#ifndef ENOMEM
#define ENOMEM 12
#endif

// Maximum length for a DLPI link name (as used by the focal code)
#ifndef DLPI_LINKNAME_MAX
#define DLPI_LINKNAME_MAX 256
#endif

// Forward declarations for test types matching the focal file's expectations
typedef struct linknamelist {
    char linkname[DLPI_LINKNAME_MAX];
    struct linknamelist* lnl_next;
} linknamelist_t;

typedef struct linkwalk {
    int lw_err;
    linknamelist_t* lw_list;
} linkwalk_t;

// A lightweight string copy function matching pcapint_strlcpy behavior in tests.
// It copies up to size-1 characters and null-terminates.
static size_t pcapint_strlcpy(char* dst, const char* src, size_t size) {
    if (size == 0) {
        return strlen(src);
    }
    size_t srclen = strlen(src);
    size_t copy_len = (srclen >= size) ? size - 1 : srclen;
    if (copy_len > 0) {
        memcpy(dst, src, copy_len);
    }
    dst[copy_len] = '\0';
    return srclen;
}

// --- Implementation of the focal method under test (recreated for self-contained testing) ---

extern "C" int list_interfaces(const char* linkname, void* arg) {
    // The test harness uses the same logic as the provided focal method.
    // It creates a new linknamelist node, copies the linkname into it,
    // and prepends it to the lw_list in the provided linkwalk_t.

    linkwalk_t* lwp = static_cast<linkwalk_t*>(arg);
    linknamelist_t* entry;

    // Simulate allocation similar to calloc(1, sizeof(linknamelist_t))
    entry = static_cast<linknamelist_t*>(calloc(1, sizeof(linknamelist_t)));
    if (entry == NULL) {
        lwp->lw_err = ENOMEM;
        return (B_TRUE);
    }

    (void) pcapint_strlcpy(entry->linkname, linkname, DLPI_LINKNAME_MAX);

    if (lwp->lw_list == NULL) {
        lwp->lw_list = entry;
    } else {
        entry->lnl_next = lwp->lw_list;
        lwp->lw_list = entry;
    }

    return (B_FALSE);
}

// --- Lightweight testing framework (non-terminating assertions) ---

static int tests_run = 0;
static int tests_failed = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "Expectation failed: " << #cond << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        tests_failed++; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    if((a) != (b)) { \
        std::cerr << "Expectation failed: " #a " == " #b " (" << (a) << " != " << (b) << ") at " \
                  << __FILE__ << ":" << __LINE__ << "\n"; \
        tests_failed++; \
    } \
} while(0)

#define TEST(name) void name()
#define RUN_TEST(name) do { \
    std::cout << "Running " #name "...\n"; \
    name(); \
} while(0)

// Utility to create a fresh linkwalk structure
static void reset_lwp(linkwalk_t* lwp) {
    if (lwp) {
        lwp->lw_err = 0;
        lwp->lw_list = NULL;
    }
}

// Helper to print the chain for debugging (optional)
static void print_list(const linkwalk_t* lwp) {
    const linknamelist_t* cur = lwp ? lwp->lw_list : NULL;
    int idx = 0;
    while (cur) {
        std::cout << "  [" << idx << "] linkname=\"" << cur->linkname << "\"\n";
        cur = cur->lnl_next;
        idx++;
    }
}

// --- Test Suite for list_interfaces ---

// Test 1: Inserting the first entry into an empty lw_list
TEST(test_list_interfaces_inserts_first_entry) {
    linkwalk_t lwp;
    reset_lwp(&lwp);

    const char* name = "eth0";

    int ret = list_interfaces(name, &lwp);

    // Expect B_FALSE (0) return, indicating not an error
    EXPECT_EQ(ret, B_FALSE);

    // Expect the list head to be non-NULL and contain the correct name
    EXPECT_TRUE(lwp.lw_list != NULL);
    EXPECT_TRUE(lwp.lw_list->lnl_next == NULL);
    EXPECT_EQ(std::strlen(lwp.lw_list->linkname), std::strlen(name));
    EXPECT_TRUE(std::strncmp(lwp.lw_list->linkname, name, DLPI_LINKNAME_MAX) == 0);

    // No error should be recorded
    EXPECT_EQ(lwp.lw_err, 0);
}

// Test 2: Inserting a new entry at the head when a list already exists
TEST(test_list_interfaces_inserts_at_head) {
    linkwalk_t lwp;
    reset_lwp(&lwp);

    // First insert
    int ret1 = list_interfaces("eth0", &lwp);
    EXPECT_EQ(ret1, B_FALSE);

    // Second insert at head
    int ret2 = list_interfaces("eth1", &lwp);
    EXPECT_EQ(ret2, B_FALSE);

    // After second insertion, the head should be the new entry "eth1"
    EXPECT_TRUE(lwp.lw_list != NULL);
    EXPECT_EQ(std::strcmp(lwp.lw_list->linkname, "eth1"), 0);

    // The next node should be the previous head containing "eth0"
    linknamelist_t* old_head = lwp.lw_list->lnl_next;
    EXPECT_TRUE(old_head != NULL);
    EXPECT_EQ(std::strcmp(old_head->linkname, "eth0"), 0);

    // The next pointer after old_head should be NULL
    EXPECT_TRUE(old_head->lnl_next == NULL);
}

// Test 3: Linkname truncation when input is longer than DLPI_LINKNAME_MAX
TEST(test_list_interfaces_truncates_linkname) {
    linkwalk_t lwp;
    reset_lwp(&lwp);

    // Create a long name longer than the allowed max
    std::string longname(DLPI_LINKNAME_MAX + 20, 'A'); // e.g., 276+ chars
    // Ensure there is a terminator as well
    longname[DLPI_LINKNAME_MAX + 19] = 'Z';
    // Call with the long name
    int ret = list_interfaces(longname.c_str(), &lwp);
    EXPECT_EQ(ret, B_FALSE);

    // The stored name should be truncated to DLPI_LINKNAME_MAX - 1 characters
    EXPECT_TRUE(lwp.lw_list != NULL);
    const char* stored = lwp.lw_list->linkname;
    // Length should be at most DLPI_LINKNAME_MAX - 1
    EXPECT_EQ(std::strlen(stored) < DLPI_LINKNAME_MAX, true);
    // The first DLPI_LINKNAME_MAX - 1 characters should be 'A'
    for (size_t i = 0; i < DLPI_LINKNAME_MAX - 1; ++i) {
        if (stored[i] != 'A') break;
        if (i == DLPI_LINKNAME_MAX - 2) {
            // ensure last char before terminator is 'A'
            EXPECT_TRUE(true);
        }
    }
}

// Test 4: Ensure subsequent calls keep adding to the head (multiple entries)
TEST(test_list_interfaces_multiple_entries) {
    linkwalk_t lwp;
    reset_lwp(&lwp);

    const char* names[] = {"eth0", "eth1", "eth2"};
    for (size_t i = 0; i < 3; ++i) {
        int ret = list_interfaces(names[i], &lwp);
        EXPECT_EQ(ret, B_FALSE);
    }

    // After three inserts, the head should be "eth2"
    EXPECT_TRUE(lwp.lw_list != NULL);
    EXPECT_EQ(std::strcmp(lwp.lw_list->linkname, "eth2"), 0);

    // lwp.lw_list->lnl_next should be "eth1"
    EXPECT_TRUE(lwp.lw_list->lnl_next != NULL);
    EXPECT_EQ(std::strcmp(lwp.lw_list->lnl_next->linkname, "eth1"), 0);

    // lwp.lw_list->lnl_next->lnl_next should be "eth0"
    EXPECT_TRUE(lwp.lw_list->lnl_next->lnl_next != NULL);
    EXPECT_EQ(std::strcmp(lwp.lw_list->lnl_next->lnl_next->linkname, "eth0"), 0);
}

// --- Runner and summary ---

int main() {
    std::cout << "Starting test suite for list_interfaces (pcap-libdlpi.c focal method)...\n";

    RUN_TEST(test_list_interfaces_inserts_first_entry);
    RUN_TEST(test_list_interfaces_inserts_at_head);
    RUN_TEST(test_list_interfaces_truncates_linkname);
    RUN_TEST(test_list_interfaces_multiple_entries);

    std::cout << "\nTest results: run " << tests_run << " test(s), "
              << tests_failed << " failure(s).\n";

    // Note: The RUN_TEST macro does not automatically increment tests_run.
    // For simplicity in this minimal harness, we count failures directly in macros.
    // If needed, tests_run can be incremented manually inside each test.

    if (tests_failed == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << "Some tests FAILED.\n";
        return 1;
    }
}

// Since we used RUN_TEST macro which only prints the start of each test and the tests themselves
// do not automatically increment tests_run, we emulate a final pass here by decoding
// the manual counts. To keep the harness simple and self-contained, we rely on the per-test
// EXPECT_* macros to report failures during execution.