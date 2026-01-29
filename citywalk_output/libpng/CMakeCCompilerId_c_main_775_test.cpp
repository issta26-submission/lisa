// Lightweight C++11 unit test for the focal method (CMakeCCompilerId.c style main)
// This test suite does not rely on Google Test and uses a small custom assertion framework.

#include <string>
#include <iostream>
#include <vector>


// Simple lightweight test framework (non-terminating assertions)
namespace TestFramework {
    static int g_failures = 0;

    inline void log_failure(const std::string& file, int line, const std::string& msg) {
        std::cerr << "TEST FAILURE: " << file << ":" << line << " - " << msg << std::endl;
        ++g_failures;
    }

    #define LOG_FAIL(msg) TestFramework::log_failure(__FILE__, __LINE__, msg)

    // Non-terminating assertions
    #define EXPECT_EQ(a, b) do { \
        auto _a = (a); \
        auto _b = (b); \
        if (!(_a == _b)) { \
            LOG_FAIL("EXPECT_EQ failed: " + std::string(#a) + " == " + std::string(#b) + " (actual: " + std::to_string(_a) + ", expected: " + std::to_string(_b) + ")"); \
        } \
    } while (false)

    #define EXPECT_TRUE(cond) do { \
        if (!(cond)) { \
            LOG_FAIL("EXPECT_TRUE failed: " #cond); \
        } \
    } while (false)
}

// Domain-specific test scaffolding to mimic the focal logic
// The original focal method sums many info_* arrays at index argc, with several compile-time branches.
// We simulate those branches using template parameters (compile-time booleans) to cover both true/false branches.

// A static container of test data (emulating the "static class members" requirement)
struct Info {
    static const int N = 8;
    // Static members representing the various info_* arrays used by the focal method
    static int info_compiler[N];
    static int info_platform[N];
    static int info_arch[N];
    static int info_version[N];
    static int info_version_internal[N];
    static int info_simulate[N];
    static int info_simulate_version[N];
    static int info_cray[N];
    static int info_language_standard_default[N];
    static int info_language_extensions_default[N];
};

// Define and initialize the static arrays with deterministic values.
// We choose values 1..N for each array to simplify expected calculation:
// At index i, each array has value i+1.
int Info::info_compiler[N]                 = {1,2,3,4,5,6,7,8};
int Info::info_platform[N]                 = {1,2,3,4,5,6,7,8};
int Info::info_arch[N]                     = {1,2,3,4,5,6,7,8};
int Info::info_version[N]                  = {1,2,3,4,5,6,7,8};
int Info::info_version_internal[N]         = {1,2,3,4,5,6,7,8};
int Info::info_simulate[N]                 = {1,2,3,4,5,6,7,8};
int Info::info_simulate_version[N]         = {1,2,3,4,5,6,7,8};
int Info::info_cray[N]                     = {1,2,3,4,5,6,7,8};
int Info::info_language_standard_default[N]= {1,2,3,4,5,6,7,8};
int Info::info_language_extensions_default[N] = {1,2,3,4,5,6,7,8};

// Core focal logic re-implemented as a templated function to exercise both branches
// MAJOR   -> include info_version
// INTERNAL -> include info_version_internal
// SIM_ID  -> include info_simulate
// SIM_VER -> include info_simulate_version
// CRAY    -> include info_cray
template<
    bool MAJOR, bool INTERNAL, bool SIM_ID, bool SIM_VER, bool CRAY
>
int focal_compute_template(
    int argc,
    const int* info_compiler,
    const int* info_platform,
    const int* info_arch,
    const int* info_version,
    const int* info_version_internal,
    const int* info_simulate,
    const int* info_simulate_version,
    const int* info_cray,
    const int* info_language_standard_default,
    const int* info_language_extensions_default
) {
    int require = 0;
    require += info_compiler[argc];
    require += info_platform[argc];
    require += info_arch[argc];
    if (MAJOR)       require += info_version[argc];
    if (INTERNAL)    require += info_version_internal[argc];
    if (SIM_ID)      require += info_simulate[argc];
    if (SIM_VER)     require += info_simulate_version[argc];
    if (CRAY)        require += info_cray[argc];
    require += info_language_standard_default[argc];
    require += info_language_extensions_default[argc];
    (void)argc; // mimic (void)argv usage from the original code
    return require;
}

// Test helpers
void test_all_true_branches() {
    // All compile-time branches enabled
    // Expectation: include all 10 arrays in the sum
    // At index argc, each array value is (argc+1); thus expected = 10 * (argc+1)
    for (int argc = 0; argc < Info::N; ++argc) {
        int val = focal_compute_template<true, true, true, true, true>(
            argc,
            Info::info_compiler,
            Info::info_platform,
            Info::info_arch,
            Info::info_version,
            Info::info_version_internal,
            Info::info_simulate,
            Info::info_simulate_version,
            Info::info_cray,
            Info::info_language_standard_default,
            Info::info_language_extensions_default
        );
        int expected = 10 * (argc + 1);
        TestFramework::EXPECT_EQ(val, expected);
    }
}

// Test with all compile-time branches disabled (minimum contributions)
void test_all_false_branches() {
    // All compile-time branches disabled
    // Expectation: only 3 base arrays + 2 language arrays = 5 arrays included
    // At index argc, expected = 5 * (argc+1)
    for (int argc = 0; argc < Info::N; ++argc) {
        int val = focal_compute_template<false, false, false, false, false>(
            argc,
            Info::info_compiler,
            Info::info_platform,
            Info::info_arch,
            Info::info_version,
            Info::info_version_internal,
            Info::info_simulate,
            Info::info_simulate_version,
            Info::info_cray,
            Info::info_language_standard_default,
            Info::info_language_extensions_default
        );
        int expected = 5 * (argc + 1);
        TestFramework::EXPECT_EQ(val, expected);
    }
}

// Mixed branches to cover partial inclusion patterns
void test_mixed_branches() {
    // MAJOR true, INTERNAL false, SIM_ID false, SIM_VER false, CRAY false
    // Included: comp, platform, arch, version, languages -> 6 arrays
    for (int argc = 0; argc < Info::N; ++argc) {
        int val = focal_compute_template<true, false, false, false, false>(
            argc,
            Info::info_compiler,
            Info::info_platform,
            Info::info_arch,
            Info::info_version,
            Info::info_version_internal,
            Info::info_simulate,
            Info::info_simulate_version,
            Info::info_cray,
            Info::info_language_standard_default,
            Info::info_language_extensions_default
        );
        int expected = 6 * (argc + 1);
        TestFramework::EXPECT_EQ(val, expected);
    }

    // MAJOR false, INTERNAL true, SIM_ID true, SIM_VER true, CRAY true
    // Included: comp, platform, arch, version_internal, simulate, simulate_version, cray, languages -> 7 arrays
    for (int argc = 0; argc < Info::N; ++argc) {
        int val = focal_compute_template<false, true, true, true, true>(
            argc,
            Info::info_compiler,
            Info::info_platform,
            Info::info_arch,
            Info::info_version,
            Info::info_version_internal,
            Info::info_simulate,
            Info::info_simulate_version,
            Info::info_cray,
            Info::info_language_standard_default,
            Info::info_language_extensions_default
        );
        int expected = 7 * (argc + 1);
        TestFramework::EXPECT_EQ(val, expected);
    }
}

// Test static member access via class name to satisfy static member handling requirement
void test_static_member_access() {
    // Access static member using the class name and verify it holds expected values
    // For argc=3, info_compiler[3] == 4 (since arrays defined as 1..N)
    int idx = 3;
    int val = Info::info_compiler[idx];
    EXPECT_EQ(val, 4);
    // Similarly verify a couple more indexes to increase coverage
    EXPECT_EQ(Info::info_platform[idx], 4);
    EXPECT_EQ(Info::info_language_standard_default[idx], 4);
}

// Run all tests
void run_all_tests() {
    test_all_true_branches();
    test_all_false_branches();
    test_mixed_branches();
    test_static_member_access();
}

// Entry point
int main(int argc, char* argv[]) {
    // (argv) is unused in the focal logic; just silence unused warning here if needed
    (void)argc;
    (void)argv;

    run_all_tests();

    if (TestFramework::g_failures != 0) {
        std::cerr << "Summary: " << TestFramework::g_failures << " test(s) failed." << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}