// Unit test suite for gzifstream::open(const char* name, std::ios_base::openmode mode)
// This test suite is designed to compile under C++11 without GTest and uses a lightweight
// test harness with non-terminating assertions to maximize code coverage.
// It relies on the zfstream.h interface provided by the project.

#include <cstring>
#include <cstdio>
#include <string>
#include <functional>
#include <iostream>
#include <vector>
#include <fstream>
#include <zfstream.h>


// Include the focal header with gzifstream and related classes.

// Lightweight test harness
static int g_failures = 0;

#define TEST_ASSERT(cond, msg) \
    do { if(!(cond)) { std::cerr << "[TEST FAILED] " << msg << " (in function " << __FUNCTION__ << ")\n"; ++g_failures; } } while(0)

// Test 1: Open an existing file in read mode should succeed and clear error state.
void test_open_success() {
    // Create a temporary test file
    const std::string fname = "gzifstream_open_success.tmp";
    {
        std::ofstream ofs(fname.c_str(), std::ios::binary);
        ofs << "data for test";
    }

    // Attempt to open the file with read mode. The implementation ORs with in, so this should succeed.
    gzifstream in;
    in.open(fname.c_str(), std::ios_base::in); // mode is augmented to include in by the implementation

    // The open should succeed and the stream should be in a good state (no failbit).
    TEST_ASSERT(in.good(), "gzifstream should be good after successful open of an existing file");

    // Cleanup
    in.close();
    std::remove(fname.c_str());
}

// Test 2: Open a non-existent file should fail and set failbit without setting badbit.
void test_open_failure() {
    const char* fname = "gzifstream_open_failure_nonexistent.tmp";

    // Ensure the file does not exist
    std::remove(fname);

    gzifstream in;
    in.open(fname, std::ios_base::in);

    // Expect fail state but not bad state
    TEST_ASSERT(in.fail(), "gzifstream should fail when opening a non-existent file (failbit set)");
    TEST_ASSERT(!in.bad(), "gzifstream should not set badbit on a normal open failure");
}

// Test 3: Ensure that after a successful open the object state is cleared (via clear()).
// This test additionally validates that the stream remains in a usable state for reading
// or further operations if supported by the backend after an open(true) path.
void test_open_success_state_cleared() {
    const std::string fname = "gzifstream_open_success_clear.tmp";
    {
        std::ofstream ofs(fname.c_str(), std::ios_base::binary);
        ofs << "payload";
    }

    gzifstream in;
    in.open(fname.c_str(), std::ios_base::in);

    // The state should be good and not contain failbit/badbit
    TEST_ASSERT(in.good(), "gzifstream should be good after open success (state cleared)");
    TEST_ASSERT(!in.fail(), "gzifstream should not be fail after open success");
    TEST_ASSERT(!in.bad(), "gzifstream should not be bad after open success");

    // Cleanup
    in.close();
    std::remove(fname.c_str());
}

// Run all tests and report summary
void runAllTests() {
    test_open_success();
    test_open_failure();
    test_open_success_state_cleared();

    if (g_failures == 0) {
        std::cout << "[ALL TESTS PASSED] gzifstream::open behavior is correct.\n";
    } else {
        std::cout << "[TEST SUMMARY] Failures: " << g_failures << "\n";
    }
}

// Main function as test entry point (no external test framework)
int main() {
    runAllTests();
    return g_failures ? 1 : 0;
}