// Unit test suite for readpng2_init in readpng2.c
// - C++11 compatible
// - No GTest usage; a lightweight in-file test harness is implemented
// - Tests focus on readpng2_init behavior under realistic conditions using libpng
// - Uses the project's own types from readpng2.h to ensure compatibility
// - Explanatory comments accompany each test

#include <setjmp.h>
#include <zlib.h>
#include <readpng2.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <png.h>


namespace test_readpng2_init_suite {

// Test 1: Basic successful initialization
// Description:
// - Provides a zero-initialized mainprog_info structure.
// - Calls readpng2_init and expects a successful return (0).
// - Verifies that libpng structures are created and assigned to mainprog_ptr fields.
// - Cleans up allocated png structures to avoid leaks.
bool test_readpng2_init_success() {
    mainprog_info mp;
    // Zero-initialize the structure to a clean state
    std::memset(&mp, 0, sizeof(mp));

    int ret = readpng2_init(&mp);

    bool success = (ret == 0) &&
                   (mp.png_ptr != nullptr) &&
                   (mp.info_ptr != nullptr);

    // Clean up allocated resources if initialization succeeded
    if (mp.png_ptr != nullptr && mp.info_ptr != nullptr) {
        // png_destroy_read_struct signature: void png_destroy_read_struct(png_structpp, png_infopp, png_infopp)
        // We pass addresses of the pointers to null them after destruction.
        png_destroy_read_struct(&mp.png_ptr, &mp.info_ptr, NULL);
        // After destruction, pointers should be NULL, but we won't rely on that post-cleanup.
    }

    // Explain outcome
    if (!success) {
        std::cerr << "[Test] test_readpng2_init_success: FAILED (ret=" << ret
                  << ", png_ptr=" << mp.png_ptr
                  << ", info_ptr=" << mp.info_ptr << ")" << std::endl;
    } else {
        std::cout << "[Test] test_readpng2_init_success: PASSED" << std::endl;
    }

    return success;
}

// Test 2: Repeated initialization should also succeed without crashing
// Description:
// - Call readpng2_init twice on the same mp structure (after proper cleanup).
// - Each call should return 0 and set non-null png_ptr and info_ptr.
// - Ensures that repeated use of the initializer is safe and consistent.
bool test_readpng2_init_repeated() {
    mainprog_info mp;
    std::memset(&mp, 0, sizeof(mp));

    int ret1 = readpng2_init(&mp);
    bool ok1 = (ret1 == 0) && (mp.png_ptr != nullptr) && (mp.info_ptr != nullptr);

    // Cleanup resources from first initialization
    if (mp.png_ptr != nullptr && mp.info_ptr != nullptr) {
        png_destroy_read_struct(&mp.png_ptr, &mp.info_ptr, NULL);
    }

    // Second initialization attempt
    int ret2 = readpng2_init(&mp);
    bool ok2 = (ret2 == 0) && (mp.png_ptr != nullptr) && (mp.info_ptr != nullptr);

    // Cleanup again after second initialization
    if (mp.png_ptr != nullptr && mp.info_ptr != nullptr) {
        png_destroy_read_struct(&mp.png_ptr, &mp.info_ptr, NULL);
    }

    bool success = ok1 && ok2;
    if (!success) {
        std::cerr << "[Test] test_readpng2_init_repeated: FAILED (ret1=" << ret1
                  << ", ret2=" << ret2
                  << ", mp.png_ptr=" << mp.png_ptr
                  << ", mp.info_ptr=" << mp.info_ptr << ")" << std::endl;
    } else {
        std::cout << "[Test] test_readpng2_init_repeated: PASSED" << std::endl;
    }

    return success;
}

// Optional: Run all tests in this suite and report a summary.
// This function ensures tests are executed in a single run, enabling coverage tracking.
void run_all_tests() {
    int tests_run = 0;
    int tests_passed = 0;

    auto run = [&](bool (*test_func)(), const char* name) {
        ++tests_run;
        bool result = test_func();
        if (result) ++tests_passed;
        // The individual test already outputs its own PASS/FAIL line.
        // We simply track counts here for a final summary via the main function.
        return result;
    };

    bool t1 = run(test_readpng2_init_success, "test_readpng2_init_success");
    bool t2 = run(test_readpng2_init_repeated, "test_readpng2_init_repeated");

    std::cout << "Test suite summary: " << tests_passed << " / "
              << tests_run << " tests passed." << std::endl;
}

} // namespace test_readpng2_init_suite

// Main entry point for the test program
int main() {
    // Run all tests in the suite
    test_readpng2_init_suite::run_all_tests();
    return 0;
}