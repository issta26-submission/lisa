// test_lsm_ckpt_nwrite.cpp
// Unit test for: u32 lsmCheckpointNWrite(u32 *aCkpt, int bDisk)
// 
// This test suite validates the behavior of lsmCheckpointNWrite
// for both branches of the bDisk predicate (true/false).
// It uses a lightweight, non-terminating assertion style suitable
// for a C++11 environment without Google Test.
//
// Notes:
// - We rely on the project-provided types (u32, u8, etc.) and CKPT_HDR_NWRITE
//   from lsmInt.h.
// - A simple lsmGetU32 mock is provided to keep tests deterministic.
// - The test suite is self-contained and can be compiled alongside the
//   existing C sources (e.g., lsm_ckpt.c) in a C++11 build.

#include <lsmInt.h>
#include <iostream>
#include <cstring>
#include <cstdint>


// Include project dependencies to ensure CKPT_HDR_NWRITE and types are available.

// Forward declarations of the function under test (C linkage for C functions)
extern "C" u32 lsmCheckpointNWrite(u32 *aCkpt, int bDisk);

// Provide a lightweight mock for lsmGetU32 used by lsmCheckpointNWrite
// This ensures a deterministic, endianness-safe interpretation of 4 bytes.
extern "C" u32 lsmGetU32(u8 *p) {
    // Assemble a 32-bit value from 4 little-endian bytes
    return (u32)p[0] | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24);
}

// Simple non-terminating assertion framework
static int g_testFailures = 0;
static int g_testsRun = 0;

#define EXPECT_EQ(expected, actual, msg) do { \
    ++g_testsRun; \
    if ((expected) != (actual)) { \
        ++g_testFailures; \
        std::cerr << "[FAIL] " << msg << " | Expected 0x" << std::hex << (expected) \
                  << ", got 0x" << (actual) << std::dec << std::endl; \
    } \
} while(0)

#define EXPECT_EQ_U32(expected, actual, msg) EXPECT_EQ(static_cast<u32>(expected), static_cast<u32>(actual), msg)

// Define a helper to perform a single test and report its name
#define RUN_TEST(testFunc) do { \
    std::cout << "RUN: " #testFunc " ... "; \
    testFunc(); \
    std::cout << "DONE" << std::endl; \
} while(0)

// Test 1: bDisk = 0 (false) should return the checkpoint's NWRITE value directly.
static void test_lsmCheckpointNWrite_falseBranch_ReturnsDirectValue() {
    // CKPT_HDR_NWRITE is the index for "nwrite" field in the checkpoint array.
    // Allocate a safe array large enough to cover that index.
    const int idx = CKPT_HDR_NWRITE;
    // Use a reasonable size, ensure we have at least one element at the index.
    const int arrSize = (idx >= 0 ? idx + 1 : 4);
    u32 aCkpt[arrSize];
    // Test with multiple values to ensure correctness across different inputs.
    aCkpt[idx] = 0x11223344u;
    u32 res = lsmCheckpointNWrite(aCkpt, 0); // bDisk = false
    EXPECT_EQ_U32(0x11223344u, res, "bDisk=false should return aCkpt[CKPT_HDR_NWRITE]");

    aCkpt[idx] = 0xAABBCCDDu;
    res = lsmCheckpointNWrite(aCkpt, 0);
    EXPECT_EQ_U32(0xAABBCCDDu, res, "bDisk=false should return updated aCkpt[CKPT_HDR_NWRITE]");
}

// Test 2: bDisk = 1 (true) should read the 4-byte value from memory via lsmGetU32.
// This validates the indirect path through lsmGetU32 and ensures endianness handling is consistent
// with the stored value in aCkpt[CKPT_HDR_NWRITE].
static void test_lsmCheckpointNWrite_trueBranch_UsesGetU32() {
    const int idx = CKPT_HDR_NWRITE;
    const int arrSize = (idx >= 0 ? idx + 1 : 4);
    u32 aCkpt[arrSize];
    aCkpt[idx] = 0x12345678u; // some test value

    u32 res = lsmCheckpointNWrite(aCkpt, 1); // bDisk = true
    // Since lsmGetU32 reconstructs from bytes of aCkpt[idx], the value should equal the stored one.
    EXPECT_EQ_U32(0x12345678u, res, "bDisk=true should return value via lsmGetU32 on aCkpt[CKPT_HDR_NWRITE]");
}

// Test 3: Multiple values ensure stability across different checkpoints
static void test_lsmCheckpointNWrite_multipleValues() {
    const int idx = CKPT_HDR_NWRITE;
    const int arrSize = (idx >= 0 ? idx + 1 : 4);
    u32 aCkpt[arrSize];

    // value 1
    aCkpt[idx] = 0x0; // boundary case
    u32 resFalse = lsmCheckpointNWrite(aCkpt, 0);
    u32 resTrue  = lsmCheckpointNWrite(aCkpt, 1);
    EXPECT_EQ_U32(0x0, resFalse, "bDisk=false with 0 value");
    EXPECT_EQ_U32(0x0, resTrue, "bDisk=true with 0 value");

    // value 2
    aCkpt[idx] = 0xFFFFFFFFu;
    resFalse = lsmCheckpointNWrite(aCkpt, 0);
    resTrue  = lsmCheckpointNWrite(aCkpt, 1);
    EXPECT_EQ_U32(0xFFFFFFFFu, resFalse, "bDisk=false with 0xFFFFFFFF");
    EXPECT_EQ_U32(0xFFFFFFFFu, resTrue, "bDisk=true with 0xFFFFFFFF");
}

// Runner for all tests
static void run_all_tests() {
    RUN_TEST(test_lsmCheckpointNWrite_falseBranch_ReturnsDirectValue);
    RUN_TEST(test_lsmCheckpointNWrite_trueBranch_UsesGetU32);
    RUN_TEST(test_lsmCheckpointNWrite_multipleValues);

    // Summary
    std::cout << "Tests run: " << g_testsRun << ", Failures: " << g_testFailures << std::endl;
}

// Main function as required when not using a test framework
int main() {
    run_all_tests();
    if (g_testFailures != 0) {
        std::cerr << "Some tests FAILED." << std::endl;
        return 1;
    }
    std::cout << "All tests PASSED." << std::endl;
    return 0;
}