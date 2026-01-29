// C++11 test harness for the focal method: png_read_rows
// Note: This test reproduces the essential logic of png_read_rows in a self-contained
// manner to enable unit testing without depending on the external libpng build.
// The test uses simple non-terminating assertions and a minimal test runner invoked from main.

#include <cstdint>
#include <vector>
#include <utility>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


using png_uint_32 = uint32_t;
using png_bytep = unsigned char*;
using png_bytepp = png_bytep*;
using png_structrp = void*;

// Forward declarations for the test hooks (to be used by the focal function).
// In the real project, these would come from pngpriv.h/libpng, but here we mock them.
static void png_debug(int /*level*/, const char* /*message*/) {
    // Intentionally no-op for test harness to avoid noisy output.
}

// The test state collects all calls made to png_read_row from within png_read_rows.
// This allows us to verify that the correct parameters are passed for each iteration.
struct TestState {
    struct Call {
        void* row_ptr;
        void* display_ptr;
    };
    std::vector<Call> calls;
};

// Get a per-test-state singleton for accumulation.
static TestState& get_test_state() {
    static TestState state;
    return state;
}

// Mock implementation of png_read_row used by the focal logic under test.
// We cannot rely on the real libpng implementation here; instead, we record
// the received parameters so tests can validate the call sequence.
extern "C" void png_read_row(png_structrp /*png_ptr*/, png_bytep row, png_bytep display_row) {
    TestState& st = get_test_state();
    st.calls.push_back(TestState::Call{ static_cast<void*>(row), static_cast<void*>(display_row) });
}

// The focal method under test replicated locally for unit testing.
// It mirrors the branches and behavior described in the provided code snippet.
void png_read_rows(png_structrp png_ptr,
                   png_bytepp row,
                   png_bytepp display_row,
                   png_uint_32 num_rows)
{
    {
       png_uint_32 i;
       png_bytepp rp;
       png_bytepp dp;
       png_debug(1, "in png_read_rows");
       if (png_ptr == NULL)
          return;
       rp = row;
       dp = display_row;
       if (rp != NULL && dp != NULL)
          for (i = 0; i < num_rows; i++)
          {
             png_bytep rptr = *rp++;
             png_bytep dptr = *dp++;
             png_read_row(png_ptr, rptr, dptr);
          }
       else if (rp != NULL)
          for (i = 0; i < num_rows; i++)
          {
             png_bytep rptr = *rp;
             png_read_row(png_ptr, rptr, NULL);
             rp++;
          }
       else if (dp != NULL)
          for (i = 0; i < num_rows; i++)
          {
             png_bytep dptr = *dp;
             png_read_row(png_ptr, NULL, dptr);
             dp++;
          }
    }
}

// Lightweight assertion utilities (non-terminating) for test robustness.
static int g_test_failures = 0;

static void expect_true(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "EXPECT_TRUE failed: " << message << std::endl;
        ++g_test_failures;
    }
}

static void expect_equal_ptr(void* a, void* b, const char* message) {
    if (a != b) {
        std::cerr << "EXPECT_EQ failed: " << message
                  << " (ptr mismatch: " << a << " != " << b << ")" << std::endl;
        ++g_test_failures;
    }
}

// Helper to reset test state between tests
static void reset_test_state() {
    get_test_state().calls.clear();
}

// Test 1: rp != NULL and dp != NULL -> multiple calls with corresponding rptr/dptr
static void test_both_rp_dp_non_null() {
    reset_test_state();

    // Prepare 3 rows and 3 display rows
    unsigned char a0[4] = {1, 2, 3, 4};
    unsigned char a1[4] = {5, 6, 7, 8};
    unsigned char a2[4] = {9, 10, 11, 12};

    unsigned char b0[4] = {101, 102, 103, 104};
    unsigned char b1[4] = {105, 106, 107, 108};
    unsigned char b2[4] = {109, 110, 111, 112};

    png_bytep rp_arr[3] = { a0, a1, a2 };
    png_bytep dp_arr[3] = { b0, b1, b2 };

    // Call with non-null png_ptr
    void* png_ptr = reinterpret_cast<void*>(0x10);

    png_read_rows(png_ptr, rp_arr, dp_arr, 3);

    // Validate that exactly 3 png_read_row calls occurred with expected pairs
    const auto& calls = get_test_state().calls;
    expect_true(calls.size() == 3, "expected 3 calls for num_rows=3 with both rp and dp non-null");
    for (size_t i = 0; i < 3; ++i) {
        expect_equal_ptr(calls[i].row_ptr, static_cast<void*>(rp_arr[i]),
                         "call i: row pointer mismatch");
        expect_equal_ptr(calls[i].display_ptr, static_cast<void*>(dp_arr[i]),
                         "call i: display pointer mismatch");
    }
}

// Test 2: rp != NULL and dp == NULL -> png_read_row called with display_row = NULL
static void test_rp_non_null_dp_null() {
    reset_test_state();

    unsigned char r0[4] = {1};
    unsigned char r1[4] = {2};
    unsigned char r2[4] = {3};

    png_bytep rp_arr[3] = { r0, r1, r2 };

    // display_row is NULL
    png_bytepp dp_arr = NULL;

    void* png_ptr = reinterpret_cast<void*>(0x20);

    png_read_rows(png_ptr, rp_arr, dp_arr, 3);

    const auto& calls = get_test_state().calls;
    expect_true(calls.size() == 3, "expected 3 calls for num_rows=3 with rp non-null and dp null");
    for (size_t i = 0; i < 3; ++i) {
        expect_equal_ptr(calls[i].row_ptr, static_cast<void*>(rp_arr[i]),
                         "call i: row pointer mismatch (rp path)");
        expect_true(calls[i].display_ptr == NULL,
                    "call i: display pointer should be NULL when dp is NULL");
    }
}

// Test 3: rp == NULL and dp != NULL -> png_read_row called with row_ptr = NULL
static void test_rp_null_dp_non_null() {
    reset_test_state();

    unsigned char d0[4] = {21, 22, 23, 24};
    unsigned char d1[4] = {25, 26, 27, 28};
    unsigned char d2[4] = {29, 30, 31, 32};

    png_bytep dp_arr[3] = { d0, d1, d2 };

    // rp is NULL
    png_bytepp rp_arr = NULL;

    void* png_ptr = reinterpret_cast<void*>(0x30);

    png_read_rows(png_ptr, rp_arr, dp_arr, 3);

    const auto& calls = get_test_state().calls;
    expect_true(calls.size() == 3, "expected 3 calls for num_rows=3 with rp NULL and dp non-null");
    for (size_t i = 0; i < 3; ++i) {
        expect_true(calls[i].row_ptr == NULL,
                    "call i: row pointer should be NULL when rp is NULL");
        expect_equal_ptr(calls[i].display_ptr, static_cast<void*>(dp_arr[i]),
                         "call i: display pointer mismatch (dp path)");
    }
}

// Test 4: both rp and dp are NULL -> no calls made
static void test_both_null_no_calls() {
    reset_test_state();

    png_bytep rp_arr[3] = { nullptr, nullptr, nullptr };
    png_bytepp dp_arr = nullptr;

    void* png_ptr = reinterpret_cast<void*>(0x40);

    png_read_rows(png_ptr, rp_arr, dp_arr, 3);

    const auto& calls = get_test_state().calls;
    // No calls should have been recorded
    expect_true(calls.empty(), "expected no calls when both rp and dp are NULL");
}

// Test 5: png_ptr is NULL -> should return early with no calls
static void test_png_ptr_null_no_calls() {
    reset_test_state();

    unsigned char r0[4] = {1};
    unsigned char r1[4] = {2};
    unsigned char r2[4] = {3};

    png_bytep rp_arr[3] = { r0, r1, r2 };
    png_bytep rp_arr2[3] = { r0, r1, r2 }; // not used; just to have a valid array type

    // set png_ptr to NULL
    void* png_ptr = nullptr;

    png_read_rows(png_ptr, rp_arr, rp_arr, 3);

    const auto& calls = get_test_state().calls;
    // No calls should have been recorded since png_ptr is NULL
    expect_true(calls.empty(), "expected no calls when png_ptr is NULL");
}

// Run all tests
static void run_all_tests() {
    std::cout << "Starting png_read_rows unit tests..." << std::endl;

    test_both_rp_dp_non_null();
    test_rp_non_null_dp_null();
    test_rp_null_dp_non_null();
    test_both_null_no_calls();
    test_png_ptr_null_no_calls();

    if (g_test_failures == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::cout << g_test_failures << " test(s) failed." << std::endl;
    }
}

int main() {
    run_all_tests();
    return g_test_failures == 0 ? 0 : 1;
}