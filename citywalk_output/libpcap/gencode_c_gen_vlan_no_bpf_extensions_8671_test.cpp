// Unit tests for gen_vlan_no_bpf_extensions in gencode.c
// Language: C++11
// Notes:
// - This test suite targets the focal method gen_vlan_no_bpf_extensions as declared
//   in the project's gencode.h (C API, compiled as part of the C codebase).
// - We rely on the project's actual implementations of dependent functions
//   (gen_vlan_tpid_test, gen_vlan_vid_test, gen_and) so that the test exercises
//   real code paths. The test harness uses the provided compiler_state_t and block
//   types from the header, ensuring alignment with the actual project definitions.
// - The tests are written with a lightweight, self-contained harness (no GTest).
// - We focus on branch coverage for the has_vlan_tag predicate (true/false) and
//   verify basic side effects (offsets updated). We keep assertions non-fatal
//   within each test function to maximize execution coverage.

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
#include <cstdint>
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


// Include the project's C header for the focal API and types.
// The header is expected to provide C declarations and extern "C" compatibility
// for C++ consumers.
extern "C" {
}

// Lightweight test harness (non-terminating style).
// Each test returns true on success and false on failure, while printing
// diagnostic messages to stdout/stderr. This allows the main() function
// to aggregate results without aborting on the first failure.

static bool test_gen_vlan_no_bpf_extensions_no_vlan_tag() {
    // Objective:
    // - Exercise the false branch of the has_vlan_tag predicate (has_vlan_tag == 0).
    // - Validate that the function returns a non-null block and that the
    //   VLAN-related offsets are advanced by 4 as specified in the focal method.

    // Prepare a zero-initialized compiler_state_t as expected by the focal API.
    compiler_state_t cstate;
    std::memset(&cstate, 0, sizeof(cstate));

    // Ensure initial values are deterministic (if the struct layout allows).
    // Access to nested fields relies on the real type definition from gencode.h.
    // The test relies on the side effect that the function increments both offsets
    // by 4 when VLAN tag handling is not bypassed.
    cstate.off_linkpl.constant_part = 0;
    cstate.off_linktype.constant_part = 0;

    // Call the focal function with has_vlan_tag = 0 (no VLAN tag in payload).
    struct block *result = gen_vlan_no_bpf_extensions(&cstate, 42, 0);

    bool ok = true;
    if (result == nullptr) {
        std::cerr << "[test_gen_vlan_no_bpf_extensions_no_vlan_tag] FAIL: result is null." << std::endl;
        ok = false;
    }

    // Validate the expected side effects on the compiler state.
    // Both offset parts should be incremented by 4 as per the focal function.
    if (cstate.off_linkpl.constant_part != 4) {
        std::cerr << "[test_gen_vlan_no_bpf_extensions_no_vlan_tag] FAIL: off_linkpl.constant_part == "
                  << cstate.off_linkpl.constant_part << ", expected 4." << std::endl;
        ok = false;
    }
    if (cstate.off_linktype.constant_part != 4) {
        std::cerr << "[test_gen_vlan_no_bpf_extensions_no_vlan_tag] FAIL: off_linktype.constant_part == "
                  << cstate.off_linktype.constant_part << ", expected 4." << std::endl;
        ok = false;
    }

    // If all checks pass, test succeeds.
    return ok;
}

// Explanation:
// - Objective: Exercise the true branch of the has_vlan_tag predicate
//   (has_vlan_tag == 1). This path also validates the same offset increments
//   and ensures a non-null result is produced when VLAN tag handling is enabled.

static bool test_gen_vlan_no_bpf_extensions_with_vlan_tag() {
    compiler_state_t cstate;
    std::memset(&cstate, 0, sizeof(cstate));

    cstate.off_linkpl.constant_part = 0;
    cstate.off_linktype.constant_part = 0;

    // Use an arbitrary VLAN number to exercise the function.
    struct block *result = gen_vlan_no_bpf_extensions(&cstate, 1234, 1);

    bool ok = true;
    if (result == nullptr) {
        std::cerr << "[test_gen_vlan_no_bpf_extensions_with_vlan_tag] FAIL: result is null." << std::endl;
        ok = false;
    }

    // Verify the same offset increments occur even when the VLAN tag path is active.
    if (cstate.off_linkpl.constant_part != 4) {
        std::cerr << "[test_gen_vlan_no_bpf_extensions_with_vlan_tag] FAIL: off_linkpl.constant_part == "
                  << cstate.off_linkpl.constant_part << ", expected 4." << std::endl;
        ok = false;
    }
    if (cstate.off_linktype.constant_part != 4) {
        std::cerr << "[test_gen_vlan_no_bpf_extensions_with_vlan_tag] FAIL: off_linktype.constant_part == "
                  << cstate.off_linktype.constant_part << ", expected 4." << std::endl;
        ok = false;
    }

    return ok;
}

// Optional: Additional scenario to ensure repeated calls do not crash and produce valid results.
// This test exercises both branches in separate fresh state instances to maximize coverage.

static bool test_gen_vlan_no_bpf_extensions_branch_coverage() {
    // Branch 1: has_vlan_tag = 0
    {
        compiler_state_t cstate;
        std::memset(&cstate, 0, sizeof(cstate));
        cstate.off_linkpl.constant_part = 0;
        cstate.off_linktype.constant_part = 0;
        struct block *r0 = gen_vlan_no_bpf_extensions(&cstate, 7, 0);
        if (r0 == nullptr) {
            std::cerr << "[test_gen_vlan_no_bpf_extensions_branch_coverage] FAIL: branch0 result null." << std::endl;
            return false;
        }
        if (cstate.off_linkpl.constant_part != 4 || cstate.off_linktype.constant_part != 4) {
            std::cerr << "[test_gen_vlan_no_bpf_extensions_branch_coverage] FAIL: branch0 offsets not 4 as expected." << std::endl;
            return false;
        }
    }

    // Branch 2: has_vlan_tag = 1
    {
        compiler_state_t cstate;
        std::memset(&cstate, 0, sizeof(cstate));
        cstate.off_linkpl.constant_part = 0;
        cstate.off_linktype.constant_part = 0;
        struct block *r1 = gen_vlan_no_bpf_extensions(&cstate, 7, 1);
        if (r1 == nullptr) {
            std::cerr << "[test_gen_vlan_no_bpf_extensions_branch_coverage] FAIL: branch1 result null." << std::endl;
            return false;
        }
        if (cstate.off_linkpl.constant_part != 4 || cstate.off_linktype.constant_part != 4) {
            std::cerr << "[test_gen_vlan_no_bpf_extensions_branch_coverage] FAIL: branch1 offsets not 4 as expected." << std::endl;
            return false;
        }
    }

    return true;
}

int main() {
    int total = 0;
    int failures = 0;

    std::cout << "Starting test suite: gen_vlan_no_bpf_extensions (C API) - C++11 harness" << std::endl;

    // Run test 1: VLAN tag absent
    ++total;
    if (!test_gen_vlan_no_bpf_extensions_no_vlan_tag()) {
        ++failures;
        std::cout << "[FAILED] test_gen_vlan_no_bpf_extensions_no_vlan_tag" << std::endl;
    } else {
        std::cout << "[PASSED] test_gen_vlan_no_bpf_extensions_no_vlan_tag" << std::endl;
    }

    // Run test 2: VLAN tag present
    ++total;
    if (!test_gen_vlan_no_bpf_extensions_with_vlan_tag()) {
        ++failures;
        std::cout << "[FAILED] test_gen_vlan_no_bpf_extensions_with_vlan_tag" << std::endl;
    } else {
        std::cout << "[PASSED] test_gen_vlan_no_bpf_extensions_with_vlan_tag" << std::endl;
    }

    // Run test 3: Branch coverage (dual-branch checks in isolated scopes)
    ++total;
    if (!test_gen_vlan_no_bpf_extensions_branch_coverage()) {
        ++failures;
        std::cout << "[FAILED] test_gen_vlan_no_bpf_extensions_branch_coverage" << std::endl;
    } else {
        std::cout << "[PASSED] test_gen_vlan_no_bpf_extensions_branch_coverage" << std::endl;
    }

    std::cout << "Test results: " << (total - failures) << " / " << total << " tests passed." << std::endl;
    return failures ? 1 : 0;
}