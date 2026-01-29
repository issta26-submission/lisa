// Test suite for the focal method: store_message
// This test suite targets the behavior of store_message(ps, pp, buffer, bufsize, pos, msg)
// under different branches (read, write, other) and with/without a test prefix.
// The tests are written in C++11 style, using a lightweight, non-GTest framework.
// Note: This test relies on the libpng internal types and the actual implementation
// of pngvalid.c being linked in. We purposefully avoid private methods and focus
// on public/externally-visible behavior of store_message.

#include <vector>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <math.h>


extern "C" {
}

// Lightweight test harness
static int total_tests = 0;
static int failed_tests = 0;

#define RUN_TEST(name) run_test(#name, [=]() { name(); })

// Helper: simple assertion for tests, non-terminating
#define ASSERT_TRUE(cond, message) do { \
    total_tests++; \
    if (!(cond)) { \
        ++failed_tests; \
        std::cerr << "TEST FAIL: " << message << "\n"; \
    } else { \
        std::cout << "TEST PASS: " << message << "\n"; \
    } \
} while (0)

// Helper: ends_with utility
static bool ends_with(const std::string &s, const char *suffix)
{
    if (suffix == nullptr) return false;
    size_t ls = s.size();
    size_t lr = std::strlen(suffix);
    if (lr > ls) return false;
    return s.compare(ls - lr, lr, suffix) == 0;
}

// Simple test runner
static void run_test(const char* name, const std::function<void()> &fn)
{
    std::cout << "Running test: " << name << "\n";
    fn();
}

// Global sep note: We do not rely on sep contents in assertions; we search for substrings
// like "read: ", "write: ", or "pngvalid: " and the provided message to verify behavior.

// Test 1: Read branch with no current (ps->current == NULL) and test prefix present
static void test_read_branch_with_test_and_no_current()
{
    // Prepare store and dummy pread pointer
    png_store ps;
    // Initialize store using the library's initialization
    store_init(&ps);

    void* read_ptr = malloc(1);
    void* write_ptr = malloc(1);

    ps.pread = (png_const_structp)read_ptr;
    ps.pwrite = (png_const_structp)write_ptr;
    ps.current = NULL; // Ensure no current->name is appended
    ps.wname = "OUTPUT.PNG";

    // Ensure test prefix exists
    ps.test[0] = '1';
    ps.test[1] = '\0';

    const char* msg = "TestReadMsg";

    char buffer[512];
    memset(buffer, 0, sizeof(buffer));

    size_t pos = 0;
    size_t bufsize = sizeof(buffer);

    // Call the focal function: correct branch (read)
    size_t newpos = store_message(&ps, ps.pread, buffer, bufsize, pos, msg);

    // Null-terminate up to the written length
    if (newpos < sizeof(buffer))
        buffer[newpos] = '\0';
    else
        buffer[sizeof(buffer) - 1] = '\0';

    std::string out(buffer);

    // Assertions: should contain "read: " and the message
    bool ok = (out.find("read: ") != std::string::npos) && out.find(msg) != std::string::npos;
    ASSERT_TRUE(ok, "store_message should produce 'read: ' prefix and include the message when reading and current is NULL");
    
    // Cleanup
    free(read_ptr);
    free(write_ptr);
}

// Test 2: Write branch with test prefix present
static void test_write_branch_with_test()
{
    png_store ps;
    store_init(&ps);

    void* read_ptr = malloc(1);
    void* write_ptr = malloc(1);

    ps.pread = (png_const_structp)read_ptr;
    ps.pwrite = (png_const_structp)write_ptr;
    ps.current = NULL;
    ps.wname = "OUTPUT.PNG";

    // Ensure test prefix exists
    ps.test[0] = 'A';
    ps.test[1] = '\0';

    const char* msg = "TestWriteMsg";

    char buffer[512];
    memset(buffer, 0, sizeof(buffer));

    size_t pos = 0;
    size_t bufsize = sizeof(buffer);

    // Call the focal function: write branch
    size_t newpos = store_message(&ps, ps.pwrite, buffer, bufsize, pos, msg);

    if (newpos < sizeof(buffer))
        buffer[newpos] = '\0';
    else
        buffer[sizeof(buffer) - 1] = '\0';

    std::string out(buffer);

    bool ok = (out.find("write: ") != std::string::npos) && out.find(ps.wname) != std::string::npos && out.find(msg) != std::string::npos;
    ASSERT_TRUE(ok, "store_message should produce 'write: ' prefix and include the message when writing and test is set");
    
    free(read_ptr);
    free(write_ptr);
}

// Test 3: Other branch (neither read nor write) when pp is NULL
static void test_other_branch_with_null_pp()
{
    png_store ps;
    store_init(&ps);

    void* read_ptr = malloc(1);
    void* write_ptr = malloc(1);

    ps.pread = (png_const_structp)read_ptr;
    ps.pwrite = (png_const_structp)write_ptr;
    ps.current = NULL;
    ps.wname = "OUTPUT.PNG";

    // Ensure test prefix exists
    ps.test[0] = 'X';
    ps.test[1] = '\0';

    const char* msg = "OtherBranchMsg";

    char buffer[512];
    memset(buffer, 0, sizeof(buffer));

    size_t pos = 0;
    size_t bufsize = sizeof(buffer);

    // Call the focal function: pass NULL pp to trigger the else branch
    size_t newpos = store_message(&ps, NULL, buffer, bufsize, pos, msg);

    if (newpos < sizeof(buffer))
        buffer[newpos] = '\0';
    else
        buffer[sizeof(buffer) - 1] = '\0';

    std::string out(buffer);

    bool ok = (out.find("pngvalid: ") != std::string::npos) && out.find(msg) != std::string::npos;
    ASSERT_TRUE(ok, "store_message should produce 'pngvalid: ' prefix and include the message for non-read/write (pp NULL)");
    
    free(read_ptr);
    free(write_ptr);
}

// Test 4: Read branch with test prefix disabled (ps->test[0] == 0)
static void test_read_branch_without_test_prefix()
{
    png_store ps;
    store_init(&ps);

    void* read_ptr = malloc(1);
    void* write_ptr = malloc(1);

    ps.pread = (png_const_structp)read_ptr;
    ps.pwrite = (png_const_structp)write_ptr;
    ps.current = NULL;
    ps.wname = "OUTPUT.PNG";

    // Disable test prefix
    ps.test[0] = 0;

    const char* msg = "MsgNoTest";

    char buffer[512];
    memset(buffer, 0, sizeof(buffer));

    size_t pos = 0;
    size_t bufsize = sizeof(buffer);

    // Read branch
    size_t newpos = store_message(&ps, ps.pread, buffer, bufsize, pos, msg);

    if (newpos < sizeof(buffer))
        buffer[newpos] = '\0';
    else
        buffer[sizeof(buffer) - 1] = '\0';

    std::string out(buffer);

    bool ok = (out.find("read: ") != std::string::npos) && out.find(msg) != std::string::npos;
    // Also ensure there is no unexpected test content appended
    bool noTestContent = (out.find("MSG_NO_TEST") == std::string::npos) && (out.find("test") == std::string::npos);
    ASSERT_TRUE(ok && noTestContent, "store_message should still prefix with 'read: ' and append message even when test prefix is empty; should not inject test content");
    
    free(read_ptr);
    free(write_ptr);
}

int main(void)
{
    // Run tests
    RUN_TEST(test_read_branch_with_test_and_no_current);
    RUN_TEST(test_write_branch_with_test);
    RUN_TEST(test_other_branch_with_null_pp);
    RUN_TEST(test_read_branch_without_test_prefix);

    std::cout << "=========================\n";
    std::cout << "Total tests: " << total_tests << "\n";
    std::cout << "Passed: " << (total_tests - failed_tests) << "\n";
    std::cout << "Failed: " << failed_tests << "\n";
    return (failed_tests == 0) ? 0 : 1;
}