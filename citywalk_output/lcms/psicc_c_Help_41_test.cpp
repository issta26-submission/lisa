#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <vector>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <cstdlib>
#include <unistd.h>
#include <utils.h>
#include <sstream>


//
// Lightweight, framework-free test suite for the focal method Help()
// located in psicc.c. The Help() function prints a usage/help banner
// to stderr and then calls exit(0). Since directly calling Help() from
// the test binary would terminate the test process, we instead run the
// psicc executable as a separate process with flags intended to invoke
// Help() (e.g., -h, --help) and capture its output and exit code.
//
// Test environment assumptions:
// - The psicc executable is available at runtime (e.g., ./psicc or in PATH).
// - When invoked with -h or --help, psicc prints the banner and usage info
//   to stdout/stderr and exits with code 0.
// - We capture both stdout and stderr by redirecting 2>&1 in the shell.
// - This test is written for a C++11 compatible environment without GTest.
//


/* Candidate Keywords derived from Step 1 to guide test coverage
   - Little CMS ICC PostScript generator
   - cmsGetEncodedCMMversion
   - CSA (Color Space Array)
   - CRD (Color Rendering Dictionary)
   - usage: psicc [flags] [<Output file>]
   - flags: -i, -o, -t, -b, -u, -c, -n
   - info@littlecms.com
   - main, Help, HandleSwitches, GenerateCSA, GenerateCRD
   - Output to stderr (fprintf(stderr, ...))
*/

/* Utility: locate psicc executable in common locations (relative or PATH) */
static bool locate_psicc_path(std::string &pathOut) {
    // 1) Try common local path
    const char* candidates[] = { "./psicc", "./psicc.exe", "psicc" };
    for (const char* c : candidates) {
        if (c == nullptr) continue;
        if (access(c, X_OK) == 0) {
            pathOut = std::string(c);
            return true;
        }
    }

    // 2) Search PATH
    const char* pathEnv = getenv("PATH");
    if (pathEnv) {
        std::string pathStr(pathEnv);
        size_t start = 0, end = 0;
        while (start < pathStr.size()) {
            end = pathStr.find(':', start);
            std::string dir = pathStr.substr(start, end - start);
            if (!dir.empty()) {
                std::string candidate = dir + "/psicc";
                if (access(candidate.c_str(), X_OK) == 0) {
                    pathOut = candidate;
                    return true;
                }
            }
            if (end == std::string::npos) break;
            start = end + 1;
        }
    }

    return false;
}

/* Utility: Run psicc with given args, capture combined stdout/stderr, and exit code */
static bool run_psicc_with_args(const std::string &args, std::string &output, int &exitCode) {
    std::string path;
    if (!locate_psicc_path(path)) {
        output = "psicc executable not found on this system";
        exitCode = -1;
        return false;
    }

    // Build shell command to capture stdout+stderr
    std::string command = path + " " + args + " 2>&1";

    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe) {
        output = "popen() failed to execute psicc";
        exitCode = -1;
        return false;
    }

    char buffer[4096];
    std::ostringstream oss;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        oss << buffer;
    }

    int status = pclose(pipe);
    // Decode exit status
    if (WIFEXITED(status)) {
        exitCode = WEXITSTATUS(status);
    } else {
        exitCode = -1;
    }

    output = oss.str();
    return true;
}

/* Simple test harness: track results and print summary */
struct TestCase {
    std::string name;
    bool passed;
    std::string details;
};

static void print_banner() {
    std::cout << "Running Help() test suite for psicc.c (focal method: Help())\n";
}

static void run_and_report(const TestCase &tc) {
    std::cout << "[TEST] " << tc.name << " ... " << (tc.passed ? "PASSED" : "FAILED") << "\n";
    if (!tc.passed) {
        std::cout << "  Details: " << tc.details << "\n";
    }
}

/* Test 1: Trigger Help via -h flag and verify banner and usage text appear
   This covers the positive path where Help() is invoked and the program exits normally. */
static TestCase test_help_via_short_help_flag() {
    TestCase t;
    t.name = "Help invoked with -h flag prints banner and usage and exits cleanly";

    std::string output;
    int exitCode = -1;
    bool ok = run_psicc_with_args("-h", output, exitCode);

    // Basic sanity: execution succeeded and exit code is 0
    if (!ok) {
        t.passed = false;
        t.details = "Executable not found or failed to run.";
        return t;
    }

    if (exitCode != 0) {
        t.passed = false;
        std::ostringstream ds;
        ds << "Unexpected exit code: " << exitCode;
        t.details = ds.str();
        t.details += "\nOutput:\n" + output;
        return t;
    }

    // Verify content heuristics: presence of banner and usage text
    bool hasBanner = output.find("Little CMS ICC PostScript generator") != std::string::npos
                 || output.find("Little CMS ICC PostScript generator -") != std::string::npos;
    bool hasUsage  = output.find("usage: psicc [flags] [<Output file>]") != std::string::npos;
    bool hasFlags  = output.find("flags:") != std::string::npos;
    bool hasInfo   = output.find("info@littlecms.com") != std::string::npos;

    if (hasBanner && hasUsage && hasFlags && hasInfo) {
        t.passed = true;
        t.details = "Output contains expected header lines and usage information.";
    } else {
        t.passed = false;
        std::ostringstream ds;
        ds << "Output missing expected sections. BannerFound=" << hasBanner
           << ", UsageFound=" << hasUsage
           << ", FlagsFound=" << hasFlags
           << ", InfoFound=" << hasInfo;
        ds << "\nFull Output:\n" << output;
        t.details = ds.str();
    }

    return t;
}

/* Test 2: Trigger Help via --help flag and verify banner and usage text appear
   This covers the alternate common invocation path for Help(). */
static TestCase test_help_via_long_help_flag() {
    TestCase t;
    t.name = "Help invoked with --help flag prints banner and usage and exits cleanly";

    std::string output;
    int exitCode = -1;
    bool ok = run_psicc_with_args("--help", output, exitCode);

    if (!ok) {
        t.passed = false;
        t.details = "Executable not found or failed to run.";
        return t;
    }

    if (exitCode != 0) {
        t.passed = false;
        std::ostringstream ds;
        ds << "Unexpected exit code: " << exitCode;
        t.details = ds.str();
        t.details += "\nOutput:\n" + output;
        return t;
    }

    bool hasBanner = output.find("Little CMS ICC PostScript generator") != std::string::npos
                 || output.find("Little CMS ICC PostScript generator -") != std::string::npos;
    bool hasUsage  = output.find("usage: psicc [flags] [<Output file>]") != std::string::npos;
    bool hasFlags  = output.find("flags:") != std::string::npos;
    bool hasInfo   = output.find("info@littlecms.com") != std::string::npos;

    if (hasBanner && hasUsage && hasFlags && hasInfo) {
        t.passed = true;
        t.details = "Output contains expected header lines and usage information (long flag).";
    } else {
        t.passed = false;
        std::ostringstream ds;
        ds << "Output missing expected sections. BannerFound=" << hasBanner
           << ", UsageFound=" << hasUsage
           << ", FlagsFound=" << hasFlags
           << ", InfoFound=" << hasInfo;
        ds << "\nFull Output:\n" << output;
        t.details = ds.str();
    }

    return t;
}

/* Main: run all tests and print a final summary. */
int main() {
    print_banner();

    std::vector<TestCase> results;
    results.push_back(test_help_via_short_help_flag());
    results.push_back(test_help_via_long_help_flag());

    int passed = 0;
    for (const auto &tc : results) {
        run_and_report(tc);
        if (tc.passed) ++passed;
    }

    std::cout << "Summary: " << passed << "/" << results.size() << " tests passed.\n";

    return (passed == (int)results.size()) ? 0 : 1;
}