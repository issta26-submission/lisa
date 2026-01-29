/*
  Lightweight C++11 test suite for the C function:
    dag_stream_long_description(const unsigned stream, const dag_size_t bufsize,
                              const dag_card_inf_t * inf)
  The tests are designed to be GTest-free and rely on a small, self-contained
  harness that exercises both branches of the conditional inside the function.

  Notes:
  - We assume the project headers (pcap-dag.h, etc.) are available in include paths
    and provide the necessary types/macros (dag_card_inf_t, dag_size_t,
    PCI_DEVICE_ID_VDAG, dag_device_name, RXTX_STR, etc.).
  - Tests avoid matching the entire returned string (which depends on MACRO
    RXTX_STR and dag_device_name) and instead validate the stable substrings and
    branch-specific fragments to ensure correct behavior.
  - Static local buffer in the focal function is not directly testable, but
    we verify that callers can rely on stable fragments being produced in the
    returned string for successive invocations.
  - Custom, non-terminating assertions are implemented via a tiny harness to keep
    execution flow and coverage high without depending on GTest.
*/

#include <string.h>
#include <errno.h>
#include <vector>
#include <endian.h>
#include <dagpci.h>
#include <pcap-util.h>
#include <unistd.h>
#include <pcap-dag.h>
#include <limits.h>
#include <dag_config_api.h>
#include <stdlib.h>
#include <config.h>
#include <pcap-int.h>
#include <string>
#include <iostream>
#include <dagapi.h>


// Bring in the C declarations (function under test and related types).
// The project likely provides pcap-dag.h (or similar) with the required API.
extern "C" {
}

// Helper to convert a C string from the focal function into a C++ string safely.
static std::string cstr_to_string(const char* s) {
    return s ? std::string(s) : std::string();
}

// Simple, non-terminating assertion replacement for test continuity.
static void expect_true(bool cond, const std::string& msg) {
    if (!cond) {
        std::cerr << "ASSERT FAILED: " << msg << std::endl;
    }
}

// Wrapper around the C function for convenient use in C++ tests.
static std::string dag_desc(unsigned stream, dag_size_t bufsize, const dag_card_inf_t* inf) {
    // The focal function returns a pointer to an internal static buffer.
    // Copy immediately to an std::string to avoid subsequent mutations.
    const char* raw = dag_stream_long_description(stream, bufsize, inf);
    return cstr_to_string(raw);
}

// Test 1: VDAG device_code branch (device_code == PCI_DEVICE_ID_VDAG)
// Expected: The base description is present (" stream <>, <MiB> MiB, ") and
// no "rev ... at ..." fragment is appended.
static bool test_vdag_branch() {
    // Prepare inf for VDAG device
    dag_card_inf_t inf;
    inf.device_code = PCI_DEVICE_ID_VDAG;
    inf.brd_rev = 5;               // Arbitrary; not used in VDAG branch
    inf.bus_id = "bus_vdag_01";     // Should not affect branch-guard

    std::string out = dag_desc(5, 10 * 1024 * 1024, &inf); // 10 MiB

    // Check for base description fragment: " stream 5, 10 MiB, "
    bool base_present = (out.find(" stream 5, 10 MiB, ") != std::string::npos);
    // Ensure the "rev" fragment is not included for VDAG
    bool no_rev_fragment = (out.find(" rev ") == std::string::npos);

    if (!base_present) {
        std::cerr << "test_vdag_branch: base fragment missing. Output: " << out << std::endl;
    }
    if (!no_rev_fragment) {
        std::cerr << "test_vdag_branch: unexpected rev fragment present. Output: " << out << std::endl;
    }

    expect_true(base_present, "VDAG branch should include base 'stream' and 'MiB' fragment.");
    expect_true(no_rev_fragment, "VDAG branch should not include 'rev ...' fragment.");

    return base_present && no_rev_fragment;
}

// Test 2: Non-VDAG device_code with brd_rev = 4 (should render 'E') and bus_id included
// Expected: The string contains base fragment, then " rev E at <bus_id>".
static bool test_non_vdag_rev4_branch() {
    dag_card_inf_t inf;
    inf.device_code = 0x1234; // Not PCI_DEVICE_ID_VDAG
    inf.brd_rev = 4;          // < 26 yields 'A' + 4 -> 'E'
    inf.bus_id = "bus_nonvdag_04";

    std::string out = dag_desc(2, 3 * 1024 * 1024, &inf); // 3 MiB

    bool base_present = (out.find(" stream 2, 3 MiB, ") != std::string::npos);
    bool rev_fragment = (out.find(" rev E at ") != std::string::npos);
    bool bus_present = (out.find("bus_nonvdag_04") != std::string::npos);

    if (!base_present) {
        std::cerr << "test_non_vdag_rev4_branch: base fragment missing. Output: " << out << std::endl;
    }
    if (!rev_fragment) {
        std::cerr << "test_non_vdag_rev4_branch: expected 'rev E at' fragment missing. Output: " << out << std::endl;
    }
    if (!bus_present) {
        std::cerr << "test_non_vdag_rev4_branch: expected bus_id not found. Output: " << out << std::endl;
    }

    expect_true(base_present, "Non-VDAG (brd_rev 4) should include base 'stream' fragment.");
    expect_true(rev_fragment, "Non-VDAG (brd_rev 4) should include 'rev E at' fragment.");
    expect_true(bus_present, "Non-VDAG (brd_rev 4) should include bus_id in output.");

    return base_present && rev_fragment && bus_present;
}

// Test 3: Non-VDAG device_code with brd_rev >= 26 should render '?' in the letter slot
// Expected: The substring "rev ? at" is present.
static bool test_non_vdag_rev26_branch() {
    dag_card_inf_t inf;
    inf.device_code = 0xFFFF; // Not VDAG
    inf.brd_rev = 26;          // >= 26 -> '?'
    inf.bus_id = "bus_nonvdag_26";

    std::string out = dag_desc(7, 2 * 1024 * 1024, &inf); // 2 MiB

    bool base_present = (out.find(" stream 7, 2 MiB, ") != std::string::npos);
    bool q_fragment = (out.find(" rev ? at ") != std::string::npos);
    bool bus_present = (out.find("bus_nonvdag_26") != std::string::npos);

    if (!base_present) {
        std::cerr << "test_non_vdag_rev26_branch: base fragment missing. Output: " << out << std::endl;
    }
    if (!q_fragment) {
        std::cerr << "test_non_vdag_rev26_branch: expected 'rev ? at' fragment missing. Output: " << out << std::endl;
    }
    if (!bus_present) {
        std::cerr << "test_non_vdag_rev26_branch: expected bus_id not found. Output: " << out << std::endl;
    }

    expect_true(base_present, "Non-VDAG (brd_rev 26) should include base 'stream' fragment.");
    expect_true(q_fragment, "Non-VDAG (brd_rev 26) should include 'rev ? at' fragment.");
    expect_true(bus_present, "Non-VDAG (brd_rev 26) should include bus_id in output.");

    return base_present && q_fragment && bus_present;
}

// Simple test harness structure
struct TestCase {
    const char* name;
    bool (*func)();
};

// Main entry: runs all tests and reports summary
int main() {
    std::vector<TestCase> tests = {
        { "dag_stream_long_description - VDAG branch", test_vdag_branch },
        { "dag_stream_long_description - Non-VDAG brd_rev 4 branch", test_non_vdag_rev4_branch },
        { "dag_stream_long_description - Non-VDAG brd_rev 26 branch", test_non_vdag_rev26_branch }
    };

    int passed = 0;
    int failed = 0;

    std::cout << "Running " << tests.size() << " tests for dag_stream_long_description..." << std::endl;

    for (const auto& t : tests) {
        bool result = false;
        try {
            result = t.func();
        } catch (...) {
            result = false;
            std::cerr << "Test threw an exception: " << t.name << std::endl;
        }
        if (result) {
            ++passed;
            std::cout << "[PASSED] " << t.name << std::endl;
        } else {
            ++failed;
            std::cout << "[FAILED] " << t.name << std::endl;
        }
    }

    std::cout << "Summary: " << passed << " passed, " << failed << " failed." << std::endl;
    return (failed == 0) ? 0 : 1;
}