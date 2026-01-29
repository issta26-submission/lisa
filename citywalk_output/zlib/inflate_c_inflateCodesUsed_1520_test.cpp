// Unit test suite for the focal method inflateCodesUsed (C++11, no GoogleTest)
// This test suite is self-contained and mocks the minimal dependencies
// required by the inflateCodesUsed implementation described in the prompt.
// It exercises true/false branches and the main computation of pointer difference.

#include <cstdint>
#include <stdio.h>
#include <iostream>
#include <inftrees.h>
#include <inffast.h>
#include <zutil.h>
#include <inflate.h>
#include <cstdlib>


// ------------------------------------------------------------
// Minimal mock/types to simulate the focused code path
// ------------------------------------------------------------

// Simulated typedefs/macro setup to resemble the original C code environment
typedef void* z_streamp;
#define Z_NULL nullptr

typedef void* alloc_func;
typedef void* free_func;

// Forward declaration of needed structures
struct inflate_state;

// The focal function expects a z_streamp that points to a state of type inflate_state
struct z_stream {
    void* state;
    void* zalloc;
    void* zfree;
};
typedef z_stream* z_streamp;

// Local (static) constants to simulate zlib inflate state range
#define HEAD 0
#define SYNC 6

// The inflate_state structure as used by the focal code
struct inflate_state {
    void* strm;      // back-pointer to the z_stream
    int mode;        // current decoding mode
    unsigned char* next;
    unsigned char* codes;
};

// Helper to emulate FAR in the original code (no-op in this test)
#define FAR

// ------------------------------------------------------------
// Minimal implementation of the focal logic for testing
// (Inflate state check and the inflateCodesUsed function)
// ------------------------------------------------------------

// Local function to check the validity of the inflate state
static int inflateStateCheck(z_streamp strm) {
    struct inflate_state* state;
    if (strm == Z_NULL ||
        strm->zalloc == (alloc_func)0 || strm->zfree == (free_func)0)
        return 1;
    state = (struct inflate_state*)strm->state;
    if (state == Z_NULL || state->strm != strm ||
        state->mode < HEAD || state->mode > SYNC)
        return 1;
    return 0;
}

// The focal method under test
static unsigned long inflateCodesUsed(z_streamp strm) {
    struct inflate_state FAR *state;
    if (inflateStateCheck(strm)) return (unsigned long)-1;
    state = (struct inflate_state FAR *)strm->state;
    return (unsigned long)(state->next - state->codes);
}

// ------------------------------------------------------------
// Test harness and cases
// ------------------------------------------------------------

// Simple non-terminating assertion macros (print on fail, continue execution)
static int g_failures = 0;

#define EXPECT_EQ(expected, actual, msg) do { \
    if ((unsigned long)(expected) != (unsigned long)(actual)) { \
        std::cerr << "EXPECT_EQ FAILED: " << msg \
                  << " | expected: " << (unsigned long)(expected) \
                  << ", actual: " << (unsigned long)(actual) << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE FAILED: " << msg << std::endl; \
        ++g_failures; \
    } \
} while(0)

int main() {
    // Test 1: Null strm should return (unsigned long)-1
    // This triggers the early invalid-state path in inflateCodesUsed
    {
        unsigned long res = inflateCodesUsed(nullptr);
        EXPECT_EQ((unsigned long)-1, res, "inflateCodesUsed(nullptr) should return -1");
    }

    // Test 2: Non-null strm but missing allocator callback should return -1
    // Simulates zalloc == 0 (null) path in inflateStateCheck
    {
        z_stream s;
        s.state = nullptr;   // state will be checked only after zalloc/zfree validation
        s.zalloc = (void*)0;  // invalid allocator
        s.zfree = (void*)1;   // non-null
        unsigned long res = inflateCodesUsed(&s);
        EXPECT_EQ((unsigned long)-1, res, "inflateCodesUsed with zalloc==0 should return -1");
    }

    // Test 3: Valid zstr stream but state->strm != strm should fail the state check
    {
        // Prepare a buffer to serve as codes/next space
        unsigned char buffer[32];
        // Create two distinct z_stream instances to decouple state->strm
        z_stream s;
        z_stream other;
        s.state = &(inflate_state){0}; // temporary placeholder; will set properly below
        s.zalloc = (void*)1;
        s.zfree = (void*)1;

        other.state = &(inflate_state){0};
        other.zalloc = (void*)1;
        other.zfree = (void*)1;

        // Properly configure the state object
        inflate_state st;
        st.strm = &s;            // pointer back to the actual strm
        st.mode = HEAD;            // in-range
        st.next = buffer;          // arbitrary buffer
        st.codes = buffer;         // next - codes would be 0, but state->strm mismatch triggers fail

        // Link the state to strm
        s.state = &st;

        // Force a mismatch: state->strm != strm
        st.strm = &other; // different strm, should fail
        // Now run the function
        unsigned long res = inflateCodesUsed(&s);
        EXPECT_EQ((unsigned long)-1, res, "inflateCodesUsed should fail when state->strm != strm");
    }

    // Test 4: Valid strm/state but mode out of range should fail the state check
    {
        unsigned char buffer[32];
        z_stream s;
        inflate_state st;
        s.zalloc = (void*)1;
        s.zfree  = (void*)1;
        s.state = &st;
        st.strm = &s;
        st.next = buffer;
        st.codes = buffer;
        st.mode = HEAD - 1; // out of valid range (< HEAD)
        unsigned long res = inflateCodesUsed(&s);
        EXPECT_EQ((unsigned long)-1, res, "inflateCodesUsed should fail when state->mode is out of range");
    }

    // Test 5: Valid case, ensure correct pointer difference is returned
    {
        unsigned char buffer[64];
        z_stream s;
        inflate_state st;

        s.zalloc = (void*)1;
        s.zfree  = (void*)1;
        s.state = &st;

        st.strm = &s;
        st.mode = HEAD; // in-range
        st.codes = buffer;
        st.next  = buffer + 10; // difference = 10

        unsigned long res = inflateCodesUsed(&s);
        EXPECT_EQ(10u, res, "inflateCodesUsed should return correct difference (next - codes)");
    }

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::cout << g_failures << " test(s) failed." << std::endl;
    }

    return g_failures;
}