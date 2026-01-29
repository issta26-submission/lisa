/*
   Test suite for the focal method: pcap_lib_version (located in pcap-libdlpi.c)

   Step 1 (Program Understanding):
   - The function pcap_lib_version(void) simply returns PCAP_VERSION_STRING.
   - There are no conditional branches inside the function; it returns a string literal (or macro expansion).
   - Dependencies are the project headers/libraries providing PCAP_VERSION_STRING and the implementation of pcap_lib_version.
   - Candidate Keywords (core components to consider while testing): pcap_lib_version, PCAP_VERSION_STRING, string literal/constant, deterministic return value.

   Step 2 (Unit Test Generation):
   - The tests here focus on verifying that:
     - The function returns a non-null, non-empty C-string.
     - The return value is stable across multiple invocations (pointer identity).
   - We avoid relying on any private state or private methods, as required.
   - We do not rely on GTest; instead, we implement a lightweight, non-terminating assertion framework.

   Step 3 (Test Case Refinement):
   - We provide robust checks using the C++ standard library without external test frameworks.
   - We ensure tests are executable with C++11 and use the correct namespace (global scope in this small harness).
   - We incorporate explanatory comments for each test case.

   Notes:
   - The tests declare the function with C linkage to call the C implementation:
       extern "C" const char* pcap_lib_version(void);
   - We refrain from depending on the actual content/value of PCAP_VERSION_STRING (to remain robust across builds); instead, we check for non-null, non-empty content and pointer stability across calls.
 */

#include <string.h>
#include <errno.h>
#include <dlpisubs.h>
#include <cctype>
#include <libdlpi.h>
#include <memory.h>
#include <sys/time.h>
#include <stdio.h>
#include <sys/bufmod.h>
#include <stropts.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <config.h>
#include <sys/types.h>
#include <cstring>
#include <sys/stream.h>
#include <iostream>


// Focal function under test: provided by the library (C code).
extern "C" const char* pcap_lib_version(void);

// Lightweight test harness (non-terminating assertions).
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE FAILED: " << (msg) << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ_PTR(a,b,msg) do { \
    if((a) != (b)) { \
        std::cerr << "EXPECT_EQ_PTR FAILED: " << (msg) \
                  << " got pointer " << static_cast<const void*>((a)) \
                  << " expected " << static_cast<const void*>((b)) << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_STREQ(a,b,msg) do { \
    if(((a)==nullptr) || ((b)==nullptr) || (std::strcmp((a),(b)) != 0)) { \
        std::cerr << "EXPECT_STREQ FAILED: " << (msg) \
                  << " got \"" << (a) << "\" expected \"" << (b) << "\"\n"; \
        ++g_failures; \
    } \
} while(0)

/*
  Test 1: Basic validity of pcap_lib_version
  - Ensure the function returns a non-null pointer.
  - Ensure the returned string is non-empty.
  - Ensure all characters in the string are printable (sanity check on content).
*/
void test_pcap_lib_version_basic() {
    // Call 1: retrieve version string
    const char* v1 = pcap_lib_version();
    EXPECT_TRUE(v1 != nullptr, "pcap_lib_version should not return null.");
    size_t len = std::strlen(v1);
    EXPECT_TRUE(len > 0, "pcap_lib_version should return a non-empty string.");

    // Check that the string contains printable characters only (basic sanity).
    for (size_t i = 0; i < len; ++i) {
        EXPECT_TRUE(std::isprint(static_cast<unsigned char>(v1[i])) != 0,
                    "pcap_lib_version should contain printable characters only.");
    }

    // Call 2: ensure deterministic/consistent pointer value across calls.
    const char* v2 = pcap_lib_version();
    EXPECT_EQ_PTR(v1, v2, "pcap_lib_version should return a stable pointer across calls.");
}

/*
  Main entry point for the test suite.
  - Executes the test cases.
  - Reports overall success or failure without terminating tests early.
*/
int main() {
    test_pcap_lib_version_basic();

    if (g_failures > 0) {
        std::cerr << "Tests finished with failures: " << g_failures << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}