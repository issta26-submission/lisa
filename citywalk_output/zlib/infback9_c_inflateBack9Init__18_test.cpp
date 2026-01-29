/*
  Unit test suite for inflateBack9Init_ (infback9.c) using a lightweight
  C++11 test harness (no Google Test). The tests cover:
  - Successful initialization path
  - Version-related error path
  - Null/invalid parameter handling
  - Memory allocation failure path (simulated via custom alloc function)

  Notes:
  - This test uses zlib types (z_stream, ZLIB_VERSION, etc.). Ensure zlib
    headers/libraries are available and linked when compiling.
  - Tests use non-terminating EXPECT_* style assertions to maximize coverage.
  - Tests are invoked from main() as requested.
*/

#include <cstring>
#include <inflate9.h>
#include <infback9.h>
#include <stdio.h>
#include <iostream>
#include <inftree9.h>
#include <zlib.h>
#include <zutil.h>
#include <inffix9.h>


// Ensure FAR is defined for compatibility with the focal function signature.
// The test environment may define FAR; define it as empty if not present.
#ifndef FAR
#define FAR
#endif

// Include zlib headers to access z_stream and related constants/types.

// Declare the focal function signature for linking with the tested object code.
// We mirror the expected C signature, using extern "C" for C linkage.
extern "C" int inflateBack9Init_(z_stream FAR *strm, unsigned char FAR *window,
                                const char *version, int stream_size);

// Simple non-terminating test framework (EXPECT_* macros)
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) \
                  << " (in " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_failed; \
    } \
    ++g_total; \
} while (0)

#define EXPECT_FALSE(cond, msg) do { \
    if (cond) { \
        std::cerr << "EXPECT_FALSE failed: " << (msg) \
                  << " (in " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_failed; \
    } \
    ++g_total; \
} while (0)

#define EXPECT_EQ(a, b, msg) do { \
    if (!((a) == (b))) { \
        std::cerr << "EXPECT_EQ failed: " << (msg) \
                  << " - left: " << (a) << " right: " << (b) \
                  << " (in " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_failed; \
    } \
    ++g_total; \
} while (0)

#define EXPECT_NE(a, b, msg) do { \
    if ((a) == (b)) { \
        std::cerr << "EXPECT_NE failed: " << (msg) \
                  << " - both are: " << (a) \
                  << " (in " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_failed; \
    } \
    ++g_total; \
} while (0)

// Forward declaration of a bad allocator to simulate allocation failure
// matching the zlib alloc_func signature.
static voidpf bad_alloc(voidpf opaque, unsigned int items, unsigned int size) {
    (void)opaque;
    (void)items;
    (void)size;
    return Z_NULL; // Simulate allocation failure
}

// Test 1: Successful initialization path
static void test_inflateBack9Init_success() {
    // Arrange
    z_stream strm;
    std::memset(&strm, 0, sizeof(z_stream));

    unsigned char window[256]; // small dummy window buffer
    const char* version = ZLIB_VERSION; // valid version
    int stream_size = (int)sizeof(z_stream);

    // Act
    int ret = inflateBack9Init_(&strm, window, version, stream_size);

    // Assert
    EXPECT_EQ(ret, Z_OK, "inflateBack9Init_ should return Z_OK on success");
    // strm.state should be allocated (non-null)
    EXPECT_TRUE(strm.state != NULL, "inflateBack9Init_ should allocate state and assign to strm.state");
    // Optionally ensure the state's window pointer is preserved (best effort; access via strm state if possible)
    // We avoid relying on internal layout; thus only verify non-null state above.
}

// Test 2: Version-related error path
static void test_inflateBack9Init_versionError() {
    // Arrange: provide an invalid version (null) to trigger Z_VERSION_ERROR
    z_stream strm;
    std::memset(&strm, 0, sizeof(z_stream));

    unsigned char window[256];
    const char* version = Z_NULL; // force version check failure
    int stream_size = (int)sizeof(z_stream);

    // Act
    int ret = inflateBack9Init_(&strm, window, version, stream_size);

    // Assert
    EXPECT_EQ(ret, Z_VERSION_ERROR, "inflateBack9Init_ should return Z_VERSION_ERROR when version is invalid");
}

// Test 3a: Null strm parameter should yield Z_STREAM_ERROR
static void test_inflateBack9Init_nullStrm() {
    // Arrange
    unsigned char window[256];
    const char* version = ZLIB_VERSION;
    int stream_size = (int)sizeof(z_stream);

    // Act
    int ret = inflateBack9Init_(nullptr, window, version, stream_size);

    // Assert
    EXPECT_EQ(ret, Z_STREAM_ERROR, "inflateBack9Init_ should return Z_STREAM_ERROR when strm is null");
}

// Test 3b: Null window parameter should yield Z_STREAM_ERROR
static void test_inflateBack9Init_nullWindow() {
    // Arrange
    z_stream strm;
    std::memset(&strm, 0, sizeof(z_stream));

    const char* version = ZLIB_VERSION;
    int stream_size = (int)sizeof(z_stream);

    // Act
    int ret = inflateBack9Init_(&strm, nullptr, version, stream_size);

    // Assert
    EXPECT_EQ(ret, Z_STREAM_ERROR, "inflateBack9Init_ should return Z_STREAM_ERROR when window is null");
}

// Test 4: Memory allocation failure during state allocation
static void test_inflateBack9Init_memAllocFail() {
    // Arrange: provide a non-null zalloc function that will fail
    z_stream strm;
    std::memset(&strm, 0, sizeof(z_stream));

    unsigned char window[256];
    const char* version = ZLIB_VERSION;
    int stream_size = (int)sizeof(z_stream);

    // Inject failing allocator
    strm.zalloc = (alloc_func)bad_alloc; // non-null to bypass auto-setting to zcalloc
    // zfree remains 0, but allocation should fail before zfree usage

    // Act
    int ret = inflateBack9Init_(&strm, window, version, stream_size);

    // Assert
    EXPECT_EQ(ret, Z_MEM_ERROR, "inflateBack9Init_ should return Z_MEM_ERROR when allocation fails");
}

int main() {
    std::cout << "Starting inflateBack9Init_ unit tests (C++11 harness)\n";

    test_inflateBack9Init_success();
    test_inflateBack9Init_versionError();
    test_inflateBack9Init_nullStrm();
    test_inflateBack9Init_nullWindow();
    test_inflateBack9Init_memAllocFail();

    std::cout << "Tests completed. Total: " << g_total << ", Failures: " << g_failed << "\n";

    // Return non-zero if any test failed
    return g_failed ? 1 : 0;
}