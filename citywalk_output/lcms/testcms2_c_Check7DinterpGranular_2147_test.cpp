// test_Check7DinterpGranular.cpp
// A lightweight C++11 test harness for the focal function Check7DinterpGranular
// This test does not rely on any external testing framework (GTest avoided).
// It uses only the C++ Standard Library and the provided testcms2.h interfaces.
//
// Step 1 (Program Understanding) summary (embedded as comments):
// - The focal method Check7DinterpGranular constructs a 7D LUT pipeline using
//   cmsPipelineAlloc and cmsStageAllocCLut16bitGranular with a 7x3x3x2x2x2x2
//   dimension grid. It samples the CLUT using Sampler7D, inserts the stage at the
//   beginning of the pipeline, and then checks several 7D input combinations
//   against CheckOne7D. If any comparison fails, it returns 0; otherwise it returns 1.
// - Key dependent components (candidates): cmsPipelineAlloc, cmsStageAllocCLut16bitGranular,
//   cmsStageSampleCLut16bit, cmsPipelineInsertStage, Sampler7D, CheckOne7D.
//
// Step 2 (Unit Test Generation) - tests to exercise the focal method and its
//         dependencies. Since the project provides test harness stubs in testcms2.h,
//         we will build two test cases to confirm basic behavior and stability.
// Step 3 (Test Case Refinement) - ensure tests are non-terminating (do not call
//         exit on failure) and collect results for reporting. We also test idempotence.

#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <iomanip>


#ifdef __cplusplus
extern "C" {
#endif
// Forward declare the focal function to ensure linkage with C code when compiled as C++
// If testcms2.h does not declare the function with C linkage, declare explicitly:
cmsInt32Number Check7DinterpGranular(void);
#ifdef __cplusplus
}
#endif

// In case testcms2.h does not declare the function due to packaging, provide a direct prototype:
#ifndef __has_include
#define HAS_INCLUDE(x) 0
#else
#define HAS_INCLUDE(x) __has_include(x)
#endif
#if !defined(Check7DinterpGranular) && defined(__cplusplus)
extern "C" cmsInt32Number Check7DinterpGranular(void);
#endif

// Lightweight test harness (non-terminating assertions)
class TestCase {
public:
    TestCase(const std::string& name) : name_(name), passed_(false), message_("") {}

    void SetResult(bool passed, const std::string& msg = "") {
        passed_ = passed;
        message_ = msg;
    }

    void Report(std::vector<std::string>& out_lines) const {
        std::ostringstream oss;
        oss << (passed_ ? "[PASS] " : "[FAIL] ") << name_;
        if (!message_.empty()) {
            oss << " - " << message_;
        }
        out_lines.push_back(oss.str());
    }

private:
    std::string name_;
    bool passed_;
    std::string message_;
};

// Helper: run a single test and collect result
static void RunTest(const std::string& name, bool condition, const std::string& note,
                    std::vector<std::string>& logs) {
    TestCase tc(name);
    if (condition) {
        tc.SetResult(true, note);
        logs.push_back("[PASS] " + name + (note.empty() ? "" : " - " + note));
    } else {
        tc.SetResult(false, note);
        logs.push_back("[FAIL] " + name + (note.empty() ? "" : " - " + note));
    }
    // Even on failure we do not abort; this is a non-terminating assertion approach.
}

// Test 1: Check7DinterpGranular returns success (1) under normal, expected conditions
// Rationale: Validate the focal method executes the 7D pipeline creation and checks
// against a set of 7D inputs. This covers the primary "success" path.
static void test_Check7DinterpGranular_ReturnsTrue(std::vector<std::string>& logs) {
    // Call the focal function through C linkage
    cmsInt32Number result = Check7DinterpGranular();
    bool passed = (result == 1);
    RunTest("Check7DinterpGranular_ReturnsTrue", passed,
            "Expected: non-zero (1) return indicating successful 7D interpolation check.",
            logs);
}

// Test 2: Check7DinterpGranular is idempotent across multiple invocations
// Rationale: Ensure repeated invocations do not crash and consistently return success.
// This exercises potential static/global state interactions within the test harness
// or the underlying 7D CLUT sampling pipeline.
static void test_Check7DinterpGranular_Idempotent(std::vector<std::string>& logs) {
    cmsInt32Number r1 = Check7DinterpGranular();
    cmsInt32Number r2 = Check7DinterpGranular();
    bool passed = (r1 == 1) && (r2 == 1);
    RunTest("Check7DinterpGranular_Idempotent", passed,
            "Both successive calls should return 1 indicating stable behavior.",
            logs);
}

// Entry point: run all tests and print a concise report
int main(int argc, char* argv[]) {
    (void)argc; (void)argv; // Silence unused parameter warnings

    std::vector<std::string> logs;

    // Run tests
    test_Check7DinterpGranular_ReturnsTrue(logs);
    test_Check7DinterpGranular_Idempotent(logs);

    // Print results
    std::cout << std::left;
    std::cout << "Test Suite: Check7DinterpGranular (C++)" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    for (const auto& line : logs) {
        std::cout << line << std::endl;
    }

    // Summary
    int fail_count = 0;
    for (const auto& line : logs) {
        if (line.find("[FAIL]") != std::string::npos) ++fail_count;
    }
    int total = static_cast<int>(logs.size());
    std::cout << "Summary: " << (total - fail_count) << " passed, " << fail_count << " failed, "
              << total << " total tests." << std::endl;

    // Return non-zero if any test failed to aid integration in CI or scripts
    return fail_count ? 1 : 0;
}