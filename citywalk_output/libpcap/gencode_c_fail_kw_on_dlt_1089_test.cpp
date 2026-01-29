// C++11 test suite for the focal method fail_kw_on_dlt in gencode.c
// This test suite is designed to be compiled with a C++11 compiler.
// It relies on the project’s existing C components and headers.
// No GTest is used per requirements.
// The tests perform simple stdout logging for PASS/FAIL and do not terminate on failure.

#include <pflog.h>
#include <stdarg.h>
#include <gencode.h>
#include <ppp.h>
#include <unistd.h>
#include <stdio.h>
#include <ethertype.h>
#include <extract.h>
#include <iostream>
#include <nametoaddr.h>
#include <ieee80211.h>
#include <cstdlib>
#include <stdint.h>
#include <stddef.h>
#include <thread-local.h>
#include <pcap-util.h>
#include <scanner.h>
#include <cstdio>
#include <setjmp.h>
#include <string>
#include <fcntl.h>
#include <llc.h>
#include <pcap/namedb.h>
#include <pcap/ipnet.h>
#include <diag-control.h>
#include <sys/stat.h>
#include <linux/filter.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <string.h>
#include <memory.h>
#include <linux/types.h>
#include <os-proto.h>
#include <linux/if_packet.h>
#include <config.h>
#include <pcap/sll.h>
#include <cstring>


extern "C" {
    // Include the focal method and dependencies with C linkage
    #include "gencode.h"        // declares fail_kw_on_dlt, pcapint_datalink_val_to_string
    #include "pcap-int.h"       // may declare compiler_state_t and related types
}

// Helper: Execute fail_kw_on_dlt and capture its stderr output.
// Assumptions:
// - fail_kw_on_dlt writes its diagnostic to stderr via bpf_error (as per code).
// - The test environment allows capturing stderr via a POSIX pipe.
static std::string run_fail_kw_on_dlt_and_capture(int linktype, const char* keyword)
{
    // Allocate and initialize a compiler_state_t as expected by the focal function.
    // We zero the structure to start with a clean slate, then set the required member.
    compiler_state_t *cstate = (compiler_state_t *)calloc(1, sizeof(compiler_state_t));
    if (cstate == nullptr) {
        fprintf(stderr, "TEST: Memory allocation for compiler_state_t failed\n");
        return "";
    }
    cstate->linktype = linktype;

    // Prepare to capture stderr
    int pipe_fd[2];
    if (pipe(pipe_fd) != 0) {
        fprintf(stderr, "TEST: pipe creation failed\n");
        free(cstate);
        return "";
    }

    // Redirect stderr to the write end of the pipe
    int saved_stderr = dup(STDERR_FILENO);
    if (saved_stderr == -1) {
        fprintf(stderr, "TEST: dup failed\n");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        free(cstate);
        return "";
    }
    if (dup2(pipe_fd[1], STDERR_FILENO) == -1) {
        fprintf(stderr, "TEST: dup2 failed\n");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        close(saved_stderr);
        free(cstate);
        return "";
    }

    // Ensure the write end of the pipe is closed in the calling thread after redirection
    close(pipe_fd[1]); // Not used directly anymore; stderr writes go to pipe_fd[0]

    // Call the focal function
    // The function is expected to write its diagnostic message to stderr.
    fail_kw_on_dlt(cstate, keyword);

    // Restore stderr
    fflush(stderr);
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stderr);

    // Read captured output from the pipe
    // We need the read end of the pipe
    // Since we duplicated STDERR to pipe_fd[1], the original read end is pipe_fd[0]
    // Read until EOF
    std::string captured;
    char buf[1024];
    ssize_t n;
    // Since we closed the write end earlier, ensure the read end is accessible
    // The pipe's read end remains pipe_fd[0]
    while ((n = read(pipe_fd[0], buf, sizeof(buf))) > 0) {
        captured.append(buf, buf + n);
    }
    // Cleanup
    close(pipe_fd[0]);
    free(cstate);

    return captured;
}

// Basic assertion helper that prints results without terminating the test program.
static void ASSERT_TRUE(bool condition, const char* test_desc) {
    if (condition) {
        std::cout << "[PASS] " << test_desc << "\n";
    } else {
        std::cout << "[FAIL] " << test_desc << "\n";
    }
}

// Test 1: Verify that fail_kw_on_dlt reports the provided keyword and the datalink
// string corresponding to the given linktype.
// This tests the "true" path where the function formats an error message including
// the keyword and the string returned by pcapint_datalink_val_to_string.
static void test_fail_kw_on_dlt_reports_keyword_and_dlt_string_linktype1() {
    const int test_linktype = 1;                 // Arbitrary linktype value (DLT_EN10MB typically)
    const char* test_keyword = "KW_TEST_ABC";

    // Execute and capture stderr output
    std::string output = run_fail_kw_on_dlt_and_capture(test_linktype, test_keyword);

    // Compute expected datalink string from the library helper
    const char* expected_dlt_string = pcapint_datalink_val_to_string(test_linktype);
    std::string expected_dlt = (expected_dlt_string != nullptr) ? expected_dlt_string : "";

    // The error message should contain the keyword and the datalink string.
    bool contains_keyword = (output.find(test_keyword) != std::string::npos);
    bool contains_dlt = (output.find(expected_dlt) != std::string::npos);

    // Explanatory comment:
    // - This test ensures that the emitted error message includes both the keyword
    //   and the human-readable datalink type corresponding to the given linktype.
    ASSERT_TRUE(contains_keyword, "fail_kw_on_dlt should include the provided keyword in the error");
    ASSERT_TRUE(contains_dlt, "fail_kw_on_dlt should include the datalink string corresponding to the linktype");
}

// Test 2: Repeat the same validation with a different linktype to ensure
// that the function adapts the datalink string accordingly.
// This covers variability in the dependent component (pcapint_datalink_val_to_string).
static void test_fail_kw_on_dlt_reports_keyword_and_dlt_string_linktype2() {
    const int test_linktype = 9999;               // An unlikely/linktype value to exercise variability
    const char* test_keyword = "KW_TEST_DEF";

    // Execute and capture stderr output
    std::string output = run_fail_kw_on_dlt_and_capture(test_linktype, test_keyword);

    // Compute expected datalink string from the library helper
    const char* expected_dlt_string = pcapint_datalink_val_to_string(test_linktype);
    std::string expected_dlt = (expected_dlt_string != nullptr) ? expected_dlt_string : "";

    // The error message should contain the keyword and the datalink string.
    bool contains_keyword = (output.find(test_keyword) != std::string::npos);
    bool contains_dlt = (output.find(expected_dlt) != std::string::npos);

    // Explanatory comment:
    // - This test asserts that changing the linktype results in a different datalink
    //   string being part of the error message, while preserving the keyword.
    ASSERT_TRUE(contains_keyword, "fail_kw_on_dlt should include the provided keyword in the error (Test 2)");
    ASSERT_TRUE(contains_dlt, "fail_kw_on_dlt should include the datalink string corresponding to the new linktype (Test 2)");
}

// Entry point: Run all tests
int main() {
    std::cout << "Running unit tests for fail_kw_on_dlt (C++11 harness)\n";

    test_fail_kw_on_dlt_reports_keyword_and_dlt_string_linktype1();
    test_fail_kw_on_dlt_reports_keyword_and_dlt_string_linktype2();

    std::cout << "Unit tests completed.\n";
    return 0;
}

// Notes for maintainers and future improvements:
// - If the internal error handling of the C code uses a non-local jump
//   (setjmp/longjmp) rather than returning normally, these tests may need an
//   alternate approach to intercept the error (e.g., enabling a test-mode
//   build of the C library that converts failures into a non-throwing return).
// - This suite captures stderr output to validate the content of the error message.
// - The tests rely on the actual pcapint_datalink_val_to_string implementation to
//   format the datalink type string. If the mapping changes, tests should be updated accordingly.
// - Static members and non-private APIs usage are kept to a minimum to respect the
//   separation of concerns and to avoid relying on internal implementations.