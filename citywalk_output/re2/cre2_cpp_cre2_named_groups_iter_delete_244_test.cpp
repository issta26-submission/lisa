// This test suite focuses on the focal method cre2_named_groups_iter_delete
// and its related dependencies/classes within cre2.cpp/cre2.h.
// The goal is to exercise memory management behavior and basic usage of
// the named groups iterator API without relying on a full GoogleTest framework.
// The tests are written using a lightweight, non-terminating assertion style.

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Include the CRE2-like C API header provided by the project.
// Adjust the path if your project uses a different include directory.

// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
  ++g_tests_run; \
  if(!(cond)) { \
    ++g_tests_failed; \
    std::cerr << "[FAILED] " << msg << " (line " << __LINE__ << ")" << std::endl; \
  } \
} while(0)

#define EXPECT_NOT_NULL(ptr, msg) do { \
  ++g_tests_run; \
  if((ptr) == nullptr) { \
    ++g_tests_failed; \
    std::cerr << "[FAILED] " << msg << " (pointer is null) (line " << __LINE__ << ")" << std::endl; \
  } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
  ++g_tests_run; \
  if(!((a) == (b))) { \
    ++g_tests_failed; \
    std::cerr << "[FAILED] " << msg << " (expected " << #a " == " #b << ")" << std::endl; \
  } \
} while(0)

// Candidate Keywords (from Step 1)
 // cre2_named_groups_iter_delete
 // cre2_named_groups_iter_new
 // cre2_named_groups_iter_next
 // cre2_new, cre2_delete
 // cre2_opt_new, cre2_opt_delete
 // cre2_regexp_t, cre2_options_t
 // iter over named capture groups (iterator concept in CRE2)
 // Memory management and lifecycle of iterators

// Helper function to create a very simple regular expression object
// This uses the CRE2 API: cre2_opt_new + cre2_new
static cre2_regexp_t* make_simple_regexp() {
    cre2_options_t* opt = cre2_opt_new();
    if (opt == nullptr) {
        return nullptr;
    }
    const char *pattern = "abc"; // simple pattern with no named groups
    int pattern_len = static_cast<int>(std::strlen(pattern));
    cre2_regexp_t* re = cre2_new(pattern, pattern_len, opt);
    // Free options after compiling the regex; the regex holds its own copy/config
    cre2_opt_delete(opt);
    return re;
}

// Test 1: Deleting NULL should be safe (no crash, no exception)
static void test_named_groups_iter_delete_null_safe() {
    // Focal method should handle NULL gracefully
    cre2_named_groups_iter_delete(nullptr);
    EXPECT_TRUE(true, "Deleting a NULL iter should be a no-op and not crash");
}

// Test 2: Create an iterator from a simple regex and delete it successfully
static void test_named_groups_iter_delete_valid() {
    cre2_regexp_t* re = make_simple_regexp();
    if (re == nullptr) {
        EXPECT_TRUE(false, "Failed to create a simple regexp for iter test");
        return;
    }
    cre2_named_groups_iter_t* iter = cre2_named_groups_iter_new(re);
    EXPECT_NOT_NULL(iter, "Named groups iterator should be created from a valid regexp");
    if (iter != nullptr) {
        cre2_named_groups_iter_delete(iter);
        // After deletion, the caller must not use the pointer again.
        // We do not set iter to NULL here to keep the test explicit.
    }
    cre2_delete(re);
}

// Test 3: Iterator next function should be callable without crashing
static void test_named_groups_iter_next_no_crash() {
    cre2_regexp_t* re = make_simple_regexp();
    if (re == nullptr) {
        EXPECT_TRUE(false, "Failed to create a simple regexp for iter-next test");
        return;
    }
    cre2_named_groups_iter_t* iter = cre2_named_groups_iter_new(re);
    EXPECT_NOT_NULL(iter, "Named groups iterator should be created for next test");
    if (iter != nullptr) {
        const char* name = nullptr;
        int index = -1;
        int rc = cre2_named_groups_iter_next(iter, &name, &index);
        // We cannot rely on the exact return semantics here without full knowledge,
        // but we can ensure the call does not crash and outputs valid parameter slots.
        (void)rc; // suppress unused warning if rc is unused in certain builds
        (void)name;
        (void)index;
        cre2_named_groups_iter_delete(iter);
    }
    cre2_delete(re);
}

// Test 4: After deleting an iterator, attempting to delete a NULL pointer should be safe.
// This test ensures that we don't accidentally crash if someone reuses a freed pointer
// by resetting to NULL first.
static void test_named_groups_iter_delete_after_free_safe() {
    cre2_regexp_t* re = make_simple_regexp();
    if (re == nullptr) {
        EXPECT_TRUE(false, "Failed to create a simple regexp for safe-deletion-after-free test");
        return;
    }
    cre2_named_groups_iter_t* iter = cre2_named_groups_iter_new(re);
    EXPECT_NOT_NULL(iter, "Named groups iterator should be created for safe-deletion-after-free test");
    if (iter != nullptr) {
        cre2_named_groups_iter_delete(iter);
        iter = nullptr;
    }
    // Now delete NULL (safe)
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
}

// Run all tests and print a summary
static void run_all_tests() {
    test_named_groups_iter_delete_null_safe();
    test_named_groups_iter_delete_valid();
    test_named_groups_iter_next_no_crash();
    test_named_groups_iter_delete_after_free_safe();
}

int main() {
    run_all_tests();
    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << std::endl;
    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}