// File: test_pcap_lib_version_no_addinfo.cpp
// Purpose: Test pcap_lib_version() when ADDITIONAL_INFO_STRING is NOT defined.
// This validates the "true" path where only PCAP_VERSION_STRING is returned.
// The test uses a lightweight harness (no GTest) with non-terminating assertions.

#include <cf.h>
#include <net/bpf.h>
#include <netdb.h>
#include <sys/sysconfig.h>
#include <unistd.h>
#include <stdio.h>
#include <zone.h>
#include <sys/types.h>
#include <iostream>
#include <sys/ioctl.h>
#include <errno.h>
#include <time.h>
#include <stddef.h>
#include <dirent.h>
#include <sys/param.h>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/cfgodm.h>
#include <net/if_types.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <string.h>
#include <sys/ioccom.h>
#include <sys/device.h>
#include <os-proto.h>
#include <config.h>
#include <sys/utsname.h>


extern "C" const char* pcap_lib_version(void);

// Simple test harness to accumulate failures without exiting on first failure
static int g_failures_no_addinfo = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST_FAIL: " << (msg) << "\n"; \
        ++g_failures_no_addinfo; \
    } \
} while(0)
#define EXPECT_NOT_NULL(ptr, msg) EXPECT_TRUE((ptr) != nullptr, (msg))

// Step 2 / Step 3 alignment notes (from domain knowledge):
// - This test targets the true-branch of pcap_lib_version: returns PCAP_VERSION_STRING.
// - STATIC and file-scope constructs inside pcap-bpf.c are not directly accessible here.
// - We verify runtime behavior (non-null, non-empty) to ensure the function executes the branch without crash.
// - No private methods/fields are accessed; this test relies solely on the public C function.
// - The test is written in C++11, but calls a C function; uses only standard library.

static void test_pcap_lib_version_no_addinfo_returns_non_empty()
{
    const char* ver = pcap_lib_version();
    EXPECT_NOT_NULL(ver, "pcap_lib_version() returned NULL");
    if (ver != nullptr) {
        size_t len = std::strlen(ver);
        EXPECT_TRUE(len > 0, "pcap_lib_version() returned an empty string");
    }
}

int main()
{
    test_pcap_lib_version_no_addinfo_returns_non_empty();

    if (g_failures_no_addinfo == 0) {
        std::cout << "TEST_PASS: test_pcap_lib_version_no_addinfo_returns_non_empty\n";
    } else {
        std::cout << "TEST_FAIL: test_pcap_lib_version_no_addinfo_returns_non_empty - "
                  << g_failures_no_addinfo << " failure(s)\n";
    }
    return g_failures_no_addinfo;
}
////////////////////////////////////////////////////////////

// File: test_pcap_lib_version_with_addinfo.cpp
// Purpose: Test pcap_lib_version() when ADDITIONAL_INFO_STRING is defined.
// This validates the "with additional info" path where PCAP_VERSION_STRING_WITH_ADDITIONAL_INFO(ADDITIONAL_INFO_STRING) is used.
// The test uses a lightweight harness (no GTest) with non-terminating assertions.


extern "C" const char* pcap_lib_version(void);

#ifdef ADDITIONAL_INFO_STRING
// Capture the expected additional info string (macro provided at compile-time)
const char* g_expected_addinfo = ADDITIONAL_INFO_STRING;
#else
#error "This test must be compiled with -DADDITIONAL_INFO_STRING defined."
#endif

// Simple test harness to accumulate failures without exiting on first failure
static int g_failures_with_addinfo = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST_FAIL: " << (msg) << "\n"; \
        ++g_failures_with_addinfo; \
    } \
} while(0)
#define EXPECT_NOT_NULL(ptr, msg) EXPECT_TRUE((ptr) != nullptr, (msg))

// Step 2 / Step 3 alignment notes (from domain knowledge):
// - This test targets the true-branch of pcap_lib_version: returns PCAP_VERSION_STRING_WITH_ADDITIONAL_INFO(ADDITIONAL_INFO_STRING).
// - We verify that the runtime string contains the provided ADDITIONAL_INFO_STRING.
// - We do not access private/internal static members; only public C function is exercised.

static void test_pcap_lib_version_with_addinfo_contains_addinfo()
{
    const char* ver = pcap_lib_version();
    EXPECT_NOT_NULL(ver, "pcap_lib_version() returned NULL");
    if (ver != nullptr) {
        std::string ver_str(ver);

        // Ensure the returned version string contains the additional info provided at compile-time
        std::string expected_addinfo = g_expected_addinfo;
        bool found = ver_str.find(expected_addinfo) != std::string::npos;
        EXPECT_TRUE(found, "pcap_lib_version() does not contain the ADDITIONAL_INFO_STRING as expected");
    }
}

int main()
{
    test_pcap_lib_version_with_addinfo_contains_addinfo();

    if (g_failures_with_addinfo == 0) {
        std::cout << "TEST_PASS: test_pcap_lib_version_with_addinfo_contains_addinfo\n";
    } else {
        std::cout << "TEST_FAIL: test_pcap_lib_version_with_addinfo_contains_addinfo - "
                  << g_failures_with_addinfo << " failure(s)\n";
    }
    return g_failures_with_addinfo;
}