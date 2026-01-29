// Unit test harness for the focal method main in CMakeCCompilerId.c
// This test suite does not rely on GTest. It uses a small, self-contained
// test runner that executes potential CMakeCCompilerId executables with
// varying argument counts to exercise the program's logic via its exit codes.
// The test harness discovers executables in the current directory whose names
// begin with "CMakeCCompilerId" and treats them as different compilation variants
// (e.g., with different compile-time macros defined). It then runs each executable
// with 0..3 extra arguments to vary argc (remember: in C/C++, argc is always >= 1
// because argv[0] is the program name).

#include <sys/stat.h>
#include <vector>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <cstdlib>
#include <windows.h>
#include <unistd.h>
#include <errno.h>
#include <sstream>


#if defined(_WIN32)
#endif

// Helper: check if a file exists
static bool file_exists(const std::string& path) {
    struct stat st;
    return (stat(path.c_str(), &st) == 0);
}

// Helper: check if a path is an executable regular file
static bool is_executable(const std::string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) return false;
#if defined(_WIN32)
    // On Windows, rely on .exe suffix or file attribute; for simplicity, treat as executable if exists
    return S_ISREG(st.st_mode);
#else
    return S_ISREG(st.st_mode) && ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH));
#endif
}

// Helper: discover executables in current dir starting with "CMakeCCompilerId"
static std::vector<std::string> discover_executables() {
    std::vector<std::string> exes;
    DIR* dirp = opendir(".");
    if (!dirp) {
        std::cerr << "Warning: Unable to open current directory for executable discovery.\n";
        return exes;
    }

    struct dirent* entry;
    while ((entry = readdir(dirp)) != nullptr) {
        if (entry->d_type == DT_REG) {
            std::string name(entry->d_name);
            // Heuristic: executables/targets with the focal name
            if (name.find("CMakeCCompilerId") == 0) {
                std::string path = "./" + name;
                if (is_executable(path)) {
                    exes.push_back(path);
                }
            }
        }
    }
    closedir(dirp);

    // If for some reason nothing found yet, also consider the plain name in current dir
    if (exes.empty()) {
        std::string plain = "./CMakeCCompilerId";
        if (is_executable(plain)) exes.push_back(plain);
        // Also try without "./" (in case the runner places the binary differently)
        plain = "CMakeCCompilerId";
        if (is_executable(plain)) exes.push_back(plain);
    }

    return exes;
}

// Helper: safely quote a command-line argument (basic handling, sufficient for test harness)
static std::string quote_arg(const std::string& s) {
    std::string r = "\"";
    for (char c : s) {
        if (c == '\"') r += "\\\"";
        else r += c;
    }
    r += "\"";
    return r;
}

// Helper: run a program with given args, return its exit code.
// On POSIX, use WEXITSTATUS; on Windows, use the low-order byte of system() return.
static int run_program(const std::string& path, const std::vector<std::string>& args) {
#if defined(_WIN32)
    // Simple Windows pathway: construct command line and use system()
    std::string cmd = path;
    for (const auto& a : args) {
        cmd += " " + quote_arg(a);
    }
    int ret = static_cast<int>(std::system(cmd.c_str()));
    // Typical Windows behavior: exit code in low byte
    return (ret & 0xFF);
#else
    std::string cmd = path;
    for (const auto& a : args) {
        cmd += " " + quote_arg(a);
    }
    int ret = static_cast<int>(std::system(cmd.c_str()));
    if (ret == -1) {
        return -1;
    }
    // WIFEXITED/WEXITSTATUS require <sys/wait.h>
    #include <sys/wait.h>
    if (WIFEXITED(ret)) {
        return WEXITSTATUS(ret);
    } else {
        // Abnormal termination
        return -1;
    }
#endif
}

// Simple non-terminating assertion helper: collects failures but does not abort.
static void expect(bool condition, const std::string& description,
                   std::vector<std::string>& failures) {
    if (!condition) {
        std::cerr << "EXPECTATION FAILED: " << description << std::endl;
        failures.push_back(description);
    }
}

// Entry point: runs a suite of tests by executing CMakeCCompilerId variants with various argc.
int main() {
    // Discover potential executables that match the focal method's targets.
    std::vector<std::string> executables = discover_executables();

    if (executables.empty()) {
        std::cerr << "No CMakeCCompilerId executables found in current directory. "
                  << "Please ensure the focal binaries are built before running tests.\n";
        return 1;
    }

    std::cout << "Discovered " << executables.size() << " potential CMakeCCompilerId executables for testing.\n";

    std::vector<std::string> failures;

    // Test scenarios:
    // For each executable, run with 0..3 additional arguments, i.e., argc values 1..4.
    // This exercises the arrays indexed by argc and checks that the program returns a valid exit code.
    for (const auto& exe : executables) {
        std::cout << "\n--- Testing executable: " << exe << " ---\n";

        // Test scenario 1: no extra arguments (argc == 1)
        {
            std::vector<std::string> args; // argc = 1 (program name only)
            int exitCode = run_program(exe, args);
            std::ostringstream desc;
            desc << "Executable \"" << exe << "\" with argc=1 (no extra args) returned exit code "
                 << exitCode;
            std::cout << desc.str() << "\n";
            // True branch and false branch coverage would ideally reflect expected values.
            // We only verify that an exit code is produced.
            expect(exitCode >= 0, desc.str(), failures);
        }

        // Test scenario 2: 1 extra argument (argc == 2)
        {
            std::vector<std::string> args = {"arg1"};
            int exitCode = run_program(exe, args);
            std::ostringstream desc;
            desc << "Executable \"" << exe << "\" with argc=2 (1 extra arg) returned exit code "
                 << exitCode;
            std::cout << desc.str() << "\n";
            expect(exitCode >= 0, desc.str(), failures);
        }

        // Test scenario 3: 2 extra arguments (argc == 3)
        {
            std::vector<std::string> args = {"arg1", "arg2"};
            int exitCode = run_program(exe, args);
            std::ostringstream desc;
            desc << "Executable \"" << exe << "\" with argc=3 (2 extra args) returned exit code "
                 << exitCode;
            std::cout << desc.str() << "\n";
            expect(exitCode >= 0, desc.str(), failures);
        }

        // Test scenario 4: 3 extra arguments (argc == 4)
        {
            std::vector<std::string> args = {"arg1", "arg2", "arg3"};
            int exitCode = run_program(exe, args);
            std::ostringstream desc;
            desc << "Executable \"" << exe << "\" with argc=4 (3 extra args) returned exit code "
                 << exitCode;
            std::cout << desc.str() << "\n";
            expect(exitCode >= 0, desc.str(), failures);
        }

        // Note: If there are macros toggled by different builds of CMakeCCompilerId
        // available in the working directory (e.g., with COMPILER_VERSION_MAJOR defined or not),
        // this test harness automatically covers them by discovering all executables that match
        // the focal prefix.
    }

    // Summary
    if (!failures.empty()) {
        std::cout << "\nSummary: " << failures.size() << " test(s) failed.\n";
        for (const auto& f : failures) {
            std::cout << " - " << f << "\n";
        }
        // Return non-zero to indicate test failures in a CI context.
        return 2;
    } else {
        std::cout << "\nAll tests passed (exit code checks). Executables exercised with varying argc.\n";
        return 0;
    }
}