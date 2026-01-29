// Test suite for the focal method fixedtables in infback.c
// This test is designed to compile under C++11 without Google Test.
// It relies on the project headers (inflate.h, inftrees.h, zutil.h, inffixed.h, etc.)
// The fixedtables function is assumed to be accessible (see note in comments).
// Two compilation variants are intended:
// - Build with -DBUILDFIXED to exercise the BUILDFIXED path (true branch).
// - Build without -DBUILDFIXED to exercise the non-BUILDFIXED path (false branch).
// Each variant contains its own test function guarded by #ifdef/#else.

#include <cstring>
#include <cstdio>
#include <cstddef>
#include <iostream>
#include <inftrees.h>
#include <inffast.h>
#include <zutil.h>
#include <inflate.h>
#include <cstdlib>


// Step 1: Import all necessary dependencies with correct paths.
// We rely on the project's C headers to declare the types used by fixedtables.
// Wrap C headers to ensure C linkage when included in C++.
extern "C" {
#ifdef __has_include
#  if __has_include("inffixed.h")
#    include "inffixed.h"
#  endif
#else
#  include "inffixed.h"
#endif
}

// Forward declaration for the focal function.
// The exact signature includes the FAR macro; we preserve it as in the original code.
extern "C" void fixedtables(struct inflate_state FAR *state);

// Lightweight test framework (non-terminating assertions).
static int g_failures = 0;

#define LOG_TEST(msg) \
    do { std::cerr << "[TEST] " << msg << std::endl; } while (0)

#define EXPECT_TRUE(cond) \
    do { if(!(cond)) { \
        ++g_failures; \
        LOG_TEST("Expectation failed: " #cond); \
    } } while (0)

#define EXPECT_EQ_UI(a,b,desc) \
    do { if(((unsigned int)(a)) != ((unsigned int)(b))) { \
        ++g_failures; \
        char _buf[256]; \
        std::snprintf(_buf, sizeof(_buf), "Expectation failed: %s (expected %u, got %u)", (desc), (unsigned int)(b), (unsigned int)(a)); \
        LOG_TEST(_buf); \
    } } while (0)

#define EXPECT_PTR_NOT_NULL(p, desc) \
    do { if((p) == nullptr) { \
        ++g_failures; \
        char _buf[256]; std::snprintf(_buf, sizeof(_buf), "Pointer check failed: %s", (desc)); LOG_TEST(_buf); \
    } } while (0)

#ifdef BUILDFIXED

// Test for the BUILDFIXED branch (the true path where fixed tables are built).
// This exercises the lens array initialization and the resulting lencode/distcode pointers.
// It also verifies that a second invocation does not overwrite the lens values (virgin guard).
static void test_fixedtables_true_branch() {
    LOG_TEST("Starting test_fixedtables_true_branch (BUILDFIXED defined).");

    // Prepare a representative inflate_state object as declared by the project.
    struct inflate_state state;
    std::memset(&state, 0, sizeof(state));

    // Provide a work area (size chosen arbitrarily; real code uses 'state->work').
    // We assume 'state.work' is a void* (as per typical zlib structures); adjust if needed.
    unsigned char work_area[1024];
    // Some implementations use 'state->work' (void*). If the member name differs (e.g., 'work'), adapt accordingly.
    // We attempt to set both possible variants safely using a union-like approach.
    // Since this is a test harness aligned with project headers, we assign to common member.
    // Note: If the actual struct uses a different member name, this line may need adjustment.
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Waddress-of-packed-member"
    state.work = work_area;
    #pragma GCC diagnostic pop

    // Initialize lens array to a known state to observe changes after fixedtables.
    for (int i = 0; i < 288; ++i) {
        state.lens[i] = 0;
    }

    // First invocation: should take the virgin path and fill in the tables.
    fixedtables(&state);

    // Validate basic postconditions:
    // - lencode and distcode should be non-null (points to built tables).
    // - lenbits == 9 and distbits == 5 per the code.
    EXPECT_PTR_NOT_NULL(state.lencode, "state.lencode should be non-null after fixedtables");
    EXPECT_PTR_NOT_NULL(state.distcode, "state.distcode should be non-null after fixedtables");
    EXPECT_TRUE(state.lenbits == 9);
    EXPECT_TRUE(state.distbits == 5);

    // Validate the expected lens array initialization.
    // 0..143  => 8
    // 144..255 => 9
    // 256..279 => 7
    // 280..287 => 8
    for (int i = 0; i < 144; ++i) {
        EXPECT_EQ_UI(state.lens[i], 8, "lens[0..143] should be 8");
    }
    for (int i = 144; i < 256; ++i) {
        EXPECT_EQ_UI(state.lens[i], 9, "lens[144..255] should be 9");
    }
    for (int i = 256; i < 280; ++i) {
        EXPECT_EQ_UI(state.lens[i], 7, "lens[256..279] should be 7");
    }
    for (int i = 280; i < 288; ++i) {
        EXPECT_EQ_UI(state.lens[i], 8, "lens[280..287] should be 8");
    }

    // Second invocation should not reinitialize lens (virgin is already 0).
    unsigned short lens_snapshot[288];
    for (int i = 0; i < 288; ++i) lens_snapshot[i] = state.lens[i];
    fixedtables(&state);
    for (int i = 0; i < 288; ++i) {
        EXPECT_EQ_UI(state.lens[i], lens_snapshot[i], "lens[] should remain unchanged after second call");
    }

    LOG_TEST("Finished test_fixedtables_true_branch.");
}

#else // not BUILDFIXED

// Test for the non-BUILDFIXED branch (the false path where inffixed.h is used).
// We validate that after fixedtables, essential pointers are non-null and bit-sized fields are set.
static void test_fixedtables_false_branch() {
    LOG_TEST("Starting test_fixedtables_false_branch (BUILDFIXED not defined).");

    struct inflate_state state;
    std::memset(&state, 0, sizeof(state));

    unsigned char work_area[1024];
    state.work = work_area;

    // Call fixedtables; in this path, lenfix/distfix come from inffixed.h
    fixedtables(&state);

    // Basic sanity checks; ensure pointers exist and bit widths are set as expected.
    EXPECT_PTR_NOT_NULL(state.lencode, "state.lencode should be non-null in non-fixed path");
    EXPECT_PTR_NOT_NULL(state.distcode, "state.distcode should be non-null in non-fixed path");
    EXPECT_TRUE(state.lenbits == 9);
    EXPECT_TRUE(state.distbits == 5);

    LOG_TEST("Finished test_fixedtables_false_branch.");
}

#endif // BUILDFIXED

// Main entry: run the appropriate test based on compile-time flag.
// The unit tests can be compiled separately for BUILDFIXED and non-BUILDFIXED scenarios.
int main() {
    // Step 2: Execute the appropriate test(s) based on compile-time configuration.
    #ifdef BUILDFIXED
    test_fixedtables_true_branch();
    #else
    test_fixedtables_false_branch();
    #endif

    // Step 3: Report results.
    if (g_failures != 0) {
        std::cerr << g_failures << " test(s) FAILED." << std::endl;
        return 1;
    } else {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    }
}