// C++11 test suite for the focal function: dlstrerror
// This test suite is designed to compile without GoogleTest.
// It uses a lightweight, self-contained test harness and asserts via runtime checks
// to maximize code coverage and allow the test to continue after failures.
// The tests rely on the actual DLPI error codes (DL_*) defined by system headers.
// If HAVE_DEV_DLPI is defined at compile time, the DL_BADPPA message will be
// "Specified PPA was invalid"; otherwise it will be "Specified PPA (device unit) was invalid".
// The tests are written to adapt to either condition using preprocessor guards.

// NOTE: The implementation under test is expected to be linked in (pcap-dlpi.c or equivalent).
// The test declares the function prototype with C linkage to match the C implementation.

#include <sys/dlpi_ext.h>
#include <unistd.h>
#include <stdio.h>
#include <stropts.h>
#include <sys/types.h>
#include <errno.h>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <fcntl.h>
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


// Attempt to align with the project's DLPI-related headers.
// These headers commonly define DL_* macros used by the focal function.
// If they are unavailable in the test environment, compilation may fail.
// Include guards are provided by the test environment; adapt as needed.

// Define bpf_u_int32 to match expected type in the focal function.
// If the project uses a specific typedef, this alias should be compatible
// since both are intended to be a 32-bit unsigned integer.
typedef uint32_t bpf_u_int32;

// Declare the focal function with C linkage for linking against the C implementation.
// The function signature:
//   char *dlstrerror(char *errbuf, size_t errbufsize, bpf_u_int32 dl_errno)
extern "C" char *dlstrerror(char *errbuf, size_t errbufsize, bpf_u_int32 dl_errno);

// Lightweight test framework (non-terminating assertions)
static int g_total_tests = 0;
static int g_passed_tests = 0;

// Helper macro to perform a test assertion and continue on failure
#define TEST_ASSERT(cond, desc) do { \
    ++g_total_tests; \
    if (cond) { \
        ++g_passed_tests; \
    } else { \
        fprintf(stderr, "FAILED: %s\n", (desc)); \
    } \
} while(0)

// Helper function to run a known-case test
static void test_known_case(uint32_t code, const char* expected_desc) {
    char errbuf[128];
    // Call the focal function; for known codes, the function returns a string literal
    // and does not modify errbuf.
    const char* result = dlstrerror(errbuf, sizeof(errbuf), (bpf_u_int32)code);
    // The function is expected to return a const string literal with the exact message.
    TEST_ASSERT(result != nullptr, "dlstrerror returned non-null pointer for known code");
    if (result != nullptr) {
        TEST_ASSERT(std::strcmp(result, expected_desc) == 0,
                    "dlstrerror returned expected message for known code");
    }
}

// Helper function to test the default path (unknown error code)
static void test_default_path_unknown_code(uint32_t unknown_code) {
    char errbuf[64];
    const char* result = dlstrerror(errbuf, sizeof(errbuf), (bpf_u_int32)unknown_code);
    // Default branch should return errbuf itself and fill with "Error %02x"
    TEST_ASSERT(result == errbuf, "dlstrerror should return errbuf pointer in default case");
    if (result == errbuf) {
        // Expect "Error %02x" with the provided code in lowercase hex
        char expected[64];
        // Emulate the formatting used by the function to verify
        // Note: Ensure lowercase hex, width 2
        std::snprintf(expected, sizeof(expected), "Error %02x", unknown_code);
        TEST_ASSERT(std::strcmp(result, expected) == 0,
                    "dlstrerror default path should format hex code as 'Error %02x'");
    }
}

// Entry point for the test suite
int main() {
    // Unit Test 1: Validate mapping for all known DL_* constants to their string messages.
    // This test covers true branches of the switch statement for each explicit DL_* case.
    // If some DL_* constants are missing on a given platform, that particular branch will
    // be skipped at compile-time, but the rest will be exercised.
    {
        // The following list uses DL_* constants as defined by the system headers.
        // For DL_BADPPA, the expected message depends on the presence of HAVE_DEV_DLPI.
        const char* badppa_expected =
#ifdef HAVE_DEV_DLPI
            "Specified PPA was invalid"
#else
            "Specified PPA (device unit) was invalid"
#endif
            ;

        // Known mappings: code -> expected message
        // Note: We read DL_* constants from system headers. If a constant is missing,
        // the compilation will fail; adjust environment accordingly.
        test_known_case(DL_ACCESS, "Improper permissions for request");
        test_known_case(DL_BADADDR, "DLSAP addr in improper format or invalid");
        test_known_case(DL_BADCORR, "Seq number not from outstand DL_CONN_IND");
        test_known_case(DL_BADDATA, "User data exceeded provider limit");
        test_known_case(DL_BADPPA, badppa_expected);
        test_known_case(DL_BADPRIM, "Primitive received not known by provider");
        test_known_case(DL_BADQOSPARAM, "QOS parameters contained invalid values");
        test_known_case(DL_BADQOSTYPE, "QOS structure type is unknown/unsupported");
        test_known_case(DL_BADSAP, "Bad LSAP selector");
        test_known_case(DL_BADTOKEN, "Token used not an active stream");
        test_known_case(DL_BOUND, "Attempted second bind with dl_max_conind");
        test_known_case(DL_INITFAILED, "Physical link initialization failed");
        test_known_case(DL_NOADDR, "Provider couldn't allocate alternate address");
        test_known_case(DL_NOTINIT, "Physical link not initialized");
        test_known_case(DL_OUTSTATE, "Primitive issued in improper state");
        test_known_case(DL_SYSERR, "UNIX system error occurred");
        test_known_case(DL_UNSUPPORTED, "Requested service not supplied by provider");
        test_known_case(DL_UNDELIVERABLE, "Previous data unit could not be delivered");
        test_known_case(DL_NOTSUPPORTED, "Primitive is known but not supported");
        test_known_case(DL_TOOMANY, "Limit exceeded");
        test_known_case(DL_NOTENAB, "Promiscuous mode not enabled");
        test_known_case(DL_BUSY, "Other streams for PPA in post-attached");
        test_known_case(DL_NOAUTO, "Automatic handling XID&TEST not supported");
        test_known_case(DL_NOXIDAUTO, "Automatic handling of XID not supported");
        test_known_case(DL_NOTESTAUTO, "Automatic handling of TEST not supported");
        test_known_case(DL_XIDAUTO, "Automatic handling of XID response");
        test_known_case(DL_TESTAUTO, "Automatic handling of TEST response");
        test_known_case(DL_PENDING, "Pending outstanding connect indications");
    }

    // Unit Test 2: Validate default path formatting for an unknown or unhandled error code.
    // This ensures the function writes the hex representation to the provided buffer
    // and returns the same buffer pointer.
    {
        // Pick an arbitrary code that is not part of the DL_* switch cases.
        test_default_path_unknown_code(0xAB); // "Error ab" in lowercase hex
    }

    // Summary reporting
    printf("DLPI dlstrerror test summary: %d tests executed, %d passed.\n",
           g_total_tests, g_passed_tests);
    if (g_total_tests == g_passed_tests) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("SOME TESTS FAILED\n");
        return 1;
    }
}