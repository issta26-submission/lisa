// Unit tests for inflateBackEnd using zlib types (C++11, no GoogleTest)
// This test suite exercises the FOCAL_METHOD inflateBackEnd as provided.
// It relies on zlib's z_streamp type, Z_NULL, Z_OK, Z_STREAM_ERROR, and the
// ZFREE macro semantics via a user-provided zfree implementation.
// NOTE: This test assumes zlib is available (link with -lz).

#include <cstdio>
#include <cstring>
#include <inftrees.h>
#include <zlib.h>
#include <inffast.h>
#include <zutil.h>
#include <inflate.h>
#include <cstdlib>


// Import zlib types and constants
extern "C" {
}

// Prototype for the function under test (C linkage)
extern "C" int inflateBackEnd(z_streamp strm);

// Global flag used by the free function to verify it's invoked
static bool freed_called = false;

// Free function used by the test to track ZFREE invocation.
// The signature matches zlib's free_func: void (*)(voidpf, voidpf)
// Here we use plain void* due to typical typedefs in zlib (<void*>, <void*>).
extern "C" void test_free_func(void* /*opaque*/, void* /*address*/) {
    freed_called = true;
    // Free the memory if provided (safe-guard)
    if (address != nullptr) {
        free(address);
    }
}

// Test 1: Null z_streamp pointer should return Z_STREAM_ERROR
bool test_inflateBackEnd_null_ptr() {
    z_streamp strm = Z_NULL;
    int ret = inflateBackEnd(strm);
    if (ret != Z_STREAM_ERROR) {
        printf("FAIL: test_inflateBackEnd_null_ptr - expected Z_STREAM_ERROR, got %d\n", ret);
        return false;
    }
    printf("PASS: test_inflateBackEnd_null_ptr\n");
    return true;
}

// Test 2: Non-null pointer but null state should return Z_STREAM_ERROR
bool test_inflateBackEnd_null_state() {
    z_stream s;
    // Explicitly simulate missing internal state
    s.state = Z_NULL;
    s.zfree = test_free_func; // set a non-null zfree to ensure it's not the failing predicate
    z_streamp strm = &s;

    int ret = inflateBackEnd(strm);
    if (ret != Z_STREAM_ERROR) {
        printf("FAIL: test_inflateBackEnd_null_state - expected Z_STREAM_ERROR, got %d\n", ret);
        return false;
    }
    printf("PASS: test_inflateBackEnd_null_state\n");
    return true;
}

// Test 3: Non-null state but zeroed zfree function pointer should return Z_STREAM_ERROR
bool test_inflateBackEnd_null_freefunc() {
    z_stream s;
    s.state = (void*)0x1234; // non-null placeholder
    s.zfree = (free_func)0;   // explicitly null-free function (cast to correct type)
    z_streamp strm = &s;

    int ret = inflateBackEnd(strm);
    if (ret != Z_STREAM_ERROR) {
        printf("FAIL: test_inflateBackEnd_null_freefunc - expected Z_STREAM_ERROR, got %d\n", ret);
        return false;
    }
    printf("PASS: test_inflateBackEnd_null_freefunc\n");
    return true;
}

// Test 4: Valid input path should free internal state and return Z_OK
bool test_inflateBackEnd_free_state() {
    z_stream s;
    // Allocate a non-null internal state to be freed by ZFREE
    void* allocated = malloc(256);
    if (allocated == nullptr) {
        printf("ERROR: malloc failed in test_inflateBackEnd_free_state\n");
        return false;
    }
    // Reset flag
    freed_called = false;

    s.state = allocated;
    s.zfree = test_free_func;
    z_streamp strm = &s;

    int ret = inflateBackEnd(strm);

    // After end, the code should set state to Z_NULL and return Z_OK
    bool ok = (ret == Z_OK) && (s.state == Z_NULL) && freed_called;
    if (!ok) {
        printf("FAIL: test_inflateBackEnd_free_state - ret=%d, state=%p, freed=%d\n",
               ret, (void*)s.state, freed_called ? 1 : 0);
        // Cleanup in case nothing freed (defensive)
        if (s.state != Z_NULL && s.state != nullptr && allocated != nullptr) {
            // best-effort; but if ZFREE didn't run, we should not double-free
            // We rely on test to catch memory leaks in real scenarios.
        }
        // Do not free allocated here as inflateBackEnd should have freed it
        return false;
    }

    printf("PASS: test_inflateBackEnd_free_state\n");
    return true;
}

// Simple test runner (non-terminating)
int main() {
    printf("Starting inflateBackEnd unit tests...\n");

    int total = 0;
    int passed = 0;

    if (test_inflateBackEnd_null_ptr()) passed++;
    total++;

    if (test_inflateBackEnd_null_state()) passed++;
    total++;

    if (test_inflateBackEnd_null_freefunc()) passed++;
    total++;

    if (test_inflateBackEnd_free_state()) passed++;
    total++;

    printf("Tests completed: %d/%d passed.\n", passed, total);
    return (passed == total) ? 0 : 1;
}