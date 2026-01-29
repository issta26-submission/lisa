// Unit test suite for the focal method dgram_socket in pcap-haiku.c
// This test suite does not use GTest. It provides a minimal in-process test harness
// with non-terminating assertions (EXPECT_* macros) and a simple main() entrypoint.
// The test suite relies on function overriding (link-time symbol override) to
// simulate both success and error paths of the underlying socket() system call.

#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <stdint.h>
#include <OS.h>
#include <net/if_dl.h>
#include <string>
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


// Forward declaration of the focal method under test.
// The actual implementation is in pcap-haiku.c.
extern "C" int dgram_socket(const int af, char *errbuf);

// -------------------------------------------------------------------------------------------------
// Test harness infrastructure
// -------------------------------------------------------------------------------------------------

// Global flags to control the mocked system call behavior.
// 0 => simulate success; any non-zero => simulate failure
extern "C" int g_mock_socket_behavior = 0;
// Optional errno value to set when simulating failure
extern "C" int g_mock_errno = 0;

// Mocked system call override for 'socket'.
// This override will be used by the focal method during the tests (via linker symbol resolution).
extern "C" int socket(int domain, int type, int protocol)
{
    (void)type;
    (void)protocol;
    // If g_mock_socket_behavior == 0, simulate a successful socket() call and return a fake fd.
    if (g_mock_socket_behavior == 0) {
        return 3; // arbitrary non-negative fd for testing
    } else {
        // Simulate failure: set errno and return -1
        errno = g_mock_errno;
        return -1;
    }
}

// Minimal non-terminating assertion utilities
static int g_failures = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    if((a) != (b)) { \
        std::cerr << "EXPECT_EQ failed: " << (a) << " != " << (b) << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_STR_CONTAINS(str, substr) do { \
    if((str).find(substr) == std::string::npos) { \
        std::cerr << "EXPECT_STR_CONTAINS failed: \"" << (str) << "\" does not contain \"" << (substr) \
                  << "\" at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

// -------------------------------------------------------------------------------------------------
// Test cases
// -------------------------------------------------------------------------------------------------

// Test 1: Success path of dgram_socket
// - Purpose: Ensure that when the underlying socket() call succeeds, dgram_socket returns a valid fd
//   and does not modify the error buffer.
void test_dgram_socket_success()
{
    // Arrange
    g_mock_socket_behavior = 0; // simulate success
    g_mock_errno = 0;
    char errbuf[4096];
    // Ensure the error buffer starts empty to validate "no error" behavior on success
    std::memset(errbuf, 0, sizeof(errbuf));

    // Act
    int ret = dgram_socket(AF_INET, errbuf);

    // Assert
    EXPECT_EQ(ret, 3);          // Our mock returns 3 for a successful socket
    EXPECT_TRUE(errbuf[0] == '\0'); // errbuf should be untouched on success

    // Extra: also validate success path with a second address family to exercise the branch
    ret = dgram_socket(AF_INET6, errbuf);
    EXPECT_EQ(ret, 3);
}

// Test 2: Failure path of dgram_socket
// - Purpose: Ensure that when the underlying socket() call fails, dgram_socket returns PCAP_ERROR
//   and that the error buffer contains the expected diagnostic mentioning "socket".
void test_dgram_socket_failure()
{
    // Arrange
    g_mock_socket_behavior = 1; // simulate failure
    g_mock_errno = EACCES;      // a plausible errno for failure
    char errbuf[4096];
    // Pre-fill errbuf to ensure it gets overwritten
    std::strcpy(errbuf, "initial");

    // Act
    int ret = dgram_socket(AF_INET, errbuf);

    // Assert
    // PCAP_ERROR is a negative sentinel; ensure a failure path is taken
    EXPECT_TRUE(ret < 0);

    // The error message should mention the operation ("socket")
    std::string errstr(errbuf);
    EXPECT_STR_CONTAINS(errstr, "socket");
}

// -------------------------------------------------------------------------------------------------
// Main harness
// -------------------------------------------------------------------------------------------------

int main()
{
    // Run tests
    test_dgram_socket_success();
    test_dgram_socket_failure();

    // Report summary
    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << "Total failures: " << g_failures << "\n";
        return 1;
    }
}

// Note:
// - The test suite uses a symbol override technique for the socket() system call to deterministically
//   exercise both branches (success and failure) of dgram_socket without depending on the actual
//   system's socket behavior.
// - To compile and run this test:
//     1) Ensure pcap-haiku.c is part of the link (and that dgram_socket is visible to the test).
//     2) Compile as C++11, providing the test file and linking against the C source, e.g.:
//          g++ -std=c++11 -c test_dgram_socket.cpp -o test_dgram_socket.o
//          gcc -c pcap-haiku.c -o pcap-haiku.o
//          g++ test_dgram_socket.o pcap-haiku.o -o test_dgram_socket_run
//     3) Run: ./test_dgram_socket_run
// - This test relies on the ability to override the symbol 'socket'. If the build system does not
//   allow symbol overriding, an alternative approach would be to inject a real UDP socket by using
//   AF_INET and checking for a non-negative file descriptor, while separately testing the error path
//   via a controlled environment (not shown here).