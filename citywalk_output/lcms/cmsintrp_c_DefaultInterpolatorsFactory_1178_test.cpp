// CMS Interpolator Factory Unit Tests (C++11, without Google Test)
// This test suite exercises cmsInterpFunction DefaultInterpolatorsFactory
// from cmsintrp.c by inspecting the resulting function pointer selections.
//
// Notes:
// - We rely on the library's public headers to expose types and constants:
//   cmsUInt32Number, cmsInterpFunction, CMS_LERP_FLAGS_FLOAT,
//   CMS_LERP_FLAGS_TRILINEAR, MAX_STAGE_CHANNELS, and DefaultInterpolatorsFactory.
// - We perform non-terminating assertions: tests continue even if one check fails.
// - We focus on pointer presence (NULL vs non-NULL) to validate which interpolation
//   routine would be used for a given configuration, rather than calling the actual
//   private implementations.
//
// The tests can be run by compiling this file together with the cmsintrp.c source
// within the same project (or with the appropriate library build). If the project
// uses a separate test harness, adapt the main() accordingly.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Simple non-terminating assertion macros
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[FAILED] " << (msg) \
                  << " (Line " << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } else { \
        std::cout << "[PASSED] " << (msg) << std::endl; \
    } \
} while (0)

#define EXPECT_NULL(ptr, msg) do { \
    if ((ptr) != NULL) { \
        std::cerr << "[FAILED] " << (msg) \
                  << " Expected NULL, got non-NULL at " \
                  << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failures; \
    } else { \
        std::cout << "[PASSED] " << (msg) << std::endl; \
    } \
} while (0)

#define EXPECT_NOT_NULL(ptr, msg) do { \
    if ((ptr) == NULL) { \
        std::cerr << "[FAILED] " << (msg) \
                  << " Expected non-NULL, got NULL at " \
                  << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failures; \
    } else { \
        std::cout << "[PASSED] " << (msg) << std::endl; \
    } \
} while (0)

// Helper to invoke the factory and return the interpolation descriptor
static cmsInterpFunction callFactory(uint32_t inCh, uint32_t outCh, uint32_t dwFlags) {
    return DefaultInterpolatorsFactory(inCh, outCh, dwFlags);
}

// Test 1: Safety check - large input/output channels should yield an empty (NULL) interpolator
// When nInputChannels >= 4 and nOutputChannels >= MAX_STAGE_CHANNELS, no interpolation should be provided.
static void test_safety_large_channels() {
    uint32_t nInputChannels = 4;
    uint32_t nOutputChannels = MAX_STAGE_CHANNELS;
    uint32_t dwFlags = 0; // no special flags
    cmsInterpFunction interp = callFactory(nInputChannels, nOutputChannels, dwFlags);

    EXPECT_NULL(interp.LerpFloat, "Safety: LerpFloat should be NULL for large input/output");
    EXPECT_NULL(interp.Lerp16,  "Safety: Lerp16 should be NULL for large input/output");
}

// Test 2: Gray LUT / 1D linear case with float flag (nInput=1, nOutput=1, IsFloat)
// Expect LerpFloat to be non-NULL and Lerp16 to remain NULL.
static void test_gray_lut_float_case() {
    uint32_t inCh = 1;
    uint32_t outCh = 1;
    uint32_t dwFlags = CMS_LERP_FLAGS_FLOAT;
    cmsInterpFunction interp = callFactory(inCh, outCh, dwFlags);

    EXPECT_NOT_NULL(interp.LerpFloat, "Gray LUT (1->1) with FLOAT: LerpFloat should be non-NULL");
    EXPECT_NULL(interp.Lerp16,      "Gray LUT (1->1) with FLOAT: Lerp16 should be NULL");
}

// Test 3: Gray LUT / 1D linear case without float flag (nInput=1, nOutput=1, not IsFloat)
// Expect Lerp16 to be non-NULL and LerpFloat to remain NULL.
static void test_gray_lut_int_case() {
    uint32_t inCh = 1;
    uint32_t outCh = 1;
    uint32_t dwFlags = 0;
    cmsInterpFunction interp = callFactory(inCh, outCh, dwFlags);

    EXPECT_NOT_NULL(interp.Lerp16, "Gray LUT (1->1) without FLOAT: Lerp16 should be non-NULL");
    EXPECT_NULL(interp.LerpFloat, "Gray LUT (1->1) without FLOAT: LerpFloat should be NULL");
}

// Test 4: Duotone (nInput=2) with float flag
// Expect LerpFloat non-NULL; Lerp16 NULL.
static void test_duotone_float_case() {
    uint32_t inCh = 2;
    uint32_t outCh = 4; // any non-zero to proceed
    uint32_t dwFlags = CMS_LERP_FLAGS_FLOAT;
    cmsInterpFunction interp = callFactory(inCh, outCh, dwFlags);

    EXPECT_NOT_NULL(interp.LerpFloat, "Duotone (2->*) with FLOAT: LerpFloat should be non-NULL");
    EXPECT_NULL(interp.Lerp16,       "Duotone (2->*) with FLOAT: Lerp16 should be NULL");
}

// Test 5: Duotone (nInput=2) without float flag
// Expect Lerp16 non-NULL; LerpFloat NULL.
static void test_duotone_int_case() {
    uint32_t inCh = 2;
    uint32_t outCh = 4;
    uint32_t dwFlags = 0;
    cmsInterpFunction interp = callFactory(inCh, outCh, dwFlags);

    EXPECT_NOT_NULL(interp.Lerp16, "Duotone (2->*) without FLOAT: Lerp16 should be non-NULL");
    EXPECT_NULL(interp.LerpFloat, "Duotone (2->*) without FLOAT: LerpFloat should be NULL");
}

// Test 6: RGB triplet (nInput=3) with various combinations of TRILINEAR and FLOAT
static void test_rgb_trilinear_variants() {
    const uint32_t inCh = 3;
    const uint32_t outCh = 3;
    // a) Trilinear + Float
    {
        uint32_t dwFlags = CMS_LERP_FLAGS_TRILINEAR | CMS_LERP_FLAGS_FLOAT;
        cmsInterpFunction interp = callFactory(inCh, outCh, dwFlags);
        EXPECT_NOT_NULL(interp.LerpFloat, "RGB (3) TRILINEAR+FLOAT: LerpFloat non-NULL");
        EXPECT_NULL(interp.Lerp16,       "RGB (3) TRILINEAR+FLOAT: Lerp16 NULL");
    }
    // b) Trilinear only (no Float)
    {
        uint32_t dwFlags = CMS_LERP_FLAGS_TRILINEAR;
        cmsInterpFunction interp = callFactory(inCh, outCh, dwFlags);
        EXPECT_NOT_NULL(interp.Lerp16, "RGB (3) TRILINEAR only: Lerp16 non-NULL");
        EXPECT_NULL(interp.LerpFloat,  "RGB (3) TRILINEAR only: LerpFloat NULL");
    }
    // c) Float only (no Trilinear)
    {
        uint32_t dwFlags = CMS_LERP_FLAGS_FLOAT;
        cmsInterpFunction interp = callFactory(inCh, outCh, dwFlags);
        EXPECT_NOT_NULL(interp.LerpFloat, "RGB (3) FLOAT only: LerpFloat non-NULL");
        EXPECT_NULL(interp.Lerp16,       "RGB (3) FLOAT only: Lerp16 NULL");
    }
    // d) Neither flag
    {
        uint32_t dwFlags = 0;
        cmsInterpFunction interp = callFactory(inCh, outCh, dwFlags);
        EXPECT_NOT_NULL(interp.Lerp16, "RGB (3) default: Lerp16 non-NULL");
        EXPECT_NULL(interp.LerpFloat,  "RGB (3) default: LerpFloat NULL");
    }
}

// Test 7: 4-input case (nInput=4) with and without FLOAT
static void test_four_inputs_variants() {
    const uint32_t inCh = 4;
    const uint32_t outCh = 8;
    // a) FLOAT
    {
        uint32_t dwFlags = CMS_LERP_FLAGS_FLOAT;
        cmsInterpFunction interp = callFactory(inCh, outCh, dwFlags);
        EXPECT_NOT_NULL(interp.LerpFloat, "4-input with FLOAT: LerpFloat non-NULL");
        EXPECT_NULL(interp.Lerp16,       "4-input with FLOAT: Lerp16 NULL");
    }
    // b) integer only
    {
        uint32_t dwFlags = 0;
        cmsInterpFunction interp = callFactory(inCh, outCh, dwFlags);
        EXPECT_NOT_NULL(interp.Lerp16, "4-input without FLOAT: Lerp16 non-NULL");
        EXPECT_NULL(interp.LerpFloat,  "4-input without FLOAT: LerpFloat NULL");
    }
}

// Test 8: Edge case - 0 input channels (not a meaningful rendering scenario but tests default path)
static void test_zero_input_channels() {
    uint32_t inCh = 0;
    uint32_t outCh = 1;
    uint32_t dwFlags = 0;
    cmsInterpFunction interp = callFactory(inCh, outCh, dwFlags);
    EXPECT_NULL(interp.LerpFloat, "Zero input: LerpFloat NULL");
    EXPECT_NULL(interp.Lerp16,  "Zero input: Lerp16 NULL");
}

int main() {
    std::cout << "Starting DefaultInterpolatorsFactory unit tests (CMS/LCMS II) ..." << std::endl;

    test_safety_large_channels();
    test_gray_lut_float_case();
    test_gray_lut_int_case();
    test_duotone_float_case();
    test_duotone_int_case();
    test_rgb_trilinear_variants();
    test_four_inputs_variants();
    test_zero_input_channels();

    if (g_failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) FAILED." << std::endl;
        return 1;
    }
}