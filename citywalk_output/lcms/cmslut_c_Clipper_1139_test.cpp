// Unit test suite for the focal method Clipper in cmslut.c
// This test is designed for C++11 compilation, without GoogleTest.
// It uses a lightweight, non-terminating assertion framework to maximize code coverage.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cmath>


// -----------------------------------------------------------------------------
// Minimal type and structure definitions to match the focal function's signature
// and enable standalone compilation alongside cmslut.c.
// Note: These definitions are intended to be compatible with the cmslut.c usage
// of Clipper and a cmsStage that contains at least InputChannels as a member.
// -----------------------------------------------------------------------------

typedef float cmsFloat32Number;
typedef uint32_t cmsUInt32Number;

struct cmsStage {
    cmsUInt32Number InputChannels;
    // The real cmsStage in the library contains more members, but for the
    // purpose of testing Clipper's behavior, only InputChannels is required here.
};

// Forward declaration of the focal function from cmslut.c
// We declare it with C linkage to match the library's ABI.
extern "C" void Clipper(const cmsFloat32Number In[],
                        cmsFloat32Number Out[],
                        const cmsStage *mpe);

// -----------------------------------------------------------------------------
// Lightweight test framework (non-terminating assertions)
// -----------------------------------------------------------------------------

static int g_failedTests = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "[TEST FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " - Condition failed: " << #cond << std::endl; \
        ++g_failedTests; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    if(!((a) == (b))) { \
        std::cerr << "[TEST FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " - Expected " << #a << " == " << #b \
                  << " but got " << (a) << " != " << (b) << std::endl; \
        ++g_failedTests; \
    } \
} while(0)

#define EXPECT_NEAR(a, b, eps) do { \
    if(std::fabs((a) - (b)) > (eps)) { \
        std::cerr << "[TEST FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " - Expected " << #a << " near " << #b \
                  << " within " << (eps) \
                  << " but got " << (a) << " vs " << (b) << std::endl; \
        ++g_failedTests; \
    } \
} while(0)

// -----------------------------------------------------------------------------
// Test Cases for Clipper
// Clipper implements a per-channel ReLU: Out[i] = max(In[i], 0)
// This suite covers:
// - Negative inputs mapped to zero (true branch of n < 0)
// - Non-negative inputs preserved (false branch of n < 0)
// - Mixed cases over multiple channels
// -----------------------------------------------------------------------------

// Test 1: Mixed negative and non-negative values on 3 channels
// Verifies that negatives are clamped to zero and non-negatives are preserved.
void test_Clipper_negative_and_non_negative() {
    cmsStage mpe;
    mpe.InputChannels = 3;

    const cmsFloat32Number In[3] = { -1.5f, 0.0f, 2.75f };
    cmsFloat32Number Out[3] = { -123.0f, -123.0f, -123.0f }; // pre-fill to detect changes

    Clipper(In, Out, &mpe);

    // Expected: [0.0, 0.0, 2.75]
    EXPECT_EQ(Out[0], 0.0f);
    EXPECT_EQ(Out[1], 0.0f);
    EXPECT_NEAR(Out[2], 2.75f, 1e-6f);

    // Additional sanity: non-modified inputs do not affect outputs beyond threshold
}

// Test 2: All-positive inputs across 3 channels
// Verifies that non-negative values are reproduced exactly.
void test_Clipper_all_positive() {
    cmsStage mpe;
    mpe.InputChannels = 3;

    const cmsFloat32Number In[3] = { 0.5f, 2.0f, 1.0f };
    cmsFloat32Number Out[3] = { -9.0f, -9.0f, -9.0f };

    Clipper(In, Out, &mpe);

    EXPECT_NEAR(Out[0], 0.5f, 1e-6f);
    EXPECT_NEAR(Out[1], 2.0f, 1e-6f);
    EXPECT_NEAR(Out[2], 1.0f, 1e-6f);
}

// Test 3: All-negative inputs across 4 channels
// Verifies that all negatives are clamped to zero.
void test_Clipper_all_negative() {
    cmsStage mpe;
    mpe.InputChannels = 4;

    const cmsFloat32Number In[4] = { -0.1f, -2.0f, -3.14f, -100.0f };
    cmsFloat32Number Out[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    Clipper(In, Out, &mpe);

    EXPECT_EQ(Out[0], 0.0f);
    EXPECT_EQ(Out[1], 0.0f);
    EXPECT_EQ(Out[2], 0.0f);
    EXPECT_EQ(Out[3], 0.0f);
}

// Test 4: Mixed values across 4 channels (edge coverage)
// Ensures per-channel processing and both branches of the conditional are exercised.
void test_Clipper_mixed_four_channels() {
    cmsStage mpe;
    mpe.InputChannels = 4;

    const cmsFloat32Number In[4] = { -0.2f, 0.0f, 3.3f, -9.9f };
    cmsFloat32Number Out[4] = { -1.0f, -1.0f, -1.0f, -1.0f };

    Clipper(In, Out, &mpe);

    EXPECT_EQ(Out[0], 0.0f);
    EXPECT_EQ(Out[1], 0.0f);
    EXPECT_NEAR(Out[2], 3.3f, 1e-6f);
    EXPECT_EQ(Out[3], 0.0f);
}

// Run all tests
void RunClipperTests() {
    test_Clipper_negative_and_non_negative();
    test_Clipper_all_positive();
    test_Clipper_all_negative();
    test_Clipper_mixed_four_channels();
}

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------

int main() {
    std::cout << "Running Clipper unit tests (CMS LittleCMS, C linkage)..." << std::endl;

    RunClipperTests();

    if (g_failedTests == 0) {
        std::cout << "[TEST SUITE PASSED] All Clipper tests succeeded." << std::endl;
        return 0;
    } else {
        std::cout << "[TEST SUITE FAILED] Number of failed tests: " << g_failedTests << std::endl;
        return 1;
    }
}