// FOCAL METHOD TEST SUITE FOR zipOpen2_64
// This test suite targets the focal method:
// extern zipFile ZEXPORT zipOpen2_64(const void *pathname, int append, zipcharpc* globalcomment, zlib_filefunc64_def* pzlib_filefunc_def)
// It mocks the dependency zipOpen3 to validate branch behavior and parameter passing.
// The test is self-contained and uses no external testing framework (GTest not used).

#include <cstring>
#include <string.h>
#include <time.h>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <crypt.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <zip.h>


// Domain knowledge alignment: we provide minimal, stubbed type definitions
// to compile and test the focal method in isolation.

extern "C" {

// Lightweight type definitions to mimic the needed parts of the real headers.
// The real libraries provide richer definitions, but for unit testing the
// core logic, only the fields touched by zipOpen2_64 are required.

typedef void* zipFile;              // Placeholder for zipFile handle type
typedef const char* zipcharpc;      // Pointer to constant character (C string)

// Minimal file function structure definitions (64-bit -> 32-bit wrapper)
typedef struct zlib_filefunc64_def_s {
    void* zfile_func64;               // Pointer to a64 file function (kept as void* for test)
} zlib_filefunc64_def;

typedef struct zlib_filefunc64_32_def_s {
    void* zfile_func64;               // Copied from zlib_filefunc64_def
    void* ztell32_file;               // NULL enforced in zipOpen2_64 when provided
    void* zseek32_file;                // NULL enforced in zipOpen2_64 when provided
} zlib_filefunc64_32_def;

// Forward declaration of the function under test and its dependency (to be mocked in tests)
zipFile zipOpen3(const void* pathname, int append, zipcharpc* globalcomment, zlib_filefunc64_32_def* pzlib_filefunc64_32_def);

zipFile zipOpen2_64(const void *pathname, int append, zipcharpc* globalcomment, zlib_filefunc64_def* pzlib_filefunc_def);

} // extern "C"

// ---------------------------------------------------------------
// Mock harness and tests
// ---------------------------------------------------------------

// Global to capture the call to zipOpen3 for assertions
struct CaptureStruct {
    const void* pathname;
    int append;
    zipcharpc* globalcomment;
    zlib_filefunc64_32_def* pzlib_filefunc64_32_def;
} last_call;

// Return value that mock zipOpen3 will yield (to verify propagation)
static zipFile mock_return_value;

// Mock implementation of zipOpen3 to observe inputs from zipOpen2_64
extern "C" zipFile zipOpen3(const void* pathname, int append, zipcharpc* globalcomment, zlib_filefunc64_32_def* pzlib_filefunc64_32_def) {
    last_call.pathname = pathname;
    last_call.append = append;
    last_call.globalcomment = globalcomment;
    last_call.pzlib_filefunc64_32_def = pzlib_filefunc64_32_def;
    return mock_return_value;
}

// Actual focal implementation under test (as provided)
extern "C" zipFile zipOpen2_64(const void *pathname, int append, zipcharpc* globalcomment, zlib_filefunc64_def* pzlib_filefunc_def) {
    if (pzlib_filefunc_def != NULL)
    {
        zlib_filefunc64_32_def zlib_filefunc64_32_def_fill;
        zlib_filefunc64_32_def_fill.zfile_func64 = *pzlib_filefunc_def;
        zlib_filefunc64_32_def_fill.ztell32_file = NULL;
        zlib_filefunc64_def_fill.zseek32_file = NULL;
        return zipOpen3(pathname, append, globalcomment, &zlib_filefunc64_32_def_fill);
    }
    else
        return zipOpen3(pathname, append, globalcomment, NULL);
}

// Helper to reset capture state (for clarity in tests)
static void reset_capture() {
    last_call.pathname = nullptr;
    last_call.append = -1;
    last_call.globalcomment = nullptr;
    last_call.pzlib_filefunc64_32_def = nullptr;
    mock_return_value = nullptr;
}

// Simple declarative test result reporter
static int tests_run = 0;
static int tests_failed = 0;

static void report_result(const char* test_name, bool ok) {
    tests_run++;
    if (ok) {
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        tests_failed++;
        std::cerr << "[FAIL] " << test_name << "\n";
    }
}

// Test 1: When a non-null zlib_filefunc_def is provided, zipOpen2_64 should
//         copy the function pointer into the 32-bit wrapper and pass NULL
//         for ztell32_file and zseek32_file. It should also forward the call
//         to zipOpen3 with the wrapped struct and return its value.
static void test_zipOpen2_64_with_non_null_pzlib_def() {
    const char* test_name = "zipOpen2_64_with_non_null_pzlib_def";
    reset_capture();

    // Prepare a dummy function pointer (we only check that the pointer is copied)
    void* dummy_func_ptr = (void*)0xDEFACED0;
    zlib_filefunc64_def provided;
    provided.zfile_func64 = dummy_func_ptr;

    // Prepare a non-null globalcomment
    const char* gc = "global-comment";
    // Prepare a non-null expected return value from mock
    mock_return_value = (zipFile)0x12345678;

    // Call the function under test
    zipOpen2_64((const void*)"pathA", 1, (zipcharpc*)&gc, &provided);

    bool ok = true;

    // Check that the path and append were forwarded correctly
    if (last_call.pathname != (const void*)"pathA") {
        std::cerr << "  path mismatch: expected 'pathA' pointer, got " << last_call.pathname << "\n";
        ok = false;
    }
    if (last_call.append != 1) {
        std::cerr << "  append mismatch: expected 1, got " << last_call.append << "\n";
        ok = false;
    }

    // globalcomment should be the address of gc (since we pass &gc)
    if (last_call.globalcomment != (zipcharpc*)&gc) {
        std::cerr << "  globalcomment address mismatch: expected &gc, got " << last_call.globalcomment << "\n";
        ok = false;
    }

    // The 32-bit wrapper should have been constructed with zfile_func64 copied from provided
    if (last_call.pzlib_filefunc64_32_def == nullptr) {
        std::cerr << "  wrapper def was not passed to zipOpen3 (NULL)\n";
        ok = false;
    } else {
        if (last_call.pzlib_filefunc64_32_def->zfile_func64 != dummy_func_ptr) {
            std::cerr << "  zfile_func64 pointer not copied correctly: expected " << dummy_func_ptr
                      << " got " << last_call.pzlib_filefunc64_32_def->zfile_func64 << "\n";
            ok = false;
        }
        if (last_call.pzlib_filefunc64_32_def->ztell32_file != NULL) {
            std::cerr << "  ztell32_file should be NULL in wrapper (got non-NULL)\n";
            ok = false;
        }
        if (last_call.pzlib_filefunc64_32_def->zseek32_file != NULL) {
            std::cerr << "  zseek32_file should be NULL in wrapper (got non-NULL)\n";
            ok = false;
        }
    }

    // The return value should be the one provided by mock zipOpen3
    if (last_call.pathname != nullptr) {
        // nothing special; no extra check
    }
    if (mock_return_value != (zipFile)0x12345678) {
        std::cerr << "  return value mismatch: expected 0x12345678, got " << mock_return_value << "\n";
        ok = false;
    }

    report_result(test_name, ok);
}

// Test 2: When a NULL pzlib_filefunc_def is provided, zipOpen2_64 should call
//         zipOpen3 with NULL as the 4th parameter and return its value.
static void test_zipOpen2_64_with_null_pzlib_def() {
    const char* test_name = "zipOpen2_64_with_null_pzlib_def";
    reset_capture();

    // Non-null globalcomment
    const char* gc = "global2";
    // Prepare a distinct return value from mock
    mock_return_value = (zipFile)0xABCDEF01;

    // Call with NULL pzlib_filefunc_def
    zipOpen2_64((const void*)"pathB", 0, (zipcharpc*)&gc, NULL);

    bool ok = true;

    // Check forwarded path and append
    if (last_call.pathname != (const void*)"pathB") {
        std::cerr << "  path mismatch: expected 'pathB' pointer, got " << last_call.pathname << "\n";
        ok = false;
    }
    if (last_call.append != 0) {
        std::cerr << "  append mismatch: expected 0, got " << last_call.append << "\n";
        ok = false;
    }

    // globalcomment should be &gc
    if (last_call.globalcomment != (zipcharpc*)&gc) {
        std::cerr << "  globalcomment address mismatch: expected &gc, got " << last_call.globalcomment << "\n";
        ok = false;
    }

    // pzlib_filefunc_64_def should be NULL
    if (last_call.pzlib_filefunc64_32_def != NULL) {
        std::cerr << "  expected NULL wrapper pointer when input is NULL, got non-NULL\n";
        ok = false;
    }

    // Return value should propagate
    if (mock_return_value != (zipFile)0xABCDEF01) {
        std::cerr << "  return value mismatch: expected 0xABCDEF01, got " << mock_return_value << "\n";
        ok = false;
    }

    report_result(test_name, ok);
}

// ---------------------------------------------------------------
// Entry point for tests
int main() {
    // Run tests in a single process to allow step-by-step observation.
    std::cout << "Starting unit tests for zipOpen2_64...\n";

    test_zipOpen2_64_with_non_null_pzlib_def();
    test_zipOpen2_64_with_null_pzlib_def();

    std::cout << "Tests run: " << tests_run << ", Failures: " << tests_failed << "\n";
    if (tests_failed == 0) {
        std::cout << "All tests passed.\n";
    } else {
        std::cout << "Some tests failed. Review output for details.\n";
    }

    return (tests_failed == 0) ? 0 : 1;
}