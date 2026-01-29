// C++11 test harness for the focal function: handle_nonexistent_dlpi_device
// This test suite targets the function located in dlpisubs.c.
// It uses plain C-style linkage for the focal function and provides small stubs
// for minimal dependencies to keep the unit-test self-contained.
// The tests are designed to be executable without GTest, and test methods are
// invoked from main() as required by the <DOMAIN_KNOWLEDGE> guidelines.

#include <libdlpi.h>
#include <unistd.h>
#include <stdio.h>
#include <zone.h>
#include <stropts.h>
#include <sys/types.h>
#include <iostream>
#include <sys/ioctl.h>
#include <errno.h>
#include <cassert>
#include <pcap.h>
#include <cstdio>
#include <string>
#include <sys/socket.h>
#include <net/if.h>
#include <cstdarg>
#include <sys/dlpi.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <sys/sockio.h>
#include <dlpisubs.h>
#include <string.h>
#include <memory.h>
#include <sys/time.h>
#include <sys/bufmod.h>
#include <config.h>
#include <cstring>
#include <sys/stream.h>


// Include the project's public PCAP headers to obtain correct macros.
// If the environment provides the standard pcap.h, this will pull the required
// PCAP_ERROR, PCAP_ERRBUF_SIZE, etc. values used by the focal function.

// Forward declaration of the focal function (C linkage)
extern "C" int handle_nonexistent_dlpi_device(const char *ifname, char *errbuf);

// The focal function relies on a few helper functions/macros from the libpcap
// codebase. To allow linking in a standalone test executable, provide lightweight
// C-style stubs that mimic the minimal required behavior. These stubs are
// intentionally simple and sufficient for the unit-test scenarios chosen here.

extern "C" {

// Minimal strlcpy implementation used by the focal code when compiling in environments
// that may not provide strlcpy in libc. Returns the length of src (as strlcpy does).
size_t strlcpy(char *dst, const char *src, size_t dstsize) {
    if (dst == nullptr || dstsize == 0) return strlen(src);
    size_t srclen = strlen(src);
    size_t copylen = (srclen >= dstsize) ? dstsize - 1 : srclen;
    // Safe copy with null-termination
    if (dstsize > 0) {
        memcpy(dst, src, copylen);
        dst[copylen] = '\0';
    }
    return srclen;
}

// Wrapper used by the focal code to copy strings into OS structures
void pcapint_strlcpy(char *dst, const char *src, size_t dstsize) {
    // Reuse our local strlcpy to keep behavior consistent in tests
    strlcpy(dst, src, dstsize);
}

// Lightweight formatter for errno-based messages used by the focal code.
// This is a minimal, safe implementation to satisfy linking in tests.
int pcapint_fmt_errmsg_for_errno(char* errbuf, size_t errbuf_size, int errno_value, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(errbuf, errbuf_size, fmt, ap);
    va_end(ap);
    // Do not attempt to emulate the full pcap error formatting; this suffices for tests
    return n;
}
} // extern "C"

//////////////////////////////////////////////////////////////////
// Test harness
//////////////////////////////////////////////////////////////////

// Simple non-terminating checks to maximize test coverage (domain knowledge)
static bool all_tests_passed = true;

// Test 1: Overly long interface name should short-circuit and report NO_SUCH_DEVICE
// This test avoids any actual socket/ioctl interactions by exercising the
// initial length-check branch of handle_nonexistent_dlpi_device.
void test_handle_nonexistent_dlpi_device_long_name() {
    // Create a very long interface name to trigger the initial bound check.
    std::string longname(1000, 'a');
    char errbuf[PCAP_ERRBUF_SIZE];
    int ret = handle_nonexistent_dlpi_device(longname.c_str(), errbuf);

    // Expect a NO_SUCH_DEVICE return and an empty error buffer (as per spec for this path)
    if (ret != PCAP_ERROR_NO_SUCH_DEVICE) {
        std::cerr << "[TEST] handle_nonexistent_dlpi_device with long name: expected "
                  << "PCAP_ERROR_NO_SUCH_DEVICE (" << PCAP_ERROR_NO_SUCH_DEVICE
                  << "), got " << ret << std::endl;
        all_tests_passed = false;
    }
    if (errbuf[0] != '\0') {
        std::cerr << "[TEST] handle_nonexistent_dlpi_device with long name: "
                  << "expected empty errbuf, got: '" << errbuf << "'" << std::endl;
        all_tests_passed = false;
    }
}

// Test 2: Existing interface name but no DLPI device available
// We choose a common interface name ("lo") that typically exists on Linux.
// The code path should proceed past the socket/ioctl checks and return
// PCAP_ERROR_CAPTURE_NOTSUP with the specific message about no DLPI device found.
void test_handle_nonexistent_dlpi_device_lo_interface() {
    char errbuf[PCAP_ERRBUF_SIZE];
    int ret = handle_nonexistent_dlpi_device("lo", errbuf);

    if (ret != PCAP_ERROR_CAPTURE_NOTSUP) {
        std::cerr << "[TEST] handle_nonexistent_dlpi_device with 'lo': expected "
                  << "PCAP_ERROR_CAPTURE_NOTSUP (" << PCAP_ERROR_CAPTURE_NOTSUP
                  << "), got " << ret << std::endl;
        all_tests_passed = false;
    }

    const char *expected = "Capturing on that interface is not supported - no DLPI device found";
    if (std::strncmp(errbuf, expected, PCAP_ERRBUF_SIZE) != 0) {
        std::cerr << "[TEST] handle_nonexistent_dlpi_device with 'lo': "
                  << "expected errbuf to start with: \"" << expected
                  << "\", got: \"" << errbuf << "\"\n";
        all_tests_passed = false;
    }
}

int main() {
    // Run tests in a single process (as required by the project constraints)
    test_handle_nonexistent_dlpi_device_long_name();
    test_handle_nonexistent_dlpi_device_lo_interface();

    if (all_tests_passed) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}