/*
Unit test suite for the focal method: updatewindow
Context:
- The test targets the inflate-related internal function updatewindow as defined
  in inflate.c (under MAKEFIXED path in the provided source).
- We assume the environment provides zlib-like headers (zlib.h, inflate.h, zutil.h)
  so that types like z_streamp, Bytef, z_stream, inflate_state, etc., are available.
- The test is written in plain C++11 (no Google Test). A small, self-contained
  test harness is used that invokes updatewindow via extern "C" linkage.
- We test both branches and edge cases:
  1) allocate window and copy full window (copy >= wsize)
  2) partial copy with wrap-around (copy < wsize, with non-zero wnex)
  3) allocation failure path
  4) initialization path where wsize is zero and window is allocated
- For memory allocation, we provide custom alloc/free functions to satisfy ZALLOC/ZFREE
  usage in the code under test.
- Each test prints PASS/FAIL with a brief description.
*/

#include <cstring>
#include <stdio.h>
#include <iostream>
#include <inftrees.h>
#include <zlib.h>
#include <inffast.h>
#include <zutil.h>
#include <inflate.h>
#include <cstdlib>


// Include the C headers from zlib-like sources.
// These headers must be available in the build environment.
// They define z_streamp, z_stream, Bytef, inflate_state, etc.
extern "C" {
}

// Declaration of the focal function (assuming non-static visibility in this build).
extern "C" int updatewindow(z_streamp strm, const Bytef *end, unsigned copy);

// Simple test framework (lightweight, no GTest)

// Allocation helpers to satisfy ZALLOC/ZFREE usage in updatewindow.
static voidpf test_alloc(voidpf opaque, uInt items, uInt size) {
    (void)opaque;
    // Basic malloc-based allocator
    return (voidpf)malloc((size_t)items * (size_t)size);
}
static void test_free(voidpf opaque, voidpf address) {
    (void)opaque;
    free(address);
}

// Utility to create and initialize a z_streamp environment suitable for updatewindow
static z_streamp create_test_stream(int wbits) {
    z_streamp strm = (z_streamp)calloc(1, sizeof(z_stream));
    if (strm == NULL) return NULL;
    // Provide allocator hooks expected by ZALLOC:
    strm->zalloc = test_alloc;
    strm->zfree  = test_free;
    strm->opaque = NULL;

    // Allocate and initialize inflate_state
    strm->state = (struct internal_state FAR *)calloc(1, sizeof(struct inflate_state));
    if (strm->state == NULL) {
        free(strm);
        return NULL;
    }
    // Initialize the inflate_state minimally for updatewindow usage
    struct inflate_state FAR *state = (struct inflate_state FAR *)strm->state;
    state->strm = strm;
    state->window = NULL;
    state->wbits = (unsigned)wbits;
    state->wsize = 0;
    state->wnext = 0;
    state->whave = 0;

    return strm;
}

// Cleanup helper
static void destroy_test_stream(z_streamp strm) {
    if (!strm) return;
    struct inflate_state FAR *state = (struct inflate_state FAR *)strm->state;
    if (state) {
        if (state->window) free(state->window);
        free(state);
    }
    free(strm);
}

// Test 1: Full window copy when copy >= wsize after allocation
static bool test_updatewindow_full_copy() {
    // Prepare environment
    int wbits = 4;              // wsize = 16
    z_streamp strm = create_test_stream(wbits);
    if (!strm) {
        std::cerr << "Test1: Failed to create test stream\n";
        return false;
    }
    struct inflate_state FAR *state = (struct inflate_state FAR *)strm->state;

    // Ensure first call allocates the window and initializes size
    state->window = nullptr;
    state->wsize = 0;
    state->wnext = 0;
    state->whave = 0;

    // Prepare end buffer: we need endBuf[0..15] for last 16 bytes, and endPtr pointing to endBuf[16]
    const unsigned int WSIZE = 1U << wbits; // 16
    unsigned char endBuf[WSIZE * 2];          // 32 bytes
    for (unsigned i = 0; i < WSIZE; ++i) endBuf[i] = (unsigned char)(i + 1);      // 1..16
    // The last copy bytes to copy are endBuf[0..WSIZE-1] (we will copy the first WSIZE bytes)
    unsigned char* endPtr = endBuf + WSIZE; // end points to endBuf[16]

    // Call: copy >= wsize
    unsigned copy = WSIZE; // 16
    int ret = updatewindow(strm, endPtr, copy);
    bool ok = (ret == 0);

    // Validate: window allocated, wsize set, wnext == 0, whave == wsize
    if (!ok) {
        std::cerr << "Test1: updatewindow returned non-zero\n";
        destroy_test_stream(strm);
        return false;
    }
    if (state->window == NULL) {
        std::cerr << "Test1: window was not allocated\n";
        destroy_test_stream(strm);
        return false;
    }
    if (state->wsize != WSIZE) {
        std::cerr << "Test1: wsize incorrect, expected " << WSIZE << " got " << state->wsize << "\n";
        destroy_test_stream(strm);
        return false;
    }
    if (state->wnext != 0) {
        std::cerr << "Test1: wnex expected 0, got " << state->wnext << "\n";
        destroy_test_stream(strm);
        return false;
    }
    if (state->whave != WSIZE) {
        std::cerr << "Test1: whave expected " << WSIZE << ", got " << state->whave << "\n";
        destroy_test_stream(strm);
        return false;
    }
    // Check copied data matches endBuf[0..15]
    bool data_ok = true;
    for (unsigned i = 0; i < WSIZE; ++i) {
        if (state->window[i] != endBuf[i]) {
            data_ok = false;
            break;
        }
    }
    if (!data_ok) {
        std::cerr << "Test1: window data mismatch after full copy\n";
        destroy_test_stream(strm);
        return false;
    }

    destroy_test_stream(strm);
    return true;
}

// Test 2: Partial copy when copy < wsize with non-zero wnxt (wrap-around behavior)
static bool test_updatewindow_partial_copy_with_wrap() {
    int wbits = 3; // wsize = 8
    z_streamp strm = create_test_stream(wbits);
    if (!strm) {
        std::cerr << "Test2: Failed to create test stream\n";
        return false;
    }
    struct inflate_state FAR *state = (struct inflate_state FAR *)strm->state;

    // Prepare a window of size 8 and set an initial non-zero wnxt
    state->window = nullptr;
    state->wsize = 0;
    state->wnext = 2; // start near the middle
    state->whave = 0;

    // We need to ensure allocation occurs for window of size wsize
    // Provide end buffer where endBuf[0..dist-1] will be copied into window[wnext..wnext+dist-1]
    const unsigned int WSIZE = 1U << wbits; // 8
    // endBuf needs to provide endBuf[0..dist-1], dist will be min(copy, wsize-wnext)
    // We'll use copy = 6, so dist = wsize - wnext = 6
    unsigned char endBuf[WSIZE + 6]; // 8 + 6 = 14
    // Fill endBuf[0..5] with known pattern to verify writing into window[2..7]
    for (unsigned i = 0; i < 6; ++i) endBuf[i] = (unsigned char)(0xA0 + i);
    // Allocate endPtr to point after the first window region
    unsigned char* endPtr = endBuf + WSIZE; // endBuf[8]

    int ret = updatewindow(strm, endPtr, 6); // copy < wsize
    bool ok = (ret == 0);
    if (!ok) {
        std::cerr << "Test2: updatewindow returned non-zero\n";
        destroy_test_stream(strm);
        return false;
    }

    // Validate: window allocated and data copied into window[wnext..wnext+dist-1] == endBuf[0..5]
    if (state->window == NULL) {
        std::cerr << "Test2: window was not allocated\n";
        destroy_test_stream(strm);
        return false;
    }
    // Check data in window[2..7]
    bool data_ok = true;
    for (unsigned i = 0; i < 6; ++i) {
        if (state->window[state->wnext + i] != endBuf[i]) { // wnex after operation should be 0, but verify actual positions
            // Note: since wnex will wrap after 2+6=8 -> 0, but we can check the target indices 2..7
            data_ok = false;
            break;
        }
    }
    // Since the wrap logic sets wnex to 0 after addition, we can explicitly validate window[2..7] equals endBuf[0..5].
    // If wnex wrapped to 0, window[2+i] should match endBuf[i] for i in 0..5.
    if (!data_ok) {
        // Attempt to validate with wrap assumption
        for (unsigned i = 0; i < 6; ++i) {
            if (state->window[2 + i] != endBuf[i]) {
                data_ok = false;
                break;
            }
        }
    }
    if (!data_ok) {
        std::cerr << "Test2: partial copy data mismatch in window\n";
        destroy_test_stream(strm);
        return false;
    }

    // Validate wnex wrap: 2 + dist (6) => 8 -> wrap to 0
    if (state->wnext != 0) {
        std::cerr << "Test2: wnex did not wrap to 0 as expected, got " << state->wnext << "\n";
        destroy_test_stream(strm);
        return false;
    }

    // Validate whave increased by dist
    if (state->whave != 6) {
        std::cerr << "Test2: whave expected 6, got " << state->whave << "\n";
        destroy_test_stream(strm);
        return false;
    }

    destroy_test_stream(strm);
    return true;
}

// Test 3: Allocation failure path (ZALLOC returns NULL)
static bool test_updatewindow_allocation_failure() {
    int wbits = 4;
    z_streamp strm = (z_streamp)calloc(1, sizeof(z_stream));
    if (!strm) {
        std::cerr << "Test3: Failed to allocate z_stream\n";
        return false;
    }
    // Force allocation to fail by setting zalloc to NULL; inflate expects ZALLOC to return NULL
    strm->zalloc = NULL;
    strm->zfree = NULL;
    strm->opaque = NULL;
    strm->state = (struct internal_state FAR *)calloc(1, sizeof(struct inflate_state));
    if (!strm->state) {
        free(strm);
        std::cerr << "Test3: Failed to allocate inflate_state\n";
        return false;
    }
    struct inflate_state FAR *state = (struct inflate_state FAR *)strm->state;
    state->window = NULL;
    state->wbits = (unsigned)wbits;
    state->wsize = 0;
    state->wnext = 0;
    state->whave = 0;

    // Prepare end buffer; content is irrelevant since allocation should fail before copy
    const unsigned int WSIZE = 1U << wbits;
    unsigned char endBuf[WSIZE];
    for (unsigned i = 0; i < WSIZE; ++i) endBuf[i] = (unsigned char)i;
    unsigned char* endPtr = endBuf + WSIZE; // end - wsize points to endBuf[0]

    int ret = updatewindow(strm, endPtr, WSIZE);
    bool passed = (ret == 1);

    destroy_test_stream(strm);
    return passed;
}

// Test 4: Initialization path where wsize == 0 and window is allocated (ensures initialization)
static bool test_updatewindow_initialization_path() {
    // Create a new stream with a small wbits
    int wbits = 3; // wsize = 8
    z_streamp strm = create_test_stream(wbits);
    if (!strm) {
        std::cerr << "Test4: Failed to create test stream\n";
        return false;
    }
    struct inflate_state FAR *state = (struct inflate_state FAR *)strm->state;

    // Ensure wsize is zero so that the initialization branch runs
    state->window = NULL;
    state->wsize = 0;
    state->wnext = 0;
    state->whave = 0;

    // Prepare end buffer to trigger a copy path (full window would also initialize)
    const unsigned int WSIZE = 1U << wbits; // 8
    unsigned char endBuf[WSIZE * 2];
    for (unsigned i = 0; i < WSIZE; ++i) endBuf[i] = (unsigned char)(i + 10);
    unsigned char* endPtr = endBuf + WSIZE;

    int ret = updatewindow(strm, endPtr, WSIZE);
    bool ok = (ret == 0);

    if (!ok) {
        std::cerr << "Test4: updatewindow initialization path returned non-zero\n";
        destroy_test_stream(strm);
        return false;
    }

    // Validate that window is allocated and wsize is set
    if (state->window == NULL) {
        std::cerr << "Test4: window was not allocated during initialization\n";
        destroy_test_stream(strm);
        return false;
    }
    if (state->wsize != WSIZE) {
        std::cerr << "Test4: wsize should be " << WSIZE << ", got " << state->wsize << "\n";
        destroy_test_stream(strm);
        return false;
    }

    destroy_test_stream(strm);
    return true;
}

int main() {
    int total = 0;
    int passed = 0;

    std::cout << "Starting updatewindow unit tests (C++11 harness)\n";

    if (test_updatewindow_full_copy()) {
        ++passed;
        std::cout << "[PASS] Test1: Full window copy when copy >= wsize\n";
    } else {
        std::cout << "[FAIL] Test1: Full window copy when copy >= wsize\n";
    }
    ++total;

    if (test_updatewindow_partial_copy_with_wrap()) {
        ++passed;
        std::cout << "[PASS] Test2: Partial copy with wrap-around (copy < wsize)\n";
    } else {
        std::cout << "[FAIL] Test2: Partial copy with wrap-around (copy < wsize)\n";
    }
    ++total;

    if (test_updatewindow_allocation_failure()) {
        ++passed;
        std::cout << "[PASS] Test3: Allocation failure path (ZALLOC returns NULL)\n";
    } else {
        std::cout << "[FAIL] Test3: Allocation failure path (ZALLOC returns NULL)\n";
    }
    ++total;

    if (test_updatewindow_initialization_path()) {
        ++passed;
        std::cout << "[PASS] Test4: Initialization path when wsize == 0\n";
    } else {
        std::cout << "[FAIL] Test4: Initialization path when wsize == 0\n";
    }
    ++total;

    std::cout << "updatewindow tests: " << passed << " / " << total << " passed.\n";

    return (passed == total) ? 0 : 1;
}