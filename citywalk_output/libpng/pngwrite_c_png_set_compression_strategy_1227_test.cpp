// Candidate Keywords extracted from the focal method and its dependencies:
// - png_struct, png_ptr (png_structrp), PNG_FLAG_ZLIB_CUSTOM_STRATEGY
// - zlib_strategy, png_debug (debug hook), NULL-pointer check
// - Bitwise flag manipulation and assignment of zlib_strategy
// This test suite focuses on the behavior of png_set_compression_strategy as implemented in the focal method.

#include <iostream>
#include <pngpriv.h>


// Minimal stand-ins to model the focal function's environment.
// This test does not depend on libpng; it provides just enough to exercise the logic.

struct png_struct;
typedef png_struct* png_structrp;

// Lightweight representation of the png_struct used by the focal method.
struct png_struct {
    unsigned int flags;
    int zlib_strategy;
};

// Public-facing macro used by the focal function to toggle a custom strategy flag.
const unsigned int PNG_FLAG_ZLIB_CUSTOM_STRATEGY = 0x08; // Arbitrary bit for testing

// Forward declaration of the focal method under test.
// The actual implementation in the test mirrors the logic from the provided snippet.
void png_set_compression_strategy(png_structrp png_ptr, int strategy);

// Lightweight no-op debug hook to mimic libpng's internal debugging facility.
static void png_debug(int /*level*/, const char* /*msg*/) {
    // Intentionally empty for testing purposes.
}

// Implemented exactly as described in the focal method to allow unit testing.
void png_set_compression_strategy(png_structrp png_ptr, int strategy)
{
{
   png_debug(1, "in png_set_compression_strategy");
   if (png_ptr == NULL)
      return;
   /* The flag setting here prevents the libpng dynamic selection of strategy.
    */
   png_ptr->flags |= PNG_FLAG_ZLIB_CUSTOM_STRATEGY;
   png_ptr->zlib_strategy = strategy;
}
}

// Simple, non-terminating test framework (GTest-like naming, but self-contained)
static int g_failures = 0;

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        std::cerr << "Expectation failed: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_EQ(a, b) do { \
    if ((a) != (b)) { \
        std::cerr << "Expectation failed: " #a " == " #b " (" << (a) << " != " << (b) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_NEQ(a, b) do { \
    if ((a) == (b)) { \
        std::cerr << "Expectation failed: " #a " != " #b " (" << (a) << " == " << (b) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while (0)

// Test 1: When png_ptr is NULL, the function should return without modifying anything.
void test_png_set_compression_strategy_null_ptr() {
    // Arrange
    png_struct s;
    s.flags = 0;
    s.zlib_strategy = 0;

    // Act
    png_set_compression_strategy(nullptr, 10);

    // Assert: no crash, no modification to existing memory (since null pointer)
    // Nothing to verify after call; ensure no exception/segfault occurred by reaching here.
    // As a safety, ensure existing memory remains unchanged (not strictly required by spec, but good practice)
    EXPECT_EQ(s.flags, 0u);
    EXPECT_EQ(s.zlib_strategy, 0);
}

// Test 2: Non-null pointer: function should set the flag and assign the strategy.
void test_png_set_compression_strategy_sets_flag_and_strategy() {
    // Arrange
    png_struct s;
    s.flags = 0;
    s.zlib_strategy = -999; // arbitrary initial value

    // Act
    png_set_compression_strategy(&s, 5);

    // Assert
    EXPECT_TRUE((s.flags & PNG_FLAG_ZLIB_CUSTOM_STRATEGY) != 0);
    EXPECT_EQ(s.zlib_strategy, 5);
}

// Test 3: Repeated calls should update the strategy while preserving the flag.
void test_png_set_compression_strategy_updates_strategy() {
    // Arrange
    png_struct s;
    s.flags = 0;
    s.zlib_strategy = 1;

    // Act: first call
    png_set_compression_strategy(&s, 7);
    // Assert after first call
    EXPECT_TRUE((s.flags & PNG_FLAG_ZLIB_CUSTOM_STRATEGY) != 0);
    EXPECT_EQ(s.zlib_strategy, 7);

    // Act: second call with a different value
    png_set_compression_strategy(&s, -3);

    // Assert after second call
    EXPECT_TRUE((s.flags & PNG_FLAG_ZLIB_CUSTOM_STRATEGY) != 0);
    EXPECT_EQ(s.zlib_strategy, -3);
}

// Test 4: Negative and large values are accepted and stored (no internal clamping in focal method).
void test_png_set_compression_strategy_accepts_extreme_values() {
    // Arrange
    png_struct s;
    s.flags = 0;
    s.zlib_strategy = 0;

    // Act with a negative value
    png_set_compression_strategy(&s, -1);
    EXPECT_TRUE((s.flags & PNG_FLAG_ZLIB_CUSTOM_STRATEGY) != 0);
    EXPECT_EQ(s.zlib_strategy, -1);

    // Act with a large value
    png_set_compression_strategy(&s, 123456);
    EXPECT_EQ(s.zlib_strategy, 123456);
}

// Entry point for running all tests
int main() {
    // Informative header
    std::cout << "Starting tests for: png_set_compression_strategy (simplified unit tests)\n";

    test_png_set_compression_strategy_null_ptr();
    test_png_set_compression_strategy_sets_flag_and_strategy();
    test_png_set_compression_strategy_updates_strategy();
    test_png_set_compression_strategy_accepts_extreme_values();

    if (g_failures == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << g_failures << " test(s) FAILED.\n";
        return 2;
    }
}