/*
Unit test suite for ITU2PCS (focal method) in jpgicc.c

Overview:
- Implements a minimal, non-terminating test harness (no GTest).
- Verifies ITU2PCS behavior by cross-checking against a direct chain: ITU2Lab(In) followed by cmsFloat2LabEncoded(...).
- Covers normal, boundary, and cargo-parameter-usage scenarios.
- Uses a lightweight assertion framework to collect and report failures while continuing execution for coverage.
- Intended to be compiled and linked with the existing project (no external testing framework).

Notes on Candidate Keywords and Dependencies:
- Core components identified from ITU2PCS:
  - ITU2Lab(In, Lab)
  - cmsFloat2LabEncoded(Out, Lab)
  - cmsCIELab Lab (intermediate)
  - In[3], Out[3] arrays of cmsUInt16Number
  - Cargo parameter (unused in function)
  - Returns TRUE (non-zero)
- Test covers: ITU2Lab, cmsFloat2LabEncoded, and ITU2PCS round-trip consistency.
- Assumes cmsUInt16Number and cmsCIELab types match the project’s definitions (common in LittleCMS-like codebases).
*/

#include <cstring>
#include <iccjpeg.h>
#include <jpeglib.h>
#include <vector>
#include <iostream>
#include <string>
#include <utils.h>


// Typedefs matching typical LittleCMS-like signatures.
// These definitions are chosen to align with common project conventions;
// if the actual project uses different internal definitions, please adjust accordingly.
typedef unsigned short cmsUInt16Number;
typedef double cmsFloat64;

struct cmsCIELab {
    cmsFloat64 L;
    cmsFloat64 a;
    cmsFloat64 b;
};

// External C interfaces from the focal class dependencies.
// We declare them here for the test harness to link against the real implementations.
extern "C" {
    // Focal method under test
    int ITU2PCS( register const cmsUInt16Number In[], register cmsUInt16Number Out[], register void*  Cargo);

    // Supporting conversions used inside ITU2PCS
    void ITU2Lab(const cmsUInt16Number In[3], cmsCIELab* Lab);
    void cmsFloat2LabEncoded( cmsUInt16Number Out[3], const cmsCIELab* Lab);
}

// Lightweight testing scaffold (non-terminating assertions)
static int g_failures = 0;

#define ASSERT_TRUE(cond, msg) \
    do { \
        if(!(cond)) { \
            ++g_failures; \
            std::cerr << "[ASSERT_TRUE] FAIL: " << msg \
                      << " (File " << __FILE__ << ", Line " << __LINE__ << ")\n"; \
        } \
    } while(0)

#define ASSERT_EQ(a, b, msg) \
    do { \
        if(!((a) == (b))) { \
            ++g_failures; \
            std::cerr << "[ASSERT_EQ] FAIL: " << msg \
                      << " | Expected: " << (b) << "  Got: " << (a) \
                      << " (File " << __FILE__ << ", Line " << __LINE__ << ")\n"; \
        } \
    } while(0)

// Helper to run a single round-trip comparison for a given input
static void test_roundtrip_for_input(const cmsUInt16Number In[3], const char* label) {
    cmsCIELab Lab;
    cmsUInt16Number Out_expected[3] = {0, 0, 0};
    cmsUInt16Number Out_actual[3] = {0, 0, 0};

    // Compute expected output via the direct chain: ITU2Lab -> cmsFloat2LabEncoded
    ITU2Lab(In, &Lab);
    cmsFloat2LabEncoded(Out_expected, &Lab);

    // Compute actual output via ITU2PCS
    int ret = ITU2PCS(In, Out_actual, nullptr);

    // Assertions
    ASSERT_TRUE(ret != 0, (std::string("ITU2PCS should return non-zero (TRUE) for ") + label).c_str());
    for (int i = 0; i < 3; ++i) {
        ASSERT_EQ(Out_expected[i], Out_actual[i],
                  (std::string("Mismatch in Out[") + char('0' + i) + "] for " + label).c_str());
    }
}

// Test 1: Basic correctness - round-trip consistency for a representative input
static void test_basic_roundtrip() {
    // Example input values
    cmsUInt16Number In[3] = {123, 45, 678};
    test_roundtrip_for_input(In, "basic_roundtrip_in123_45_678");

    // Additional small value set
    cmsUInt16Number In2[3] = {1024, 2048, 3072};
    test_roundtrip_for_input(In2, "basic_roundtrip_in1024_2048_3072");
}

// Test 2: Boundary values
static void test_boundary_values() {
    // All zeros
    cmsUInt16Number In0[3] = {0, 0, 0};
    test_roundtrip_for_input(In0, "boundary_in_0_0_0");

    // All maximum values (commonly 0xFFFF)
    cmsUInt16Number InMax[3] = {0xFFFF, 0xFFFF, 0xFFFF};
    test_roundtrip_for_input(InMax, "boundary_in_max_max_max");

    // Mixed boundary values
    cmsUInt16Number InMix[3] = {0, 0xFFFF, 0x7FFF};
    test_roundtrip_for_input(InMix, "boundary_in_0_max_32767");
}

// Test 3: Cargo parameter usage (should be ignored by ITU2PCS)
static void test_cargo_unused() {
    cmsUInt16Number In[3] = {100, 200, 300};

    cmsUInt16Number Out_with_null[3];
    int ret_null = ITU2PCS(In, Out_with_null, nullptr);
    cmsUInt16Number LabEncoded[3];
    cmsCIELab Lab;
    ITU2Lab(In, &Lab);
    cmsFloat2LabEncoded(LabEncoded, &Lab);

    // Compute same output with a non-null cargo pointer
    cmsUInt16Number dummyCargo = 0xDEAD;
    cmsUInt16Number Out_with_cargo[3];
    int ret_with_cargo = ITU2PCS(In, Out_with_cargo, &dummyCargo);

    // Basic assertions
    ASSERT_TRUE(ret_null != 0, "ITU2PCS should return non-zero when cargo is NULL");
    ASSERT_TRUE(ret_with_cargo != 0, "ITU2PCS should return non-zero when cargo is provided");

    for (int i = 0; i < 3; ++i) {
        ASSERT_EQ(LabEncoded[i], Out_with_null[i],
                  "Output should be identical to null-cargo run for test_cargo_unused (channel " + std::to_string(i) + ")");
        ASSERT_EQ(LabEncoded[i], Out_with_cargo[i],
                  "Output should be identical to cargo-run (channel " + std::to_string(i) + ")");
    }
    // Also confirm that the two outputs are equal to each other
    for (int i = 0; i < 3; ++i) {
        ASSERT_EQ(Out_with_null[i], Out_with_cargo[i],
                  "Out_with_null and Out_with_cargo should be identical for cargo test");
    }
}

// Test 4: Repeated calls produce consistent results (stability check)
static void test_repeated_calls_consistency() {
    cmsUInt16Number In[3] = {4096, 8192, 16384};
    cmsUInt16Number Out1[3];
    cmsUInt16Number Out2[3];
    int ret1 = ITU2PCS(In, Out1, nullptr);
    int ret2 = ITU2PCS(In, Out2, nullptr);

    ASSERT_TRUE(ret1 != 0, "First ITU2PCS call should succeed");
    ASSERT_TRUE(ret2 != 0, "Second ITU2PCS call should succeed");
    for (int i = 0; i < 3; ++i) {
        ASSERT_EQ(Out1[i], Out2[i], "Repeated ITU2PCS calls should produce identical outputs");
    }
}

// Entry point for tests
int main(int argc, char* argv[]) {
    (void)argc; (void)argv; // suppress unused parameter warnings

    std::cout << "Starting ITU2PCS unit tests...\n";

    test_basic_roundtrip();
    test_boundary_values();
    test_cargo_unused();
    test_repeated_calls_consistency();

    if (g_failures == 0) {
        std::cout << "All ITU2PCS tests passed.\n";
        return 0;
    } else {
        std::cerr << g_failures << " ITU2PCS test(s) failed.\n";
        return 1;
    }
}