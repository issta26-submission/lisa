// Minimal C++11 test harness for the focal function: check_location
// This test suite is designed to be linked with the pngset.c source that
// contains the implementation of check_location(...) as shown in the prompt.
// It provides lightweight test scaffolding (no GoogleTest/GMock) and exercises
// various branches of the function, including error/warning behavior.

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <stdexcept>


// Domain-specific definitions (mocked for testing environment)
#define PNG_IS_READ_STRUCT 0x01
#define PNG_HAVE_IHDR     0x01
#define PNG_HAVE_PLTE     0x02
#define PNG_AFTER_IDAT    0x04

typedef unsigned char png_byte;

// Forward declaration of the internal png_struct_def used by libpng.
// The real pngset.c expects a pointer to a const struct png_struct_def via
// the typedef png_const_structrp. We model the essential field used by check_location.
struct png_struct_def;
typedef const struct png_struct_def* png_const_structrp;

// Declaration of the focal function under test (link with pngset.c)
extern "C" png_byte check_location(png_const_structrp png_ptr, int location);

// Mocked support functions expected by check_location (provided by this test file)
extern "C" void png_app_warning(png_const_structrp png_ptr, const char* msg);
extern "C" void png_error(png_const_structrp png_ptr, const char* msg);

// Concrete definition of the png_struct_def used by tests.
// We only expose the 'mode' field since check_location reads it.
struct png_struct_def {
    unsigned int mode;
};

// Global test harness state
static int g_warning_count = 0;
static int g_expected_failures = 0;
static std::vector<std::string> g_failure_messages;

// Helper to record a test failure with message
static void log_failure(const std::string& msg) {
    g_failure_messages.push_back(msg);
    ++g_expected_failures;
}

// Simple assertion-style helper that also records failures without terminating the test run
#define EXPECT_EQ(actual, expected, test_desc) do {                          \
    if ((actual) != (expected)) {                                              \
        char buf[256];                                                         \
        std::snprintf(buf, sizeof(buf), "EXPECT_EQ failed: %s | actual=%u, expected=%u", \
                      (test_desc), static_cast<unsigned>(actual), static_cast<unsigned>(expected)); \
        log_failure(buf);                                                      \
    }                                                                          \
} while(0)

#define EXPECT_TRUE(cond, test_desc) do {                                       \
    if (!(cond)) {                                                               \
        char buf[256];                                                         \
        std::snprintf(buf, sizeof(buf), "EXPECT_TRUE failed: %s", (test_desc)); \
        log_failure(buf);                                                      \
    }                                                                          \
} while(0)

// Implementations to satisfy linker for png_app_warning and png_error
// so that check_location can be exercised without bringing in the real libpng error handling.
extern "C" void png_app_warning(png_const_structrp /*png_ptr*/, const char* /*msg*/) {
    // Track that a warning was issued (we only care that a warning occurred)
    ++g_warning_count;
}

extern "C" void png_error(png_const_structrp /*png_ptr*/, const char* /*msg*/) {
    // Simulate libpng fatal error by throwing an exception that tests can catch
    throw std::runtime_error("png_error: invalid location flag encountered");
}

// A small function to run all tests and report results
static void run_all_tests()
{
    g_warning_count = 0;
    g_failure_messages.clear();

    // Test 1: Non-zero location with multiple bits should mask and return MSB (no warning)
    {
        struct png_struct_def s;
        s.mode = 0; // PNG_IS_READ_STRUCT not set
        unsigned char res = check_location((png_const_structrp)&s, 0x03); // IHDR|PLTE -> MSB 0x02
        EXPECT_EQ(res, 0x02, "Test1: MSB extraction from 0x03 yields 0x02");
        EXPECT_TRUE(g_warning_count == 0, "Test1: No warning issued for non-zero input");
    }

    // Test 2: Location with higher bits (>7) should be masked to 0x07 and MSB returned
    {
        struct png_struct_def s;
        s.mode = 0;
        unsigned char res = check_location((png_const_structrp)&s, 0x0F); // masked to 0x07 -> MSB 0x04
        EXPECT_EQ(res, 0x04, "Test2: MSB extraction from masked 0x0F -> 0x04");
        EXPECT_TRUE(g_warning_count == 0, "Test2: No warning issued when location is non-zero after mask");
    }

    // Test 3: location == 0, not in READ_STRUCT mode -> warning and use mode bits
    // mode has IHDR (0x01); expect result 0x01 and one warning
    {
        g_warning_count = 0;
        struct png_struct_def s;
        s.mode = 0x01; // not READ_STRUCT, but location will be 0 -> trigger warning
        unsigned char res = check_location((png_const_structrp)&s, 0x00);
        EXPECT_EQ(res, 0x01, "Test3: location 0 with mode IHDR yields 0x01 after warning");
        EXPECT_TRUE(g_warning_count == 1, "Test3: Warning should be issued exactly once");
    }

    // Test 4: location == 0, READ_STRUCT mode set -> should trigger png_error (exception)
    {
        struct png_struct_def s;
        s.mode = PNG_IS_READ_STRUCT; // 0x01
        bool caught = false;
        try {
            unsigned char res = check_location((png_const_structrp)&s, 0x00);
            // If no exception, then test fails
            (void)res;
        } catch (const std::exception& /*e*/) {
            caught = true;
        }
        EXPECT_TRUE(caught, "Test4: location==0 with READ_STRUCT should trigger png_error exception");
    }

    // Test 5: location with two bits including multiple bits after mask -> MSB is returned
    // Ensure that for 0x06 (IHDR+PLTE), the result is 0x04 (MSB)
    {
        struct png_struct_def s;
        s.mode = 0; // doesn't affect since location != 0
        unsigned char res = check_location((png_const_structrp)&s, 0x06);
        EXPECT_EQ(res, 0x04, "Test5: 0x06 masked to 0x04 (MSB) -> 0x04");
    }

    // Test 6: location with only single bit (0x02) -> returns 0x02
    {
        struct png_struct_def s;
        s.mode = 0;
        unsigned char res = check_location((png_const_structrp)&s, 0x02);
        EXPECT_EQ(res, 0x02, "Test6: 0x02 returns itself as MSB");
    }

    // Summary output
    std::cout << "Tests run: " << 6 << ", Failures: " << g_failure_messages.size() << std::endl;
    for (size_t i = 0; i < g_failure_messages.size(); ++i) {
        std::cerr << "Failure " << (i + 1) << ": " << g_failure_messages[i] << std::endl;
    }

    // If any failures occurred, exit with non-zero status to signal failure to harness
    if (!g_failure_messages.empty()) {
        std::exit(1);
    }
}

// Entrypoint
int main(int /*argc*/, char** /*argv*/)
{
    run_all_tests();
    std::cout << "All tests passed." << std::endl;
    return 0;
}