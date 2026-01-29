/*
 * Unit test suite for F(opt_state_t *opt_state, int code, bpf_u_int32 v0, bpf_u_int32 v1)
 * - Targeted for C++11 compilation (no GoogleTest; a small custom test harness is used)
 * - Covers: 
 *     - True branch: existing matching entry is returned without modification
 *     - False branch: new entry is allocated, and (when applicable) vmap is updated
 * - Uses the actual optimize.h / optimize.c definitions from the project
 * - Static members within optimize.* are accessed through the provided API/structs
 * - Tests are executed from main() to satisfy non-GTest requirement
 *
 * Note: This test assumes the environment provides the necessary pcap/BPF related
 * macros (e.g., MODULUS, BPF_IMM, BPF_LD, BPF_LDX, etc.) via optimize.h.
 * The code compiles against optimize.c with C linkage.
 */

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <memory.h>
#include <optimize.h>
#include <cstdlib>
#include <gencode.h>
#include <os-proto.h>
#include <stdio.h>
#include <limits.h>
#include <cstdio>
#include <stdlib.h>
#include <setjmp.h>
#include <config.h>
#include <pcap-int.h>
#include <cstring>
#include <pcap-types.h>


extern "C" {
}

// Prototypes for C functions from the target module
extern "C" unsigned int F(opt_state_t *opt_state, int code, bpf_u_int32 v0, bpf_u_int32 v1);

// Helper: compute the hash used by F (matches internal F hash calculation)
static inline unsigned int compute_hash(int code, bpf_u_int32 v0, bpf_u_int32 v1) {
    unsigned int h = (unsigned int)code ^ (v0 << 4) ^ (v1 << 8);
    return h % MODULUS;
}

// Simple test harness: track failures without aborting on first failure
static int total_failures = 0;
static void log_failure(const char* test_name, const char* message) {
    ++total_failures;
    std::fprintf(stderr, "TEST FAILURE - %s: %s\n", test_name, message);
}

// Test 1: When an existing matching entry is present in the hash bucket, F should return its value
bool test_F_returns_existing_value_when_found() {
    // Prepare a clean opt_state
    opt_state_t *state = (opt_state_t*) std::calloc(1, sizeof(opt_state_t));
    if (!state) {
        log_failure("test_F_returns_existing_value_when_found", "memory allocation failed for opt_state_t");
        return false;
    }

    // Prepare a pre-existing node and place it in the proper hash bucket
    static struct valnode existing;
    int code = 0x123;            // arbitrary code
    bpf_u_int32 v0 = 7;
    bpf_u_int32 v1 = 9;
    unsigned int hash = compute_hash(code, v0, v1);

    existing.code = code;
    existing.v0 = v0;
    existing.v1 = v1;
    existing.val = 0xA5A5; // existing value to be returned
    existing.next = NULL;

    // Inject into the bucket
    state->hashtbl[hash] = &existing;

    // Call F with the same (code, v0, v1)
    unsigned int ret = F(state, code, v0, v1);

    // Validate: must return existing.val
    if (ret != existing.val) {
        char msg[128];
        std::snprintf(msg, sizeof(msg), "expected %u, got %u", existing.val, ret);
        log_failure("test_F_returns_existing_value_when_found", msg);
        std::free(state);
        return false;
    }

    std::free(state);
    return true;
}

// Test 2: When no matching entry exists, F should allocate a new value, and (if condition true) set vmap entry
bool test_F_creates_new_entry_and_updates_vmap() {
    // Prepare state
    opt_state_t *state = (opt_state_t*) std::calloc(1, sizeof(opt_state_t));
    if (!state) {
        log_failure("test_F_creates_new_entry_and_updates_vmap", "memory allocation failed for opt_state_t");
        return false;
    }

    // Pool of valnode to be used for new allocations
    static struct valnode pool[4];
    std::memset(pool, 0, sizeof(pool));
    state->next_vnode = pool;

    // Ensure the bucket is empty
    int code = 0;                 // chosen to likely trigger IMM+LD/LDX branch in F
    bpf_u_int32 v0 = 5;
    bpf_u_int32 v1 = 9;
    unsigned int hash = compute_hash(code, v0, v1);
    state->hashtbl[hash] = NULL;

    // Call F (no existing entry)
    unsigned int ret = F(state, code, v0, v1);

    // Validate: a new value should be created and returned (val should be 1 on first insertion)
    if (ret != 1) {
        char msg[128];
        std::snprintf(msg, sizeof(msg), "expected new val 1, got %u", ret);
        log_failure("test_F_creates_new_entry_and_updates_vmap", msg);
        std::free(state);
        return false;
    }

    // Validate vmap update when IMM+LD/LDX condition is true
    // The first newly created value index is 1
    if (!(state->vmap[1].is_const == 1 && state->vmap[1].const_val == v0)) {
        log_failure("test_F_creates_new_entry_and_updates_vmap",
                    "vmap[1] was not updated with const_val = v0 and is_const = 1");
        std::free(state);
        return false;
    }

    // Validate that the new node in pool contains the expected fields
    if (!(pool[0].val == 1 && pool[0].code == code && pool[0].v0 == v0 && pool[0].v1 == v1)) {
        log_failure("test_F_creates_new_entry_and_updates_vmap",
                    "new vnode in pool does not contain correct fields");
        std::free(state);
        return false;
    }

    std::free(state);
    return true;
}

int main() {
    int tests_run = 0;
    int tests_passed = 0;

    // Run Test 1
    bool t1 = test_F_returns_existing_value_when_found();
    tests_run++;
    if (t1) {
        std::puts("TEST 1 PASSED: F returns existing value when found");
        tests_passed++;
    } else {
        std::puts("TEST 1 FAILED: F did not return the expected existing value");
    }

    // Run Test 2
    bool t2 = test_F_creates_new_entry_and_updates_vmap();
    tests_run++;
    if (t2) {
        std::puts("TEST 2 PASSED: F creates new entry and updates vmap correctly");
        tests_passed++;
    } else {
        std::puts("TEST 2 FAILED: F did not create new entry or update vmap as expected");
    }

    // Summary
    std::printf("SUMMARY: %d/%d tests passed, %d failures total\n",
                tests_passed, tests_run, total_failures);

    return (total_failures == 0) ? 0 : 1;
}