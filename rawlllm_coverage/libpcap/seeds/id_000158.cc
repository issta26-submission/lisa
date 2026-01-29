#include <pcap/bluetooth.h>
#include <pcap/can_socketcan.h>
#include <pcap/compiler-tests.h>
#include <pcap/dlt.h>
#include <pcap/funcattrs.h>
#include <pcap/ipnet.h>
#include <pcap/nflog.h>
#include <pcap/pcap-inttypes.h>
#include <pcap/pcap.h>
#include <pcap/sll.h>
#include <pcap/socket.h>
#include <pcap/usb.h>
#include <pcap/vlan.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 158
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
#if __has_include(<pcap/pcap.h>)
# include <pcap/pcap.h>
#elif __has_include(<pcap.h>)
# include <pcap.h>
#else
# error "pcap header not found"
#endif


// Simple packet handler used with pcap_dispatch/pcap_loop.
// It increments an integer counter passed via the user argument.
static void packet_handler(u_char* user, const struct pcap_pkthdr* h, const u_char* bytes) {
    if (!user) return;
    int* counter = reinterpret_cast<int*>(user);
    ++(*counter);
    // Do not attempt to access bytes beyond this demo.
}

int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];
    std::memset(errbuf, 0, sizeof(errbuf));

    pcap_if_t* alldevs = nullptr;
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // pcap_findalldevs failed (e.g., no privileges). Clean up and return an error.
        // errbuf contains a human-readable error; we won't print it here, but free resources if any.
        if (alldevs) pcap_freealldevs(alldevs);
        return -1;
    }

    // Choose the first device if available.
    pcap_if_t* dev = alldevs;
    pcap_t* handle = nullptr;
    bool used_live_handle = false;

    if (dev != nullptr) {
        // Try creating with the newer pcap_create / pcap_activate sequence first to demonstrate that API.
        pcap_t* created = pcap_create(dev->name, errbuf);
        if (created != nullptr) {
            // Configure parameters then activate.
            pcap_set_snaplen(created, 65535);
            pcap_set_promisc(created, 1);
            pcap_set_timeout(created, 1000); // milliseconds
            // Optionally set immediate mode if available. Ignore errors for these optional settings.
#if defined(PCAP_SET_IMMEDIATE_MODE)
            pcap_set_immediate_mode(created, 1);
#endif
            int acode = pcap_activate(created);
            if (acode == 0) {
                // Activated successfully. Use this handle for subsequent actions.
                handle = created;
                used_live_handle = true;
            } else {
                // Activation failed; close and fall back to pcap_open_live.
                const char* aerr = pcap_statustostr ? pcap_statustostr(acode) : nullptr;
                (void)aerr; // silence unused variable warnings if not used
                pcap_close(created);
                created = nullptr;
            }
        }

        // If pcap_create/activate failed, try pcap_open_live as a fallback.
        if (!handle) {
            std::memset(errbuf, 0, sizeof(errbuf));
            handle = pcap_open_live(dev->name, 65535, 1, 1000, errbuf);
            if (!handle) {
                // open_live failed; clean up and continue to try other operations (e.g., compile with open_dead).
                // We'll not return immediately; we will still demonstrate compilation via open_dead below.
            } else {
                used_live_handle = true;
            }
        }
    }

    // Demonstrate compiling a filter. If we have a live handle, use it. Otherwise use pcap_open_dead.
    pcap_t* compiler_handle = nullptr;
    bool compiler_handle_is_dead = false;
    if (handle) {
        compiler_handle = handle;
    } else {
        // No live handle available; use pcap_open_dead for compilation (DLT_EN10MB typical).
        compiler_handle = pcap_open_dead(DLT_EN10MB, 65535);
        if (!compiler_handle) {
            if (alldevs) pcap_freealldevs(alldevs);
            return -2;
        }
        compiler_handle_is_dead = true;
    }

    struct bpf_program fp;
    const char* filter_exp = "tcp and port 80";
    std::memset(&fp, 0, sizeof(fp));
    bpf_u_int32 netmask = PCAP_NETMASK_UNKNOWN;

    if (pcap_compile(compiler_handle, &fp, filter_exp, 1, netmask) == -1) {
        // Compilation failed.
        if (compiler_handle_is_dead) pcap_close(compiler_handle);
        if (handle && (!compiler_handle_is_dead)) { /* handle still managed below */ }
        if (alldevs) pcap_freealldevs(alldevs);
        return -3;
    }

    // If we have a real live handle, install the filter on it.
    if (handle && !compiler_handle_is_dead) {
        if (pcap_setfilter(handle, &fp) == -1) {
            pcap_freecode(&fp);
            if (compiler_handle_is_dead) pcap_close(compiler_handle);
            if (alldevs) pcap_freealldevs(alldevs);
            return -4;
        }
    } else {
        // If we compiled with open_dead, we can still free the code after use.
    }

    // Free compiled filter program when no longer needed.
    pcap_freecode(&fp);

    // Optionally toggle non-blocking mode on the live handle to show other APIs.
    if (handle) {
        std::memset(errbuf, 0, sizeof(errbuf));
        // pcap_setnonblock returns 0 on success, PCAP_ERROR or PCAP_WARNING on failure.
        int nb = pcap_setnonblock(handle, 1, errbuf);
        (void)nb; // ignore the return code here for the demo
    }

    // Demonstrate retrieving data-link type if we have a live handle.
    if (handle) {
        int dlt = pcap_datalink(handle);
        (void)dlt; // used to demonstrate the call; no further action required
    }

    // Demonstrate statistics (may not be supported on all platforms).
    if (handle) {
        struct pcap_stat stats;
        std::memset(&stats, 0, sizeof(stats));
        if (pcap_stats(handle, &stats) == 0) {
            // stats.ps_recv, stats.ps_drop, etc. available on success.
            (void)stats;
        } else {
            // pcap_stats not supported or failed; continue without error.
        }
    }

    // Demonstrate packet capture dispatch with a callback.
    int packet_count = 0;
    if (handle) {
        // Process up to 10 packets (if any); this will return quickly if no packets are available.
        int dispatched = pcap_dispatch(handle, 10, packet_handler, reinterpret_cast<u_char*>(&packet_count));
        if (dispatched < 0) {
            // Error during dispatch; retrieve error if available, but continue with cleanup.
            const char* perr = pcap_geterr(handle);
            (void)perr;
        }
    }

    // Demonstrate breaking a loop (no-op here since we didn't start pcap_loop that runs indefinitely).
    // pcap_breakloop(handle); // would be used inside another thread or a callback.

    // Close handles and free resources.
    if (handle) {
        pcap_close(handle);
        handle = nullptr;
    }
    if (compiler_handle_is_dead && compiler_handle) {
        pcap_close(compiler_handle);
        compiler_handle = nullptr;
    }
    if (alldevs) {
        pcap_freealldevs(alldevs);
        alldevs = nullptr;
    }

    // If we reached here without returning error codes above, consider the sequence successful.
    return 66;
}