// C++11 minimal unit tests for sqlite3_quota_file_available
// This test suite exercises the focal function using a real FILE* backed quota_FILE.
// It avoids GTest and uses a tiny in-house test harness to allow non-terminating assertions.

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <test_quota.h>
#include <os_setup.h>
#include <iostream>


// The focal function and quota_FILE type are defined in the C quota subsystem.
// Include the C header that exposes quota_FILE and related definitions.

extern "C" long sqlite3_quota_file_available(quota_FILE *p);

// Simple lightweight test harness (non-terminating assertions)
// Keeps track of failures and reports a final summary.
static int g_failures = 0;
#define TEST_CHECK(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "FAIL: " << (msg) << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            ++g_failures; \
        } \
    } while(0)


// Helper to create a temporary in-memory file and populate with data.
// Returns FILE* on success, or nullptr on failure.
static FILE* create_temp_file_with_data(const char* data, size_t len) {
    // Use tmpfile() to create a temporary binary file that is automatically removed on close.
    FILE* f = tmpfile();
    if(!f) return nullptr;
    if(len > 0) {
        size_t written = fwrite(data, 1, len, f);
        if(written != len) {
            fclose(f);
            return nullptr;
        }
        fflush(f);
        // Start at beginning for tests that assume pos1 == 0
        if(fseek(f, 0, SEEK_SET) != 0) {
            fclose(f);
            return nullptr;
        }
    } else {
        // Ensure position is at 0 for empty content case
        if(fseek(f, 0, SEEK_SET) != 0) {
            fclose(f);
            return nullptr;
        }
    }
    return f;
}

// Test 1: Starting at beginning of a 10-byte buffer -> expect 10 bytes available
static void test_quota_file_available_from_start() {
    const char data[10] = "0123456789"; // 10 bytes
    FILE* f = create_temp_file_with_data(data, 10);
    TEST_CHECK(f != nullptr, "Unable to create temp file for test 1");

    quota_FILE q;
    // The quota_FILE struct is expected to have at least member 'f'
    q.f = f;

    long avail = sqlite3_quota_file_available(&q);
    TEST_CHECK(avail == 10, "Expected 10 bytes available from start (pos1=0, end at 10)");

    fclose(f);
}

// Test 2: Seek to a non-zero position (e.g., 4) then end; expect end - pos == remaining bytes
static void test_quota_file_available_from_middle() {
    const char data[10] = "0123456789"; // 10 bytes
    FILE* f = create_temp_file_with_data(data, 10);
    TEST_CHECK(f != nullptr, "Unable to create temp file for test 2");

    // Move to position 4 (simulate mid-file)
    int rc = fseek(f, 4, SEEK_SET);
    TEST_CHECK(rc == 0, "fseek to position 4 should succeed for test 2");

    quota_FILE q;
    q.f = f;

    long avail = sqlite3_quota_file_available(&q);
    TEST_CHECK(avail == 6, "Expected 6 bytes available from middle (pos1=4, end=10)");

    fclose(f);
}

// Test 3: Empty file should yield 0 available
static void test_quota_file_available_empty_file() {
    // Create an empty temp file
    FILE* f = tmpfile();
    TEST_CHECK(f != nullptr, "Unable to create empty temp file for test 3");

    // Ensure current position is at 0
    int rc = fseek(f, 0, SEEK_SET);
    TEST_CHECK(rc == 0, "fseek to 0 should succeed for test 3");

    quota_FILE q;
    q.f = f;

    long avail = sqlite3_quota_file_available(&q);
    TEST_CHECK(avail == 0, "Expected 0 bytes available for empty file");

    fclose(f);
}


// Entry point to run all tests
int main() {
    std::cout << "Running sqlite3_quota_file_available test suite (C++11)..." << std::endl;

    test_quota_file_available_from_start();
    test_quota_file_available_from_middle();
    test_quota_file_available_empty_file();

    if(g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) FAILED" << std::endl;
        return 1;
    }
}