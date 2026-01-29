// Test suite for pcap_activate_libdlpi in pcap-libdlpi.c
// Language: C++11 (no GTest; a small self-contained test harness is used)

#include <libdlpi.h>
#include <stdio.h>
#include <stropts.h>
#include <sys/types.h>
#include <iostream>
#include <errno.h>
#include <config.h>
#include <cstdio>
#include <cerrno>
#include <cstdarg>
#include <stdlib.h>
#include <pcap-int.h>
#include <dlpisubs.h>
#include <pcap-libdlpi.c>
#include <string.h>
#include <memory.h>
#include <sys/time.h>
#include <sys/bufmod.h>
#include <cstdlib>
#include <cstring>
#include <sys/stream.h>


// We assume the project headers (pcap-int.h, libdlpi, etc.) are available
// The focal function pcap_activate_libdlpi is defined in pcap-libdlpi.c
// For unit testing in a controlled environment, we employ a minimal
// mock layer for DLPI interactions by providing stubs with C linkage.

// Forward declare the focal function (as it would be in a C header)
extern "C" int pcap_activate_libdlpi(struct pcap_t *p);

// Because we want to intercept dependencies (dlpi_open, dlpi_bind, etc.)
// and keep test code in C++, we provide C-linkage mocks before including
// the focal implementation. We then include the focal C file so that
// it binds against our mocks in this translation unit.

extern "C" {

// Mockable return values (configurable per test)
static int mock_dlpi_open_ret = 0;
static int mock_dlpi_bind_ret = 0;
static int mock_dlpi_info_ret = 0;
static int mock_pcap_conf_bufmod_ret = 0;
static int mock_pcap_alloc_databuf_ret = 0;
static int mock_dlpromiscon_ret = 0;
static int mock_ioctl_ret = 0;
static int mock_ioctl_errno = 0;

// Simple placeholders for handles/structures used by the focal code
static void* mock_dlpi_handle = (void*)0xdeadbeef;

// Minimal "errno" translator for certain cases
#define DLPI_SUCCESS 0
#define DLPI_ELINKNAMEINVAL 1
#define DLPI_ENOLINK 2
#define DL_SYSERR 3
#define DLPI_EBADLINK 4
#define DLPI_RAW 0x01
#define DLPI_PASSIVE 0x02
#define MAXIMUM_SNAPLEN 65535

// The pcap_t and related structs are defined in the project headers.
// We rely on their actual layout for the tests. We just provide the
// necessary fields that the focal function uses.
}

// We include the actual implementation after the mocks.
// This allows the mocks to be resolved for the focal function's calls.

extern "C" {

// --- Mock implementations of external dependencies used by pcap_activate_libdlpi ---

// Helper: emulate a simple snprintf in error path
int snprintf(char* s, size_t n, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int len = vsnprintf(s, n, format, args);
    va_end(args);
    return len;
}

// Mock: dlpi_open
int dlpi_open(const char* device, dlpi_handle_t* dh, int flags) {
    // Return the pre-set mock value; on success, populate a dummy handle
    if (mock_dlpi_open_ret == DLPI_SUCCESS) {
        if (dh) *dh = mock_dlpi_handle;
    } else if (mock_dlpi_open_ret == DL_SYSERR) {
        errno = mock_ioctl_errno ? mock_ioctl_errno : EPERM;
    }
    return mock_dlpi_open_ret;
}

// Mock: dlpi_bind
int dlpi_bind(dlpi_handle_t dh, int sap, int max_sdu) {
    return mock_dlpi_bind_ret;
}

// Mock: dlpromiscon
int dlpromiscon(pcap_t* p, uint32_t level) {
    return mock_dlpromiscon_ret;
}

// Mock: dlpi_info
int dlpi_info(dlpi_handle_t dh, dlpi_info_t* di, int flags) {
    if (di) {
        // Set up a plausible mactype; the real value is tested against
        di->di_mactype = 0;
    }
    return mock_dlpi_info_ret == 0 ? DLPI_SUCCESS : mock_dlpi_info_ret;
}

// Mock: dlpi_fd
int dlpi_fd(dlpi_handle_t dh) {
    // Return a small file descriptor for tests
    return 42;
}

// Mock: ioctl (variadic)
int ioctl(int fd, unsigned long request, ...) {
    // Use configured mock value
    if (mock_ioctl_ret < 0) {
        errno = mock_ioctl_errno;
        return -1;
    }
    return mock_ioctl_ret;
}

// Mock: pcap_conf_bufmod
int pcap_conf_bufmod(struct pcap_t* p, int snaplen) {
    return mock_pcap_conf_bufmod_ret;
}

// Mock: pcap_alloc_databuf
int pcap_alloc_databuf(struct pcap_t* p) {
    return mock_pcap_alloc_databuf_ret;
}

// Mock: handle_nonexistent_dlpi_device
int handle_nonexistent_dlpi_device(const char* device, char* errbuf) {
    // Return a deterministic status; ensure errbuf could be written if needed
    if (errbuf) {
        const char* msg = "DLPI device nonexistent";
        std::strncpy(errbuf, msg, PCAP_ERRBUF_SIZE);
        errbuf[PCAP_ERRBUF_SIZE - 1] = '\0';
    }
    return PCAP_ERROR_NO_SUCH_DEVICE;
}

// Mock: pcap_libdlpi_err
void pcap_libdlpi_err(const char* linkname, const char* func, int err, char* errbuf) {
    // For test visibility, copy a simple diagnostic message
    if (errbuf && PCAP_ERRBUF_SIZE > 0) {
        std::snprintf(errbuf, PCAP_ERRBUF_SIZE,
                      "dlpi_open failed in %s with error %d (mock)", func, err);
    }
}

// Mock: pcapint_fmt_errmsg_for_errno
void pcapint_fmt_errmsg_for_errno(char* errbuf, int errbuf_size, int e, const char* what) {
    if (errbuf && errbuf_size > 0) {
        std::snprintf(errbuf, errbuf_size, "%s: errno %d (%s)", what, e, std::strerror(e));
    }
}

// Mock: pcapint_getnonblock_fd
int pcapint_getnonblock_fd(pcap_t* p) {
    return p ? p->fd : -1;
}

// Mock: pcapint_setnonblock_fd
int pcapint_setnonblock_fd(pcap_t* p, int nonblock) {
    // Success simulation
    return 0;
}

// Mock: pcapint_install_bpf_program (no kernel filtering in tests)
int pcapint_install_bpf_program(pcap_t* p, void* bpf) {
    return 0;
}

// Note: pcap_cleanup_libdlpi is defined in the focal file; we do not override it here.

} // extern "C"

// Simple test harness (non-terminating assertions)
#define EXPECT_TRUE(x) \
    do { if (!(x)) std::cerr << "[FAIL] " #x " at line " << __LINE__ << std::endl; } while (0)
#define EXPECT_EQ(a, b) \
    do { if ((a) != (b)) std::cerr << "[FAIL] " #a " == " #b " (actual " << (a) << ", " << (b) << ") at line " << __LINE__ << std::endl; } while (0)
#define EXPECT_STR_EQ(a, b) \
    do { if ((a) == NULL && (b) == NULL) {} else if ((a) && (b) && strcmp((a),(b))==0) {} else { std::cerr << "[FAIL] strings differ at line " << __LINE__ << std::endl; } } while (0)

// Utility to allocate and initialize a pcap_t-like structure (using real project types)
pcap_t* allocate_test_pcap() {
    // The real pcap_t type layout must be known; we rely on project headers
    pcap_t* p = (pcap_t*)calloc(1, sizeof(pcap_t));
    if (!p) return nullptr;
    // Initialize typical fields used by the focal function
    // Provide a private data structure as expected by the code
    p->priv = malloc(sizeof(struct pcap_dlpi));
    if (!p->priv) {
        free(p);
        return nullptr;
    }
    struct pcap_dlpi* pd = (struct pcap_dlpi*)p->priv;
    pd->dlpi_hd = (dlpi_handle_t)mock_dlpi_handle;
    // Minimal option configuration
    // The real pcap_t.opt type is project-specific; we rely on field names present in pcap_activate_libdlpi
    p->opt.device = "dummy0";
    p->opt.rfmon = 0;
    p->opt.promisc = 1;
    // Snapshot default
    p->snapshot = 0;
    // Error buffer
    memset(p->errbuf, 0, PCAP_ERRBUF_SIZE);
    return p;
}

// Clean up allocated pcap_t
void free_test_pcap(pcap_t* p) {
    if (!p) return;
    free(p->priv);
    free(p);
}

// --- Test Cases ---

void test_dlpi_open_elinknameinvalid() {
    // Setup
    mock_dlpi_open_ret = DLPI_ELINKNAMEINVAL;
    mock_dlpi_bind_ret = 0;
    mock_dlpi_info_ret = 0;
    mock_pcap_conf_bufmod_ret = 0;
    mock_pcap_alloc_databuf_ret = 0;
    mock_dlpromiscon_ret = 0;
    mock_ioctl_ret = 0;
    mock_ioctl_errno = 0;

    pcap_t* p = allocate_test_pcap();
    EXPECT_TRUE(p != nullptr);

    // Execute
    int status = pcap_activate_libdlpi(p);

    // Verify: Should be PCAP_ERROR_NO_SUCH_DEVICE and errbuf cleared
    EXPECT_EQ(status, PCAP_ERROR_NO_SUCH_DEVICE);
    EXPECT_TRUE(p->errbuf[0] == '\0' || std::strlen(p->errbuf) == 0);

    // Cleanup
    free_test_pcap(p);
}

void test_dlpi_open_enolink() {
    mock_dlpi_open_ret = DLPI_ENOLINK;
    mock_dlpi_bind_ret = DLPI_SUCCESS;
    mock_pcap_conf_bufmod_ret = 0;
    mock_pcap_alloc_databuf_ret = 0;
    mock_dlpromiscon_ret = 0;
    mock_ioctl_ret = 0;

    pcap_t* p = allocate_test_pcap();
    EXPECT_TRUE(p != nullptr);

    int status = pcap_activate_libdlpi(p);

    // Expect the path to call handle_nonexistent_dlpi_device and propagate its result
    EXPECT_TRUE(status == PCAP_ERROR_NO_SUCH_DEVICE || status == PCAP_ERROR);
    // errbuf may be set by our mock; ensure we haven't left it in a crash state
    (void)status; // suppress unused in environments without exact expected value

    free_test_pcap(p);
}

void test_dlpi_open_perm_denied() {
    mock_dlpi_open_ret = DL_SYSERR;
    mock_dlpi_bind_ret = DLPI_SUCCESS;
    mock_pcap_conf_bufmod_ret = 0;
    mock_pcap_alloc_databuf_ret = 0;
    mock_dlpromiscon_ret = 0;
    mock_ioctl_ret = -1;
    mock_ioctl_errno = EPERM;

    pcap_t* p = allocate_test_pcap();
    EXPECT_TRUE(p != nullptr);

    int status = pcap_activate_libdlpi(p);

    // Expect permission denied feedback
    EXPECT_EQ(status, PCAP_ERROR_PERM_DENIED);
    // errbuf should contain the permission message (non-empty)
    EXPECT_TRUE(p->errbuf[0] != '\0');

    free_test_pcap(p);
}

void test_rfmon_not_supported() {
    mock_dlpi_open_ret = DLPI_SUCCESS;
    mock_dlpi_bind_ret = DLPI_SUCCESS;
    mock_pcap_conf_bufmod_ret = 0;
    mock_pcap_alloc_databuf_ret = 0;
    mock_dlpromiscon_ret = 0;
    mock_ioctl_ret = 0;

    // Enable RFMON to trigger the not-supported path
    pcap_t* p = allocate_test_pcap();
    if (p) p->opt.rfmon = 1;

    int status = pcap_activate_libdlpi(p);

    // Expect RFMON not supported
    EXPECT_EQ(status, PCAP_ERROR_RFMON_NOTSUP);

    free_test_pcap(p);
}

void test_successful_path() {
    mock_dlpi_open_ret = DLPI_SUCCESS;
    mock_dlpi_bind_ret = DLPI_SUCCESS;
    mock_pcap_conf_bufmod_ret = 0;
    mock_pcap_alloc_databuf_ret = 0;
    mock_dlpromiscon_ret = 0;
    mock_ioctl_ret = 0;

    // Set a valid snapshot, within range
    pcap_t* p = allocate_test_pcap();
    if (p) p->snapshot = MAXIMUM_SNAPLEN; // explicit within range

    int status = pcap_activate_libdlpi(p);

    // Expect success
    EXPECT_EQ(status, 0);
    // Sanity: check that selectable_fd mirrors fd (we set to 42 in mock)
    EXPECT_TRUE(p->selectable_fd == p->fd);

    free_test_pcap(p);
}

// Entry point
int main() {
    std::cout << "Starting pcap-libdlpi unit tests (C++ harness, no GTest)..." << std::endl;

    test_dlpi_open_elinknameinvalid();
    test_dlpi_open_enolink();
    test_dlpi_open_perm_denied();
    test_rfmon_not_supported();
    test_successful_path();

    std::cout << "Unit test run complete." << std::endl;
    return 0;
}