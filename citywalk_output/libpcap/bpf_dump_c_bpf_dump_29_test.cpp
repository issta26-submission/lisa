// test_bpf_dump.cpp
// A pragmatic, self-contained unit-test harness for the focal method bpf_dump
// under C++11 without using Google Test.
// The tests are designed to be compiled alongside the original bpf_dump.c
// (or with an equivalent environment providing the same symbols and types).
// The harness focuses on the three main execution paths of bpf_dump:
//
//  - option > 2  : prints the program length and all instructions (code, jt, jf, k)
//  - option > 1  : prints braces-wrapped instruction representations
//  - otherwise  : prints the textual image of each instruction via bpf_image(insn, i)
//                 (here we provide a small stub for bpf_image to keep the test self-contained)
//
// Notes and caveats:
// - This test relies on the assumption that the struct definitions used by the focal
//   function (struct bpf_program and struct bpf_insn) in the original source can be
//   represented in this test harness. We forward-declare minimal versions that align
//   with how bpf_dump accesses the fields (bf_len, bf_insns for the program; code, jt,
//   jf, k for an instruction). If the real definitions differ in your build, these tests
//   may need to be adapted accordingly.
// - To isolate the output for verification, stdout is captured via a POSIX pipe.
// - A lightweight stub for bpf_image is provided here. It does not depend on external
//   libraries and simply returns deterministic strings for verification.
// - The test code does not rely on private methods or fields beyond the focal API.
// - The tests can be extended to cover edge cases (e.g., zero-length programs) as needed.
//
// How to run (typical command line):
//   g++ -std=c++11 -O0 -g -Wall test_bpf_dump.cpp bpf_dump.c -D_BDEBUG=0 -D_BDEBUG
//   ./a.out
//
// The test is designed to be included in a larger test suite or compiled standalone.

#include <cassert>
#include <vector>
#include <optimize.h>
#include <sys/stat.h>
#include <pcap.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <cstdio>
#include <functional>
#include <config.h>
#include <sys/types.h>
#include <cstring>
#include <string>
#include <iostream>


extern "C" {
    // Forward declaration of the focal function from the production code.
    // We declare it with the expected signature used by the original code.
    struct bpf_program;
    struct bpf_insn;

    void bpf_dump(const struct bpf_program *p, int option);
}

// Minimal forward declarations for structures used by the focal function.
// These definitions mirror the fields accessed by bpf_dump.
struct bpf_insn {
    unsigned int code;
    unsigned int jt;
    unsigned int jf;
    unsigned int k;
};

// The real bpf_dump uses: struct bpf_program { int bf_len; struct bpf_insn *bf_insns; }
struct bpf_program {
    int bf_len;
    struct bpf_insn *bf_insns;
};

// Stub for bpf_image to keep tests self-contained.
// We declare it with C linkage to match the expected symbol that bpf_dump will resolve.
extern "C" const char* bpf_image(const struct bpf_insn* /*insn*/, int /*idx*/) {
    // Simple deterministic placeholder image for each instruction index.
    // We reuse a small rotating buffer to avoid returning a pointer to a temporary.
    static char buffers[8][64];
    static int idxBuf = 0;
    int i = idxBuf;
    idxBuf = (idxBuf + 1) % 8;
    snprintf(buffers[i], sizeof(buffers[i]), "image-%d", idx);
    return buffers[i];
}

// A helper to capture stdout output from a function call.
// It redirects stdout to a pipe, runs the provided callable, then collects the output.

std::string capture_stdout(const std::function<void()> &func) {
    // Create a pipe to capture stdout
    int pipe_fd[2];
    if (pipe(pipe_fd) != 0) {
        perror("pipe");
        return "";
    }

    // Save current stdout
    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1) {
        perror("dup");
        return "";
    }

    // Redirect stdout to pipe_write end
    if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) {
        perror("dup2");
        return "";
    }

    // Close write end in this process since it's now dup'ed to stdout
    close(pipe_fd[1]);

    // Execute the test function
    func();

    // Flush and restore stdout
    fflush(stdout);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    // Read all data from the pipe_read end
    constexpr size_t BUFSIZE = 4096;
    char buffer[BUFSIZE];
    std::string output;
    ssize_t bytes;
    // The read may block if the writer hasn't closed; ensure we close the write end earlier.
    while ((bytes = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, buffer + bytes);
    }
    close(pipe_fd[0]);
    return output;
}

// Helper to compare actual output with expected string (trimmed for robust tests)
static void assert_equal(const std::string &actual, const std::string &expected, const char *testName) {
    if (actual != expected) {
        std::cerr << "TEST FAILED: " << testName << "\n";
        std::cerr << "Expected:\n" << expected << "\n";
        std::cerr << "Actual:\n" << actual << "\n";
        // Use assertion to stop on first failure for clarity
        assert(false && "Output did not match expected");
    } else {
        std::cout << "TEST PASSED: " << testName << "\n";
    }
}

// Test 1: option == 3 (option > 2 branch)
// Expect: first line is the length, followed by n lines each with: "<code> <jt> <jf> <k>"
static void test_bpf_dump_option_gt2_prints_header_and_instructions() {
    // Prepare a small program with 2 instructions
    const int N = 2;
    struct bpf_insn insns[N] = {
        {0x1234, 1, 0, 0xABCD}, // code, jt, jf, k
        {0x4321, 0, 2, 0x1234}
    };
    struct bpf_program prog;
    prog.bf_len = N;
    prog.bf_insns = insns;

    // Expected output
    // First line: "2\n" are the total number of instructions
    // Then two lines with decimal values of fields
    // 0x1234 = 4660, 0xABCD = 43981
    // 0x4321 = 17185, 0x1234 = 4660
    const char *expected_cstr =
        "2\n"
        "4660 1 0 43981\n"
        "17185 0 2 4660\n";

    // Capture stdout while invoking bpf_dump
    std::string actual = capture_stdout([&prog]() {
        bpf_dump(&prog, 3);
    });

    // Compare
    assert_equal(actual, std::string(expected_cstr), "bpf_dump option 3 prints header and instructions");
}

// Test 2: option == 2 (option > 1 branch)
// Expect: two lines formatted as C array-like entries:
// "{ 0x1234, 1, 0, 0x0000abcd },\n{ 0x4321, 0, 2, 0x00001234 },\n"
static void test_bpf_dump_option_eq2_prints_struct_format() {
    // Prepare a small program with 2 instructions
    const int N = 2;
    struct bpf_insn insns[N] = {
        {0x1234, 1, 0, 0xABCD},
        {0x4321, 0, 2, 0x1234}
    };
    struct bpf_program prog;
    prog.bf_len = N;
    prog.bf_insns = insns;

    const char *expected_cstr =
        "{ 0x1234, 1, 0, 0x0000abcd },\n"
        "{ 0x4321, 0, 2, 0x00001234 },\n";

    std::string actual = capture_stdout([&prog]() {
        bpf_dump(&prog, 2);
    });

    assert_equal(actual, std::string(expected_cstr), "bpf_dump option 2 prints struct-like representation");
}

// Test 3: option == 0 (default branch, non-BDEBUG path)
// We assume BDEBUG is not defined, so the code calls bpf_image(insn, i) for each instruction.
// We provided a stub for bpf_image which returns deterministic outputs like "image-<idx>".
// Expect: a line per instruction with the image string (one per instruction), no extra banners.
static void test_bpf_dump_option_default_prints_bpf_image_outputs() {
    // Prepare a small program with 2 instructions
    const int N = 2;
    struct bpf_insn insns[N] = {
        {0x1111, 0, 0, 0},
        {0x2222, 0, 0, 0}
    };
    struct bpf_program prog;
    prog.bf_len = N;
    prog.bf_insns = insns;

    // Expected output from our bpf_image stub for two calls: "image-0" and "image-1"
    const char *expected_cstr = "image-0\nimage-1\n";

    std::string actual = capture_stdout([&prog]() {
        bpf_dump(&prog, 0);
    });

    assert_equal(actual, std::string(expected_cstr), "bpf_dump default path prints bpf_image outputs");
}

// Entry point: run all tests
int main() {
    // Run tests sequentially. Any assertion failure will abort the test and report.
    test_bpf_dump_option_gt2_prints_header_and_instructions();
    test_bpf_dump_option_eq2_prints_struct_format();
    test_bpf_dump_option_default_prints_bpf_image_outputs();
    std::cout << "All tests completed." << std::endl;
    return 0;
}