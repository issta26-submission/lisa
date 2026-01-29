/*
Test suite for the focal method dlinforeq located in pcap-dlpi.c

Overview:
- We create a lightweight C++11 test harness (no GoogleTest) with non-terminating
  assertions to maximize coverage.
- We provide a mock implementation of the C function:
    int send_request(int fd, char *ptr, int len, char *what, char *ebuf)
  so that dlinforeq can be exercised without performing real I/O.
- The mock inspects the emitted request (dl_info_req_t) to verify that:
  a) it sets req.dl_primitive = DL_INFO_REQ
  b) the length passed to send_request equals sizeof(dl_info_req_t)
  c) the "info" description string is passed as the what parameter
  d) the ebuf pointer is forwarded as-is
- We rely on the presence of the Solaris/illumos-like headers (<sys/dlpi.h>) 
  for the type dl_info_req_t and constant DL_INFO_REQ. The test is written to be
  portable to environments where the original project headers are available.
- The test suite does not rely on GTest. It prints a summary at the end.

Notes:
- This test links against pcap-dlpi.c and uses a C linkage for interop with the C API.
- If the build system places dlinforeq in a separate translation unit, ensure
  the test is linked with pcap-dlpi.c so that the symbol dlinforeq is resolved.

Usage:
- Compile with: g++ -std=c++11 -o test_dlinforeq test_dlinforeq.cpp pcap-dlpi.c
- Run: ./test_dlinforeq

*/

#include <sys/dlpi_ext.h>
#include <unistd.h>
#include <stdio.h>
#include <stropts.h>
#include <sys/types.h>
#include <iostream>
#include <errno.h>
#include <cstdlib>
#include <string>
#include <fcntl.h>
#include <vector>
#include <sys/stat.h>
#include <sys/systeminfo.h>
#include <limits.h>
#include <sys/dlpi.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <dlpisubs.h>
#include <string.h>
#include <memory.h>
#include <sys/time.h>
#include <os-proto.h>
#include <sys/bufmod.h>
#include <config.h>
#include <cstring>
#include <sys/stream.h>


// Include the dlpi headers to obtain the types/constants used by dlinforeq
// This is platform dependent; the original project uses <sys/dlpi.h> and related
// headers. If your environment requires different path names, adjust accordingly.

// Forward declare the focal function from the C file under test.
extern "C" int dlinforeq(int fd, char *ebuf);

// The mock state to capture invocations of send_request called by dlinforeq
static int mock_send_return = 0;
static int last_fd = -1;
static const dl_info_req_t* last_req_ptr = nullptr;
static int last_len = 0;
static const char* last_what = nullptr;
static char* last_ebuf = nullptr;

// Provide a C linkage mock for send_request so dlinforeq can call into it.
// This mock does not perform any I/O; it simply records the inputs for assertions.
extern "C" int send_request(int fd, char *ptr, int len, char *what, char *ebuf)
{
    last_fd = fd;
    last_req_ptr = (const dl_info_req_t*)ptr;
    last_len = len;
    last_what = what;
    last_ebuf = ebuf;
    return mock_send_return;
}

// Lightweight test harness (non-terminating assertions)
static int test_count = 0;
static int fail_count = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++test_count; \
    if(!(cond)) { \
        ++fail_count; \
        std::cerr << "Test failed: " << msg << " (condition " #cond " evaluated to false)" << std::endl; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    ++test_count; \
    if(!((a) == (b))) { \
        ++fail_count; \
        std::cerr << "Test failed: " << msg << " - expected " << (b) << ", got " << (a) << std::endl; \
    } \
} while(0)

#define EXPECT_STR_EQ(a, b, msg) do { \
    ++test_count; \
    if(!((a) && (b) && (strcmp((a), (b)) == 0))) { \
        ++fail_count; \
        std::cerr << "Test failed: " << msg << " - expected \"" << (b) << "\", got \"" \
                  << ((a) ? a : "NULL") << "\"" << std::endl; \
    } \
} while(0)

#define EXPECT_STR_NEQ(a, b, msg) do { \
    ++test_count; \
    if((a) && (b) && (strcmp((a), (b)) == 0)) { \
        ++fail_count; \
        std::cerr << "Test failed: " << msg << " - strings should differ but both are \"" << (a) << "\"" << std::endl; \
    } \
} while(0)

// Helper to reset mock state between tests
static void reset_mock_state()
{
    mock_send_return = 0;
    last_fd = -1;
    last_req_ptr = nullptr;
    last_len = 0;
    last_what = nullptr;
    last_ebuf = nullptr;
}

// Test 1: Validate that dlinforeq creates a proper info request and forwards to send_request.
// - Verifies that dl_primitive is set to DL_INFO_REQ.
// - Verifies the length matches sizeof(dl_info_req_t).
// - Verifies that the "what" string passed is "info".
// - Verifies that the ebuf parameter is forwarded.
static void test_dlinforeq_constructs_info_and_forwards()
{
    reset_mock_state();
    mock_send_return = 12345; // sentinel value to verify return
    const char ebuf[] = "error-buffer";

    int ret = dlinforeq(42, (char*)ebuf);

    // Assertions
    EXPECT_EQ(ret, 12345, "dlinforeq should return the value provided by send_request");
    EXPECT_EQ(last_fd, 42, "send_request fd should equal the input fd");
    EXPECT_TRUE(last_req_ptr != nullptr, "send_request should receive a non-null request pointer");
    if (last_req_ptr != nullptr) {
        EXPECT_EQ(last_req_ptr->dl_primitive, DL_INFO_REQ, "dl_info_req_t.dl_primitive should be DL_INFO_REQ");
    }
    EXPECT_TRUE(last_what != nullptr, "send_request should receive a non-null what string");
    if (last_what != nullptr) {
        EXPECT_TRUE(std::strcmp(last_what, "info") == 0, "what parameter should be 'info'");
    }
    EXPECT_EQ(last_len, sizeof(dl_info_req_t), "send_request length should be sizeof(dl_info_req_t)");
    EXPECT_TRUE(last_ebuf != nullptr, "send_request should receive a non-null ebuf");
    if (last_ebuf != nullptr) {
        EXPECT_TRUE(std::strcmp(last_ebuf, "error-buffer") == 0, "ebuf contents should be forwarded unchanged");
    }
}

// Test 2: Ensure that dlinforeq forwards null ebuf correctly.
// - Verifies that passing NULL as ebuf is possible and propagated.
static void test_dlinforeq_null_ebuf_handling()
{
    reset_mock_state();
    mock_send_return = 777; // sentinel

    int ret = dlinforeq(7, nullptr);

    EXPECT_EQ(ret, 777, "dlinforeq should return the value provided by send_request when ebuf is NULL");
    EXPECT_TRUE(last_ebuf == nullptr, "ebuf parameter should be forwarded as NULL when NULL is passed");
}

// Test 3: Multiple invocations should update the forwarded values accordingly.
static void test_dlinforeq_multiple_calls_update_state()
{
    reset_mock_state();
    mock_send_return = 555; // first call

    int ret1 = dlinforeq(9, (char*)"eb1");
    EXPECT_EQ(ret1, 555, "first call should return mock value");
    EXPECT_EQ(last_fd, 9, "first call fd should be 9");

    // Second call with different parameters
    mock_send_return = 888;
    int ret2 = dlinforeq(13, (char*)"eb2");
    EXPECT_EQ(ret2, 888, "second call should return updated mock value");
    EXPECT_EQ(last_fd, 13, "second call fd should be 13");
    if (last_what) {
        EXPECT_STR_EQ(last_what, "info", "second call should still pass 'info' as what");
    } else {
        EXPECT_TRUE(false, "second call produced a NULL what string");
    }
}

// Entry point
int main()
{
    std::cout << "Starting dlinforeq unit tests (non-GTest) ..." << std::endl;

    test_dlinforeq_constructs_info_and_forwards();
    test_dlinforeq_null_ebuf_handling();
    test_dlinforeq_multiple_calls_update_state();

    if (fail_count == 0) {
        std::cout << "ALL TESTS PASSED (" << test_count << " tests)" << std::endl;
        return 0;
    } else {
        std::cout << fail_count << " TEST(S) FAILED OUT OF " << test_count << std::endl;
        return 1;
    }
}