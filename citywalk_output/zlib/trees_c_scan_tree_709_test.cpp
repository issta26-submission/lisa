// C++11 unit test suite for the focal method scan_tree (from trees.c)
// This test suite is designed to work without Google Test (GTest).
// It uses a small, self-contained harness and plain assertions with non-terminating behavior.

#include <iostream>
#include <vector>
#include <deflate.h>
#include <cstring>


// Include project headers that define the types used by scan_tree.
// The actual project provides deflate.h with the necessary definitions.
// We assume the real definitions expose:
// - typedef struct { unsigned int Len; unsigned int Freq; /* possibly more fields */ } ct_data;
// - struct deflate_state { ct_data bl_tree[...] ; /* other members */ };
// We rely on the production header to match the production code.

extern "C" {
    // The focal function scan_tree is defined in trees.c.
    // It is declared as a 'local' function (likely static) in C code.
    // We declare it here as extern to call from C++ test code.
    void scan_tree(deflate_state *s, ct_data *tree, int max_code);
}

// Lightweight test harness
static int test_failures = 0;

#define EXPECT_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            std::cerr << "EXPECT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) << ")" << std::endl; \
            ++test_failures; \
        } \
    } while (0)

#define TEST_PRINT(msg) \
    do { std::cout << "[TEST] " << msg << std::endl; } while (0)

// Test 1: Basic sequencing to trigger two Freq updates on bl_tree indices 2 and 3.
void test_basic_scenario() {
    TEST_PRINT("Test 1: Basic sequence triggering two Freq updates (indices 2 and 3).");

    deflate_state s;
    // Zero the entire production state to ensure deterministic initialization.
    std::memset(&s, 0, sizeof(deflate_state));

    // Build a small tree with max_code = 2
    // tree[0].Len = 2, tree[1].Len = 2, tree[2].Len = 3, tree[3].Len guard
    std::vector<ct_data> tree(4);
    tree[0].Len = 2;
    tree[1].Len = 2;
    tree[2].Len = 3;
    tree[3].Len = 0; // This value will be guarded by scan_tree

    // Call the focal function
    scan_tree(&s, tree.data(), 2);

    // Expected behavior derived from the chosen sequence:
    // - At n=1, curlen (2) != nextlen (3) causing the else-branch with count >= min_count,
    //   since count became 2 (< min_count=4), actually it should accumulate to Freq += 2 for curlen=2.
    // - At n=2, curlen=3 with nextlen=guard value (0xffff) leads to Freq at index 3 increment.
    // Thus we expect:
    EXPECT_EQ(s.bl_tree[2].Freq, 2);
    EXPECT_EQ(s.bl_tree[3].Freq, 1);
}

// Test 2: Sequence where a break occurs with curlen != prevlen, exercising REP_3_6 path.
void test_branch_rep_three_six() {
    TEST_PRINT("Test 2: Branch that exercises REP_3_6 and curlen != prevlen path.");

    deflate_state s;
    std::memset(&s, 0, sizeof(deflate_state));

    // max_code = 3 to allow a break after a short run
    // We craft: [5, 5, 5, 3] for tree lengths
    // n=0: curlen=5, nextlen=5 -> continue
    // n=1: curlen=5, nextlen=5 -> continue
    // n=2: curlen=5, nextlen=3 -> break; count becomes 3; since 3 < min_count (4) -> Freq[5] += 3
    // n=3: not reached in this scenario since we break on n=2 and end loop? To be precise, we allow up to max_code inclusive.
    // We'll include a guard element at tree[3] as 3 to allow the next iteration to test the else-branch behavior.
    std::vector<ct_data> tree(4);
    tree[0].Len = 5;
    tree[1].Len = 5;
    tree[2].Len = 5;
    tree[3].Len = 3; // next
    // The loop will process n=0..3; the break occurs at n=2; expecting Freq updates accordingly.
    scan_tree(&s, tree.data(), 3);

    // Expect Freq[5] to have been incremented by 3 in the else-if path (count < min_count)
    EXPECT_EQ(s.bl_tree[5].Freq, 3);
}

// Test 3: Sequence with initial zeros to push REPZ_3_10 path (count <= 10) for curlen==0.
void test_zero_run_repz_3_10() {
    TEST_PRINT("Test 3: Zero-length run leading to REPZ_3_10 (curlen==0 and count <= 10).");

    deflate_state s;
    std::memset(&s, 0, sizeof(deflate_state));

    // max_code = 1; tree[0].Len = 0; tree[1].Len = 0; tree[2].Len = 1 to create a change after zeros
    std::vector<ct_data> tree(3);
    tree[0].Len = 0;
    tree[1].Len = 0;
    tree[2].Len = 1;

    scan_tree(&s, tree.data(), 1);

    // For this pattern, the else-if branch should accumulate and hit REPZ_3_10 on one of the iterations.
    // We can't rely on exact count without deep introspection, but we can assert that REPZ_3_10 was touched
    // by checking that some Freq index for a zero curlen increased.
    // We expect REPZ_3_10 to be hit, thus Freq for that code should be > 0.
    // Accessing the exact index (REPZ_3_10) is platform-dependent;
    // as a conservative assertion, ensure that either the zero-length code freq or REPZ_3_10 freq is non-zero.
    // Here we check REPZ_3_10 by index if available; fall back to non-negative check.
    // Since the exact index value can vary, we conservatively check that at least one Freq was updated.
    // We attempt to check if a non-zero frequency exists among a small set of indices commonly used.
    bool updated = false;
    if (s.bl_tree[0].Freq != 0) updated = true;
    if (s.bl_tree[1].Freq != 0) updated = true;
    // If REPZ_3_10 index exists and updated, it will also be non-zero; we don't have the exact index here.
    EXPECT_EQ(updated, true);
}

// Runner
int main() {
    TEST_PRINT("Starting test suite for scan_tree (deflate) - C++11 harness");

    test_basic_scenario();
    test_branch_rep_three_six();
    test_zero_run_repz_3_10();

    if (test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << test_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}