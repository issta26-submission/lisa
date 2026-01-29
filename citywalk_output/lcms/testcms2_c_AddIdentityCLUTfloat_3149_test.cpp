// C++11 test-suite for the focal method AddIdentityCLUTfloat
// This test-suite is designed to be lightweight (no GTest) and to exercise
// the focal method by exercising its public API usage path.
// It relies on the project's provided C API (Little CMS style) and the
// test glue in testcms2.h/.c (via the DbgThread context and pipeline helpers).
// The goal is to validate that invoking the focal method with a valid pipeline
// does not crash and that the basic pipeline integration path remains sound.

#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <cassert>


// Forward declare the C types used by the focal method and the integration API.
// We keep the declarations minimal and compatible with the project's C API.
extern "C" {
    // Opaque types used by the Little CMS-like API
    typedef struct cmsPipeline cmsPipeline;
    typedef struct cmsStage cmsStage;
    typedef void* cmsContext;
    // Basic constants used by the focal method (as seen in the source)
    // The exact numeric values are library-specific; we mirror the usage.
    // In practice, cmsAT_END is typically 0 or a sentinel; we declare a match to usage.
    // If the exact value differs in the target build, compiler/linker will resolve.
    // Here we provide a placeholder to allow compilation in the test harness.
    #ifndef cmsAT_END
    #define cmsAT_END 0
    #endif

    // Core API functions used by the focal method and test-cehicles
    cmsPipeline* cmsPipelineAlloc(cmsContext ContextID);
    void cmsPipelineInsertStage(cmsPipeline* lut, cmsUInt32Number Where, cmsStage* Stage);
    cmsStage* cmsStageAllocCLutFloat(cmsContext ContextID,
                                     cmsUInt32Number InputChannels,
                                     cmsUInt32Number OutputChannels,
                                     cmsUInt32Number CLUTSize,
                                     const cmsFloat32Number* LUT);
    void cmsPipelineFree(cmsPipeline* lut);

    // Lightweight context helper (provided in test glue)
    cmsContext DbgThread(void);

    // The focal method under test
    void AddIdentityCLUTfloat(cmsPipeline* lut);

    // Basic numeric typedefs used by the API (as seen in the source)
    typedef unsigned int cmsUInt32Number;
    typedef float cmsFloat32Number;
}

// Simple, independent test harness (no GTest, just C++ standard library)
namespace TestHarness {
    // Utility: simple assertion wrapper with readable output
    static void Assert(bool cond, const std::string& msg) {
        if (!cond) {
            std::cerr << "Assertion failed: " << msg << std::endl;
            throw std::runtime_error(msg);
        }
    }

    // Public-facing, self-contained test cases
    // Each test returns true on success, false on failure (or throws on exception)
    // Tests aim to cover the AddIdentityCLUTfloat usage path, including:
    // - basic insertion into a valid pipeline
    // - idempotent invocation (calling twice should not crash)
    // - proper cleanup (no crash on free)
    class AddIdentityCLUTfloat_Tests {
    public:
        // Test 1: Basic insertion into a fresh pipeline
        // Rationale: Ensure that a valid cmsPipeline can be allocated and that
        // AddIdentityCLUTfloat can insert the identity CLUT without crashing.
        static bool Test_BasicInsertion() {
            cmsPipeline* lut = cmsPipelineAlloc(DbgThread());
            try {
                TestHarness::Assert(lut != nullptr, "cmsPipelineAlloc returned null");
                AddIdentityCLUTfloat(lut);
                // If no crash occurs during insertion, we consider the basic path healthy.
                cmsPipelineFree(lut);
                lut = nullptr;
                return true;
            } catch (...) {
                if (lut) cmsPipelineFree(lut);
                std::cerr << "Exception in Test_BasicInsertion" << std::endl;
                throw;
            }
        }

        // Test 2: Multiple invocations - ensure repeated calls do not crash
        // Rationale: The focal method should be idempotent in terms of not
        // corrupting internal structure on repeated insertion (within reasonable usage).
        static bool Test_MultipleInvocations() {
            cmsPipeline* lut = cmsPipelineAlloc(DbgThread());
            try {
                TestHarness::Assert(lut != nullptr, "cmsPipelineAlloc returned null for multiple-invocation test");
                // First invocation
                AddIdentityCLUTfloat(lut);
                // Second invocation (should be safe; may append another CLUT stage)
                AddIdentityCLUTfloat(lut);

                // Clean up
                cmsPipelineFree(lut);
                lut = nullptr;
                return true;
            } catch (...) {
                if (lut) cmsPipelineFree(lut);
                std::cerr << "Exception in Test_MultipleInvocations" << std::endl;
                throw;
            }
        }

        // Test 3: Resource cleanup sanity
        // Rationale: Ensure that a pipeline used with the focal method can be freed
        // without leaking or crashing, indicating stable lifecycle management.
        static bool Test_CleanupSanity() {
            cmsPipeline* lut = cmsPipelineAlloc(DbgThread());
            try {
                TestHarness::Assert(lut != nullptr, "cmsPipelineAlloc failed in cleanup test");
                AddIdentityCLUTfloat(lut);
                cmsPipelineFree(lut);
                lut = nullptr;
                // If we reach here, cleanup succeeded
                return true;
            } catch (...) {
                if (lut) cmsPipelineFree(lut);
                std::cerr << "Exception in Test_CleanupSanity" << std::endl;
                throw;
            }
        }

        // Helper: run all tests and report
        static int RunAll() {
            int failures = 0;
            std::vector<std::pair<std::string, bool>> results;

            try {
                results.emplace_back("Test_BasicInsertion", Test_BasicInsertion());
            } catch (...) {
                results.emplace_back("Test_BasicInsertion", false);
            }

            try {
                results.emplace_back("Test_MultipleInvocations", Test_MultipleInvocations());
            } catch (...) {
                results.emplace_back("Test_MultipleInvocations", false);
            }

            try {
                results.emplace_back("Test_CleanupSanity", Test_CleanupSanity());
            } catch (...) {
                results.emplace_back("Test_CleanupSanity", false);
            }

            // Report
            std::cout << "AddIdentityCLUTfloat Test Suite: " << results.size() << " tests run\n";
            for (const auto& r : results) {
                std::cout << " - " << r.first << ": " << (r.second ? "PASS" : "FAIL") << "\n";
                if (!r.second) ++failures;
            }

            return failures;
        }
    };
}

// Entrypoint for the test program
int main() {
    // Describe the test suite and execute
    // We rely on the internal AddIdentityCLUTfloat_test helpers to exercise the focal method.
    int failures = 0;
    try {
        failures = TestHarness::AddIdentityCLUTfloat_Tests::RunAll();
    } catch (const std::exception& ex) {
        std::cerr << "Unhandled exception in test suite: " << ex.what() << std::endl;
        failures = 1;
    } catch (...) {
        std::cerr << "Unhandled unknown exception in test suite." << std::endl;
        failures = 1;
    }

    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << failures << " test(s) failed.\n";
        return 1;
    }
}