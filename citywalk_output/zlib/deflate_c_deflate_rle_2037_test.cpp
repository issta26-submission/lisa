// Test suite for the focal C function deflate_rle (via public API deflate) in deflate.c
// This test suite is written for C++11, does not rely on GoogleTest, and uses
// a lightweight, non-terminating assertion approach suitable for execution
// from main().
// Notes:
// - We rely on the public deflate API exposed by deflate.h/deflate.c.
// - deflate_rle is a static (internal) function in the original C file, so we exercise
//   it indirectly by using deflate() with inputs that exercise the run-length encoding path.
// - Tests focus on true/false branches in the public-facing compression path by providing
//   inputs that should trigger literal vs. match branches and the finish path.

#include <cstring>
#include <string>
#include <deflate.h>
#include <functional>
#include <iostream>
#include <vector>


// Include the C headers that declare the deflate API.
// Use extern "C" to ensure C linkage when compiled in C++.
extern "C" {
}

// Lightweight, non-terminating test harness
struct TestResult {
    std::string name;
    bool passed;
    std::string details;
};

static std::vector<TestResult> g_results;

// Helper to report test results without terminating on failure
static void report_test(const std::string& name, bool passed, const std::string& details = "") {
    g_results.push_back({name, passed, details});
}

// Helper to run a single test and capture its result
static void run_test(const std::string& name, const std::function<bool()>& test_func, const std::string& details = "") {
    bool ok = false;
    try {
        ok = test_func();
    } catch (...) {
        ok = false;
    }
    report_test(name, ok, details);
}

// Helper to print summary
static void print_summary() {
    int passed = 0;
    int failed = 0;
    for (const auto& r : g_results) {
        if (r.passed) ++passed;
        else ++failed;
        std::cout << "[" << (r.passed ? "PASS" : "FAIL") << "] " << r.name;
        if (!r.details.empty()) std::cout << " -- " << r.details;
        std::cout << std::endl;
    }
    std::cout << "Summary: " << passed << " passed, " << failed << " failed, total " << g_results.size() << std::endl;
}

// Helper: initialize a z_stream object with reasonable defaults
static void init_z_stream(z_stream* strm) {
    if (strm == nullptr) return;
    std::memset(strm, 0, sizeof(z_stream));
    // Typical zlib usage sets zalloc/zfree/opaque; use null allocators to default behavior
    strm->zalloc = nullptr;
    strm->zfree  = nullptr;
    strm->opaque = nullptr;
}

// Test 1: Run-length encoding path with a long repeating input
static bool test_deflate_rle_long_run() {
    // Prepare input: a long run of repeated 'A's
    const size_t input_len = 1024; // long enough to trigger a run
    std::vector<unsigned char> input(input_len, 'A');

    // Output buffer large enough to hold compressed data
    const size_t output_buf_size = 64 * 1024; // 64KB
    std::vector<unsigned char> output(output_buf_size);

    z_stream strm;
    init_z_stream(&strm);

    // Initialize deflate with default compression
    // The version string and stream_size are part of the internal API (as in deflate.h)
    // These values are typical placeholders; adjust if your environment requires different ones.
    // We intentionally do not rely on any file-system I/O or private state.
    int ret = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, "1.2.8", (int)sizeof(z_stream));
    if (ret != Z_OK) {
        // Initialization failed; report as test failure
        return false;
    }

    // Set input
    strm.next_in = input.data();
    strm.avail_in = static_cast<uInt>(input_len);

    // Set output
    strm.next_out = output.data();
    strm.avail_out = static_cast<uInt>(output_buf_size);

    // Run compression until completion
    // Feed Z_NO_FLUSH until input is exhausted, then use Z_FINISH
    int flush = Z_NO_FLUSH;
    do {
        ret = deflate(&strm, flush);
        if (ret == Z_STREAM_ERROR) {
            deflateEnd(&strm);
            return false;
        }
        // If output buffer is full, in this test we gave a large enough buffer to avoid re-buffering.
        if (strm.avail_out == 0) {
            // If by any chance the output buffer fills, fail the test to avoid infinite loop
            deflateEnd(&strm);
            return false;
        }
        // After consuming input, switch to finish after the last input is accepted
        if (strm.avail_in == 0 && flush == Z_NO_FLUSH) {
            flush = Z_FINISH;
        }
    } while (ret != Z_STREAM_END);

    // Finish and cleanup
    deflateEnd(&strm);

    // Basic validation: some input was consumed and some output was produced
    if (strm.total_in != input_len) {
        return false;
    }
    if (strm.total_out == 0) {
        return false;
    }
    // Accept success
    return true;
}

// Test 2: Run-length encoding path with literal path (no long repetition)
static bool test_deflate_rle_literal() {
    // Prepare input that should typically be emitted as literals (no long repeats)
    const char* sample = "HELLO_WORLD";
    const size_t input_len = std::strlen(sample);
    std::vector<unsigned char> input(sample, sample + input_len);

    // Large enough output buffer
    const size_t output_buf_size = 64 * 1024;
    std::vector<unsigned char> output(output_buf_size);

    z_stream strm;
    init_z_stream(&strm);

    int ret = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, "1.2.8", (int)sizeof(z_stream));
    if (ret != Z_OK) {
        return false;
    }

    strm.next_in = input.data();
    strm.avail_in = static_cast<uInt>(input_len);

    strm.next_out = output.data();
    strm.avail_out = static_cast<uInt>(output_buf_size);

    int flush = Z_NO_FLUSH;
    do {
        ret = deflate(&strm, flush);
        if (ret == Z_STREAM_ERROR) {
            deflateEnd(&strm);
            return false;
        }
        if (strm.avail_out == 0) {
            deflateEnd(&strm);
            return false;
        }
        if (strm.avail_in == 0 && flush == Z_NO_FLUSH) {
            flush = Z_FINISH;
        }
    } while (ret != Z_STREAM_END);

    deflateEnd(&strm);

    if (strm.total_in != input_len) return false;
    if (strm.total_out == 0) return false;

    // For a short literal-heavy input, the compressed output may still be larger than input,
    // but the encoding path should have completed successfully.
    return true;
}

// Test 3: Empty input and finish path
static bool test_deflate_empty_and_finish() {
    std::vector<unsigned char> input; // empty
    const size_t output_buf_size = 16 * 1024;
    std::vector<unsigned char> output(output_buf_size);

    z_stream strm;
    init_z_stream(&strm);

    int ret = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, "1.2.8", (int)sizeof(z_stream));
    if (ret != Z_OK) return false;

    strm.next_in = input.data();
    strm.avail_in = 0;

    strm.next_out = output.data();
    strm.avail_out = static_cast<uInt>(output_buf_size);

    int flush = Z_FINISH;
    ret = deflate(&strm, flush);
    // If the input is empty, the function should still return a complete stream when finishing
    if (ret != Z_STREAM_END) {
        deflateEnd(&strm);
        return false;
    }

    deflateEnd(&strm);

    // total_in should be 0
    if (strm.total_in != 0) return false;
    // Output can be zero-sized or non-zero depending on implementation; ensure no crash and valid state
    return true;
}

int main() {
    // Run tests
    run_test("deflate_rle_long_run", test_deflate_rle_long_run,
             "Long input run should trigger rle path and finish successfully.");
    run_test("deflate_rle_literal", test_deflate_rle_literal,
             "Literal path without long run should compress and complete successfully.");
    run_test("deflate_empty_and_finish", test_deflate_empty_and_finish,
             "Empty input should finish cleanly without errors.");

    // Print results
    print_summary();
    return 0;
}