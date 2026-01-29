/*
Unit Test Suite for the focal method CreateCGATS in tifdiff.c
- No GTest; a lightweight, self-contained test harness is provided.
- Uses C++11, std::cout for results, and simple logging to validate behavior.
- Provides minimal C-style mocks for the Little CMS API used by CreateCGATS.
- Focuses on validating the data-flow decisions inside CreateCGATS:
  - NUMBER_OF_FIELDS is set to 6
  - Data formats for the 6 fields are registered
  - NUMBER_OF_SETS is chosen according to Channels (1 -> 3, 3 -> 5, 4 -> 6)
  - TAX: The function writes a CGATS file name via cmsIT8SaveToFile
- We exercise true branches for Channels values 1, 3, and 4.
- Assumes the project compiles tifdiff.c together with this test harness.
*/

#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <utils.h>
#include <tiffio.h>
#include <ctime>


// Forward declaration of focal method to be tested (linker will resolve against tifdiff.c)
extern "C" void CreateCGATS(const char* TiffName1, const char* TiffName2);

// Minimal fake types to satisfy method signatures in tifdiff.c
struct STAT { double dummy; };          // simplistic stand-in for the real STAT
typedef STAT* LPSTAT;                    // interpret as pointer to STAT (consistent with common LPSTAT usage)
typedef void* cmsHANDLE;

// Global variables and data structures used by CreateCGATS (as expected by tifdiff.c)
STAT ColorantStat[4];                     // array used with &ColorantStat[i] in switch branches
double EuclideanStat;                       // Euclidean statistic
double ColorimetricStat;                    // Colorimetric statistic
const char* CGATSout;                        // output filename used by cmsIT8SaveToFile
int Channels;                                // color channels (1, 3, 4 supported in the switch)

// Logging for assertions
static std::vector<std::string> g_logs;
static std::vector<std::string> g_errors;

// Helpers for tests
static void reset_logs() { g_logs.clear(); g_errors.clear(); }
static bool log_contains(const std::string& needle)
{
    for (const auto& s : g_logs)
        if (s.find(needle) != std::string::npos) return true;
    return false;
}
static bool error_occurred() { return !g_errors.empty(); }

// C-style mocks for Little CMS API used by CreateCGATS
extern "C" {

// Basic opaque handle
typedef void* cmsHANDLE;

// Forward declaration of AddOneCGATSRow (defined in tifdiff.c)
extern "C" void AddOneCGATSRow(cmsHANDLE hIT8, char* Name, LPSTAT st);

// Mock implementations that log their calls
cmsHANDLE cmsIT8Alloc(int) {
    g_logs.emplace_back("cmsIT8Alloc");
    return reinterpret_cast<cmsHANDLE>(1);
}

void cmsIT8SetSheetType(cmsHANDLE, const char* s) {
    g_logs.emplace_back(std::string("SheetType:") + s);
}

void cmsIT8SetComment(cmsHANDLE, const char* c) {
    g_logs.emplace_back(std::string("Comment:") + c);
}

void cmsIT8SetPropertyStr(cmsHANDLE, const char* key, const char* val) {
    g_logs.emplace_back(std::string("PropertyStr:") + key + "=" + val);
}

void cmsIT8SetPropertyDbl(cmsHANDLE, const char* key, double val) {
    // Use fixed formatting that matches std::to_string behavior for doubles
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%.6f", val);
    g_logs.emplace_back(std::string("PropertyDbl:") + key + "=" + buf);
}

void cmsIT8SetDataFormat(cmsHANDLE, int idx, const char* fmt) {
    g_logs.emplace_back(std::string("DataFormat:") + std::to_string(idx) + "=" + fmt);
}

void cmsIT8SaveToFile(cmsHANDLE, const char* filename) {
    g_logs.emplace_back(std::string("SaveToFile:") + filename);
    CGATSout = filename;
}

void cmsIT8Free(cmsHANDLE) {
    g_logs.emplace_back("cmsIT8Free");
}

// FatalError stub (to catch unintended paths in tests)
void FatalError(const char* msg) {
    g_errors.emplace_back(std::string("FatalError:") + msg);
}

// Access to AddOneCGATSRow is expected to be provided by tifdiff.c.
// We cannot redefine it here (would clash with the real implementation).
// Thus, we only declare it to satisfy the linker.
}

// Test helpers to build assertions in plain C++
static void expect_true(bool cond, const char* desc) {
    if (cond) {
        std::cout << "[PASS] " << desc << "\n";
    } else {
        std::cout << "[FAIL] " << desc << "\n";
        // Record error for aggregated report
        g_errors.emplace_back(desc);
    }
}

// Test 1: Channel = 1 should configure 3 sets and write to file
static void test_Channel1_Creates3Sets()
{
    reset_logs();
    // Prepare global state expected by CreateCGATS
    Channels = 1;                      // 1-channel grayscale
    ColorantStat[0].dummy = 0.0;
    EuclideanStat = 0.0;
    ColorimetricStat = 0.0;
    CGATSout = "out_channel1.cgats";

    // Call focal method
    CreateCGATS("FirstImage.tif", "SecondImage.tif");

    // Assertions:
    // 1) Sheet type and creation are logged
    expect_true(log_contains("SheetType:TIFFDIFF"), "Channel1: TIFFDIFF sheet type set");
    // 2) The number of fields is defined as 6
    expect_true(log_contains("PropertyDbl:NUMBER_OF_FIELDS=6.000000"), "Channel1: NUMBER_OF_FIELDS = 6");
    // 3) Data formats for the 6 fields are registered in order
    expect_true(log_contains("DataFormat:0=SAMPLE_ID"), "Channel1: DataFormat 0 = SAMPLE_ID");
    expect_true(log_contains("DataFormat:1=PER100_EQUAL"), "Channel1: DataFormat 1 = PER100_EQUAL");
    expect_true(log_contains("DataFormat:2=MEAN_DE"), "Channel1: DataFormat 2 = MEAN_DE");
    expect_true(log_contains("DataFormat:3=STDEV_DE"), "Channel1: DataFormat 3 = STDEV_DE");
    expect_true(log_contains("DataFormat:4=MIN_DE"), "Channel1: DataFormat 4 = MIN_DE");
    expect_true(log_contains("DataFormat:5=MAX_DE"), "Channel1: DataFormat 5 = MAX_DE");
    // 4) NUMBER_OF_SETS matches 3 for 1 channel
    expect_true(log_contains("PropertyDbl:NUMBER_OF_SETS=3.000000"), "Channel1: NUMBER_OF_SETS = 3");
    // 5) The output CGATS file path was recorded
    expect_true(log_contains("SaveToFile:out_channel1.cgats"), "Channel1: CGATS output filename saved");
    // 6) Ensure no FatalError occurred
    expect_true(!error_occurred(), "Channel1: No fatal error occurred");
}

// Test 2: Channel = 3 should configure 5 sets
static void test_Channel3_Creates5Sets()
{
    reset_logs();
    Channels = 3;  // RGB
    ColorantStat[0].dummy = 0.0;
    ColorantStat[1].dummy = 0.0;
    ColorantStat[2].dummy = 0.0;
    EuclideanStat = 0.0;
    ColorimetricStat = 0.0;
    CGATSout = "out_channel3.cgats";

    CreateCGATS("Left.png", "Right.png");

    // Assertions:
    expect_true(log_contains("SheetType:TIFFDIFF"), "Channel3: TIFFDIFF sheet type set");
    expect_true(log_contains("PropertyDbl:NUMBER_OF_FIELDS=6.000000"), "Channel3: NUMBER_OF_FIELDS = 6");
    expect_true(log_contains("DataFormat:0=SAMPLE_ID"), "Channel3: DataFormat 0 = SAMPLE_ID");
    expect_true(log_contains("DataFormat:1=PER100_EQUAL"), "Channel3: DataFormat 1 = PER100_EQUAL");
    expect_true(log_contains("DataFormat:2=MEAN_DE"), "Channel3: DataFormat 2 = MEAN_DE");
    expect_true(log_contains("DataFormat:3=STDEV_DE"), "Channel3: DataFormat 3 = STDEV_DE");
    expect_true(log_contains("DataFormat:4=MIN_DE"), "Channel3: DataFormat 4 = MIN_DE");
    expect_true(log_contains("DataFormat:5=MAX_DE"), "Channel3: DataFormat 5 = MAX_DE");
    expect_true(log_contains("PropertyDbl:NUMBER_OF_SETS=5.000000"), "Channel3: NUMBER_OF_SETS = 5");
    expect_true(log_contains("SaveToFile:out_channel3.cgats"), "Channel3: CGATS output filename saved");
    expect_true(!error_occurred(), "Channel3: No fatal error occurred");
}

// Test 3: Channel = 4 should configure 6 sets
static void test_Channel4_Creates6Sets()
{
    reset_logs();
    Channels = 4;  // CMYK
    ColorantStat[0].dummy = 0.0;
    ColorantStat[1].dummy = 0.0;
    ColorantStat[2].dummy = 0.0;
    ColorantStat[3].dummy = 0.0;
    EuclideanStat = 0.0;
    ColorimetricStat = 0.0;
    CGATSout = "out_channel4.cgats";

    CreateCGATS("A.tif", "B.tif");

    // Assertions:
    expect_true(log_contains("SheetType:TIFFDIFF"), "Channel4: TIFFDIFF sheet type set");
    expect_true(log_contains("PropertyDbl:NUMBER_OF_FIELDS=6.000000"), "Channel4: NUMBER_OF_FIELDS = 6");
    expect_true(log_contains("DataFormat:0=SAMPLE_ID"), "Channel4: DataFormat 0 = SAMPLE_ID");
    expect_true(log_contains("DataFormat:1=PER100_EQUAL"), "Channel4: DataFormat 1 = PER100_EQUAL");
    expect_true(log_contains("DataFormat:2=MEAN_DE"), "Channel4: DataFormat 2 = MEAN_DE");
    expect_true(log_contains("DataFormat:3=STDEV_DE"), "Channel4: DataFormat 3 = STDEV_DE");
    expect_true(log_contains("DataFormat:4=MIN_DE"), "Channel4: DataFormat 4 = MIN_DE");
    expect_true(log_contains("DataFormat:5=MAX_DE"), "Channel4: DataFormat 5 = MAX_DE");
    expect_true(log_contains("PropertyDbl:NUMBER_OF_SETS=6.000000"), "Channel4: NUMBER_OF_SETS = 6");
    expect_true(log_contains("SaveToFile:out_channel4.cgats"), "Channel4: CGATS output filename saved");
    expect_true(!error_occurred(), "Channel4: No fatal error occurred");
}

// Entrypoint for executing tests
int main() {
    std::cout << "Starting CreateCGATS unit tests (no GTest, lightweight harness)\n";

    // Run tests
    test_Channel1_Creates3Sets();
    test_Channel3_Creates5Sets();
    test_Channel4_Creates6Sets();

    // Summary
    if (g_errors.empty()) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << g_errors.size() << " TEST(S) FAILED\n";
        return 1;
    }
}