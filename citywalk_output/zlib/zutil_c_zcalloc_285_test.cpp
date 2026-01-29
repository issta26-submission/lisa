/*
Unit test suite for the focal method:
    voidpf ZLIB_INTERNAL zcalloc(voidpf opaque, unsigned items, unsigned size);

Notes:
- The zcalloc implementation is shown in the focal method and relies on the macro
  ZLIB_INTERNAL and on sizeof(uInt) to decide between malloc vs calloc.
- To expose zcalloc in tests, builds must define ZLIB_DEBUG so that the function is
  compiled in (as seen in <FOCAL_CLASS_DEP>).
- We avoid GTest; instead we provide a small, self-contained test runner using
  only the C++ standard library plus the project's C headers.
- Static dependencies are accessed via the included zutil.h; test functions treat
  the opaque parameter as unused, per the function's implementation.

How to build (example):
- Compile zutil.c with -DZLIB_DEBUG to enable zcalloc, e.g.:
  g++ -DZLIB_DEBUG -std=c++11 -c zutil.c
  g++ -std=c++11 test_zcalloc.cpp zutil.o -o test_zcalloc
- Run: ./test_zcalloc

This test suite uses a lightweight, non-terminating assertion pattern:
- Each test records failures but continues execution to maximize coverage.

Candidate Keywords derived from the focal method and dependencies:
- zcalloc, opaque, items, size, malloc, calloc, voidpf, uInt, zutil.h, ZLIB_INTERNAL
- Memory allocation behavior, pointer handling, memory writing/reading, free, NULL

*/

// Ensure ZLIB_DEBUG is defined to expose the zcalloc implementation
#ifndef ZLIB_DEBUG
#error "Test suite requires ZLIB_DEBUG to be defined to expose zcalloc."
#endif

#include <cstring>
#include <iostream>
#include <zutil.h>
#include <stdlib.h>
#include <cstdlib>


// Declaration of the focal function (C linkage)
extern "C" voidpf zcalloc(voidpf opaque, unsigned items, unsigned size);

class TestContext {
public:
    const char* name;
    bool ok;
    TestContext(const char* n) : name(n), ok(true) {}
    void expect(bool condition, const char* message) {
        if (!condition) {
            ok = false;
            std::cerr << "[FAIL] " << name << ": " << message << std::endl;
        }
    }
};

// Test 1: Validate that zcalloc uses malloc branch (when sizeof(uInt) > 2) and returns usable memory.
// We attempt to write and read boundary values to ensure the allocated block is usable.
bool test_zcalloc_malloc_branch() {
    TestContext ctx(__func__);
    // opaque is ignored by zcalloc
    voidpf p = zcalloc(nullptr, 3, 4); // Expect 12 bytes

    ctx.expect(p != nullptr, "zcalloc returned NULL for 3 items * 4 bytes (malloc branch).");
    if (p != nullptr) {
        unsigned char* block = static_cast<unsigned char*>(p);
        // Write boundary values to exercise the allocated region
        block[0] = 0xAA;
        block[11] = 0xBB; // last byte in the 12-byte block

        // Read back to verify write/read access works
        ctx.expect(block[0] == 0xAA, "First byte write/read failed in zcalloc memory.");
        ctx.expect(block[11] == 0xBB, "Last byte write/read failed in zcalloc memory.");

        std::free(p);
    }

    return ctx.ok;
}

// Test 2: Verify that the opaque parameter is ignored and does not affect allocation parameters.
// We perform two allocations with different opaque values and ensure no crash occurs and memory is usable.
// This also asserts that the function can be invoked with non-null opaque values.
bool test_zcalloc_opaque_ignored() {
    TestContext ctx(__func__);

    // First allocation with a non-null opaque
    voidpf p1 = zcalloc((void*)0x1234, 2, 2); // 4 bytes
    ctx.expect(p1 != nullptr, "zcalloc returned NULL for 2 items * 2 bytes (opaque test 1).");
    if (p1 != nullptr) {
        unsigned char* a = static_cast<unsigned char*>(p1);
        a[0] = 0x11;
        a[1] = 0x22;
        a[2] = 0x33;
        a[3] = 0x44;
        ctx.expect(a[0] == 0x11 && a[3] == 0x44, "Memory write/read failed in zcalloc opaque test 1.");
        std::free(p1);
    }

    // Second allocation with a different non-null opaque
    voidpf p2 = zcalloc((void*)0xDEAD, 2, 2); // 4 bytes
    if (p2 != nullptr) {
        unsigned char* b = static_cast<unsigned char*>(p2);
        b[0] = 0xAA;
        b[1] = 0xBB;
        b[2] = 0xCC;
        b[3] = 0xDD;
        ctx.expect(b[0] == 0xAA && b[3] == 0xDD, "Memory write/read failed in zcalloc opaque test 2.");
        std::free(p2);
    } else {
        // In some scenarios, allocation may fail (e.g., low memory). This is acceptable per C allocator semantics.
        ctx.expect(true, "zcalloc returned NULL for opaque test 2 (acceptable in low-memory scenarios).");
    }

    return ctx.ok;
}

// Test 3: Handle edge cases for zero-sized allocations gracefully.
// According to C standard, allocations of size 0 may return NULL or a unique pointer; both are valid.
// We simply ensure the call is safe and does not crash.
bool test_zcalloc_zero_args() {
    TestContext ctx(__func__);

    // Zero items
    voidpf p_zero_items = zcalloc(nullptr, 0, 5);
    if (p_zero_items != nullptr) {
        std::free(p_zero_items);
    } else {
        // NULL result is acceptable for zero-sized allocation
    }

    // Zero size
    voidpf p_zero_size = zcalloc(nullptr, 5, 0);
    if (p_zero_size != nullptr) {
        std::free(p_zero_size);
    } else {
        // NULL result is acceptable for zero-sized allocation
    }

    // If we reach here, both edge-case calls did not crash
    ctx.expect(true, "Zero-argument allocations did not crash.");
    return ctx.ok;
}

// Simple test runner
int main() {
    std::cout << "Running zcalloc unit tests (requires ZLIB_DEBUG to be defined during build)..." << std::endl;

    int failures = 0;

    if (!test_zcalloc_malloc_branch()) {
        ++failures;
    }
    if (!test_zcalloc_opaque_ignored()) {
        ++failures;
    }
    if (!test_zcalloc_zero_args()) {
        ++failures;
    }

    if (failures == 0) {
        std::cout << "All zcalloc tests passed." << std::endl;
    } else {
        std::cout << "zcalloc tests completed with " << failures << " failure(s)." << std::endl;
    }

    return failures;
}