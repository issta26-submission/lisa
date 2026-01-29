// Unit test suite for PrelinEval16 in cmsopt.c
// - Uses direct inclusion of cmsopt.c to obtain exact struct/layout
// - No GoogleTest; a simple test harness with non-terminating checks
// - Tests cover single input/output path, multiple inputs, and zero-sized cases

#include <cstring>
#include <cstdio>
#include <cmsopt.c>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>


// Bring cmsopt.c (which contains PrelinEval16 and Prelin16Data) into scope.
// Ensure C linkage for the included C file.
extern "C" {
}

// Forward declare test curve functions with the assumed signatures used by cmsopt.c.
// We rely on the actual types defined inside cmsopt.c after including it.
static void MyInputCurve1(const cmsUInt16Number* In, cmsUInt16Number* Out, const void* P);
static void MyCLUT1(const cmsUInt16Number* In, cmsUInt16Number* Out, const void* P);
static void MyOutputCurve1(const cmsUInt16Number* In, cmsUInt16Number* Out, const void* P);

static void MyInputCurve2(const cmsUInt16Number* In, cmsUInt16Number* Out, const void* P);
static void MyCLUT2(const cmsUInt16Number* In, cmsUInt16Number* Out, const void* P);
static void MyOutputCurve2(const cmsUInt16Number* In, cmsUInt16Number* Out, const void* P);

static bool Test_SingleInputOutput();
static bool Test_MultipleInputsOutputs();
static bool Test_ZeroSizes();

// Simple non-terminating assertion helper for integers
static void check_eq_int(const char* note, int got, int expected) {
    if (got != expected) {
        std::cerr << "CHECK FAILED: " << note << " | got: " << got << " expected: " << expected << "\n";
    } else {
        // Optional: print success for debugging
        // std::cout << "CHECK PASSED: " << note << "\n";
    }
}

// Test 1: Single input, single output
static bool Test_SingleInputOutput() {
    // Prepare a Prelin16Data instance (layout defined in cmsopt.c)
    Prelin16Data p16;

    // Configure for 1 input, 1 output
    p16.nInputs = 1;
    p16.nOutputs = 1;

    // Assign curve evaluators
    p16.EvalCurveIn16[0] = MyInputCurve1; // identity curve
    p16.ParamsCurveIn16[0] = NULL;

    // CLUT function: multiplies input by 2
    p16.CLUTparams = NULL;
    p16.EvalCLUT = MyCLUT1;

    // Output curve: adds 3 to the CLUT output
    p16.EvalCurveOut16[0] = MyOutputCurve1;
    p16.ParamsCurveOut16[0] = NULL;

    // Input and output buffers
    cmsUInt16Number Input[1]  = { 4 };   // StageABC input
    cmsUInt16Number Output[1] = { 0 };   // will be filled

    // Execute
    PrelinEval16(Input, Output, &p16);

    // Expected: Input 4 -> CLUT 8 -> Output 11 (8 + 3)
    check_eq_int("Test1 Output[0]", (int)Output[0], 11);

    // Return true if test passes (Output matches expected)
    return (Output[0] == 11);
}

// Test 2: Two inputs, two outputs
static bool Test_MultipleInputsOutputs() {
    Prelin16Data p16;

    p16.nInputs = 2;
    p16.nOutputs = 2;

    // Use two identical input curves (identity)
    p16.EvalCurveIn16[0] = MyInputCurve2;
    p16.EvalCurveIn16[1] = MyInputCurve2;
    p16.ParamsCurveIn16[0] = NULL;
    p16.ParamsCurveIn16[1] = NULL;

    p16.CLUTparams = NULL;
    // CLUT uses a different function handling two inputs
    p16.EvalCLUT = MyCLUT2;

    p16.EvalCurveOut16[0] = MyOutputCurve2;
    p16.EvalCurveOut16[1] = MyOutputCurve2;
    p16.ParamsCurveOut16[0] = NULL;
    p16.ParamsCurveOut16[1] = NULL;

    cmsUInt16Number Input[2]  = {3, 5};   // StageABC inputs
    cmsUInt16Number Output[2] = {0, 0};   // outputs

    PrelinEval16(Input, Output, &p16);

    // Expected:
    // CLUT2: StageDEF[0] = In[0] + 1 = 4; StageDEF[1] = In[1] + 2 = 7
    // Output curve: Output[i] = StageDEF[i] + 3
    // => Output[0] = 7, Output[1] = 10
    check_eq_int("Test2 Output[0]", (int)Output[0], 7);
    check_eq_int("Test2 Output[1]", (int)Output[1], 10);

    return (Output[0] == 7 && Output[1] == 10);
}

// Test 3: Zero inputs/outputs should leave Output untouched
static bool Test_ZeroSizes() {
    Prelin16Data p16;

    p16.nInputs = 0;
    p16.nOutputs = 0;
    p16.CLUTparams = NULL;
    p16.EvalCLUT = NULL;

    // Provide some dummy curves (won't be called)
    p16.EvalCurveIn16[0] = nullptr;
    p16.EvalCurveOut16[0] = nullptr;
    p16.ParamsCurveIn16[0] = NULL;
    p16.ParamsCurveOut16[0] = NULL;

    cmsUInt16Number Input[1]  = { 123 };
    cmsUInt16Number Output[1] = { 0xFFFF }; // sentinel to detect modification

    PrelinEval16(Input, Output, &p16);

    // Expect Output to remain unchanged
    check_eq_int("Test3 Output[0] unchanged", (int)Output[0], (int)0xFFFF);

    return (Output[0] == 0xFFFF);
}

// Implementation of test curve functions (signatures must match cmsopt.c's internal typedefs)
static void MyInputCurve1(const cmsUInt16Number* In, CmsUInt16Number* Out, const void* P) {
    // identity
    *Out = *In;
}
static void MyCLUT1(const cmsUInt16Number* In, cmsUInt16Number* Out, const void* P) {
    // Only single input in Test1
    Out[0] = In[0] * 2; // simple scaling
}
static void MyOutputCurve1(const cmsUInt16Number* In, cmsUInt16Number* Out, const void* P) {
    // add a constant to the stage DEF value
    *Out = In[0] + 3;
}

// Test2 variants (nInputs == 2)
static void MyInputCurve2(const cmsUInt16Number* In, cmsUInt16Number* Out, const void* P) {
    *Out = *In; // identity per input
}
static void MyCLUT2(const cmsUInt16Number* In, cmsUInt16Number* Out, const void* P) {
    // Support two inputs
    Out[0] = In[0] + 1;
    Out[1] = In[1] + 2;
}
static void MyOutputCurve2(const cmsUInt16Number* In, cmsUInt16Number* Out, const void* P) {
    // add a constant to StageDEF value
    *Out = In[0] + 3;
}

int main() {
    // Run tests; report summary
    std::cout << "Running PrelinEval16 unit tests...\n";

    bool t1 = Test_SingleInputOutput();
    bool t2 = Test_MultipleInputsOutputs();
    bool t3 = Test_ZeroSizes();

    int passed = (t1 ? 1 : 0) + (t2 ? 1 : 0) + (t3 ? 1 : 0);
    int total  = 3;

    std::cout << "Tests passed: " << passed << "/" << total << "\n";

    // Return 0 if all tests passed; otherwise non-zero
    return (passed == total) ? 0 : 1;
}

// Note:
// - The tests rely on the exact structure of Prelin16Data and function pointer types
//   as defined in cmsopt.c. By including cmsopt.c, the layout is consistent with
//   the implementation under test.
// - The tests use simple, deterministic curves to verify the flow: EvalCurveIn16,
//   EvalCLUT, and EvalCurveOut16 are invoked in sequence.
// - No GTest is used as requested; a lightweight main-based harness is provided.
// - Static helper functions are used for test-specific curve behavior.
// - Static members in the tested file, if present, are accessed through the included
//   cmsopt.c header/definitions; tests do not rely on private access beyond the
//   provided Prelin16Data interface.