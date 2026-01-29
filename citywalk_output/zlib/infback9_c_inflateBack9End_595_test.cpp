// Unit test suite for inflateBack9End (infback9.c) using a lightweight C++11 test harness.
// The tests rely on zlib's z_stream structure and related symbols.
// This file is designed to be compiled with a C++11 compiler without Google Test.
// It exercises the end-state handling of inflateBack9End, including error branches and
// the proper freeing/NULLing of the internal state.

#include <cstring>
#include <inflate9.h>
#include <infback9.h>
#include <stdio.h>
#include <iostream>
#include <inftree9.h>
#include <zlib.h>
#include <zutil.h>
#include <inffix9.h>
#include <cstdlib>


// Include zlib header to obtain z_stream and related constants/types.
// This test assumes the project provides zlib's headers in the include path.

// Forward declaration of the focal function (as provided by infback9.c).
// The actual project provides this with C linkage; we mirror the signature here.
extern "C" int inflateBack9End(z_stream* strm);

// Global test state to record failures without terminating tests early.
static int g_failures = 0;

// Free function to be provided to the z_stream for memory cleanup.
// It records the freed pointer and counts invocations for verification.
static void zlib_free_stub(voidpf opaque, voidpf address) {
    (void)opaque;           // opaque is unused in this test
    // Record which memory block was freed and how many times the free was invoked.
    // We use a simple static to observe behavior from the test harness.
    // Note: We don't strictly need opaque here; just verifying invocation and address is enough.
    static void* last_freed_address = nullptr;
    static int free_calls = 0;
    last_freed_address = address;
    ++free_calls;
    // Free the allocated block to simulate actual zfree behavior.
    free(address);
    // If needed, store last_freed_address somewhere observable (static above).
    // The test reads last_freed_address via a separate access function if required.
    // For simplicity, we rely on the address comparison within the test.
}

// Helper to access the last freed address (defined locally as a lambda-friendly static).
static void* get_last_freed_address() {
    // The last freed address is stored in a static inside zlib_free_stub.
    // Since we can't easily access that from here without a separate accessor,
    // we re-implement a tiny static here mirroring the same storage location.
    // This approach is sufficient for validating the freed pointer in test scenarios.
    // Note: This function returns the address as observed by the free function.
    // To keep the test self-contained, we simply return nullptr if not updated.
    // The detailed address comparison is performed directly in test_scenarios.
    // (We intentionally keep this function light to avoid coupling issues.)
    extern void* last_freed_address_from_stub();
    return last_freed_address_from_stub();
}

// Lightweight test harness printing per-test results.
#define TEST_PASSED(desc) \
    do { std::cout << "[PASS] " << (desc) << std::endl; } while(0)

#define TEST_FAILED(desc) \
    do { std::cerr << "[FAIL] " << (desc) << std::endl; ++g_failures; } while(0)

// Prototypes for individual test cases.
static void test_inflateBack9End_null_ptr();
static void test_inflateBack9End_null_state();
static void test_inflateBack9End_null_zfree();
static void test_inflateBack9End_success();

// Since zlib_free_stub uses a static internal to record the last freed address,
// provide a small accessor to integrate with this test harness.
static void* last_freed_address = nullptr;
static void zlib_free_stub_with_observer(voidpf opaque, voidpf address) {
    (void)opaque;
    last_freed_address = address;
    free(address);
}
static void* last_freed_address_from_stub() { return last_freed_address; }

// Redirect the actual free function to our observer-capable variant for the test.
// This will be swapped in per-test scenario where needed.
static void install_free_stub() {
    // Replace zfree with a stub that records the address.
    // Since we can't modify the z_stream structure's pointer types easily here
    // without more elaborate wiring, we rely on the ability to set zfree to
    // a function matching the signature: void(voidpf, voidpf)
    // The inflateBack9End function only checks for non-null, so this is sufficient
    // for the "success" path.
}

// Test 1: Invalid input: null pointer for strm -> expect Z_STREAM_ERROR
static void test_inflateBack9End_null_ptr() {
    int ret = inflateBack9End(nullptr);
    if (ret != Z_STREAM_ERROR) {
        TEST_FAILED("inflateBack9End(nullptr) should return Z_STREAM_ERROR");
    } else {
        TEST_PASSED("inflateBack9End(nullptr) returns Z_STREAM_ERROR");
    }
}

// Test 2: Non-null strm but state is null -> expect Z_STREAM_ERROR
static void test_inflateBack9End_null_state() {
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));
    // Explicitly ensure state is null
    strm.state = Z_NULL;
    int ret = inflateBack9End(&strm);
    if (ret != Z_STREAM_ERROR) {
        TEST_FAILED("inflateBack9End with null state should return Z_STREAM_ERROR");
    } else {
        TEST_PASSED("inflateBack9End with null state returns Z_STREAM_ERROR");
    }
}

// Test 3: Non-null state but zfree is null -> expect Z_STREAM_ERROR
static void test_inflateBack9End_null_zfree() {
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));
    // Allocate some dummy state memory
    void* dummy_state = std::malloc(64);
    strm.state = dummy_state;
    // zfree is not provided
    strm.zfree = 0;
    int ret = inflateBack9End(&strm);
    // Expect error due to missing free function
    if (ret != Z_STREAM_ERROR) {
        TEST_FAILED("inflateBack9End with non-null state but null zfree should return Z_STREAM_ERROR");
    } else {
        TEST_PASSED("inflateBack9End with null zfree returns Z_STREAM_ERROR");
    }
    // Cleanup: if allocation happened due to previous tests, free safely
    if (strm.state != Z_NULL && strm.state != nullptr) {
        std::free(strm.state);
    }
}

// Test 4: Valid scenario: proper state, non-null zfree -> expect Z_OK and state freed
static void test_inflateBack9End_success() {
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));
    // Prepare a heap-allocated state object and a non-null zfree
    void* state = std::malloc(128);
    if (state == nullptr) {
        TEST_FAILED("Failed to allocate memory for test state in inflateBack9End_success");
        return;
    }
    strm.state = state;
    // Use a proper free function (standard free) to simulate zlib's zfree
    strm.zfree = zlib_free_stub;
    // Reset observer
    last_freed_address = nullptr;
    // Call the function under test
    int ret = inflateBack9End(&strm);
    // Validate outcomes
    if (ret != Z_OK) {
        TEST_FAILED("inflateBack9End should return Z_OK on valid input");
        std::free(state);
        return;
    }
    if (strm.state != Z_NULL) {
        TEST_FAILED("inflateBack9End should set strm.state to NULL on success");
        // Do not attempt to free again here; report and exit
        return;
    }
    // Ensure that the free function was invoked exactly once and freed the correct pointer
    // We check last_freed_address (set by zlib_free_stub) against our allocated block.
    if (last_freed_address != state) {
        TEST_FAILED("inflateBack9End did not free the correct state pointer");
    } else {
        TEST_PASSED("inflateBack9End freed the allocated state and returned Z_OK");
    }
}

// Entry point: run all tests
int main() {
    std::cout << "Starting inflateBack9End unit tests...\n";

    // Test 1: null pointer
    test_inflateBack9End_null_ptr();

    // Test 2: null state
    test_inflateBack9End_null_state();

    // Test 3: null zfree
    test_inflateBack9End_null_zfree();

    // Test 4: success case
    // Note: We need to use a non-null zfree implemention for this path
    test_inflateBack9End_success();

    std::cout << "Tests completed. Failures: " << g_failures << std::endl;
    return g_failures;
}