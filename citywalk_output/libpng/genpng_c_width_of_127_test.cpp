// Test suite for the focal method width_of in genpng.c
// This test harness uses fork to isolate exit(1) calls from invalid inputs.
// It compiles with C++11 and links against the C implementation of width_of.
// Notes:
// - We test both valid and invalid inputs according to the function logic.
// - For valid inputs, the child writes the computed width to a pipe and exits with code 0.
// - For invalid inputs, width_of calls exit(1); the parent detects this and marks the test as invalid.
// - The tests are invoked from main() as plain C++ code (no Google Test framework).

#include <unistd.h>
#include <../../png.h>
#include <cerrno>
#include <vector>
#include <stddef.h>
#include <string.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


extern "C" double width_of(const char *arg); // forward declaration for C function

// Small helper to compare doubles with tolerance
static bool almost_equal(double a, double b, double tol = 1e-9) {
    if (std::isnan(a) || std::isnan(b)) return false;
    return std::fabs(a - b) <= tol * (1.0 + std::fabs(a) + std::fabs(b));
}

// Structure to hold test results
struct TestResult {
    bool valid;  // true if width_of returned a value; false if invalid input caused exit(1)
    double value; // valid only if valid == true
};

// Run width_of in a separate process to safely capture exit(1) on invalid input.
// Returns TestResult with valid flag and value (value is meaningful only if valid == true)
TestResult run_width_of_in_isolated_proc(const std::string& input) {
    TestResult res;
    res.valid = false;
    res.value = 0.0;

    int pipefd[2];
    if (pipe(pipefd) != 0) {
        std::cerr << "Pipe creation failed\n";
        return res;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed\n";
        close(pipefd[0]);
        close(pipefd[1]);
        return res;
    }

    if (pid == 0) {
        // Child process
        // Close read end
        close(pipefd[0]);
        // Call the focal function
        double v = width_of(input.c_str());

        // Write the result to the pipe for the parent to read
        char buf[128];
        int n = std::snprintf(buf, sizeof(buf), "%0.15g\n", v);
        if (n < 0) n = 0;
        ssize_t written = 0;
        while (written < static_cast<ssize_t>(n)) {
            ssize_t w = write(pipefd[1], buf + written, n - written);
            if (w <= 0) break;
            written += w;
        }
        close(pipefd[1]);
        _exit(0); // normal child exit
    } else {
        // Parent process
        // Close write end
        close(pipefd[1]);
        int status = 0;
        // Wait for child
        if (waitpid(pid, &status, 0) == -1) {
            std::cerr << "waitpid failed\n";
            close(pipefd[0]);
            return res;
        }

        if (WIFEXITED(status)) {
            int es = WEXITSTATUS(status);
            if (es == 0) {
                // Child returned successfully; read the value from pipe
                std::string acc;
                char buf[128];
                ssize_t r;
                while ((r = read(pipefd[0], buf, sizeof(buf))) > 0) {
                    acc.append(buf, static_cast<size_t>(r));
                }
                close(pipefd[0]);
                if (acc.empty()) {
                    // No data returned; treat as invalid
                    res.valid = false;
                    return res;
                }
                // Parse the value
                const char* s = acc.c_str();
                char* endptr = nullptr;
                double val = std::strtod(s, &endptr);
                // We accept trailing whitespace; ensure endptr reaches end of string after trimming spaces/newlines
                while (endptr != nullptr && *endptr != '\0' && std::isspace(static_cast<unsigned char>(*endptr))) {
                    ++endptr;
                }
                if (endptr == nullptr || *endptr == '\0') {
                    res.valid = true;
                    res.value = val;
                    return res;
                } else {
                    // Unexpected extra characters; still consider as valid numeric if conversion consumed all relevant characters earlier
                    res.valid = true;
                    res.value = val;
                    return res;
                }
            } else if (es == 1) {
                // Invalid input path: width_of called exit(1)
                close(pipefd[0]);
                res.valid = false;
                return res;
            } else {
                // Other exit code: treat as invalid for safety
                close(pipefd[0]);
                res.valid = false;
                return res;
            }
        } else {
            // Abnormal termination (e.g., killed). Treat as invalid
            close(pipefd[0]);
            res.valid = false;
            return res;
        }
    }

    // Should not reach here
    close(pipefd[0]);
    return res;
}

int main() {
    // Helper macro for pretty printing
    auto print_case = [](int idx, const std::string& label, bool pass) {
        std::cout << "Test " << idx << ": " << label << " -> " << (pass ? "PASS" : "FAIL") << std::endl;
    };

    int test_index = 1;
    bool overall_pass = true;

    // 1) width_of("filled") should return 0
    {
        TestResult r = run_width_of_in_isolated_proc("filled");
        bool pass = r.valid && almost_equal(r.value, 0.0);
        print_case(test_index++, "width_of(\"filled\") == 0", pass);
        overall_pass = overall_pass && pass;
    }

    // 2) width_of("2.5") should return 2.5
    {
        TestResult r = run_width_of_in_isolated_proc("2.5");
        bool pass = r.valid && almost_equal(r.value, 2.5);
        print_case(test_index++, "width_of(\"2.5\") == 2.5", pass);
        overall_pass = overall_pass && pass;
    }

    // 3) width_of("10") should return 10
    {
        TestResult r = run_width_of_in_isolated_proc("10");
        bool pass = r.valid && almost_equal(r.value, 10.0);
        print_case(test_index++, "width_of(\"10\") == 10", pass);
        overall_pass = overall_pass && pass;
    }

    // 4) width_of("") should be invalid (exit(1))
    {
        TestResult r = run_width_of_in_isolated_proc("");
        bool pass = !r.valid;
        print_case(test_index++, "width_of(\"\") invalid", pass);
        overall_pass = overall_pass && pass;
    }

    // 5) width_of("0") should be invalid
    {
        TestResult r = run_width_of_in_isolated_proc("0");
        bool pass = !r.valid;
        print_case(test_index++, "width_of(\"0\") invalid", pass);
        overall_pass = overall_pass && pass;
    }

    // 6) width_of("-1") should be invalid
    {
        TestResult r = run_width_of_in_isolated_proc("-1");
        bool pass = !r.valid;
        print_case(test_index++, "width_of(\"-1\") invalid", pass);
        overall_pass = overall_pass && pass;
    }

    // 7) width_of("2.5x") should be invalid
    {
        TestResult r = run_width_of_in_isolated_proc("2.5x");
        bool pass = !r.valid;
        print_case(test_index++, "width_of(\"2.5x\") invalid", pass);
        overall_pass = overall_pass && pass;
    }

    // 8) width_of("abc") should be invalid
    {
        TestResult r = run_width_of_in_isolated_proc("abc");
        bool pass = !r.valid;
        print_case(test_index++, "width_of(\"abc\") invalid", pass);
        overall_pass = overall_pass && pass;
    }

    // 9) width_of("2.5 ") should be invalid (trailing space makes epoch not end at NUL in strict path)
    {
        TestResult r = run_width_of_in_isolated_proc("2.5 ");
        // Depending on strtod behavior, this is considered invalid due to trailing non-nul after numeric
        bool pass = !r.valid;
        print_case(test_index++, "width_of(\"2.5 \") invalid", pass);
        overall_pass = overall_pass && pass;
    }

    // Final result
    if (overall_pass) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << "Some tests FAILED." << std::endl;
        return 1;
    }
}