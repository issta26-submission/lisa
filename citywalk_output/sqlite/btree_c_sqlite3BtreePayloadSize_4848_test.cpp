// Unit test suite for sqlite3BtreePayloadSize
// This test targets the focal method described in the prompt and relies on
// the real sqlite3 B-Tree implementation present in the build (btree.c) along
// with its public-dependent header (btreeInt.h).
// The test uses a real BtCursor type provided by btreeInt.h and the helper
// sqlite3BtreeFakeValidCursor() to obtain a cursor suitable for testing.
// It also exercises the assertion path by forking a child process to isolate
// assertion aborts (since sqlite3BtreePayloadSize uses asserts to enforce
// preconditions).

#include <cstdio>
#include <sys/wait.h>
#include <btreeInt.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// Include the real BtCursor type and related API from the sqlite3 codebase.
// This header is expected to provide:
 // - typedef struct BtCursor BtCursor;
 // - a member: int eState;
 // - a nested member: struct { uint32_t nPayload; } info;
 // - macro CURSOR_VALID
 // - function: extern "C" u32 sqlite3BtreePayloadSize(BtCursor *pCur);
 // - function: BtCursor *sqlite3BtreeFakeValidCursor(void);

extern "C" {
    // Declaration of the focal method under test (from btree.c)
    // Provided by the actual library; we declare in C linkage to ensure correct binding.
    typedef uint32_t u32;
    u32 sqlite3BtreePayloadSize(BtCursor *pCur);

    // Helper to obtain a valid test cursor; assumed to be available in the build.
    BtCursor *sqlite3BtreeFakeValidCursor(void);
}

// Simple, descriptive test harness
static void test_normal_payload_size()
{
    // This test exercises the "happy path" where:
    // - The cursor is in a valid state
    // - The internal getCellInfo() populates pCur->info.nPayload
    // - sqlite3BtreePayloadSize returns the same value as pCur->info.nPayload

    BtCursor *p = sqlite3BtreeFakeValidCursor();
    if (p == nullptr) {
        std::cerr << "test_normal_payload_size: failed to obtain a fake valid cursor\n";
        std::exit(1);
    }

    // Ensure the precondition is satisfied for the test:
    // The code asserts that the cursor is in the CURSOR_VALID state.
    p->eState = CURSOR_VALID;

    // Call the function under test
    u32 payloadSize = sqlite3BtreePayloadSize(p);

    // Validate internal consistency: the function returns the value from pCur->info.nPayload
    // If getCellInfo() populated that field, payloadSize should match it.
    if (payloadSize != p->info.nPayload) {
        std::cerr << "test_normal_payload_size: mismatch - "
                  << "returned payloadSize=" << payloadSize
                  << ", p->info.nPayload=" << p->info.nPayload << "\n";
        std::exit(2);
    } else {
        std::cout << "test_normal_payload_size: passed (payloadSize=" << payloadSize << ")\n";
    }
}

// Test that the precondition assertions correctly abort the process when violated.
// We isolate this in a forked child so that an assertion abort does not abort the
// entire test suite.
static void test_assert_aborts_on_invalid_state()
{
    pid_t pid = fork();
    if (pid < 0) {
        std::perror("fork failed");
        std::exit(1);
    }

    if (pid == 0) {
        // Child process: intentionally violate the precondition by corrupting eState
        BtCursor *p = sqlite3BtreeFakeValidCursor();
        if (p == nullptr) {
            // If the helper returns null, treat as fatal for this test
            std::_Exit(1);
        }

        // Invalidate the cursor state to trigger the second assert:
        // pCur->eState == CURSOR_VALID
        p->eState = 0; // invalid state

        // This should trigger an assertion; if it returns, something went wrong.
        sqlite3BtreePayloadSize(p);

        // If we reach here, the assertion did not abort as expected.
        _exit(0);
    } else {
        int status = 0;
        if (waitpid(pid, &status, 0) == -1) {
            std::perror("waitpid failed");
            std::exit(1);
        }

        // We expect the child to be aborted due to an assertion failure (SIGABRT)
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGABRT) {
            std::cout << "test_assert_aborts_on_invalid_state: pass (aborted as expected)\n";
        } else if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            std::cerr << "test_assert_aborts_on_invalid_state: fail (child exited normally; abort expected)\n";
            std::exit(1);
        } else {
            std::cerr << "test_assert_aborts_on_invalid_state: fail (unexpected child status)\n";
            std::exit(1);
        }
    }
}

// Entry point for the test suite
int main()
{
    // Run normal-path test
    test_normal_payload_size();

    // Run assertion-path test (via fork isolation)
    test_assert_aborts_on_invalid_state();

    std::cout << "All tests completed.\n";
    return 0;
}