// Test suite for the focal method zpull in gzjoin.c
// This test harness is designed for a C++11 environment without gtest.
// It provides minimal, non-terminating assertions and a simple test runner
// that executes test cases from main().

#include <cstdio>
#include <cstring>
#include <gzjoin.c>
#include <fcntl.h>
#include <stdio.h>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>


// -----------------------------------------------------------------------------
// Minimal mock of zlib types expected by gzjoin.c
// We provide just enough to compile and run the focal function in a controlled test.
// The actual project may define these in "zlib.h". To keep this self-contained
// for the test, we supply lightweight definitions here.
// -----------------------------------------------------------------------------

// Mock of z_streamp / z_stream as used by zpull
struct z_stream {
    unsigned int avail_in;
    unsigned char *next_in;
};
typedef z_stream* z_streamp;

// Forward declaration of bin type used by zpull.
// The real project defines 'bin' in gzjoin.c or in a related header.
// We rely on the minimal fields accessed by zpull:
// - left (unsigned long or similar)
// - name (char*)
// - next (unsigned char*)
// If gzjoin.c defines 'bin' differently, this test may need adjustment.
// For the purpose of this test, we assume the following layout is compatible.
struct bin {
    unsigned long left;
    char *name;
    unsigned char *next;
};

// -----------------------------------------------------------------------------
// Include the focal implementation
// We include gzjoin.c so that zpull and its dependencies are compiled in this
// translation unit. The test relies on the actual implementation being linked
// into the same unit; this allows direct invocation from test code.
// Note: If the repository uses the macro 'local' to mark internal linkage, and
// it expands to nothing in this build, zpull will have external linkage and can
// be called from here.
// -----------------------------------------------------------------------------

// The C source may rely on C linkage; ensure compatibility when included in C++.
extern "C" {
}

// -----------------------------------------------------------------------------
// Lightweight test framework (non-GTest)
// Provides non-terminating assertions and collects a summary at the end.
// -----------------------------------------------------------------------------

static int gFailures = 0;

#define ASSERT_TRUE(cond, msg)                                        \
    do {                                                              \
        if (!(cond)) {                                                \
            std::fprintf(stderr, "Test failure: %s\n", (msg));       \
            ++gFailures;                                             \
        }                                                             \
    } while (0)

#define ASSERT_EQ(a, b, msg)                                           \
    do {                                                               \
        if (!((a) == (b))) {                                         \
            std::fprintf(stderr, "Test failure: %s (expected %lld, got %lld)\n", \
                         (msg), static_cast<long long>(a), static_cast<long long>(b)); \
            ++gFailures;                                              \
        }                                                              \
    } while (0)


// Helper to create a simple, printable pointer value for debugging
static const char* ptr_to_cstr(const void* p) {
    if (p == nullptr) return "nullptr";
    return "<ptr>";
}

// -----------------------------------------------------------------------------
// Test 1: zpull should propagate in->left to strm->avail_in and in->next to strm->next_in
// This exercises the common path where in->left > 0 on entry to zpull.
// It does not attempt to exercise the internal bload/bail path (which require
// internal state of in->left == 0 and internal helpers). The focus here is to
// validate the observable effect on z_streamp fields when left > 0.
// -----------------------------------------------------------------------------

static void test_zpull_sets_input_pointers_basic() {
    // Arrange
    bin in;
    // Case 1: left > 0
    in.left = 5;
    const char* fname = "test.bin";
    in.name = const_cast<char*>(fname);
    unsigned char data[] = { 0x01, 0x02, 0x03, 0x04 };
    in.next = data;

    z_stream s;
    s.avail_in = 0;
    s.next_in = nullptr;
    z_streamp strm = &s;

    // Act
    zpull(strm, &in);

    // Assert: the function should propagate in->left and in->next to the stream
    ASSERT_EQ(strm->avail_in, in.left, "zpull should copy in->left to strm->avail_in when left > 0");
    ASSERT_TRUE(strm->next_in == in.next, "zpull should set strm->next_in to in->next when left > 0");

    // Also ensure in still retains its left value (no side-effect on in->left in this path)
    ASSERT_EQ(in.left, 5, "zpull should not modify in->left when left > 0 (as observed in this path)");
}

// -----------------------------------------------------------------------------
// Test 2: zpull should handle a non-zero next_in pointer and a different left value.
// This checks that diverse inputs yield consistent propagation of data to the stream.
// -----------------------------------------------------------------------------

static void test_zpull_sets_input_pointers_with_different_left() {
    // Arrange
    bin in;
    in.left = 12;
    const char* fname = "another.bin";
    in.name = const_cast<char*>(fname);
    unsigned char data2[] = { 0xAA, 0xBB, 0xCC };
    in.next = data2;

    z_stream s;
    s.avail_in = 0;
    s.next_in = nullptr;
    z_streamp strm = &s;

    // Act
    zpull(strm, &in);

    // Assert
    ASSERT_EQ(strm->avail_in, in.left, "zpull should propagate in->left to strm->avail_in for another case");
    ASSERT_TRUE(strm->next_in == in.next, "zpull should propagate in->next to strm->next_in for another case");
    ASSERT_EQ(in.left, 12, "in->left should remain unchanged after zpull for this path");
    // Note: We do not rely on bload/bail behavior in this test since it depends on
    // internal state when left == 0. The test focuses on the common, observable path.
}

// -----------------------------------------------------------------------------
// Test harness (main)
// Executes the test cases and prints a summary.
// -----------------------------------------------------------------------------

int main() {
    std::printf("Starting zpull unit tests (non-GTest, C++11) ...\n");

    test_zpull_sets_input_pointers_basic();
    test_zpull_sets_input_pointers_with_different_left();

    if (gFailures == 0) {
        std::printf("All tests passed.\n");
        return 0;
    } else {
        std::fprintf(stderr, "Total failures: %d\n", gFailures);
        return 1;
    }
}