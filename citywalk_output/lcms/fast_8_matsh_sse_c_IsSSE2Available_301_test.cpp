/*
  Automated unit test suite for the focal method:
  cmsBool IsSSE2Available(void)

  Summary of approach:
  - Provide a lightweight, self-contained C++11 test harness (no GTest) that calls the
    focal C function IsSSE2Available() (declared with C linkage).
  - Build-time platform differences:
      - On Linux, test the function's reported SSE2 capability against actual CPU flags
        parsed from /proc/cpuinfo to exercise true/false branches as much as possible
        given runtime hardware. This validates correctness against the real hardware feature.
      - On non-Linux platforms, we still verify basic invariants (type, range) and
        repeatability to cover function usage without requiring a specific CPU feature.
  - Coverage goals:
      - True/false branch coverage is approached by comparing with the host CPU flags
        on Linux (best-effort true/false coverage across runs and environments).
      - Verify deterministic behavior by repeated calls.
      - Ensure returned value is a valid boolean-like integer (0 or 1).

  Important notes:
  - The test does not rely on GTest or any non-standard framework.
  - The test uses only the C++ standard library (plus the pre-existing focal C function).
  - Static members or private/internal details of the focal module are not accessed directly.
  - This test file should be compiled and linked with the C source file containing:
      fast_8_matsh_sse.c (which provides IsSSE2Available)

  Candidate Keywords (Step 1): IsSSE2Available, SSE2, cpuid, __cpuid, __cpuid_count, _MSC_VER, SSE2Available,
  cpuinfo, __get_cpuid_max, intrin, XMM, XMM2, cmsBool, boolean flag, runtime CPU features
*/

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <cpuid.h>


// The focal function is provided in C. We declare it with C linkage for linking correctness.
extern "C" int IsSSE2Available(void);

// Lightweight helper to determine SSE2 presence from Linux /proc/cpuinfo.
// Returns true if the "flags" line for any CPU contains "sse2".
#ifdef __linux__
static bool cpuSse2FlagFromProc() {
    FILE* f = std::fopen("/proc/cpuinfo", "r");
    if (!f) return false; // If we can't read the CPU info, conservatively assume false.
    char line[1024];
    bool found = false;
    while (std::fgets(line, sizeof(line), f)) {
        // Normalize to lower-case for robust substring search
        char* ptr = line;
        // Look for a "flags" line
        if (std::strncasecmp(line, "flags", 5) == 0) {
            // The line format is: flags : ... space-separated flags ...
            char* colon = std::strchr(line, ':');
            if (colon) {
                ++colon;
                // Point to the flags segment after the colon
                // Simple search for "sse2" token case-insensitive
                if (std::strcasestr(colon, "sse2") != nullptr) {
                    found = true;
                    break;
                }
            }
        }
    }
    std::fclose(f);
    return found;
}
#else
// On non-Linux platforms, we can't reliably inspect /proc/cpuinfo.
static bool cpuSse2FlagFromProc() {
    return false;
}
#endif

struct TestCase {
    std::string name;
    std::function<void(void)> run;
    bool passed;
    std::string message;
};

// Minimal test harness (non-GTest) with explanations for each test.
int main() {
    std::vector<TestCase> tests;

    // Test 1: Linux environment – validate SSE2 availability against actual /proc/cpuinfo flags
    // This exercises true/false branches as reported by the host CPU, within the constraints of
    // the runtime hardware. If the CPU supports SSE2, IsSSE2Available should return non-zero.
    tests.push_back({
        "CPU-SSE2-Flag-Consistency-Linux",
        []() {
            // Only run this check on Linux; otherwise skip gracefully.
#ifdef __linux__
            bool expected = cpuSse2FlagFromProc();
            int actual = IsSSE2Available();
            bool actualBool = (actual != 0);
            if (actualBool != expected) {
                throw std::runtime_error("IsSSE2Available() result (" +
                                         std::to_string(actual) +
                                         ") does not match /proc/cpuinfo SSE2 flag (" +
                                         (expected ? "true" : "false") + ").");
            }
#else
            // If not Linux, skip with a non-failing message
#endif
        },
        true,
        ""
    });

    // Test 2: Repeatability – calling the function twice should yield consistent results
    // This ensures no statefulness within the function and deterministic behavior.
    tests.push_back({
        "SSE2-Repeatability",
        []() {
            int first  = IsSSE2Available();
            int second = IsSSE2Available();
            if ((first != 0 && first != 1) || (second != 0 && second != 1)) {
                throw std::runtime_error("IsSSE2Available() returned non-boolean-like value: first="
                                         + std::to_string(first) + ", second=" + std::to_string(second));
            }
            if ((first != 0) != (second != 0)) {
                // If one call reports true and the other false, that's a genuine nondeterminism.
                throw std::runtime_error("IsSSE2Available() is nondeterministic: first=" +
                                         std::to_string(first) + ", second=" + std::to_string(second));
            }
        },
        true,
        ""
    });

    // Test 3: Sanity check – the function should always return 0 or 1 (boolean-like)
    tests.push_back({
        "SSE2-BooleanRangeSanity",
        []() {
            int val = IsSSE2Available();
            if (val != 0 && val != 1) {
                throw std::runtime_error("IsSSE2Available() returned invalid value: " + std::to_string(val)
                                         + " (expected 0 or 1).");
            }
        },
        true,
        ""
    });

    // Execute tests
    int total = static_cast<int>(tests.size());
    int passed = 0;
    for (auto &t : tests) {
        try {
            t.run();
            t.passed = true;
            ++passed;
        } catch (const std::exception &ex) {
            t.passed = false;
            t.message = ex.what();
        } catch (...) {
            t.passed = false;
            t.message = "Unknown exception during test.";
        }
    }

    // Reporting
    std::cout << "IsSSE2Available test suite results: " << passed << "/" << total << " tests passed.\n";
    for (const auto &t : tests) {
        std::cout << " - " << t.name << ": " << (t.passed ? "PASSED" : "FAILED");
        if (!t.passed && !t.message.empty()) {
            std::cout << " | Reason: " << t.message;
        }
        std::cout << "\n";
    }

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
} 

/*
Usage notes:
- Build as a single executable that links against fast_8_matsh_sse.c (the source containing IsSSE2Available).
- For Linux environments, this test compares the runtime SSE2 capability with /proc/cpuinfo flags to
  exercise the true/false branches as exercised by the hardware.
- For non-Linux environments, the test focuses on basic invariants (boolean-like return and repeatability).
- If a specific environment requires additional coverage (e.g., Windows), consider adding platform-specific
  tests under corresponding #ifdef blocks, but maintain the non-GTest, single-main structure.
*/