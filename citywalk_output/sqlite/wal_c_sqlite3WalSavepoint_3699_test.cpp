/*
  Unit test suite for the focal method:
  sqlite3WalSavepoint(Wal *pWal, u32 *aWalData)

  Step 1 (Program Understanding):
  - The function asserts that the Wal write lock is held (pWal->writeLock).
  - It copies 4 fields into aWalData:
      aWalData[0] = pWal->hdr.mxFrame
      aWalData[1] = pWal->hdr.aFrameCksum[0]
      aWalData[2] = pWal->hdr.aFrameCksum[1]
      aWalData[3] = pWal->nCkpt
  - Therefore, the core dependencies are: Wal struct, its hdr sub-structure (with mxFrame and aFrameCksum[2]), and nCkpt, plus the writeLock flag.

  Step 2 (Unit Test Generation):
  - Using the provided class dependencies block (which contains Wal, WalIndexHdr, etc.), we will exercise:
      1) True-branch behavior: writeLock = 1 and non-zero values for mxFrame, aFrameCksum[], and nCkpt; ensure aWalData receives the exact values from Wal.
      2) Variation of input data: different values to confirm mapping is stable and not dependent on specific numbers.
  - We avoid relying on private methods; we rely on the public function prototype and the Wal type defined in wal.h.
  - We avoid GTest; we implement a small standalone test harness in C++11 with non-terminating assertions (log-based).

  Step 3 (Test Case Refinement):
  - Ensure tests are self-contained, compile with C++11, and do not require GoogleTest.
  - Include explanatory comments for each test case describing what scenario is validated.
  - Use the Wal type and u32 type from wal.h to preserve type consistency with the focal method.
  - Present a minimal harness that reports PASS/FAIL without terminating on a single failure.

  Note: Because sqlite3WalSavepoint uses an assert(pWal->writeLock), testing the false branch (writeLock == 0) would terminate the test program due to assert. To maintain safety in unit tests, we only exercise the true-branch scenario and document this limitation in comments. The test suite is designed to maximize code execution coverage around the core data-copying logic.
*/

#include <Windows.h>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <wal.h>


// Bring in WAL definitions and the focal function with C linkage
extern "C" {
}

// Simple non-terminating test harness (non-GTest)
class WalSavepointTestHarness {
public:
    // Record a failure message
    void fail(const std::string& msg) {
        failures.push_back(msg);
    }

    // Run all tests and report results
    int run() {
        test_normal_case(); // Test 1: normal data propagation
        test_another_case(); // Test 2: different values
        report();
        return failures.empty() ? 0 : 1;
    }

private:
    std::vector<std::string> failures;

    // Test 1: Normal data propagation
    // Description:
    //   - Initialize a Wal instance with writeLock = 1
    //   - Fill hdr.mxFrame and hdr.aFrameCksum[] with known values
    //   - Set nCkpt to a known value
    //   - Call sqlite3WalSavepoint and verify all 4 entries in aWalData
    void test_normal_case() {
        Wal w;
        std::memset(&w, 0, sizeof(Wal));

        // true-branch conditions for the assertion
        w.writeLock = 1;
        w.hdr.mxFrame = 0x12345678;         // 4-byte value
        w.hdr.aFrameCksum[0] = 0x9abcdef0;   // first checksum part
        w.hdr.aFrameCksum[1] = 0x13579bdf;   // second checksum part
        w.nCkpt = 42;                        // checkpoint counter

        u32 aWalData[4];
        sqlite3WalSavepoint(&w, aWalData);

        // Validate mapping
        if (aWalData[0] != w.hdr.mxFrame) {
            fail("Test normal_case: aWalData[0] (mxFrame) mismatch");
            return;
        }
        if (aWalData[1] != w.hdr.aFrameCksum[0]) {
            fail("Test normal_case: aWalData[1] (aFrameCksum[0]) mismatch");
            return;
        }
        if (aWalData[2] != w.hdr.aFrameCksum[1]) {
            fail("Test normal_case: aWalData[2] (aFrameCksum[1]) mismatch");
            return;
        }
        if (aWalData[3] != w.nCkpt) {
            fail("Test normal_case: aWalData[3] (nCkpt) mismatch");
            return;
        }
        // If we reach here, the test passes
    }

    // Test 2: Another set of values to ensure general behavior
    void test_another_case() {
        Wal w;
        std::memset(&w, 0, sizeof(Wal));

        // true-branch conditions
        w.writeLock = 1;
        w.hdr.mxFrame = 0;                   // interesting edge value
        w.hdr.aFrameCksum[0] = 0xFFFFFFFF;   // large value
        w.hdr.aFrameCksum[1] = 0x0000ABCD;   // mixed value
        w.nCkpt = 123456;

        u32 aWalData[4];
        sqlite3WalSavepoint(&w, aWalData);

        // Validate mapping
        if (aWalData[0] != w.hdr.mxFrame) {
            fail("Test another_case: aWalData[0] (mxFrame) mismatch");
            return;
        }
        if (aWalData[1] != w.hdr.aFrameCksum[0]) {
            fail("Test another_case: aWalData[1] (aFrameCksum[0]) mismatch");
            return;
        }
        if (aWalData[2] != w.hdr.aFrameCksum[1]) {
            fail("Test another_case: aWalData[2] (aFrameCksum[1]) mismatch");
            return;
        }
        if (aWalData[3] != w.nCkpt) {
            fail("Test another_case: aWalData[3] (nCkpt) mismatch");
            return;
        }
    }

    // Report summary
    void report() {
        if (failures.empty()) {
            std::cout << "ALL TESTS PASSED\n";
        } else {
            std::cerr << failures.size() << " TEST(S) FAILED\n";
            for (const auto& s : failures) {
                std::cerr << s << "\n";
            }
        }
    }
};

// Explanation about test coverage and design decisions:
// - The focal function is a simple 4-field copier guarded by an assert on writeLock.
// - Our tests exercise the true-branch path by ensuring writeLock is non-zero.
// - We verify exact data propagation for mxFrame, aFrameCksum[0], aFrameCksum[1], and nCkpt.
// - A test to exercise the false branch (writeLock == 0) would trigger an assertion failure (abort),
//   which is undesirable in a non-terminating test harness. Therefore, we limit to true-branch coverage.
// - Static/internal helper functions in wal.c are not directly accessible here, consistent with the
//   requirement to avoid private members in tests. We rely on the public Wal struct from wal.h.

int main() {
    WalSavepointTestHarness harness;
    return harness.run();
}