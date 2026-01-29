// Test suite for gzofstream::attach(int fd, std::ios_base::openmode mode)
// FOCAL_METHOD under test:
// gzofstream::attach(int fd,
//                    std::ios_base::openmode mode)
// {
// {
//   if (!sb.attach(fd, mode | std::ios_base::out))
//     this->setstate(std::ios_base::failbit);
//   else
//     this->clear();
// }
// }

// This test focuses on:
// - Branch where sb.attach(...) returns false -> gzofstream sets failbit (test failsafe path).
// - Branch where sb.attach(...) returns true  -> gzofstream clears state (success path).
// We simulate a failing fd (-1) to trigger the false branch, and a valid pipe fd to trigger the true branch.

#include <cstring>
#include <cstdio>
#include <cerrno>
#include <iostream>
#include <zfstream.h>
#include <unistd.h>


// Lightweight test harness (non-terminating) to enable multiple tests to run.
// It prints results and accumulates a failure count.
namespace TestHarness {
    int g_failure_count = 0;

    // Non-terminating expectation: record failure but continue.
    #define EXPECT_TRUE(cond, msg) do { \
        if (!(cond)) { \
            ++TestHarness::g_failure_count; \
            std::cerr << "EXPECT_TRUE FAILED: " << (msg) \
                      << "  [Condition: " << #cond << "]" << std::endl; \
        } \
    } while (0)

    #define EXPECT_FALSE(cond, msg) do { \
        if ((cond)) { \
            ++TestHarness::g_failure_count; \
            std::cerr << "EXPECT_FALSE FAILED: " << (msg) \
                      << "  [Condition: " << #cond << "]" << std::endl; \
        } \
    } while (0)

    // Convenience to report summary
    void print_summary(const std::string& title) {
        if (g_failure_count == 0) {
            std::cout << "[PASS] " << title << std::endl;
        } else {
            std::cout << "[FAIL] " << title << " - Failures: " << g_failure_count << std::endl;
        }
    }

    // Test 1: Attach with invalid file descriptor should set failbit.
    void test_attach_failure_invalid_fd() {
        // Explanation:
        // Pass an invalid fd (-1). gzofstream::attach should call sb.attach with mode|out.
        // sb.attach is expected to fail, leading to this->setstate(ios_base::failbit)
        // and thus stream::fail() should be true.
        gzofstream ofs;
        int invalid_fd = -1;
        ofs.attach(invalid_fd, std::ios_base::out);

        // Assertions:
        // - The stream should be in a failed state.
        EXPECT_TRUE(ofs.fail(), "gzofstream should set failbit when attach invoked with invalid fd");
        // - The stream should not be considered good.
        EXPECT_FALSE(ofs.good(), "gzofstream should not be good after failed attach");
    }

    // Test 2: Attach with a valid fd (constructed via pipe) should succeed and clear state.
    void test_attach_success_valid_fd() {
        // Explanation:
        // Create a pipe and use the write end as a valid file descriptor.
        // If sb.attach succeeds, gzofstream should clear its state (be good).
        int pipefd[2];
        if (pipe(pipefd) != 0) {
            perror("pipe failed in test_attach_success_valid_fd");
            ++g_failure_count;
            return;
        }

        int write_fd = pipefd[1];
        gzofstream ofs;
        ofs.attach(write_fd, std::ios_base::out);

        // Assertions:
        EXPECT_TRUE(ofs.good(), "gzofstream should be good after attach with a valid fd");
        // Cleanup
        close(pipefd[0]);
        close(pipefd[1]);
    }

    // Run all tests
    void run_all() {
        test_attach_failure_invalid_fd();
        test_attach_success_valid_fd();
    }
} // namespace TestHarness

int main() {
    // Run the suite
    TestHarness::run_all();

    // Print a final summary
    TestHarness::print_summary("gzofstream::attach coverage");

    // Return non-zero if there were failures to aid CI tooling.
    return TestHarness::g_failure_count;
}