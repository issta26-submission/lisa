// Lightweight C++11 test harness for png_colormap_compose
// This test suite focuses on exercising the two branches of the
// encoding predicate (P_LINEAR vs P_sRGB) and validates expected results
// under the assumption that decode_gamma(display, value, encoding) behaves
// deterministically for the selected test inputs (i.e., returns the input value
// for the tested encodings). The tests use non-terminating assertions:
// failures are logged but do not abort the test run, enabling higher coverage.

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>


extern "C" {
    // Forward declaration of the function under test from the library/C file.
    // The actual project must provide a corresponding implementation linked at
    // compile time. We assume the library exposes this symbol with the exact
    // signature used here.
    struct png_image_read_control;
    png_uint_32 png_colormap_compose(png_image_read_control *display,
        png_uint_32 foreground, int foreground_encoding, png_uint_32 alpha,
        png_uint_32 background, int encoding);

    // libpng helper macro (assumed to be available when linking with libpng)
    // Converts a linear 0..65535 value to its sRGB-encoded counterpart.
    // The actual macro is provided by libpng headers (png.h).
    unsigned int PNG_sRGB_FROM_LINEAR(unsigned int); // macro, used as function-like
    // Public constants from libpng describing color encodings
    // (P_LINEAR, P_sRGB are typically enums/macros defined in libpng headers).
    // We declare them here so the test remains self-contained in concept.
    // If these are not available, include the appropriate libpng header in build.
    extern const int P_LINEAR;
    extern const int P_sRGB;
    // The type used by libpng for 32-bit unsigned integers
    typedef uint32_t png_uint_32;
}

// Simple logging helper to accumulate test results without terminating on failures
struct TestLogger {
    std::vector<std::string> failures;

    void log_failure(const char* test_name, uint32_t got, uint32_t expected, const char* detail = "") {
        char buf[512];
        if (detail && detail[0] != '\0') {
            snprintf(buf, sizeof(buf),
                     "%s: FAIL. Got=%u, Expected=%u. %s",
                     test_name, (unsigned)got, (unsigned)expected, detail);
        } else {
            snprintf(buf, sizeof(buf),
                     "%s: FAIL. Got=%u, Expected=%u",
                     test_name, (unsigned)got, (unsigned)expected);
        }
        failures.emplace_back(buf);
    }

    void log_success(const char* test_name) {
        char buf[256];
        snprintf(buf, sizeof(buf), "%s: PASS", test_name);
        // Use as a non-fatal report; we can print later if desired
        // failures.emplace_back(buf); // Optional: log passes as well
    }

    void report() const {
        size_t total = failures.size();
        if (total == 0) {
            std::cout << "All tests completed with no failures.\n";
            return;
        }
        std::cout << total << " failures:\n";
        for (const auto& s : failures) {
            std::cout << "  " << s << "\n";
        }
    }
};

// Helper to compute the "expected" result for the linear path, assuming
// decode_gamma returns the input value for both foreground and background.
// This mirrors the core arithmetic in png_colormap_compose when encoding == P_LINEAR.
static uint32_t expected_linear_identity(uint32_t foreground, uint32_t alpha, uint32_t background) {
    // Compute: f = foreground * alpha + background * (255 - alpha)
    // Then: f = (f * 257) + (f * 1) >> 16; but implemented as
    // f = f * 257; f += f >> 16; f = (f + 32768) >> 16
    uint64_t f = static_cast<uint64_t>(foreground) * static_cast<uint64_t>(alpha)
               + static_cast<uint64_t>(background) * static_cast<uint64_t>(255u - alpha);
    f = f * 257ull;
    f += (f >> 16);
    f = (f + 32768ull) >> 16;
    return static_cast<uint32_t>(f);
}

// Forward declare a small identity-decode helper to compute the expected sRGB path.
// For the sRGB path, we compute using the same pre-alpha-composite value and apply
// the library's sRGB mapping.
static uint32_t expected_srgb_path(uint32_t foreground, uint32_t alpha, uint32_t background) {
    uint32_t pre_f = expected_linear_identity(foreground, alpha, background);
    // PNG_sRGB_FROM_LINEAR maps linear to sRGB space; use the macro as a function-like
    // call to obtain the expected value (this relies on libpng being available).
    return static_cast<uint32_t>(PNG_sRGB_FROM_LINEAR(pre_f));
}

// Test 1: Validate the P_LINEAR branch with a simple input.
// Foreground=128, Background=64, Alpha=128, Encoding=P_LINEAR
// Expectation computed via identity decode_gamma (i.e., as if no extra gamma processing).
static bool test_png_colormap_compose_linear_basic(TestLogger& logger) {
    // Prepare a minimal display object (fields are assumed unused by this test path)
    png_image_read_control display = {};
    // Call the function under test
    uint32_t result = png_colormap_compose(&display,
                                           128u, P_LINEAR, 128u,
                                           64u, P_LINEAR);
    // Expected using the linear-identity model
    uint32_t expected = expected_linear_identity(128u, 128u, 64u);
    if (result != expected) {
        logger.log_failure("test_png_colormap_compose_linear_basic", result, expected,
                           "Linear path with (foreground=128, alpha=128, background=64)");
        return false;
    }
    logger.log_success("test_png_colormap_compose_linear_basic");
    return true;
}

// Test 2: Validate the P_LINEAR branch with alpha=0 (edge case).
// Foreground=64, Background=64, Alpha=0, Encoding=P_LINEAR
static bool test_png_colormap_compose_linear_alpha_zero(TestLogger& logger) {
    png_image_read_control display = {};
    uint32_t result = png_colormap_compose(&display,
                                           64u, P_LINEAR, 0u,
                                           64u, P_LINEAR);
    uint32_t expected = expected_linear_identity(64u, 0u, 64u);
    if (result != expected) {
        logger.log_failure("test_png_colormap_compose_linear_alpha_zero", result, expected,
                           "Linear path with alpha=0");
        return false;
    }
    logger.log_success("test_png_colormap_compose_linear_alpha_zero");
    return true;
}

// Test 3: Validate the P_sRGB branch using the same pre-combined value
// as in the linear path, to ensure the sRGB mapping is applied.
// Foreground=128, Background=64, Alpha=128, Encoding=P_sRGB
static bool test_png_colormap_compose_srgb_branch(TestLogger& logger) {
    png_image_read_control display = {};
    uint32_t result = png_colormap_compose(&display,
                                           128u, P_LINEAR, 128u,
                                           64u, P_sRGB);
    uint32_t expected = expected_srgb_path(128u, 128u, 64u);
    if (result != expected) {
        logger.log_failure("test_png_colormap_compose_srgb_branch", result, expected,
                           "sRGB branch with (foreground=128, alpha=128, background=64)");
        return false;
    }
    logger.log_success("test_png_colormap_compose_srgb_branch");
    return true;
}

int main() {
    TestLogger logger;

    // Run tests
    bool a = test_png_colormap_compose_linear_basic(logger);
    bool b = test_png_colormap_compose_linear_alpha_zero(logger);
    bool c = test_png_colormap_compose_srgb_branch(logger);

    // Final report
    if (!logger.failures.empty()) {
        // Print all failures already accumulated
        logger.report();
        std::cerr << "Some tests FAILED. See details above.\n";
        return 1;
    } else {
        std::cout << "All tests PASSED.\n";
        return 0;
    }
}