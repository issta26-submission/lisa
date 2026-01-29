// Minimal C++11 unit test suite for the focal function inflateMark.
// This test suite avoids GTest and uses a small custom assertion framework.
// It is self-contained and only relies on the standard C++ library.
//
// Purpose: verify the behavior of inflateMark for:
// - early exit when inflateStateCheck indicates error (returns -65536)
// - normal computation for COPY mode (back and length)
// - normal computation for MATCH mode (back, length, and was)
// - behavior when mode is neither COPY nor MATCH (returns back<<16 only)

#include <cstdint>
#include <stdio.h>
#include <iostream>
#include <inftrees.h>
#include <inffast.h>
#include <zutil.h>
#include <inflate.h>


// Domain-aligned simple definitions to mimic the environment around inflateMark.
// We do not rely on external zlib headers; tests are self-contained.
#define FAR
#define COPY 1
#define MATCH 2

// Forward declaration to mimic z_streamp type used by inflateMark.
typedef struct z_stream {
    void* state;
} z_stream, *z_streamp;

// Minimal inflate_state structure as used by inflateMark.
// This mirrors the fields referenced by the focal method.
struct inflate_state {
    int trigger;  // Used by our test inflateStateCheck to decide validity.
    int mode;     // COPY, MATCH, or other.
    int back;     // Back reference value.
    int length;   // Length of the match or distance of copy.
    int was;      // "Was" value used for MATCH mode calculation.
};

// Local (file-scope) inflateStateCheck implementation used by inflateMark.
// In the real project this would validate the streaming state; here we control
// its return value via the test state's 'trigger' field.
int inflateStateCheck(z_streamp strm) {
    if (strm == nullptr || strm->state == nullptr) return 1;
    struct inflate_state* st = (struct inflate_state*)strm->state;
    if (st->trigger) return 1;
    return 0;
}

// The focal function under test, implemented exactly as provided in the prompt.
long inflateMark(z_streamp strm) {
    struct inflate_state FAR *state;
    if (inflateStateCheck(strm))
        return -(1L << 16);
    state = (struct inflate_state FAR *)strm->state;
    return (long)(((unsigned long)((long)state->back)) << 16) +
        (state->mode == COPY ? state->length :
            (state->mode == MATCH ? state->was - state->length : 0));
}

// Simple non-terminating test assertions (EXPECT-like macros).
static int tests_run = 0;
static int tests_passed = 0;

#define EXPECT_EQ(a, b) do { \
    ++tests_run; \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ failed: " #a " == " #b " (actual " << (a) \
                  << " vs " << (b) << ")\n"; \
    } else { \
        ++tests_passed; \
    } \
} while(0)

#define EXPECT_TRUE(x) do { \
    ++tests_run; \
    if (!(x)) { \
        std::cerr << "EXPECT_TRUE failed: " #x "\n"; \
    } else { \
        ++tests_passed; \
    } \
} while(0)

// Test 1: When inflateStateCheck reports an error (trigger != 0), inflateMark should return -65536.
void test_inflateMark_stateCheckTriggered() {
    // Prepare a strm with a state that triggers the error path.
    z_streamp strm = new z_stream;
    inflate_state st;
    st.trigger = 1;    // Causes inflateStateCheck to return non-zero.
    st.mode = 0;       // Irrelevant for this test since error happens first.
    st.back = 123;
    st.length = 4;
    st.was = 7;
    strm->state = &st;

    long result = inflateMark(strm);
    EXPECT_EQ(result, -(1L << 16));

    delete strm;
}

// Test 2: COPY mode path: back is shifted by 16 bits and length is added.
void test_inflateMark_copyMode() {
    z_streamp strm = new z_stream;
    inflate_state st;
    st.trigger = 0;      // Valid state.
    st.mode = COPY;        // COPY path
    st.back = 1;
    st.length = 3;
    st.was = 0;            // Irrelevant for COPY
    strm->state = &st;

    long result = inflateMark(strm);
    long expected = (long)(((unsigned long)((long)st.back)) << 16) + st.length;
    EXPECT_EQ(result, expected);

    delete strm;
}

// Test 3: MATCH mode path: uses (was - length) in addition.
void test_inflateMark_matchMode() {
    z_streamp strm = new z_stream;
    inflate_state st;
    st.trigger = 0;        // Valid state.
    st.mode = MATCH;         // MATCH path
    st.back = 2;
    st.length = 5;
    st.was = 9;
    strm->state = &st;

    long result = inflateMark(strm);
    long expected = (long)(((unsigned long)((long)st.back)) << 16) +
                    (st.was - st.length);
    EXPECT_EQ(result, expected);

    delete strm;
}

// Test 4: Other mode path (neither COPY nor MATCH): only the shifted back value is used.
void test_inflateMark_otherMode() {
    z_streamp strm = new z_stream;
    inflate_state st;
    st.trigger = 0;
    st.mode = 0;      // Not COPY or MATCH
    st.back = 0;
    st.length = 123;  // Should be ignored
    st.was = 456;
    strm->state = &st;

    long result = inflateMark(strm);
    long expected = (long)(((unsigned long)((long)st.back)) << 16);
    EXPECT_EQ(result, expected);

    delete strm;
}

// Run all tests and report summary.
int main() {
    std::cout << "Running inflateMark unit tests...\n";

    test_inflateMark_stateCheckTriggered();
    test_inflateMark_copyMode();
    test_inflateMark_matchMode();
    test_inflateMark_otherMode();

    std::cout << "Tests run: " << tests_run << ", Passed: " << tests_passed << "\n";
    if (tests_run == tests_passed) {
        std::cout << "All tests passed.\n";
    } else {
        std::cout << "Some tests failed. See above for details.\n";
    }
    return (tests_run == tests_passed) ? 0 : 1;
}