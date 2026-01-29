// psicc_tests.cpp
// A lightweight, framework-free test suite for the focal main() of psicc.c
// This test suite is designed to be compiled in a C++11 environment without
// using any heavy testing frameworks (like Google Test).
// It follows the provided step-by-step guidance and includes explanatory
// comments for each test case.

// Important notes and assumptions:
// - The project under test is expected to be linkable as a separate translation unit
//   (psicc.c) providing the focal main() function and the referenced global/state
//   variables (cInProf, cOutProf, xoptind, OutFile) and the utility functions
//   (InitUtils, HandleSwitches, GenerateCSA, GenerateCRD).
// - We intentionally keep the tests non-terminating (they do not abort on failure),
//   instead they log results and continue, allowing multiple tests to run in a single
//   executable.
// - We avoid private members and any direct access to private state inside the
//   focal module beyond the declared externs.
// - The tests rely on the observable side effect of file creation when an output
//   filename is provided as an argument to main (nargs == 1 path).

#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <utils.h>


extern "C" {
    // Prototype of the focal main() under test
    int main(int argc, char *argv[]);

    // External symbols used by psicc.c (as seen in <FOCAL_CLASS_DEP>)
    // We assume these are defined somewhere in the project (psicc.c or linked libs)
    extern FILE *OutFile;        // Global output FILE*
    extern char *cInProf;        // Input profile indicator
    extern char *cOutProf;       // Output profile indicator
    extern int xoptind;           // Index into argv for first non-option arg

    // Utility functions
    extern void InitUtils(const char *);
    extern void HandleSwitches(int argc, char *argv[]);
    extern void GenerateCSA(void);
    extern void GenerateCRD(void);
}

// Simple non-terminating assertion macro
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        fprintf(stderr, "ASSERTION FAILED: %s (at %s:%d)\n", (msg), __FILE__, __LINE__); \
    } else { \
        fprintf(stdout, "ASSERTION PASSED: %s\n", (msg__dummy)); \
    } \
} while(0)

#define ASSERT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        fprintf(stderr, "ASSERTION FAILED: %s (expected %ld, found %ld) (at %s:%d)\n", (msg), (long)(b), (long)(a), __FILE__, __LINE__); \
    } else { \
        fprintf(stdout, "ASSERTION PASSED: %s\n", (msg__dummy)); \
    } \
} while(0)

// Helper: remove a file if it exists
static bool remove_file_if_exists(const char* path) {
    struct stat buffer;
    if (stat(path, &buffer) == 0) {
        return std::remove(path) == 0;
    }
    return true; // file doesn't exist, treat as success
}

// Helper: check whether a file exists (POSIX-friendly)
static bool file_exists(const char* path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

// Helper: determine file size if exists
static long file_size(const char* path) {
    struct stat st;
    if (stat(path, &st) != 0) return -1;
    return static_cast<long>(st.st_size);
}

// Helper: run the focal main() with given argv/argc
static int run_main_with_args(const std::vector<std::string>& args) {
    // Build argv array
    int argc = static_cast<int>(args.size());
    // We allocate a contiguous array of C-strings for the test call
    std::vector<char*> argv_ptrs;
    argv_ptrs.reserve(argc);
    for (auto const& s : args) {
        // Copy into a new C-string buffer to ensure it's null-terminated
        char* cstr = new char[s.size() + 1];
        std::strcpy(cstr, s.c_str());
        argv_ptrs.push_back(cstr);
    }

    // Ensure argv is NULL-terminated for safety (as per traditional main signature)
    argv_ptrs.push_back(nullptr);

    int ret = 0;
    // Call the project's main() function. This is a direct call to the focal entry point.
    // Note: Calling main() directly is generally discouraged, but the instructions imply
    // test code can invoke the focal main() to exercise control flow.
    ret = main(argc, argv_ptrs.data());

    // Clean up allocated argv strings
    for (char* p : argv_ptrs) {
        if (p) delete [] p;
    }

    return ret;
}

// Test 1: When an output filename is provided and both cInProf and cOutProf are NULL,
// the code should open the output file, proceed (subject to other predicates), and
// then close the file if nargs == 1. We verify the file is created (and optionally empty).
static void test_main_argc_two_creates_output_file() {
    printf("Test 1: argc=2 with an output filename; cInProf/cOutProf NULL\n");

    // Prepare: ensure previous test artefacts are removed
    const char* out_path = "psicc_test_out1.txt";
    remove_file_if_exists(out_path);

    // Setup: by convention, argv[0] is program name; argv[1] is the output filename
    std::vector<std::string> args = { "psicc", out_path };

    // Ensure global state for the test conditions
    cInProf = NULL;
    cOutProf = NULL;

    // Run the focal main()
    int ret = run_main_with_args(args);

    // Observations
    bool exists = file_exists(out_path);
    long size = exists ? file_size(out_path) : -1;

    // Assertions (non-terminating)
    // Expect a successful return (0)
    if (ret == 0) {
        printf("  [OK] main() returned 0 as expected.\n");
    } else {
        fprintf(stderr, "  [WARN] main() returned %d; expected 0.\n", ret);
    }

    // Expect the output file to exist (and ideally be empty, as no write is performed in this test)
    if (exists) {
        printf("  [INFO] Output file '%s' was created. Size: %ld bytes.\n", out_path, size);
        if (size == 0) {
            printf("  [OK] Output file is empty as expected for this test.\n");
        } else {
            printf("  [INFO] Output file is not empty (size > 0). This may indicate GenerateCSA/CRD wrote data; acceptable for coverage but noted.\n");
        }
    } else {
        fprintf(stderr, "  [WARN] Output file '%s' was NOT created. This may indicate early exit or branch conditions.\n", out_path);
    }

    // Cleanup: remove the artefact to avoid side effects on subsequent tests
    remove_file_if_exists(out_path);
}

// Test 2: Basic path with no additional arguments should return cleanly.
// This does not strictly validate internal branches but exercises the default path.
static void test_main_argc_one_returns_success() {
    printf("Test 2: argc=1 with no output file; expect clean return\n");

    // Ensure clean state
    const char* dummy_path = "psicc_test_out_none.txt";
    remove_file_if_exists(dummy_path);

    std::vector<std::string> args = { "psicc" };

    // Ensure global state for the test conditions
    cInProf = NULL;
    cOutProf = NULL;

    int ret = run_main_with_args(args);

    if (ret == 0) {
        printf("  [OK] main() returned 0 as expected.\n");
    } else {
        fprintf(stderr, "  [WARN] main() returned %d; expected 0.\n", ret);
    }

    // No output file should have been created in this scenario
    if (file_exists(dummy_path)) {
        fprintf(stderr, "  [WARN] Unexpected file '%s' created when argc=1.\n", dummy_path);
        remove_file_if_exists(dummy_path);
    } else {
        printf("  [OK] No output file created as expected.\n");
    }
}

// Entry point for running tests without a framework
int main() {
    printf(" psicc unit-test-suite: starting tests\n");
    test_main_argc_two_creates_output_file();
    test_main_argc_one_returns_success();
    printf(" psicc unit-test-suite: tests completed\n");
    return 0;
}