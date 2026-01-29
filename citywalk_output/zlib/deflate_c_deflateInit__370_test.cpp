// High-quality C++11 unit test suite for the focal method: deflateInit_
// Focus: deflateInit_(z_streamp strm, int level, const char *version, int stream_size)
// This test suite follows the provided guidance to reason about dependencies
// and generate a structured, executable test harness without using GTest.
// Note: The tests rely on the project’s deflate.h and related C code being
// linkable in the test environment. The tests use a lightweight in-house
// testing framework to maximize coverage and experimentation without
// terminating on first failure.

#include <cstring>
#include <string>
#include <deflate.h>
#include <functional>
#include <iostream>
#include <vector>
#include <memory>


// Include the project headers that declare deflateInit_ and the zlib-like types.
// It is expected that deflate.h (and related headers) are resolvable by the build system.

// Ensure the C-linkage for the focal function to avoid name mangling issues when linking.
extern "C" {
    int ZEXPORT deflateInit_(z_streamp strm, int level, const char *version, int stream_size);
    // The following is typically declared in zlib-like libraries; we guard its use.
    int ZEXPORT deflateEnd(z_streamp strm);
}

// Lightweight, in-house test harness (no GTest/GMock). Minimal assertions that do not
// terminate unless explicitly desired, enabling maximal code execution for coverage.
namespace TestFramework {

struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};

class TestRunner {
public:
    using TestFn = std::function<void(std::vector<TestResult>&)>;

    void addTest(const std::string& name, TestFn fn) {
        tests_.emplace_back(name, fn);
    }

    int run() {
        int passed = 0;
        int failed = 0;
        std::vector<TestResult> results;
        for (auto& t : tests_) {
            std::vector<TestResult> single;
            // Execute test, capturing per-test results
            t.fn(single);
            // Aggregate
            for (auto& r : single) {
                results.push_back(r);
                if (r.passed) ++passed;
                else ++failed;
            }
        }

        // Summary
        std::cout << "Test Summary: " << passed << " passed, " << failed << " failed.\n";
        for (auto& r : results) {
            std::cout << (r.passed ? "[PASSED] " : "[FAILED] ") << r.name;
            if (!r.message.empty()) std::cout << " - " << r.message;
            std::cout << "\n";
        }
        return (failed == 0) ? 0 : -1;
    }

    // Helper: create a per-test test function easily
    static TestFn makeTest(std::function<void(std::vector<TestResult>&)> f) {
        return f;
    }

private:
    std::vector<std::pair<std::string, TestFn>> tests_;
};

} // namespace TestFramework

// Candidate Keywords gathered from Step 1 (for reference in code comments)
// - deflateInit_               : focal wrapper function under test
// - deflateInit2_              : forwarder called by deflateInit_
// - z_streamp                   : pointer to zlib stream/state structure
// - version                     : user-supplied version string
// - stream_size                 : user-supplied stream size hint
// - Z_DEFLATED, MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY
// - deflateStateCheck, deflateInit2_ : internal dependencies (documented in test comments)
// - Z_OK-like return semantics     (assumed non-negative on success)
// The tests do not mock private/static functions in this C file; they exercise the public API.

// Helper to create and clean a z_stream object safely.
// The exact layout of z_stream is defined by the project headers. We rely on the
// project type and constructor semantics to work with real build environments.
static z_streamp create_zstream_for_test() {
    // Allocate a z_stream-like object. If the project defines z_stream as a struct
    // type, this will allocate correctly in a C++ translation unit.
    // We zero-initialize to start from a clean state.
    z_streamp s = nullptr;
    // Try to allocate using 'new' to respect C++ environment; cast to z_streamp.
    try {
        s = new z_stream(); // z_stream is expected to be defined by deflate.h
        std::memset(s, 0, sizeof(z_stream));
    } catch (...) {
        s = nullptr;
    }
    return s;
}

// Helper to safely destroy a z_streamp allocated by create_zstream_for_test
static void destroy_zstream_for_test(z_streamp s) {
    if (s) {
        // If the library exposes deflateEnd, use it to gracefully release internal resources.
        deflateEnd(s); // Ignore potential errors; test harness focuses on init path
        // Free the allocated memory (if deflateEnd did not fully reclaim it)
        delete s;
    }
}

// Domain knowledge-based tests for deflateInit_.
// Test 1: Basic, typical usage with a common deflate level and valid version string.
// Expectation: deflateInit_ should succeed (return 0 in typical zlib-like semantics) and
// the stream should be initializable without crashing.
static void test_deflateInit_basic_valid(std::vector<TestFramework::TestResult>& results) {
    std::string testName = "deflateInit_basic_valid";
    z_streamp strm = create_zstream_for_test();
    int level = 6;
    const char *version = "1.2.11";
    int stream_size = 0;

    if (!strm) {
        results.push_back({testName, false, "Failed to allocate z_stream for test."});
        return;
    }

    int ret = deflateInit_(strm, level, version, stream_size);
    bool ok = (ret == 0); // Expect Z_OK (0) for success in typical zlib semantics
    destroy_zstream_for_test(strm);

    if (ok) {
        results.push_back({testName, true, ""});
    } else {
        results.push_back({testName, false, "deflateInit_ did not return success (expected 0). Actual: " + std::to_string(ret)});
    }
}

// Test 2: Boundary levels (minimum and maximum) to ensure the wrapper forwards correctly.
// We expect success for typical deflate levels (0..9) depending on library policy.
static void test_deflateInit_level_boundaries(std::vector<TestFramework::TestResult>& results) {
    std::string testName = "deflateInit_level_boundaries";
    z_streamp strm = create_zstream_for_test();
    if (!strm) {
        results.push_back({testName, false, "Failed to allocate z_stream for test."});
        return;
    }

    const char *version = "1.2.11";
    int stream_size = 0;
    bool all_ok = true;
    for (int level : {0, 9}) {
        int ret = deflateInit_(strm, level, version, stream_size);
        bool ok = (ret == 0);
        if (!ok) {
            all_ok = false;
            results.push_back({testName, false,
                "Level " + std::to_string(level) + " failed with ret=" + std::to_string(ret)});
        }
        // Clean between attempts to avoid residual state
        deflateEnd(strm);
    }

    destroy_zstream_for_test(strm);
    if (all_ok) {
        results.push_back({testName, true, ""});
    }
}

// Test 3: Different version strings should not crash and should be accepted by the wrapper.
// We verify that multiple non-null version strings still yield a valid init call.
static void test_deflateInit_variant_versions(std::vector<TestFramework::TestResult>& results) {
    std::string testName = "deflateInit_variant_versions";
    z_streamp strm = create_zstream_for_test();
    if (!strm) {
        results.push_back({testName, false, "Failed to allocate z_stream for test."});
        return;
    }

    const char *versions[] = { "1.2.11", "1.3.0", "0.0", "9.9" };
    int stream_size = 0;
    bool all_ok = true;
    for (const char* v : versions) {
        int ret = deflateInit_(strm, 6, v, stream_size);
        bool ok = (ret == 0);
        if (!ok) {
            all_ok = false;
            results.push_back({testName, false,
                std::string("Version '") + v + "' init failed with ret=" + std::to_string(ret)});
        }
        deflateEnd(strm);
    }

    destroy_zstream_for_test(strm);
    if (all_ok) {
        results.push_back({testName, true, ""});
    }
}

// Test 4: Edge-case: null version pointer.
// Some implementations may treat nullptr version as invalid; ensure we handle gracefully.
static void test_deflateInit_null_version(std::vector<TestFramework::TestResult>& results) {
    std::string testName = "deflateInit_null_version";
    z_streamp strm = create_zstream_for_test();
    if (!strm) {
        results.push_back({testName, false, "Failed to allocate z_stream for test."});
        return;
    }

    int level = 5;
    const char *version = nullptr;
    int stream_size = 0;

    int ret = deflateInit_(strm, level, version, stream_size);
    bool ok = (ret == 0); // Depending on library, this could be allowed or fail; we tolerate both outcomes but record it
    // Do not crash; still attempt to clean up
    deflateEnd(strm);
    destroy_zstream_for_test(strm);

    if (ok) {
        results.push_back({testName, true, "Initialization succeeded with null version pointer (allowed)."});
    } else {
        results.push_back({testName, true, "Initialization returned error for null version pointer (recorded as expected in some environments)."});
    }
}

// Test 5: stream_size influence check (pass non-zero to verify wrapper forwards value).
static void test_deflateInit_with_stream_size(std::vector<TestFramework::TestResult>& results) {
    std::string testName = "deflateInit_with_stream_size";
    z_streamp strm = create_zstream_for_test();
    if (!strm) {
        results.push_back({testName, false, "Failed to allocate z_stream for test."});
        return;
    }

    int level = 6;
    const char *version = "1.2.11";
    int stream_size = 1024; // non-zero

    int ret = deflateInit_(strm, level, version, stream_size);
    bool ok = (ret == 0);
    deflateEnd(strm);
    destroy_zstream_for_test(strm);

    if (ok) {
        results.push_back({testName, true, ""});
    } else {
        results.push_back({testName, false,
            "deflateInit_ with non-zero stream_size failed with ret=" + std::to_string(ret)});
    }
}

// Main entry: assemble and run all tests.
int main() {
    TestFramework::TestRunner runner;

    // Register tests with descriptive names and inline-lambda wrappers.
    runner.addTest("deflateInit_basic_valid", TestFramework::makeTest(test_deflateInit_basic_valid));
    runner.addTest("deflateInit_level_boundaries", TestFramework::makeTest(test_deflateInit_level_boundaries));
    runner.addTest("deflateInit_variant_versions", TestFramework::makeTest(test_deflateInit_variant_versions));
    runner.addTest("deflateInit_null_version", TestFramework::makeTest(test_deflateInit_null_version));
    runner.addTest("deflateInit_with_stream_size", TestFramework::makeTest(test_deflateInit_with_stream_size));

    int result = runner.run();
    // Return 0 on success (all tests passed), -1 otherwise.
    return result;
}

// End of test suite.
// Each test case above contains inline comments explaining the rationale, expectations,
// and steps to maximize coverage while staying close to the focal function's behavior.
// The tests assume the presence of the project's deflate.h and the corresponding C sources
// (deflate.c, etc.) to provide the actual implementation of deflateInit_, deflateInit2_, and
// the zlib-like types (z_streamp, z_stream, etc.).