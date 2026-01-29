// Test suite for the focal method: from16to16 in cmsalpha.c
// Step 2/3 context (as per instructions): generate tests that verify endianness swap behavior for 16-bit data.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <iomanip>


// Step 1: Provide C linkage for the focal function under test.
// The function signature in the focal C file is:
// void from16to16(void* dst, const void* src)
extern "C" void from16to16(void* dst, const void* src);

// Global counter to accumulate test failures without terminating on first error.
static int g_failures = 0;

// Lightweight assertion helper: prints detailed message on failure but does not abort tests.
#define ASSERT_EQ_UI16(expected, actual, message) do { \
    if (static_cast<uint16_t>(actual) != static_cast<uint16_t>(expected)) { \
        std::cerr << "[FAIL] " << (message) << " | Expected 0x" << std::hex \
                  << std::setw(4) << std::setfill('0') << (uint16_t)(expected) \
                  << ", Got 0x" << std::setw(4) << std::setfill('0') \
                  << (uint16_t)(actual) << std::dec << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define ASSERT_TRUE(cond, message) do { \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << (message) << std::endl; \
        ++g_failures; \
    } \
} while(0)

// Step 3: Test cases (covering true/false branches of endianness swap semantics and pointer aliasing)
namespace tests {

    // Test 1: Basic swap of a standard 16-bit value 0x1234 -> 0x3412
    void test_swap_basic() {
        uint16_t src = 0x1234;
        uint16_t dst = 0;
        from16to16(&dst, &src);
        uint16_t expected = 0x3412;
        ASSERT_EQ_UI16(expected, dst, "from16to16 should swap bytes for 0x1234 to 0x3412");
    }

    // Test 2: Swap another value 0xABCD -> 0xCDAB
    void test_swap_another_value() {
        uint16_t src = 0xABCD;
        uint16_t dst = 0;
        from16to16(&dst, &src);
        uint16_t expected = 0xCDAB;
        ASSERT_EQ_UI16(expected, dst, "from16to16 should swap bytes for 0xABCD to 0xCDAB");
    }

    // Test 3: Identity case for 0x0000 -> 0x0000
    void test_swap_zero() {
        uint16_t src = 0x0000;
        uint16_t dst = 0;
        from16to16(&dst, &src);
        uint16_t expected = 0x0000;
        ASSERT_EQ_UI16(expected, dst, "from16to16 should preserve 0x0000 as 0x0000 after swap");
    }

    // Test 4: Identity case for 0xFFFF -> 0xFFFF (swap bytes of 0xFFFF remains 0xFFFF)
    void test_swap_ones() {
        uint16_t src = 0xFFFF;
        uint16_t dst = 0;
        from16to16(&dst, &src);
        uint16_t expected = 0xFFFF;
        ASSERT_EQ_UI16(expected, dst, "from16to16 should preserve 0xFFFF as 0xFFFF after swap");
    }

    // Test 5: Pointer aliasing (dst and src point to the same memory)
    // Validate that reading happens before writing.
    void test_alias_src_dst_same() {
        uint16_t value = 0x1234;
        // Use the same memory location for dst and src
        from16to16(&value, &value);
        uint16_t expected = 0x3412;
        ASSERT_EQ_UI16(expected, value, "from16to16 should swap bytes when src and dst aliases refer to the same memory");
    }

    // Test 6: Bulk usage by performing two successive swaps on separate elements
    // Ensure that multiple calls operate independently as expected.
    void test_multiple_elements() {
        uint16_t src[2] = {0x1122, 0x3344};
        uint16_t dst[2] = {0x0000, 0x0000};

        from16to16(&dst[0], &src[0]); // swap first element
        from16to16(&dst[1], &src[1]); // swap second element

        ASSERT_EQ_UI16(0x2211, dst[0], "from16to16 should swap first element correctly in an array");
        ASSERT_EQ_UI16(0x4433, dst[1], "from16to16 should swap second element correctly in an array");
    }

    void run_all() {
        test_swap_basic();
        test_swap_another_value();
        test_swap_zero();
        test_swap_ones();
        test_alias_src_dst_same();
        test_multiple_elements();
    }
}

// Entry point
int main() {
    std::cout << "Running tests for from16to16 (endianness swap) in cmsalpha.c\n";

    tests::run_all();

    if (g_failures == 0) {
        std::cout << "[PASS] All from16to16 tests passed.\n";
        return 0;
    } else {
        std::cout << "[WARN] " << g_failures << " test(s) failed for from16to16.\n";
        // Do not terminate with non-zero elsewhere since we want non-terminating assertions behavior
        // but still indicate failure status to the caller.
        return 1;
    }
}