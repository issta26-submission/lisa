// test_dag_set_datalink.cpp
// A lightweight C++11 unit test harness for the focal function dag_set_datalink
// in pcap-dag.c. This test uses the public dag_create/dag_get_datalink/dag_set_datalink
// APIs to create a pcap_t instance, set the datalink, and verify the result
// through the provided getter. No GoogleTest is used; a small custom harness
// prints non-terminating results and returns a non-zero exit code on failure.

#include <string.h>
#include <errno.h>
#include <vector>
#include <cstdlib>
#include <endian.h>
#include <dagpci.h>
#include <pcap.h>
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


// Include libpcap header to get the pcap_t type and PCAP_ERRBUF_SIZE macro.
// The project under test is compatible with C++11 test environments, and
// pcap.h is typically available in libpcap installations.

extern "C" {

// Forward declare the C functions from the dag subsystem.
// These declarations mirror the signatures used in the C source.
typedef struct pcap_t pcap_t;

pcap_t* dag_create(const char* device, char* errbuf, int* is_ours);
int dag_set_datalink(pcap_t* p, int dlt);
int dag_get_datalink(pcap_t* p);
void delete_pcap_dag(const pcap_t* p);

} // extern "C"

// Simple non-terminating assertion/logging helper
static void log_failure(const std::string& test_name, const std::string& message) {
    std::cerr << "UNIT TEST FAILURE: " << test_name << " -> " << message << std::endl;
}

// Unit test: verify dag_set_datalink correctly updates the datalink type
// and that dag_get_datalink returns the same value after setting.
// This test uses a real pcap_t instance created via dag_create and cleaned up
// with delete_pcap_dag to ensure proper integration with the actual library code.
static bool test_case_set_and_get_datalink(int dlt) {
    const std::string test_name = "test_case_set_and_get_datalink(dlt=" + std::to_string(dlt) + ")";

    // Prepare an error buffer for the DAG creation (as per typical libpcap usage)
    char errbuf[PCAP_ERRBUF_SIZE];
    int is_ours = 0;

    // Create a pcap_t instance for the DAG device; the device name is not critical
    // for this unit test as we only verify datalink management.
    pcap_t* p = dag_create("DAG_TEST_DEVICE", errbuf, &is_ours);
    if (p == nullptr) {
        log_failure(test_name, "dag_create() returned NULL. errbuf=\"" + std::string(errbuf) + "\"");
        return false;
    }

    // Act: set the datalink type via the function under test
    int ret = dag_set_datalink(p, dlt);
    if (ret != 0) {
        log_failure(test_name, "dag_set_datalink() returned non-zero: " + std::to_string(ret));
        delete_pcap_dag(p);
        return false;
    }

    // Assert: verify the new value using the getter provided by the codebase
    int actual = dag_get_datalink(p);
    if (actual != dlt) {
        log_failure(test_name, "dag_get_datalink() returned " + std::to_string(actual) +
                               ", expected " + std::to_string(dlt));
        delete_pcap_dag(p);
        return false;
    }

    // Cleanup
    delete_pcap_dag(p);
    return true;
}

int main() {
    // Lightweight, non-terminating test harness:
    // Run a series of test cases and report a summary at the end.
    // Do not abort on first failure to maximize code coverage.

    std::vector<std::string> test_names;
    int total = 0;
    int failed = 0;

    // Domain-aware test values to exercise the setter/getter
    // (true/false branches are not explicit in this function, but multiple
    // values exercise the code path with different dlt values.)
    const int test_values[] = { 1, 150, -1, 0, 65535 };
    const size_t N = sizeof(test_values) / sizeof(test_values[0]);

    for (size_t i = 0; i < N; ++i) {
        int dlt = test_values[i];
        ++total;
        if (!test_case_set_and_get_datalink(dlt)) {
            ++failed;
            test_names.push_back("dag_set_datalink(dlt=" + std::to_string(dlt) + ")");
        } else {
            test_names.push_back("dag_set_datalink(dlt=" + std::to_string(dlt) + ") PASSED");
        }
    }

    // Summary
    std::cout << "dag_set_datalink test suite finished. "
              << "Total: " << total << ", Passed: " << (total - failed)
              << ", Failed: " << failed << std::endl;

    // Optional: print detailed per-test results
    // (comment out if output verbosity is not desired)
    for (const auto& name : test_names) {
        // The per-test results were printed inline on failure; here we print the statuses as well.
        // In this minimal harness, we rely on the inline failure messages for details.
        (void)name; // suppress unused warning if not printing here
    }

    return failed ? 1 : 0;
}