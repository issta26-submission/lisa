// Minimal C++11 unit test suite for fill_fopen64_filefunc in ioapi.c
// - No GTest; tests are executed from main
// - Uses a lightweight test harness with non-terminating assertions
// - Focuses on functional behavior of fill_fopen64_filefunc as described in the focal method

#include <cstring>
#include <string>
#include <cstdint>
#include <iostream>
#include <vector>
#include <ioapi.h>



static std::vector<std::string> g_failures; // collect failures without aborting tests

// Helper: log a failure message (non-throwing)
static void logFailure(const std::string& msg) {
    g_failures.push_back(msg);
}

// Test 1: Ensure fill_fopen64_filefunc initializes all required function pointers
// and sets opaque to NULL. This validates the primary contract of the focal method.
static void test_fill64_sets_all_pointers_and_null_opaque() {
    // Arrange
    zlib_filefunc64_def def;
    std::memset(&def, 0, sizeof(def)); // clear to simulate uninitialized state

    // Act
    fill_fopen64_filefunc(&def);

    // Assert
    bool ok = true;
    ok = ok && (def.zopen64_file != NULL);
    ok = ok && (def.zread_file  != NULL);
    ok = ok && (def.zwrite_file != NULL);
    ok = ok && (def.ztell64_file != NULL);
    ok = ok && (def.zseek64_file != NULL);
    ok = ok && (def.zclose_file != NULL);
    ok = ok && (def.zerror_file != NULL);
    ok = ok && (def.opaque == NULL);

    if (!ok) {
        logFailure("test_fill64_sets_all_pointers_and_null_opaque: One or more fields were not properly initialized by fill_fopen64_filefunc.");
        logFailure("Pointers: zopen64_file=" +
                   std::to_string(def.zopen64_file != NULL) + ", zread_file=" +
                   std::to_string(def.zread_file != NULL) + ", zwrite_file=" +
                   std::to_string(def.zwrite_file != NULL) + ", ztell64_file=" +
                   std::to_string(def.ztell64_file != NULL) + ", zseek64_file=" +
                   std::to_string(def.zseek64_file != NULL) + ", zclose_file=" +
                   std::to_string(def.zclose_file != NULL) + ", zerror_file=" +
                   std::to_string(def.zerror_file != NULL) + ", opaque=" +
                   (def.opaque == NULL ? "NULL" : "NON-NULL"));
    }
}

// Test 2: Ensure that fill_fopen64_filefunc overwrites pre-initialized (sentinel) values.
// This checks that the function does not preserve prior garbage values.
static void test_fill64_overwrites_sentinals() {
    // Arrange: initialize with sentinel non-null values
    zlib_filefunc64_def def;
    voidpf sentinel = reinterpret_cast<voidpf>(reinterpret_cast<void*>(0x1234));
    def.zopen64_file  = sentinel;
    def.zread_file   = sentinel;
    def.zwrite_file  = sentinel;
    def.ztell64_file  = sentinel;
    def.zseek64_file  = sentinel;
    def.zclose_file  = sentinel;
    def.zerror_file  = sentinel;
    def.opaque       = reinterpret_cast<voidpf>(nullptr); // explicitly NULL

    // Act
    fill_fopen64_filefunc(&def);

    // Assert: all fields should be overwritten and no longer equal to sentinel
    bool ok = true;
    ok = ok && (def.zopen64_file  != sentinel);
    ok = ok && (def.zread_file   != sentinel);
    ok = ok && (def.zwrite_file  != sentinel);
    ok = ok && (def.ztell64_file  != sentinel);
    ok = ok && (def.zseek64_file  != sentinel);
    ok = ok && (def.zclose_file  != sentinel);
    ok = ok && (def.zerror_file  != sentinel);
    // opaque should be NULL as set by implementation
    ok = ok && (def.opaque == NULL);

    if (!ok) {
        logFailure("test_fill64_overwrites_sentinals: fill_fopen64_filefunc did not overwrite sentinel values as expected.");
    }
}

// Test 3: Idempotence/consistency check: multiple invocations produce the same pointers.
// The function should assign the same known non-NULL function pointers on every call.
static void test_fill64_idempotence_of_pointers() {
    zlib_filefunc64_def def1;
    memset(&def1, 0, sizeof(def1));
    fill_fopen64_filefunc(&def1);

    // Capture the first set of pointers
    voidpf first_zopen64 = def1.zopen64_file;
    voidpf first_zread  = def1.zread_file;
    voidpf first_zwrite = def1.zwrite_file;
    voidpf first_ztell64 = def1.ztell64_file;
    voidpf first_zseek64 = def1.zseek64_file;
    voidpf first_zclose64 = def1.zclose_file;
    voidpf first_zerror  = def1.zerror_file;
    voidpf first_opaque  = def1.opaque;

    // Act: second invocation
    fill_fopen64_filefunc(&def1);

    // Assert: pointers should be identical across calls
    bool ok = true;
    ok = ok && (def1.zopen64_file  == first_zopen64);
    ok = ok && (def1.zread_file   == first_zread);
    ok = ok && (def1.zwrite_file  == first_zwrite);
    ok = ok && (def1.ztell64_file  == first_ztell64);
    ok = ok && (def1.zseek64_file  == first_zseek64);
    ok = ok && (def1.zclose_file  == first_zclose64);
    ok = ok && (def1.zerror_file  == first_zerror);
    ok = ok && (def1.opaque       == first_opaque);

    if (!ok) {
        logFailure("test_fill64_idempotence_of_pointers: pointers differ between consecutive calls to fill_fopen64_filefunc.");
    }
}

// Test 4: Edge case: ensure function preserves its semantics when struct is already zeroed
// This helps ensure there are no uninitialized reads affecting behavior.
static void test_fill64_when_already_zero_initialized() {
    zlib_filefunc64_def def;
    // Do not memset here to simulate a random state; test that function still assigns non-NULLs
    // However, to keep test deterministic, we reset it just before call
    std::memset(&def, 0, sizeof(def));
    fill_fopen64_filefunc(&def);

    bool ok = true;
    ok = ok && (def.zopen64_file != NULL);
    ok = ok && (def.zread_file  != NULL);
    ok = ok && (def.zwrite_file != NULL);
    ok = ok && (def.ztell64_file != NULL);
    ok = ok && (def.zseek64_file != NULL);
    ok = ok && (def.zclose_file != NULL);
    ok = ok && (def.zerror_file != NULL);
    ok = ok && (def.opaque     == NULL);

    if (!ok) {
        logFailure("test_fill64_when_already_zero_initialized: fill_fopen64_filefunc failed to initialize fields on zeroed input.");
    }
}

// Simple test runner
static void run_all_tests() {
    test_fill64_sets_all_pointers_and_null_opaque();
    test_fill64_overwrites_sentinals();
    test_fill64_idempotence_of_pointers();
    test_fill64_when_already_zero_initialized();
}

// Entry point
int main() {
    run_all_tests();

    if (g_failures.empty()) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_failures.size() << " TEST(S) FAILED:\n";
        for (const auto& msg : g_failures) {
            std::cerr << "- " << msg << "\n";
        }
        return 1;
    }
}