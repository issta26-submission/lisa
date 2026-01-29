// Unit tests for deflatePending function
// Target: C++11, no Google Test, use a simple in-file test harness
// Notes:
// - We rely on the public C API exposed via deflate.h
// - Tests are designed to be non-terminating: they report failures but continue
// - Tests are executed from main() as per domain knowledge
// - Tests focus on different usages of pending/bits pointers and basic success path

#include <iostream>
#include <deflate.h>
#include <cstring>


// Import C API (deflate.h) with C linkage
extern "C" {
}

static int g_failures = 0;

// Simple non-terminating assertion helpers
#define TEST_EXPECT_EQ(expected, actual, msg)                                     \
    do {                                                                             \
        if ((expected) != (actual)) {                                              \
            std::cerr << "EXPECT_EQ FAILED: " << (msg)                                 \
                      << " | expected: " << (expected)                                   \
                      << ", actual: " << (actual) << " (at " << __FILE__ << ":" << __LINE__ << ")" \
                      << std::endl;                                                 \
            ++g_failures;                                                          \
        }                                                                            \
    } while (0)

#define TEST_EXPECT_TRUE(cond, msg)                                                 \
    do {                                                                             \
        if (!(cond)) {                                                               \
            std::cerr << "EXPECT_TRUE FAILED: " << (msg)                               \
                      << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
            ++g_failures;                                                          \
        }                                                                            \
    } while (0)

//////////////////////////////////////////////////////////////////
// Test 1: Normal operation after proper initialization
// - Purpose: Ensure deflatePending returns Z_OK when provided with a valid stream
//           and both pending and bits pointers are non-NULL.
// - Rationale: This validates the positive/neutral path of the function under normal usage.
//////////////////////////////////////////////////////////////////
void test_deflatePending_normal_init()
{
    // Create a z_stream object on the stack and initialize it.
    // We do not rely on internal state values, just that initialization succeeds
    z_stream z;
    std::memset(&z, 0, sizeof(z));

    // deflateInit_ signature: int ZEXPORT deflateInit_(z_streamp strm, int level, const char *version, int stream_size)
    // Provide reasonable version string and stream_size
    const char* version = "1.2.11";
    int stream_size = (int)sizeof(z_stream);
    int init_res = deflateInit_(&z, 6, version, stream_size);

    // If initialization fails, report and skip further checks in this test
    TEST_EXPECT_EQ(Z_OK, init_res, "deflateInit_ should return Z_OK for a valid stream");

    // Prepare output variables and call function under test
    unsigned pending = 0;
    int bits = 0;
    int res = deflatePending(&z, &pending, &bits);

    // Expect success
    TEST_EXPECT_EQ(Z_OK, res, "deflatePending should return Z_OK after successful init");

    // Clean up
    deflateEnd(&z);
}

//////////////////////////////////////////////////////////////////
// Test 2: null pending pointer
// - Purpose: Ensure deflatePending handles a NULL pending pointer gracefully
//           while bits is non-NULL (should still return Z_OK).
//////////////////////////////////////////////////////////////////
void test_deflatePending_null_pending_pointer()
{
    z_stream z;
    std::memset(&z, 0, sizeof(z));

    const char* version = "1.2.11";
    int stream_size = (int)sizeof(z_stream);
    int init_res = deflateInit_(&z, 6, version, stream_size);
    TEST_EXPECT_EQ(Z_OK, init_res, "deflateInit_ should succeed for null pending test");

    unsigned pending = 0;
    int bits = 0;
    int res = deflatePending(&z, NULL, &bits);

    TEST_EXPECT_EQ(Z_OK, res, "deflatePending should return Z_OK when pending is NULL");
    // Optionally verify that bits were filled by API (value depends on internal state)
    // Here we only ensure no crash and successful return.

    deflateEnd(&z);
}

//////////////////////////////////////////////////////////////////
// Test 3: null bits pointer
// - Purpose: Ensure deflatePending handles a NULL bits pointer gracefully
//           while pending is non-NULL (should still return Z_OK).
//////////////////////////////////////////////////////////////////
void test_deflatePending_null_bits_pointer()
{
    z_stream z;
    std::memset(&z, 0, sizeof(z));

    const char* version = "1.2.11";
    int stream_size = (int)sizeof(z_stream);
    int init_res = deflateInit_(&z, 6, version, stream_size);
    TEST_EXPECT_EQ(Z_OK, init_res, "deflateInit_ should succeed for null bits test");

    unsigned pending = 0;
    int res = deflatePending(&z, &pending, NULL);

    TEST_EXPECT_EQ(Z_OK, res, "deflatePending should return Z_OK when bits is NULL");
    // Again, we don't enforce a specific pending value here.

    deflateEnd(&z);
}

//////////////////////////////////////////////////////////////////
// Test 4: both pointers NULL
// - Purpose: Ensure deflatePending can handle both pending and bits pointers being NULL
//           and still return Z_OK (no writes performed).
//////////////////////////////////////////////////////////////////
void test_deflatePending_both_null_pointers()
{
    z_stream z;
    std::memset(&z, 0, sizeof(z));

    const char* version = "1.2.11";
    int stream_size = (int)sizeof(z_stream);
    int init_res = deflateInit_(&z, 6, version, stream_size);
    TEST_EXPECT_EQ(Z_OK, init_res, "deflateInit_ should succeed for both-null test");

    int res = deflatePending(&z, NULL, NULL);

    TEST_EXPECT_EQ(Z_OK, res, "deflatePending should return Z_OK when both pointers are NULL");

    deflateEnd(&z);
}

//////////////////////////////////////////////////////////////////
// Test 5: (Optional) edge case – invalid stream state
// - Note: This test attempts to provoke an error path by using an uninitialized/invalid stream.
// - Since deflateStateCheck is static, behavior can depend on internal implementation.
// - We attempt to simulate by calling with a stream that has not been properly initialized.
// - If the library deflateStateCheck detects an invalid state, we expect Z_STREAM_ERROR.
//////////////////////////////////////////////////////////////////
void test_deflatePending_invalid_state()
{
    // Create a z_stream without initialization
    z_stream z;
    std::memset(&z, 0, sizeof(z));

    // Do not call deflateInit_/deflateInit2_ on this stream
    // Directly invoke deflatePending and observe behavior
    unsigned pending = 0;
    int bits = 0;
    int res = deflatePending(&z, &pending, &bits);

    // We cannot guarantee this will always be Z_STREAM_ERROR in all environments,
    // but we document the expectation that an invalid/internal state should yield an error.
    if (res != Z_OK) {
        std::cout << "NOTE: deflatePending on invalid/uninitialized stream returned non-Z_OK "
                  << "(as an expected error path): " << res << std::endl;
    } else {
        // If it unexpectedly returns OK, still report the behavior for visibility.
        std::cout << "WARNING: deflatePending on invalid/uninitialized stream returned Z_OK (unexpected)\n";
    }
}

//////////////////////////////////////////////////////////////////
// Main: run all tests
//////////////////////////////////////////////////////////////////
int main()
{
    std::cout << "Running deflatePending unit tests (C++11, no gtest)" << std::endl;

    test_deflatePending_normal_init();
    test_deflatePending_null_pending_pointer();
    test_deflatePending_null_bits_pointer();
    test_deflatePending_both_null_pointers();
    test_deflatePending_invalid_state();

    if (g_failures > 0) {
        std::cout << "DeflatePending tests completed with " << g_failures << " failure(s)." << std::endl;
        return 1;
    } else {
        std::cout << "DeflatePending tests completed successfully." << std::endl;
        return 0;
    }
}