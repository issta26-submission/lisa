// Automated unit tests for the focal method: sqlite3TouchRegister
// This test harness is self-contained and does not rely on GTest.
// It uses a minimal Parse structure with only the members required by the focal method.

#include <iostream>
#include <sqliteInt.h>
#include <string>


// FOCAL METHOD (exposed in this test as-is for validation)
void sqlite3TouchRegister(struct Parse *pParse, int iReg){
  if( pParse->nMem<iReg ) pParse->nMem = iReg;
}

// Minimal class-dependency representation (subset required for sqlite3TouchRegister testing)
struct Parse {
  int nMem; // represents the memory/register counter used by sqlite3TouchRegister
};

// Namespace to contain tests and avoid global collisions
namespace sqlite3TouchRegisterTests {

  // Simple test harness state
  static int gTestsRun = 0;
  static int gTestsFailed = 0;

  // Utility assertion to preserve test flow (non-terminating, logs on failure)
  static void assertTrue(bool condition, const std::string& description, int line) {
    ++gTestsRun;
    if(!condition){
      ++gTestsFailed;
      std::cerr << "[FAIL] " << description << " (line " << line << ")" << std::endl;
    }
  }

  // Helper macro to capture line information automatically
  #define ASSERT(cond, desc) assertTrue((cond), (desc), __LINE__)

  // Test 1: When pParse->nMem < iReg, nMem should be updated to iReg
  // This validates the true-branch of the conditional.
  bool test_update_when_nMem_less_than_iReg() {
    // Arrange
    Parse p;
    p.nMem = 0; // initial memory usage
    int iReg = 4;

    // Act
    sqlite3TouchRegister(&p, iReg);

    // Assert
    ASSERT(p.nMem == iReg, "Expected nMem to be updated to iReg when nMem < iReg");
    return true;
  }

  // Test 2: When pParse->nMem >= iReg, nMem should remain unchanged
  // This validates the false-branch of the conditional.
  bool test_no_update_when_nMem_ge_iReg() {
    // Arrange
    Parse p;
    p.nMem = 10;
    int iReg = 7;

    // Act
    sqlite3TouchRegister(&p, iReg);

    // Assert
    ASSERT(p.nMem == 10, "Expected nMem to remain unchanged when nMem >= iReg");
    return true;
  }

  // Test 3: Negative iReg should still behave correctly
  // Verifies that a negative register index does not cause an erroneous update
  bool test_negative_iReg_behavior() {
    // Arrange
    Parse p;
    p.nMem = -5;
    int iReg = -2;

    // Act
    sqlite3TouchRegister(&p, iReg);

    // Assert
    // Since -2 > -5, the condition pParse->nMem < iReg holds true, and nMem should become -2
    ASSERT(p.nMem == iReg, "Expected nMem to update to a less negative iReg when iReg is negative and greater than current nMem");
    return true;
  }

  // Test 4: Multiple successive calls update appropriately
  // Ensures state persists across multiple invocations
  bool test_multiple_calls_update_then_no_change() {
    // Arrange
    Parse p;
    p.nMem = 2;
    int firstCall = 6;
    int secondCall = 4;

    // Act
    sqlite3TouchRegister(&p, firstCall); // should update to 6
    sqlite3TouchRegister(&p, secondCall); // should stay 6

    // Assert
    ASSERT(p.nMem == firstCall, "Expected nMem to update to the first iReg (6) and not change on a smaller subsequent iReg (4)");
    return true;
  }

  // Run all tests and report aggregated results
  int runAll() {
    bool r1 = test_update_when_nMem_less_than_iReg();
    bool r2 = test_no_update_when_nMem_ge_iReg();
    bool r3 = test_negative_iReg_behavior();
    bool r4 = test_multiple_calls_update_then_no_change();

    // Summary
    std::cout << "sqlite3TouchRegisterTests: "
              << gTestsRun << " tests run, "
              << gTestsFailed << " failures."
              << std::endl;

    // Return non-zero if any test failed
    return (gTestsFailed==0) ? 0 : 1;
  }

} // namespace sqlite3TouchRegisterTests

// Expose a C++ main to run tests (as required when GTest is not used)
int main() {
  // Execute the test suite
  int rc = sqlite3TouchRegisterTests::runAll();
  return rc;
}