// Unit test suite for the focal method pcap_inject_haiku
// This test is designed to compile in a C++11 environment without GoogleTest.
// It provides a minimal test harness with non-terminating assertions and prints a summary.

#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <errno.h>
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


// Domain-specific constants and types (mocked minimal environment)
#define PCAP_ERRBUF_SIZE 256
#define PCAP_ERROR -1

// Minimal pcap_t structure used by the focal function
typedef struct pcap_t {
    char errbuf[PCAP_ERRBUF_SIZE];
} pcap_t;

// Forward declarations to resemble production environment
extern "C" size_t pcapint_strlcpy(char *dst, const char *src, size_t dstsize);

// The focal method under test (re-created here to enable isolated unit testing
// without relying on the full project build system).
extern "C" int pcap_inject_haiku(pcap_t *handle, const void *buffer, int size) {
    // Haiku currently (hrev57588) does not support sending raw packets.
    // https://dev.haiku-os.org/ticket/18810
    pcapint_strlcpy(handle->errbuf, "Sending packets isn't supported yet", PCAP_ERRBUF_SIZE);
    return PCAP_ERROR;
}

// Minimal implementation of pcapint_strlcpy (static-like helper function)
extern "C" size_t pcapint_strlcpy(char *dst, const char *src, size_t dstsize) {
    if (dstsize == 0) {
        // No space to copy; return the length of src as per strlcpy convention
        return std::strlen(src);
    }
    size_t srclen = std::strlen(src);
    size_t copy_len = (srclen < dstsize - 1) ? srclen : (dstsize - 1);
    if (dst != nullptr && copy_len > 0) {
        std::memcpy(dst, src, copy_len);
    }
    dst[copy_len] = '\0';
    return srclen;
}

// Simple test harness (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_TRUE(cond) \
    do { \
        ++g_total; \
        if(!(cond)) { \
            ++g_failed; \
            std::cerr << "[FAIL] " __FILE__ ":" << __LINE__ << " - expected true but got false: " #cond "\n"; \
        } \
    } while(0)

#define EXPECT_EQ(a, b) \
    do { \
        ++g_total; \
        if(!((a) == (b))) { \
            ++g_failed; \
            std::cerr << "[FAIL] " __FILE__ ":" << __LINE__ << " - expected " #a " == " #b " (" << (a) << " != " << (b) << ")\n"; \
        } \
    } while(0)

#define EXPECT_STREQ(a, b) \
    do { \
        ++g_total; \
        if(std::strcmp((a), (b)) != 0) { \
            ++g_failed; \
            std::cerr << "[FAIL] " __FILE__ ":" << __LINE__ << " - strings differ: \"" << (a) << "\" != \"" << (b) << "\"\n"; \
        } \
    } while(0)

// Test 1: Ensure pcap_inject_haiku returns PCAP_ERROR and sets the exact error message
void test_inject_haiku_sets_error_message() {
    // Arrange
    pcap_t handle;
    // Pre-fill with a known value to ensure the function overwrites it
    std::strcpy(handle.errbuf, "initial");

    // Act
    int result = pcap_inject_haiku(&handle, nullptr, 0);

    // Assert
    EXPECT_EQ(result, PCAP_ERROR);
    EXPECT_STREQ(handle.errbuf, "Sending packets isn't supported yet");
}

// Test 2: Validate that the error message length does not overflow the buffer
void test_error_message_within_buffer_limit() {
    // Arrange
    pcap_t handle;
    std::memset(handle.errbuf, 0, sizeof(handle.errbuf));

    // Act
    int result = pcap_inject_haiku(&handle, nullptr, 0);

    // Assert
    // Result should be error and the string must be properly null-terminated
    EXPECT_EQ(result, PCAP_ERROR);
    // The length should be strictly less than the buffer size
    EXPECT_TRUE(std::strlen(handle.errbuf) < PCAP_ERRBUF_SIZE);
    // And content should be exactly as expected
    EXPECT_STREQ(handle.errbuf, "Sending packets isn't supported yet");
}

// Test 3: Ensure that repeated invocations consistently produce the same results
void test_repeated_calls_are_consistent() {
    pcap_t handle1;
    pcap_t handle2;
    std::strcpy(handle1.errbuf, "old1");
    std::strcpy(handle2.errbuf, "old2");

    int r1 = pcap_inject_haiku(&handle1, nullptr, 0);
    int r2 = pcap_inject_haiku(&handle2, nullptr, 0);

    EXPECT_EQ(r1, PCAP_ERROR);
    EXPECT_EQ(r2, PCAP_ERROR);
    EXPECT_STREQ(handle1.errbuf, "Sending packets isn't supported yet");
    EXPECT_STREQ(handle2.errbuf, "Sending packets isn't supported yet");
}

// Helper to summarize results
void report_results() {
    std::cerr << "Tests run: " << g_total << ", Failures: " << g_failed << "\n";
}

// Main function: run all tests and exit with non-zero on failure
int main() {
    test_inject_haiku_sets_error_message();
    test_error_message_within_buffer_limit();
    test_repeated_calls_are_consistent();

    report_results();
    return (g_failed == 0) ? 0 : 1;
}