#include <lsmInt.h>
#include <iostream>
#include <cstdint>


// Declaration of the focal C function from lsm_varint.c
// We use C linkage to ensure correct name mangling when linking with C++ test code.
extern "C" int lsmVarintSize(unsigned char c);

// Simple non-terminating test harness for C++11 without any external testing framework.
static void runCase(const char* name, uint8_t input, int expected, int& failCount) {
    int got = lsmVarintSize(input);
    if (got != expected) {
        std::cerr << "[FAIL] " << name
                  << " | input=" << int(input)
                  << " | expected=" << expected
                  << " | got=" << got << "\n";
        ++failCount;
    } else {
        std::cout << "[PASS] " << name
                  << " | input=" << int(input)
                  << " | expected=" << expected
                  << " | got=" << got << "\n";
    }
}

int main() {
    int failCount = 0;

    // Step 2/3: Coverage for all branches of lsmVarintSize
    // Branch 1: if (c < 241) return 1;
    // Test values that should take the first branch
    runCase("varintSize_branch1_input_0",   0,   1, failCount);
    runCase("varintSize_branch1_input_240", 240, 1, failCount);

    // Branch 2: if (c < 249) return 2;
    // Test values that should take the second branch (241..248)
    runCase("varintSize_branch2_input_241", 241, 2, failCount);
    runCase("varintSize_branch2_input_248", 248, 2, failCount);

    // Branch 3: else return (int)(c - 246);
    // Test values that should take the third branch (>= 249)
    runCase("varintSize_branch3_input_249", 249, 3, failCount);
    runCase("varintSize_branch3_input_255", 255, 9, failCount);

    // Summary
    if (failCount > 0) {
        std::cerr << "[SUMMARY] Total failures: " << failCount << "\n";
    } else {
        std::cout << "[SUMMARY] All tests passed.\n";
    }

    return failCount;
}