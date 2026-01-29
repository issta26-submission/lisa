// Unit tests for the focal method SetRange in transicc.c
// This test suite is written for C++11 without any testing framework (e.g., GTest).
// It assumes the following external C symbols are available from the compiled transicc.c:
//   void SetRange(double range, unsigned char IsInput);
//   double InputRange;
//   double OutputRange;
//
// Note: We use extern "C" to ensure correct C linkage for the probe functions/variables.

#include <iostream>
#include <cmath>
#include <utils.h>


// Prototypes for the focal function and its global state as observed from tests.
// These declarations assume the transicc.c defines the symbols with the following types.
// If the actual project uses slightly different typedefs (e.g., cmsBool, cmsFloat64Number),
// this test may require adjustment to match the exact types.
extern "C" {
    void SetRange(double range, unsigned char IsInput);
    extern double InputRange;
    extern double OutputRange;
}

// Utility: compare doubles with a tolerance
static bool almost_equal(double a, double b, double eps = 1e-12) {
    return std::fabs(a - b) <= eps;
}

// Test 1: Verify that SetRange updates InputRange when IsInput is true
bool test_SetRange_UpdatesInputRange() {
    // Initialize known state
    InputRange = -1.0e9;
    OutputRange = -1.0e9;

    double testVal = 123.456789;
    SetRange(testVal, 1); // IsInput = true

    bool okInput = almost_equal(InputRange, testVal);
    bool okOutputUnchanged = almost_equal(OutputRange, -1.0e9);

    if (!okInput) {
        std::cerr << "Test 1 FAILED: InputRange should be updated to " << testVal
                  << " but was " << InputRange << std::endl;
    }
    if (!okOutputUnchanged) {
        std::cerr << "Test 1 FAILED: OutputRange changed unexpectedly to " << OutputRange << std::endl;
    }

    return okInput && okOutputUnchanged;
}

// Test 2: Verify that SetRange updates OutputRange when IsInput is false
bool test_SetRange_UpdatesOutputRange() {
    // Initialize known state
    InputRange = -2.0;
    OutputRange = -3.0;

    double testVal = 9.87654321;
    SetRange(testVal, 0); // IsInput = false

    bool okOutput = almost_equal(OutputRange, testVal);
    bool okInputUnchanged = almost_equal(InputRange, -2.0);

    if (!okOutput) {
        std::cerr << "Test 2 FAILED: OutputRange should be updated to " << testVal
                  << " but was " << OutputRange << std::endl;
    }
    if (!okInputUnchanged) {
        std::cerr << "Test 2 FAILED: InputRange changed unexpectedly to " << InputRange << std::endl;
    }

    return okOutput && okInputUnchanged;
}

// Test 3: Sequential updates should affect only the targeted range
bool test_SetRange_SequentialUpdates() {
    // Initialize known state
    InputRange = 0.5;
    OutputRange = 0.5;

    // Step 1: update InputRange
    SetRange(-7.25, 1);
    bool okInput1 = almost_equal(InputRange, -7.25);
    bool okOutput1 = almost_equal(OutputRange, 0.5); // OutputRange should remain as initially set

    // Step 2: update OutputRange
    SetRange(3.14159, 0);
    bool okOutput2 = almost_equal(OutputRange, 3.14159);
    bool okInput2 = almost_equal(InputRange, -7.25); // InputRange should remain unchanged

    if (!(okInput1 && okOutput1 && okOutput2 && okInput2)) {
        std::cerr << "Test 3 FAILED: Sequential updates did not isolate changes between ranges.\n"
                  << "  After first set: InputRange=" << InputRange << ", OutputRange=" << OutputRange << "\n"
                  << "  After second set: InputRange=" << InputRange << ", OutputRange=" << OutputRange << std::endl;
    }

    return okInput1 && okOutput1 && okOutput2 && okInput2;
}

// Test 4: Boundary values for range input
bool test_SetRange_BoundaryValues() {
    // Initialize known state
    InputRange = 1.0;
    OutputRange = -1.0;

    // Boundary: zero for InputRange
    SetRange(0.0, 1);
    bool okInputZero = almost_equal(InputRange, 0.0);
    bool okOutputZeroUnchanged = almost_equal(OutputRange, -1.0);

    // Boundary: large value for OutputRange
    SetRange(1.0e308, 0);
    bool okOutputLarge = almost_equal(OutputRange, 1.0e308);
    bool okInputUnchanged = almost_equal(InputRange, 0.0);

    if (!okInputZero) {
        std::cerr << "Test 4 FAILED: InputRange should be 0.0 after SetRange(0.0, 1); actual " << InputRange << std::endl;
    }
    if (!okOutputZeroUnchanged) {
        std::cerr << "Test 4 FAILED: OutputRange changed unexpectedly when setting InputRange to 0.0\n";
    }
    if (!okOutputLarge) {
        std::cerr << "Test 4 FAILED: OutputRange should be updated to 1.0e308; actual " << OutputRange << std::endl;
    }
    if (!okInputUnchanged) {
        std::cerr << "Test 4 FAILED: InputRange changed unexpectedly when updating OutputRange; actual " << InputRange << std::endl;
    }

    return okInputZero && okOutputZeroUnchanged && okOutputLarge && okInputUnchanged;
}

int main() {
    bool allTestsPassed = true;

    std::cout << "Running SetRange unit tests (no GTest, C++11)...\n";

    if (!test_SetRange_UpdatesInputRange()) {
        allTestsPassed = false;
        std::cerr << "=> Test 1 failed.\n";
    } else {
        std::cout << "=> Test 1 passed.\n";
    }

    if (!test_SetRange_UpdatesOutputRange()) {
        allTestsPassed = false;
        std::cerr << "=> Test 2 failed.\n";
    } else {
        std::cout << "=> Test 2 passed.\n";
    }

    if (!test_SetRange_SequentialUpdates()) {
        allTestsPassed = false;
        std::cerr << "=> Test 3 failed.\n";
    } else {
        std::cout << "=> Test 3 passed.\n";
    }

    if (!test_SetRange_BoundaryValues()) {
        allTestsPassed = false;
        std::cerr << "=> Test 4 failed.\n";
    } else {
        std::cout << "=> Test 4 passed.\n";
    }

    if (allTestsPassed) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cerr << "Some tests FAILED. See above for details.\n";
        return 1;
    }
}