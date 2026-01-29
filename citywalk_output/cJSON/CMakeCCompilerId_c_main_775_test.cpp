/*
  Lightweight C++11 test harness to validate the focal method main in CMakeCCompilerId.c.

  Approach:
  - Compile the focal C file multiple times with different preprocessor macro combinations.
  - For each build, run the resulting executable with varying argc values by supplying command-line arguments.
  - Baseline: compile with no macros defined. Record exit codes for argc values {1, 2, 3} (i.e., no extra args, 1 extra arg, 2 extra args).
  - Single-macro tests: for each macro in {COMPILER_VERSION_MAJOR, COMPILER_VERSION_INTERNAL, SIMULATE_ID, SIMULATE_VERSION_MAJOR, __CRAYXT_COMPUTE_LINUX_TARGET}, compile with that macro defined. For each argc, compute delta = exit_code_with_macro - baseline_exit_code.
  - Validate non-negativity deltas (assuming info_* values are non-negative as is typical for "info" arrays).
  - Pairwise tests: for each pair of macros, compile with both defined. For each argc, verify exit_code == baseline + delta_macro1 + delta_macro2.
  - All test runs are performed via system() to invoke the system C compiler (cc/clang/gcc). The test does not depend on GTest; it uses a tiny in-house assertion mechanism.
  - Explanatory comments accompany each unit-test section.

  Notes:
  - The focal C file is expected to be in the same directory as this test (CMakeCCompilerId.c).
  - The test uses the program name as argv[0], and argc is controlled by the number of command-line arguments supplied to the executable.
  - This harness targets POSIX environments for exit status handling; on Windows, WEXITSTATUS is not applicable, but the code path is primarily intended for POSIX-like systems.

  Limitations:
  - If any info_* entry for a given argc is zero for all macros, deltas may be zero; the test logic tolerates non-negative deltas but flags failures if a defined macro yields a negative delta.
  - The test assumes the CMakeCCompilerId.c file compiles with standard C compiler flags and does not require additional include paths.

  Usage:
  - Build this test as part of your project or run it standalone in a UNIX-like environment.
  - Ensure the CMakeCCompilerId.c is accessible at the path provided in the harness (same directory by default).

  C++11 code begins here.
*/

#include <windows.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdio>
#include <map>
#include <memory>
#include <cstdlib>
#include <string>


#ifdef _WIN32
#else
#endif

// Helper to run a system command and capture its exit status (0..255 on POSIX).
int run_command(const std::string& cmd) {
#ifdef _WIN32
    int ret = std::system(cmd.c_str());
    if (ret == -1) return -1;
    // On Windows, system() returns a status that needs extraction; we'll approximate.
    // WEXITSTATUS is not available on Windows; assume upper 8 bits contain exit code.
    return (ret & 0xFF);
#else
    int status = std::system(cmd.c_str());
    if (status == -1) return -1;
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    } else {
        // Abnormal termination
        return -1;
    }
#endif
}

// Compile CMakeCCompilerId.c into an executable named <exeName>, with optional macros defined.
bool compile_with_macros(const std::string& exeName, const std::vector<std::string>& macros) {
    std::string cmd = "cc -std=c11 -O0 -pipe -static -s CMakeCCompilerId.c -o " + exeName;
    for (const auto& m : macros) {
        cmd += " -D" + m;
    }
    int ret = run_command(cmd);
    if (ret != 0) {
        std::cerr << "Compilation failed for " << exeName << " with macros: ";
        for (size_t i = 0; i < macros.size(); ++i) {
            if (i) std::cerr << ",";
            std::cerr << macros[i];
        }
        std::cerr << std::endl;
        return false;
    }
    return true;
}

// Run the executable named exeName with argc-1 extra command-line arguments (to achieve argc value).
// Returns the exit code of the executable.
int run_with_argc(const std::string& exeName, int argc) {
    if (argc < 1) return -1;
    std::string cmd = "./" + exeName;
    // argv[0] is program name; we supply (argc-1) extra args.
    for (int i = 1; i < argc; ++i) {
        cmd += " arg" + std::to_string(i);
    }
    int exitCode = run_command(cmd);
    return exitCode;
}

// Simple helper to print test result
void log_result(bool pass, const std::string& msg) {
    if (pass) {
        std::cout << "[PASS] " << msg << std::endl;
    } else {
        std::cout << "[FAIL] " << msg << std::endl;
    }
}

int main() {
    // Path to focal C file
    const std::string focalCFile = "CMakeCCompilerId.c";

    // Ensure the focal file exists
    FILE* f = std::fopen(focalCFile.c_str(), "r");
    if (!f) {
        std::cerr << "ERROR: Could not open " << focalCFile << ". Make sure the file exists in the current directory." << std::endl;
        return 2;
    }
    std::fclose(f);

    // Step 1: Baseline compilation with no macros defined
    // Expected: We record the exit codes for argc values 1..3 (i.e., providing 0..2 extra arguments)
    log_result(true, "Starting baseline compilation (no macros defined).");
    const std::string baselineExe = "icbaseline";

    if (!compile_with_macros(baselineExe, {})) {
        log_result(false, "Baseline compilation failed. Aborting tests.");
        return 3;
    }

    // Baseline exit codes for argc = 1, 2, 3
    const int argc_values[] = {1, 2, 3};
    const int NUM_ARGS = sizeof(argc_values) / sizeof(argc_values[0]);
    std::vector<int> baseline_codes(NUM_ARGS, -1);
    for (int i = 0; i < NUM_ARGS; ++i) {
        baseline_codes[i] = run_with_argc(baselineExe, argc_values[i]);
    }

    // If any baseline code is negative, something is wrong
    bool baseline_ok = true;
    for (int i = 0; i < NUM_ARGS; ++i) {
        if (baseline_codes[i] < 0) {
            baseline_ok = false;
            break;
        }
    }
    if (!baseline_ok) {
        log_result(false, "Baseline run failed for one or more argc values.");
        return 4;
    }
    log_result(true, "Baseline compilation and runs successful. Recorded exit codes for argc 1..3.");

    // Step 2: Single-macro tests
    const std::vector<std::string> macros = {
        "COMPILER_VERSION_MAJOR",
        "COMPILER_VERSION_INTERNAL",
        "SIMULATE_ID",
        "SIMULATE_VERSION_MAJOR",
        "__CRAYXT_COMPUTE_LINUX_TARGET"
    };

    // Map: macro -> vector of deltas per argc (delta = exit_with_macro - baseline[argc])
    std::map<std::string, std::vector<int>> deltas;

    // For each macro, compile with that macro defined and record deltas
    for (const auto& macro : macros) {
        std::string exeName = "ictest_" + macro;
        if (!compile_with_macros(exeName, {macro})) {
            log_result(false, "Failed to compile with macro " + macro);
            return 5;
        }
        std::vector<int> deltas_for_macro(NUM_ARGS, 0);
        bool macro_ok = true;
        for (int i = 0; i < NUM_ARGS; ++i) {
            int code = run_with_argc(exeName, argc_values[i]);
            if (code < 0) {
                macro_ok = false;
                break;
            }
            deltas_for_macro[i] = code - baseline_codes[i];
        }
        if (!macro_ok) {
            log_result(false, "Runtime failure when running with macro " + macro);
            return 6;
        }
        deltas[macro] = deltas_for_macro;
        // Check non-negativity of delta for this macro across argc values
        bool nonneg = true;
        for (int i = 0; i < NUM_ARGS; ++i) {
            if (deltas_for_macro[i] < 0) {
                nonneg = false;
                break;
            }
        }
        if (nonneg) {
            log_result(true, "Delta non-negativity holds for macro: " + macro);
        } else {
            log_result(false, "Delta negative observed for macro: " + macro + ". Delta values: " +
                       std::to_string(deltas_for_macro[0]) + ", " +
                       std::to_string(deltas_for_macro[1]) + ", " +
                       std::to_string(deltas_for_macro[2]));
        }
    }

    // Step 3: Pairwise macro tests
    // For each pair, verify exit_code == baseline + delta_macro1 + delta_macro2 for each argc
    std::vector<std::string> macroNames;
    for (const auto& m : macros) macroNames.push_back(m);
    bool pairwise_all_ok = true;
    for (size_t i = 0; i < macroNames.size(); ++i) {
        for (size_t j = i + 1; j < macroNames.size(); ++j) {
            std::string exeName = "ictest_pair_" + macroNames[i] + "_" + macroNames[j];
            if (!compile_with_macros(exeName, {macroNames[i], macroNames[j]})) {
                log_result(false, "Failed to compile pair with macros: " +
                           macroNames[i] + ", " + macroNames[j]);
                return 7;
            }
            bool pair_ok = true;
            for (int k = 0; k < NUM_ARGS; ++k) {
                int code = run_with_argc(exeName, argc_values[k]);
                int expected = baseline_codes[k] +
                               deltas[macroNames[i]][k] +
                               deltas[macroNames[j]][k];
                if (code != expected) {
                    pair_ok = false;
                    std::cerr << "  Mismatch for pair (" << macroNames[i] << ", " << macroNames[j] << ") "
                              << "argc=" << argc_values[k] << ": expected " << expected
                              << ", got " << code << std::endl;
                }
            }
            if (pair_ok) {
                log_result(true, "Pair test passed for (" + macroNames[i] + ", " + macroNames[j] + ").");
            } else {
                log_result(false, "Pair test FAILED for (" + macroNames[i] + ", " + macroNames[j] + "). See details above.");
                pairwise_all_ok = false;
            }
        }
    }

    if (pairwise_all_ok) {
        log_result(true, "All pairwise macro tests passed.");
    } else {
        log_result(false, "Some pairwise macro tests failed.");
    }

    // Summary
    log_result(true, "Test harness completed. Baseline and macro-delta tests executed.");
    // Cleanup: remove generated executables for cleanliness (best effort)
    // Warning: best-effort removal; ignore failures.
    auto rm = [](const std::string& name) {
        std::string cmd = "rm -f " + name;
        run_command(cmd);
    };

    rm(baselineExe);
    for (const auto& macro : macros) {
        rm("ictest_" + macro);
    }
    for (size_t i = 0; i < macros.size(); ++i) {
        for (size_t j = i + 1; j < macros.size(); ++j) {
            rm("ictest_pair_" + macros[i] + "_" + macros[j]);
        }
    }

    // Exit with 0 if all performed cleanly; non-zero if a critical step failed is already returned earlier.
    return 0;
}