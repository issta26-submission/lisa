/*
Unit test suite for the focal method bt_monitor_create located in pcap-bt-monitor-linux.c
- No GoogleTest; a lightweight, self-contained test harness is used.
- The tests target the two branches of the function:
  1) Device mismatch -> returns NULL and is_ours set to 0.
  2) Device match -> returns non-NULL, is_ours set to 1.
- The tests rely on INTERFACE_NAME macro being defined by the project headers.
  If not defined, a safe default is provided for the tests to compile.
- Notes:
  - We verify only high-level behavior (return value and is_ours flag) to avoid coupling to private internals.
  - We use a minimal, non-terminating assertion mechanism to maximize coverage.
*/

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <pcap-bt-monitor-linux.h>
#include <cstdlib>
#include <pcap/bluetooth.h>
#include <stdint.h>
#include <cstdio>
#include <stdlib.h>
#include <config.h>
#include <bluetooth/hci.h>
#include <bluetooth/bluetooth.h>
#include <pcap-int.h>
#include <cstring>
#include <string>
#include <iostream>


// Provide a default INTERFACE_NAME if the project headers do not define it.
// In the real project, INTERFACE_NAME should come from the library headers.
#ifndef INTERFACE_NAME
#define INTERFACE_NAME "btmon0"
#endif

// Forward declaration of pcap_t to avoid requiring the real library headers in the test harness.
// The actual definition is provided by the library; here we only need the type for the prototype.
struct pcap_t;
typedef struct pcap_t pcap_t;

// Prototype of the focal function under test.
// The real project provides this; we declare it here to allow linking with the actual implementation.
extern "C" pcap_t *bt_monitor_create(const char *device, char *ebuf, int *is_ours);

// Simple non-terminating test harness
static int g_total = 0;
static int g_failed = 0;

// Expectation macros that do not abort on failure, allowing multiple tests to run.
#define EXPECT_TRUE(cond) do { \
    ++g_total; \
    if(!(cond)) { \
        ++g_failed; \
        fprintf(stderr, "Expectation failed: %s:%d: %s is false\n", __FILE__, __LINE__, #cond); \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    ++g_total; \
    if(!((a) == (b))) { \
        ++g_failed; \
        fprintf(stderr, "Expectation failed: %s:%d: %s == %s (got %ld vs %ld)\n", __FILE__, __LINE__, #a, #b, (long)(a), (long)(b)); \
    } \
} while(0)

#define EXPECT_NE(a, b) do { \
    ++g_total; \
    if((a) == (b)) { \
        ++g_failed; \
        fprintf(stderr, "Expectation failed: %s:%d: %s != %s (both are %ld)\n", __FILE__, __LINE__, #a, #b, (long)(a)); \
    } \
} while(0)

// Test 1: Device does not match the interface name
// Expectation: is_ours should be set to 0 and the function should return NULL.
void test_bt_monitor_create_device_mismatch() {
    // Arrange
    char ebuf[256] = {0};
    int is_ours = -1;
    std::string device = std::string("NOT_") + INTERFACE_NAME; // deliberately different from INTERFACE_NAME

    // Act
    pcap_t *handle = bt_monitor_create(device.c_str(), ebuf, &is_ours);

    // Assert
    EXPECT_TRUE(handle == nullptr);
    EXPECT_EQ(is_ours, 0);
}

// Test 2: Device matches the interface name
// Expectation: is_ours should be set to 1 and the function should return a non-NULL handle.
// Note: We do not dereference internal fields of pcap_t to keep tests robust against private internals.
void test_bt_monitor_create_device_match() {
    // Arrange
    char ebuf[256] = {0};
    int is_ours = -1;
    std::string device = INTERFACE_NAME; // use the macro value if defined as a string literal

    // Act
    pcap_t *handle = bt_monitor_create(device.c_str(), ebuf, &is_ours);

    // Assert
    EXPECT_TRUE(handle != nullptr);
    EXPECT_EQ(is_ours, 1);
}

int main() {
    // Run tests
    test_bt_monitor_create_device_mismatch();
    test_bt_monitor_create_device_match();

    // Summary
    std::cout << "\nTest summary: " << g_total - g_failed << " passed, "
              << g_failed << " failed, out of " << g_total << " tests." << std::endl;

    if (g_failed != 0) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}