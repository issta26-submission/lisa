/*
Unit Test Suite for pcap_inject_hurd (conceptual in this isolated environment)
- This test harness is self-contained and uses a minimal, mocked environment to exercise
  the core logic of pcap_inject_hurd without depending on the actual pcap/hurd runtime.
- It mirrors the original function behavior:
    kr = device_write(ph->mach_dev, D_NOWAIT, 0, (io_buf_ptr_t)buf, size, &count);
    if (kr) { format error in p->errbuf; return -1; }
    return count;
- Two main scenarios are covered:
  1) Success path: device_write returns 0 and count is set to size -> function returns size.
  2) Error path: device_write returns non-zero -> function returns -1 and errbuf is populated.
- The tests use non-terminating assertions: failures are collected and reported, but test execution continues.

Note: This is a standalone, simplified adaptation intended for conceptual validation in environments
where the full pcap-hurd.c is not available. It defines minimal structures and prototypes to mirror
the focal function's behavior.
*/

#include <string.h>
#include <errno.h>
#include <device/net_status.h>
#include <net/if_ether.h>
#include <time.h>
#include <iostream>
#include <stddef.h>
#include <stdio.h>
#include <hurd.h>
#include <device/device.h>
#include <cstdio>
#include <stdlib.h>
#include <device/device_types.h>
#include <config.h>
#include <pcap-int.h>
#include <mach.h>
#include <fcntl.h>
#include <cstring>


// Domain-model constants and types (simplified for isolated testing)
#define PCAP_ERRBUF_SIZE 256
#define D_NOWAIT 0

typedef int kern_return_t;
typedef void* io_buf_ptr_t;

// Minimal mock structures to emulate the real ones used by pcap_inject_hurd
struct pcap_hurd {
    void *mach_dev;
};

struct pcap_t {
    pcap_hurd *priv;
    char errbuf[PCAP_ERRBUF_SIZE];
};

// Forward declaration of the function under test (as it would appear in the real code)
int pcap_inject_hurd(pcap_t *p, const void *buf, int size);

// Global flag to control mock behavior of device_write
// 0 -> success, 1 -> failure
int test_device_write_behavior = 0;

// Minimal mock of the underlying device_write used by the focal function.
// This mock is deliberate: it provides deterministic behavior for testing.
kern_return_t device_write(void *mach_dev, int flags, int reserved,
                           io_buf_ptr_t buf, int size, int *count)
{
    (void)mach_dev; (void)flags; (void)reserved; (void)buf; // suppress unused warnings
    if (test_device_write_behavior == 0) {
        // Success path: pretend we wrote exactly 'size' bytes.
        *count = size;
        return 0;
    } else {
        // Failure path: simulate an error returned by the device write.
        return -1;
    }
}

// Simple helper to format error messages, mimicking the real helper.
// Writes a concise message into errbuf if kr indicates an error.
void pcapint_fmt_errmsg_for_kern_return_t(char *errbuf, size_t errbuf_size,
                                         kern_return_t kr, const char *src)
{
    if (errbuf == nullptr || errbuf_size == 0) return;
    if (kr) {
        // On error, store a recognizable message including the source label.
        std::snprintf(errbuf, errbuf_size, "%s: kernel error %d", src, (int)kr);
    } else {
        // On success, ensure the buffer is cleared.
        errbuf[0] = '\0';
    }
}

// Implementation of the focal function (mirroring the original logic)
int pcap_inject_hurd(pcap_t *p, const void *buf, int size)
{
    {
        struct pcap_hurd *ph;
        kern_return_t kr;
        int count;

        ph = p->priv;
        kr = device_write(ph->mach_dev, D_NOWAIT, 0,
                          (io_buf_ptr_t)buf, size, &count);
        if (kr) {
            pcapint_fmt_errmsg_for_kern_return_t(p->errbuf, PCAP_ERRBUF_SIZE, kr,
                "device_write");
            return -1;
        }
        return count;
    }
}

// Simple non-GTest test framework (non-terminating assertions)
static int g_failures = 0;
#define EXPECT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        ++g_failures; \
        std::cerr << "EXPECT_EQ FAILED: " << (msg) << " | expected: " << (b) << " actual: " << (a) << "\n"; \
    } \
} while (0)

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        ++g_failures; \
        std::cerr << "EXPECT_TRUE FAILED: " << (msg) << "\n"; \
    } \
} while (0)

#define TEST_START(name) {
#define TEST_END(name)  }

// Test 1: Inject should succeed when device_write reports success and count == size
void test_inject_success()
{
    // Arrange
    pcap_t p;
    p.priv = new pcap_hurd();
    p.priv->mach_dev = nullptr;
    // Ensure error buffer is initially empty
    std::memset(p.errbuf, 0, PCAP_ERRBUF_SIZE);

    const char test_buf[] = "TEST_PACKET";
    int size = (int)sizeof(test_buf);

    // Act
    test_device_write_behavior = 0; // simulate success
    int ret = pcap_inject_hurd(&p, test_buf, size);

    // Assert
    EXPECT_EQ(ret, size, "pcap_inject_hurd should return exact written size on success");
    EXPECT_TRUE(std::strlen(p.errbuf) == 0, "errbuf should be empty on success");
    delete p.priv;
}

// Test 2: Inject should return -1 and populate errbuf when device_write reports failure
void test_inject_error()
{
    // Arrange
    pcap_t p;
    p.priv = new pcap_hurd();
    p.priv->mach_dev = nullptr;
    std::memset(p.errbuf, 0, PCAP_ERRBUF_SIZE);

    const char test_buf[] = "TEST_PACKET";
    int size = (int)sizeof(test_buf);

    // Act
    test_device_write_behavior = 1; // simulate failure
    int ret = pcap_inject_hurd(&p, test_buf, size);

    // Assert
    EXPECT_EQ(ret, -1, "pcap_inject_hurd should return -1 on device_write failure");
    // errbuf should contain a message referencing device_write
    EXPECT_TRUE(std::strstr(p.errbuf, "device_write") != nullptr,
                "errbuf should contain 'device_write' after failure");
    delete p.priv;
}

// Test 3: Inject with zero size should reflect count == 0 and not crash
void test_inject_zero_size()
{
    // Arrange
    pcap_t p;
    p.priv = new pcap_hurd();
    p.priv->mach_dev = nullptr;
    std::memset(p.errbuf, 0, PCAP_ERRBUF_SIZE);

    const char test_buf[] = "IGNORED";
    int size = 0;

    // Act
    test_device_write_behavior = 0; // simulate success
    int ret = pcap_inject_hurd(&p, test_buf, size);

    // Assert
    EXPECT_EQ(ret, 0, "pcap_inject_hurd should return 0 when size is 0");
    EXPECT_TRUE(std::strlen(p.errbuf) == 0, "errbuf should be empty when size is 0 and success");
    delete p.priv;
}

int main()
{
    // Run tests
    TEST_START("pcap_inject_hurd unit tests")
        test_inject_success();
        test_inject_error();
        test_inject_zero_size();
    TEST_END("pcap_inject_hurd unit tests")

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed.\n";
        return 1;
    }
}