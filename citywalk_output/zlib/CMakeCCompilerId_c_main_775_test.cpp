// Automated C++11 unit-test suite for the focal method logic (reproduced locally for testing)
// This test suite intentionally avoids any external testing framework (GTest, etc.),
// and uses a lightweight in-process test harness with non-terminating assertions.
// The tests cover the branches implied by the focal code's conditional accumulation.

// The focal logic being tested (recreated here for unit testing purposes):
// - There are a number of static arrays (info_compiler, info_platform, info_arch, ...).
// - The computed value (require) is the sum of selected array elements indexed by argc.
// - Some terms are included conditionally (as if governed by compile-time macros in the real file).
// - We emulate the conditional branches via boolean flags passed at runtime to exercise
//   true/false branches for each predicate.

// This file is self-contained and does not rely on the original CMakeCCompilerId.c.

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>


// Namespace to respect "Use the correct namespace" guidance.
namespace test_framework {

// Candidate Keywords extracted from the focal method (core dependent components):
// info_compiler, info_platform, info_arch, info_version, info_version_internal,
// info_simulate, info_simulate_version, info_cray, info_language_standard_default,
// info_language_extensions_default

// Static data mimicking the original arrays (indexed by argc).
static const int info_compiler[4] =  {1, 2, 3, 4};
static const int info_platform[4]  = {10, 20, 30, 40};
static const int info_arch[4]      = {100, 200, 300, 400};
static const int info_version[4]     = {1000, 2000, 3000, 4000};
static const int info_version_internal[4] = {100, 200, 300, 400};
static const int info_simulate[4]        = {5, 5, 5, 5};
static const int info_simulate_version[4] = {7, 14, 21, 28};
static const int info_cray[4]             = {9, 9, 9, 9};
static const int info_language_standard_default[4] = {2, 3, 4, 5};
static const int info_language_extensions_default[4] = {0, 1, 0, 1};

// The focal logic rewritten for testability (runtime-controlled branches).
int compute_require_runtime(
    int argc,
    bool include_version_major,
    bool include_version_internal,
    bool include_simulate,
    bool include_simulate_version,
    bool include_cray)
{
    // Guard against out-of-range argc (mimicking typical usage where argc in {0..3}).
    if (argc < 0 || argc >= 4) {
        return -1; // Indicate invalid input for test purposes.
    }

    int require = 0;
    require += info_compiler[argc];
    require += info_platform[argc];
    require += info_arch[argc];

    // Optional compile-time guarded branches simulated via runtime flags.
    if (include_version_major) {
        require += info_version[argc];
    }
    if (include_version_internal) {
        require += info_version_internal[argc];
    }
    if (include_simulate) {
        require += info_simulate[argc];
    }
    if (include_simulate_version) {
        require += info_simulate_version[argc];
    }
    if (include_cray) {
        require += info_cray[argc];
    }

    // Always-present predicates in the original code.
    require += info_language_standard_default[argc];
    require += info_language_extensions_default[argc];

    return require;
}

// Manual expected computations to validate compute_require_runtime without relying on
// the exact same code path (helps verify indexing and sums independently).
int manualComputeNoOption(int argc) {
    if (argc < 0 || argc >= 4) return -1;
    return info_compiler[argc]
         + info_platform[argc]
         + info_arch[argc]
         + info_language_standard_default[argc]
         + info_language_extensions_default[argc];
}

int manualComputeAllOptions(int argc) {
    if (argc < 0 || argc >= 4) return -1;
    return info_compiler[argc]
         + info_platform[argc]
         + info_arch[argc]
         + info_version[argc]
         + info_version_internal[argc]
         + info_simulate[argc]
         + info_simulate_version[argc]
         + info_cray[argc]
         + info_language_standard_default[argc]
         + info_language_extensions_default[argc];
}

int manualComputeMixed(int argc,
                      bool maj,
                      bool vint,
                      bool sim,
                      bool simv,
                      bool cray) {
    if (argc < 0 || argc >= 4) return -1;
    int sum = 0;
    sum += info_compiler[argc];
    sum += info_platform[argc];
    sum += info_arch[argc];
    if (maj)      sum += info_version[argc];
    if (vint)     sum += info_version_internal[argc];
    if (sim)      sum += info_simulate[argc];
    if (simv)     sum += info_simulate_version[argc];
    if (cray)     sum += info_cray[argc];
    sum += info_language_standard_default[argc];
    sum += info_language_extensions_default[argc];
    return sum;
}

// Lightweight test harness to accumulate results without terminating on first failure.
struct TestHarness {
    struct Result { std::string name; bool passed; std::string detail; };
    std::vector<Result> results;

    void add(const std::string& name, bool passed, const std::string& detail = "") {
        results.push_back({name, passed, detail});
    }

    void report() const {
        int total = (int)results.size();
        int ok = 0;
        for (const auto& r : results) {
            if (r.passed) ++ok;
        }
        std::cout << "Test results: " << ok << " / " << total << " passed\n";
        for (const auto& r : results) {
            std::cout << (r.passed ? "[PASS] " : "[FAIL] ")
                      << r.name << (r.detail.empty() ? "" : " - " + r.detail) << "\n";
        }
    }
};

// Public entry for test execution
void runAllTests() {
    TestHarness th;

    // Test 1: No optional branches activated (simulate true branch off for all optional features).
    // Coverage: ensure basic operands are summed and arrays are indexed with argc.
    {
        int argc_vals[] = {0, 1, 2, 3};
        bool fail = false;
        for (int argc : argc_vals) {
            int got = compute_require_runtime(argc,
                                             false, // include_version_major
                                             false, // include_version_internal
                                             false, // include_simulate
                                             false, // include_simulate_version
                                             false  // include_cray
                                             );
            int expected = manualComputeNoOption(argc);
            if (got != expected) {
                fail = true;
                th.add("Test1_noOption(argc=" + std::to_string(argc) + ")", false,
                       "got=" + std::to_string(got) + " expected=" + std::to_string(expected));
                break;
            }
        }
        th.add("Test1_NoOption_AllIndices", !fail);
    }

    // Test 2: All optional branches activated.
    // Coverage: exercise sums of all arrays (version, internal, simulate, simulate_version, cray).
    {
        int argc_vals[] = {0, 1, 2, 3};
        bool fail = false;
        for (int argc : argc_vals) {
            int got = compute_require_runtime(argc,
                                             true,  // include_version_major
                                             true,  // include_version_internal
                                             true,  // include_simulate
                                             true,  // include_simulate_version
                                             true   // include_cray
                                             );
            int expected = manualComputeAllOptions(argc);
            if (got != expected) {
                fail = true;
                th.add("Test2_AllOptions(argc=" + std::to_string(argc) + ")", false,
                       "got=" + std::to_string(got) + " expected=" + std::to_string(expected));
                break;
            }
        }
        th.add("Test2_AllOptions_AllIndices", !fail);
    }

    // Test 3: Mixed branches to exercise partial inclusion.
    // Include version_major and simulate, exclude others in this particular mix.
    {
        int argc_vals[] = {0, 1, 2, 3};
        bool fail = false;
        for (int argc : argc_vals) {
            int got = compute_require_runtime(argc,
                                             true,   // include_version_major
                                             false,  // include_version_internal
                                             true,   // include_simulate
                                             false,  // include_simulate_version
                                             false   // include_cray
                                             );
            int expected = manualComputeMixed(argc, true, false, true, false, false);
            if (got != expected) {
                fail = true;
                th.add("Test3_Mixed(argc=" + std::to_string(argc) + ")", false,
                       "got=" + std::to_string(got) + " expected=" + std::to_string(expected));
                break;
            }
        }
        th.add("Test3_MixedBranches_maj_and_sim", !fail);
    }

    // Test 4: Boundary condition check with argc at upper bound (3).
    // Ensure indexing uses correct elements and returns consistent results.
    {
        int argc = 3;
        int got = compute_require_runtime(argc,
                                         false, // include_version_major
                                         false, // include_version_internal
                                         false, // include_simulate
                                         false, // include_simulate_version
                                         false  // include_cray
                                         );
        int expected = manualComputeNoOption(argc);
        th.add("Test4_ArgBoundary_3_NoOptional", got == expected,
               "argc=3 got=" + std::to_string(got) + " expected=" + std::to_string(expected));
    }

    // Report results
    th.report();
}

} // namespace test_framework

// Entrypoint
int main(int argc, char* argv[]) {
    // Run the lightweight test suite.
    test_framework::runAllTests();
    return 0;
}