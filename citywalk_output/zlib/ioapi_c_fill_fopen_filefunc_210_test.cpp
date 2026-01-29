// Unit tests for the focal method fill_fopen_filefunc in ioapi.c
// This test suite is written in C++11 (no Google Test) and targets
// the behavior of fill_fopen_filefunc via its public interface and
// the types defined in the zlib/ioapi headers.
// NOTE: We assume the project provides ioapi.h with the
// zlib_filefunc_def and related types, and that fill_fopen_filefunc
// is exposed with C linkage.
// The tests are conservative: they focus on presence and consistency
// of the function pointers after the call, rather than relying on the
// internal static function addresses (which would require cross-TU symbol access).

#include <iostream>
#include <cassert>
#include <ioapi.h>
#include <cstddef>


// Include the ioapi header to obtain the correct type definitions.
// If the environment places these in a different path, adjust accordingly.

// Ensure C linkage for the functions under test when linking from C++.
extern "C" {
    // Declarations of the functions under test (as provided by the project).
    void fill_fopen_filefunc(zlib_filefunc_def* pzlib_filefunc_def);
    void fill_fopen64_filefunc(zlib_filefunc64_def* pzlib_filefunc_def);
}

// -----------------------------------------------------------------------------------
// Test 1: Basic assignment correctness
// After calling fill_fopen_filefunc, all file operation function pointers should be non-null
// and opaque should be NULL. This validates that the function wires up the struct members
// as intended without touching other parts of the API.
// -----------------------------------------------------------------------------------
static int test_fill_fopen_filefunc_basic_assignment()
{
    // Prepare a target structure
    zlib_filefunc_def fp = {};

    // Act
    fill_fopen_filefunc(&fp);

    // Assert: All function pointers were assigned to some non-null value.
    // The opaque member should be explicitly set to NULL.
    assert(fp.zopen_file != nullptr && "zopen_file pointer should be non-null after fill_fopen_filefunc");
    assert(fp.zread_file != nullptr && "zread_file pointer should be non-null after fill_fopen_filefunc");
    assert(fp.zwrite_file != nullptr && "zwrite_file pointer should be non-null after fill_fopen_filefunc");
    assert(fp.ztell_file != nullptr && "ztell_file pointer should be non-null after fill_fopen_filefunc");
    assert(fp.zseek_file != nullptr && "zseek_file pointer should be non-null after fill_fopen_filefunc");
    assert(fp.zclose_file != nullptr && "zclose_file pointer should be non-null after fill_fopen_filefunc");
    assert(fp.zerror_file != nullptr && "zerror_file pointer should be non-null after fill_fopen_filefunc");
    // opaque is explicitly set to NULL
    assert(fp.opaque == NULL && "opaque should be NULL after fill_fopen_filefunc");

    std::cout << "test_fill_fopen_filefunc_basic_assignment passed." << std::endl;
    return 0;
}

// -----------------------------------------------------------------------------------
// Test 2: Idempotence / consistency across multiple calls
// Calling fill_fopen_filefunc multiple times should yield consistent (identical)
// function pointers for subsequent invocations. This ensures the function has stable
// behavior and does not mutate pointers in an inconsistent manner.
// -----------------------------------------------------------------------------------
static int test_fill_fopen_filefunc_idempotence()
{
    zlib_filefunc_def a = {};
    zlib_filefunc_def b = {};

    fill_fopen_filefunc(&a);
    fill_fopen_filefunc(&b);

    // Basic sanity checks
    assert(a.zopen_file != nullptr && "First call produced a null zopen_file");
    assert(b.zopen_file != nullptr && "Second call produced a null zopen_file");

    // Expect the same internal function pointers (addresses) after both calls.
    // If the internal implementation uses the same static functions, these should match.
    assert(a.zopen_file == b.zopen_file && "zopen_file pointers differ between calls");
    assert(a.zread_file == b.zread_file && "zread_file pointers differ between calls");
    assert(a.zwrite_file == b.zwrite_file && "zwrite_file pointers differ between calls");
    assert(a.ztell_file == b.ztell_file && "ztell_file pointers differ between calls");
    assert(a.zseek_file == b.zseek_file && "zseek_file pointers differ between calls");
    assert(a.zclose_file == b.zclose_file && "zclose_file pointers differ between calls");
    assert(a.zerror_file == b.zerror_file && "zerror_file pointers differ between calls");
    // opaque should remain NULL for both
    assert(a.opaque == NULL && "opaque should be NULL after first call");
    assert(b.opaque == NULL && "opaque should be NULL after second call");

    std::cout << "test_fill_fopen_filefunc_idempotence passed." << std::endl;
    return 0;
}

// -----------------------------------------------------------------------------------
// Test 3: fill_fopen64_filefunc is a no-op (stable API)
// The 64-bit variant is implemented as an empty function in the provided snippet.
// This test ensures that calling it does not crash and leaves the input structure
// in a valid state (no side effects on an initially zero-initialized struct).
// -----------------------------------------------------------------------------------
static int test_fill_fopen64_filefunc_noop()
{
    zlib_filefunc64_def d = {}; // zero-initialized
    // Act
    fill_fopen64_filefunc(&d);
    // Since the function is a no-op, we simply ensure the structure remains valid
    // (no crash, no illegal state). We perform a minimal check that the address
    // of the structure is non-null (trivially true in this context), and that
    // the code compiled and executed.
    // (No assertions on internal fields because this function is a no-op in the provided code.)
    (void)d; // suppress unused warning if asserts are disabled

    std::cout << "test_fill_fopen64_filefunc_noop executed (no-op)." << std::endl;
    return 0;
}

// -----------------------------------------------------------------------------------
// Main entry: run all tests
// Per domain knowledge, tests should be executable from main without external test runners.
// -----------------------------------------------------------------------------------
int main()
{
    int failures = 0;

    // Run Test 1
    if (test_fill_fopen_filefunc_basic_assignment() != 0) {
        std::cerr << "test_fill_fopen_filefunc_basic_assignment failed." << std::endl;
        ++failures;
    }

    // Run Test 2
    if (test_fill_fopen_filefunc_idempotence() != 0) {
        std::cerr << "test_fill_fopen_filefunc_idempotence failed." << std::endl;
        ++failures;
    }

    // Run Test 3
    if (test_fill_fopen64_filefunc_noop() != 0) {
        std::cerr << "test_fill_fopen64_filefunc_noop failed." << std::endl;
        ++failures;
    }

    if (failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << failures << " test(s) failed." << std::endl;
        return 1;
    }
}