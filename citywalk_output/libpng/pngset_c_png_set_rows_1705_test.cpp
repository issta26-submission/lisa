// Test suite for png_set_rows function (reproduced minimal environment for unit testing)
// This file provides a self-contained test harness without GTest, using only C++11 standard library.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>


// Domain-adapted minimal stubs to emulate the libpng-like environment necessary for png_set_rows.
// This is a focused, self-contained test harness and does not require the real libpng.

typedef unsigned char png_byte;
typedef png_byte* png_bytep;
typedef unsigned char** png_bytepp; // pointer to an array of pointers (per libpng convention)

// Forward declare the png_ptr type as void* to keep tests simple and independent.
typedef void* png_const_structrp;

// Minimal infor structure to mirror the subset used by png_set_rows.
struct PNGInfo {
    png_bytepp row_pointers;
    unsigned int valid;
};

// Pointer type alias for infor structure
typedef PNGInfo* png_inforp;

// Constants used by png_set_rows implementation
static const int PNG_FREE_ROWS = 1;
static const unsigned int PNG_INFO_IDAT = 0x01;

// Debug/log helper (no-op for test determinism)
void png_debug(int /*level*/, const char* /*msg*/) {
    // Intentionally left blank to avoid noisy test output
}

// Global tracking for calls to png_free_data to validate correct behavior
static int g_free_data_calls = 0;
static void* g_last_free_png_ptr = nullptr;
static png_inforp g_last_free_info_ptr = nullptr;
static int g_last_free_type = -1;
static int g_last_free_num = -1;

void png_free_data(void* png_ptr, png_inforp info_ptr, int free_type, int num) {
    ++g_free_data_calls;
    g_last_free_png_ptr = png_ptr;
    g_last_free_info_ptr = info_ptr;
    g_last_free_type = free_type;
    g_last_free_num = num;
}

// The focal method under test (reproduced according to provided snippet)
void png_set_rows(png_const_structrp png_ptr, png_inforp info_ptr, png_bytepp row_pointers) {
    png_debug(1, "in png_set_rows");
    if (png_ptr == nullptr || info_ptr == nullptr)
        return;
    if (info_ptr->row_pointers != nullptr && (info_ptr->row_pointers != row_pointers))
        png_free_data(png_ptr, info_ptr, PNG_FREE_ROWS, 0);
    info_ptr->row_pointers = row_pointers;
    if (row_pointers != nullptr)
        info_ptr->valid |= PNG_INFO_IDAT;
}

// Simple assertion helpers for test readability
#define ASSERT_TRUE(cond, msg)                                  \
    do { if (!(cond)) {                                         \
            std::cerr << "ASSERT TRUE FAILED: " << (msg) << "\n"; \
            return false;                                      \
        } } while (0)

#define ASSERT_EQ(a, b, msg)                                    \
    do { if ((a) != (b)) {                                        \
            std::cerr << "ASSERT EQUAL FAILED: " << (msg) << " ("
                      << #a << " = " << (a) << ", "
                      << #b << " = " << (b) << ")\n";           \
            return false;                                        \
        } } while (0)

#define ASSERT_PTR_EQ(a, b, msg)                                 \
    do { if ((void*)(a) != (void*)(b)) {                          \
            std::cerr << "ASSERT PTR EQUAL FAILED: " << (msg) << " ("
                      << #a << " pointer = " << a << ", "
                      << #b << " pointer = " << b << ")\n";        \
            return false;                                        \
        } } while (0)

#define TEST_REPORT(name, passed)                                   \
    do {                                                            \
        std::cout << (passed ? "[PASS] " : "[FAIL] ") << name << "\n"; \
    } while (0)

// Test 1: When png_ptr is NULL, png_set_rows should return immediately and not modify info_ptr
// This covers the first predicate true branch (early return).
bool test_png_set_rows_null_png_ptr_and_nonnull_info() {
    // Arrange
    static int A = 100;
    static int B = 200;
    png_bytepp oldRowPointers = reinterpret_cast<png_bytepp>(static_cast<void*>(&A));
    PNGInfo info;
    info.row_pointers = oldRowPointers;
    info.valid = 0;

    png_const_structrp png_ptr = nullptr; // NULL
    png_bytepp newRowPointers = reinterpret_cast<png_bytepp>(static_cast<void*>(&B));

    // Act
    png_set_rows(png_ptr, &info, newRowPointers);

    // Assert: no change to info_ptr
    ASSERT_EQ(info.row_pointers, oldRowPointers, "info_ptr.row_pointers should remain unchanged when png_ptr is NULL");
    ASSERT_EQ(info.valid, 0u, "info_ptr.valid should remain unchanged when png_ptr is NULL");
    ASSERT_TRUE(g_free_data_calls == 0, "png_free_data should not be called when png_ptr is NULL");

    return true;
}

// Test 2: When info_ptr->row_pointers is non-NULL and different from new row_pointers, png_free_data should be invoked
// and info_ptr.row_pointers should be updated; valid should have PNG_INFO_IDAT set.
bool test_png_set_rows_free_old_when_different() {
    // Arrange
    static int A = 10;
    static int B = 20;
    png_bytepp oldRowPointers = reinterpret_cast<png_bytepp>(static_cast<void*>(&A));
    png_bytepp newRowPointers = reinterpret_cast<png_bytepp>(static_cast<void*>(&B));

    PNGInfo info;
    info.row_pointers = oldRowPointers;
    info.valid = 0;

    // Non-null png_ptr
    png_const_structrp png_ptr = reinterpret_cast<png_const_structrp>(static_cast<void*>(reinterpret_cast<uint8_t*>(0xDEADBEEF)));

    // Reset free data log
    g_free_data_calls = 0;
    g_last_free_info_ptr = nullptr;
    g_last_free_png_ptr = nullptr;
    g_last_free_type = -1;
    g_last_free_num = -1;

    // Act
    png_set_rows(png_ptr, &info, newRowPointers);

    // Assert
    ASSERT_EQ(g_free_data_calls, 1, "png_free_data should be called exactly once when old pointers differ");
    ASSERT_PTR_EQ(g_last_free_info_ptr, &info, "png_free_data should be called with the correct info_ptr");
    ASSERT_PTR_EQ(g_last_free_png_ptr, png_ptr, "png_free_data should be called with the correct png_ptr");
    ASSERT_EQ(g_last_free_type, PNG_FREE_ROWS, "png_free_data should be called with PNG_FREE_ROWS");
    ASSERT_EQ(g_last_free_num, 0, "png_free_data should be called with num = 0");

    ASSERT_PTR_EQ(info.row_pointers, newRowPointers, "info_ptr.row_pointers should be updated to newRowPointers");
    ASSERT_TRUE((info.valid & PNG_INFO_IDAT) != 0, "info_ptr.valid should have PNG_INFO_IDAT bit set after non-null row_pointers");

    return true;
}

// Test 3: When info_ptr->row_pointers equals the new row_pointers, png_free_data should not be called
// and info_ptr.row_pointers should remain the same; valid should be set for non-null row_pointers.
bool test_png_set_rows_no_free_when_same_pointer() {
    // Arrange
    static int A = 30;
    static int B = 40;
    png_bytepp oldRowPointers = reinterpret_cast<png_bytepp>(static_cast<void*>(&A));
    png_bytepp sameRowPointers = reinterpret_cast<png_bytepp>(static_cast<void*>(&B));

    PNGInfo info;
    info.row_pointers = sameRowPointers;
    info.valid = 0;

    // Non-null png_ptr
    png_const_structrp png_ptr = reinterpret_cast<png_const_structrp>(static_cast<void*>(reinterpret_cast<uint8_t*>(0xFEEDBEEF)));

    // Reset free data log
    g_free_data_calls = 0;

    // Act
    png_set_rows(png_ptr, &info, sameRowPointers);

    // Assert
    ASSERT_EQ(g_free_data_calls, 0, "png_free_data should not be called when old and new pointers are the same");
    ASSERT_PTR_EQ(info.row_pointers, sameRowPointers, "info_ptr.row_pointers should remain the same as newRowPointers");
    ASSERT_TRUE((info.valid & PNG_INFO_IDAT) != 0, "info_ptr.valid should have PNG_INFO_IDAT bit set when row_pointers is non-null");

    return true;
}

// Test 4: When row_pointers argument is NULL, info_ptr.row_pointers should become NULL
// and the PNG_INFO_IDAT bit should not be set.
bool test_png_set_rows_null_row_pointers_causes_no_valid_set() {
    // Arrange
    static int A = 55;
    static int B = 66;
    png_bytepp oldRowPointers = reinterpret_cast<png_bytepp>(static_cast<void*>(&A));
    png_bytepp nullRowPointers = nullptr;

    PNGInfo info;
    info.row_pointers = oldRowPointers;
    info.valid = 0;

    // Non-null png_ptr
    png_const_structrp png_ptr = reinterpret_cast<png_const_structrp>(static_cast<void*>(reinterpret_cast<uint8_t*>(0xC0FFEE)));

    // Reset free data log
    g_free_data_calls = 0;

    // Act
    png_set_rows(png_ptr, &info, nullRowPointers);

    // Assert
    ASSERT_EQ(info.row_pointers, nullptr, "info_ptr.row_pointers should be set to NULL when row_pointers is NULL");
    ASSERT_EQ(info.valid, 0u, "info_ptr.valid should remain unchanged when row_pointers is NULL");
    ASSERT_EQ(g_free_data_calls, 0, "png_free_data should not be called when row_pointers is NULL");

    return true;
}

// Main entry: run all tests and report
int main() {
    int total = 0;
    int passed = 0;

    std::cout << "Running unit tests for png_set_rows ..." << std::endl;

    bool t1 = test_png_set_rows_null_png_ptr_and_nonnull_info();
    total++; if (t1) ++passed; TEST_REPORT("Test 1: Null png_ptr early return", t1);

    bool t2 = test_png_set_rows_free_old_when_different();
    total++; if (t2) ++passed; TEST_REPORT("Test 2: Free old pointers when different", t2);

    bool t3 = test_png_set_rows_no_free_when_same_pointer();
    total++; if (t3) ++passed; TEST_REPORT("Test 3: No free when old == new", t3);

    bool t4 = test_png_set_rows_null_row_pointers_causes_no_valid_set();
    total++; if (t4) ++passed; TEST_REPORT("Test 4: row_pointers NULL handling", t4);

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}