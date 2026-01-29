/*
Unit test suite for the focal method: pcap_t *dag_create(const char *device, char *ebuf, int *is_ours)

Notes:
- This test suite targets the real implementation as provided by libpcap (DAG capture backend).
- It uses a lightweight, non-terminating assertion approach suitable for C++11, without GoogleTest.
- It assumes libpcap is available in the environment (headers and linkage for pcap.h and -lpcap).
- Each test is implemented as a separate function with explanatory comments.
- The tests exercise true/false branches based on device name parsing and boundary conditions.

Build guidance (if environment provides libpcap):
g++ -std=c++11 -o dag_create_tests dag_create_tests.cpp -lpcap
*/

#include <string.h>
#include <errno.h>
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
#include <cstring>
#include <iostream>
#include <dagapi.h>


// Lightweight test harness (non-terminating assertions)
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

static void log_result(const std::string& test_name, bool ok, const std::string& details = "") {
    ++tests_run;
    if (ok) {
        ++tests_passed;
        std::cout << "[PASS] " << test_name;
        if (!details.empty()) std::cout << " - " << details;
        std::cout << std::endl;
    } else {
        ++tests_failed;
        std::cout << "[FAIL] " << test_name;
        if (!details.empty()) std::cout << " - " << details;
        std::cout << std::endl;
    }
}

// Helper to safely print a summary at the end
static void print_summary() {
    std::cout << "\nTest run summary: " << tests_run
              << " ran, " << tests_passed << " passed, "
              << tests_failed << " failed." << std::endl;
}

/*
Test 1: Non-DAG device string should return NULL and is_ours should remain 0.
- Device: "eth0"
- Expectation: pcap_t* == NULL, is_ours == 0, ebuf unchanged (empty).
*/
static void test_dag_create_non_dag_device_returns_null_and_no_ours() {
    char ebuf[PCAP_ERRBUF_SIZE];
    // Initialize error buffer to a known non-empty value to ensure no silent overwrite.
    std::memset(ebuf, 'X', sizeof(ebuf));
    ebuf[PCAP_ERRBUF_SIZE - 1] = '\0';
    ebuf[0] = '\0'; // reset to empty for clean check

    int is_ours = -1;

    pcap_t *p = dag_create("eth0", ebuf, &is_ours);

    bool ok = (p == nullptr) && (is_ours == 0) && (ebuf[0] == '\0');
    log_result("test_dag_create_non_dag_device_returns_null_and_no_ours", ok,
               (ok ? "" : "Expected NULL p, is_ours=0, empty ebuf"));
    if (p != nullptr) pcap_close(p);
}

/*
Test 2: Basic valid DAG device name without explicit stream.
- Device: "dag0"
- Expectation: pcap_t* != NULL, is_ours == 1, ebuf empty.
*/
static void test_dag_create_basic_dag_device() {
    char ebuf[PCAP_ERRBUF_SIZE];
    std::memset(ebuf, 0, sizeof(ebuf));

    int is_ours = 0;
    pcap_t *p = dag_create("dag0", ebuf, &is_ours);

    bool ok = (p != nullptr) && (is_ours == 1) && (ebuf[0] == '\0');
    log_result("test_dag_create_basic_dag_device", ok,
               (ok ? "" : "Expected non-NULL p, is_ours=1, empty ebuf"));
    if (p != nullptr) pcap_close(p);
}

/*
Test 3: DAG device name with stream number.
- Device: "dag0:1"
- Expectation: pcap_t* != NULL, is_ours == 1, ebuf empty.
*/
static void test_dag_create_dag_with_stream() {
    char ebuf[PCAP_ERRBUF_SIZE];
    std::memset(ebuf, 0, sizeof(ebuf));

    int is_ours = 0;
    pcap_t *p = dag_create("dag0:1", ebuf, &is_ours);

    bool ok = (p != nullptr) && (is_ours == 1) && (ebuf[0] == '\0');
    log_result("test_dag_create_dag_with_stream", ok,
               (ok ? "" : "Expected non-NULL p, is_ours=1, empty ebuf"));
    if (p != nullptr) pcap_close(p);
}

/*
Test 4: Invalid syntax after a colon (non-numeric stream).
- Device: "dag0:abc"
- Expectation: pcap_t* == NULL, is_ours == 0.
*/
static void test_dag_create_invalid_stream_syntax() {
    char ebuf[PCAP_ERRBUF_SIZE];
    std::memset(ebuf, 0, sizeof(ebuf));

    int is_ours = 0;
    pcap_t *p = dag_create("dag0:abc", ebuf, &is_ours);

    bool ok = (p == nullptr) && (is_ours == 0);
    log_result("test_dag_create_invalid_stream_syntax", ok,
               (ok ? "" : "Expected NULL p and is_ours=0"));
    if (p != nullptr) pcap_close(p);
}

/*
Test 5: Extremely large devnum beyond INT_MAX range should fail but set is_ours to 1.
- Device: "dag2147483648" (2^31)
- Expectation: p == NULL, is_ours == 1 (due to syntax validation), ebuf contains error message.
*/
static void test_dag_create_large_devnum() {
    char ebuf[PCAP_ERRBUF_SIZE];
    std::memset(ebuf, 0, sizeof(ebuf));

    int is_ours = 0;
    pcap_t *p = dag_create("dag2147483648", ebuf, &is_ours);

    bool ok = (p == nullptr) && (is_ours == 1) && (std::strncmp(ebuf, "DAG device name", 16) == 0);
    log_result("test_dag_create_large_devnum", ok,
               (ok ? "" : "Expected NULL p, is_ours=1, error message set"));
    if (p != nullptr) pcap_close(p);
}

int main() {
    // Execute test suite
    test_dag_create_non_dag_device_returns_null_and_no_ours();
    test_dag_create_basic_dag_device();
    test_dag_create_dag_with_stream();
    test_dag_create_invalid_stream_syntax();
    test_dag_create_large_devnum();

    print_summary();
    return (tests_failed > 0) ? 1 : 0;
}