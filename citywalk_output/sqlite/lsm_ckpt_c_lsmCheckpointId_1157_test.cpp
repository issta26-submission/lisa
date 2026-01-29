// C++11 test suite for the focal method: lsmCheckpointId (in lsm_ckpt.c)
// This test harness provides lightweight, non-terminating assertions and does not rely on Google Test.
// It covers both branches of the condition (bDisk == 0 and bDisk == 1) and validates 64-bit composition
// of the checkpoint ID using macros CKPT_HDR_ID_MSW and CKPT_HDR_ID_LSW from lsmInt.h.

#include <vector>
#include <lsmInt.h>
#include <iomanip>
#include <iostream>
#include <cstdint>


// Import C dependencies and types used by lsmCheckpointId
extern "C" {
}

// Declaration of the focal function (C linkage)
extern "C" {
    i64 lsmCheckpointId(u32 *aCkpt, int bDisk);
}

// Small helper: write a 32-bit value to a byte buffer in little-endian order
static void writeLE(uint8_t* dst, uint32_t val) {
    dst[0] = (uint8_t)(val & 0xFF);
    dst[1] = (uint8_t)((val >> 8) & 0xFF);
    dst[2] = (uint8_t)((val >> 16) & 0xFF);
    dst[3] = (uint8_t)((val >> 24) & 0xFF);
}

// Simple non-terminating test harness for individual test cases
struct TestCase {
    const char* name;
    bool diskBranch;      // true => bDisk = 1; false => bDisk = 0
    uint32_t msw;         // 32-bit most-significant word of the 64-bit ID
    uint32_t lsw;         // 32-bit least-significant word of the 64-bit ID
};

// Run a single test case and return true if it passes
static bool runCase(const TestCase& tc) {
    // Determine how many 32-bit words are needed to address both MSW/LSW indices
    int maxIndex = std::max(CKPT_HDR_ID_MSW, CKPT_HDR_ID_LSW) + 1;

    // Helper: compute the expected 64-bit ID from MSW/LSW values
    auto expectedId = [](uint32_t msw, uint32_t lsw) -> i64 {
        return (((i64)msw) << 32) | (i64)lsw;
    };

    // Case when bDisk == 0 (false branch)
    if (!tc.diskBranch) {
        // Use a properly typed array of 32-bit words
        std::vector<u32> aCkpt(maxIndex, 0);
        aCkpt[CKPT_HDR_ID_MSW] = tc.msw;
        aCkpt[CKPT_HDR_ID_LSW] = tc.lsw;

        // Call focal function
        i64 id = lsmCheckpointId(aCkpt.data(), 0);
        i64 expected = expectedId(tc.msw, tc.lsw);

        if (id != expected) {
            std::cerr << "[FAIL] " << tc.name << ": bDisk=0 produced id="
                      << std::setw(0) << id << " but expected=" << expected << "\n";
            return false;
        }

        // PASS
        return true;
    }

    // Case when bDisk == 1 (true branch)
    else {
        // Allocate 32-bit words, but we will fill their in-memory byte representation
        std::vector<u32> aCkpt(maxIndex, 0);
        // Cast to byte pointer to emulate the layout used by lsmCheckpointId
        uint8_t* p = reinterpret_cast<uint8_t*>(aCkpt.data());

        // Write MSW and LSW to their respective 4-byte segments in little-endian form
        writeLE(p + ((size_t)CKPT_HDR_ID_MSW * 4), tc.msw);
        writeLE(p + ((size_t)CKPT_HDR_ID_LSW * 4), tc.lsw);

        i64 id = lsmCheckpointId(aCkpt.data(), 1);
        i64 expected = expectedId(tc.msw, tc.lsw);

        if (id != expected) {
            std::cerr << "[FAIL] " << tc.name << ": bDisk=1 produced id="
                      << std::setw(0) << id << " but expected=" << expected << "\n";
            return false;
        }

        // PASS
        return true;
    }
}

int main() {
    // Define a concise set of test cases to cover both branches and several value combinations.
    // The values are chosen to exercise normal, boundary, and large 64-bit compositions.
    TestCase tests[] = {
        // False branch: small values
        {"lsmCheckpointId false branch (MSW=1, LSW=2)", false, 1, 2},
        // False branch: large MSW
        {"lsmCheckpointId false branch (MSW=0xFFFFFFFF, LSW=0)", false, 0xFFFFFFFFu, 0},
        // True branch: small values
        {"lsmCheckpointId true branch (MSW=1, LSW=2)", true, 1, 2},
        // True branch: large and max values
        {"lsmCheckpointId true branch (MSW=0xFFFFFFFF, LSW=0xFFFFFFFF)", true, 0xFFFFFFFFu, 0xFFFFFFFFu}
    };

    const int nTests = sizeof(tests) / sizeof(TestCase);
    int passed = 0;
    int failed = 0;

    for (int i = 0; i < nTests; ++i) {
        bool result = runCase(tests[i]);
        if (result) {
            std::cout << "[PASS] " << tests[i].name << "\n";
            ++passed;
        } else {
            ++failed;
        }
    }

    // Summary (non-terminating): report counts and exit code
    std::cout << "\nTest Summary: " << passed << " passed, " << failed << " failed, "
              << (passed + failed) << " total tests.\n";

    // Return non-zero if any test failed
    return (failed == 0) ? 0 : 1;
}