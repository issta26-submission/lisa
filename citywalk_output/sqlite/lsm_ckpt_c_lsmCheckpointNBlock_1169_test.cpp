// Test suite for lsmCheckpointNBlock in lsm_ckpt.c
// This test is written in C++11 without using GoogleTest.
// It dynamically discovers the index CKPT_HDR_NBLOCK used by lsmCheckpointNBlock
// and validates that the function returns the value stored at that index.

#include <lsmInt.h>
#include <iostream>
#include <cstdint>
#include <vector>


// Extern C declaration to call the focal function from C code.
// We use unsigned int as aubiquitous 32-bit type for compatibility with common
// typedefs (u32) in C codebases. If u32 maps to a larger type on the target
// platform, linking should still work as the function only reads a memory
// location and returns its 32-bit value.
extern "C" unsigned int lsmCheckpointNBlock(unsigned int *aCkpt);

// Simple non-terminating assertion utility.
// Logs failures but does not exit, enabling full test coverage.
static void myAssert(bool cond, const char *msg) {
    if (!cond) {
        std::cerr << "ASSERT FAILED: " << msg << std::endl;
    }
}

// Entry point for the test suite.
// Step-by-step:
// 1) Detect CKPT_HDR_NBLOCK index by probing memory locations.
// 2) Validate that lsmCheckpointNBlock returns the value stored at that index.
// 3) Run a few additional value checks to increase coverage.
int main() {
    // Step 1: Detect CKPT_HDR_NBLOCK index at runtime.
    // We allocate a reasonably large array and test each position until
    // setting that position to a unique sentinel value makes the function
    // return that sentinel. This reveals CKPT_HDR_NBLOCK without relying on
    // hard-coded knowledge of its value.
    const int N = 256;
    std::vector<uint32_t> aCkpt(N, 0);

    const uint32_t sentinel = 0xA5A5A5A5;
    int detectedIdx = -1;

    for (int idx = 0; idx < N; ++idx) {
        // Reset all to zero
        for (int j = 0; j < N; ++j) aCkpt[j] = 0;
        // Place sentinel at position idx
        aCkpt[idx] = sentinel;

        unsigned int res = lsmCheckpointNBlock(aCkpt.data());

        if (res == sentinel) {
            detectedIdx = idx;
            break;
        }
    }

    myAssert(detectedIdx != -1, "Failed to detect CKPT_HDR_NBLOCK index. Ensure the function is linked correctly and the test array is valid.");

    // If detection succeeded, proceed with concrete tests.
    int idx = detectedIdx;
    std::cerr << "Detected CKPT_HDR_NBLOCK at index: " << idx << std::endl;

    // Step 2: Basic functional test - value propagation.
    // Set a known value at CKPT_HDR_NBLOCK and ensure function returns it.
    for (int j = 0; j < N; ++j) aCkpt[j] = 0;
    aCkpt[idx] = 12345u;
    unsigned int r1 = lsmCheckpointNBlock(aCkpt.data());
    myAssert(r1 == 12345u, "lsmCheckpointNBlock should return the value stored at CKPT_HDR_NBLOCK");

    // Step 3: Edge case tests - other values at CKPT_HDR_NBLOCK
    // Case 0
    for (int j = 0; j < N; ++j) aCkpt[j] = 0;
    aCkpt[idx] = 0u;
    unsigned int r0 = lsmCheckpointNBlock(aCkpt.data());
    myAssert(r0 == 0u, "lsmCheckpointNBlock should return 0 when the value at CKPT_HDR_NBLOCK is 0");

    // Case max uint32
    for (int j = 0; j < N; ++j) aCkpt[j] = 0;
    aCkpt[idx] = 0xFFFFFFFFu;
    unsigned int rMax = lsmCheckpointNBlock(aCkpt.data());
    myAssert(rMax == 0xFFFFFFFFu, "lsmCheckpointNBlock should handle max 32-bit value");

    // Additional sanity check: use a different non-zero value
    for (int j = 0; j < N; ++j) aCkpt[j] = 0;
    aCkpt[idx] = 0x12345678u;
    unsigned int rOther = lsmCheckpointNBlock(aCkpt.data());
    myAssert(rOther == 0x12345678u, "lsmCheckpointNBlock should return independently set values");

    std::cout << "lsmCheckpointNBlock tests completed. CKPT_HDR_NBLOCK index=" << idx << std::endl;
    return 0;
}