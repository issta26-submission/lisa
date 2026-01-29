// C++11 test harness for the focal method bpf_open in pcap-bpf.c
// This test suite uses a lightweight, non-GTest harness to exercise
// various code paths inside bpf_open by providing a custom open().
// The custom open() simulates OS-level responses to drive branching.
// Each test is designed to cover true/false branches and static behavior
// without terminating on first failure, allowing multiple tests to run
// and report results.
//
// Important: The real production code (pcap-bpf.c) is compiled separately
// and linked with this test. We override the system call 'open' to control
// the environment for bpf_open. All tests aim to be non-destructive and
// continue execution after failures (non-terminating assertions).

#include <cf.h>
#include <net/bpf.h>
#include <netdb.h>
#include <sys/sysconfig.h>
#include <unistd.h>
#include <stdio.h>
#include <zone.h>
#include <sys/types.h>
#include <iostream>
#include <sys/ioctl.h>
#include <errno.h>
#include <time.h>
#include <stddef.h>
#include <dirent.h>
#include <sys/param.h>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/cfgodm.h>
#include <cstdarg>
#include <net/if_types.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <string.h>
#include <sys/ioccom.h>
#include <sys/device.h>
#include <os-proto.h>
#include <config.h>
#include <sys/utsname.h>


// Forward declare the focal function from the production file.
extern "C" int bpf_open(char *errbuf);

// ------------------ Open() override scaffolding ------------------

// Test scenarios enumerator to drive the mocked open() behavior.
enum OpenScenario {
    OS_BASIC_SUCCESS,           // Test 1: first open succeeds ( /dev/bpf with O_RDWR )
    OS_ENOENT_RDONLY,             // Test 2: ENOENT on first open, O_RDONLY path succeeds
    OS_NO_DEVICES,                // Test 3: no BPF devices available
    OS_BUSY_PERM,                 // Test 4: busy devices then permission denied
    OS_EACCES_INIT                 // Test 5: immediate EACCES on initial open
};

// Global state used by the mocked open() to determine behavior per test.
static OpenScenario g_open_scenario = OS_BASIC_SUCCESS;

// Implementation helper for the mocked open().
static int open_impl(const char* path, int flags) {
    // Normalize path lookups inside tests
    if (g_open_scenario == OS_BASIC_SUCCESS) {
        // Test 1: /dev/bpf opened with O_RDWR returns a valid fd (simulated as 3)
        if (path && std::strcmp(path, "/dev/bpf") == 0 && flags == O_RDWR) {
            return 3; // pretend we acquired a valid BPF device fd
        }
        // Any other path/flag is treated as not found for test isolation
        errno = ENOENT;
        return -1;
    }

    if (g_open_scenario == OS_ENOENT_RDONLY) {
        // Test 2: First attempt ENOENT, second attempt O_RDONLY succeeds
        if (path && std::strcmp(path, "/dev/bpf") == 0 && flags == O_RDWR) {
            errno = ENOENT;
            return -1;
        }
        if (path && std::strcmp(path, "/dev/bpf") == 0 && flags == O_RDONLY) {
            errno = 0;
            return 7; // success value
        }
        errno = ENOENT;
        return -1;
    }

    if (g_open_scenario == OS_NO_DEVICES) {
        // Test 3: No /dev/bpfN devices exist; first attempted minor returns ENOENT
        if (path && std::strcmp(path, "/dev/bpf0") == 0 && flags == O_RDWR) {
            errno = ENOENT;
            return -1;
        }
        // Default fail for other paths
        errno = ENOENT;
        return -1;
    }

    if (g_open_scenario == OS_BUSY_PERM) {
        // Test 4: Sequence to reach permission denied after busy and perm events
        if (path && std::strcmp(path, "/dev/bpf") == 0 && flags == O_RDWR) {
            errno = ENOENT;
            return -1;
        }
        if (path && std::strcmp(path, "/dev/bpf0") == 0 && flags == O_RDWR) {
            errno = EBUSY;
            return -1;
        }
        if (path && std::strcmp(path, "/dev/bpf1") == 0 && flags == O_RDWR) {
            // First attempt to O_RDWR on /dev/bpf1 yields EACCES to trigger O_RDONLY path
            errno = EACCES;
            return -1;
        }
        if (path && std::strcmp(path, "/dev/bpf1") == 0 && flags == O_RDONLY) {
            errno = EACCES;
            return -1;
        }
        // Any other path defaults to a generic error
        errno = EACCES;
        return -1;
    }

    if (g_open_scenario == OS_EACCES_INIT) {
        // Test 5: Immediate EACCES on initial open
        if (path && std::strcmp(path, "/dev/bpf") == 0 && flags == O_RDWR) {
            errno = EACCES;
            return -1;
        }
        errno = ENOENT;
        return -1;
    }

    // Fallback: not handled by a specific test scenario
    errno = ENOENT;
    return -1;
}

// C-compatible variadic wrapper to override the standard open().
extern "C" int open(const char* path, int flags, ...) {
    // The production code never passes the mode for O_CREAT here, so variadic arg ignored.
    (void)path;
    (void)flags;
    // Forward to implementation (path and flags are carried in the parameters of this wrapper)
    // Note: the actual path/flags will be provided by the production code; the test harness
    // uses the global g_open_scenario to control behavior.
    return open_impl(path, flags);
}

// ------------------ Simple test harness (non-GTest) ------------------

static int g_test_failures = 0;

static void log_failure(const std::string& test_name, const std::string& reason) {
    std::cerr << "[FAIL] " << test_name << ": " << reason << std::endl;
    ++g_test_failures;
}

static void log_success(const std::string& test_name) {
    std::cout << "[PASS] " << test_name << std::endl;
}

// Helper macro-like inline to perform a test and automatically log outcome
#define RUN_TEST(name) bool name()

/*
Candidate keyword mapping to the method under test (bpf_open):
- "/dev/bpf" (cloning device)
- "/dev/bpfN" minors (no_cloning_bpf path)
- O_RDWR, O_RDONLY (open modes)
- ENOENT, EACCES, EBUSY (errno values)
- PCAP-like error paths: PCAP_ERROR_CAPTURE_NOTSUP, PCAP_ERROR_PERM_DENIED, PCAP_ERROR
- errbuf behavior (non-empty on certain errors)
- success path returns a positive fd
*/

// Test 1: Basic path where opening /dev/bpf with O_RDWR succeeds on the first try.
static bool test_bpf_open_basic_success() {
    // Describe the test
    // Expectation: bpf_open returns a positive fd and errbuf remains empty.
    g_open_scenario = OS_BASIC_SUCCESS;
    char errbuf[256];
    std::memset(errbuf, 0, sizeof(errbuf));

    int fd = bpf_open(errbuf);

    bool passed = (fd > 0) && (std::string(errbuf).empty());
    if (!passed) {
        if (fd <= 0) log_failure("test_bpf_open_basic_success", "expected a valid fd from bpf_open");
        if (std::strlen(errbuf) != 0) log_failure("test_bpf_open_basic_success", "unexpected error message in errbuf");
    } else {
        log_success("test_bpf_open_basic_success");
    }
    return passed;
}

// Test 2: First attempt ENOENT on /dev/bpf, then O_RDONLY path succeeds on second attempt.
static bool test_bpf_open_enoent_then_readonly_success() {
    // Expectation: First open to /dev/bpf with O_RDWR fails with ENOENT, second open with O_RDONLY succeeds (returns a valid fd).
    g_open_scenario = OS_ENOENT_RDONLY;
    char errbuf[256];
    std::memset(errbuf, 0, sizeof(errbuf));

    int fd = bpf_open(errbuf);

    bool passed = (fd > 0) && (std::string(errbuf).empty());
    if (!passed) {
        if (fd <= 0) log_failure("test_bpf_open_enoent_then_readonly_success", "expected non-negative fd after second open");
        if (std::string(errbuf).size() > 0) log_failure("test_bpf_open_enoent_then_readonly_success", "unexpected error message in errbuf");
    } else {
        log_success("test_bpf_open_enoent_then_readonly_success");
    }
    return passed;
}

// Test 3: No cloning devices (/dev/bpf0, /dev/bpf1, ...) exist; bpf_open should report not supported.
static bool test_bpf_open_no_devices() {
    // Expectation: bpf_open returns a negative value and errbuf contains a message about no BPF devices.
    g_open_scenario = OS_NO_DEVICES;
    char errbuf[256];
    std::memset(errbuf, 0, sizeof(errbuf));

    int fd = bpf_open(errbuf);

    bool passed = (fd < 0);
    if (!passed) {
        log_failure("test_bpf_open_no_devices", "expected failure when no BPF devices exist");
    } else {
        std::string msg(errbuf);
        if (msg.find("no BPF devices") == std::string::npos) {
            log_failure("test_bpf_open_no_devices", "errbuf does not mention no BPF devices as expected");
            passed = false;
        } else {
            log_success("test_bpf_open_no_devices");
        }
    }
    return passed;
}

// Test 4: Busy devices and then permission denied on a later minor; expect PCAP_ERROR_PERM_DENIED path.
static bool test_bpf_open_busy_then_perm_denied() {
    // Expectation: Final result is an error with a permission-denied message referencing the device.
    g_open_scenario = OS_BUSY_PERM;
    char errbuf[256];
    std::memset(errbuf, 0, sizeof(errbuf));

    int fd = bpf_open(errbuf);

    bool passed = (fd < 0);
    if (!passed) {
        log_failure("test_bpf_open_busy_then_perm_denied", "expected negative fd when permissions are denied");
    } else {
        std::string msg(errbuf);
        if (msg.find("root privileges") == std::string::npos &&
            msg.find("Permission") == std::string::npos) {
            log_failure("test_bpf_open_busy_then_perm_denied", "errbuf does not mention permission/root privileges");
            passed = false;
        } else {
            log_success("test_bpf_open_busy_then_perm_denied");
        }
    }
    return passed;
}

// Test 5: Immediate EACCES on initial /dev/bpf open; expect an error with a root-privileges message.
static bool test_bpf_open_initial_eacces() {
    // Expectation: Immediate EACCES leads to PCAP_ERROR_PERM_DENIED path and an error message about root privileges.
    g_open_scenario = OS_EACCES_INIT;
    char errbuf[256];
    std::memset(errbuf, 0, sizeof(errbuf));

    int fd = bpf_open(errbuf);

    bool passed = (fd < 0);
    if (!passed) {
        log_failure("test_bpf_open_initial_eacces", "expected negative fd on initial EACCES");
    } else {
        std::string msg(errbuf);
        if (msg.find("root privileges") == std::string::npos) {
            log_failure("test_bpf_open_initial_eacces", "errbuf missing root-privileges message");
            passed = false;
        } else {
            log_success("test_bpf_open_initial_eacces");
        }
    }
    return passed;
}

// ------------------ Main runner ------------------

int main() {
    std::cout << "Starting bpf_open unit tests (non-GTest harness)" << std::endl;

    // Run tests in sequence, keeping going on failures
    bool all_passed =
        test_bpf_open_basic_success() &
        test_bpf_open_enoent_then_readonly_success() &
        test_bpf_open_no_devices() &
        test_bpf_open_busy_then_perm_denied() &
        test_bpf_open_initial_eacces();

    if (g_test_failures > 0) {
        std::cerr << "Some tests failed: " << g_test_failures << " failure(s)." << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}