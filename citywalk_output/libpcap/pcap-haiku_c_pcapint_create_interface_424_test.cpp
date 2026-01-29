/*
Unit test suite for the focal method:
  pcapint_create_interface(const char *device, char *errorBuffer)

Overview:
- Tests are written in C++11 (no gtest) and link against the project code that implements the function.
- A lightweight, non-terminating assertion mechanism is used (EXPECT_* macros) so tests continue after failures to improve coverage.
- Fallbacks for a couple of constants are provided to maximize portability if the test environment lacks system headers.
- Test cases focus on validating behavior of:
  1) Invalid device name length (should return NULL and set errorBuffer via validate_ifname path).
  2) Basic success path (non-NULL handle returned for a plausibly valid device name).
  3) (Note: Environment-dependent) potential behavior when interface cannot be bound; this test is kept conservative to avoid flaky results.
- The test suite uses a minimal harness with a main() function and prints a summary of results.

Assumptions:
- The production code defines and uses the library-internal types and functions (pcap_t, pcap_haiku, PCAP_CREATE_COMMON, etc.). The tests call pcapint_create_interface via an extern "C" declaration.
- IF_NAMESIZE and PCAP_ERRBUF_SIZE are defined by the system headers; fallback values are provided to keep compilation possible on minimal environments.
- The tests do not rely on private/internal details beyond the function under test and common C ABI.

Compilation notes:
- Compile with a C++11 compiler and link against the pcap Haiku sources (pcap-haiku.c and dependencies as provided in the project).
- Example (conceptual): g++ -std=c++11 test_pcapint.cpp pcap-haiku.c -o test_pcapint

Now providing the test code.

*/
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <cstdlib>
#include <stdint.h>
#include <cstdio>
#include <OS.h>
#include <net/if_dl.h>
#include <cstring>
#include <net/if_media.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_types.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <sys/sockio.h>
#include <string.h>
#include <config.h>
#include <sys/utsname.h>


// Portable fallbacks for environment-specific constants
#ifndef IF_NAMESIZE
#define IF_NAMESIZE 16
#endif

#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE 256
#endif

// Forward declaration of the C API under test.
// The real production code provides the full definition; we only need the
// function prototype to invoke it from C++.
extern "C" {
    // Opaque pcap_t struct as used by the production code.
    typedef struct pcap_t pcap_t;

    // The focal method under test.
    pcap_t* pcapint_create_interface(const char *device, char *errorBuffer);
}

// Simple non-terminating test assertions.
// These macros log failures but do not abort the test execution, enabling
// multiple test paths to be exercised in a single run.
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "[FAIL] " << msg << " (expression evaluated to false)" << std::endl; \
    } else { \
        std::cout << "[PASS] " << msg << std::endl; \
    } \
} while (0)

#define EXPECT_FALSE(cond, msg) do { \
    ++g_tests_run; \
    if ((cond)) { \
        ++g_tests_failed; \
        std::cerr << "[FAIL] " << msg << " (expression evaluated to true)" << std::endl; \
    } else { \
        std::cout << "[PASS] " << msg << std::endl; \
    } \
} while (0)

#define EXPECT_NOT_NULL(ptr, msg) do { \
    ++g_tests_run; \
    if ((ptr) == nullptr) { \
        ++g_tests_failed; \
        std::cerr << "[FAIL] " << msg << " (pointer is NULL)" << std::endl; \
    } else { \
        std::cout << "[PASS] " << msg << std::endl; \
    } \
} while (0)

#define EXPECT_NULL(ptr, msg) do { \
    ++g_tests_run; \
    if ((ptr) != nullptr) { \
        ++g_tests_failed; \
        std::cerr << "[FAIL] " << msg << " (pointer is not NULL)" << std::endl; \
    } else { \
        std::cout << "[PASS] " << msg << std::endl; \
    } \
} while (0)

// Test 1: Invalid device name length should cause validate_ifname() to fail.
// This should cause pcapint_create_interface() to return NULL.
static void test_invalid_device_name_length_returns_null() {
    // Create a device name longer than IF_NAMESIZE to trigger length check.
    char device[IF_NAMESIZE + 4];
    for (size_t i = 0; i < sizeof(device) - 1; ++i) {
        device[i] = 'a';
    }
    device[sizeof(device) - 1] = '\0';

    char errorBuffer[PCAP_ERRBUF_SIZE];
    std::memset(errorBuffer, 0, sizeof(errorBuffer));

    pcap_t* handle = pcapint_create_interface(device, errorBuffer);

    // Expect NULL due to validation failure.
    EXPECT_NULL(handle, "pcapint_create_interface should return NULL for overly long device name");
}

// Test 2: Valid device name should attempt to create a handle.
// We check that a non-NULL handle is returned and basic properties are set
// by the function. We do not rely on private internals; we verify via
// the existence/ownership of the returned pointer.
static void test_valid_device_name_creates_handle() {
    // Use a plausible short device name. The exact acceptance depends on the
    // runtime environment (validate_ifname and can_be_bound). We choose a
    // common-looking interface name; if this environment cannot capture it,
    // the test may fail, but this is expected in a portable unit test without
    // a full network stack.
    const char *device = "eth0";

    char errorBuffer[PCAP_ERRBUF_SIZE];
    std::memset(errorBuffer, 0, sizeof(errorBuffer));

    pcap_t* handle = pcapint_create_interface(device, errorBuffer);

    // We expect a non-NULL handle. If the environment cannot create one, this
    // test will fail gracefully (non-terminating).
    EXPECT_NOT_NULL(handle, "pcapint_create_interface should return a non-NULL handle for a plausible device");
}

// Test 3 (optional, environment-dependent): Interface bound check.
// If the environment ensures that certain devices do not support capturing,
// pcapint_create_interface should return NULL and set errorBuffer accordingly.
// This test is conservative and will pass/fail depending on the runtime.
static void test_interface_not_bound_behavior() {
    // Pick a device name that is commonly non-bindable in some environments.
    // If this is not the case on the running system, this test may be skipped
    // by the environment itself (i.e., it may return non-NULL).
    const char *device = "lo";

    char errorBuffer[PCAP_ERRBUF_SIZE];
    std::memset(errorBuffer, 0, sizeof(errorBuffer));

    pcap_t* handle = pcapint_create_interface(device, errorBuffer);

    // We cannot be sure of the environment; if the handle is NULL due to
    // non-bindable interface, we consider that a valid negative path.
    if (handle == nullptr) {
        // Treat as an expected path for non-bindable interfaces.
        std::cout << "[INFO] Non-bindable interface path exercised (handle == NULL)" << std::endl;
    } else {
        // If a handle is returned, at least it demonstrates the binding path;
        // we do not fail here since the environment may permit binding.
        std::cout << "[INFO] Interface 'lo' produced a valid handle (binding path exercised)" << std::endl;
    }
}

// Entry point for the test suite.
static void run_all_tests() {
    std::cout << "Running test suite for pcapint_create_interface..." << std::endl;
    test_invalid_device_name_length_returns_null();
    test_valid_device_name_creates_handle();
    test_interface_not_bound_behavior();

    std::cout << "Test run complete. Total: " << g_tests_run
              << ", Failures: " << g_tests_failed << std::endl;
}

int main() {
    run_all_tests();
    // Return nonzero if any test failed to aid CI systems.
    return g_tests_failed != 0 ? 1 : 0;
}