// Test suite for the focal method Help located in transicc.c
// This test harness is designed for C++11 environments without GTest.
// It captures stderr output produced by Help() and validates presence
// of key sections and option lines to ensure basic correctness and
// coverage of the Help() functionality.
//
// Assumptions:
// - transicc.c is compiled separately and linked with this test binary.
// - The Help function uses C linkage: extern "C" void Help(void);
// - The environment is POSIX-compatible (pipe, dup, dup2, read).

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <unistd.h>
#include <utils.h>



extern "C" void Help(void); // Focal method with C linkage

// Simple test harness state
static bool g_all_tests_passed = true;

// Simple assertion macro that does not terminate on failure
#define EXPECT(cond, message) \
    do { \
        if (!(cond)) { \
            std::cerr << "TEST FAILURE: " << (message) << std::endl; \
            g_all_tests_passed = false; \
        } \
    } while (0)

// Capture the output printed to stderr by calling the focal method Help().
static std::string captureHelpOutput() {
    // Create a pipe to capture stderr
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        return "";
    }

    // Save current stderr
    int saved_stderr = dup(fileno(stderr));
    if (saved_stderr == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stderr to the pipe
    if (dup2(pipefd[1], fileno(stderr)) == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stderr);
        return "";
    }
    // Close the write end in the current process; Help() writes to stderr now via the pipe
    close(pipefd[1]);

    // Invoke the focal method
    Help();

    // Ensure output is flushed
    fflush(stderr);

    // Read from the read end of the pipe
    std::string output;
    char buffer[4096];
    ssize_t bytesRead;
    while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(bytesRead));
    }

    // Restore original stderr
    dup2(saved_stderr, fileno(stderr));
    close(saved_stderr);
    close(pipefd[0]);

    return output;
}

// Test 1: Ensure the Help() output contains the usage header line
static void test_help_contains_usage_line() {
    std::string out = captureHelpOutput();
    const std::string needle = "usage: transicc [flags] [CGATS input] [CGATS output]";
    EXPECT(out.find(needle) != std::string::npos,
           "Help output should contain the usage header line.");
}

// Test 2: Ensure the Help() output contains the flags section header
static void test_help_contains_flags_header() {
    std::string out = captureHelpOutput();
    const std::string needle = "flags:\n\n"; // exact newline sequence as printed
    // The program prints "flags:" followed by two newlines
    // Some environments may condense newlines in capture; still rely on exact match here.
    EXPECT(out.find(needle) != std::string::npos,
           "Help output should contain the 'flags' header section.");
}

// Test 3: Check presence of major option lines (robust by substring)
static void test_help_contains_major_options() {
    std::string out = captureHelpOutput();

    // Verbosity option line
    const std::string opt_v = "-v<0..3> - Verbosity level";
    // Encoded representation option
    const std::string opt_e = "-e[op] - Encoded representation of numbers";
    // 16-bit and hex options (tab may precede; accept both forms)
    const bool has_w = (out.find("\t-w - use 16 bits") != std::string::npos) ||
                       (out.find("-w - use 16 bits") != std::string::npos);
    const bool has_x = (out.find("\t-x - Hexadecimal") != std::string::npos) ||
                       (out.find("-x - Hexadecimal") != std::string::npos);
    // Bounded mode and Quantize
    const std::string opt_s = "-s - bounded mode (clip negatives and highlights)";
    const std::string opt_q = "-q - Quantize (round decimals)";
    // Input/Output/Device-link profiles
    const std::string opt_i = "-i<profile> - Input profile (defaults to sRGB)";
    const std::string opt_o = "-o<profile> - Output profile (defaults to sRGB)";
    const std::string opt_l = "-l<profile> - Transform by device-link profile";

    // Soft proof options
    const std::string opt_p = "-p<profile> - Soft proof profile";
    const std::string opt_m = "-m<0,1,2,3> - Soft proof intent";

    // Assertions
    EXPECT(out.find(opt_v) != std::string::npos, "Help should mention verbosity option.");
    EXPECT(out.find(opt_e) != std::string::npos, "Help should mention encoded representation option.");
    EXPECT(has_w, "Help should mention 16-bit encoding option (-w).");
    EXPECT(has_x, "Help should mention hexadecimal encoding option (-x).");
    EXPECT(out.find(opt_s) != std::string::npos, "Help should mention bounded mode (-s).");
    EXPECT(out.find(opt_q) != std::string::npos, "Help should mention quantize option (-q).");
    EXPECT(out.find(opt_i) != std::string::npos, "Help should mention input profile option (-i).");
    EXPECT(out.find(opt_o) != std::string::npos, "Help should mention output profile option (-o).");
    EXPECT(out.find(opt_l) != std::string::npos, "Help should mention device-link profile option (-l).");
    EXPECT(out.find(opt_p) != std::string::npos, "Help should mention soft proof profile option (-p).");
    EXPECT(out.find(opt_m) != std::string::npos, "Help should mention soft proof intent option (-m).");
}

// Test 4: Ensure the program blurb about Little CMS appears (presence of key phrases)
static void test_help_contains_blurb() {
    std::string out = captureHelpOutput();
    // Check for presence of "Little CMS" and a mail/contact line
    bool hasLittleCMS = (out.find("Little CMS") != std::string::npos);
    bool hasContact = (out.find("info@littlecms.com") != std::string::npos);
    EXPECT(hasLittleCMS, "Help should mention Little CMS in program blurb.");
    EXPECT(hasContact, "Help should mention contact email in program blurb.");
}

// Run all tests and report overall result
static void run_all_tests() {
    test_help_contains_usage_line();
    test_help_contains_flags_header();
    test_help_contains_major_options();
    test_help_contains_blurb();

    if (g_all_tests_passed) {
        std::cout << "ALL TESTS PASSED: Help() produced expected content." << std::endl;
    } else {
        std::cerr << "SOME TESTS FAILED: See above for details." << std::endl;
    }
}

// Main entry: run tests from the main function as required when GTest isn't available
int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    run_all_tests();
    return g_all_tests_passed ? 0 : 1;
}