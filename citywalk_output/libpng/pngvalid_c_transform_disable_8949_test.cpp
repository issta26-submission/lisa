/*
  Unit test suite for the focal method: transform_disable(const char *name)
  in pngvalid.c.

  Summary of approach:
  - Recreate a minimal linked-list of image_transform nodes that pngvalid.c uses:
    image_transform_first, image_transform_end, and a few image_transform nodes with
    fields: name, enable, and list (pointer to next node).
  - Test 3 scenarios:
    1) Disable the second transform (the one after the first) by name.
    2) Disable the first transform by name.
    3) Attempt to disable a non-existent transform and intercept exit(99) via a custom exit()
       to avoid terminating the test process.

  Notes for maintainers:
  - We rely on the actual pngvalid.c implementation to provide the real
    image_transform_first and image_transform_end symbols. The test overrides
    these symbols in-place by assigning the test list to image_transform_first
    and by using a local sentinel object as image_transform_end.
  - We override the C exit(int) function to catch calls from transform_disable when
    an unknown transform is requested. This is done by providing a non-standard exit()
    in this translation unit which the linker will prefer for the symbol.
  - This test suite does not depend on GTest or any other framework; it uses a tiny
    custom assertion harness and a main() that runs all tests in sequence.
  - The test suite uses only C/C++ standard facilities to be compatible with
    C++11 compilation as requested.

  Candidate Keywords (Step 1): image_transform, image_transform_first, image_transform_end,
  transform_disable, linked list, strcmp, unknown transform, exit(99), sentinel, enable flag.
*/

/* Include C linkage for the focal C components */
extern "C" {
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <math.h>

}

/* Forward declare the C symbols used from pngvalid.c */
extern "C" {
typedef struct image_transform {
    const char *name;
    int enable;
    struct image_transform *list;
} image_transform;

/* Globals defined in pngvalid.c (actual project) that form the list head and end sentinel */
extern image_transform *image_transform_first;
extern image_transform image_transform_end;

/* The function under test (focal method) */
void transform_disable(const char *name);
}

/* Test harness in C++11, with non-terminating assertions and a small runner */


// We will override the C exit(int) function to intercept calls from transform_disable
extern "C" {

// Global to capture the exit status for the test
static int g_exit_status = -1;

/* jmp buffer to return control to test after "exit" is invoked by the code under test */
static jmp_buf g_jmp_buf;

/* Custom exit override to catch status and unwind control flow instead of terminating */
void exit(int status)
{
    g_exit_status = status;
    // Return control to the test harness
    longjmp(g_jmp_buf, 1);
}
}

// Helper: minimal assertion macro (non-terminating)
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "ASSERT_TRUE failed: " << (msg) << " (" << #cond << ")\n"; \
        ++g_fail_count; \
    } else { \
        ++g_pass_count; \
    } \
} while(0)

static int g_pass_count = 0;
static int g_fail_count = 0;

/* Test 1: Disable the second transform by name
   Setup a two-element transform list: t1 -> t2 -> end sentinel
   t2 should be disabled when transform_disable("rotate") is called. */
void test_disable_second_transform_by_name()
{
    // Local test transforms
    static image_transform t1;
    static image_transform t2;
    static image_transform endSentinel;

    // Link the list: first -> second -> end sentinel
    t1.name = "scale";
    t1.enable = 1;
    t1.list = &t2;

    t2.name = "rotate";
    t2.enable = 1;
    t2.list = &endSentinel;

    endSentinel.name = NULL;
    endSentinel.enable = 0;
    endSentinel.list = &endSentinel; // self-end

    // Point the global head to our test list
    image_transform_first = &t1;
    image_transform_end = endSentinel;

    // Precondition assertions
    ASSERT_TRUE(t1.enable == 1, "t1 should start enabled");
    ASSERT_TRUE(t2.enable == 1, "t2 should start enabled");

    // Action: disable the second transform by name
    transform_disable("rotate");

    // Postconditions: t2 should be disabled, t1 unchanged
    ASSERT_TRUE(t2.enable == 0, "t2 should be disabled after transform_disable");
    ASSERT_TRUE(t1.enable == 1, "t1 should remain enabled after disabling t2");
}

/* Test 2: Disable the first transform by name
   Rebuild the list similar to Test 1 and disable the first element. */
void test_disable_first_transform_by_name()
{
    // Local test transforms
    static image_transform t1;
    static image_transform t2;
    static image_transform endSentinel;

    // Link the list: first -> second -> end sentinel
    t1.name = "scale";
    t1.enable = 1;
    t1.list = &t2;

    t2.name = "rotate";
    t2.enable = 1;
    t2.list = &endSentinel;

    endSentinel.name = NULL;
    endSentinel.enable = 0;
    endSentinel.list = &endSentinel;

    image_transform_first = &t1;
    image_transform_end = endSentinel;

    // Precondition assertions
    ASSERT_TRUE(t1.enable == 1, "t1 should start enabled (Test 2)");
    ASSERT_TRUE(t2.enable == 1, "t2 should start enabled (Test 2)");

    // Action: disable the first transform by name
    transform_disable("scale");

    // Postconditions: t1 should be disabled, t2 unchanged
    ASSERT_TRUE(t1.enable == 0, "t1 should be disabled after transform_disable");
    ASSERT_TRUE(t2.enable == 1, "t2 should remain enabled after disabling t1");
}

/* Test 3: Unknown transform should cause exit(99)
   We intercept exit by providing a custom exit override and catching via setjmp/longjmp.
   The function should call exit(99). We verify that by checking g_exit_status. */
void test_unknown_transform_exits()
{
    // Install a fresh sentinel list
    static image_transform t1;
    static image_transform t2;
    static image_transform endSentinel;

    t1.name = "scale";
    t1.enable = 1;
    t1.list = &t2;

    t2.name = "rotate";
    t2.enable = 1;
    t2.list = &endSentinel;

    endSentinel.name = NULL;
    endSentinel.enable = 0;
    endSentinel.list = &endSentinel;

    image_transform_first = &t1;
    image_transform_end = endSentinel;

    // Reset exit capture
    g_exit_status = -1;

    // Use setjmp to catch the exit() call
    if (setjmp(g_jmp_buf) == 0) {
        // No longjmp yet; call the function that may call exit
        transform_disable("notexist");
        // If transform_disable returns normally, this is unexpected for unknown transform
        ASSERT_TRUE(false, "transform_disable unexpectedly returned for unknown transform");
    } else {
        // We landed here via exit(99) override
        ASSERT_TRUE(g_exit_status == 99, "transform_disable should exit with status 99 for unknown transform");
    }
}

/* Simple test runner that prints a summary and returns non-zero on failure */
int main(void)
{
    // Reset counters
    g_pass_count = 0;
    g_fail_count = 0;

    // Run tests
    test_disable_second_transform_by_name();
    test_disable_first_transform_by_name();
    test_unknown_transform_exits();

    // Summary
    std::cout << "Test results: " << g_pass_count << " passed, "
              << g_fail_count << " failed." << std::endl;

    // Return 0 if all tests passed, else non-zero
    return (g_fail_count == 0) ? 0 : 1;
}