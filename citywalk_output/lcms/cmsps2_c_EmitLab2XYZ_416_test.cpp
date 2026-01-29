// Test suite for EmitLab2XYZ in cmsps2.c without using GTest
// This test uses a small in-file redirection of the internal _cmsIOPrintf
// to capture the emitted PostScript-like resource into a C++ string.
// We include cmsps2.c directly with a macro hook to MockCmsIOPrintf.
// The tests are self-contained, non-terminating (no premature exits on failure),
// and rely only on the C++ standard library and the provided headers via cmsps2.c.

#include <cstdio>
#include <cmsps2.c>
#include <iostream>
#include <string>
#include <cstdarg>
#include <lcms2_internal.h>


// Global output buffer to capture all prints performed by EmitLab2XYZ
static std::string gOutput;

// Mock implementation for the internal _cmsIOPrintf.
// We intentionally take the first parameter as void* to avoid needing
// a concrete cmsIOHANDLER type at this point of compilation.
// The function formats the incoming variadic arguments and appends
// them to the global output buffer.
int MockCmsIOPrintf(void* /*m*/, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    // Large enough buffer to hold a single formatted line
    char buf[4096];
    int n = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (n > 0) {
        gOutput.append(buf, static_cast<size_t>(n));
    }
    return n;
}

// Redirect _cmsIOPrintf(m, ...) calls to our MockCmsIOPrintf
#define _cmsIOPrintf(m, ...) MockCmsIOPrintf((void*)(m), __VA_ARGS__)

// Include the focal implementation file (contains EmitLab2XYZ)
// We rely on the macro above to capture its output.
// This approach allows us to test a single function in isolation without
// requiring a full test harness like GTest and without modifying the original
// library to add test hooks.

// Helper: simple assertion with console output, non-terminating
static void assert_true(bool cond, const std::string& msg) {
    if (cond) {
        std::cout << "[PASS] " << msg << "\n";
    } else {
        std::cout << "[FAIL] " << msg << "\n";
    }
}

// Domain-knowledge guided tests
// Step 2: Test EmitLab2XYZ output against the exact expected PostScript-like string.
// We capture all emitted text into gOutput via the _cmsIOPrintf redirection.
static bool TestEmitLab2XYZ_OutputMatches() {
    gOutput.clear();
    // The focal function prints into the provided I/O handle.
    // We pass nullptr here; the MockCmsIOPrintf ignores the handle.
    EmitLab2XYZ(nullptr);

    // Expected exact output as produced by cmsps2.c EmitLab2XYZ
    const std::string expected =
        "/RangeABC [ 0 1 0 1 0 1]\n"
        "/DecodeABC [\n"
        "{100 mul  16 add 116 div } bind\n"
        "{255 mul 128 sub 500 div } bind\n"
        "{255 mul 128 sub 200 div } bind\n"
        "]\n"
        "/MatrixABC [ 1 1 1 1 0 0 0 0 -1]\n"
        "/RangeLMN [ -0.236 1.254 0 1 -0.635 1.640 ]\n"
        "/DecodeLMN [\n"
        "{dup 6 29 div ge {dup dup mul mul} {4 29 div sub 108 841 div mul} ifelse 0.964200 mul} bind\n"
        "{dup 6 29 div ge {dup dup mul mul} {4 29 div sub 108 841 div mul} ifelse } bind\n"
        "{dup 6 29 div ge {dup dup mul mul} {4 29 div sub 108 841 div mul} ifelse 0.824900 mul} bind\n"
        "]\n";

    bool ok = (gOutput == expected);
    return ok;
}

// Step 3: Refinement test to ensure repeated invocations do not crash and
// produce stable output for each isolated invocation.
// We simulate two separate calls by clearing the buffer between calls and
// verifying the output of each call matches the expected string.
static bool TestEmitLab2XYZ_MultipleCallsProducesConsistentOutput() {
    gOutput.clear();
    EmitLab2XYZ(nullptr);
    const std::string first = gOutput;

    gOutput.clear();
    EmitLab2XYZ(nullptr);
    const std::string second = gOutput;

    // Both invocations should produce identical output when using the same IO
    // capture mechanism.
    bool ok = (first == second) && !first.empty();
    return ok;
}

// Step 3: Public test harness that runs all tests and reports results.
int main() {
    std::cout << "Running EmitLab2XYZ unit tests (CMS PS resource emission) ...\n";

    bool t1 = TestEmitLab2XYZ_OutputMatches();
    assert_true(t1, "EmitLab2XYZ output matches the expected PostScript resource content.");

    bool t2 = TestEmitLab2XYZ_MultipleCallsProducesConsistentOutput();
    assert_true(t2, "EmitLab2XYZ produces consistent output across multiple invocations with isolated IO.");

    // Overall exit status: 0 on success, non-zero on any failure
    if (t1 && t2) {
        std::cout << "[SUMMARY] All tests PASSED.\n";
        return 0;
    } else {
        std::cout << "[SUMMARY] Some tests FAILED.\n";
        return 1;
    }
}