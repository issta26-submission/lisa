/* File: test_dlbindreq_rawdls.cpp
 * Purpose: Unit test for dlbindreq(fd, sap, ebuf) exercising the DL_HP_RAWDLS branch.
 * Approach:
 *  - Provide a C-style mock for send_request to capture the generated DLPI bind request.
 *  - Compile this test together with pcap-dlpi.c and define DL_HP_RAWDLS to trigger RAWDLS path.
 *  - Verify that the produced request contains:
 *      - dl_primitive == DL_BIND_REQ
 *      - dl_sap == sap argument
 *      - dl_service_mode == DL_HP_RAWDLS
 *      - what == "bind"
 *      - len == sizeof(dl_bind_req_t)
 *      - ebuf is forwarded correctly
 *  - Use a lightweight, non-terminating assertion mechanism.
 */

#include <sys/dlpi_ext.h>
#include <unistd.h>
#include <stdio.h>
#include <stropts.h>
#include <sys/types.h>
#include <errno.h>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/systeminfo.h>
#include <limits.h>
#include <sys/dlpi.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <dlpisubs.h>
#include <string.h>
#include <memory.h>
#include <sys/time.h>
#include <os-proto.h>
#include <sys/bufmod.h>
#include <config.h>
#include <cstring>
#include <sys/stream.h>


// Declaration of the function under test (from pcap-dlpi.c)
extern "C" int dlbindreq(int fd, unsigned int sap, char *ebuf);

// Globals to capture information from the mock send_request
static unsigned int g_last_primitive = 0;
static unsigned int g_last_sap = 0;
static unsigned int g_last_service_mode = 0;
static int          g_last_len = 0;
static char         g_last_what[64] = "";
static char         g_last_ebuf[256] = "";

// Lightweight non-terminating assertion helpers
static int g_failures = 0;

#define EXPECT_EQ(msg, a, b) \
    do { if ((a) != (b)) { fprintf(stderr, "EXPECT_EQ FAILED: %s | expected %u, got %u\n", (msg), (unsigned)(b), (unsigned)(a)); ++g_failures; } } while (0)

#define EXPECT_STR_EQ(msg, a, b) \
    do { if (strcmp((a), (b)) != 0) { fprintf(stderr, "EXPECT_STR_EQ FAILED: %s | expected \"%s\", got \"%s\"\n", (msg), (b), (a)); ++g_failures; } } while (0)

#define EXPECT_NOT_NULL(msg, p) \
    do { if ((p) == NULL) { fprintf(stderr, "EXPECT_NOT_NULL FAILED: %s | pointer is NULL\n", (msg)); ++g_failures; } } while (0)


// Mock for the cross-language C function used by dlbindreq
extern "C" int send_request(int fd, char *ptr, int len, char *what, char *ebuf) {
    // Capture the buffer content for assertions
    if (ptr != NULL) {
        const dl_bind_req_t* req = (const dl_bind_req_t*)ptr;
        g_last_primitive = req->dl_primitive;
        g_last_sap = req->dl_sap;
        g_last_service_mode = req->dl_service_mode;
        g_last_len = len;
    } else {
        g_last_len = len;
    }

    if (what != NULL) {
        strncpy(g_last_what, what, sizeof(g_last_what) - 1);
        g_last_what[sizeof(g_last_what) - 1] = '\0';
    } else {
        g_last_what[0] = '\0';
    }

    if (ebuf != NULL) {
        strncpy(g_last_ebuf, ebuf, sizeof(g_last_ebuf) - 1);
        g_last_ebuf[sizeof(g_last_ebuf) - 1] = '\0';
    } else {
        g_last_ebuf[0] = '\0';
    }

    // Simulate success
    return 0;
}

int main() {
    // Prepare test parameters
    const int fd = 7;
    const unsigned int sap = 0x1234;
    const char ebuf[] = "OK";

    // Initialize expected outputs (optional)
    g_last_primitive = 0;
    g_last_sap = 0;
    g_last_service_mode = 0;
    g_last_len = 0;
    g_last_what[0] = '\0';
    g_last_ebuf[0] = '\0';

    // Call the function under test
    int ret = dlbindreq(fd, sap, (char*)ebuf);
    (void)ret; // we only check behavior via the mock

    // Assertions
    EXPECT_EQ("dl_primitive must be DL_BIND_REQ", g_last_primitive, (unsigned int)DL_BIND_REQ);
    EXPECT_EQ("dl_sap must match sap argument", g_last_sap, sap);
    EXPECT_EQ("dl_service_mode must be DL_HP_RAWDLS in RAWDLS build", g_last_service_mode, (unsigned int)DL_HP_RAWDLS);
    EXPECT_STR_EQ("what string must be 'bind'", g_last_what, "bind");
    EXPECT_STR_EQ("ebuf string must propagate", g_last_ebuf, ebuf);
    EXPECT_EQ("payload length must be sizeof(dl_bind_req_t)", g_last_len, (int)sizeof(dl_bind_req_t));

    if (g_failures == 0) {
        printf("ODD-RAW test: All checks passed.\n");
        return 0;
    } else {
        fprintf(stderr, "ODD-RAW test: %d failure(s) detected.\n", g_failures);
        return 1;
    }
}

/* End of test_dlbindreq_rawdls.cpp */


/* File: test_dlbindreq_cldls.cpp
 * Purpose: Unit test for dlbindreq(fd, sap, ebuf) exercising the DL_CLDLS branch.
 * Approach:
 *  - Provide a C-style mock for send_request to capture the generated DLPI bind request.
 *  - Compile this test together with pcap-dlpi.c and define DL_CLDLS to trigger CLDLS path.
 *  - Verify that the produced request contains the same fields as RAWDLS test, except
 *    service_mode should be DL_CLDLS.
 */


// Declaration of the function under test (from pcap-dlpi.c)
extern "C" int dlbindreq(int fd, unsigned int sap, char *ebuf);

// Globals to capture information from the mock send_request
static unsigned int g_last_primitive = 0;
static unsigned int g_last_sap = 0;
static unsigned int g_last_service_mode = 0;
static int          g_last_len = 0;
static char         g_last_what[64] = "";
static char         g_last_ebuf[256] = "";

// Lightweight non-terminating assertion helpers
static int g_failures = 0;

#define EXPECT_EQ(msg, a, b) \
    do { if ((a) != (b)) { fprintf(stderr, "EXPECT_EQ FAILED: %s | expected %u, got %u\n", (msg), (unsigned)(b), (unsigned)(a)); ++g_failures; } } while (0)

#define EXPECT_STR_EQ(msg, a, b) \
    do { if (strcmp((a), (b)) != 0) { fprintf(stderr, "EXPECT_STR_EQ FAILED: %s | expected \"%s\", got \"%s\"\n", (msg), (b), (a)); ++g_failures; } } while (0)

#define EXPECT_NOT_NULL(msg, p) \
    do { if ((p) == NULL) { fprintf(stderr, "EXPECT_NOT_NULL FAILED: %s | pointer is NULL\n", (msg)); ++g_failures; } } while (0)


// Mock for the cross-language C function used by dlbindreq
extern "C" int send_request(int fd, char *ptr, int len, char *what, char *ebuf) {
    if (ptr != NULL) {
        const dl_bind_req_t* req = (const dl_bind_req_t*)ptr;
        g_last_primitive = req->dl_primitive;
        g_last_sap = req->dl_sap;
        g_last_service_mode = req->dl_service_mode;
        g_last_len = len;
    } else {
        g_last_len = len;
    }

    if (what != NULL) {
        strncpy(g_last_what, what, sizeof(g_last_what) - 1);
        g_last_what[sizeof(g_last_what) - 1] = '\0';
    } else {
        g_last_what[0] = '\0';
    }

    if (ebuf != NULL) {
        strncpy(g_last_ebuf, ebuf, sizeof(g_last_ebuf) - 1);
        g_last_ebuf[sizeof(g_last_ebuf) - 1] = '\0';
    } else {
        g_last_ebuf[0] = '\0';
    }

    return 0;
}

int main() {
    // Setup
    const int fd = 9;
    const unsigned int sap = 0x0ABC;
    const char ebuf[] = "OK_CLDLS";

    // Reset captured data
    g_last_primitive = 0;
    g_last_sap = 0;
    g_last_service_mode = 0;
    g_last_len = 0;
    g_last_what[0] = '\0';
    g_last_ebuf[0] = '\0';

    int ret = dlbindreq(fd, sap, (char*)ebuf);
    (void)ret;

    // Assertions
    EXPECT_EQ("dl_primitive must be DL_BIND_REQ", g_last_primitive, (unsigned int)DL_BIND_REQ);
    EXPECT_EQ("dl_sap must match sap argument", g_last_sap, sap);
    EXPECT_EQ("dl_service_mode must be DL_CLDLS in CLDLS build", g_last_service_mode, (unsigned int)DL_CLDLS);
    EXPECT_STR_EQ("what string must be 'bind'", g_last_what, "bind");
    EXPECT_STR_EQ("ebuf string must propagate", g_last_ebuf, ebuf);
    EXPECT_EQ("payload length must be sizeof(dl_bind_req_t)", g_last_len, (int)sizeof(dl_bind_req_t));

    if (g_failures == 0) {
        printf("ODD-CLDLS test: All checks passed.\n");
        return 0;
    } else {
        fprintf(stderr, "ODD-CLDLS test: %d failure(s) detected.\n", g_failures);
        return 1;
    }
}

/* End of test_dlbindreq_cldls.cpp */


/* File: test_dlbindreq_nopath.cpp
 * Purpose: Unit test for dlbindreq(fd, sap, ebuf) when neither DL_HP_RAWDLS nor DL_CLDLS is defined.
 * Approach:
 *  - Do not define DL_HP_RAWDLS or DL_CLDLS to exercise the default path.
 *  - Validate that service_mode is left at 0 (as the code memset to 0 and only sets mode in defined branches).
 *  - Validate other fields as in previous tests.
 */


// Declaration of the function under test (from pcap-dlpi.c)
extern "C" int dlbindreq(int fd, unsigned int sap, char *ebuf);

// Globals to capture information from the mock send_request
static unsigned int g_last_primitive = 0;
static unsigned int g_last_sap = 0;
static unsigned int g_last_service_mode = 0;
static int          g_last_len = 0;
static char         g_last_what[64] = "";
static char         g_last_ebuf[256] = "";

// Lightweight non-terminating assertion helpers
static int g_failures = 0;

#define EXPECT_EQ(msg, a, b) \
    do { if ((a) != (b)) { fprintf(stderr, "EXPECT_EQ FAILED: %s | expected %u, got %u\n", (msg), (unsigned)(b), (unsigned)(a)); ++g_failures; } } while (0)

#define EXPECT_STR_EQ(msg, a, b) \
    do { if (strcmp((a), (b)) != 0) { fprintf(stderr, "EXPECT_STR_EQ FAILED: %s | expected \"%s\", got \"%s\"\n", (msg), (b), (a)); ++g_failures; } } while (0)


// Mock for the cross-language C function used by dlbindreq
extern "C" int send_request(int fd, char *ptr, int len, char *what, char *ebuf) {
    if (ptr != NULL) {
        const dl_bind_req_t* req = (const dl_bind_req_t*)ptr;
        g_last_primitive = req->dl_primitive;
        g_last_sap = req->dl_sap;
        g_last_service_mode = req->dl_service_mode;
        g_last_len = len;
    } else {
        g_last_len = len;
    }

    if (what != NULL) {
        strncpy(g_last_what, what, sizeof(g_last_what) - 1);
        g_last_what[sizeof(g_last_what) - 1] = '\0';
    } else {
        g_last_what[0] = '\0';
    }

    if (ebuf != NULL) {
        strncpy(g_last_ebuf, ebuf, sizeof(g_last_ebuf) - 1);
        g_last_ebuf[sizeof(g_last_ebuf) - 1] = '\0';
    } else {
        g_last_ebuf[0] = '\0';
    }

    return 0;
}

int main() {
    // Setup
    const int fd = 11;
    const unsigned int sap = 0x0DEF;
    const char ebuf[] = "OK-NOPATH";

    // Reset captured data
    g_last_primitive = 0;
    g_last_sap = 0;
    g_last_service_mode = 0;
    g_last_len = 0;
    g_last_what[0] = '\0';
    g_last_ebuf[0] = '\0';

    int ret = dlbindreq(fd, sap, (char*)ebuf);
    (void)ret;

    // Assertions
    EXPECT_EQ("dl_primitive must be DL_BIND_REQ", g_last_primitive, (unsigned int)DL_BIND_REQ);
    EXPECT_EQ("dl_sap must match sap argument", g_last_sap, sap);
    EXPECT_EQ("service_mode should remain 0 in none path", g_last_service_mode, 0u);
    EXPECT_STR_EQ("what string must be 'bind'", g_last_what, "bind");
    EXPECT_STR_EQ("ebuf string must propagate", g_last_ebuf, ebuf);
    EXPECT_EQ("payload length must be sizeof(dl_bind_req_t)", g_last_len, (int)sizeof(dl_bind_req_t));

    if (g_failures == 0) {
        printf("ODD-NOPATH test: All checks passed.\n");
        return 0;
    } else {
        fprintf(stderr, "ODD-NOPATH test: %d failure(s) detected.\n", g_failures);
        return 1;
    }
}

/* End of test_dlbindreq_nopath.cpp */