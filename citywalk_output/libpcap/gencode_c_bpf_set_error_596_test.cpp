/*
 * Unit tests for the focal method:
 *   bpf_set_error(compiler_state_t *cstate, const char *fmt, ...)
 *
 * Purpose:
 * - Verify that when cstate->error_set is 0, the function formats the message into
 *   cstate->bpf_pcap->errbuf and sets cstate->error_set to 1.
 * - Verify that when cstate->error_set is already 1, the function does not override
 *   the existing error message or clear the error flag.
 *
 * Notes:
 * - This test uses the project's actual gencode.h and the real types defined there.
 * - The tests rely on the presence of compiler_state_t and bpf_pcap (as used inside
 *   gencode.c). It uses a lightweight, non-terminating assertion approach.
 * - We assume PCAP_ERRBUF_SIZE is defined by the project headers.
 * - The tests are written for C++11 (no Google Test/GTest allowed).
 *
 * How to run (conceptual in this environment):
 * - Compile this test together with the project's sources (or link against the
 *   gencode.o / library that provides bpf_set_error).
 * - Ensure the include path contains the project's headers (e.g., -I/path/to/project/include).
 * - Run the produced executable; it will print PASS/FAIL details.
 */

// Include the project's focal header to obtain real type definitions.
// This header is expected to declare compiler_state_t and the relevant bpf_pcap type.
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
#include <type_traits>
#include <stdint.h>
#include <stddef.h>
#include <thread-local.h>
#include <pcap-util.h>
#include <scanner.h>
#include <cstdio>
#include <setjmp.h>
#include <string>
#include <llc.h>
#include <pcap/namedb.h>
#include <pcap/ipnet.h>
#include <diag-control.h>
#include <cstdarg>
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



// Declare the focal function with C linkage for correct linkage in C++ tests.
extern "C" void bpf_set_error(compiler_state_t *cstate, const char *fmt, ...);

static int gFailures = 0;

// Lightweight non-terminating assertion macro
#define CHECK(cond, msg) \
    do { \
        if (!(cond)) { \
            ++gFailures; \
            std::cerr << "TEST FAILURE: " << (msg) << " [at " << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
        } \
    } while (0)

// Helper: read the current errbuf safely as a std::string (assumes errbuf is NUL-terminated)
static std::string read_errbuf(const compiler_state_t *cstate) {
    // Access through the known path: cstate->bpf_pcap->errbuf
    // We assume the underlying types are defined by gencode.h.
    return std::string(cstate->bpf_pcap->errbuf);
}

// Test 1: When error_set is 0, bpf_set_error should format the message into errbuf and set error_set = 1
static void test_sets_error_and_marks_flag() {
    // Allocate and zero a minimal compiler_state_t instance
    compiler_state_t *cstate = reinterpret_cast<compiler_state_t*>(std::malloc(sizeof(compiler_state_t)));
    std::memset(cstate, 0, sizeof(compiler_state_t));

    // Allocate the nested bpf_pcap object (type is defined by gencode.h)
    // Use type deduction to remain agnostic to the exact underlying typedefs.
    using BpfPcapType = typename std::remove_pointer<decltype(cstate->bpf_pcap)>::type;
    cstate->bpf_pcap = new BpfPcapType();
    // Initialize its internal buffer to an empty string to start clean
    std::memset(reinterpret_cast<void*>(cstate->bpf_pcap->errbuf), 0, PCAP_ERRBUF_SIZE);

    // Call the focal function
    bpf_set_error(cstate, "hello %d", 42);

    // Validate: error_set should be 1
    CHECK(cstate->error_set == 1, "error_set should be set to 1 after first error");

    // Validate: errbuf should contain the formatted string
    std::string got = read_errbuf(cstate);
    CHECK(got == std::string("hello 42"), "errbuf should contain the formatted message 'hello 42'");

    // Cleanup
    delete cstate->bpf_pcap;
    std::free(cstate);
}

// Test 2: When error_set is already non-zero, bpf_set_error should not overwrite the existing error
static void test_does_not_override_when_error_already_set() {
    // Allocate and zero a minimal compiler_state_t instance
    compiler_state_t *cstate = reinterpret_cast<compiler_state_t*>(std::malloc(sizeof(compiler_state_t)));
    std::memset(cstate, 0, sizeof(compiler_state_t));

    // Allocate the nested bpf_pcap object
    using BpfPcapType = typename std::remove_pointer<decltype(cstate->bpf_pcap)>::type;
    cstate->bpf_pcap = new BpfPcapType();
    std::memset(reinterpret_cast<void*>(cstate->bpf_pcap->errbuf), 0, PCAP_ERRBUF_SIZE);

    // Pre-set an existing error to simulate a lexical or syntax error already captured
    cstate->error_set = 1;
    std::snprintf(cstate->bpf_pcap->errbuf, PCAP_ERRBUF_SIZE, "existing error");

    // Attempt to set a new error; this should be ignored due to error_set being non-zero
    bpf_set_error(cstate, "new error: %s", "ignored");

    // Validate: error_set remains 1
    CHECK(cstate->error_set == 1, "error_set should remain 1 when called again with non-zero state");

    // Validate: errbuf remains unchanged
    std::string got = read_errbuf(cstate);
    CHECK(got == std::string("existing error"), "errbuf should not be overwritten when error already set");

    // Cleanup
    delete cstate->bpf_pcap;
    std::free(cstate);
}

int main() {
    // Run tests
    std::cout << "Running bpf_set_error unit tests (C++11)..." << std::endl;

    test_sets_error_and_marks_flag();
    test_does_not_override_when_error_already_set();

    if (gFailures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << gFailures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}