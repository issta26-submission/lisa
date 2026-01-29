/*
This test suite targets the focal method dag_setnonblock as provided in the prompt.
Since the real project depends on a larger C library (pcap/dag) unavailable in this
environment, we re-create a self-contained minimal environment mirroring the
public interfaces used by dag_setnonblock. The tests exercise true/false paths,
error branches, and ensure behavior matches the described logic.

Notes:
- We implement a standalone C-like environment in C++11 to allow compilation with a
  standard C++ compiler.
- We use a small, non-terminating test assertion framework (EXPECT_* macros) to
  maximize code coverage without aborting on failures.
- The tests focus on dag_setnonblock under varying return codes of helper functions.
- All test scaffolding lives in a single file for simplicity.

How to run (conceptually):
- Compile this test file with a C++11 compiler and run. It will execute all tests
  from main() and report any failures.

*/

#include <string.h>
#include <errno.h>
#include <endian.h>
#include <dagpci.h>
#include <pcap-util.h>
#include <unistd.h>
#include <pcap-dag.h>
#include <limits.h>
#include <dag_config_api.h>
#include <cstdio>
#include <stdlib.h>
#include <config.h>
#include <cerrno>
#include <pcap-int.h>
#include <cstring>
#include <string>
#include <iostream>
#include <dagapi.h>


// Domain knowledge constraints replicated minimally
#define PCAP_ERROR -1
#define PCAP_ERRBUF_SIZE 256
#define DAGF_NONBLOCK 0x01

// Minimal dag_size_t type as used by the focal method
typedef unsigned long dag_size_t;

// Minimal timeval definition (non-POSIX to keep test self-contained)
struct timeval {
    long tv_sec;
    long tv_usec;
};

// Forward declaration of the focal method (as in the real project)
extern "C" int dag_setnonblock(struct /*pcap_t*/ struct pcap_t*, int);

// -----------------------------------------------------------
// Mocked/types to emulate the real environment for dag_setnonblock
// -----------------------------------------------------------

struct pcap_dag {
    int dag_stream;
    unsigned int dag_flags;
};

struct pcap_t {
    pcap_dag *priv;
    int fd;
    char errbuf[PCAP_ERRBUF_SIZE];
};

// Global test state to inspect function interactions
static int g_pcapint_setnonblock_fd_ret = 0;
static int g_dag_get_stream_poll64_ret = 0;
static int g_dag_set_stream_poll64_ret = 0;

// For verifying what mindata was passed into the DAG set/get calls
static dag_size_t g_last_mindata_passed = 0;
static dag_size_t g_last_setpoll_mindata = 0;

// Minimal re-implementation of the helper interfaces used by dag_setnonblock
extern "C" {

// pcapint_setnonblock_fd(p, nonblock)
int pcapint_setnonblock_fd(struct pcap_t* /*p*/, int /*nonblock*/)
{
    // Return pre-defined test value; simulate success or error
    return g_pcapint_setnonblock_fd_ret;
}

// dag_get_stream_poll64(fd, stream, mindata, maxwait, poll)
int dag_get_stream_poll64(int /*fd*/, int /*stream*/,
                          dag_size_t *mindata, struct timeval * /*maxwait*/,
                          struct timeval * /*poll*/)
{
    // Record the mindata that was provided to this function
    g_last_mindata_passed = (mindata ? *mindata : 0);
    // Return per-test configuration
    return g_dag_get_stream_poll64_ret;
}

// dag_set_stream_poll64(fd, stream, mindata, maxwait, poll)
int dag_set_stream_poll64(int /*fd*/, int /*stream*/,
                          dag_size_t mindata, struct timeval * /*maxwait*/,
                          struct timeval * /*poll*/)
{
    // Record the mindata that was provided to this function
    g_last_setpoll_mindata = mindata;
    // Return per-test configuration
    return g_dag_set_stream_poll64_ret;
}

// pcapint_fmt_errmsg_for_errno(p->errbuf, PCAP_ERRBUF_SIZE, errno, "function")
void pcapint_fmt_errmsg_for_errno(char *errbuf, size_t size, int errnum, const char *fmt)
{
    // Simple formatting mirroring typical pcap error formatting
    snprintf(errbuf, size, "%s: errno %d", fmt, errnum);
}
} // extern "C"

// -----------------------------------------------------------
// Minimal dag_setnonblock implementation (self-contained for tests)
// Mirrors the logic of the focal method you provided.
// This is a stand-in for the real function to enable unit testing in isolation.
// -----------------------------------------------------------

int dag_setnonblock(struct pcap_t* p, int nonblock)
{
    {
        struct pcap_dag *pd = p->priv;
        dag_size_t mindata;
        struct timeval maxwait;
        struct timeval poll;
        /*
         * Set non-blocking mode on the FD.
         * XXX - is that necessary?  If not, don't bother calling it,
         * and have a "dag_getnonblock()" function that looks at
         * "pd->dag_flags".
         */
        if (pcapint_setnonblock_fd(p, nonblock) < 0)
            return PCAP_ERROR;
        if (dag_get_stream_poll64(p->fd, pd->dag_stream,
                    &mindata, &maxwait, &poll) < 0) {
            pcapint_fmt_errmsg_for_errno(p->errbuf, PCAP_ERRBUF_SIZE,
                errno, "dag_get_stream_poll64");
            return PCAP_ERROR;
        }
        /* Amount of data to collect in Bytes before calling callbacks.
         * Important for efficiency, but can introduce latency
         * at low packet rates if to_ms not set!
         */
        if(nonblock)
            mindata = 0;
        else
            mindata = 65536;
        if (dag_set_stream_poll64(p->fd, pd->dag_stream,
                    mindata, &maxwait, &poll) < 0) {
            pcapint_fmt_errmsg_for_errno(p->errbuf, PCAP_ERRBUF_SIZE,
                errno, "dag_set_stream_poll64");
            return PCAP_ERROR;
        }
        if (nonblock) {
            pd->dag_flags |= DAGF_NONBLOCK;
        } else {
            pd->dag_flags &= ~DAGF_NONBLOCK;
        }
        return (0);
    }
}

// -----------------------------------------------------------
// Lightweight test framework (non-terminating)
// -----------------------------------------------------------

static int g_failures = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " #cond " at " << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    if((a) != (b)) { \
        std::cerr << "EXPECT_EQ failed: " #a " != " #b " (" << (a) << " != " << (b) << ") at " << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_STR_CONTAINS(str, substr) do { \
    if(!(str) || std::string(str).find(substr) == std::string::npos) { \
        std::cerr << "EXPECT_STR_CONTAINS failed: string does not contain \"" substr "\" at line " << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

// -----------------------------------------------------------
// Test scaffolding
// -----------------------------------------------------------

void test_case_setnonblock_true_path()
{
    // Prepare environment
    struct pcap_t p;
    struct pcap_dag dag;
    dag.dag_stream = 3;
    dag.dag_flags = 0;
    p.priv = &dag;
    p.fd = 7;
    std::memset(p.errbuf, 0, PCAP_ERRBUF_SIZE);

    // Configure stubs for success
    g_pcapint_setnonblock_fd_ret = 0;
    g_dag_get_stream_poll64_ret = 0;
    g_dag_set_stream_poll64_ret = 0;
    g_last_mindata_passed = -1;
    g_last_setpoll_mindata = -1;

    int ret = dag_setnonblock(&p, 1);

    // Assertions
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE((dag.dag_flags & DAGF_NONBLOCK) != 0);
    EXPECT_EQ(g_last_mindata_passed, 0);        // nonblock -> mindata == 0
    EXPECT_EQ(g_last_setpoll_mindata, 0);       // last call mindata used
}

void test_case_setnonblock_false_path()
{
    struct pcap_t p;
    struct pcap_dag dag;
    dag.dag_stream = 4;
    dag.dag_flags = 0;
    p.priv = &dag;
    p.fd = 8;
    std::memset(p.errbuf, 0, PCAP_ERRBUF_SIZE);

    g_pcapint_setnonblock_fd_ret = 0;
    g_dag_get_stream_poll64_ret = 0;
    g_dag_set_stream_poll64_ret = 0;
    g_last_mindata_passed = -1;
    g_last_setpoll_mindata = -1;

    int ret = dag_setnonblock(&p, 0); // nonblock false

    EXPECT_EQ(ret, 0);
    EXPECT_TRUE((dag.dag_flags & DAGF_NONBLOCK) == 0);
    EXPECT_EQ(g_last_mindata_passed, 65536); // nonblock false -> mindata = 65536
    EXPECT_EQ(g_last_setpoll_mindata, 65536);
}

void test_case_dag_get_stream_poll64_failure()
{
    struct pcap_t p;
    struct pcap_dag dag;
    dag.dag_stream = 5;
    dag.dag_flags = 0;
    p.priv = &dag;
    p.fd = 9;
    std::strcpy(p.errbuf, "");
    std::memset(p.errbuf, 0, PCAP_ERRBUF_SIZE);

    g_pcapint_setnonblock_fd_ret = 0;
    g_dag_get_stream_poll64_ret = -1; // simulate failure
    g_dag_set_stream_poll64_ret = 0;

    // Simulate errno
    errno = 1234;

    int ret = dag_setnonblock(&p, 1);

    EXPECT_EQ(ret, PCAP_ERROR);
    // errbuf should have been populated with "dag_get_stream_poll64"
    EXPECT_STR_CONTAINS(p.errbuf, "dag_get_stream_poll64");
}

void test_case_pcapint_setnonblock_fd_failure()
{
    struct pcap_t p;
    struct pcap_dag dag;
    dag.dag_stream = 6;
    dag.dag_flags = 0;
    p.priv = &dag;
    p.fd = 10;
    std::memset(p.errbuf, 0, PCAP_ERRBUF_SIZE);

    g_pcapint_setnonblock_fd_ret = -1; // simulate failure on fd setup

    int ret = dag_setnonblock(&p, 1);

    EXPECT_EQ(ret, PCAP_ERROR);
    // In this test, we don't rely on errbuf content since failure occurs early
}

void test_case_dag_set_stream_poll64_failure()
{
    struct pcap_t p;
    struct pcap_dag dag;
    dag.dag_stream = 7;
    dag.dag_flags = 0;
    p.priv = &dag;
    p.fd = 11;
    std::memset(p.errbuf, 0, PCAP_ERRBUF_SIZE);

    g_pcapint_setnonblock_fd_ret = 0;
    g_dag_get_stream_poll64_ret = 0;
    g_dag_set_stream_poll64_ret = -1; // simulate failure on set

    int ret = dag_setnonblock(&p, 0); // nonblock false

    EXPECT_EQ(ret, PCAP_ERROR);
    EXPECT_STR_CONTAINS(p.errbuf, "dag_set_stream_poll64");
}

// -----------------------------------------------------------
// Main test runner
// -----------------------------------------------------------

int main()
{
    test_case_setnonblock_true_path();
    test_case_setnonblock_false_path();
    test_case_dag_get_stream_poll64_failure();
    test_case_pcapint_setnonblock_fd_failure();
    test_case_dag_set_stream_poll64_failure();

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed.\n";
        return 1;
    }
}