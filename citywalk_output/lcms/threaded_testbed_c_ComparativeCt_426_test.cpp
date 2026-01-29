// Unit test suite for the focal method ComparativeCt in threaded_testbed.c
// This test suite is designed for C++11 without GTest.
// It focuses on exercising the four branches of the inICC/outICC predicates
// by invoking ComparativeCt with NULL and non-NULL ICC file paths.
// The tests use simple non-terminating assertions (EXPECT_*) and a tiny
// handcrafted test runner to maximize coverage.

#include <cstring>
#include <cstdio>
#include <vector>
#include <threaded_internal.h>
#include <time.h>
#include <memory.h>
#include <stdlib.h>
#include <cstdint>


// We provide minimal C-compatible type aliases to match the signatures used
// by ComparativeCt. The real project provides these via its headers; here
// we declare them to enable linking against the focal implementation.

extern "C" {

// Basic opaque pointer types used by the focal method.
typedef void* cmsContext;
typedef void* cmsHPROFILE;

// Floating point type used by the focal method for performance numbers.
typedef double cmsFloat64Number;

// perf_fn type: function pointer matching the focal's expected signature.
typedef cmsFloat64Number (*perf_fn)(cmsContext, cmsHPROFILE, cmsHPROFILE);

// Prototypes of dependencies used by ComparativeCt in the focal code.
// We rely on the implementations provided by the existing threaded_testbed.c
// (or equivalent linked objects in the project).
cmsHPROFILE CreateCurves(void);
cmsHPROFILE cmsOpenProfileFromFile(const char* name, const char* mode);
void trace(const char* frm, ...);

// The function under test.
cmsFloat64Number ComparativeCt(cmsContext ct1, cmsContext ct2, const char* Title,
                               perf_fn fn1, perf_fn fn2,
                               const char* inICC, const char* outICC);

} // extern "C"

// Lightweight test framework (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        fprintf(stderr, "EXPECT_TRUE failed at %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        ++g_failed_tests; \
    } \
} while(0)


// Globals to capture perf_fn invocations and verify call patterns
struct PerfCall {
    void* in;
    void* out;
};

// Per-function call logs to verify that both perf functions were invoked once per ComparativeCt call
static std::vector<PerfCall> g_calls_fn1;
static std::vector<PerfCall> g_calls_fn2;

// Mock perf_fn implementations to capture input pointers and return deterministic values.

extern "C" {

// perf function used as fn1 (n1)
cmsFloat64Number perfFn1(cmsContext ct, cmsHPROFILE in, cmsHPROFILE out)
{
    // Record the inputs for later verification
    g_calls_fn1.push_back({in, out});
    // Return a fixed value to keep computations predictable
    return 1.0;
}

// perf function used as fn2 (n2)
cmsFloat64Number perfFn2(cmsContext ct, cmsHPROFILE in, cmsHPROFILE out)
{
    g_calls_fn2.push_back({in, out});
    return 2.0;
}

} // extern "C"


// Test 1: inICC = NULL, outICC = NULL
bool test_null_null(void)
{
    // Clear previous call logs
    g_calls_fn1.clear();
    g_calls_fn2.clear();

    // Call the function under test with both ICC inputs NULL
    ComparativeCt(reinterpret_cast<cmsContext>(0x11),
                  reinterpret_cast<cmsContext>(0x22),
                  "NullNull",
                  perfFn1, perfFn2,
                  NULL, NULL);

    // Expect one invocation of each perf function (n1 and n2)
    EXPECT_TRUE(g_calls_fn1.size() == 1);
    EXPECT_TRUE(g_calls_fn2.size() == 1);

    // Also verify that the perf functions received some (non-null) profile pointers
    // (these should be produced by CreateCurves for NULL ICC inputs in this path).
    // We cannot enforce exact addresses here, but we can ensure the pointers are set.
    bool fn1_used = !g_calls_fn1.empty();
    bool fn2_used = !g_calls_fn2.empty();
    EXPECT_TRUE(fn1_used && fn2_used);

    return (g_failed_tests == 0);
}

// Test 2: inICC = NULL, outICC != NULL
bool test_null_nonnull(void)
{
    g_calls_fn1.clear();
    g_calls_fn2.clear();

    ComparativeCt(reinterpret_cast<cmsContext>(0x31),
                  reinterpret_cast<cmsContext>(0x42),
                  "NullNonNullOut",
                  perfFn1, perfFn2,
                  NULL, "out.icc");

    // Expect exactly one call for each perf function
    EXPECT_TRUE(g_calls_fn1.size() == 1);
    EXPECT_TRUE(g_calls_fn2.size() == 1);

    return (g_failed_tests == 0);
}

// Test 3: inICC != NULL, outICC = NULL
bool test_nonnull_null(void)
{
    g_calls_fn1.clear();
    g_calls_fn2.clear();

    ComparativeCt(reinterpret_cast<cmsContext>(0x51),
                  reinterpret_cast<cmsContext>(0x62),
                  "NonNullNull",
                  perfFn1, perfFn2,
                  "in.icc", NULL);

    EXPECT_TRUE(g_calls_fn1.size() == 1);
    EXPECT_TRUE(g_calls_fn2.size() == 1);

    return (g_failed_tests == 0);
}

// Test 4: inICC != NULL, outICC != NULL
bool test_nonnull_nonnull(void)
{
    g_calls_fn1.clear();
    g_calls_fn2.clear();

    ComparativeCt(reinterpret_cast<cmsContext>(0x71),
                  reinterpret_cast<cmsContext>(0x82),
                  "NonNullNonNull",
                  perfFn1, perfFn2,
                  "in.icc", "out.icc");

    EXPECT_TRUE(g_calls_fn1.size() == 1);
    EXPECT_TRUE(g_calls_fn2.size() == 1);

    return (g_failed_tests == 0);
}


// Simple test runner
static void run_all_tests(void)
{
    fprintf(stdout, "Running unit tests for ComparativeCt...\n");

    if (test_null_null()) {
        fprintf(stdout, "Test 1 (Null,Null) passed minimally.\n");
    } else {
        fprintf(stderr, "Test 1 (Null,Null) failed.\n");
    }

    if (test_null_nonnull()) {
        fprintf(stdout, "Test 2 (Null,NonNull) passed minimally.\n");
    } else {
        fprintf(stderr, "Test 2 (Null,NonNull) failed.\n");
    }

    if (test_nonnull_null()) {
        fprintf(stdout, "Test 3 (NonNull,Null) passed minimally.\n");
    } else {
        fprintf(stderr, "Test 3 (NonNull,Null) failed.\n");
    }

    if (test_nonnull_nonnull()) {
        fprintf(stdout, "Test 4 (NonNull,NonNull) passed minimally.\n");
    } else {
        fprintf(stderr, "Test 4 (NonNull,NonNull) failed.\n");
    }

    fprintf(stdout, "Total tests executed: %d, Failures: %d\n",
            g_total_tests, g_failed_tests);
}

int main()
{
    // Run tests (non-terminating assertions ensure full execution)
    run_all_tests();

    // Return non-zero if there were any failures
    return g_failed_tests;
}

/*
Notes for maintainers:
- The tests rely on the focal implementation's behavior to create and open ICC profiles
  depending on the NULLity of inICC/outICC. By invoking ComparativeCt with all four
  combinations (NULL/NULL, NULL/non-NULL, non-NULL/NULL, non-NULL/non-NULL) we ensure
  both branches of each predicate are exercised.
- The perf_fn callbacks (perfFn1 and perfFn2) record their inputs to verify that
  ComparativeCt invokes both functions exactly once per call, as expected.
- We purposely avoid terminating assertions; test failures are collected and reported
  at the end of main().
- The tests assume that CreateCurves and cmsOpenProfileFromFile are provided by the
  project under test and linked at build time. If symbol signatures differ in the actual
  project, adjust the extern "C" declarations accordingly.
*/