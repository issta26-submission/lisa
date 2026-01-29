// Test suite for DupMatShaper in cmsopt.c
// NOTE: This test is designed to be compiled in a project where cmsopt.c and the
// required cms headers are available. It uses a lightweight, self-contained
// test harness (no Google Test) and relies on standard C/C++ facilities only.
//
// Strategy overview (aligned with Step 3/Domain Knowledge):
// - Test that DupMatShaper delegates memory duplication to _cmsDupMem by verifying
//   that the returned pointer is non-null and that the initial bytes match the
//   source data (assuming MatShaper8Data size is at least a few bytes).
// - Test that two distinct input buffers yield two distinct duplicated blocks.
// - Test memory independence: modifying the source after duplication should not
//   affect the duplicated memory (first few bytes).
//
// Limitations/Assumptions:
// - We do not rely on freeing the duplicated memory (the cms memory management is
//   library-controlled). Leaks in unit tests are generally acceptable for the
//   sake of coverage in this constrained scenario.
// - We assume MatShaper8Data is a relatively small structure; thus a 256-byte
//   source buffer is safely larger than the size used by DupMatShaper.

#include <cstring>
#include <cstdio>
#include <cstddef>
#include <cassert>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include CMS public headers to obtain the correct types.
// If your environment uses a different path for the LittleCMS headers, adjust accordingly.

// Declaration of focal method (as it appears in cmsopt.c).
// Ensure C linkage for the function when compiling under C++.
extern "C" void* DupMatShaper(cmsContext ContextID, const void* Data);

// Lightweight test framework helpers (no external libraries).
static int tests_run = 0;
static int tests_failed = 0;

#define ASSERT_TRUE(cond, msg) do {                         \
    tests_run++;                                            \
    if (!(cond)) {                                          \
        ++tests_failed;                                     \
        std::fprintf(stderr, "Test failed: %s (line %d): %s\n", __FILE__, __LINE__, msg); \
    }                                                       \
} while (0)

#define ASSERT_NOT_NULL(ptr, msg) do {                       \
    tests_run++;                                             \
    if ((ptr) == nullptr) {                                \
        ++tests_failed;                                      \
        std::fprintf(stderr, "Test failed: %s (line %d): %s\n", __FILE__, __LINE__, msg); \
    }                                                        \
} while (0)

static void test_dup_basic_memory_content_and_independence() {
    // Description:
    // - Create a source buffer filled with a known pattern.
    // - Call DupMatShaper to duplicate it.
    // - Verify the duplicate is non-null and initially matches the source for the
    //   first several bytes (at least a few bytes, since the exact MatShaper8Data
    //   size is implementation-defined).
    // - Verify the returned pointer is not the same as the source pointer (no aliasing).
    // - Modify the source and verify the duplicate's content does not change
    //   (memory independence).
    const std::size_t SRC_SIZE = 256;
    unsigned char src[SRC_SIZE];
    for (std::size_t i = 0; i < SRC_SIZE; ++i) {
        src[i] = static_cast<unsigned char>(i & 0xFF);
    }

    void* dup = DupMatShaper(reinterpret_cast<cmsContext>(0), (const void*)src);
    ASSERT_NOT_NULL(dup, "DupMatShaper returned NULL for valid input");

    unsigned char* dup_bytes = static_cast<unsigned char*>(dup);
    // Validate a small prefix match to ensure content was duplicated.
    const std::size_t PREFIX_LEN = 32;
    bool prefix_match = true;
    for (std::size_t i = 0; i < PREFIX_LEN; ++i) {
        if (dup_bytes[i] != src[i]) {
            prefix_match = false;
            break;
        }
    }
    ASSERT_TRUE(prefix_match, "DupMatShaper did not copy the initial bytes correctly");

    // Ensure the output is not the same memory as input (no in-place aliasing)
    ASSERT_TRUE(dup_bytes != src, "DupMatShaper returned a pointer equal to input (aliasing)");

    // Modify the source after duplication and ensure the duplicate is unaffected.
    unsigned char old0 = dup_bytes[0];
    src[0] = 0xAB;
    if (dup_bytes[0] == 0xAB) {
        // If duplication happened to be a shallow copy or if memory is aliased,
        // this would indicate a problem in independence.
        ASSERT_TRUE(false, "DupMatShaper's duplicate changed when source modified (memory not independent)");
    } else {
        // At least the first byte didn't reflect the updated source value,
        // implying a proper independent copy.
        ASSERT_TRUE(true, "DupMatShaper duplicate remained independent after source mutation");
    }

    // Clean up: do not forcibly free since the CMS memory management is library-controlled.
    // Relying on process termination to reclaim memory in unit tests is acceptable here.
}

static void test_dup_multiple_inputs_are_distinct() {
    // Description:
    // - Ensure two different input buffers yield two distinct duplicates.
    const std::size_t SIZE_A = 128;
    const std::size_t SIZE_B = 128;

    unsigned char a[SIZE_A];
    unsigned char b[SIZE_B];
    for (std::size_t i = 0; i < SIZE_A; ++i) a[i] = static_cast<unsigned char>((i * 3) & 0xFF);
    for (std::size_t i = 0; i < SIZE_B; ++i) b[i] = static_cast<unsigned char>((i * 7) & 0xFF);

    void* dupA = DupMatShaper(reinterpret_cast<cmsContext>(0x1), (const void*)a);
    void* dupB = DupMatShaper(reinterpret_cast<cmsContext>(0x2), (const void*)b);

    ASSERT_NOT_NULL(dupA, "DupMatShaper returned NULL for first input");
    ASSERT_NOT_NULL(dupB, "DupMatShaper returned NULL for second input");

    // Pointers should be distinct
    ASSERT_TRUE(dupA != dupB, "DupMatShaper produced identical duplicates for different inputs");

    unsigned char* dupA_bytes = static_cast<unsigned char*>(dupA);
    unsigned char* dupB_bytes = static_cast<unsigned char*>(dupB);

    // Compare a few bytes to ensure contents reflect their respective inputs
    bool matchA = true;
    bool matchB = true;
    const std::size_t COMP_LEN = 16;
    for (std::size_t i = 0; i < COMP_LEN; ++i) {
        if (dupA_bytes[i] != a[i]) matchA = false;
        if (dupB_bytes[i] != b[i]) matchB = false;
    }
    ASSERT_TRUE(matchA, "First duplicate content does not match first input for initial bytes");
    ASSERT_TRUE(matchB, "Second duplicate content does not match second input for initial bytes");
}

static int main_wrapper() {
    test_dup_basic_memory_content_and_independence();
    test_dup_multiple_inputs_are_distinct();
    // Report summary
    if (tests_failed == 0) {
        std::printf("All DupMatShaper tests passed (%d tests).\n", tests_run);
    } else {
        std::printf("DupMatShaper tests completed with %d failure(s) out of %d test(s).\n",
                    tests_failed, tests_run);
    }
    return tests_failed;
}

int main() {
    return main_wrapper();
}