// Test suite for the CheckScreening function (C++11, no GTest)
// This test uses a lightweight, in-process mock of cmsWriteTag/cmsReadTag
// and IsGoodFixed15_16 to exercise both branches of CheckScreening.
// The tests are written to be executed by a main() in this file.

#include <cmath>
#include <cstring>
#include <cstdio>
#include <testcms2.h>


// Import necessary declarations from the provided CMS test header.
// We rely on the presence of types like cmsInt32Number, cmsHPROFILE, cmsTagSignature, etc.
extern "C" {
}

// Forward declare the focal function (C linkage)
extern "C" cmsInt32Number CheckScreening(cmsInt32Number Pass, cmsHPROFILE hProfile, cmsTagSignature tag);

// We also provide C-linkage declarations for the library's tag/io helpers,
// so we can override them in this test environment.
extern "C" cmsInt32Number cmsWriteTag(cmsHPROFILE hProfile, cmsTagSignature tag, const void* data);
extern "C" void* cmsReadTag(cmsHPROFILE hProfile, cmsTagSignature tag);
extern "C" cmsBool IsGoodFixed15_16(const char* Title, cmsFloat64Number in, cmsFloat64Number out);

// Lightweight test harness (non-terminating assertions)
static int g_totalTests = 0;
static int g_failedTests = 0;

#define EXPECT_TRUE(cond) do { \
    g_totalTests++; \
    if(!(cond)) { \
        g_failedTests++; \
        fprintf(stderr, "Test failed: %s:%d: condition %s is false\n", __FILE__, __LINE__, #cond); \
    } \
} while(0)

#define EXPECT_EQ_INT(a,b) do { \
    g_totalTests++; \
    if( (int)(a) != (int)(b) ) { \
        g_failedTests++; \
        fprintf(stderr, "Test failed: %s:%d: Expected %d, got %d\n", __FILE__, __LINE__, (int)(b), (int)(a)); \
    } \
} while(0)


// Global mock state to intercept tag IO
static cmsScreening g_writtenSc;        // Last written screening data
static bool g_tagStored = false;
static cmsHPROFILE g_tagProfile = NULL;
static cmsTagSignature g_tag = 0;

// Readback buffer (static to avoid dynamic memory in tests)
static cmsScreening g_readSc;
static bool g_readScValid = false;

// Override controls to simulate invalid readouts during CheckScreening(Pass==2)
static bool g_override_nchannels = false;
static bool g_override_flag = false;
static bool g_override_freq = false;
static bool g_override_angle = false;
static cmsFloat64Number g_override_freq_value = 0.0;
static cmsFloat64Number g_override_angle_value = 0.0;

// Resets mock state between tests
static void resetMocks() {
    g_tagStored = false;
    g_tagProfile = NULL;
    g_tag = 0;
    memset(&g_writtenSc, 0, sizeof(g_writtenSc));
    g_readScValid = false;
    g_readSc = *((cmsScreening*)NULL); // ensure zero initialization later
    // Clear overrides
    g_override_nchannels = false;
    g_override_flag = false;
    g_override_freq = false;
    g_override_angle = false;
    g_override_freq_value = 0.0;
    g_override_angle_value = 0.0;
}

// Mock implementation of cmsWriteTag: store the data so that cmsReadTag can return it
extern "C" cmsInt32Number cmsWriteTag(cmsHPROFILE hProfile, cmsTagSignature tag, const void* data)
{
    // Record the tag and profile, store a copy of the screening struct
    g_tagProfile = hProfile;
    g_tag = tag;
    g_tagStored = true;
    if (data != NULL) {
        g_writtenSc = *(const cmsScreening*)data;
    } else {
        memset(&g_writtenSc, 0, sizeof(g_writtenSc));
    }
    return 1; // success
}

// Mock implementation of cmsReadTag: return a copy of the previously written screening
// If no data stored or tag/profile mismatch, return NULL
extern "C" void* cmsReadTag(cmsHPROFILE hProfile, cmsTagSignature tag)
{
    if (!g_tagStored) return NULL;
    if (hProfile != g_tagProfile || tag != g_tag) return NULL;

    // Copy stored data into a local static buffer to return a stable pointer
    g_readSc = g_writtenSc;

    // Apply any active overrides to simulate bad reads
    if (g_override_nchannels) g_readSc.nChannels = 2;
    if (g_override_flag) g_readSc.Flag = 1;
    if (g_override_freq) {
        // We assume the structure's nested field exists as in the production code.
        g_readSc.Channels[0].Frequency = g_override_freq_value;
    }
    if (g_override_angle) {
        g_readSc.Channels[0].ScreenAngle = g_override_angle_value;
    }
    g_readScValid = true;
    return &g_readSc;
}

// Lightweight IsGoodFixed15_16: provide deterministic comparison with tolerance
extern "C" cmsBool IsGoodFixed15_16(const char* Title, cmsFloat64Number in, cmsFloat64Number out)
{
    cmsBool ok = (std::fabs(in - out) < 1e-5);
    if(!ok) {
        fprintf(stderr, "IsGoodFixed15_16 failed for %s: got %f, expected %f\n", Title, (double)in, (double)out);
    }
    return ok;
}

// Test constants used by the test harness
static const cmsHPROFILE TEST_PROFILE = (cmsHPROFILE)0xDEADBEEF;
static const cmsTagSignature TEST_TAG = (cmsTagSignature)0x1234;

// Test 1: Ensure Pass 1 writes the tag and returns success
static void test_pass1_writes_tag()
{
    resetMocks();
    cmsInt32Number rc = CheckScreening(1, TEST_PROFILE, TEST_TAG);
    EXPECT_EQ_INT(rc, 1);
}

// Test 2: Ensure Pass 2 reads back the data written by Pass 1 and validates it
// (true path: all fields match expected values)
static void test_pass2_reads_good_data()
{
    resetMocks();
    // First, perform Pass 1 to store data
    cmsInt32Number rc1 = CheckScreening(1, TEST_PROFILE, TEST_TAG);
    EXPECT_EQ_INT(rc1, 1);
    // Then, perform Pass 2 to read and validate
    cmsInt32Number rc2 = CheckScreening(2, TEST_PROFILE, TEST_TAG);
    EXPECT_EQ_INT(rc2, 1);
}

// Test 3: Ensure Pass 2 returns 0 when there is no prior Pass 1 write (no tag stored)
static void test_pass2_no_tag()
{
    resetMocks();
    // Directly attempt to read without writing
    cmsInt32Number rc = CheckScreening(2, TEST_PROFILE, TEST_TAG);
    EXPECT_TRUE(rc == 0);
}

// Test 4: Ensure Pass 2 fails when nChannels != 1 (simulated mismatch)
static void test_pass2_bad_nchannels()
{
    resetMocks();
    // Write valid data
    cmsInt32Number rc1 = CheckScreening(1, TEST_PROFILE, TEST_TAG);
    EXPECT_EQ_INT(rc1, 1);
    // Enable override to simulate bad nChannels on read
    g_override_nchannels = true;
    cmsInt32Number rc2 = CheckScreening(2, TEST_PROFILE, TEST_TAG);
    EXPECT_TRUE(rc2 == 0);
}

// Test 5: Ensure Pass 2 fails when Frequency is not the expected fixed-point value
static void test_pass2_bad_frequency()
{
    resetMocks();
    // Write valid data
    cmsInt32Number rc1 = CheckScreening(1, TEST_PROFILE, TEST_TAG);
    EXPECT_EQ_INT(rc1, 1);
    // Override Frequency to an incorrect value
    g_override_freq = true;
    g_override_freq_value = 1.0; // not equal to 2.0
    cmsInt32Number rc2 = CheckScreening(2, TEST_PROFILE, TEST_TAG);
    EXPECT_TRUE(rc2 == 0);
}

// Test 6: Ensure Pass 2 fails when Flag is non-zero on read
static void test_pass2_bad_flag()
{
    resetMocks();
    // Write valid data
    cmsInt32Number rc1 = CheckScreening(1, TEST_PROFILE, TEST_TAG);
    EXPECT_EQ_INT(rc1, 1);
    // Override Flag to non-zero
    g_override_flag = true;
    cmsInt32Number rc2 = CheckScreening(2, TEST_PROFILE, TEST_TAG);
    EXPECT_TRUE(rc2 == 0);
}

// Main: run all tests and report summary
int main()
{
    // Execute tests
    test_pass1_writes_tag();
    test_pass2_reads_good_data();
    test_pass2_no_tag();
    test_pass2_bad_nchannels();
    test_pass2_bad_frequency();
    test_pass2_bad_flag();

    // Summary
    if (g_failedTests == 0) {
        fprintf(stderr, "All tests PASSED (%d tests).\n", g_totalTests);
        return 0;
    } else {
        fprintf(stderr, "Tests completed with failures: %d failed, %d attempted.\n",
                g_failedTests, g_totalTests);
        return 1;
    }
}