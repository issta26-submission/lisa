// C++11 test suite for the focal function: init_user_chunk_info
// This test focuses on verifying that the function correctly assigns the
// provided info_ptr into the chunk_data structure and zeroes the rest of the
// structure memory as per its implementation.
// Note: We do not rely on GTest. A lightweight test harness is implemented.

#include <cstdint>
#include <vector>
#include <string.h>
#include <time.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <cstddef>


// Step 1: Provide C linkage for the focal function under test and minimal type
// definitions to enable compilation/linking with the existing C codebase.
// We intentionally keep the type definitions minimal and aligned with the
// observable usage inside init_user_chunk_info.

extern "C" {
    // The real libpng type alias in the library
    // Here we approximate as a generic pointer for compatibility in test code.
    typedef void* png_const_infop;

    // The focal struct used by the function under test.
    // We assume the info_ptr is the first (and likely only) member in the
    // library's user_chunk_info struct. This helps ensure a safe offset of 0
    // for the member in our synthetic definition used only in tests.
    typedef struct user_chunk_info {
        png_const_infop info_ptr; // expected member in the actual library struct
        // The real library may have additional members, but for the test we
        // only model the visible member that the focal function touches.
    } user_chunk_info;

    // Focal function prototype (C linkage)
    void init_user_chunk_info(png_const_infop info_ptr, user_chunk_info *chunk_data);
}

// Simple test harness ---------------------------------------------------------

class TestRunner {
public:
    TestRunner() : total_(0), passed_(0) {}

    // Generic equality assertion for arbitrary types
    template <typename T>
    void expect_eq(const T& a, const T& b, const std::string& msg) {
        ++total_;
        if (a == b) {
            ++passed_;
        } else {
            failures_.push_back("FAILED: " + msg);
            // Record a concise representation for pointers
            failures_.push_back("       actual  : " + to_string(a) +
                                " vs expected: " + to_string(b));
        }
    }

    void report() const {
        std::cout << "Test results: " << passed_ << " / " << total_ << " passed.\n";
        for (const auto& f : failures_) {
            std::cout << f << "\n";
        }
        if (!failures_.empty()) {
            std::cout << "Some tests failed.\n";
        } else {
            std::cout << "All tests passed.\n";
        }
    }

    bool all_passed() const { return failures_.empty(); }

private:
    size_t total_;
    size_t passed_;
    std::vector<std::string> failures_;

    // Small helper to print pointer-like values or integral values
    template <typename T>
    static std::string to_string(const T& value) {
        // Fallback for non-pointer types
        return std::to_string(static_cast<long long>(value));
    }

    // Overloads for pointer types to print address nicely
    static std::string to_string(void* ptr) {
        char buf[64];
        std::snprintf(buf, sizeof(buf), "%p", ptr);
        return std::string(buf);
    }

    // Varargs-friendly overload for pointers of unknown type
    template <typename U>
    static std::string to_string(const U& ptr) {
        return to_string(reinterpret_cast<void*>(ptr));
    }
};

// Test cases ------------------------------------------------------------------

// Test: init_user_chunk_info should set the provided info_ptr into chunk_data
// and (by the function's design) zero the rest of the memory region up to the
// size of the library's user_chunk_info struct. We validate only the visible
// effect: info_ptr is stored correctly.
void test_init_user_chunk_info_sets_info_ptr(TestRunner& tr) {
    // Prepare a non-null fake info_ptr
    png_const_infop info_ptr = reinterpret_cast<png_const_infop>(0xDEADBEEF);

    // Provide a large enough buffer to host the library's struct. We don't know
    // the exact size of the library's struct, so we allocate a generous buffer
    // and cast the beginning to our local struct type. This is safe as long as
    // the first member in the library's struct is info_ptr (assumed here).
    const std::size_t BUFFER_SIZE = 4096;
    std::vector<uint8_t> buffer(BUFFER_SIZE);
    user_chunk_info* chunk_data = reinterpret_cast<user_chunk_info*>(buffer.data());

    // Pre-fill with a non-zero pattern to ensure memset in the focal function would overwrite it.
    for (std::size_t i = 0; i < buffer.size(); ++i) {
        buffer[i] = static_cast<uint8_t>(0xAA);
    }

    // Call the focal function
    init_user_chunk_info(info_ptr, chunk_data);

    // Validate that info_ptr was stored correctly
    tr.expect_eq<void*>(chunk_data->info_ptr, info_ptr,
        "chunk_data->info_ptr should be equal to the provided info_ptr after init_user_chunk_info");
}

// Test: init_user_chunk_info should handle a null info_ptr gracefully.
// We verify that the function assigns a null to the info_ptr member when given a null input.
void test_init_user_chunk_info_sets_null_ptr(TestRunner& tr) {
    png_const_infop info_ptr = nullptr;

    const std::size_t BUFFER_SIZE = 1024;
    std::vector<uint8_t> buffer(BUFFER_SIZE);
    user_chunk_info* chunk_data = reinterpret_cast<user_chunk_info*>(buffer.data());

    // Initialize with a non-null value to start, ensuring the function overwrites it
    chunk_data->info_ptr = reinterpret_cast<png_const_infop>(0xBADC0DE);

    init_user_chunk_info(info_ptr, chunk_data);

    tr.expect_eq<png_const_infop>(chunk_data->info_ptr, nullptr,
        "chunk_data->info_ptr should be nullptr when init_user_chunk_info is called with null info_ptr");
}

// Main function to run tests ---------------------------------------------------

int main() {
    TestRunner tr;

    // Run test cases
    test_init_user_chunk_info_sets_info_ptr(tr);
    test_init_user_chunk_info_sets_null_ptr(tr);

    // Report results
    tr.report();

    // Exit with non-zero status if any test failed
    return tr.all_passed() ? 0 : 1;
}