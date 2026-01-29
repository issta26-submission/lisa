// Unit test suite for the focal method: cmsFormatter my_FormatterFactory
// This test file is designed to be compiled with the provided project headers.
// It uses a lightweight, non-terminating assertion framework (no GTest).
// The tests focus on exercising true/false branches of the predicate inside
// cmsFormatter my_FormatterFactory as described in the focal method.
// The test suite assumes the existence of the following from the project headers
// (via testcms2.h or equivalent):
// - cmsUInt32Number, cmsFormatterDirection, TYPE_RGB_565, CMS_PACK_FLAGS_FLOAT
// - cmsFormatter struct with member Fmt16 (a function pointer)
// - cmsFormatterInput enumerator value
// - extern "C" cmsFormatter my_FormatterFactory(cmsUInt32Number, cmsFormatterDirection, cmsUInt32Number)
// - extern "C" cmsFormatter my_FormatterFactory2(...) // present in the file but not directly used
// - Declaration/definition of my_Unroll565 within the same translation unit as the focal function
//
// Note: We test only presence of the branch outcomes (Fmt16 non-null vs null). We do not
// assume knowledge about the exact function pointer target due to 'static' scoping in the
// original file. This ensures true/false coverage for the predicate without relying on
// internal static details.

#include <cstring>
#include <iostream>
#include <testcms2.h>


// Ensure the function under test has C linkage to link with the C implementation.
// We declare it here so the C++ test can call it.
extern "C" cmsFormatter my_FormatterFactory(cmsUInt32Number Type,
                                         cmsFormatterDirection Dir,
                                         cmsUInt32Number dwFlags);

// Lightweight test harness
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ << " - " \
                      << (msg) << " (expected: true, actual: false)" << std::endl; \
            ++g_failures; \
        } \
    } while(0)

#define EXPECT_FALSE(cond, msg) \
    do { \
        if(cond) { \
            std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ << " - " \
                      << (msg) << " (expected: false, actual: true)" << std::endl; \
            ++g_failures; \
        } \
    } while(0)

#define EXPECT_NOT_NULL(ptr, msg) \
    do { \
        if((ptr) == nullptr) { \
            std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ << " - " \
                      << (msg) << " (pointer was NULL)" << std::endl; \
            ++g_failures; \
        } \
    } while(0)

#define EXPECT_EQ_UI32(a, b, msg) \
    do { \
        if((static_cast<cmsUInt32Number>(a)) != (static_cast<cmsUInt32Number>(b))) { \
            std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ << " - " \
                      << (msg) << " (expected: " << static_cast<cmsUInt64>(a) \
                      << ", actual: " << static_cast<cmsUInt64>(b) << ")" << std::endl; \
            ++g_failures; \
        } \
    } while(0)

static void test_true_branch_sets_fmt16()
{
    // Arrange: use the values that should trigger the true branch
    // Type == TYPE_RGB_565, Dir == cmsFormatterInput, and no CMS_PACK_FLAGS_FLOAT
    cmsUInt32Number Type = TYPE_RGB_565;
    cmsFormatterDirection Dir = cmsFormatterInput;
    cmsUInt32Number dwFlags = 0; // no CMS_PACK_FLAGS_FLOAT

    // Act
    cmsFormatter Result = my_FormatterFactory(Type, Dir, dwFlags);

    // Assert: Fmt16 should be non-null (pointer assigned to my_Unroll565 in the original function)
    EXPECT_NOT_NULL(Result.Fmt16, "Expected Fmt16 to be non-null for RGB_565 input without float flag");
}

static void test_false_branch_type_mismatch()
{
    // Arrange: Type != TYPE_RGB_565, with valid Dir and flags
    cmsUInt32Number Type = TYPE_RGB_565 ^ 0x1; // ensure a different value (bitwise trick, not guaranteed but likely different)
    cmsFormatterDirection Dir = cmsFormatterInput;
    cmsUInt32Number dwFlags = 0;

    // Act
    cmsFormatter Result = my_FormatterFactory(Type, Dir, dwFlags);

    // Assert: Fmt16 should be NULL
    EXPECT_TRUE(Result.Fmt16 == nullptr, "Expected Fmt16 to be NULL when Type != TYPE_RGB_565");
}

static void test_false_branch_direction_mismatch()
{
    // Arrange: Type RGB_565 but wrong direction
    cmsUInt32Number Type = TYPE_RGB_565;
    cmsFormatterDirection Dir = cmsFormatterOutput; // not cmsFormatterInput
    cmsUInt32Number dwFlags = 0;

    // Act
    cmsFormatter Result = my_FormatterFactory(Type, Dir, dwFlags);

    // Assert: Fmt16 should be NULL
    EXPECT_TRUE(Result.Fmt16 == nullptr, "Expected Fmt16 to be NULL when Dir != cmsFormatterInput");
}

static void test_false_branch_float_flag_set()
{
    // Arrange: Type RGB_565 and Dir input, but bit flag is set
    cmsUInt32Number Type = TYPE_RGB_565;
    cmsFormatterDirection Dir = cmsFormatterInput;
    cmsUInt32Number dwFlags = CMS_PACK_FLAGS_FLOAT; // flag is set

    // Act
    cmsFormatter Result = my_FormatterFactory(Type, Dir, dwFlags);

    // Assert: Fmt16 should be NULL
    EXPECT_TRUE(Result.Fmt16 == nullptr, "Expected Fmt16 to be NULL when CMS_PACK_FLAGS_FLOAT is set");
}

static int run_all_tests()
{
    test_true_branch_sets_fmt16();
    test_false_branch_type_mismatch();
    test_false_branch_direction_mismatch();
    test_false_branch_float_flag_set();

    if(g_failures == 0) {
        std::cout << "[PASSED] All my_FormatterFactory tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "[PARTIAL] my_FormatterFactory tests completed with "
                  << g_failures << " failure(s)." << std::endl;
        return 1;
    }
}

int main()
{
    // Run the tests and return non-zero on failure to integrate with common CI systems.
    int result = run_all_tests();
    return result;
}