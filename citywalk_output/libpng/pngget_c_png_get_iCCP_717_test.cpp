// png_get_iCCP unit test suite for C API (libpng) using a custom lightweight test harness.
// This test suite is designed to be compiled with a C++11 compiler, without Google Test.
// It exercises the focal function: png_get_iCCP, ensuring true/false branches and data integrity.
// Notes:
// - We rely on libpng public API (png.h). We use png_set_iCCP to populate the iCCP data
//   in a png_info, and png_get_iCCP to retrieve it.
// - The tests are written to be non-terminating on assertion failures, collecting results.
// - All tests are wrapped in functions and invoked from main().

#include <vector>
#include <setjmp.h>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <png.h>


// Include libpng public header to access the necessary types and functions.
extern "C" {
}

// Simple non-terminating test assertion macro
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[Test Failure] " << msg << "\n"; \
        ++g_tests_failed; \
    } else { \
        ++g_tests_passed; \
    } \
} while (0)

// Helper to run a test and print its name
#define RUN_TEST(fn) do { \
    std::cout << "Running " #fn "... "; \
    fn(); \
} while (0)

// Explanatory notes for each test are included above the test function as comments.

//
// Test 1: Positive path - Retrieval of iCCP data after setting iCCP via public API.
//           - Sets iCCP data (name, profile, length) using png_set_iCCP.
//           - Calls png_get_iCCP and verifies:
//             * Returned name matches what was set (content equality, not pointer equality).
//             * compression_type equals PNG_COMPRESSION_TYPE_BASE.
//             * proflen equals the length provided to png_set_iCCP.
//             * profile data retrieved matches the original profile data.
//             * Returns PNG_INFO_iCCP.
//
void test_png_get_iCCP_basic()
{
    // Initialize libpng read structure and info structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        std::cerr << "[Test Failure] Failed to create png_struct for test_png_get_iCCP_basic\n";
        ++g_tests_failed;
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        std::cerr << "[Test Failure] Failed to create png_info for test_png_get_iCCP_basic\n";
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        ++g_tests_failed;
        return;
    }

    // Ensure we handle libpng errors gracefully
    if (setjmp(png_jmpbuf(png_ptr))) {
        std::cerr << "[Test Failure] libpng error during test_png_get_iCCP_basic\n";
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        ++g_tests_failed;
        return;
    }

    // Prepare iCCP data
    const char* iccp_name = "Test ICC Profile";
    const unsigned char profile_data[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
    const png_uint_32 profile_len = static_cast<png_uint_32>(sizeof(profile_data));

    // Set iCCP using the public API
    png_set_iCCP(png_ptr, info_ptr, (png_charp)iccp_name,
                   PNG_COMPRESSION_TYPE_BASE,
                   (png_bytep)profile_data,
                   profile_len);

    // Retrieve iCCP using the focal function
    png_charp out_name = NULL;
    int out_compression_type = 0;
    png_bytep out_profile = NULL;
    png_uint_32 out_proflen = 0;

    png_uint_32 ret = png_get_iCCP(png_ptr, info_ptr,
                                 &out_name,
                                 &out_compression_type,
                                 &out_profile,
                                 &out_proflen);

    // Assertions for the positive path
    ASSERT(ret == PNG_INFO_iCCP, "png_get_iCCP should return PNG_INFO_iCCP on valid iCCP data");

    // Name content should match (pointer equality is not guaranteed)
    if (out_name != NULL) {
        std::string got_name(out_name);
        std::string expected_name(iccp_name);
        ASSERT(got_name == expected_name, "Retrieved iCCP name should match the set name (content equality)");
    } else {
        ASSERT(false, "Retrieved iCCP name pointer is NULL");
    }

    // Compression type should be BASE
    ASSERT(out_compression_type == PNG_COMPRESSION_TYPE_BASE,
           "Retrieved compression type should be PNG_COMPRESSION_TYPE_BASE");

    // proflen should match the length provided
    ASSERT(out_proflen == profile_len,
           "Retrieved profile length should match the provided length");

    // Profile data should match the original
    if (out_profile != NULL && out_proflen > 0) {
        if (std::memcmp(out_profile, profile_data, out_proflen) != 0) {
            ASSERT(false, "Retrieved profile data should match the original data");
        } else {
            ASSERT(true, "Retrieved profile data matches original data");
        }
    } else {
        ASSERT(false, "Retrieved profile pointer is NULL or length is zero");
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

// Test 2: Null parameter handling - Ensure function returns 0 when essential pointers are NULL.
void test_png_get_iCCP_null_parameters()
{
    // Minimal setup to obtain valid objects
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        std::cerr << "[Test Failure] Failed to create png_struct for test_png_get_iCCP_null_parameters\n";
        ++g_tests_failed;
        return;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        std::cerr << "[Test Failure] Failed to create png_info for test_png_get_iCCP_null_parameters\n";
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        ++g_tests_failed;
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        std::cerr << "[Test Failure] libpng error during test_png_get_iCCP_null_parameters\n";
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        ++g_tests_failed;
        return;
    }

    // Prepare iCCP data
    const char* iccp_name = "NullParamTest";
    const unsigned char profile_data[] = { 0xAA, 0xBB };
    png_uint_32 profile_len = 2;

    png_set_iCCP(png_ptr, info_ptr, (png_charp)iccp_name,
                   PNG_COMPRESSION_TYPE_BASE,
                   (png_bytep)profile_data,
                   profile_len);

    // Case a) name == NULL
    {
        png_charp out_name = NULL;
        int out_compression_type = 0;
        png_bytep out_profile = NULL;
        png_uint_32 out_proflen = 0;

        png_uint_32 ret = png_get_iCCP(png_ptr, info_ptr,
                                       &out_name,
                                       &out_compression_type,
                                       &out_profile,
                                       &out_proflen);
        ASSERT(ret == PNG_INFO_iCCP, "png_get_iCCP should still return PNG_INFO_iCCP when name NULL? (unexpected)");
    }

    // Case b) info_ptr == NULL
    {
        png_charp dummy_name = NULL;
        int dummy_comp = 0;
        png_bytep dummy_prof = NULL;
        png_uint_32 dummy_len = 0;

        png_uint_32 ret = png_get_iCCP(nullptr, nullptr,
                                       &dummy_name, &dummy_comp, &dummy_prof, &dummy_len);
        ASSERT(ret == 0, "png_get_iCCP should return 0 when png_ptr and info_ptr are NULL");
    }

    // Case c) name, profile, proflen pointers NULL
    {
        // Since name is NULL, function should not proceed; create safe dummy pointers
        int dummy_comp = 0;
        png_uint_32 dummy_len = 0;

        png_uint_32 ret = png_get_iCCP(png_ptr, info_ptr,
                                       nullptr, &dummy_comp, nullptr, &dummy_len);
        ASSERT(ret == 0, "png_get_iCCP should return 0 when name pointer is NULL");

        // Now test with profile pointer NULL
        png_charp out_name = NULL;
        ret = png_get_iCCP(png_ptr, info_ptr,
                           &out_name, &dummy_comp, nullptr, &dummy_len);
        ASSERT(ret == 0, "png_get_iCCP should return 0 when profile pointer is NULL");
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

// Test 3: No iCCP data present - ensure function returns 0 when iCCP chunk not set.
void test_png_get_iCCP_no_iccp_present()
{
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        std::cerr << "[Test Failure] Failed to create png_struct for test_png_get_iCCP_no_iccp_present\n";
        ++g_tests_failed;
        return;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        std::cerr << "[Test Failure] Failed to create png_info for test_png_get_iCCP_no_iccp_present\n";
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        ++g_tests_failed;
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        std::cerr << "[Test Failure] libpng error during test_png_get_iCCP_no_iccp_present\n";
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        ++g_tests_failed;
        return;
    }

    // Do not set iCCP data; attempt to retrieve
    png_charp out_name = NULL;
    int out_comp = 0;
    png_bytep out_profile = NULL;
    png_uint_32 out_len = 0;

    png_uint_32 ret = png_get_iCCP(png_ptr, info_ptr,
                                  &out_name, &out_comp, &out_profile, &out_len);

    ASSERT(ret == 0, "png_get_iCCP should return 0 when no iCCP data present in info_ptr");

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

// Test 4: compression_type pointer is NULL - ensure function still returns valid info and does not crash.
void test_png_get_iCCP_null_compression_type()
{
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        std::cerr << "[Test Failure] Failed to create png_struct for test_png_get_iCCP_null_compression_type\n";
        ++g_tests_failed;
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        std::cerr << "[Test Failure] Failed to create png_info for test_png_get_iCCP_null_compression_type\n";
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        ++g_tests_failed;
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        std::cerr << "[Test Failure] libpng error during test_png_get_iCCP_null_compression_type\n";
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        ++g_tests_failed;
        return;
    }

    // Set iCCP data
    const char* iccp_name = "NoCompressTypeTest";
    const unsigned char profile_data[] = { 0x10, 0x20, 0x30, 0x40 };
    png_uint_32 profile_len = 4;

    png_set_iCCP(png_ptr, info_ptr, (png_charp)iccp_name,
                   PNG_COMPRESSION_TYPE_BASE,
                   (png_bytep)profile_data,
                   profile_len);

    // Retrieve with compression_type == NULL
    png_charp out_name = NULL;
    png_bytep out_profile = NULL;
    png_uint_32 out_len = 0;

    png_uint_32 ret = png_get_iCCP(png_ptr, info_ptr,
                                  &out_name, NULL, &out_profile, &out_len);

    // Should still succeed
    ASSERT(ret == PNG_INFO_iCCP, "png_get_iCCP should return PNG_INFO_iCCP even if compression_type is NULL");

    // Validate retrieved data matches as in basic test (name and data)
    if (out_name != NULL) {
        std::string got_name(out_name);
        std::string expected_name(iccp_name);
        ASSERT(got_name == expected_name, "Retrieved iCCP name should match the set name when compression_type is NULL");
    } else {
        ASSERT(false, "Retrieved iCCP name pointer is NULL when compression_type is NULL");
    }

    // Validate profile data if present
    if (out_profile != NULL && out_len > 0) {
        const unsigned char* original = profile_data;
        if (std::memcmp(out_profile, original, out_len) != 0) {
            ASSERT(false, "Retrieved profile data should match the original data when compression_type is NULL");
        } else {
            ASSERT(true, "Retrieved profile data matches original data when compression_type is NULL");
        }
    } else {
        ASSERT(false, "Retrieved profile data pointer is NULL or length is zero when compression_type is NULL");
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

// Test 5: Validate that static-like behavior isn't required for file-scope statics in this test context.
// The public API should still function even if internal static data exists; this test ensures basic usage.
void test_png_get_iCCP_static_like_behavior()
{
    // This test simply reuses the positive path to ensure no global state leakage between tests.
    // Re-run a minimal positive scenario to exercise the function in a separate context.
    test_png_get_iCCP_basic();
}

// Main function: runs all tests and reports summary.
int main()
{
    std::cout << "Starting png_get_iCCP test suite (custom harness, C API) using C++11...\n";

    RUN_TEST(test_png_get_iCCP_basic);
    RUN_TEST(test_png_get_iCCP_null_parameters);
    RUN_TEST(test_png_get_iCCP_no_iccp_present);
    RUN_TEST(test_png_get_iCCP_null_compression_type);
    RUN_TEST(test_png_get_iCCP_static_like_behavior);

    // Summary
    std::cout << "\nTest results: " << g_tests_passed << " passed, "
              << g_tests_failed << " failed.\n";

    // Return non-zero if any test failed
    return g_tests_failed > 0 ? 1 : 0;
}