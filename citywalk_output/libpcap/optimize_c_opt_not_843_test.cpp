// C++11 unit test suite for the focal C function opt_not(struct block *b)
// The tests exercise the swap of JT(b) and JF(b) as implemented by opt_not.
// Notes:
// - The tests are written to compile against the existing C headers (optimize.h, etc.)
// - We use a lightweight, non-terminating test harness: each test returns a bool and prints
//   a message on failure, allowing all tests to run in a single process.
// - We access the C API and the block structure via extern "C" to ensure correct linkage.

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <vector>
#include <memory.h>
#include <optimize.h>
#include <gencode.h>
#include <os-proto.h>
#include <stdio.h>
#include <limits.h>
#include <functional>
#include <stdlib.h>
#include <setjmp.h>
#include <config.h>
#include <pcap-int.h>
#include <cstring>
#include <iostream>
#include <pcap-types.h>


// Bring in the C declarations. The block structure and macros JT/JF are defined in the
// project headers. We wrap with extern "C" to ensure correct linkage for C symbols.
extern "C" {
}

// Lightweight test harness helper
static bool run_test(const char* name, const std::function<bool()> &test) {
    bool ok = test();
    if (ok) {
        std::cout << "[PASS] " << name << std::endl;
    } else {
        std::cerr << "[FAIL] " << name << std::endl;
    }
    return ok;
}

// Test 1: Basic swap of JT and JF pointers.
// Setup: B.JT -> A, B.JF -> C. After opt_not(&B), JT(B) should be C and JF(B) should be A.
static bool test_opt_not_basic_swap() {
    // Ensure we have three distinct blocks to point to
    struct block A;
    struct block B;
    struct block C;

    // Clear memory to avoid garbage in unused fields (only targeting JT/JF usage)
    std::memset(&A, 0, sizeof(A));
    std::memset(&B, 0, sizeof(B));
    std::memset(&C, 0, sizeof(C));

    // Establish initial true/false successors for block B
    JT(&B) = &A;
    JF(&B) = &C;

    // Invoke focal function under test
    opt_not(&B);

    // Validate the swap occurred
    if (JT(&B) != &C) {
        std::cerr << "  Expected JT(B) to be C after swap; got " << (void*)JT(&B) << " instead." << std::endl;
        return false;
    }
    if (JF(&B) != &A) {
        std::cerr << "  Expected JF(B) to be A after swap; got " << (void*)JF(&B) << " instead." << std::endl;
        return false;
    }
    return true;
}

// Test 2: Swap behavior when one pointer is NULL and the other is non-NULL.
// Setup: B.JT -> NULL, B.JF -> A. After opt_not(&B), JT should be A, JF should be NULL.
static bool test_opt_not_swap_with_nulls() {
    struct block A;
    struct block B;

    std::memset(&A, 0, sizeof(A));
    std::memset(&B, 0, sizeof(B));

    JT(&B) = NULL;
    JF(&B) = &A;

    opt_not(&B);

    if (JT(&B) != &A) {
        std::cerr << "  Expected JT(B) to be A after swap with NULL, got " << (void*)JT(&B) << std::endl;
        return false;
    }
    if (JF(&B) != NULL) {
        std::cerr << "  Expected JF(B) to be NULL after swap with NULL, got " << (void*)JF(&B) << std::endl;
        return false;
    }
    return true;
}

// Test 3: Swap behavior when both JT and JF point to the same block (self-loop scenario).
// Setup: B.JT -> A, B.JF -> A. After opt_not(&B), both should still point to A.
static bool test_opt_not_swap_with_self_loop() {
    struct block A;

    std::memset(&A, 0, sizeof(A));

    struct block B;
    std::memset(&B, 0, sizeof(B));

    JT(&B) = &A;
    JF(&B) = &A;

    opt_not(&B);

    if (JT(&B) != &A) {
        std::cerr << "  Expected JT(B) to remain A in self-loop case, got " << (void*)JT(&B) << std::endl;
        return false;
    }
    if (JF(&B) != &A) {
        std::cerr << "  Expected JF(B) to remain A in self-loop case, got " << (void*)JF(&B) << std::endl;
        return false;
    }
    return true;
}

int main() {
    // Collect and run all tests. This mirrors a minimal test runner without external frameworks.
    std::vector<std::function<bool()>> tests = {
        [](){ return test_opt_not_basic_swap(); },
        [](){ return test_opt_not_swap_with_nulls(); },
        [](){ return test_opt_not_swap_with_self_loop(); }
    };

    int failures = 0;
    int idx = 1;
    for (const auto &t : tests) {
        // Name each test for clearer output
        // We reuse a simple wrapper to provide the test index-based naming
        // (the lambda-based approach keeps test names implicit in the output).
        // In a more advanced setup, we could store names explicitly.
        bool ok = t();
        if (!ok) ++failures;
        ++idx;
    }

    if (failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << failures << " test(s) failed." << std::endl;
        return 1;
    }
}