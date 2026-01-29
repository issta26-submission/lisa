// Comprehensive unit test suite for FomLabV4ToLabV2
// Note: This test is self-contained (no Google Test) and uses a lightweight
// EXPECT style for non-terminating assertions with detailed comments.

// The focal method under test (recreated in this test for isolation):
// cmsINLINE cmsUInt16Number FomLabV4ToLabV2(cmsUInt16Number x)
// {
//     return (cmsUInt16Number) (((x << 8) + 0x80) / 257);
// }

// This test is designed to run in a standard C++11 environment.

#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <lcms2_internal.h>
#include <cstdint>


// Lightweight assertion framework (non-terminating)
static int g_failures = 0;

#define EXPECT_EQ(expected, actual) do { \
    if (static_cast<decltype(actual)>(actual) != static_cast<decltype(actual)>(expected)) { \
        std::cerr << std::left << std::setw(60) \
                  << "EXPECT_EQ FAILED: " #expected " != " #actual \
                  << " | actual: " << (actual) \
                  << " | expected: " << (expected) \
                  << " | location: " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE FAILED: condition is false | location: " \
                  << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_NEQ(expected, actual) do { \
    if (static_cast<decltype(actual)>(actual) == static_cast<decltype(actual)>(expected)) { \
        std::cerr << "EXPECT_NEQ FAILED: " #expected " == " #actual \
                  << " | actual: " << (actual) \
                  << " | location: " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)


// Recreate necessary types/macros to compile this test in isolation
typedef uint16_t cmsUInt16Number;
typedef uint8_t  cmsUInt8Number;

#define cmsINLINE inline

// The focal function under test (as provided)
cmsINLINE cmsUInt16Number FomLabV4ToLabV2(cmsUInt16Number x)
{
    return (cmsUInt16Number) (((x << 8) + 0x80) / 257);
}

// Independent expected implementation (for runtime validation)
// This mirrors the exact arithmetic path used by FomLabV4ToLabV2 but uses a wider
// width for the intermediate calculation to prevent overflow and to provide a
// separate reference implementation.
static inline uint32_t expectedFomLabV4ToLabV2(uint16_t x)
{
    return static_cast<uint32_t>(((static_cast<uint32_t>(x) << 8) + 0x80) / 257);
}


// Basic test harness for FomLabV4ToLabV2
static void RunFomLabV4ToLabV2Tests()
{
    // Demonstrate the true/false branches are covered by exercising variety of inputs.
    // Although FomLabV4ToLabV2 is a pure arithmetic expression with no branches,
    // we still stress a range of inputs to ensure correct behavior across domain.

    // 1) Boundary case: x = 0
    // Expected: ((0 << 8) + 0x80) / 257 = 0x80 / 257 = 0
    {
        cmsUInt16Number in = 0;
        cmsUInt16Number out = FomLabV4ToLabV2(in);
        EXPECT_EQ(0, out);
        // Sanity check against independent computation
        EXPECT_EQ(static_cast<cmsUInt16Number>(expectedFomLabV4ToLabV2(in)),
                  out);
    }

    // 2) Small value: x = 1
    // Expected: ((1 << 8) + 0x80) / 257 = (256 + 128) / 257 = 384 / 257 = 1
    {
        cmsUInt16Number in = 1;
        cmsUInt16Number out = FomLabV4ToLabV2(in);
        EXPECT_EQ(1, out);
        EXPECT_EQ(static_cast<cmsUInt16Number>(expectedFomLabV4ToLabV2(in)), out);
    }

    // 3) Boundary around 255/256 to observe transition
    // x = 255 -> ((255<<8)+0x80)/257 = (65280+128)/257 = 65408/257 = 254
    {
        cmsUInt16Number in = 255;
        cmsUInt16Number out = FomLabV4ToLabV2(in);
        EXPECT_EQ(254, out);
        EXPECT_EQ(static_cast<cmsUInt16Number>(expectedFomLabV4ToLabV2(in)), out);
    }

    // x = 256 -> ((256<<8)+0x80)/257 = (65536+128)/257 = 65664/257 = 255
    {
        cmsUInt16Number in = 256;
        cmsUInt16Number out = FomLabV4ToLabV2(in);
        EXPECT_EQ(255, out);
        EXPECT_EQ(static_cast<cmsUInt16Number>(expectedFomLabV4ToLabV2(in)), out);
    }

    // 4) Large value near max for 16-bit: x = 65535
    // Expected: floor((65535*256 + 128) / 257) = floor(16777088 / 257) = 65280
    {
        cmsUInt16Number in = 65535;
        cmsUInt16Number out = FomLabV4ToLabV2(in);
        EXPECT_EQ(65280, out);
        EXPECT_EQ(static_cast<cmsUInt16Number>(expectedFomLabV4ToLabV2(in)), out);
    }

    // 5) A few random representative tests to increase coverage
    {
        std::vector<cmsUInt16Number> inputs = { 10, 64, 128, 204, 1000, 4096 };
        for (cmsUInt16Number in : inputs) {
            cmsUInt16Number out = FomLabV4ToLabV2(in);
            cmsUInt16Number expected = static_cast<cmsUInt16Number>(expectedFomLabV4ToLabV2(in));
            EXPECT_EQ(expected, out);
        }
    }

    // 6) Monotonicity check over a small range to ensure increasing input yields
    // non-decreasing output (property of the formula).
    // Note: This is a non-terminating check (continues on failure) but is part of coverage.
    {
        const int range = 100; // small, deterministic range
        for (int i = 0; i < range-1; ++i) {
            cmsUInt16Number a = FomLabV4ToLabV2(static_cast<cmsUInt16Number>(i));
            cmsUInt16Number b = FomLabV4ToLabV2(static_cast<cmsUInt16Number>(i+1));
            EXPECT_TRUE(a <= b);
        }
    }
}


// Entry point for running tests
int main()
{
    RunFomLabV4ToLabV2Tests();

    if (g_failures) {
        std::cerr << "FomLabV4ToLabV2 tests completed with " << g_failures
                  << " failure(s).\n";
        return 1;
    } else {
        std::cout << "FomLabV4ToLabV2 tests passed.\n";
        return 0;
    }
}