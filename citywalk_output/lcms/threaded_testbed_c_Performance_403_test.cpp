/*
  Unit test suite for the focal method: Performance (as found in threaded_testbed.c)

  Context and approach:
  - The method under test is:
      cmsFloat64Number Performance(const char* Title, perf_fn fn, cmsContext ct,
                                   const char* inICC, const char* outICC,
                                   size_t sz, cmsFloat64Number prev)
    It loads input/output profiles, executes a provided function pointer (perf_fn),
    prints timing information, and conditionally prints a speed-up factor when prev > 0.

  - The tests below aim to validate:
    a) Basic return value consistency through the perf_fn pointer.
    b) Correct stdout formatting, including:
       - Title presence
       - The two numeric outputs (n and n*sz) with two decimals
       - The optional speed-up indicator when prev > 0 and imp > 1
    c) True/false branching behavior for the conditional printing of the (x ...) block.

  - To observe stdout without affecting the global test flow, stdout is temporarily redirected
    to an in-memory stream using open_memstream, then restored.

  Assumptions and notes:
  - The code is intended to be compiled and linked in an environment where threaded_testbed.c
    (containing the actual Performance implementation and its dependencies) is part of the
    build. The test harness declares the required C interfaces (types and function pointer type)
    and provides three dedicated perf_fn implementations to exercise various branches.
  - The tests focus on behavior, not on actual ICC profile parsing. A minimal dummy approach
    is used for inICC/outICC; loadProfile is assumed to be a no-op in the test environment
    or returns a non-null stub; this harness does not depend on its concrete behavior.

  How to run (conceptual):
  - Compile threaded_testbed.c with the test file, ensuring the C interfaces are linked.
  - Run the produced executable to observe PASS/FAIL messages for tests A, B, and C.
*/

#include <cstring>
#include <cstdio>
#include <threaded_internal.h>
#include <string>
#include <iostream>
#include <time.h>
#include <cstdarg>
#include <cassert>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>


// Domain types and function pointer (as declared by the original code)
extern "C" {
    // Lightweight typedefs to match the signatures used by Performance
    typedef void* cmsContext;
    typedef void* cmsHPROFILE;
    typedef double cmsFloat64Number;

    // perf_fn: function pointer type expected by Performance
    typedef cmsFloat64Number (*perf_fn)(cmsContext, cmsHPROFILE, cmsHPROFILE);

    // Prototypes of dependencies (as expected by the focal method)
    cmsHPROFILE loadProfile(const char* name);
    void trace(const char* frm, ...);

    // The focal method under test
    cmsFloat64Number Performance(const char* Title, perf_fn fn, cmsContext ct,
                                 const char* inICC, const char* outICC,
                                 size_t sz, cmsFloat64Number prev);
}

// Forward declarations of test-specific perf_fn implementations
extern "C" cmsFloat64Number perf_fn_4pt4(cmsContext, cmsHPROFILE, cmsHPROFILE);
extern "C" cmsFloat64Number perf_fn_5pt0(cmsContext, cmsHPROFILE, cmsHPROFILE);
extern "C" cmsFloat64Number perf_fn_3pt0(cmsContext, cmsHPROFILE, cmsHPROFILE);

// Helper: capture stdout produced during a Performance call
static std::string capture_Performance_Output(const char* Title,
                                            perf_fn fn,
                                            cmsContext ct,
                                            const char* inICC,
                                            const char* outICC,
                                            size_t sz,
                                            cmsFloat64Number prev)
{
    // Redirect stdout to an in-memory stream
    fflush(stdout);

    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1) {
        // Fallback: run without capture on error
        cmsFloat64Number ret = Performance(Title, fn, ct, inICC, outICC, sz, prev);
        (void)ret;
        return "";
    }

    char* buffer = nullptr;
    size_t buffer_size = 0;
    FILE* memstream = open_memstream(&buffer, &buffer_size);
    if (memstream == nullptr) {
        // Fallback: run without capture on error
        cmsFloat64Number ret = Performance(Title, fn, ct, inICC, outICC, sz, prev);
        (void)ret;
        close(saved_stdout);
        return "";
    }

    // Redirect stdout to memstream
    fflush(stdout);
    if (dup2(fileno(memstream), fileno(stdout)) == -1) {
        // If redirection fails, run without capture
        fclose(memstream);
        free(buffer);
        close(saved_stdout);
        cmsFloat64Number ret = Performance(Title, fn, ct, inICC, outICC, sz, prev);
        (void)ret;
        return "";
    }

    // Call the function under test
    cmsFloat64Number ret = Performance(Title, fn, ct, inICC, outICC, sz, prev);

    // Flush and restore stdout
    fflush(stdout);
    fclose(memstream); // this finalizes the captured content into buffer
    dup2(saved_stdout, fileno(stdout));
    close(saved_stdout);

    // Build std::string from captured buffer
    std::string output;
    if (buffer && buffer_size > 0) {
        output.assign(buffer, buffer_size);
    }

    // Clean up
    if (buffer) free(buffer);

    (void)ret; // silence unused in case not asserted
    return output;
}

// Test 1: perf_fn returns 4.4, prev = 0.0, sz = 5
// Expect: Title appears, "4.40" and "22.00" appear, no speed-up "(x ...)" is printed.
extern "C" cmsFloat64Number perf_fn_4pt4(cmsContext, cmsHPROFILE, cmsHPROFILE)
{
    return 4.4;
}

// Test 2: perf_fn returns 5.0, prev = 2.0, sz = 2
// Expect: Title, "5.00", "10.00" and speed-up "(x 2.5)" printed.
extern "C" cmsFloat64Number perf_fn_5pt0(cmsContext, cmsHPROFILE, cmsHPROFILE)
{
    return 5.0;
}

// Test 3: perf_fn returns 3.0, prev = 4.0, sz = 1
// Expect: Title, "3.00", "3.00" printed, but no speed-up since imp = 0.75 <= 1.0.
extern "C" cmsFloat64Number perf_fn_3pt0(cmsContext, cmsHPROFILE, cmsHPROFILE)
{
    return 3.0;
}

// Simple substring helper
static bool contains(const std::string& s, const std::string& sub)
{
    return s.find(sub) != std::string::npos;
}

// Test A: Basic output and return value with prev <= 0
static void test_Performance_Basic_NoPrev()
{
    const char* Title = "Performance_Basic_NoPrev";
    std::string out = capture_Performance_Output(Title, perf_fn_4pt4, nullptr, "in.icc", "out.icc", 5, 0.0);

    bool ok = true;
    if (!contains(out, Title)) ok = false;
    if (!contains(out, "4.40")) ok = false;     // n
    if (!contains(out, "22.00")) ok = false;    // n * sz
    if (contains(out, "(x")) ok = false;        // should not print imp
    printf("Test A - Basic NoPrev: %s\n", ok ? "PASS" : "FAIL");
}

// Test B: prev > 0 and imp > 1 triggers speed-up print
static void test_Performance_WithSpeedup()
{
    const char* Title = "Performance_WithSpeedup";
    std::string out = capture_Performance_Output(Title, perf_fn_5pt0, nullptr, "in.icc", "out.icc", 2, 2.0);

    bool ok = true;
    if (!contains(out, Title)) ok = false;
    if (!contains(out, "5.00")) ok = false;     // n
    if (!contains(out, "10.00")) ok = false;    // n * sz
    if (!contains(out, "(x 2.5)")) ok = false;  // imp = 2.5
    printf("Test B - WithSpeedup: %s\n", ok ? "PASS" : "FAIL");
}

// Test C: prev > 0 but imp <= 1 does not print the speed-up
static void test_Performance_NoSpeedupWhenLowImp()
{
    const char* Title = "Performance_NoSpeedupWhenLowImp";
    std::string out = capture_Performance_Output(Title, perf_fn_3pt0, nullptr, "in.icc", "out.icc", 1, 4.0);

    bool ok = true;
    if (!contains(out, Title)) ok = false;
    if (!contains(out, "3.00")) ok = false;     // n
    if (!contains(out, "3.00")) ok = false;     // n * sz
    if (contains(out, "(x")) ok = false;         // imp = 0.75; should not print
    printf("Test C - NoSpeedupWhenLowImp: %s\n", ok ? "PASS" : "FAIL");
}

// Main: run all tests
int main()
{
    // Step 2/3: Execute generated tests
    test_Performance_Basic_NoPrev();
    test_Performance_WithSpeedup();
    test_Performance_NoSpeedupWhenLowImp();

    return 0;
}

/*
  Explanation of test cases (mapping to Candidate Keywords and behaviors):
  - Candidate Keywords extracted from the focal method:
    Performance, Title, perf_fn, ct (cmsContext), inICC/outICC (profile paths),
    sz (size), prev (previous metric), loadProfile, trace.

  - Test A (Basic_NoPrev) covers:
    - prev <= 0.0 branch: no special "(x ...)" printing.
    - Correct emission of n and n*sz.
    - Basic formatting check for the Title and numeric outputs.

  - Test B (WithSpeedup) covers:
    - prev > 0.0 and imp > 1.0 path: ensures the "(x %-2.1f)" portion is printed.
    - Verifies the exact expected numbers including n, n*sz, and the speed-up factor.

  - Test C (NoSpeedupWhenLowImp) covers:
    - prev > 0.0 but imp <= 1.0: ensures the speed-up indicator is not printed.
    - Verifies n and n*sz formatting remains correct.

  - The tests use in-memory stdout capture for non-terminating assertions and rely on
    simple substring checks (content-based) to verify output rather than memory comparison.
*/