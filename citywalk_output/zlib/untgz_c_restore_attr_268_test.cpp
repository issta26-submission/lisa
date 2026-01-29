// This test suite targets the restore_attr function from untgz.c.
// It uses a small, self-contained harness (no GTest) and relies on
// actual filesystem side-effects (mtime and mode) to validate behavior.
// The tests exercise the true/false branches by creating, processing,
// and verifying attribute restoration on real files.

#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>


// Forward declarations of the C functions defined in untgz.c.
// We assume these functions use C linkage.
extern "C" void push_attr(struct attr_item **list, char *fname, int mode, time_t time);
extern "C" void restore_attr(struct attr_item **list);

// Forward declaration of the struct used by the linked list.
// The test interacts with the API provided (push_attr/restore_attr) and
// does not rely on the internal layout of struct attr_item.
struct attr_item;

// Simple assertion helper for test output
#define ASSERT(cond, msg)                                 \
    do {                                                    \
        if (!(cond)) {                                      \
            std::cerr << "ASSERTION FAILED: " << msg << "\n"; \
            return false;                                   \
        }                                                   \
    } while (0)

// Candidate Keywords (Step 1)
 // - restore_attr: core method under test
 // - push_attr: helper to build the attr_item list
 // - attr_item: list node containing fname, time, mode, next
 // - setfiletime: dependency invoked inside restore_attr (time restoration)
 // - chmod: dependency invoked inside restore_attr (permissions)
 // - list pointer semantics: verifies NULL-ing of the list after processing

// Helper: create a temporary file and return its path
static bool create_temp_file(char* out_path, size_t len) {
    // Template for mkstemp to create a unique file
    char tmpl[] = "/tmp/untgztest-XXXXXX";
    int fd = mkstemp(tmpl);
    if (fd == -1) {
        perror("mkstemp");
        return false;
    }
    // Close the descriptor; the file exists now
    close(fd);
    if (strlen(tmpl) >= len) {
        // Should not happen with our buffer sizes, but guard anyway
        return false;
    }
    strncpy(out_path, tmpl, len);
    return true;
}

// Test 1: Empty list should be handled gracefully, no crash and list remains NULL
static bool test_empty_list() {
    struct attr_item *list = NULL;
    // Should not crash or modify list
    restore_attr(&list);
    ASSERT(list == NULL, "restore_attr should leave NULL list unchanged for empty input");
    return true;
}

// Test 2: Single item restoration should set mtime and mode, and NULL the list after
static bool test_single_item() {
    char path[256];
    if (!create_temp_file(path, sizeof(path))) {
        std::cerr << "Failed to create temp file for single item test\n";
        return false;
    }

    // Ensure initial mode is different from target
    // Start with 0600 (read/write for owner only)
    if (chmod(path, 0600) != 0) {
        perror("chmod initial");
        remove(path);
        return false;
    }

    time_t target_time = 1625097600; // 2021-07-01 00:00:00 UTC
    int target_mode = 0644;

    // Build the list with a single item
    struct attr_item *list = NULL;
    // We need a persistent C-string for fname; allocate and keep it valid
    char *fname = strdup(path);
    push_attr(&list, fname, target_mode, target_time);

    // Act: restore attributes
    restore_attr(&list);

    // Post-condition: list should be NULL
    ASSERT(list == NULL, "restore_attr should NULL the list after processing");

    // Verify time and mode on the actual file
    struct stat st;
    if (stat(path, &st) != 0) {
        perror("stat after restore_attr");
        remove(path);
        free(fname);
        return false;
    }

    // Compare permission bits (lower 9 bits)
    if ( (st.st_mode & 0777) != (target_mode & 0777) ) {
        std::cerr << "File mode mismatch: expected " << std::oct << (target_mode & 0777)
                  << ", got " << (st.st_mode & 0777) << std::dec << "\n";
        remove(path);
        free(fname);
        return false;
    }

    // Compare modification time
    // Note: Some FSs store times with sub-second resolution, but st_mtime is seconds.
    if (st.st_mtime != static_cast<time_t>(target_time)) {
        std::cerr << "File mtime mismatch: expected " << target_time
                  << ", got " << st.st_mtime << "\n";
        remove(path);
        free(fname);
        return false;
    }

    // Cleanup
    remove(path);
    free(fname);
    return true;
}

// Test 3: Multiple items restoration should handle all nodes, with independent attributes
static bool test_multiple_items() {
    char path1[256], path2[256];
    if (!create_temp_file(path1, sizeof(path1)) || !create_temp_file(path2, sizeof(path2))) {
        std::cerr << "Failed to create temp files for multiple items test\n";
        return false;
    }

    // Initialize different modes to ensure both are applied
    if (chmod(path1, 0600) != 0 || chmod(path2, 0700) != 0) {
        perror("chmod initial for multiple items");
        remove(path1);
        remove(path2);
        return false;
    }

    time_t t1 = 1610000000; // some past date
    time_t t2 = 1630000000; // a different past date
    int m1 = 0644;
    int m2 = 0755;

    struct attr_item *list = NULL;
    char *f1 = strdup(path1);
    char *f2 = strdup(path2);
    // Push two items; the order matters for traceability
    push_attr(&list, f1, m1, t1);
    push_attr(&list, f2, m2, t2);

    restore_attr(&list);

    // After processing, the list should be NULL
    if (list != NULL) {
        std::cerr << "List not NULL after processing multiple items\n";
        remove(path1);
        remove(path2);
        free(f1);
        free(f2);
        return false;
    }

    // Verify file 1
    struct stat st1;
    if (stat(path1, &st1) != 0) {
        perror("stat path1 after restore_attr");
        remove(path1);
        remove(path2);
        free(f1);
        free(f2);
        return false;
    }
    if ( (st1.st_mode & 0777) != (m1 & 0777) ) {
        std::cerr << "path1 mode mismatch. expected " << std::oct << (m1 & 0777)
                  << ", got " << (st1.st_mode & 0777) << std::dec << "\n";
        remove(path1);
        remove(path2);
        free(f1);
        free(f2);
        return false;
    }
    if (st1.st_mtime != t1) {
        std::cerr << "path1 mtime mismatch. expected " << t1
                  << ", got " << st1.st_mtime << "\n";
        remove(path1);
        remove(path2);
        free(f1);
        free(f2);
        return false;
    }

    // Verify file 2
    struct stat st2;
    if (stat(path2, &st2) != 0) {
        perror("stat path2 after restore_attr");
        remove(path1);
        remove(path2);
        free(f1);
        free(f2);
        return false;
    }
    if ( (st2.st_mode & 0777) != (m2 & 0777) ) {
        std::cerr << "path2 mode mismatch. expected " << std::oct << (m2 & 0777)
                  << ", got " << (st2.st_mode & 0777) << std::dec << "\n";
        remove(path1);
        remove(path2);
        free(f1);
        free(f2);
        return false;
    }
    if (st2.st_mtime != t2) {
        std::cerr << "path2 mtime mismatch. expected " << t2
                  << ", got " << st2.st_mtime << "\n";
        remove(path1);
        remove(path2);
        free(f1);
        free(f2);
        return false;
    }

    // Cleanup
    remove(path1);
    remove(path2);
    free(f1);
    free(f2);
    return true;
}

// Entry point: run all tests
int main() {
    int failures = 0;

    std::cout << "Starting unit tests for restore_attr in untgz.c (C++11 harness)\n";

    if (!test_empty_list()) {
        std::cerr << "Test 1 (empty list) failed.\n";
        failures++;
    } else {
        std::cout << "Test 1 (empty list) passed.\n";
    }

    if (!test_single_item()) {
        std::cerr << "Test 2 (single item) failed.\n";
        failures++;
    } else {
        std::cout << "Test 2 (single item) passed.\n";
    }

    if (!test_multiple_items()) {
        std::cerr << "Test 3 (multiple items) failed.\n";
        failures++;
    } else {
        std::cout << "Test 3 (multiple items) passed.\n";
    }

    if (failures) {
        std::cout << failures << " test(s) FAILED.\n";
        return 1;
    } else {
        std::cout << "All tests PASSED.\n";
        return 0;
    }
}